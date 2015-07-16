#!/bin/bash


chmod -R +rwx Tests/fonctionnels/tests/ 

echo "-------------------------------------------------------------"
echo "Tests in /Tests/fonctionnels/tests/bad:"
echo "-------------------------------------------------------------"
for f in ./Tests/fonctionnels/tests/bad/*.sh; 
do 
	./Tests/fonctionnels/tests/cleaner.sh 
	./$f
	if [ "$?" -ne 0 ]; then
		echo "[ OK ] ------ $(basename $f)"
	else
		echo "[ FAIL ] ------ $(basename $f)"
	fi 
done


echo -e 
echo "/////////////////////////////////////////////////////////////"
echo -e

echo "-------------------------------------------------------------"
echo "Tests in ./tests/good:"
echo "-------------------------------------------------------------"
for f in ./Tests/fonctionnels/tests/good/*.sh; 
do 
	./Tests/fonctionnels/tests/cleaner.sh 
	./$f
	if [ "$?" -eq 0 ]; then
		echo "[ OK ] ------ $(basename $f)"
	else
		echo "[ FAIL ] ------ $(basename $f)"
	fi 
done


echo -e 

./Tests/fonctionnels/tests/cleaner.sh
	