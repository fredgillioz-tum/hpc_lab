#!/bin/bash

for i in {1..500}; do
	# Measure performance without valgrind overhead.
	./a.out $i
	valgrind --tool=cachegrind ./a.out $i silent 2> cachegrind.txt
	grep -o -P '(?<=D1  miss rate:)[^%]*' cachegrind.txt | xargs >> logfile_dgemm.txt
	rm cachegrind.txt
done
