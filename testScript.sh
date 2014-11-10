#!/bin/bash

for i in 1 2 3 4 5 6 7 8 9
do
	for j in 1 2 3 4 5 6 7
	do
		for k in 1 2 3 4 5
		do
			for l in 1 2 3
			do
				for m in 1
				do
					./NandC $i$j$k$l$m
				done
			done
		done
	done
done

