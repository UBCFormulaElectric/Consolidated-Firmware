from codegen_shared import *

class CanTxFileGenerator(CanFileGenerator):
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

        self._periodicTxTableName = 'PeriodicCanTxMsgTable'

        self.__init_variables()

        # Initialize function objects so we can get its declaration and
        # definition when generating the source and header fie
        self.__init_functions(function_prefix)

    def __init_variables(self):
        self._cantx_initialized = Variable('static bool', 'cantx_initialized', 'false', 'Boolean flag indicating whether this library is initialized')

    def __init_functions(self, function_prefix):
        self._Init = Function(
            'void %s_Init(void)' % function_prefix,
            'Initialize CAN TX library',
            '    %s = true;' % self._cantx_initialized.get_name())

        FunctionDef = '''
    shared_assert(%s == true);\n\n''' % self._cantx_initialized.get_name()

        FunctionDef += '\n\n'.join(['''\
    // Is it time to transmit this particular CAN message?
    if ((HAL_GetTick() % {period}) == 0)
    {{
        struct CanMsg tx_message;
        memset(&tx_message, 0, sizeof(tx_message));

        // Prepare CAN message header
        tx_message.std_id = {std_id};
        tx_message.dlc = {dlc};

        // Prepare CAN message payload (The packing function isn't thread-safe
        // so we must guard it)
        vPortEnterCritical();
        {msg_packing_function}(
            &tx_message.data[0],
            &{periodic_table_name}.{msg_snake_name},
            tx_message.dlc);
        vPortExitCritical();

        App_SharedCan_TxMessageQueueSendtoBack(&tx_message);
    }}'''.format(msg_snake_name=msg.snake_name,
                 msg_packing_function='CanMsgs_%s_pack' % msg.snake_name,
                 periodic_table_name=self._periodicTxTableName,
                 std_id='CANMSGS_%s_FRAME_ID' % msg.snake_name.upper(),
                 dlc='CANMSGS_%s_LENGTH' % msg.snake_name.upper(),
                 period='CANMSGS_%s_CYCLE_TIME_MS' % msg.snake_name.upper()) for msg in self._periodic_cantx_msgs])

        self._TransmitPeriodicMsgs = Function('void %s_TransmitPeriodicMessages(void)' % function_prefix,
                                              'Transmits periodic CAN TX messages according to the cycle time specified in the DBC. This should be called in a 1kHz task.',
                                              FunctionDef)

        self._PeriodicTxSignalSetters = list(Function(
            'void %s_SetPeriodicSignal_%s(%s value)' % (
            function_prefix, signal.uppercase_name, signal.type_name),
            '',
            '''\
    {periodic_table_name}.{msg_name}.{signal_name} = value;'''.format(
                msg_name=signal.msg_name_snakecase,
                periodic_table_name=self._periodicTxTableName,
                signal_name=signal.snakecase_name)
        ) for signal in self._periodic_cantx_signals)

        self._EnqueueNonPeriodicMsgs = list(Function(
            'void %s_EnqueueNonPeriodicMsg_%s(struct CanMsgs_%s_t* payload)'
            % (function_prefix, msg.snake_name.upper(), msg.snake_name),
            '',
            '''\
    shared_assert({initialized_flag} == true);
    shared_assert(payload != NULL);

    struct CanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(tx_msg));
    tx_msg.std_id = CANMSGS_{msg_name_uppercase}_FRAME_ID;
    tx_msg.dlc    = CANMSGS_{msg_name_uppercase}_LENGTH;
    memcpy(&tx_msg.data[0], &payload, CANMSGS_{msg_name_uppercase}_LENGTH); 
    CanMsgs_{msg_name_snakecase}_pack(&tx_msg.data[0], payload, CANMSGS_{msg_name_uppercase}_LENGTH);
    App_SharedCan_TxMessageQueueSendtoBack(&tx_msg);'''.format(
                initialized_flag=self._cantx_initialized.get_name(),
                msg_name_uppercase=msg.snake_name.upper(),
                msg_name_snakecase=msg.snake_name)
        ) for msg in self._non_periodic_cantx_msgs)

        self._ForceEnqueueNonPeriodicMsgs = list(Function(
            'void %s_ForceEnqueueNonPeriodicMsg_%s(struct CanMsgs_%s_t* payload)' % (function_prefix, msg.snake_name.upper(), msg.snake_name),
            '',
            '''\
    shared_assert({initialized_flag} == true);
    shared_assert(payload != NULL);

    struct CanMsg tx_msg;
    memset(&tx_msg, 0, sizeof(tx_msg));
    tx_msg.std_id = CANMSGS_{msg_name_uppercase}_FRAME_ID;
    tx_msg.dlc    = CANMSGS_{msg_name_uppercase}_LENGTH;
    memcpy(&tx_msg.data[0], &payload, CANMSGS_{msg_name_uppercase}_LENGTH); 
    CanMsgs_{msg_name_snakecase}_pack(&tx_msg.data[0], payload, CANMSGS_{msg_name_uppercase}_LENGTH);
    App_SharedCan_TxMessageQueueForceSendToBack(&tx_msg);'''.format(
                initialized_flag=self._cantx_initialized.get_name(),
                msg_name_uppercase=msg.snake_name.upper(),
                msg_name_snakecase=msg.snake_name)
        ) for msg in self._non_periodic_cantx_msgs)

