CC=gcc
CFLAGS= -pedantic -Wall -std=c99 -D _POSIX_C_SOURCE=200112L

OBJDIR := ../Objects


%: %.c
	$(CC) $(CFLAGS) $^ -o $(OBJDIR)/$@

	
	
ALL=print_demon mpr init_simulateurs sim_impr
all: $(ALL)

print_demon: print_demon.c Outils.c fifo.c List.c 
mpr: mpr.c Outils.c
init_simulateurs: init_simulateurs.c Outils.c fifo.c 
sim_impr: sim_impr.c Outils.c 

clean: 
	rm tube*

cleanall:
	rm -rf *~ $(OBJDIR)
