#!/bin/bash
# script receives name of folder with bin files (day1 for example) as argument
dir=$(printf "%s" $0 | sed -nr 's/([^\/]+)$//;p')
var_ex=`echo "$dir$1"`
var=$(echo $var_ex | sed -nr 's/^.\///;p')
if [ -z $1 ] 
then
	for i in {1..25} 
	do
		if [ -d day$i ]
		then
			day=`echo "day$i"`
			time $var_ex$day/first $var$day/input.txt
			time $var_ex$day/second $var$day/input.txt
		fi
	done
else
	time $var_ex/first $var/input.txt
	time $var_ex/second $var/input.txt
fi
