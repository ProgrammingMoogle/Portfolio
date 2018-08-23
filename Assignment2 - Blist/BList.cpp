/******************************************************************************/
/*!
\file   BList.cpp
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS280
\par    Assignment #2
\date   6/15/2017
\brief
  This is the implementation file for all member functions
  of a class called BList, as specified in specification
  file BList.h.

Operations include:

- push_back       - pushes node to the back
- push_front      - pushes node to the front 
- insert          - inserts node into the list
- remove          - removes the node from the list
- remove_by_value - removes the node based on the value
- find            - finds a node in a list and gives the index if found
- size            - gives the size of the list
- clear           - clears all of the nodes in the list
- nodesize        - gives the size of the node
- GetHead         - gets the head of the list
- GetStats        - gets the statistics of the list
- AllocateNode    - allocates a node
- DeallocateNode  - deallocates a node
- GetValue        - gets the value
- Split           - splits the node
- InsertNode      - inserts a node into a list
- RegularNode     - a regular insert for a linked list
- FindPosition    - finds the position of a node
- CopyHelp        - a copy helper

*/
/******************************************************************************/

/******************************************************************************/
/*!

  A templated default constructor

*/
/******************************************************************************/
template <typename T, int Size>
BList<T, Size>::BList() : head_(nullptr), tail_(nullptr)
{
  mStats.ArraySize = Size;
  mStats.NodeSize = sizeof(BNode);
}

/******************************************************************************/
/*!

  A templated copy constructor

  /param rhs
    the BList being copied
*/
/******************************************************************************/
template <typename T, int Size>
BList<T, Size>::BList(const BList& rhs)
{
  CopyHelp(rhs);
}

/******************************************************************************/
/*!

  A templated  deconstructor

*/
/******************************************************************************/
template <typename T, int Size>
BList<T, Size>::~BList()
{
  clear();
}

/******************************************************************************/
/*!

  A templated assignment operator

  \param rhs
    the Blist that is getting assigned
*/
/******************************************************************************/
template <typename T, int Size>
BList<T, Size>& BList<T, Size>::operator=(const BList& rhs)
{
  if (this == &rhs)
  {
    return *this;
  }
  BNode* originHead = head_;

  CopyHelp(rhs);

  while (originHead != nullptr)
  {
    BNode* temp = originHead;
    originHead = originHead->next;
    delete temp;
  }

  return *this;
}

/******************************************************************************/
/*!

  A templated push back function

  \param value
    the value to be pushed back

*/
/******************************************************************************/
template<typename T, int Size>
void BList<T, Size>::push_back(const T& value)
{
  try
  {
    if (!tail_)
    {
      tail_ = AllocateNode();
      if (!head_)
      {
        head_ = tail_;
      }
    }
    if (tail_->count == Size)
    {
      BNode* newNode = AllocateNode(nullptr, tail_);
      tail_->next = newNode;
      tail_ = newNode;
    }
    tail_->values[tail_->count++] = value;
  }
  catch (std::bad_alloc& except)
  {
    throw BListException(BListException::BLIST_EXCEPTION::E_NO_MEMORY,
      std::string("Inefficient amount of memory for new node: ") + 
      except.what());
  }
  ++mStats.ItemCount;
}

/******************************************************************************/
/*!

  A templated push front function
  
  \param value
    the value being pushed to the front

*/
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::push_front(const T& value)
{
  try 
  {
    if (!head_)
    {
      head_ = AllocateNode();
      if (!tail_)
      {
        tail_ = head_;
      }
    }

    if (head_->count == Size)
    { 
      BNode* newNode = AllocateNode(head_, nullptr);
      head_->prev = newNode;
      head_ = newNode;
    }

    
    for (int i = head_->count - 1; i >= 0; --i)
    {
      head_->values[i + 1] = head_->values[i]; 
    }

    
    head_->values[0] = value;
    head_->count += 1;

  }
  catch (std::bad_alloc & except)
  {
    throw BListException(BListException::BLIST_EXCEPTION::E_NO_MEMORY,
      std::string("Inefficient amount of memory for new node: ")
      + except.what());
  }

  ++mStats.ItemCount;
}

/******************************************************************************/
/*!

  A templated insert function

  \param value
    the value being inserted into the list

*/
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::insert(const T& value)
{

  
  if (!head_)
  {
    push_front(value);
    return;
  }

  if (Size == 1) 
  {
    RegularInsert(value);
    return;
  }

  BNode* iter = FindPosition(value);

  if (iter->count == Size)
  { 
    Split(iter);
  }

  BNode* nextNode = iter->next;
  if (nextNode && nextNode->values[0] < value)
  {
    iter = nextNode;
  }
  
  InsertNode(iter, value);

}

