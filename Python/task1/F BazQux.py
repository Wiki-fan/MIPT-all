def barbuzzify(x):
    if x % 15 == 0:
        return 'BazQux'
    elif x % 3 == 0:
        return 'Baz'
    elif x % 5 == 0:
        return 'Qux'
    else:
        return str(x)


arr = [x for x in range(1, 101)]
print(' '.join(map(str, arr)))
arr2 = list(map(barbuzzify, arr))
print(' '.join(arr2))
