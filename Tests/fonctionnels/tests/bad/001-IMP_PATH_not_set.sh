#!/bin/bash
if [ -n $IMP_PATH ]; then
	unset IMP_PATH
fi

./Tests/fonctionnels/tests/start_system.sh #starting printing system 

#trying to send a printing request with IMP_PATH variable unset 
imprimante=$(cat "fichierConfiguration" | head -n 1 | cut -f 1 -d ' ')
echo "ciao" > fic_test_1
fichier_existante="fic_test_1"

./Objects/mpr -P $imprimante $fichier_existante Objects/tubeClient &> Objects/res_client

#if we find the IMP_PATH error line in mpr standard error
cat Objects/res_client | grep IMP_PATH | grep error &> /dev/null

if [ "$?" -eq "0" ];
then
	exit 1
else
	exit 0
fi

rm fic_test_1 res_client