/******************************************************************************/
/*!

  A templated remove function

  \param index
    the index at which the node will be removed

*/
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::remove(int index)
{

  if (index > mStats.ItemCount)
  {
    throw BListException(BListException::BLIST_EXCEPTION::E_BAD_INDEX,
      "Index is bigger than list size");
  }
  if (index < 0)
  {
    throw BListException(BListException::BLIST_EXCEPTION::E_BAD_INDEX, 
      "Index cannot be a negative value");
  }
  int counter = index;
  BNode* iter = head_;

  while (counter >= iter->count)
  {
    counter -= iter->count;
    iter = iter->next;
  }

  for (int i = counter; i < iter->count - 1; ++i)
  {
    iter->values[i] = iter->values[i + 1];
  }

  if ((iter->count -= 1) == 0)
  {
    DeallocateNode(iter);
  }
}

/******************************************************************************/
/*!

  A templated remove by value function

  \param value
    the value that will be removed

*/
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::remove_by_value(const T& value)
{
  int counter = -1;
  BNode* iter = head_;
  bool isFound = false;

  while (iter != nullptr) {

    for (counter = 0; counter < iter->count; ++counter) 
    {
      if (value == iter->values[counter]) 
      {
        isFound = true;
        break;
      }
    }

    if (isFound) 
    {
      break;
    }
    iter = iter->next;
  }

  if (counter < 0 || iter == nullptr)
  {
    return;
  }
  --mStats.ItemCount;

  for(int i = counter; i < iter->count - 1; ++i)
  {
    iter->values[i] = iter->values[i + 1];
  }

  if ((iter->count -= 1) == 0) 
  {
    DeallocateNode(iter);
  }

}

/******************************************************************************/
/*!

  A templated find function

  \param value
    the value to be found
  
  \return
    the index or -1 if it isn't in the list

*/
/******************************************************************************/
template <typename T, int Size>
int BList<T, Size>::find(const T& value) const
{
  int counter = 0;
  BNode* iter = head_;
  while (iter != nullptr)
  {
    for (int i = 0; i < iter->count; ++i)
    {
      if (value == iter->values[i])
      {
        return counter + i;
      }
    }

    counter += iter->count;
    iter = iter->next;
  }

  return -1;
}

/******************************************************************************/
/*!

  A templated index operator

  \param index
    the index that the value is at
  
  \return
    the value at the index

*/
/******************************************************************************/
template <typename T, int Size>
T& BList<T, Size>::operator[](int index)
{
  return GetValue(index);
}

/******************************************************************************/
/*!

  A templated index operator that is const

  \param index
    the index that the value is at

  \return
    the value at the index

*/
/******************************************************************************/
template <typename T, int Size>
const T& BList<T, Size>::operator[](int index) const
{
  return GetValue(index);
}

/******************************************************************************/
/*!

  A templated size function

  \return
    the size of the list

*/
/******************************************************************************/
template <typename T, int Size>
size_t BList<T, Size>::size() const
{
  return mStats.ItemCount;
}

/******************************************************************************/
/*!

  A templated clear function

*/
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::clear()
{
  BNode* iter;
  while (head_)
  {
    iter = head_;
    head_ = head_->next;

    delete iter;
  }

  mStats.ItemCount = 0;
  mStats.NodeCount = 0;
}

/******************************************************************************/
/*!

  A templated GetStats function

  /return
    the statistics of the list

*/
/******************************************************************************/
template <typename T, int Size>
BListStats BList<T, Size>::GetStats() const
{
  return mStats;
}

/******************************************************************************/
/*!

  A templated nodesize function

  \return
    the size of the node

*/
/******************************************************************************/
template <typename T, int Size>
size_t BList<T, Size>::nodesize(void)
{
  return sizeof(BNode);
}

/******************************************************************************/
/*!

  A templated GetHead function

  /return
    the head of the list

*/
/******************************************************************************/
template <typename T, int Size>
const typename BList<T, Size>::BNode* BList<T, Size>::GetHead() const
{
  return head_;	
}

/******************************************************************************/
/*!

  A templated Allocate Node function

  /param next
    the next node following the new node

  /param prev
    the previous node before the new node

  /return
    gives the new node

*/
/******************************************************************************/
template < typename T, int Size >
typename BList<T, Size>::BNode* BList<T, Size>::AllocateNode(BNode* next,
  BNode* prev)
{
  BNode* newNode = new BNode(next, prev);
  if (next)
  {
    next->prev = newNode;
  }
  if (prev)
  {
    prev->next = newNode;
  }
  ++mStats.NodeCount;
  return newNode;
}

/******************************************************************************/
/*!

  A templated Deallocate Node function

  /param toRemove
    the node to be deallocated

*/
/******************************************************************************/
template<typename T, int Size>
void BList<T, Size>::DeallocateNode(BNode * toRemove)
{

  if (toRemove == head_) 
  {
    head_ = head_->next;
    if (head_)
    {
      head_->prev = nullptr;
    }
  }
  else if (toRemove == tail_) 
  {
    tail_ = tail_->prev;
    if (tail_)
    {
      tail_->next = nullptr;
    }
  }
  else 
  {
    BNode* nextItem = toRemove->next;
    BNode* prevItem = toRemove->prev;

    nextItem->prev = prevItem;
    prevItem->next = nextItem;
  }

  --mStats.NodeCount;
  delete toRemove;
}

