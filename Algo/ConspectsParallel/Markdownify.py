import sys

with open(sys.argv[1], "r") as fin:
    s = fin.read()
print (s)

to_delete = ['<section data-markdown>\n', '<script type="text/template">\n', '</script>\n','</section>\n',
             '<!-- Slide -->\n', '<section>\n','<div data-markdown>\n', '</div>\n', '<div style="text-align: center;">\n',
            '<!--\n', '-->\n']
to_replace = {'<pre><code data-trim>':'```', '</code></pre>':'```',
              '&lt;':'<', '&gt;':'>','###':'### ', '<img src="':'![](', '"/>':')'}
for frm, to in to_replace.items():
    s = s.replace(frm, to)
for what in to_delete:
    s = s.replace(what, '')
with open(sys.argv[1], "w") as fout:
    fout.write(s)

