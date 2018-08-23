/******************************************************************************/
/*!
\file   BSTree.cpp
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS280
\par    Assignment #4
\date   7/7/2017
\brief
This is the implementation file for all member functions
specified in BSTree.h.

Operations include:

- operator=           - overloaded assignment operator
- operator[]          - an overloaded indexing operator (not implemented)
- insert              - inserts into the tree
- remove              - removes from the tree
- clear               - clears a part of the tree
- ClearAll            - clears the entire tree
- find                - finds a node of tree
- empty               - checks if empty
- size                - gives the size
- height              - the height of the tree
- root                - the root of the tree
- ImplementedIndexing - tells if indexing was implemented or not
- get_root            - gets the root of the tree
- make_node           - makes a node
- free_node           - frees a node
- tree_height         - gives the trees height
- find_predecessor    - finds the predecessor of the current node
- CopyHelp            - copy helper
- InsertIt            - inserts an item
- DeleteIt            - deletes an item
- FindIt              - finds an item

*/
/******************************************************************************/
#include "BSTree.h"

/******************************************************************************/
/*!

  \brief
    A non defualt constructor
  
  \param oa
    the object allocator
  
  \param ShareOA
    tells whether not the object allocator is shared

*/
/******************************************************************************/
template<typename T>
BSTree<T>::BSTree(ObjectAllocator * oa, bool ShareOA)
  : mHead(0), mHeight(-1), mNumNodes(0)
{
  if (oa)
  {
    mOwnerOA = false;
    mAlloc = oa;
  }
  else
  {
    OAConfig config(true);
    mAlloc = new ObjectAllocator(sizeof(BinTreeNode), config);
    mOwnerOA = true;
  }
  if (ShareOA)
  {
    mShareAlloc = true;
  }
  else
  {
    mShareAlloc = false;
  }
}

/******************************************************************************/
/*!

  \brief
    A copy constructor
  
  \param rhs
    the tree being copied

*/
/******************************************************************************/
template<typename T>
BSTree<T>::BSTree(const BSTree & rhs)
{
  if (rhs.mShareAlloc)
  {
    mAlloc = rhs.mAlloc;

    mOwnerOA = false;
    mShareAlloc = true;
  }
  else
  {
    OAConfig config(true);
    mAlloc = new ObjectAllocator(sizeof(BinTreeNode), config);
    mOwnerOA = true;
    mShareAlloc = false;
  }

  CopyHelp(mHead, rhs.mHead);

  mNumNodes = rhs.mNumNodes;
  mHeight = rhs.mHeight;
}

/******************************************************************************/
/*!

  \brief
    Deconstructor

*/
/******************************************************************************/
template<typename T>
BSTree<T>::~BSTree()
{
  clear();
  if (mOwnerOA)
  {
    delete mAlloc;
  }
}

/******************************************************************************/
/*!

  \brief
    A assignment operator
  
  \param rhs
    the tree being assigned

  \return
    returns the new tree

*/
/******************************************************************************/
template<typename T>
BSTree<T>& BSTree<T>::operator=(const BSTree & rhs)
{
  if (this == &rhs)
  {
    return *this;
  }
  if (rhs.mShareAlloc)
  {
    if (mOwnerOA)
    {
      clear();
      delete mAlloc;
    }
    mAlloc = rhs.mAlloc;
    mOwnerOA = false;
    mShareAlloc = true;
  }

  clear();
  CopyHelp(mHead, rhs.mHead);

  mNumNodes = rhs.mNumNodes;
  mHeight = rhs.mHeight;

  return *this;
}

/******************************************************************************/
/*!

  \brief
    A indexing operator
  
  \param index
    the index of the tree

  \return
    returns 0 since it is not implemented

*/
/******************************************************************************/
template<typename T>
const typename BSTree<T>::BinTreeNode* BSTree<T>::operator[](int index) const
{
  if (index)
  {
    return 0;
  }
  else
  {
    return 0;
  }
}

/******************************************************************************/
/*!

  \brief
    inserts an item
  
  \param value
    the value being inserted

*/
/******************************************************************************/
template<typename T>
void BSTree<T>::insert(const T & value)
{
  try
  {
    InsertIt(mHead, value, 0);
  }
  catch (const BSTException &except)
  {
    throw except;
  }
}

/******************************************************************************/
/*!

  \brief
    removes an item
  
  \param value
    the value being removed

*/
/******************************************************************************/
template<typename T>
void BSTree<T>::remove(const T & value)
{
  DeleteIt(mHead, const_cast<T&>(value));

  mHeight = tree_height(mHead);
}

/******************************************************************************/
/*!

  \brief
    clears the tree

*/
/******************************************************************************/
template<typename T>
void BSTree<T>::clear()
{
  if (mHead)
  {
    ClearAll(mHead);

    mHead = 0;
    mHeight = -1;
    mNumNodes = 0;
  }
}

/******************************************************************************/
/*!

  \brief
    clears everything in the tree

  \param tree
    the tree being cleared

*/
/******************************************************************************/
template<typename T>
void BSTree<T>::ClearAll(BinTree tree)
{
  if (!tree)
  {
    return;
  }
  ClearAll(tree->left);
  ClearAll(tree->right);

  free_node(tree);
}

/******************************************************************************/
/*!

  \brief
    finds the value in the current tree
  
  \param value
    the value being compared
  
  \param compares
    the address of the value being compared

  \return
    returns true if found or false if not

*/
/******************************************************************************/
template<typename T>
bool BSTree<T>::find(const T & value, unsigned & compares) const
{
  return FindIt(mHead, value,compares);
}

