#!/bin/bash

cp main2.txt main
make fresh >> independent.txt

for j in 10 50 100 500 1000
do
	for f in 0..4
	do
		for i in 0..3
		do	
			main i j >> independent.txt
		done
	done
done
