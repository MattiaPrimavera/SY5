#!/bin/bash
IMP_PATH="error_tube"
export IMP_PATH

./Tests/fonctionnels/tests/start_system.sh #starting printing system 

#trying to send a printing request with IMP_PATH variable unset 
imprimante=$(cat fichierConfiguration | head -n 1 | cut -f 1 -d ' ')

echo "ciao" > Objects/fic_test_1

./mpr -P $imprimante Objects/fic_test_1 Objects/tubeClient &> Objects/res_client
A=$?
rm Objects/res_client Objects/fic_test_1
exit $A 


 

