/******************************************************************************/
/*!
\file   matrix-thread.h
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS180
\par    Assignment #4
\date   11/9/2017
\brief  
  The a struct containing info on the threads.

*/
/******************************************************************************/

  /*! struct to use when spawning threads */
typedef struct
{
  int row;          /*!< row being worked on     */
  int col;          /*!< column being worked on  */
  int width;        /*!< width of the matrix     */
  int *mat;         /*!< matrix to get data from */
  int *resultMat;   /*!< matrix that stores it   */
} threadData;


/*****************************************************************************/
/*!
  \brief
    Performs matrix multiplication and stores it in resulting matrix.
    
  \param input
    what is being given to be multiplied.
    
*/
/*****************************************************************************/
void *multiply(void *input);
