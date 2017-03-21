import sys
import argparse
import re


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


def process_paragraph(line, paragraph_spaces, line_length):
    def is_punctuation_marks(s):
        for c in s:
            if c not in {",", ".", "?", "!", "-", ":", "'"}:
                return False
        return True

    def is_single_piece(s):
        return s != ' ' and not is_punctuation_marks(s)

    words = re.findall(r'[A-Za-z0-9]+|[,.?!\-:\']', line)

    new_arr = ['']
    for word in words:
        if is_punctuation_marks(word):
            new_arr[-1] += word
        else:
            new_arr.append(word)
    words = new_arr

    out_text = ''
    out_line = ' ' * paragraph_spaces
    cur_len = paragraph_spaces

    for word in words:
        if len(word) > line_length:
            raise Exception('Too long')
        if cur_len + len(word) >= line_length:
            out_text += out_line
            out_text += '\n'
            out_line = word
            cur_len = len(word)
        else:
            if is_single_piece(word) and cur_len != paragraph_spaces:
                cur_len += 1
                out_line += ' '
            cur_len += len(word)
            out_line += word

    out_text += out_line
    out_text += '\n'

    return out_text


def main():
    parser = argparse.ArgumentParser(description='Text formatter')
    parser.add_argument('--input', '-i', metavar='filename', type=str,
                        default=None)
    parser.add_argument('--output', '-o', metavar='filename', type=str,
                        default=None)
    parser.add_argument('--line-length', '-l', metavar='w', type=int,
                        required=True)
    parser.add_argument('--paragraph-spaces', '-p', metavar='b', type=int,
                        required=True)

    args = parser.parse_args()

    if args.paragraph_spaces > args.line_length:
        raise Exception("Paragraph spaces greater than line length")

    input_stream = sys.stdin if args.input is None else open(args.input, 'r')
    output_stream = sys.stdout if args.output is None else open(args.output,
                                                                'w')
    paragraphs = get_paragraphs(input_stream)
    for paragraph in paragraphs:
        to_append = process_paragraph(paragraph,
                                      args.paragraph_spaces,
                                      args.line_length)
        output_stream.write(to_append)

    input_stream.close()
    output_stream.close()


main()
