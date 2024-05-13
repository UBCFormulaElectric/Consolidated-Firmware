import re


class Lexer:
    TOKEN_REGEX = {
        "NUMBER": r"\d+(\.\d+)?",
        "PLUS": r"\+",
        "MINUS": r"-",
        "MULTIPLY": r"\*",
        "DIVIDE": r"\/",
        "LPAREN": r"\(",
        "RPAREN": r"\)",
    }

    def __init__(self):
        pass

    def generate_tokens(self, expr):
        tokens = []

        i = 0
        j = 0
        while i < len(expr) and j < 5:
            found_match = False
            for token_type, regex in self.TOKEN_REGEX.items():
                match = re.match(regex, expr[i:])
                if match:
                    found_match = True
                    token_value = match.group(0)
                    tokens.append((token_type, token_value))
                    i += len(token_value)
                    break
            if found_match is False:
                i += 1

        return tokens
