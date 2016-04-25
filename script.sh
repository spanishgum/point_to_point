#!/bin/sh
cp main1.txt main.cpp
for i in 0..4
do
	make fresh
	main > together.txt
done
