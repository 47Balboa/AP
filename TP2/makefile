poisson : poissonSORRB.o
	gcc poissonSORRB.o -fopenmp -o poisson -lm

poissonSORRB.o : poissonSORRB.c
	gcc -c poissonSORRB.c

clean:
	rm -r poisson
	rm -r poissonSORRB.o
