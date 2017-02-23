import sys


def get_to_char(s: str, c):
    pos = s.find(c)
    return s[:pos], s[pos + 1:]


def get_in_brackets(s: str, left_bracket, right_bracket):
    temp, s = get_to_char(s, left_bracket)
    substr, s = get_to_char(s, right_bracket)
    return substr, s[1:]


def get_in_chars(s: str, c: str):
    return get_in_brackets(s, c, c)


def parse_line(s: str):
    d = {}

    d["ip"], s = get_to_char(s, ' ')
    temp, s = get_to_char(s, ' ')  # two blank
    temp, s = get_to_char(s, ' ')
    d["time"], s = get_in_brackets(s, '[', ']')
    d["request_string"], s = get_in_chars(s, '"')
    d["response_code"], s = get_to_char(s, ' ')
    d["response_size"], s = get_to_char(s, ' ')
    d["referrer"], s = get_in_chars(s, '"')
    d["user_agent"], s = get_in_chars(s, '"')

    return d


os_count = {"Windows": 0, "Ubuntu": 0, "OS X": 0, "Unknown": 0}
lines = sys.stdin.readlines()
for line in lines:
    if line == '' or line == '\n':
        continue
    res_dict = parse_line(line)
    print(res_dict)
    if "Windows" in res_dict["user_agent"]:
        os_count["Windows"] += 1
    elif "Ubuntu" in res_dict["user_agent"]:
        os_count["Ubuntu"] += 1
    elif "Macintosh" in res_dict["user_agent"]:
        os_count["OS X"] += 1
    else:
        os_count["Unknown"] += 1

sorted_os_count = sorted(os_count.items(), key=lambda x: x[1])

for key, value in sorted_os_count:
    print("{}: {}".format(key, value))
