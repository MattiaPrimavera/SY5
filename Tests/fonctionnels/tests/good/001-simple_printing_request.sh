#!/bin/bash

IMP_PATH="tubeServer"
export IMP_PATH

./Tests/fonctionnels/tests/start_system.sh #starting printing system 

echo "ciao" > Objects/fic_test_1
imprimante=$(cat fichierConfiguration | head -n 1 | cut -f 1 -d ' ')

./Objects/mpr -P $imprimante Objects/fic_test_1 Objects/tubeClient &> Objects/res_client
cat Objects/res_client | grep "Message envoyé avec success" &> /dev/null

if [ "$?" -eq "0" ]; then
	exit 0
fi 
