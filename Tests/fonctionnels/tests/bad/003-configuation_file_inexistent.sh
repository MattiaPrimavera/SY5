#!/bin/bash

#initial system configuration
IMP_PATH="tubeServeur"
export IMP_PATH

file="inexistent_configuration_file"
if [[ -f "inexistent_configuration_file" ]]; 
then
    exit 1
fi

./Objects/init_simulateurs -c "inexistent_configuration_file" &> /dev/null
sim_impr_process=$(ps ax | grep sim_impr | wc -l)

if [ "$sim_impr_process" -eq "1" ]; then
	exit 1
fi 

