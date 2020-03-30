from codegen_shared import *

class AppCanTxFileGenerator(CanFileGenerator):
    def __init__(self, database, output_path, sender, function_prefix):
        super().__init__(database, output_path, sender)
        self.__cantx_msgs = \
            list(msg for msg in self._get_can_msgs() if sender in msg.senders)
        self.__database = database

        self._sender = sender
        self._function_prefix = function_prefix
        self._non_periodic_cantx_msgs = \
            list(msg for msg in self.__cantx_msgs if msg.cycle_time == 0)
        self._periodic_cantx_msgs = \
            list(msg for msg in self.__cantx_msgs if msg.cycle_time > 0)
        self._periodic_cantx_signals = \
            [CanSignal(signal.type_name, signal.snake_name, msg.snake_name)
             for msg in self._periodic_cantx_msgs for signal in msg.signals]

        # Initialize function objects so we can get its declaration and
        # definition when generating the source and header fie
        self.__init_functions(function_prefix)

    def __init_functions(self, function_prefix):
        function_params = \
            [("    void (*send_non_periodic_msg_%s)(struct CanMsgs_%s_t* payload),"
              % (msg.snake_name.upper(), msg.snake_name))
             for msg in self._non_periodic_cantx_msgs]
        init_senders = \
            [("    can_tx_interface->send_non_periodic_msg_%s = send_non_periodic_msg_%s;"
              % (msg.snake_name.upper(), msg.snake_name.upper()))
             for msg in self._non_periodic_cantx_msgs]

        self._Create = Function(
            'struct CanTxInterface* %s_Create(%s)'
                % (function_prefix, '\n' + '\n'.join(function_params)[:-1]),
            'Allocate and initialize a CAN TX interface',
            '''\
    static struct CanTxInterface can_tx_interfaces[MAX_NUM_OF_CANTX_INTERFACES];
    static size_t alloc_index = 0;

    shared_assert(alloc_index < MAX_NUM_OF_CANTX_INTERFACES);

    struct CanTxInterface* can_tx_interface = &can_tx_interfaces[alloc_index++];

    // TODO: Instead of clearing the table to 0's, use .dbc to fill in the
    // initial signal values
    memset(&can_tx_interface->periodic_can_tx_table, 0, sizeof(can_tx_interface->periodic_can_tx_table));\n\n'''
    + '\n'.join(init_senders) + '\n\n'
    + '''\
    return can_tx_interface;''')

        self._PeriodicTxSignalSetters = list(Function(
            'void %s_SetPeriodicSignal_%s(struct CanTxInterface* can_tx_interface, %s value)' % (
            function_prefix, signal.uppercase_name, signal.type_name),
            '',
            '''\
    can_tx_interface->periodic_can_tx_table.{msg_name}.{signal_name} = value;'''.format(
                msg_name=signal.msg_name_snakecase,
                signal_name=signal.snakecase_name)
        ) for signal in self._periodic_cantx_signals)

        self._PeriodicTxMsgPointerGetters = list(Function(
            'struct CanMsgs_%s_t* %s_GetPeriodicMsgPointer_%s(struct CanTxInterface* can_tx_interface)' % (
                msg.snake_name, function_prefix, msg.snake_name.upper()),
            '',
            '''\
    return &can_tx_interface->periodic_can_tx_table.{msg_name};'''.format(
                msg_name=msg.snake_name)
        ) for msg in self._periodic_cantx_msgs)

        self._SendNonPeriodicMsgs = list(Function(
        'void %s_SendNonPeriodicMsg_%s(struct CanTxInterface* can_tx_interface, struct CanMsgs_%s_t* payload)' % (
            function_prefix, msg.snake_name.upper(), msg.snake_name),
        '',
        '''\
    can_tx_interface->send_non_periodic_msg_%s(payload);''' % msg.snake_name.upper()
    ) for msg in self._non_periodic_cantx_msgs)

class AppCanTxHeaderFileGenerator(AppCanTxFileGenerator):
    def __init__(self, database, output_path, sender, function_prefix):
        super().__init__(database, output_path, sender, function_prefix)

    def generateHeader(self):
        self._write_output(self._generateHeader(
            'CAN TX Library for the application layer',
            self.__generateHeaderIncludes(),
            self.__generateForwardDeclarations(),
            self.__generateFunctionDeclarations()))

    def __generateHeaderIncludes(self):
        header_names = ['<stdint.h>',
                        '"auto_generated/App_CanMsgs.h"']
        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateForwardDeclarations(self):
        return ''

    def __generateFunctionDeclarations(self):
        function_declarations = []
        function_declarations.append(self._Create.declaration)
        function_declarations.append(
            '/** @brief Signal getters for periodic CAN TX messages */\n'
            + '\n'.join([func.declaration for func in self._PeriodicTxSignalSetters]))
        function_declarations.append(
            '/** @brief Getter for pointer to an entry in the periodic CAN TX message table */\n'
            + '\n'.join([func.declaration for func in self._PeriodicTxMsgPointerGetters]))
        function_declarations.append('/** @brief Send a non-periodic CAN TX message */\n'
            + '\n'.join([func.declaration for func in self._SendNonPeriodicMsgs]))
        return '\n\n'.join(function_declarations)

