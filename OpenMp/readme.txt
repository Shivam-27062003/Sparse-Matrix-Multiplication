COL380 (Parallel and Distributed Programming)
Assignment 1
Shivam Singh
2020CS10383

Number of Approches: 5

Approch 1:
Idea-
    Make the nxn matrix and initiate the matrix with all 0 entries and then fill the block in the matrix which are present in input. And then use the most basic matrix multiplication algroithm in O(n^3) without multi-threading.
Why attempted- 
    The motive was to develop the base code so that it can be optimised further.
Result-
    This is very slow and may take more than 10 sec for n=1000 and on bigger matrces it fails to perfrom the computation.
Drawback-
    This implementation is very resource consuming both in terms of time and memory and it does not even use the concept of blocks to compute A.A.


Approch 2:
Idea-
    Using multi-threading in previous approch. 
Why attempted- 
    In order to make use of parallel computing in preious algroithm and attempting to make it faster.
Result-
    Using 48 cores can bring the runtime for n=1000 to less than 1 sec. But as we increase n from 1000 to 100000. It takes more than 10 sec.
Drawback-
    The implementation remains resource comsuming in terms of memory but we gained some speed and reduced the runtime. Also, it does not use the sparseness of the matrix.


Approch 3:
Idea-
    Using sparse matrix multiplication algroithm. The runtime for this algroithm with size of matrix nxn and sparse block mxm:
Why attempted-
    In order to use the sparseness of the matrix and develop the basic code for the sparse matrix multiplication.
Result-
    let  k = n/m, then:
        time = O(k^3xm^3) = O(n^3)
    Which is slow and may take more than 10 sec for n = 1000 and fail to compute the multiplication for bigger matrices.
Drawback-
    The time complexity is high at the same time the memory comsumption is also very high. 


Approch 4:
Idea-
    Using approch 3 but this time we will skip the computation for the blocks which have all 0 entry i.e. if there are two blocks a and b & atleast one of them is all zero valued then we can say the result will be all zero block without computation. 
Why attempted-
    In attemp to make approch 3 time efficient and  avoiding unnecessary computations happending in approch 3.
Result-
    This is faster than approch 3 on an average.
Drawback-
    In worst case the time complexity is O(n^3) same as previous one.


Final Approch:
Idea-
    Using approch 4 with parallel computation (i.e., using omp parallel tasks in multiplication). Made each block computation in the output matrix as an individual independent task I applied parallelism using openmp library. Assuming output matrix as matrix of blocks (each block is an element), so for each (i,j)th block we will create and independent task.
Why attempted-
    In order to make approch 4 faster by using multi-threading or parallelism and making the code memory efficient.
Result-
    After implementing the parallel task model the program was able to boost the runtime for n=100000 and m = 25 from 50 sec in preious approch to 8 sec in the final approch.


--Performance table--
_________________________________________________________________________________________
| Non-0 input blocks    | Non-0 output blocks   | 2 cores | 4 cores | 8 cores | 16 cores |
|_______________________|_______________________|_________|_________|_________|__________|
| 2^10                  | 2^12                  | 14.0587 | 08.0148 | 05.1061 | 03.0134  |
| 2^15                  | 2^17                  | 19.5635 | 10.2016 | 07.0781 | 04.1259  |
| 2^20                  | 2^22                  | 63.1046 | 34.0963 | 21.6425 | 13.6237  |
| 2^25                  | 2^27                  | 95.0015 | 50.2608 | 30.6505 | 23.9203  |
|_______________________|_______________________|_________|_________|_________|__________|

Scalability:
    For larger blocks it works very fine but as the size of block decreases parallelism in pairwise block multiplication decreases as the number of rows in block decreases. But at the same time number of blocks increases thus the number of independent tasks increases but the number of cores remain the same. So, overall effect is that as size of block increases the performance increases but after a certain threshold it decreases as there is a tradeoff between the number of blocks and the size of blocks.