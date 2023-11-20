import parse_expression

class Entry_point:
    def __init__(self):
        # Initialize the signal dictionairy from stored signals
        self.signal_expression_dict = {}
        self.signal_dict = {}# Probably be a dataframe?

    def write_to_sig_database(self):
        pass

    def get_signal(self, signal):
        return self.signal_dict.get(signal, None)
