#!/bin/bash
#//////////////////////////////////////////////////////
#	Created by Nan Mihai on 21.05.2022
#	Copyright (c) 2022 Nan Mihai. All rights reserved.
#	Checker Tema 3 - Structuri de date
#	Facultatea de Automatica si Calculatoare
#	Anul Universitar 2021-2022, Seria CD
#/////////////////////////////////////////////////////
print_header()
{
	header="${1}"
	header_len=${#header}
	printf "\n"
	if [ $header_len -lt 71 ]; then
		padding=$(((71 - $header_len) / 2))
		for ((i = 0; i < $padding; i++)); do
			printf " "
		done
	fi
	printf "= %s =\n\n" "${header}"
}

result1=0
result2=0
result3=0
result=0
depunctari=0
ZERO=0
VALGRIND="valgrind --leak-check=full --track-origins=yes -q --log-file=rezultat_valgrind.txt"
BEST="100"
TESTS=("test0.in" "test0.in" "test1.in" "test1.in" "test2.in" "test2.in" "test3.in" "test3.in" "test4.in" "test4.in")
NODES=("B" "H" "C" "D" "I" "E" "V3" "V5" "L" "C")
KVALUES=(3 4 4 3 2 4 5 3 2 5)

print_header "Testare - Tema 3 (SD-CD)"

if test -f "README"; then
	echo "+0.0: Verificare README ................................................. PASS"
	result=$(awk "BEGIN {print $result+10; exit}")
else
	echo "-10.0: Verificare README ................................................ FAIL"
fi
make build > /dev/null 2>&1

#Cerința 1 - Verificarea ciclurilor în graf
echo "Cerința 1 - Verificarea ciclurilor"
for i in {0..9}
do
	fileIn="teste/test"$i".in"
	fileRef="teste/test"$i"_1.ref"
	fileOut="blockdag.out"
	cp "$fileIn" "blockdag.in"
        rm $fileOut > /dev/null 2>&1
	timeout 20 ./blockdag -c1 > /dev/null 2>&1
	diff $fileOut $fileRef > /dev/null
	EXIT_CODE=$?
	if (( i < 10 )); then
		idx=" $i"
	else
		idx=$i
	fi
	if [ $EXIT_CODE -eq $ZERO ] 
	then
		echo "+0.0: Cerința 1 Test $idx ................................................. PASS"
		result=$(awk "BEGIN {print $result+3; exit}")
		result1=$(awk "BEGIN {print $result1+3; exit}")
	else
		echo "-3.0: Cerința 1 Test $idx ................................................. FAIL"
	fi
done

#Cerința 2 - Tipurile nodurilor
echo "Cerința 2 - Tipurile nodurilor"
for i in {0..9}
do
	fileIn="teste/"${TESTS[$i]}
	fileOut="blockdag.out"
	fileRef="teste/test"$i"_2.ref"
	cp "$fileIn" "blockdag.in"
	timeout 20 ./blockdag -c2 ${NODES[$i]} > /dev/null 2>&1
	diff $fileOut $fileRef > /dev/null
	EXIT_CODE=$?
	if (( i < 10 )); then
		idx=" $i"
	else
		idx=$i
	fi
	if [ $EXIT_CODE -eq $ZERO ] 
	then
		echo "+0.0: Cerința 2 Test $idx ................................................. PASS"
		result=$(awk "BEGIN {print $result+3; exit}")
		result2=$(awk "BEGIN {print $result2+3; exit}")
	else
		echo "-3.0: Cerința 2 Test $idx ................................................. FAIL"
	fi
done

#Cerința 3 - Maximum K-Cluster
echo "Cerința 3 - Maximum K-Cluster"
for i in {0..9}
do
	fileIn="teste/"${TESTS[$i]}
	fileOut="blockdag.out"
	fileRef="teste/test"$i"_3.ref"
	cp "$fileIn" "blockdag.in"
	timeout 60 ./blockdag -c3 ${KVALUES[$i]} > /dev/null 2>&1
	EXIT_CODE=$(comm -13 <(sort -u $fileRef) <(sort -u $fileOut))
	if (( i < 10 )); then
		idx=" $i"
	else
		idx=$i
	fi
	if [ -s "$fileOut" ]
	then
		if [[ "$EXIT_CODE" == "" ]] 
		then
			echo "+0.0: Cerința 3 Test $idx ................................................. PASS"
			result=$(awk "BEGIN {print $result+3; exit}")
			result3=$(awk "BEGIN {print $result3+3; exit}")
		else
			echo "-3.0: Cerința 3 Test $idx ................................................. FAIL"
		fi
	else
		echo "-3.0: Cerința 3 Test $idx ................................................. FAIL"
	fi
done

echo "Verificarea dealocării memoriei"
fileIn="teste/test9.in"
fileOut="blockdag.out"
cp "$fileIn" "blockdag.in"
$VALGRIND ./blockdag -c1 > /dev/null 2>&1
if [[ -z $(cat rezultat_valgrind.txt) ]]; then
	printf "+0.0: VALGRIND Cerința 1 ................................................ PASS\n"
else
	depunctare=$(echo "scale=1; -($result1/10)" | bc -l)
	printf -- "$depunctare: VALGRIND Cerința 1 ................................................ FAIL\n"
	result=$(awk "BEGIN {print $result-($result1/10); exit}")
	depunctari=$(awk "BEGIN {print $depunctari+($result1/10); exit}")
fi
fileIn="teste/test3.in"
fileOut="blockdag.out"
cp "$fileIn" "blockdag.in"
$VALGRIND ./blockdag -c2 V3 > /dev/null 2>&1
if [[ -z $(cat rezultat_valgrind.txt) ]]; then
	printf "+0.0: VALGRIND Cerința 2 ................................................ PASS\n"
else
	depunctare=$(echo "scale=1; -($result2/10)" | bc -l)
	printf -- "$depunctare: VALGRIND Cerința 2 ................................................ FAIL\n"
	result=$(awk "BEGIN {print $result-($result2/10); exit}")
	depunctari=$(awk "BEGIN {print $depunctari+($result2/10); exit}")
fi
$VALGRIND ./blockdag -c3 3 > /dev/null 2>&1
if [[ -z $(cat rezultat_valgrind.txt) ]]; then
	printf "+0.0: VALGRIND Cerința 3 ................................................ PASS\n"
else
	depunctare=$(echo "scale=1; -($result3/10)" | bc -l)
	printf -- "$depunctare: VALGRIND Cerința 3 ................................................ FAIL\n"
	result=$(awk "BEGIN {print $result-($result3/10); exit}")
	depunctari=$(awk "BEGIN {print $depunctari+($result3/10); exit}")
fi

echo "Rezultate"
printf "Cerința 1 : "$result1"\n"
printf "Cerința 2 : "$result2"\n"
printf "Cerința 3 : "$result3"\n"
printf "Depunctări: "$depunctari"\n"
if awk "BEGIN {exit !($result < $BEST)}"; then
	echo "Ai acumulat "$result"p din maxim 100p! :("
else
	echo "Felicitări! Ai punctajul maxim: "$BEST"p! :)"
fi
make clean > /dev/null 2>&1