/******************************************************************************/
/*!

  \brief
    tells whether or not the tree is empty
  
  \return
    returns true if empty or false if not

*/
/******************************************************************************/
template<typename T>
bool BSTree<T>::empty() const
{
  if (mHeight == -1)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/******************************************************************************/
/*!

  \brief
    tells the size of tree

  \return
    returns the number of nodes

*/
/******************************************************************************/
template<typename T>
unsigned int BSTree<T>::size() const
{
  return mNumNodes;
}

/******************************************************************************/
/*!

  \brief
    tells the height of tree

  \return
    returns the height of the tree

*/
/******************************************************************************/
template<typename T>
int BSTree<T>::height() const
{
  return mHeight;
}

/******************************************************************************/
/*!

  \brief
    tells the root of the tree

  \return
    returns the root of the tree

*/
/******************************************************************************/
template<typename T>
typename BSTree<T>::BinTree BSTree<T>::root() const
{
  return mHead;
}

/******************************************************************************/
/*!

  \brief
    tells whether or not indexing was implemented

  \return
    returns false

*/
/******************************************************************************/
template<typename T>
bool BSTree<T>::ImplementedIndexing()
{
  return false;
}

/******************************************************************************/
/*!

  \brief
    Gets the root

  \return
    returns the root

*/
/******************************************************************************/
template<typename T>
typename BSTree<T>::BinTree & BSTree<T>::get_root()
{
  return mHead;
}

/******************************************************************************/
/*!

  \brief
    makes a node

  \param value
    the value being turned into a node

  \return
    returns the new node

*/
/******************************************************************************/
template<typename T>
typename BSTree<T>::BinTree BSTree<T>::make_node(const T & value) const
{
  try
  {
    BinTree mem = reinterpret_cast<BinTree>(mAlloc->Allocate());

    BinTree node = new (mem) BinTreeNode(value);
    return node;
  }
  catch (const OAException &except)
  {
    throw(BSTException(BSTException::E_NO_MEMORY, except.what()));
  }
}

/******************************************************************************/
/*!

  \brief
    frees a node

  \param node
    the node being freed

*/
/******************************************************************************/
template<typename T>
void BSTree<T>::free_node(BinTree node)
{
  node->~BinTreeNode();

  mAlloc->Free(node);
}

/******************************************************************************/
/*!

  \brief
    gives the tree height

  \param tree
    the tree giving the height

  \return
    returns the height of the tree

*/
/******************************************************************************/
template<typename T>
int BSTree<T>::tree_height(BinTree tree) const
{
  if (tree == 0)
  {
    return -1;
  }
  else
  {
    return (1 + std::max(tree_height(tree->left), tree_height(tree->right)));
  }
}

/******************************************************************************/
/*!

  \brief
    finds the predecessor

  \param tree
    the tree being used

  \param predecessor
    the predecessor

*/
/******************************************************************************/
template<typename T>
void BSTree<T>::find_predecessor(BinTree tree, BinTree & predecessor) const
{
  predecessor = tree->left;

  while (predecessor->right != 0)
  {
    predecessor = predecessor->right;
  }
}

/******************************************************************************/
/*!

  \brief
    copies a tree into a new tree

  \param dest
    what is being copied to

  \param source
    what is being copied
*/
/******************************************************************************/
template<typename T>
void BSTree<T>::CopyHelp(BinTree &dest, const BinTree &source)
{
  if (!source)
  {
    dest = 0;
  }
  else
  {
    dest = make_node(source->data);

    CopyHelp(dest->left, source->left);
    CopyHelp(dest->right, source->right);
  }
}

/******************************************************************************/
/*!

  \brief
    Inserts an item into the tree

  \param tree
    the tree getting inserted

  \param val
    the value being inserted

  \param depth
    how deep it is getting inserted

*/
/******************************************************************************/
template<typename T>
void BSTree<T>::InsertIt(BinTree & tree, const T & val, int depth)
{
  try 
  {
    if (tree == 0)
    {
      if (depth > mHeight)
      {
        ++mHeight;
      }
      tree = make_node(val);

      ++mNumNodes;
      return;
    }
    if (val < tree->data)
    {
      InsertIt(tree->left, val, depth + 1);
    }
    else
    {
      InsertIt(tree->right, val, depth + 1);
    }
  }
  catch (const BSTException &except)
  {
    throw except;
  }
}

/******************************************************************************/
/*!

  \brief
    Deletes an item from the tree

  \param node
    the tree that is losing something

  \param data
    the data being removed

*/
/******************************************************************************/
template<typename T>
void BSTree<T>::DeleteIt(BinTree & tree, const T & data)
{
  if (tree == 0)
  {
    return;
  }
  else if (data < tree->data)
  {
    DeleteIt(tree->left, data);
  }
  else if (data > tree->data)
  {
    DeleteIt(tree->right, data);
  }
  else
  {
    if (tree->left == 0)
    {
      BinTree tmp = tree;
      tree = tree->right;
      free_node(tmp);
      --mNumNodes;
    }
    else if (tree->right == 0)
    {
      BinTree tmp = tree;
      tree = tree->left;
      free_node(tmp);
      --mNumNodes;
    }
    else
    {
      BinTree pred = 0;
      find_predecessor(tree, pred);
      tree->data = pred->data;
      DeleteIt(tree->left, tree->data);
    }
  }
}

/******************************************************************************/
/*!

\brief
makes a node

\param value
the value being turned into a node

\return
returns the new node

*/
/******************************************************************************/
template<typename T>
bool BSTree<T>::FindIt(BinTree tree, const T & data, unsigned & comp) const
{
  ++comp;

  if (tree == 0)
  {
    return false;
  }
  else if (data == tree->data)
  {
    return true;
  }
  else if (data < tree->data)
  {
    return FindIt(tree->left, data, comp);
  }
  else
  {
    return FindIt(tree->right, data, comp);
  }
}

