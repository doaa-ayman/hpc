#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <omp.h>
int main(int argc, char** argv) { 
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   int totalsum=0,sum=0;
    if (rank == 0) {
        int arrsize;
  printf("Hello from master process.\n Number of slave processes is %d\nPlease enter the size of the array :\n", size - 1);
        scanf("%d", &arrsize); 
	printf("enter the elements of the array:\n ");
 	int arr[arrsize];
	for (int i =0;i<arrsize;i++){
	scanf("%d",&arr[i]);
}
        int numSlaves = size - 1;
        int smlSize = arrsize / numSlaves;
        int remaining = arrsize % numSlaves;
 	for (int i=1;i<= numSlaves;i++){
            MPI_Send(&smlSize, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&arr[(i-1)*smlSize], smlSize, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
       if (remaining!=0) {
 	int st= numSlaves*smlSize;
for (int i=st;i<arrsize;i++){
sum+=arr[i];
}
 }
    }  
     else {
        int sml;
        MPI_Recv(&sml, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	int arr[sml];
        MPI_Recv(arr, sml, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       
       #pragma omp parallel for reduction(+:sum)
         for (int i = 0 ;i<sml ;i++){ 
	sum+=arr[i]; }}
MPI_Reduce(&sum,&totalsum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

      if (rank==0){
printf ("the total sum of the array is %d\n",totalsum);
}  

    MPI_Finalize();
    return 0;
}

