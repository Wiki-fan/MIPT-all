import sys


class ComplexNumber:
    r = 0.0
    i = 0.0

    def __init__(self, real=0, imaginary=0):
        self.r = real
        self.i = imaginary

    def __add__(self, other):
        return ComplexNumber(self.r + other.r, self.i + other.i)

    def __sub__(self, other):
        return ComplexNumber(self.r - other.r, self.i - other.i)

    def __mul__(self, other):
        return ComplexNumber(self.r * other.r - self.i * other.i,
                             self.r * other.i + self.i * other.r)

    def __truediv__(self, other):
        return ComplexNumber(
            (self.r * other.r + self.i * other.i) /
            (other.r ** 2 + other.i ** 2),
            (self.i * other.r - self.r * other.i) /
            (other.r ** 2 + other.i ** 2))

    def __str__(self):
        if self.r == self.i == 0.0:
            return '0.00'
        elif self.r == 0:
            return '%.2fi' % self.i
        elif self.i == 0:
            return '%.2f' % self.r
        else:
            r_part = '' if self.r == 0 else '%.2f' % self.r
            i_part = '' if self.i == 0 else '%.2fi' % abs(self.i)
            joiner = ' + ' if self.i > 0 else ' - '
            return r_part + joiner + i_part


if __name__ == "__main__":
    for line in sys.stdin.readlines():
        print(eval(line.strip()))
