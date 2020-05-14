#!/bin/bash
#
#PBS -N poissonSORRB
#PBS -l walltime=01:59:00
#PBS -l nodes=1:r662:ppn=48
#PBS -q mei

module purge
module load gcc/7.2.0

cd $PBS_O_WORKDIR

make

cd ./results/

echo "\nGrid_Points,Number_of_Threads,Tolerance,Iteration,Execution_Time">> resultados_poisson.csv

cd ..

# i -> Nº Grid Points
# j -> Nº Threads
# z -> Tolerance
for i in 100 500 1000 2000 5000; do
	echo "Para N:"
	for j in 1 4 8 16 24 32; do
		echo "Para Threads:"
		./poisson $i $j
	done
done
echo "Finished."