class ASTNode:
    pass

class NumberNode(ASTNode):
    def __init__(self, value):
        self.value = value 

    def __repr__(self):
        return f"Number : {self.value}"

class BinaryOperatorNode(ASTNode):
    def __init__(self, left, op, right):
        self.left = left
        self.right = right
        self.op = op

    def __repr__(self):
        return f"{self.op}, left: {self.left}, right: {self.right}"
