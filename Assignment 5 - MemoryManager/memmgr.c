/******************************************************************************/
/*!
\file   memmgr.c
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS180
\par    Assignment #5
\date   11/29/2017
\brief
  The implementation of a memory manager using policies: Best-Fit and 
  First-Fit.

*/
/******************************************************************************/

#include "memmgr.h"
#include <stdlib.h> /* malloc */
#include <string.h> /* strncpy */

/* tracks the policy globally */
static MMPolicy mPolicy;

/*****************************************************************************/
/*!
  \brief
    Allocates block of memory to use in the manager.

  \param bytes
    Size of block memory.

  \param policy
    Best fit or First fit.

  \return
    Pointer to the head of all the memory
*/
/*****************************************************************************/
block_info *mm_create(size_t bytes, enum MMPolicy policy)
{
  /* allocate head node */
  block_info *heap = malloc(sizeof(block_info));

  heap->allocated = 0;            /* not yet given memory */
  heap->size = bytes;             /* set size */
  heap->address = malloc(bytes);  /* allocate amount */
  strncpy(heap->label, "START", LABEL_SIZE);        /* set label */
  mPolicy = policy;                         /* save policy */
  heap->next = 0;                 /* set to null */

                                  /*check if malloced */
  if (heap->address == NULL)
  {
    return NULL;
  }
  /* return pointer to head node */
  return heap;
}

/*****************************************************************************/
/*!
  \brief
    Allocates node in the list based on policy.

  \param heap
    the list getting a new allocated block.

  \param bytes
    Size of memory for the block.

  \param label
    label for the node being allocated.

  \return
    Address of the newly allocated node.
*/
/*****************************************************************************/
void *mm_allocate(block_info *heap, size_t bytes, char *label)
{
  if (mPolicy == mmpFirstFit)
  {
    block_info *slice;

    /* walk the list */
    while (heap)
    {
      /* perfect match */
      if (heap->size == bytes && !heap->allocated)
      {
        /* set allocation flag to 1*/
        heap->allocated = 1;

        /* copy label desired */
        strncpy(heap->label, label, LABEL_SIZE);


        /* since this node was already allocated just no longer used,
        * the other fields will already have correct data */

        /* return address of this block */
        return heap->address;
      }

      /* found a chunk that will fit, but will have to slice in two */
      if (heap->size > bytes && !heap->allocated)
      {
        /* create a node to slice out the empty space */
        slice = malloc(sizeof(block_info));

        /* allocation flag set to 0 */
        slice->allocated = 0;

        /* calculate remaining size */
        slice->size = heap->size - bytes;

        /* calculate address */
        slice->address = heap->address + bytes;

        /* copy label free into empty block */
        strncpy(slice->label, "FREE", LABEL_SIZE);

        /* link up to list */
        slice->next = heap->next;

        /* now adjust heap node for allocation */


        /* allocation flag set to 1 */
        heap->allocated = 1;

        /* set number of bytes for this block */
        heap->size = bytes;

        /* copy label into the node */
        strncpy(heap->label, label, LABEL_SIZE);

        /* link up to list */
        heap->next = slice;

        /* return address of allocated node */
        return heap->address;
      }

      /* move pointer */
      heap = heap->next;
    }

    /* reached of of list with no suitable allocations
    * return null */
    return NULL;
  }
  else if (mPolicy == mmpBestFit)
  {
    size_t diff = -1;
    block_info *bestBlck = 0, *chop;

    while (heap)
    {
      /* found perfect match! */
      if (heap->allocated == 0 && heap->size == bytes)
      {
        /* set flag */
        heap->allocated = 1;

        /* copy label */
        strncpy(heap->label, label, LABEL_SIZE);

        /* return the address */
        return heap->address;
      }

      if (heap->allocated == 0 &&
        heap->size > bytes &&
        heap->size - bytes < diff)
      {
        diff = heap->size - bytes;
        bestBlck = heap;
      }

      heap = heap->next;
    }

    if (!bestBlck)
      return NULL;

    chop = malloc(sizeof(block_info));

    chop->allocated = 0;

    chop->size = bestBlck->size - bytes;

    chop->address = bestBlck->address + bytes;

    strncpy(chop->label, "FREE", LABEL_SIZE);

    chop->next = bestBlck->next;

    bestBlck->allocated = 1;

    bestBlck->size = bytes;

    strncpy(bestBlck->label, label, LABEL_SIZE);

    bestBlck->next = chop;

    return bestBlck->address;

  }
  /*policy not recognized*/
  return NULL;
}

/******************************************************************************/
/*!
  \brief
    Deallocates a node.

  \param heap
    the list losing a node.

  \param address
    address of the node being removed.

  \return
    Returns SUCCESS for deallocation and FAILURE if failed.
*/
/******************************************************************************/
int mm_deallocate(block_info *heap, void *address)
{
  /* trails in case of merging */
  block_info *prev = heap;

  /* temp for merging*/
  block_info *temp;

  /* changes address to char* to be compared */
  char* compAd = (char*)address;

  /* walk the list */
  while (heap)
  {
    /* hold current address*/
    char *checkAd = heap->address;

    /* compares against the address we are looking for */
    if (checkAd == compAd)
    {
      /* if it was already deallocated*/
      if (heap->allocated == 0)
        return FAILURE;

      /* set flag to 0 */
      heap->allocated = 0;

      /* checks infront in case of merging */
      if (heap->next && heap->next->allocated == 0)
      {
        /* increase the size of current node */
        heap->size += heap->next->size;

        /* take the pointer of the one being merged */
        temp = heap->next;

        /* skip over */
        heap->next = heap->next->next;

        /* free temp */
        free(temp);
      }
      /* check the prev in case of merging */
      if (prev != heap && prev->allocated == 0)
      {
        /* add the sizes together */
        prev->size += heap->size;

        /* link them together */
        prev->next = heap->next;

        /* free thee merged node */
        free(heap);
      }
      /* SUCCESS! */
      return SUCCESS;
    }

    /* go to the next node */
    prev = heap;
    heap = heap->next;
  }

  /* End of the List */
  return FAILURE;
}


/*****************************************************************************/
/*!
  \brief
    Destroys everything by a given heap.

  \param heap
    the manager being destroyed.
*/
/*****************************************************************************/
void mm_destroy(block_info *heap)
{
  /* temp pointer to traverse list */
  block_info *temp;

  /* free chunk of memory alloc'ed in create */
  free(heap->address);

  while (heap)
  {
    /* set temp to next node */
    temp = heap->next;

    /* free current node */
    free(heap);

    /* move to next node */
    heap = temp;
  }
}