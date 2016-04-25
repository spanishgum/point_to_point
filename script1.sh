#!/bin/bash

cp main2.txt main

for f in 0..4
do
	for i in 0..3
	do
		make fresh
		main i > independent.txt
	done
done
