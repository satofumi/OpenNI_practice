# OpenNI_practice

GENERATED_HTML = generated_html

all :
	cd src/ && $(MAKE)

clean :
	cd src/ && $(MAKE) clean

html : $(GENERATED_HTML)/index.html

$(GENERATED_HTML)/index.html : Doxyfile $(wildcard dox/* src/*)
	doxygen
