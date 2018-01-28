import sys


def getToChar(s: str, c):
    pos = s.find(c)
    return s[:pos], s[pos + 1:]


def getInQuotes(s: str):
    temp, s = getToChar(s, '"')
    substr, s = getToChar(s, '"')
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


count = 0
lines = sys.stdin.readlines()
for line in lines:
    res_dict = parse_line(line)
    if res_dict["user_agent"] == 'Go-http-client/1.1':
        count += 1
        # print(res_dict)

print(count)
