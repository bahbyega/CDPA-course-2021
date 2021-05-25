DOCNAME=main

all: report

.PHONY: clean

report:
	@echo 'PDF generation is started.'
	@echo '--------------------------First step.--------------------------'
	pdflatex $(DOCNAME).tex
	@echo '--------------------------Bibliography generation.--------------------------'
	bibtex $(DOCNAME).aux
	@echo '--------------------------Second step.--------------------------'
	pdflatex $(DOCNAME).tex
	pdflatex $(DOCNAME).tex
	@echo 'Report compilation is finished successfully.'

view: report
	open $(DOCNAME).pdf

clean:
	rm *.blg *.bbl *.aux *.log *.out