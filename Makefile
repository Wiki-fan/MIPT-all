TARGET: akos.pdf

all: TARGET

akos.pdf: akos.tex
	latexmk -pdf -pdflatex="pdflatex --shell-escape -interaction=nonstopmode" -use-make akos.tex
	
clean:
	latexmk -CA
	rm -r _minted-akos
	
