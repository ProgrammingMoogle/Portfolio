/******************************************************************************/
/*!
\file   matrix-thread.c
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS180
\par    Assignment #4
\date   11/9/2017
\brief  
  The implementation of the thread info for it to be used in main.

*/
/******************************************************************************/

#include <stdlib.h>        /* malloc, free      */
#include "matrix-thread.h" /* struct            */

/******************************************************************************/
/*!
  \brief
    Performs matrix multiplication and stores it in resulting matrix.
    
  \param input
    what is being given to be multiplied.
    
*/
/******************************************************************************/
void *multiply(void *input)
{
  int count;        /* counter                            */
  int *row;         /* stores row data                    */
  int *col;         /* stores column data                 */
  int width;        /* width                              */
  int prod = 0;     /* product                            */
  int rowOp;        /* row being worked on this thread    */
  int colOp;        /* column being worked on this thread */
  
  /* cast parameter to useful struct */
  threadData *thread = (threadData *)input;
  
  /* record width of matrix */
  width = thread->width;
  
  /* record the row and column */
  rowOp = thread->row;
  colOp = thread->col;
  
  /* allocate memory for the rows and columns */
  row = malloc(sizeof(int) * width);
  col = malloc(sizeof(int) * width);
  
  /* fill in row matrix */
  for(count = 0; count < width; count++)
  {
    row[count] = thread->mat[width * rowOp + count];
  }
  
  /* fill in column array */
  for(count = 0; count < width; count++)
  {
    col[count] = thread->mat[width * count + colOp];
  }
  
  /* ITS MULTIPLYING! */
  for(count = 0; count < width; count++)
  {
    prod += row[count] * col[count];
  }
    
  /* STORING DATA */
  thread->resultMat[width * rowOp + colOp] = prod;
  
  /* I"M FREE! WHEEEEEEE! */
  free (col);
  free (row);
  
  /* -hands over nothing- here you go! */
  return NULL;
}
