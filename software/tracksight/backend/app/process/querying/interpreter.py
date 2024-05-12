class Interpreter:
    def visit(self, node):
        method_name = f"visit_{type(node).__name__}"
        visitor = getattr(self, method_name)
        return visitor(node)

    def visit_NumberNode(self, node):
        return node.value

    def visit_SignalNode(self, node):
        if node.value:
            return node.value
        # To do look up the signal data
    
    def visit_BinaryOperatorNode(self, node):
        left = self.visit(node.left)
        right = self.visit(node.right)
        if node.op == "PLUS":
            return left + right
        elif node.op == "MINUS":
            return left - right
        elif node.op == "MULTIPLY":
            return left * right
        elif node.op == "DIVIDE":
            return left / right
