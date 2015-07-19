#!/bin/bash

#initial system configuration
IMP_PATH="tubeServeur"
export IMP_PATH

./Tests/fonctionnels/tests/start_system.sh
./Objects/mpr -P francino "./mpr.c" Objects/tubeClient &> Objects/res_client

cat Objects/res_client | grep "Message envoyé avec success" &> /dev/null

if [ "$?" -eq "0" ]; then
	exit 0
fi

rm Objects/res_client
