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

echo "\nPontos da Grelha,Número de Threads,Tolerância,Iterações,Tempo de execução">> resultados.csv

cd ..

# i : pontos
# j : Threads

for i in 64 128 256 512 1024; do
	echo "Para N:"
	for j in 1 4 8 16 24 32; do
		echo "Para Threads:"
		./poisson $i $j
	done
done
echo "Finished."
