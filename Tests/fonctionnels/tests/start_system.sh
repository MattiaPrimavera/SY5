#!/bin/bash
make -s

#starting printers simulators 
./Objects/init_simulateurs -c fichierConfiguration &> /dev/null &

#starting server
./Objects/print_demon -t Objects/tubeServeur -c fichierConfiguration &> /dev/null &
