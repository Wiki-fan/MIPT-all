all: lectures.pdf

%.pdf: %.tex boilerplate.tex header.tex
	pdflatex --halt-on-error -shell-escape $^

clean:
	rm *.aux *.log *.out
