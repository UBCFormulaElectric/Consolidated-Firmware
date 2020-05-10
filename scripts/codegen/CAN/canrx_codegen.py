from codegen_shared import *

class CanRxFileGenerator(CanFileGenerator):
    def __init__(self, database, output_path, receiver):
        super().__init__(database, output_path, receiver)
        self._receiver = receiver
        self._canrx_msgs = self.__get_canrx_msgs()

    def __get_canrx_msgs(self):
        canrx_msg = []
        for msg in self._get_can_msgs():
            # If we receive any signal in this message, we mark it as a CAN RX message
            for signal in msg.signals:
                if self._receiver in signal.receivers:
                    canrx_msg.append(msg)
                    break
        return canrx_msg

class AppCanRxFileGenerator(CanRxFileGenerator):
    def __init__(self, database, output_path, receiver, function_prefix):
        super().__init__(database, output_path, receiver)

        # Initialize function objects so we can get its declaration and
        # definition when generating the source and header fie
        self.__init_functions(function_prefix)

    def __init_functions(self, function_prefix):
        initial_signal_setters = '\n'.join(
            ["""\
    App_CanRx_{msg_name}_SetSignal_{signal_name}(can_rx_interface, {initial_value});""".
                format(msg_name=msg.snake_name.upper(),
                       signal_name=signal.snake_name.upper(),
                       initial_value=signal.initial if signal.initial != None else '0'
                       ) for msg in self._canrx_msgs for signal in msg.signals])

        self._Create = Function(
            'struct %sCanRxInterface* %s_Create(void)' % (self._receiver.capitalize(), function_prefix),
            'Allocate and initialize a CAN RX interface',
            '''\
    struct {board}CanRxInterface* can_rx_interface = malloc(sizeof(struct {board}CanRxInterface));
    
    assert(can_rx_interface != NULL);
    
{initial_signal_setters}

    return can_rx_interface;'''.format(
        board=self._receiver.capitalize(),
        initial_signal_setters=initial_signal_setters))

        self._Destroy = Function(
             'void %s_Destroy(struct %sCanRxInterface* can_rx_interface)'
                % (function_prefix, self._receiver.capitalize()),
            'Destroy a CAN RX interface, freeing the memory associated with it',
            '''    free(can_rx_interface);''')

        self._CanRxSignalGetters = [
            Function('%s %s_%s_GetSignal_%s (const struct %sCanRxInterface* can_rx_interface)'
                     % (signal.type_name, function_prefix, 
                         msg.snake_name.upper(), signal.snake_name.upper(), 
                         self._receiver.capitalize()),
                     '',
                     '''\
    return can_rx_interface->can_rx_table.{msg_name}.{signal_name};'''.format(
                         signal_type=signal.type_name,
                         signal_name=signal.snake_name,
                         msg_name=msg.snake_name))
            for msg in self._canrx_msgs for signal in msg.signals]

        self._CanRxSignalSetters = list(Function(
            'void %s_%s_SetSignal_%s(struct %sCanRxInterface* can_rx_interface, %s value)' % (
            function_prefix, msg.snake_name.upper(), 
            signal.snake_name.upper(), self._receiver.capitalize(), signal.type_name),
            '',
            '''\
    if (App_CanMsgs_{msg_snakecase_name}_{signal_snakecase_name}_is_in_range(value) == true)
    {{
        can_rx_interface->can_rx_table.{msg_snakecase_name}.{signal_snakecase_name} = value;
    }}'''.format(
                msg_snakecase_name=msg.snake_name,
                signal_snakecase_name=signal.snake_name)
        ) for msg in self._canrx_msgs for signal in msg.signals)

