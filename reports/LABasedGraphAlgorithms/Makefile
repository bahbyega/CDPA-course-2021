DOCNAME=main

all: report

.PHONY: clean

report:
	@echo 'PDF generation has started.'
	@echo '--------------------------First step.--------------------------'
	pdflatex $(DOCNAME).tex
	@echo '--------------------------Bibliography generation.--------------------------'
	bibtex $(DOCNAME).aux
	@echo '--------------------------Second step.--------------------------'
	pdflatex $(DOCNAME).tex
	pdflatex $(DOCNAME).tex
	@echo 'Report compilation has finished successfully.'

clean:
	rm *.blg *.bbl *.aux *.log *.out