res = 0
for i in range(0, 10):
    res += (-1) ** i / (2 * i + 1)
res *= 4
print(res)
