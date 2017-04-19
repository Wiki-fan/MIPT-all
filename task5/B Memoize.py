import functools

import sys


def memoize(func):
    @functools.wraps(func)
    def decorated(*args, **kwargs):
        try:
            all_arguments = (args, tuple(kwargs.items()))
            if all_arguments not in func.cached:
                result = func(*args, **kwargs)
                func.cached[all_arguments] = result
                return result
            else:
                return func.cached[all_arguments]
        except AttributeError:
            func.cached = dict()
            return decorated(*args, **kwargs)

    return decorated


exec(sys.stdin.read())