class CanTxHeaderFileGenerator(CanTxFileGenerator):
    def __init__(self, database, output_path, sender, function_prefix):
        super().__init__(database, output_path, sender, function_prefix)

    def generateHeader(self):
        self._write_output(self._generateHeader(
            'CAN TX Library',
            '%s_H' % self._function_prefix.upper(),
            self.__generateHeaderIncludes(),
            self.__generateFunctionDeclarations()))

    def __generateHeaderIncludes(self):
        header_names = ['<stdint.h>',
                        '"auto_generated/CanMsgs.h"']
        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateFunctionDeclarations(self):
        function_declarations = []
        function_declarations.append(self._Init.declaration)
        function_declarations.append(self._TransmitPeriodicMsgs.declaration)
        function_declarations.append('/** @brief Signal getters for periodic CAN TX messages */\n' + '\n'.join([func.declaration for func in self._PeriodicTxSignalSetters]))
        function_declarations.append('/** @brief "Normal" enqueue functions for non-periodic CAN TX messages */\n' + '\n'.join([func.declaration for func in self._EnqueueNonPeriodicMsgs]))
        function_declarations.append('/** @brief "Force" enqueue functions for non-periodic CAN TX messages*/\n' + '\n'.join([func.declaration for func in self._ForceEnqueueNonPeriodicMsgs]))
        return '\n\n'.join(function_declarations)

class CanTxSourceFileGenerator(CanTxFileGenerator):
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
            [StructMember('struct CanMsgs_%s_t' % msg.snake_name, msg.snake_name, 'INIT_PERIODIC_CANTX_MSG()') for msg in self._periodic_cantx_msgs],
            'Periodic CAN TX message')

    def __generateHeaderIncludes(self):
        header_names = ['<sched.h>',
                        '<string.h>',
                        '<FreeRTOS.h>',
                        '"auto_generated/CanMsgs.h"',
                        '"SharedAssert.h"',
                        '"SharedCan.h"']
        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateTypedefs(self):
        typedefs = []
        typedefs.append(self.__PeriodicCanTxMsgs.declaration)
        return '\n\n'.join(typedefs)

    def __generateMacros(self):
        macros = []
        macros.append(Macro(
            'INIT_PERIODIC_CANTX_MSG()',
            '    {0}',
            'Initialize a periodic CAN TX message').declaration)
        return '\n\n'.join(macros)

    def __generateVariables(self):
        variables = []
        variables.append(self.__PeriodicCanTxMsgs.get_definition(self._periodicTxTableName, 'Table of periodic CAN TX messages'))
        variables.append(self._cantx_initialized.get_definition())
        return '\n\n'.join(variables)

    def __generatePrivateFunctionDeclarations(self):
        func_declarations = []
        return  '\n\n'.join(func_declarations)

    def __generatePrivateFunctionDefinitions(self):
        function_defs = []
        return '\n'.join(function_defs)

    def __generateFunctionDefinitions(self):
        function_defs = []
        function_defs.append(self._Init.definition)
        function_defs.append(self._TransmitPeriodicMsgs.definition)
        function_defs.extend(func.definition for func in self._PeriodicTxSignalSetters)
        function_defs.extend(func.definition for func in self._EnqueueNonPeriodicMsgs)
        function_defs.extend(func.definition for func in self._ForceEnqueueNonPeriodicMsgs)

        return '\n\n'.join(function_defs)
