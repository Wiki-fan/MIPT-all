import sys
import functools


def takes(*args_types, **kwargs_types):
    def decorated_wrapper(func):
        @functools.wraps(func)
        def decorated(*args, **kwargs):
            for type, arg in zip(args_types, args):
                if not isinstance(arg, type):
                    raise TypeError
            for arg_name, arg_value in kwargs.items():
                if not isinstance(arg_value, kwargs_types[arg_name]):
                    raise TypeError
            return func(*args, **kwargs)

        return decorated

    return decorated_wrapper


exec(sys.stdin.read())