class AppCanRxHeaderFileGenerator(AppCanRxFileGenerator):
    def __init__(self, database, output_path, receiver, function_prefix):
        super().__init__(database, output_path, receiver, function_prefix)

    def generateHeader(self):
        self._write_output(self._generateHeader(
            'CAN RX Library for the App layer',
            self.__generateHeaderIncludes(),
            self.__generateForwardDeclarations(),
            self.__generateFunctionDeclarations()))

    def __generateHeaderIncludes(self):
        header_names = ['<stdint.h>']
        return '\n'.join([HeaderInclude(name).get_include() for name in header_names])

    def __generateForwardDeclarations(self):
        return ''

    def __generateFunctionDeclarations(self):
        function_declarations = []
        function_declarations.append(self._Create.declaration)
        function_declarations.append(self._Destroy.declaration)
        function_declarations.append(
            '/** @brief CAN RX signal getters */\n'
            + '\n'.join([func.declaration for func in self._CanRxSignalGetters]))
        function_declarations.append(
            '/** @brief CAN RX signal setters */\n'
            + '\n'.join([func.declaration for func in self._CanRxSignalSetters]))
        return '\n\n'.join(function_declarations)

class AppCanRxSourceFileGenerator(AppCanRxFileGenerator):
    def __init__(self, database, output_path, receiver, function_prefix):
        super().__init__(database, output_path, receiver, function_prefix)
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
        self.__CanRxMsgs = Struct(
            'CanRxMsgs',
            [StructMember('struct CanMsgs_%s_t' % msg.snake_name,
                          msg.snake_name,
                          '0') for msg in self._canrx_msgs],
            'CAN RX Messages')
        self.__CanRxInterface = Struct(
            '%sCanRxInterface' % self._receiver.capitalize(),
            [StructMember('struct CanRxMsgs',
                          'can_rx_table',
                          0)],
            'CAN RX interface')

    def __generateHeaderIncludes(self):
        header_names = ['<stdlib.h>',
                        '<assert.h>',
                        '"App_CanRx.h"',
                        '"App_CanMsgs.h"']

        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateTypedefs(self):
        typedefs = []
        typedefs.append(self.__CanRxMsgs.declaration)
        typedefs.append(self.__CanRxInterface.declaration)
        return '\n' + '\n\n'.join(typedefs)

    def __generateMacros(self):
        macros = []
        return '\n' + '\n'.join(macros)

    def __generateVariables(self):
        variables = []
        return '\n'.join(variables)

    def __generatePrivateFunctionDeclarations(self):
        func_declarations = []
        return '\n'.join(func_declarations)

    def __generatePrivateFunctionDefinitions(self):
        function_defs = []
        return '\n'.join(function_defs)

    def __generateFunctionDefinitions(self):
        function_defs = []
        function_defs.append(self._Create.definition)
        function_defs.append(self._Destroy.definition)
        function_defs.extend([func.definition for func in self._CanRxSignalGetters])
        function_defs.extend([func.definition for func in self._CanRxSignalSetters])
        return '\n\n'.join(function_defs)

