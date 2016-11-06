#!/bin/bash

nlist=(250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750)
for n in "${nlist[@]}"; do
	# Measure performance without valgrind overhead.
	./a.out $n
	valgrind --tool=cachegrind ./a.out $n silent 2> cachegrind.txt
	grep -o -P '(?<=D1  miss rate:)[^%]*' cachegrind.txt | xargs >> logfile_dgemm.txt
	rm cachegrind.txt
done
