import numpy as np

def test_important_ci():
    for _ in range(int(np.random.uniform(0, 10))):
        if np.random.uniform() > 0.9:
            raise UnluckyCIException("Major CI Failure")

class UnluckyCIException(Exception):
    pass
