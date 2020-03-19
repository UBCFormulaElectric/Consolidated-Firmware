from codegen_shared import *

class CanRxFileGenerator(CanFileGenerator):
    def __init__(self, database, output_path, receiver, function_prefix):
        super().__init__(database, output_path, receiver)
        self.__database = database
        self.__receiver = receiver
        self._canrx_msgs = self.__get_canrx_msgs()
        self._canrx_signals = self.__get_canrx_signals()
        self._receiver = receiver
        self._function_prefix = function_prefix
        self._RxTableName = 'CanRxMsgTable'

        self.__init_variables()

        # Initialize function objects so we can get its declaration and
        # definition when generating the source and header fie
        self.__init_functions(function_prefix)

    def __init_variables(self):
        self._canrx_initialized = Variable('static bool', 'canrx_initialized', 'false', 'Boolean flag indicating whether this library is initialized')

    def __init_functions(self, function_prefix):
        self._Init = Function(
            'void %s_Init(void)' % function_prefix,
            'Initialize CAN RX library',
            '''\
    %s_InitializePeriodicCanRxMsgMutexes();
    %s = true;''' % (function_prefix, self._canrx_initialized.get_name()))

        self._InitPeriodicCanRxMutex = Function(
            'static void %s_InitializePeriodicCanRxMsgMutexes(void)' % function_prefix,
            'Initialize mutex for CAN RX messages',
            '\n\n'.join(['''\
    {table_name}.{msg_snake_name}.xSemaphore = xSemaphoreCreateMutexStatic(&{table_name}.{msg_snake_name}.xMutexBuffer);
    assert_param({table_name}.{msg_snake_name}.xSemaphore);'''
                        .format(msg_snake_name=msg.snake_name, table_name=self._RxTableName)
                         for msg in self._canrx_msgs]))

        self._CanRxSignalGetters = [
            Function('%s %s_%s_GetSignal_%s (void)'
                     % (signal.type_name, self._function_prefix, signal.msg_name_uppercase, signal.uppercase_name),
                     '',
                     '''\
    shared_assert({initialized_flag} == true);
    
    xSemaphoreTake({table_name}.{msg_name}.xSemaphore, portMAX_DELAY);

    {signal_type} signal_value = {table_name}.{msg_name}.payload.{signal_name};

    xSemaphoreGive({table_name}.{msg_name}.xSemaphore);
    
    return signal_value;'''.format(
                         initialized_flag=self._canrx_initialized.get_name(),
                         signal_type=signal.type_name,
                         signal_name=signal.snakecase_name,
                         table_name=self._RxTableName,
                         msg_name=signal.msg_name_snakecase))
            for signal in self._canrx_signals]

        _CanRxReadMessageIntoTableFromISR_Cases = ['''\
        case CANMSGS_{msg_uppercase_name}_FRAME_ID:
        {{
            if (xSemaphoreTakeFromISR({table_name}.{msg_snakecase_name}.xSemaphore, pxHigherPriorityTaskWoken) == pdTRUE)
            {{
                CanMsgs_{msg_snakecase_name}_unpack(&{table_name}.{msg_snakecase_name}.payload, &data[0], CANMSGS_{msg_uppercase_name}_LENGTH);
                xSemaphoreGiveFromISR({table_name}.{msg_snakecase_name}.xSemaphore, pxHigherPriorityTaskWoken);
            }}
            else
            {{
                status = false;
            }}
            break;
        }}'''.format(msg_uppercase_name=msg.snake_name.upper(), msg_snakecase_name=msg.snake_name, table_name=self._RxTableName) for msg in self._canrx_msgs]

        self._CanRxReadMessageIntoTableFromISR = Function(
            'bool %s_ReadMessageIntoTableFromISR(uint32_t std_id, uint8_t data[8], void* pxHigherPriorityTaskWoken)' % self._function_prefix,
            "Copy a CAN message to the CAN RX table in ISR. Returns FALSE if we failed to acquire mutex (Recall that mutex can't block in ISR).",
            '''\
    shared_assert({initialized_flag} == true);
            
    // This function is intended to be called from an interrupt-context
    portASSERT_IF_IN_ISR();
    
    bool status = true;
    
    switch (std_id)
    {{
{cases}
        default:
        {{
            break;
        }}
    }}
    return status; '''.format(
                initialized_flag=self._canrx_initialized.get_name(),
                cases='\n'.join(_CanRxReadMessageIntoTableFromISR_Cases)))

        _CanRxReadMessageIntoTableFromTask_Cases = ['''\
        case CANMSGS_{msg_uppercase_name}_FRAME_ID:
        {{
            if (xSemaphoreTake({table_name}.{msg_snakecase_name}.xSemaphore, portMAX_DELAY) == pdTRUE)
            {{
                CanMsgs_{msg_snakecase_name}_unpack(&{table_name}.{msg_snakecase_name}.payload, &data[0], CANMSGS_{msg_uppercase_name}_LENGTH);
                xSemaphoreGive({table_name}.{msg_snakecase_name}.xSemaphore);
            }}
            break;
        }}'''.format(msg_uppercase_name=msg.snake_name.upper(),
                     msg_snakecase_name=msg.snake_name,
                     table_name=self._RxTableName) for msg in
                                               self._canrx_msgs]

        self._CanRxReadMessageIntoTableFromTask = Function(
            'void %s_ReadMessageIntoTableFromTask(uint32_t std_id, uint8_t data[8])' % self._function_prefix,
            "Copy a CAN message to the CAN RX table in a task.",
            '''\
    shared_assert({initialized_flag} == true);
    
    switch (std_id)
    {{
{cases}
        default:
        {{
            break;
        }}
    }}'''.format(
                initialized_flag=self._canrx_initialized.get_name(),
                cases='\n'.join(_CanRxReadMessageIntoTableFromTask_Cases)))

    def __get_canrx_signals(self):
        canrx_signals = []
        for msg in self.__get_canrx_msgs():
            for signal in msg.signals:
                if self.__receiver in signal.receivers:
                    canrx_signals.append(
                        CanSignal(signal.type_name, signal.snake_name,
                                    msg.snake_name))
        return canrx_signals

    def __get_canrx_msgs(self):
        canrx_msg = []
        for msg in self._get_can_msgs():
            # If we receive any signal in this message, we mark it as a CAN RX message
            for signal in msg.signals:
                if self.__receiver in signal.receivers:
                    canrx_msg.append(msg)
                    break
        return canrx_msg

