# OpenNI_practice

GENERATED_HTML = generated_html

all :
	cd src/ && $(MAKE)
	$(MAKE) html

clean :
	cd src/ && $(MAKE) clean
	$(RM) -rf $(GENERATED_HTML)

html : $(GENERATED_HTML)/index.html $(wildcard dox/*)

$(GENERATED_HTML)/index.html : Doxyfile $(wildcard dox/* src/*)
	doxygen
