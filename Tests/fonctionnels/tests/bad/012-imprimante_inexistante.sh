#!/bin/bash

#initial system configuration
IMP_PATH="Objects/tubeServeur"
export IMP_PATH

./Tests/fonctionnels/tests/start_system.sh
./Objects/mpr -P franca Sources/mpr.c Objects/tubeClient &> Objects/res_client

cat Objects/res_client | grep "error: imprimante demandee' n'existe pas" &> /dev/null

if [ "$?" -eq "0" ]; then
	exit 1
fi 

rm Objects/res_client


