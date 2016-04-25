#!/bin/sh

cp main1.txt main.cpp
make fresh >> together.txt

for i in 0..4
do
	main >> together.txt
done
