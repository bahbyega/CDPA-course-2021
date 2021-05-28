DOCNAME=main

.ONESHELL:

all: report

.PHONY: clean

report:
	@echo 'Report generation has started.'
	cd images
	@echo 'Figures generation has started.'
	python draw.py
	@echo 'Figures generation has finished.'
	cd ../
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