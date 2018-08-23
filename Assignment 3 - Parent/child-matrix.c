/******************************************************************************/
/*!
\file   child-matrix.c
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS180
\par    Assignment #3
\date   10/2/2017
\brief  
  The child-matrix being used for parent-matrix.c .

*/
/******************************************************************************/
#include <sys/shm.h> /* shmat, shmdt          */
#include <stdlib.h>  /* exit, malloc          */

/*****************************************************************************/
/*!

  \brief
    This is launched using exec from parent-matrix.c. The child will perform
    matrix multiplication on one row and column using the shared memory.
    
  \param argc
    Number of arguments given to the exectuable
    
  \param argv
    Array of the arguments passed to child
    
  \return
    Standard return 0 to tell the parent the child is done.
    
*/
/*****************************************************************************/

int main(int argc, char **argv)
{
  int *buff;                    /* shared memory */
  int width;                      /* widht of matrix */
  int *row, *column;              /* arrays to hold values from matrix */
  int i;                          /* loop counter */
  int result = 0;                 /* result of multiplications */
  int shmid = atoi(argv[1]);      /* get id of shared memory */
  int rowOp = atoi(argv[3]);     /* get row number to work on */
  int columnOp = atoi(argv[4]);  /* get column number to work on */
  int childNum = atoi(argv[2]);  /* child num to store answer */
  
    /* if there were no arguments just return */
  if(!argc)
  {
    return 0;
  }
    /* attach to shared memory made by parent */
  buff = (int *) shmat(shmid, NULL, 0);
  
    /* record width of matrix from buff */
  width = buff[0];
  
    /* allocate space for arrays */
  row = malloc(width * sizeof(int));
  column = malloc(width * sizeof(int));
  
    /* fill in the rows */
  for(i = 0; i < width; i++)
  {
    row[i] = buff[1 + width * rowOp + i];
  }
    /* fill in the column */
  for(i = 0; i < width; i++)
  {
    column[i] = buff[1 + (width * i) + columnOp];
  } 
    /* multiply row by column and store it */
  for(i = 0; i < width; i++)
  {
    result += row[i] * column[i];
  }
  
    /* put the result in shared memory */
  buff[1 + (width * width) + childNum] = result;
    
    /* detach from shared memory */
  shmdt(buff);
  
    /* free allocated arrays */
  free(row);
  free(column);
  
  return 0;
}
