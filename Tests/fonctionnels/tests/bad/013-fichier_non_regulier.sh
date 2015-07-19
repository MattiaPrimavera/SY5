#!/bin/bash

#initial system configuration
IMP_PATH="Objects/tubeServeur"
export IMP_PATH

mkdir -p Objects/dir_test

./Tests/fonctionnels/tests/start_system.sh
./Objects/mpr -P francino Objects/dir_test Objects/tubeClient &> Objects/res_client

cat Objects/res_client | grep "error: fichier non regulier" &> /dev/null

if [ "$?" -eq "0" ]; then
	exit 1
fi

rm Objects/res_client
rmdir Objects/dir_test
