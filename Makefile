make: matrix

matrix: matrixcalc.c
	gcc matrixcalc.c -l pthread -o matrix

