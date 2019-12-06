#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

void work_division(int n, int p, int rank, int *out_first, int *out_last){
	int delta = n/p;
	int range_start = delta * rank;
	int range_end = delta * (rank + 1) - 1;
	if (rank < n%p){
		range_start += rank;
		range_end += rank + 1;
	}
	else{
		range_start += n%p;
		range_end += n%p;
	}	
	*out_first = range_start + 1;
	*out_last = range_end + 1;
}

int main(int argc, char **argv){
	int n = atoi(argv[1]), p, rank;
	unsigned long long *result = malloc(sizeof(unsigned long long) * ((n * (n + 1))/2));
	int start_range, end_range;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	work_division(((n * (n + 1))/2), p, rank, &start_range, &end_range);
	int count = 0, pre_j, pre_i = start_range, running = start_range, x = 1;
	while(running > n - (x - 1)){
		running -= n - (x - 1);
		pre_i += x;
		x++;
	}
	int _i = (pre_i - 1) / n + 1;
	pre_j = start_range + ((_i * (_i - 1))/2);
	int _j = (pre_i - 1)%n + 1;
	printf("rank: %d start_range: %d end_range: %d\n", rank, start_range, end_range);
	for (int j = _j; j <= n; j++){
		if (count > end_range - start_range){
			break;
		}	
		printf("-%d * %d = %d\n", _i, j, _i * j);
		count++;
	}
	
	_i++;
	for (int i = _i; i <= n; i++){
		for (int j = i; j <= n; j++){
			if (count > end_range - start_range){
				break;
			}	
			count++;
			//insert into tree
			printf("%d * %d = %d\n", i, j, i * j);
		}
		if (count > end_range - start_range){
			break;
		}
	}
	int start = (int)log2(p);
	for (int stage = p; stage > 0; stage--){
		if (rank > pow(2, stage) - 1){
			//send
		}
		else if(pow(2, stage - 1) - 1 < rank && rank <= pow(2, stage) - 1){
			//recv
		}
	}
	MPI_Finalize();
	return 0;
}