class AppCanTxSourceFileGenerator(AppCanTxFileGenerator):
    def __init__(self, database, output_path, sender, function_prefix):
        super().__init__(database, output_path, sender, function_prefix)
        self.__init_structs()

    def generateSource(self):
        self._write_output(self._generateSource(
            self.__generateHeaderIncludes(),
            self.__generateTypedefs(),
            self.__generateMacros(),
            self.__generateVariables(),
            self.__generateFunctionDefinitions(),
            self.__generatePrivateFunctionDefinitions(),
            self.__generatePrivateFunctionDeclarations()))

    def __init_structs(self):
        self.__PeriodicCanTxMsgs = Struct(
            'PeriodicCanTxMsgs',
            [StructMember('struct CanMsgs_%s_t' % msg.snake_name,
                          msg.snake_name,
                          '0')
             for msg in self._periodic_cantx_msgs],
            'Periodic CAN TX message')
        self.__CanTxInterface = Struct(
            'CanTxInterface',
            [StructMember('struct PeriodicCanTxMsgs',
                          'periodic_can_tx_table',
                          '0')] +
            [StructMember('void (*send_non_periodic_msg_%s)(struct CanMsgs_%s_t* payload)'
                            % (msg.snake_name.upper(), msg.snake_name),
                          '',
                          '0')
             for msg in self._non_periodic_cantx_msgs],
            'Can TX interface')

    def __generateHeaderIncludes(self):
        header_names = ['<string.h>',
                        '<stdlib.h>',
                        '"auto_generated/App_CanTx.h"',
                        '"App_SharedAssert.h"']
        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateTypedefs(self):
        typedefs = []
        typedefs.append(self.__PeriodicCanTxMsgs.declaration)
        typedefs.append(self.__CanTxInterface.declaration)
        return '\n\n'.join(typedefs)

    def __generateMacros(self):
        macros = []
        macros.append(Macro(
            'MAX_NUM_OF_CANTX_INTERFACES',
            ' 1',
            'Maximum number for CAN TX interfaces').declaration)
        return '\n\n'.join(macros)

    def __generateVariables(self):
        variables = []
        return '\n\n'.join(variables)

    def __generatePrivateFunctionDeclarations(self):
        func_declarations = []
        return '\n\n'.join(func_declarations)

    def __generatePrivateFunctionDefinitions(self):
        function_defs = []
        return '\n'.join(function_defs)

    def __generateFunctionDefinitions(self):
        function_defs = []
        function_defs.append(self._Create.definition)
        function_defs.extend(func.definition for func in self._PeriodicTxSignalSetters)
        function_defs.extend(func.definition for func in self._PeriodicTxMsgPointerGetters)
        function_defs.extend(func.definition for func in self._SendNonPeriodicMsgs)
        return '\n\n'.join(function_defs)

