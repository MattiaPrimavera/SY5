#!/bin/bash

#initial system configuration
IMP_PATH="Objects/tubeServeur"
export IMP_PATH

echo "ciao" > Objects/new_fic.txt
chmod 000 Objects/new_fic.txt

./Tests/fonctionnels/tests/start_system.sh
./Objects/mpr -P francino Objects/new_fic.txt Objects/tubeClient &> Objects/res_client
 
cat Objects/res_client | grep "error: pas de droits sur le fichier demande'" &> /dev/null

if [ "$?" -eq "0" ];
then
	exit 1
fi

rm Objects/res_client

