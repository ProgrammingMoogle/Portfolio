/******************************************************************************/
/*!
\file   parent-matrix.c
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS180
\par    Assignment #3
\date   10/2/2017
\brief  
  This is the implementation file for parent-matrix that forks for child-matrix.

*/
/******************************************************************************/
#include <stdio.h>   /* fopen, fscanf, fclose */
#include <stdlib.h>  /* exit, malloc          */
#include <sys/shm.h> /* shmat, shmdt, shmctl  */
#include <unistd.h>  
#include <sys/wait.h>

/*****************************************************************************/
/*!

  \brief
    Gets matrix data from a file.
    
  \param filename
    Pointer to the file with data in it.
    
  \param width
    Width of the matrix
    
  \return
    Pointer to data.
    
*/
/*****************************************************************************/
int *get_matrix(const char *filename, int *width)
{
  int value, *matrix;
  FILE *fp;
  
    /* Open the file in text/translated mode */  
  fp = fopen(filename, "rt");
  if (!fp)
  {
    printf("Can't open file: %s\n", filename);
    exit(-1);
  }
  
    /* Read the width and allocate the matrix */
  fscanf(fp, "%d", width);
  matrix = malloc(*width * *width * sizeof(int));
  if (!matrix)
  {
    printf("Can't malloc matrix\n");
    fclose(fp);
    exit (-2);
  }
  
    /* Read the vaules and put in the matrix */
  while (!feof(fp))
  {
    int result = fscanf(fp, "%d", &value);
    if (result == -1)
      break;
    *matrix++ = value;
  }
  fclose(fp);
  
    /* Return the address of the matrix */
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
  int width;       /* width of the matrix           */
  int *matrix;     /* the matrix read in            */
  int *buff;       /* shared memory area            */
  char *childExe; /* name of the child exe          */
  char *args[6];   /* holds arguments for exec call */
  char args_1[8];  /* for args assignment           */
  char args_2[8];  /* for args assignment           */
  char args_3[8];  /* for args assignment           */
  char args_4[8];  /* for args assignment           */
  int *children;   /* array to hold children ids    */
  int shmid;       /* shared memory id              */
  int i, j;        /* loop counters                 */
  key_t key = 123; /* key for shmget                */  
  int shmgetSize;
  
  if (argc < 3)
  {
    printf("Insufficient parameters supplied\n");
    return -1;
  }

  /* name of the child executable */
  childExe = argv[2];
    
    /* read in matrix values from file */
    /* don't forget to free the memory */  
  matrix = get_matrix(argv[1], &width);
  
    /* print the matrix */
  print_matrix(matrix, width);
  
  /* calculate number of bytes to allocate */
  shmgetSize = (1 + width * width * 2) * sizeof(int);
    
    /* get shared memory */
  shmid = shmget(key, shmgetSize, 0600 | IPC_CREAT);
  
  if(shmid == -1)
  {
    printf("shmget failed\n");
    return 0;
  }
  
  /* attach shared memory */
  buff = (int *) shmat(shmid, NULL, 0);
  
  /* manually insert into shared memory */
  buff[0] = width;
  
    /* fill in matrix data */
  for(i = 0; i < width * width; i++)
  {
    buff[1 + i] = matrix[i];
  }
  
    /* allocate array to hold children ids */
  children = malloc(width * width * sizeof(int));
  
    /* check malloc */
  if(!children)
  {
    return 0;
  }
    /* zeroth element name of child executable */
  args[0] = child_exe;
  
   /* null terminate last element */
  args[5] = 0;
  
  
    /* Fork a child for each matrix entry       */
    /* May need an array to hold the child pids */
  for (i = 0; i < width; i++)
  {
    for(j = 0; j < width; j++)
    {
      children[i * width * j] = fork();
      
      if(children[i * width + j] == 0) 
      { 
          /* first element shared memory id */
        sprintf(args_1, "%d", shmid);
        args[1] = args_1;
        
          /* child number to store result */
        sprintf(args_2, "%d", i * width + j);
        args[2] = args_2;
        
          /* row to work on */
        sprintf(args_3, "%d", i);
        args[3] = args_3;
        
          /* column to work on */
        sprintf(args_4, "%d", j);
        args[4] = args_4;
        
          /* exec child program to perform multiplication */
        execv(childExe, args);
        
          /* if we got here exec failed */
        return 0;
      }
    }
    /* Do forking and stuff */
  
  }

  /* wait for children */
  for(i = 0; i < width * width; i++)
  {
    int state;
    
    waitpid(children[i], &state, 0);
    
    if(WIFEXITED(state))
    {
      continue;
    }
  }
  
    /* print matrix from shared buffer */
  print_matrix(buffer + 1 + width * width, width);
  
  
  /* cleanup */
  shmdt(buff);
  
  shmctl(shmid, IPC_RMID, 0);

  free(children);
  
  free(matrix);
  
  return 0;
}