class IoCanTxFileGenerator(CanFileGenerator):
    def __init__(self, database, output_path, sender, function_prefix):
        super().__init__(database, output_path, sender)
        self.__cantx_msgs = list(msg for msg in self._get_can_msgs() if sender in msg.senders)
        self.__database = database

        self._sender = sender
        self._function_prefix = function_prefix
        self._non_periodic_cantx_msgs = list(msg for msg in self.__cantx_msgs if msg.cycle_time == 0)
        self._periodic_cantx_msgs = list(msg for msg in self.__cantx_msgs if msg.cycle_time > 0)
        self._periodic_cantx_signals = \
            [CanSignal(signal.type_name, signal.snake_name, msg.snake_name)
             for msg in self._periodic_cantx_msgs for signal in msg.signals]

        # Initialize function objects so we can get its declaration and
        # definition when generating the source and header fie
        self.__init_functions(function_prefix)

    def __init_functions(self, function_prefix):
        FunctionDef = '''\
    struct CanMsg tx_message; \n\n'''

        FunctionDef += '\n\n'.join(['''\
    // Is it time to transmit this particular CAN message?
    if ((current_ms % {period}) == 0)
    {{
        memset(&tx_message, 0, sizeof(tx_message));

        // Prepare CAN message header
        tx_message.std_id = {std_id};
        tx_message.dlc = {dlc};

        // Prepare CAN message payload (The packing function isn't thread-safe
        // so we must guard it)
        vPortEnterCritical();
        {msg_packing_function}(
            &tx_message.data[0],
            {msg_function_ptr_getter}(can_tx_interface),
            tx_message.dlc);
        vPortExitCritical();

        Io_SharedCan_TxMessageQueueSendtoBack(&tx_message);
    }}'''.format(msg_snake_name=msg.snake_name,
                 msg_packing_function='App_CanMsgs_%s_pack' % msg.snake_name,
                 msg_function_ptr_getter=
                    'App_CanTx_GetPeriodicMsgPointer_%s' % msg.snake_name.upper(),
                 std_id='CANMSGS_%s_FRAME_ID' % msg.snake_name.upper(),
                 dlc='CANMSGS_%s_LENGTH' % msg.snake_name.upper(),
                 period='CANMSGS_%s_CYCLE_TIME_MS' % msg.snake_name.upper())
                                    for msg in self._periodic_cantx_msgs])

        self._EnqueuePeriodicMsgs = Function('''\
void %s_EnqueuePeriodicMsgs(struct CanTxInterface* can_tx_interface, const uint32_t current_ms)''' % function_prefix,
            'Enqueue periodic CAN TX messages according to the cycle time specified in the DBC. This should be called in a 1kHz task.',
            FunctionDef)

        self._EnqueueNonPeriodicMsgs = list(Function(
            'void %s_EnqueueNonPeriodicMsg_%s(struct CanMsgs_%s_t* payload)'
            % (function_prefix, msg.snake_name.upper(), msg.snake_name),
            '',
            '''\
    shared_assert(payload != NULL);

    struct CanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(tx_msg));
    tx_msg.std_id = CANMSGS_{msg_name_uppercase}_FRAME_ID;
    tx_msg.dlc    = CANMSGS_{msg_name_uppercase}_LENGTH;
    memcpy(&tx_msg.data[0], &payload, CANMSGS_{msg_name_uppercase}_LENGTH);
    App_CanMsgs_{msg_name_snakecase}_pack(&tx_msg.data[0], payload, CANMSGS_{msg_name_uppercase}_LENGTH);
    Io_SharedCan_TxMessageQueueSendtoBack(&tx_msg);'''.format(
                msg_name_uppercase=msg.snake_name.upper(),
                msg_name_snakecase=msg.snake_name)
        ) for msg in self._non_periodic_cantx_msgs)

class IoCanTxHeaderFileGenerator(IoCanTxFileGenerator):
    def __init__(self, database, output_path, sender, function_prefix):
        super().__init__(database, output_path, sender, function_prefix)

    def generateHeader(self):
        self._write_output(self._generateHeader(
            'CAN TX Library for the IO layer',
            self.__generateHeaderIncludes(),
            self.__generateForwardDeclarations(),
            self.__generateFunctionDeclarations()))

    def __generateHeaderIncludes(self):
        header_names = ['<stdint.h>',
                        '"auto_generated/App_CanMsgs.h"']
        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateForwardDeclarations(self):
        forward_declarations = []
        forward_declarations.append('struct CanTxInterface;')
        return '\n' + '\n'.join(forward_declarations)

    def __generateFunctionDeclarations(self):
        function_declarations = []
        function_declarations.append(self._EnqueuePeriodicMsgs.declaration)
        function_declarations.append(
            '/** @brief Enqueue non-periodic CAN message to the CAN TX queue */\n'
            + '\n'.join([func.declaration for func in self._EnqueueNonPeriodicMsgs]))
        return '\n' + '\n'.join(function_declarations)

class IoCanTxSourceFileGenerator(IoCanTxFileGenerator):
    def __init__(self, database, output_path, sender, function_prefix):
        super().__init__(database, output_path, sender, function_prefix)

    def generateSource(self):
        self._write_output(self._generateSource(
            self.__generateHeaderIncludes(),
            self.__generateTypedefs(),
            self.__generateMacros(),
            self.__generateVariables(),
            self.__generateFunctionDefinitions(),
            self.__generatePrivateFunctionDefinitions(),
            self.__generatePrivateFunctionDeclarations()))

    def __generateHeaderIncludes(self):
        header_names = ['<string.h>',
                        '<FreeRTOS.h>',
                        '<portmacro.h>',
                        '"auto_generated/Io_CanTx.h"',
                        '"auto_generated/App_CanTx.h"',
                        '"Io_SharedCan.h"',
                        '"App_SharedAssert.h"']
        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateTypedefs(self):
        return ''

    def __generateMacros(self):
        return ''

    def __generateVariables(self):
        return ''

    def __generatePrivateFunctionDefinitions(self):
        return ''

    def __generatePrivateFunctionDeclarations(self):
        return ''

    def __generateFunctionDefinitions(self):
        function_defs = []
        function_defs.append(self._EnqueuePeriodicMsgs.definition)
        function_defs.extend(func.definition for func in self._EnqueueNonPeriodicMsgs)
        return '\n\n'.join(function_defs)
