info_TEXINFOS = ooc.texinfo

html-local:
	cp *.png $(HTMLS)
	doxygen Doxyfile.linux

install-html-local:
	cp -r api ${docdir}

EXTRA_DIST = Doxyfile.linux ooc.html api ooc.pdf
