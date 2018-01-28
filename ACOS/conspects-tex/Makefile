TARGET: akos-seminars.pdf akos-lectures.pdf

all: TARGET

akos-seminars.pdf: akos-seminars.tex
	#latexmk -pdf -pdflatex="pdflatex --shell-escape -interaction=nonstopmode" -use-make akos.tex
	pdflatex -shell-escape akos-seminars

akos-lectures.pdf: akos-lectures.tex
	#latexmk -pdf -pdflatex="pdflatex --shell-escape -interaction=nonstopmode" -use-make akos-lectures.tex
	pdflatex -shell-escape akos-lectures
clean:
	latexmk -CA
	rm -rf _minted* *.pyg
	
