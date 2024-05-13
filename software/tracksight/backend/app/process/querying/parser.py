from .AST import BinaryOperatorNode, NumberNode


class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.current_token = None
        self.next_token()

    def next_token(self):
        self.current_token = self.tokens.pop(0) if self.tokens else None

    def expression(self):
        node = self.term()
        while self.current_token and self.current_token[0] in ("PLUS", "MINUS"):
            operator = self.current_token[0]
            self.next_token()
            node = BinaryOperatorNode(node, operator, self.term())
        return node

    def term(self):
        node = self.factor()
        while self.current_token and self.current_token[0] in ("MULTIPLY", "DIVIDE"):
            operator = self.current_token[0]
            self.next_token()
            node = BinaryOperatorNode(node, operator, self.factor())
        return node

    def factor(self):
        token = self.current_token
        if token[0] == "NUMBER":
            self.next_token()
            return NumberNode(float(token[1]))
        elif token[0] == "LPAREN":
            self.next_token()
            node = self.expression()
            if not self.current_token or self.current_token[0] != "RPAREN":
                raise SyntaxError("Expected ')'")
            self.next_token()
            return node
        else:
            raise SyntaxError(f"Unexpected token {token[0]}")
