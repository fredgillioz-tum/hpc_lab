#!/bin/bash

nlist=(128, 256, 512, 768, 1024, 1280, 1536, 1792, 2048)
for n in "${nlist[@]}"; do
	blist=(8, 16, 32, 64, 256, 512, 768, 1024, 2048)
	for bs in "${blist[@]}"; do
		# Measure performance without valgrind overhead.
		./a.out $n nosilent $bs
		valgrind --tool=cachegrind ./a.out $n silent $bs 2> cachegrind.txt
		grep -o -P '(?<=D1  miss rate:)[^%]*' cachegrind.txt | xargs >> logfile_dgemm.txt
		rm cachegrind.txt
	done
done
