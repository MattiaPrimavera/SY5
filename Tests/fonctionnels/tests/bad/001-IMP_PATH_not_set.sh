#!/bin/bash
if [ -n $IMP_PATH ]; then
	unset IMP_PATH
fi

./Tests/fonctionnels/tests/start_system.sh #starting printing system

#trying to send a printing request with IMP_PATH variable unset
imprimante=$(cat "fichierConfiguration" | head -n 1 | cut -f 1 -d ' ')
echo "ciao" > Objects/fic_test_1

./Objects/mpr -P $imprimante Objects/fic_test_1 Objects/tubeClient &> Objects/res_client

#if we find the IMP_PATH error line in mpr standard error
cat Objects/res_client | grep IMP_PATH | grep error &> /dev/null

if [ "$?" -eq "0" ];
then
	exit 1
else
	exit 0
fi

rm Objects/fic_test_1 Objects/res_client
