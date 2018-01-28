import time
import functools
import sys


def profiler(func):
    recursion_depth = 0
    calls_counter = 0

    @functools.wraps(func)
    def decorated(*args, **kwargs):
        nonlocal recursion_depth, calls_counter

        calls_counter += 1
        recursion_depth += 1
        start_time = time.time()
        result = func(*args, **kwargs)
        recursion_depth -= 1

        if recursion_depth == 0:
            decorated.calls = calls_counter
            decorated.last_time_taken = time.time() - start_time
            calls_counter = 0
        return result

    decorated.last_time_taken = 0
    decorated.calls = 0
    return decorated


"""@profiler
def task(x):
    print("lol")
    if x > 0:
        time.sleep(0.1)

        return task(x - 1)+task(x - 1)
    else:
        return 1

start_time = time.time()
task(3)
print(task.last_time_taken)
print(task.calls)
print(time.time() - start_time)

start_time = time.time()
task(3)
print(task.last_time_taken)
print(task.calls)
print(time.time() - start_time)"""

exec(sys.stdin.read())
