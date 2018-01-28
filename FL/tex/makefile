all: main.pdf

%.pdf: %.tex boilerplate.tex header.tex
	pdflatex --halt-on-error $^

clean:
	rm *.aux *.log *.out
