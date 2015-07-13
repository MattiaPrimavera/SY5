SOURCES := Sources
OBJDIR := Objects

make:
	mkdir -p $(OBJDIR)
	make -f makefile -C $(SOURCES) 

clean:
	rm $(OBJDIR)/tube*
	
	
cleanall: 
	make cleanall -f makefile -C $(SOURCES) 
