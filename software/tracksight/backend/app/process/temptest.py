from lexer import Lexer
from parser import Parser
from interpreter import Interpreter


expr = "5 + 3 - 2"
l = Lexer()
tok = l.generate_tokens(expr)
p = Parser(tok)
tree = p.expression()
inter = Interpreter()
print(inter.visit(tree))

