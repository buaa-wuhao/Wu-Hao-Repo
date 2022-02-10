/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

void transfer_diagnoal_blkN(int* blk_src_addr, int* blk_dst_addr,int M,int N,int blkSz)
{
   int cache[8];
   for(int i=0;i<blkSz;i++) 
   {
       int k=0,old_i =i;;
       for(int j=0;j<blkSz;j++)
           cache[k++] = *(blk_src_addr+i*M+j);
       i++;
       for(int j=0;j<blkSz;j++)
           cache[k++] = *(blk_src_addr+i*M+j);

       k = 0;
       i=old_i;
       for(int j=0;j<blkSz;j++) 
           *(blk_dst_addr+j*N+i) = cache[k++];
       i++;
       for(int j=0;j<blkSz;j++) 
           *(blk_dst_addr+j*N+i) = cache[k++];
   }
   return;
}
void transfer_blkN(int* blk_src_addr, int* blk_dst_addr,int M,int N,int blkSz)
{
   for(int i=0;i<blkSz;i++) 
   {
       for(int j=0;j<blkSz;j++)
       {
            *(blk_dst_addr+j*N+i) =  *(blk_src_addr+i*M+j);
       }
   }
}

void transfer_diagnoal_blk8(int* blk_src_addr, int* blk_dst_addr,int M,int N)
{
   int cache[8];
   for(int i=0;i<8;i++) 
   {
       for(int j=0;j<8;j++)
           cache[j] = *(blk_src_addr+i*M+j);
       for(int j=0;j<8;j++) 
           *(blk_dst_addr+j*N+i) = cache[j];
   }
   return;
}
void transfer_blk8(int* blk_src_addr, int* blk_dst_addr,int M,int N)
{
   for(int i=0;i<8;i++) 
   {
       for(int j=0;j<8;j++)
       {
            *(blk_dst_addr+j*N+i) =  *(blk_src_addr+i*M+j);
       }
   }
}

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
void transpose_submit_32(int M, int N, int* A, int* B)
{
    int rows = N/8,cols = M/8;
    for(int i=0;i<rows*cols;i++)
    {
       int r = i/cols,c = i%cols;
       int* blk_src_addr = (int*)A + r*8*8*cols  + c*8;
       int* blk_dst_addr = (int*)B + c*8*8*rows  + r*8;
       if(r==c){
           //Diagonal Blocks need special handling.
           transfer_diagnoal_blk8(blk_src_addr,blk_dst_addr,M,N);
       }
       else{
           //each 8*8 block has 64 int , each int is 4 bytes
           transfer_blk8(blk_src_addr,blk_dst_addr,M,N);
       }
    }
    return;
}
void transpose_sub_blk(int blk8_rows,int blk8_cols, int M,int N,int* A,int* B)
{
    for(int i=0;i<blk8_rows;i++)
    {
        for(int j=0;j<blk8_cols;j++)
        {
            int* blk_src_addr = A + i*8*M  + j*8;
            int* blk_dst_addr = B + j*8*N  + i*8;
            if(i==j)
                transfer_diagnoal_blk8(blk_src_addr,blk_dst_addr,M,N);
            else
                transfer_blk8(blk_src_addr,blk_dst_addr,M,N);
        }
    }
}
void transpose_submit_64(int M, int N, int* A, int* B)
{
    /*
    transpose_sub_blk(4,4,M,N,A,B);
    transpose_sub_blk(4,4,M,N,A+64*4*8+8*4,B+64*4*8+8*4);    
    transpose_sub_blk(4,4,M,N,A+8*4,B+64*4*8);
    transpose_sub_blk(4,4,M,N,A+64*4*8,B+8*4);
    */
   for(int i=0;i<16;i++)
   {
       for(int j=0;j<16;j++)
       {
           if(j%2==0 && (j==i||j==i+1))
                transfer_diagnoal_blkN(A+i*M*4+j*4,B+j*M*4+i*4,M,N,4);
           else if(j%2 &&(j==i||j==i-1)){
                transfer_diagnoal_blkN(A+i*M*4+j*4,B+j*M*4+i*4,M,N,4);
           }
           else{
               //transfer_blkN(A+i*M*4+j*4,B+j*M*4+i*4,M,N,4);
               transfer_diagnoal_blkN(A+i*M*4+j*4,B+j*M*4+i*4,M,N,4);
           }
       }
   }		
   }

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit_wuhao(int M, int N, int A[N][M], int B[M][N])
{
    if(M==32)
        //transpose_submit_32(M,N,(int*)A,(int*)B);
        transpose_sub_blk(N/8,M/8,M,N,(int*)A,(int*)B);
    else if(M==64)
        transpose_submit_64(M,N,(int*)A,(int*)B);
    return;
}
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	/*
	if (M == N) {
		trans_square(M, N, A, B);
	} else {
		trans_diff(M, N, A, B);
	}
	*/
	int i, j, row_block, col_block;
	int diag = 0;
	int temp = 0;

	/*
	The access pattern for the defined problem sizes incorporate blocking; we define a sub-matrix of the matrix A with so	 me size b to be a square block. The outer-loops iterate across these block structures, with the two inner loops		  iterating through each block.
	*/

	if (N == 32) 
	{
		//Finding optimal block sizes included some guess-work; increased B-size for larger M & N when M == N proved less 		 //efficient. However, when M != N, i.e. rectangular matrix, larger B was the way to go.
		for (col_block = 0; col_block < N; col_block += 8) 
		{
			for (row_block = 0; row_block < N; row_block += 8) 
			{
			
				for (i = row_block; i < row_block + 8; i ++) 
				{

					for (j = col_block; j < col_block + 8; j ++) 
					{
						if (i != j) 
						{
							B[j][i] = A[i][j];
						}
						 else {
							//Reduce misses m < i*j in B by storing in temp instead of missing in B[j][i]
							temp = A[i][j];
							diag = i;
						}
					}
					
					//Transpose of a square-matrix has a unique property; no need to move elements on the diagonal. 
				
					if (row_block == col_block) 
					{
						//Misses in B reduced to m < i
						B[diag][diag] = temp;
					}
				}	
				
			}
		}

	} 

	else if (N == 64) 
	{

		//Iterate through matrix using column-major iteration over blocks
		for (col_block = 0; col_block < N; col_block += 4) 
		{
			for (row_block = 0; row_block < N; row_block += 4) 
			{
				//Iterate over each row using row-major iteration			
				for (i = row_block; i < row_block + 4; i ++) 
				{
					for (j = col_block; j < col_block + 4; j ++) 
					{
						if (i != j) 
						{
							B[j][i] = A[i][j];
						}
						else {
							//On the diagonal
							temp = A[i][j];
							diag = i;
						}
					}

					if (row_block == col_block) 
					{
						B[diag][diag] = temp;
					}
				}	
				
			}
		}
		

	} 
	else {
		
		//Iterate through matrix using column-major iteration over blocks
		for (col_block = 0; col_block < M; col_block += 16) 
		{
			for (row_block = 0; row_block < N; row_block += 16) 
			{		
				//Since our sizes are prime, not all blocks will be square sub-matrices
				//Consider corner-case when (row_block + 16 > N) => invalid access. Explicit check for i, j < n, m
				for (i = row_block; (i < row_block + 16) && (i < N); i ++) 
				{
					for (j = col_block; (j < col_block + 16) && (j < M); j ++) 
					{
					
						if (i != j) 
						{
							B[j][i] = A[i][j];
						}
						else 
						{
							temp = A[i][j];
							diag = i;
						}
					}

					if (row_block == col_block) {
						B[diag][diag] = temp;
					}

				}
			
	 		}
		}
	
	}

}


/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

