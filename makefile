SOURCES := Sources
OBJDIR := Objects

make:
	mkdir -p $(OBJDIR)
	cp fichierConfiguration Objects/
	make -f makefile -C $(SOURCES)

clean:
	rm $(OBJDIR)/tube*

cleanall:
	make cleanall -f makefile -C $(SOURCES)
