/******************************************************************************/
/*!
\file   cachelist.c
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS180
\par    Assignment #1
\date   9/20/2017
\brief  
  This is the implementation file for all functions prototyped in cachelist.h.

*/
/******************************************************************************/
#include <stdio.h>     /* printf          */
#include <stdlib.h>    /* malloc, exit    */
#include <string.h>    /* strncpy         */
#include "cachelist.h" /* cachelist stuff */

/******************************************************************************/
/*!

  \brief
    Creates a new node from a given value and label.
          
  \param value 
    A value given to be used in the new node.
    
  \param label
    A label given for the new node.
    
  \return
    returns the newly created node.

*/
/******************************************************************************/
static cl_node *make_node(int value, const char *label)
{
  /* Allocates memory based on the size of the node                       */
  cl_node *node = (cl_node *)malloc(sizeof(cl_node));
  /* if no memory was allocated, exit program                             */
  if (!node)
  {
    printf("Can't allocate new node.\n");
    exit(1);
  }
  
  /* Sets the value of node to the given var and the next pointer to null */
  node->value = value;
  node->next = NULL;

  strncpy(node->label, label, LABEL_SIZE - 1);
  node->label[LABEL_SIZE - 1] = 0;
  
  return node;
}

/******************************************************************************/
/*!

  \brief
    Dumps the entire list onto the terminal.
          
  \param list 
    the head of the list being dumped.

*/
/******************************************************************************/
void cl_dump(const cl_node *list)
{
  printf("==================\n");
  while (list)
  {
    printf("%4i: %s\n", list->value, list->label);
    list = list->next;    
  }
}

/******************************************************************************/
/*!

  \brief
    Finds the first occurence of a value and depending on if cache is on/off,
    moves the that node to the front of the list else it just returns the list.
          
  \param list 
    the head of the list being searched through.
    
  \param search_value
    the value being searched for.
  
  \param cache
    depending if it 1 or 0 caching will occur where we move the searched value
    to the front of the list.
    
  \param compares
    the number of times it takes to find the value being searched for.
    
  \return
    returns the list with searched value in the front of list or just gives 
    back the list without doing anything to it.

*/
/******************************************************************************/
cl_node *cl_find(cl_node *list, int search_value, int cache, int *compares)
{
  cl_node *step;

  step = list;

  *compares = 0;
  
  while(step != NULL)
  {
    if(step->value == search_value)
    {
      *compares+= 1;
      if(cache == TRUE)
      {
        list = cl_remove(list, search_value);
        list = cl_add_front(list, curr->value, curr->label);
        
        return list;
      }
      else
      {
        return list;
      }
    }
    step = step->next;
    *compares += 1;
  }
  return list;
}

/******************************************************************************/
/*!

  \brief
    Adds a node to the front of the list.
          
  \param list 
    the list getting a new node in the front.
    
  \param value
    the value for the node.
  
  \param label
    the label for the node
    
  \return
    returns the new list with the node in the front.

*/
/******************************************************************************/
cl_node *cl_add_front(cl_node *list, int value, const char *label)
{
  cl_node *front = 0;
  cl_node *temp;
  
  if(!list)
  {
    cl_node *temp;
    temp = make_node(value, label);
    list = temp;
    
    free(temp);
    
    return list;
  }
  temp = make_node(value, label);
  front = temp;
  free(temp);
  front->next = list;
  
  return front;
}

cl_node *cl_add_end(cl_node *list, int value, const char *label)
{
  cl_node *step;
  cl_node *back;
  cl_node *temp;
  
  if(!list)
  {
    temp = make_node(value, label);
    list = temp;
    
    free(temp);
    
    return list;
  }
  
  temp = make_node(value, label);
  back = temp;
  free(temp);
  
  step = list;
  
  while(step->next)
  {
    step = step->next;
  }
  
  step->next = back;
  
  return list;
}

cl_node *cl_remove(cl_node *list, int search_value)
{
  cl_node *curr;
  cl_node *prev;
  
  curr = list;
  prev = curr;
  
  if(list == NULL)
  {
    return list;
  }
  
  while(curr && curr->value != search_value)
  {
    prev = curr;
    curr = curr->next;
  }
  
  if(curr == list)
  {
    list = list->next;
    
    free(curr);
    
    return list;
  }
  
  if(curr && curr->value == search_value)
  {
    curr = curr->next;
    
    free(prev->next);
    
    prev->next = curr;
    
    return list;
  }
  return list;
}

cl_node *cl_insert_before(cl_node *list, int search_value, int value, 
                          const char *label)
{
  cl_node *curr;
  cl_node *prev;
  cl_node *toinsert;
  
  curr = list;
  prev = curr;
  
  while(curr && curr->value != search_value)
  {
    prev = curr;
    curr = curr->next;
  }
  
  if(curr == list)
  {
    toinsert = make_node(value, label);
    
    toinsert->next = list;
    
    list = toinsert;
    
    
    return list;
  }
  
  if(curr)
  {
    toinsert = make_node(value, label);
    
    prev->next = toinsert;
    toinsert->next = curr;
    
    return list;
  }
  else
  {
    return list;
  }
}

void cl_insert_after(cl_node *list, int search_value, int value, 
                     const char *label)
{
  cl_node *curr;
  cl_node *aftercurr;
  cl_node *toinsert;
  
  curr = list;
  while(curr && curr->value != search_value)
  {
    curr = curr->next;
  }
  
  if(curr)
  {
    toinsert = make_node(value, label);
    
    aftercurr = curr->next;
    
    curr->next = toinsert;
    
    toinsert->next = aftercurr;
  }
}

void cl_destroy(cl_node *list)
{
  cl_node *destroy;
  
  destroy = list;
  
  if(list == NULL)
  {
    return;
  }
 
 
  while(list)
  {
    destroy = list->next;
    free(list);
    list = destroy;
  }
}