/******************************************************************************/
/*!

  A templated GetValue function

  /param index
    the index at which to get the value from

  /return
    gives the value at that index

*/
/******************************************************************************/
template<typename T, int Size>
T & BList<T, Size>::GetValue(int index) const
{
  if (index > mStats.ItemCount)
  {
    throw BListException(BListException::BLIST_EXCEPTION::E_BAD_INDEX,
      "Subscript index is higher than list size");
  }
  if (index < 0)
  {
    throw BListException(BListException::BLIST_EXCEPTION::E_BAD_INDEX,
      "Subscript index cannot be a negative value");
  }
  if (index == 0)
  {
    return head_->values[0];
  }
  int counter = index;
  BNode* iter = head_;

  while (counter >= iter->count)
  {
    counter -= iter->count;
    iter = iter->next;
  }

  return iter->values[counter];
}

/******************************************************************************/
/*!

  A templated Split function

  /param toSplit
    the node to be split

*/
/******************************************************************************/
template<typename T, int Size>
void BList<T, Size>::Split(BNode *toSplit)
{
  BNode* newNode = AllocateNode(toSplit->next, toSplit);

  int midIndex = (Size) / 2;
  int indexNode = 0;
  int indexSource = midIndex;

  while (indexSource < Size) 
  {
    newNode->values[indexNode++] = toSplit->values[indexSource++];
  }

  // update information
  toSplit->count = midIndex;
  newNode->count = indexNode;

  // adjust if tail was split
  if(tail_->next)
  {
    tail_ = tail_->next;
  }
}

/******************************************************************************/
/*!

  A templated Insert Node function

  /param toInsert
    the node to be inserted

  /param value
    the value being placed in the node

*/
/******************************************************************************/
template < typename T, int Size >
void BList<T, Size>::InsertNode(BNode* toInsert, const T& value)
{

  int index = 0;

  // will quit loop when index = the index to insert
  while (index < toInsert->count && toInsert->values[index] < value)
  {
    ++index;
  };

  // shift everything
  for (int counter = toInsert->count; counter >= index; --counter)
  {
    toInsert->values[counter] = toInsert->values[counter - 1];
  }

  toInsert->values[index] = value;
  toInsert->count += 1;
  ++mStats.ItemCount;
}

/******************************************************************************/
/*!

  A templated Regular Insert function

  /param value
    the value being inserted into the list

*/
/******************************************************************************/
template<typename T, int Size>
void BList<T, Size>::RegularInsert(const T & value)
{
  BNode* iter;
  
  if (tail_->values[0] < value) 
  { 
    push_back(value);
  }
  else if (value < head_->values[0]) 
  {
    push_front(value);
  }
  else 
  { 

    iter = head_->next;
    while (iter->values[0] < value)
    {
      iter = iter->next;
    }

    iter = iter->prev;

    BNode* newNode = AllocateNode(iter->next, iter);
    newNode->values[0] = value;
    newNode->count = 1;

    ++mStats.ItemCount;
  }
}

/******************************************************************************/
/*!

  A templated Find Position function

  /param value
    the value to be found

  /return
    gives the position of the node with the value in it

*/
/******************************************************************************/
template<typename T, int Size>
typename BList<T, Size>::BNode * BList<T, Size>::FindPosition(const T & value)
{
  BNode* returner;

  if (tail_->values[0] < value) 
  {
    returner = tail_;
  }
  else
  {
    returner = head_;
    while (returner->values[0] < value)
    {
      returner = returner->next;
    }

    BNode* prevNode = returner->prev;

    if (prevNode && (value < prevNode->values[prevNode->count - 1]
        || prevNode->count != Size || returner->count == Size))
    {
      returner = prevNode;
    }
  }

  return returner;
}

/******************************************************************************/
/*!

  A templated Allocate Node function

  /param other
    BList that is having its contents copied

*/
/******************************************************************************/
template<typename T, int Size>
void BList<T, Size>::CopyHelp(BList const & other)
{
  try
  {
    if (other.head_)
    {
      head_ = new BNode(*other.head_);
      head_->prev = nullptr;
      BNode* currNode = head_->next;
      BNode* prevNode = head_;
      BNode* otherCurr = other.head_->next;
      while (otherCurr)
      {
        currNode = new BNode(*otherCurr);
        prevNode->next = currNode;
        currNode = currNode->next;
        prevNode = prevNode->next;
        otherCurr = otherCurr->next;
      }

      tail_ = prevNode;
      tail_->next = nullptr;
    }
  }
  catch (std::bad_alloc& except)
  {
    BNode* iter = head_;

    while (iter)
    {
      BNode* next = iter->next;
      delete iter;
      iter = next;
    }

    throw BListException(BListException::BLIST_EXCEPTION::E_DATA_ERROR, 
      std::string("Copy construction: ") + except.what());
  }

  mStats = other.mStats;

}
