#!/bin/bash

#initial system configuration
IMP_PATH="Objects/tubeServeur"
export IMP_PATH

./Objects/init_simulateurs -c fichierConfiguration &> /dev/null

./Objects/print_demon Objects/tubeServeur -c fileConfigurazione &> Objects/res_server 
cat Objects/res_server | grep "Usage Server :" &> /dev/null

if [ "$?" -eq "0" ]; then
	exit 1
fi 

rm Objects/res_server


