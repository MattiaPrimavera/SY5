#!/bin/bash

#initial system configuration
IMP_PATH="Objects/tubeServeur"
export IMP_PATH

./Tests/fonctionnels/tests/start_system.sh

echo "ciao" > Objects/toPrint
./Objects/mpr -Z francino Objects/toPrint Objects/tubeClient &> Objects/res_client

cat Objects/res_client | grep "Usage" &> /dev/null

if [ "$?" -eq "0" ]; then
	exit 1
fi

rm Objects/res_client Objects/toPrint
