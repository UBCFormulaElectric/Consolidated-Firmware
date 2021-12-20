from codegen_shared import *
from decimal import Decimal

def _format_decimal(value, is_float=False):
    if int(value) == value:
        value = int(value)

        if is_float:
            return str(value) + '.0'
        else:
            return str(value)
    else:
        return str(value)

def _generate_clamp(signal):
    """
    Generate a clamping expression for a given CAN signal value
    """
    scale = signal.decimal.scale
    offset = (signal.decimal.offset / scale)
    minimum = signal.decimal.minimum
    maximum = signal.decimal.maximum

    if minimum is not None:
        minimum = (minimum / scale - offset)

    if maximum is not None:
        maximum = (maximum / scale - offset)

    if minimum is None and signal.minimum_value is not None:
        if signal.minimum_value > signal.minimum_type_value:
            minimum = signal.minimum_value

    if maximum is None and signal.maximum_value is not None:
        if signal.maximum_value < signal.maximum_type_value:
            maximum = signal.maximum_value

    suffix = signal.type_suffix
    check = []

    if minimum is not None:
        if not signal.is_float:
            minimum = Decimal(int(minimum))

        minimum_type_value = signal.minimum_type_value

        if (minimum_type_value is None) or (minimum > minimum_type_value):
            minimum = _format_decimal(minimum, signal.is_float)
            check.append('(value < {minimum}) ? {minimum} :'.format(minimum=minimum + suffix))

    if maximum is not None:
        if not signal.is_float:
            maximum = Decimal(int(maximum))

        maximum_type_value = signal.maximum_type_value

        if (maximum_type_value is None) or (maximum < maximum_type_value):
            maximum = _format_decimal(maximum, signal.is_float)
            check.append('(value > {maximum}) ? {maximum} : value'.format(maximum=maximum + suffix))

    if not check:
        return '(void)value;'
    else:
        return 'value = {};'.format(' '.join(check))


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

        # Initialize function objects so we can get its declaration and
        # definition when generating the source and header fie
        self.__init_functions(function_prefix)

    def __init_functions(self, function_prefix):
        function_params = \
            [("    void (*send_non_periodic_msg_%s)(const struct CanMsgs_%s_t* payload),"
              % (msg.snake_name.upper(), msg.snake_name))
             for msg in self._non_periodic_cantx_msgs]
        init_senders = \
            [("    can_tx_interface->send_non_periodic_msg_%s = send_non_periodic_msg_%s;"
              % (msg.snake_name.upper(), msg.snake_name.upper()))
             for msg in self._non_periodic_cantx_msgs]

        initial_signal_setters = '\n'.join(
            ["""\
    App_CanTx_SetPeriodicSignal_{signal_name}(can_tx_interface, {initial_value});""".
                 format(signal_name=signal.snake_name.upper(),
                        initial_value=signal.initial if signal.initial != None else '0'
                        ) for msg in self._periodic_cantx_msgs for signal in msg.signals])

        self._Create = Function(
            'struct %sCanTxInterface* %s_Create(%s)'
                % (self._sender.capitalize(), function_prefix, '\n' + '\n'.join(function_params)[:-1]),
            'Allocate and initialize a CAN TX interface',
            '''\
    struct {sender}CanTxInterface* can_tx_interface = malloc(sizeof(struct {sender}CanTxInterface));

    assert(can_tx_interface != NULL);\n\n'''
    .format(sender=self._sender.capitalize())
    + '\n'.join(init_senders)
    + '''

{initial_signal_setters}

    return can_tx_interface;'''.format(
        sender=self._sender.capitalize(),
        initial_signal_setters=initial_signal_setters))

        self._Destroy = Function(
             'void %s_Destroy(struct %sCanTxInterface* can_tx_interface)'
                % (function_prefix, self._sender.capitalize()),
            'Destroy a CAN TX interface, freeing the memory associated with it',
            '''    free(can_tx_interface);''')

        lst = []

        for msg in self._periodic_cantx_msgs:
            for signal in msg.signals:

                clamp = _generate_clamp(signal)
             
                if signal.is_float:
                    lst.append(Function(
                        'void %s_SetPeriodicSignal_%s(struct %sCanTxInterface* can_tx_interface, %s value)' % (
                        function_prefix, signal.snake_name.upper(), self._sender.capitalize(), signal.type_name),
                        '',
                        '''\
    if (isnan(value))
    {{
        can_tx_interface->periodic_can_tx_table.{msg_snakecase_name}.{signal_snakecase_name} = value;
    }}
    else
    {{
        // Clamp the given value if it is out of range
        {clamp}
        can_tx_interface->periodic_can_tx_table.{msg_snakecase_name}.{signal_snakecase_name} = value;
    }}'''.format(msg_snakecase_name=msg.snake_name,
                 signal_snakecase_name=signal.snake_name,
                 clamp=clamp)))
                else:
                    lst.append(Function(
                        'void %s_SetPeriodicSignal_%s(struct %sCanTxInterface* can_tx_interface, %s value)' % (
                        function_prefix, signal.snake_name.upper(), self._sender.capitalize(), signal.type_name),
                        '',
                        '''\
    // Clamp the given value if it is out of range
    {clamp}
    can_tx_interface->periodic_can_tx_table.{msg_snakecase_name}.{signal_snakecase_name} = value;'''.format(
            msg_snakecase_name=msg.snake_name,
            signal_snakecase_name=signal.snake_name,
            clamp=clamp)))

        self._PeriodicTxSignalSetters = lst

        self._PeriodicTxSignalGetters = list(Function(
            '%s %s_GetPeriodicSignal_%s(const struct %sCanTxInterface* can_tx_interface)' % (
            signal.type_name, function_prefix, signal.snake_name.upper(), self._sender.capitalize()),
            '',
            '''\
    return can_tx_interface->periodic_can_tx_table.{msg_snakecase_name}.{signal_snakecase_name};'''.format(
                msg_snakecase_name=msg.snake_name,
                signal_snakecase_name=signal.snake_name)
        ) for msg in self._periodic_cantx_msgs for signal in msg.signals)

        self._PeriodicTxMsgPointerGetters = list(Function(
            'const struct CanMsgs_%s_t* %s_GetPeriodicMsgPointer_%s(const struct %sCanTxInterface* can_tx_interface)' % (
                msg.snake_name, function_prefix, msg.snake_name.upper(), self._sender.capitalize()),
            '',
            '''\
    return &can_tx_interface->periodic_can_tx_table.{msg_name};'''.format(
                msg_name=msg.snake_name)
        ) for msg in self._periodic_cantx_msgs)

        self._SendNonPeriodicMsgs = list(Function(
        'void %s_SendNonPeriodicMsg_%s(const struct %sCanTxInterface* can_tx_interface, const struct CanMsgs_%s_t* payload)' % (
            function_prefix, msg.snake_name.upper(), self._sender.capitalize(), msg.snake_name),
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
                        '"App_CanMsgs.h"']
        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateForwardDeclarations(self):
        return ''

    def __generateFunctionDeclarations(self):
        function_declarations = []
        function_declarations.append(self._Create.declaration)
        function_declarations.append(self._Destroy.declaration)
        function_declarations.append(
            '/** @brief Signal setters for periodic CAN TX messages */\n'
            + '\n'.join([func.declaration for func in self._PeriodicTxSignalSetters]))
        function_declarations.append(
            '/** @brief Signal getters for periodic CAN TX messages */\n'
            + '\n'.join([func.declaration for func in self._PeriodicTxSignalGetters]))
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
            '{sender}CanTxInterface'.format(sender=self._sender.capitalize()),
            [StructMember('struct PeriodicCanTxMsgs',
                          'periodic_can_tx_table',
                          '0')] +
            [StructMember('void (*send_non_periodic_msg_%s)(const struct CanMsgs_%s_t* payload)'
                            % (msg.snake_name.upper(), msg.snake_name),
                          '',
                          '0')
             for msg in self._non_periodic_cantx_msgs],
            'Can TX interface')

    def __generateHeaderIncludes(self):
        header_names = ['<stdlib.h>',
                        '<assert.h>',
                        '<math.h>',
                        '"App_CanTx.h"']
        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateTypedefs(self):
        typedefs = []
        typedefs.append(self.__PeriodicCanTxMsgs.declaration)
        typedefs.append(self.__CanTxInterface.declaration)
        return '\n\n'.join(typedefs)

    def __generateMacros(self):
        macros = []
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
        function_defs.append(self._Destroy.definition)
        function_defs.extend(func.definition for func in self._PeriodicTxSignalSetters)
        function_defs.extend(func.definition for func in self._PeriodicTxSignalGetters)
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
void %s_EnqueuePeriodicMsgs(struct %sCanTxInterface* can_tx_interface, const uint32_t current_ms)''' % (function_prefix, self._sender.capitalize()),
            'Enqueue periodic CAN TX messages according to the cycle time specified in the DBC. This should be called in a 1kHz task.',
            FunctionDef)

        self._EnqueueNonPeriodicMsgs = list(Function(
            'void %s_EnqueueNonPeriodicMsg_%s(const struct CanMsgs_%s_t* payload)'
            % (function_prefix, msg.snake_name.upper(), msg.snake_name),
            '',
            '''\
    assert(payload != NULL);

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
                        '"App_CanMsgs.h"']
        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateForwardDeclarations(self):
        forward_declarations = []
        forward_declarations.append('struct {sender}CanTxInterface;'.format(sender=self._sender.capitalize()))
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
                        '<assert.h>',
                        '"Io_CanTx.h"',
                        '"App_CanTx.h"',
                        '"Io_SharedCan.h"']
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
