import sys


class Stack:
    lst = []

    def __init__(self, obj):
        self.lst = [element for element in obj]

    def push(self, element):
        self.lst.append(element)

    def pop(self):
        return self.lst.pop()

    def top(self):
        return self.lst[-1]

    def __len__(self):
        return len(self.lst)

    def __str__(self):
        return ' '.join([str(element) for element in self.lst])


exec(sys.stdin.read())
