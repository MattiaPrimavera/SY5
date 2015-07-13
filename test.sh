#!/bin/bash 

echo "-------------------------------------------------------------"
echo "                  TEST FONCTIONNELS                          "
echo "-------------------------------------------------------------"
echo -e
./Tests/fonctionnels/test_maker.sh


#echo "-------------------------------------------------------------"
#echo "                  TEST UNITAIRES                             "
#echo "-------------------------------------------------------------"
#echo -e

#gcc Tests/unitaires/test_cunit.c -o test_cunit
#./Tests/unitaires/test_cunit

./Tests/fonctionnels/tests/cleaner.sh