class IoCanRxFileGenerator(CanRxFileGenerator):
    def __init__(self, database, output_path, receiver, function_prefix):
        super().__init__(database, output_path, receiver)

        # Initialize function objects so we can get its declaration and
        # definition when generating the source and header fie
        self.__init_functions(function_prefix)

    def __init_functions(self, function_prefix):
        _CanRxFilterMessageId_Cases = ['''\
        case CANMSGS_{msg_uppercase_name}_FRAME_ID:'''.
            format(msg_uppercase_name=msg.snake_name.upper()) for msg in self._canrx_msgs]

        self._CanRxFilterMessage = Function(
            'bool %s_FilterMessageId(uint32_t std_id)' % function_prefix,
            'Returns true if %s listens to the given message ID' % self._receiver,
            '''\
    bool isFound = false;
    switch (std_id)
    {{
{cases}
        {{
            isFound = true;
            break;
        }}
        default:
        {{
            break;
        }}
    }}
    return isFound;'''.format(cases='\n'.join(_CanRxFilterMessageId_Cases)))

        _CanRxUpdateRxTableWithMessage_Cases = []
        for msg in self._canrx_msgs:
            unpack_func = '''
            App_CanMsgs_{msg_snakecase_name}_unpack(
                &buffer,
                &message->data[0],
                CANMSGS_{msg_uppercase_name}_LENGTH);'''.format(
                    msg_snakecase_name=msg.snake_name,
                    msg_uppercase_name=msg.snake_name.upper())

            signal_setters_fmt = '''
            App_CanRx_{msg_uppercase_name}_SetSignal_{signal_name_uppercase}(
                can_rx_interface,
                buffer.{signal_name_snakecase});'''
            signal_setters = '\n'.join([
                signal_setters_fmt.format(
                    msg_uppercase_name=msg.snake_name.upper(),
                    signal_name_uppercase=signal.snake_name.upper(),
                    signal_name_snakecase=signal.snake_name)
                 for signal in msg.signals if self._receiver in signal.receivers])

            _CanRxUpdateRxTableWithMessage_Cases.append('''\
        case CANMSGS_{msg_uppercase_name}_FRAME_ID:
        {{
            struct CanMsgs_{msg_snakecase_name}_t buffer;
            {unpack}
            {set_signals}
        }}
        break;'''.format(msg_uppercase_name=msg.snake_name.upper(),
                         msg_snakecase_name=msg.snake_name,
                         unpack=unpack_func,
                         set_signals=signal_setters))

        self._CanRxUpdateRxTableWithMessage = Function(
            'void %s_UpdateRxTableWithMessage(struct %sCanRxInterface* can_rx_interface, const struct CanMsg* message)' % (function_prefix, self._receiver.capitalize()),
            "Update the CAN RX table with the given CAN message.",
            '''\
    assert(can_rx_interface != NULL);
    assert(message != NULL);

    switch (message->std_id)
    {{
{cases}
        default:
        {{
            break;
        }}
    }}'''.format(cases='\n'.join(_CanRxUpdateRxTableWithMessage_Cases)))

class IoCanRxHeaderFileGenerator(IoCanRxFileGenerator):
    def __init__(self, database, output_path, receiver, function_prefix):
        super().__init__(database, output_path, receiver, function_prefix)

    def generateHeader(self):
        self._write_output(self._generateHeader(
            'CAN RX Library for the IO layer',
            self.__generateHeaderIncludes(),
            self.__generateForwardDeclarations(),
            self.__generateFunctionDeclarations()))

    def __generateHeaderIncludes(self):
        header_names = ['<stdbool.h>',
                        '<stdint.h>']
        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateForwardDeclarations(self):
        forward_declarations = []
        forward_declarations.append('struct %sCanRxInterface;' % self._receiver.capitalize())
        forward_declarations.append('struct CanMsg;')
        return '\n'.join(forward_declarations)

    def __generateFunctionDeclarations(self):
        function_declarations = []
        function_declarations.append(self._CanRxUpdateRxTableWithMessage.declaration)
        function_declarations.append(self._CanRxFilterMessage.declaration)
        return '\n' + '\n\n'.join(function_declarations)

class IoCanRxSourceFileGenerator(IoCanRxFileGenerator):
    def __init__(self, database, output_path, receiver, function_prefix):
        super().__init__(database, output_path, receiver, function_prefix)

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
        header_names = ['<assert.h>',
                        '"App_CanMsgs.h"',
                        '"App_CanRx.h"',
                        '"Io_CanRx.h"',
                        '"Io_SharedCanMsg.h"']

        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateTypedefs(self):
        typedefs = []
        return '\n\n'.join(typedefs)

    def __generateMacros(self):
        macros = []
        return '\n'.join(macros)

    def __generateVariables(self):
        variables = []
        return '\n\n'.join(variables)

    def __generatePrivateFunctionDeclarations(self):
        func_declarations = []
        return '\n'.join(func_declarations)

    def __generatePrivateFunctionDefinitions(self):
        function_defs = []
        return '\n'.join(function_defs)

    def __generateFunctionDefinitions(self):
        function_defs = []
        function_defs.append(self._CanRxUpdateRxTableWithMessage.definition)
        function_defs.append(self._CanRxFilterMessage.definition)
        return '\n\n'.join(function_defs)
