#!/bin/bash

#initial system configuration
IMP_PATH="tubeServer"
export IMP_PATH

./Tests/fonctionnels/tests/start_system.sh
./Objects/mpr 1 2 3 4 5 6 7 8 &> Objects/res_client

cat Objects/res_client | grep "Usage" &> /dev/null

if [ "$?" -eq "0" ]; then
	exit 1
fi

rm Objects/res_client