class CanRxHeaderFileGenerator(CanRxFileGenerator):
    def __init__(self, database, output_path, receiver, function_prefix):
        super().__init__(database, output_path, receiver, function_prefix)

    def generateHeader(self):
        self._write_output(self._generateHeader(
            'CAN RX Library',
            self.__generateHeaderIncludes(),
            self.__generateForwardDeclarations(),
            self.__generateFunctionDeclarations()))

    def __generateHeaderIncludes(self):
        header_names = ['<stdint.h>',
                        '<stdbool.h>']
        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateForwardDeclarations(self):
        return ''

    def __generateFunctionDeclarations(self):
        function_declarations = []
        function_declarations.append(self._Init.declaration)
        function_declarations.append(self._CanRxReadMessageIntoTableFromISR.declaration)
        function_declarations.append(self._CanRxReadMessageIntoTableFromTask.declaration)
        function_declarations.append('/** @brief CAN RX signal getters */\n' + '\n'.join([func.declaration for func in self._CanRxSignalGetters]))
        return '\n\n'.join(function_declarations)

class CanRxSourceFileGenerator(CanRxFileGenerator):
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
        self.__CanRxMsgs_MessageSpecific = list(
            Struct('CanRxMsg_%s' % msg.snake_name,
                   [
                       StructMember('struct CanMsgs_%s_t' % msg.snake_name,
                                    'payload', 'NULL'),
                       StructMember('SemaphoreHandle_t', 'xSemaphore', 'NULL'),
                       StructMember('StaticSemaphore_t', 'xMutexBuffer', 'NULL')
                   ],
                   'Enforce thread-safe access for %s' % msg.snake_name.upper()
                   ) for msg in self._canrx_msgs)
        self.__CanRxMsgs = Struct(
            'CanRxMsgs',
            [StructMember('struct CanRxMsg_%s' % msg.snake_name,
                          msg.snake_name, 'INIT_CANRX_MESSAGE()') for
             msg in self._canrx_msgs],
            'CAN RX Messages')

    def __generateHeaderIncludes(self):
        header_names = ['<sched.h>',
                        '<string.h>',
                        '<FreeRTOS.h>',
                        '<semphr.h>',
                        '"auto_generated/%s"' % self._output_name.replace('.c', '.h'),
                        '"auto_generated/CanMsgs.h"',
                        '"SharedAssert.h"',
                        '"SharedCan.h"']

        return '\n'.join(
            [HeaderInclude(name).get_include() for name in header_names])

    def __generateTypedefs(self):
        typedefs = []
        typedefs.extend(struct.declaration for struct in
                        self.__CanRxMsgs_MessageSpecific)
        typedefs.append(self.__CanRxMsgs.declaration)
        return '\n'.join(typedefs)

    def __generateMacros(self):
        macros = []
        macros.append(Macro(
            'INIT_CANRX_MESSAGE()',
            '{.payload={0}, .xSemaphore=NULL, .xMutexBuffer={{NULL}}}',
            'Initialize a CAN RX message').declaration)
        return '\n'.join(macros)

    def __generateVariables(self):
        variables = []
        variables.append(
            self.__CanRxMsgs.get_definition(self._RxTableName,
                                                   'Table of CAN RX messages'))
        variables.append(self._canrx_initialized.get_definition())
        return '\n\n'.join(variables)

    def __generatePrivateFunctionDeclarations(self):
        func_declarations = []
        func_declarations.append(self._InitPeriodicCanRxMutex.declaration)
        return  '\n'.join(func_declarations)

    def __generatePrivateFunctionDefinitions(self):
        function_defs = []
        function_defs.append(self._InitPeriodicCanRxMutex.definition)
        return '\n'.join(function_defs)

    def __generateFunctionDefinitions(self):
        function_defs = []
        function_defs.append(self._Init.definition)
        function_defs.append(self._CanRxReadMessageIntoTableFromISR.definition)
        function_defs.append(self._CanRxReadMessageIntoTableFromTask.definition)
        function_defs.extend([func.definition for func in self._CanRxSignalGetters])
        return '\n\n'.join(function_defs)
