import sys
import argparse
import re

from enum import Enum

parser = argparse.ArgumentParser(description='Форматирователь текста.')
parser.add_argument('--input', '-i', metavar='filename', type=str,
                    default='stdin')
parser.add_argument('--output', '-o', metavar='filename', type=str,
                    default='stdout')
parser.add_argument('--line-length', '-l', metavar='w', type=int,
                    default='stdin')
parser.add_argument('--paragraph-spaces', '-p', metavar='b', type=int,
                    default='stdin')

args = parser.parse_args()
punctuation_marks = tuple(map(chr, {44, 46, 63, 33, 45, 58, 39}))

def is_punctuation_marks(s):
    for c in s:
        if c not in punctuation_marks:
            return False
    return True

def is_chars(s):
    return (s != ' ' and not is_punctuation_marks(s))

def get_paragraphs(inp):
    lines = inp.readlines()
    paragraphs = []
    cur_paragraph = ''
    for line in lines:
        if line != '\n':
            cur_paragraph += line.replace('\n', ' ')
        elif cur_paragraph != '':
            paragraphs.append(cur_paragraph)
            cur_paragraph = ''
    if cur_paragraph != '':
        paragraphs.append(cur_paragraph)
    return paragraphs


def process_paragraph(line):
    #arr = list(filter(lambda x: x != '', line.split(' ')))
    arr = re.findall(r'[A-Za-z0-9]+|[\,\.\?\!\-\:\']', line)
    #print (arr)

    i = 0
    while i <len(arr)-1:
        if (is_punctuation_marks(arr[i]) or is_chars(arr[i])) and is_punctuation_marks(arr[i+1]):
            arr[i] += arr[i+1]

            del arr[i+1]
            i += 2
        else:
            i += 1

    #print (arr)

    out = ''
    out_line = ' ' * args.paragraph_spaces
    cur_len = args.paragraph_spaces

    for word in arr:
        if len(word) > args.line_length:
            raise Exception('Too long')
        if cur_len + len(word) >= args.line_length:
            out += out_line
            out += '\n'
            out_line = word
            cur_len = len(word)
        else:
            if is_chars(word) and cur_len != args.paragraph_spaces:
                cur_len += 1
                out_line += ' '
            cur_len += len(word)
            out_line += word

    out += out_line
    out += '\n'

    return out


def main():
    inp = sys.stdin
    out = sys.stdout
    paragraphs = get_paragraphs(inp)
    for par in paragraphs:
        to_append = process_paragraph(par)
        out.write(to_append)

main()
