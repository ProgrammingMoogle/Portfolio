/******************************************************************************/
/*!
\file   main-thread.c
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
#include <stdio.h>         /* printf, fopen, fclose, fscanf */
#include <stdlib.h>        /* exit, malloc                  */
#include <pthread.h>       /* thread create/join/exit       */
#include "matrix-thread.h" /* thread data struct, thread fn */

/*****************************************************************************/
/*!

  \brief
    Gets matrix data from a file.
    
  \param filename
    Pointer to the file with data in it.
    
  \param width
    Width of the matrix
    
  \return
    Pointer to matrix.
    
*/
/*****************************************************************************/
int *get_matrix(const char *filename, int *width)
{
  int value, *matrix, result;
  FILE *fp;
  
  fp = fopen(filename, "rt");
  if (!fp)
  {
    printf("Can't open file: %s\n", filename);
    exit(-1);
  }
  
  result = fscanf(fp, "%d", width);
  if (result == -1)
  {
    printf("Can't read from file: %s\n", filename);
    fclose(fp);
    exit(-1);
  }

  matrix = malloc(*width * *width * sizeof(int));
  if (!matrix)
  {
    printf("Can't malloc matrix\n");
    fclose(fp);
    exit (-2);
  }
  
  while (!feof(fp))
  {
    result = fscanf(fp, "%d", &value);
    if (result == -1)
      break;
    *matrix++ = value;
  }
  fclose(fp);
  return matrix - (*width * *width);
}

/*****************************************************************************/
/*!

  \brief
    Prints out the matrix.
    
  \param matrix
    the matrix getting printed.
    
  \param width
    width of the matrix.
    
*/
/*****************************************************************************/
void print_matrix(int *matrix, int width)
{
  int i, size = width * width;
  for (i = 0; i < size; i++)
  {
    printf("%8i", matrix[i]);
    if ( (i + 1) % width == 0)
      printf("\n");
  }
  printf("\n");
}

/*****************************************************************************/
/*!

  \brief
    Handles spawning all the children and waiting for them to be done.
    
  \param argc
    Number of arguments from cmd line
    
  \param argv
    Array of strings that are the arguments from the cmd line
    
  \return
    returns 0 to show that the parent is done.
    
*/
/*****************************************************************************/
int main(int argc, char **argv) 
{
  int i, j;
  int width;           /* width of the matrix           */
  int matsize;         /* total matrix values           */
  int *input_matrix;   /* the matrix read in            */
  int *result_matrix;  /* threads will put results here */
  threadData *threads; /* threads... hope there isn't any loose ones */
  pthread_t *threadID; /* they have IDs... okay. */
  
  if (argc < 2)
  {
    printf("Insufficient parameters supplied\n");
    return -1;
  }
  
    /* Reading the input matrix from a file into it's memory. */
  input_matrix = get_matrix(argv[1], &width);
  matsize = width * width;
  
    /* Printing the input matrix. */
  print_matrix(input_matrix, width);
  
  /* here an array for all those loose threads to put data into */
  threads = malloc(sizeof(threadData) * matsize);
  
  /* I need some storage space for those IDs... */
  threadID = malloc(sizeof(pthread_t) * matsize);
  
   /* Let's make sure they all fit in the results */
  result_matrix = malloc(sizeof(int) * width * width);
  
    /* Creating all of the other threads and supplying them with */
    /* their parameters                                          */ 
  for (i = 0; i < width; i++)
  {
    for(j = 0; j < width; j++)
    {
      /* fill in the data! */
      threads[i * width + j].row = i;
      threads[i * width + j].col = j;
      threads[i * width + j].width = width;
      threads[i * width + j].mat = input_matrix;
      threads[i * width + j].resultMat = result_matrix;
      pthread_create(&threadID[i * width + j], NULL, 
                     multiply, &threads[i * width + j]);
    }
  }

    /* Waiting for all of the threads to finish. */
  for(i = 0; i < matsize; i++)
  {
    pthread_join(threadID[i], NULL);
  }  
    
    /* Printing the resulting squared matrix. */
  print_matrix(result_matrix, width);
  
    /* Cleaning up any memory or resources the main thread created. */
  free(input_matrix);
  free(result_matrix);
  free(threadID);
  free(threads);
  
  
  return 0;
}
