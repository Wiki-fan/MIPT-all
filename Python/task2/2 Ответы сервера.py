import sys


def getToChar(s: str, c):
    pos = s.find(c)
    return s[:pos], s[pos + 1:]


def getInQuotes(s: str):
    temp, s = getToChar(s, '"')
    substr, s = getToChar(s, '"')
    temp, s = getToChar(s, ' ')
    return substr, s


def parse_line(s: str):
    d = {}

    d["ip"], s = getToChar(s, ' ')
    getToChar(s, ' ')  # two blank
    getToChar(s, ' ')
    d["time"], s = getToChar(s, ' ')
    d["request_string"], s = getInQuotes(s)
    d["response_code"], s = getToChar(s, ' ')
    d["response_size"], s = getToChar(s, ' ')
    d["referrer"], s = getInQuotes(s)
    d["user_agent"], s = getInQuotes(s)

    return d


code200 = 0
code300_309 = 0
code_other = 0
lines = sys.stdin.readlines()
for line in lines:
    res_dict = parse_line(line)
    if int(res_dict["response_code"]) == 200:
        code200 += 1
    elif 300 <= int(res_dict["response_code"]) <= 309:
        code300_309 += 1
    else:
        code_other += 1

print(code200)
print(code300_309)
print(code_other)
print(code200 + code300_309 + code_other)
