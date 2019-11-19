import os
from cantools.database.can.c_source import Message

HEADER_TEMPLATE = '''\
/**
 * @brief {comment}
 * @note This file is auto-generated for {sender}. !!! Do not modify !!!
 */
// clang-format off
#ifndef {header_guard}
#define {header_guard}

/******************************************************************************
 * Includes
 ******************************************************************************/
{headers}

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
{function_declarations}

#endif /* {header_guard} */
'''

SOURCE_TEMPLATE = '''\
/**
 * @note This file is auto-generated for {board}. !!! Do not modify !!!
 */
// clang-format off
/******************************************************************************
 * Includes
 ******************************************************************************/
{headers}

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/
{typedefs}

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/
{macros}

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
{variables}

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
{private_function_defs}

 /******************************************************************************
 * Private Function Declarations
 ******************************************************************************/
{private_function_decls}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
{function_defs}
'''

class CanFileGenerator:
    def __init__(self, database, output_path, board):
        self._database = database
        self.__board = board
        self._output_dir = os.getcwd() if os.path.dirname(output_path) is '' else os.path.dirname(output_path)
        self._output_name = os.path.basename(output_path)

    def _get_can_msgs(self):
        return list(msg for msg in map(Message, self._database.messages))

    def _write_output(self, text_output):
        # Generate output folder if it doesn't exist yet
        if not os.path.exists(self._output_dir):
            os.mkdir(self._output_dir)
        # Write file to disk
        with open(os.path.join(self._output_dir, self._output_name), 'w') as fout:
            fout.write(text_output)

    def _generateSource(self, headers, typedefs, macros, variables, function_defs,
                        private_function_defs='', private_function_decls=''):
        return SOURCE_TEMPLATE.format(
            board=self.__board, headers=headers, typedefs=typedefs, macros=macros,
            variables=variables, private_function_defs=private_function_defs,
            private_function_decls=private_function_decls,function_defs=function_defs)

    def _generateHeader(self, comment, header_guard, headers, function_declarations):
        return HEADER_TEMPLATE.format(
            comment=comment, sender=self.__board, header_guard=header_guard,
            headers=headers, function_declarations=function_declarations)

class Macro:
    def __init__(self, name, body, comment):
        self.name = name
        self.declaration = '''\
/**
 * @brief {comment}
 */
#define {name} {body}'''.format(comment=comment, name=name, body=body)

class Function:
    def __init__(self, signature, comment, body):
        if comment == '':
            self.declaration = '''\
{signature};'''.format(comment=comment, signature=signature)
        else:
            self.declaration = '''\
/** @brief {comment} */
{signature};'''.format(comment=comment, signature=signature)

        self.definition = '''\
{signature}
{{
{body}
}}'''.format(signature=signature, body=body)

class Struct:
    def __init__(self, name, members, declaration_comment):
        self.__name = name
        self.__definition_members = list("    .%s = %s," % (member.name, member.initial_val) for member in members)
        self.declaration = '''\
/** @brief {comment} */
struct {name}
{{
{members};
}};'''.format(comment = declaration_comment,
              name = name,
              members = ';\n'.join(list("    %s %s" % (member.type, member.name) for member in members)))
        self.__definition_template = '''\
/** @brief {comment} */
struct {name} {var_name} =
{{
{members}
}};'''

    def get_definition(self, var_name, comment):
        return self.__definition_template.format(
            comment=comment,
            name=self.__name,
            var_name=var_name,
            members='\n'.join(self.__definition_members))

class Enum:
    def __init__(self, name, members, declaration_comment):
        self.declaration = '''\
/** @brief {comment} */
enum {name}
{{
{members}
}};'''.format(comment=declaration_comment, name=name,
              members='\n'.join(members))

class HeaderInclude:
    def __init__(self, header_name):
        self.__name = header_name
    def get_include(self):
        return '#include %s' % self.__name

class StructMember:
    def __init__(self, type, name, initial_val='N/A'):
        self.type = type
        self.name = name
        self.initial_val = initial_val

class CanSignal:
    def __init__(self, type_name, signal_name_snakecase, msg_name_snakecase):
        self.type_name=type_name
        self.snakecase_name = signal_name_snakecase
        self.uppercase_name = signal_name_snakecase.upper()
        self.msg_name_snakecase = msg_name_snakecase
        self.msg_name_uppercase = msg_name_snakecase.upper()

class Variable:
    def __init__(self, type, name, init_val, comment):
        self.__name = name
        self.__definition = '''\
/** @brief {comment} */
{type} {name} = {init_val};'''.format(
            comment=comment, type=type, name=name, init_val=init_val)

    def get_definition(self):
        return self.__definition
    def get_name(self):
        return self.__name
