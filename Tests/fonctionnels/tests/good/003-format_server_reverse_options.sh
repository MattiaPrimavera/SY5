#!/bin/bash

#initial system configuration
IMP_PATH="tubeServeur"
export IMP_PATH


./Objects/init_simulateurs -c fichierConfiguration &> /dev/null &
./Objects/print_demon -c fichierConfiguration -t Objects/tubeServer &> /dev/null & 

if [ "$?" -eq 0 ];
then
	exit 0
fi


