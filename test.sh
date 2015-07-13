#!/bin/bash 

echo "-------------------------------------------------------------"
echo "                  TEST FONCTIONNELS                          "
echo "-------------------------------------------------------------"
echo -e
./Tests/fonctionnels/test_maker.sh


echo "-------------------------------------------------------------"
echo "                  TEST UNITAIRES                             "
echo "-------------------------------------------------------------"
echo -e

gcc -ITests/unitaires/CUnit/include Tests/unitaires/test_cunit.c -LTests/unitaires/CUnit/lib -lcunit -o Tests/unitaires/test_cunit
./Tests/unitaires/test_cunit


echo -e
echo -e "Elimination Fichiers Temporaires ..."
./Tests/fonctionnels/tests/cleaner.sh
rm ./Tests/unitaires/test_cunit
make cleanall