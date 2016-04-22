TARGET: akos.pdf akos-lectures.pdf

all: TARGET

akos.pdf: akos.tex
	#latexmk -pdf -pdflatex="pdflatex --shell-escape -interaction=nonstopmode" -use-make akos.tex
	pdflatex -shell-escape akos

akos-lectures.pdf: akos-lectures.tex
	#latexmk -pdf -pdflatex="pdflatex --shell-escape -interaction=nonstopmode" -use-make akos-lectures.tex
	pdflatex -shell-escape akos-lectures
clean:
	latexmk -CA
	rm -rf _minted* *.pyg
	
