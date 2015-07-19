#!/bin/bash

#nettoyage fichiers temp et processus
res_grep=$(ls Objects | grep tube | wc -l)

if [ "$res_grep" -ne "0" ]; then
	make -s clean
fi

killall sim_impr &> /dev/null
killall print_demon &> /dev/null
