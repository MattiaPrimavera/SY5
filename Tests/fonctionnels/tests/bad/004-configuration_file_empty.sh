#!/bin/bash

#initial system configuration
IMP_PATH="Objects/tubeServeur"
export IMP_PATH

touch Objects/config_file_try_n_1

./Objects/init_simulateurs -c "Objects/config_file_try_n_1" &> /dev/null
sim_impr_process=$(ps ax | grep sim_impr | wc -l)

if [ "$sim_impr_process" -eq "1" ]; then
	exit 1
fi

rm Objects/config_file_try_n_1
