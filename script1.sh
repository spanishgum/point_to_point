#!/bin/bash

cp main2.txt main
make fresh >> independent.sh
for f in 0..4
do
	for i in 0..3
	do	
		main i >> independent.txt
	done
done
