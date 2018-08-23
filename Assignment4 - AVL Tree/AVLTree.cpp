/******************************************************************************/
/*!
\file   AVLTree.cpp
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS280
\par    Assignment #4
\date   7/7/2017
\brief
This is the implementation file for all member functions
specified in AVLTree.h.

Operations include:

- insert        - this inserts an item into the tree
- remove        - removes an item from the tree
- InsertItAVL   - this inserts an item into the tree (Helper)
- BalanceIt     - Balances the tree
- RightHeavyBal - Balances the tree if the right is heavy
- LeftHeavyBal  - Balances the tree if the left is heavy
- AttachNRot    - attachs a rotated tree to the parent tree
- count_nodes   - counts the number of nodes
- RotLeft       - rotates the tree left
- RotRight      - rotates the tree right
- DeleteItAVL   - deletes an item in the tree

*/
/******************************************************************************/
#include "AVLTree.h"

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
AVLTree<T>::AVLTree(ObjectAllocator * oa, bool ShareOA) : BSTree<T>(oa, ShareOA)
{

}

/******************************************************************************/
/*!

  \brief
    Deconstructor

*/
/******************************************************************************/
template<typename T>
AVLTree<T>::~AVLTree()
{

}

/******************************************************************************/
/*!

  \brief
    Inserts a value into the tree

  \param value
    the value being inserted

*/
/******************************************************************************/
template<typename T>
void AVLTree<T>::insert(const T & value)
{
  try
  {
    std::stack<BinTree> nodes;
    InsertItAVL(this->mHead, value, nodes, 0);

    this->mHeight = this->tree_height(this->mHead);
  }
  catch (const BSTException &except)
  {
    throw except;
  }
}

/******************************************************************************/
/*!

  \brief
    removes an item based on value

  \param value
    the value being removed

*/
/******************************************************************************/
template<typename T>
void AVLTree<T>::remove(const T & value)
{
  std::stack<BinTree> node;
  DeleteItAVL(this->mHead, value, node);

  this->mHeight = this->tree_height(this->mHead);
}

/******************************************************************************/
/*!

  \brief
    Inserts an item into the AVLTree

  \param tree
    the tree that is being inserted into

  \param val
    what is being inserted

  \param nodes
    a stack of nodes

  \param depth
    the depth of the tree

*/
/******************************************************************************/
template<typename T>
void AVLTree<T>::InsertItAVL(BinTree & tree, const T & val, stack & nodes, int depth)
{
  try
  {
    if (!tree)
    {
      tree = this->make_node(val);

      BalanceIt(nodes);

      ++(this->mNumNodes);
    }
    else if (val < tree->data)
    {
      nodes.push(tree);
      InsertItAVL(tree->left, val, nodes, ++depth);
    }
    else
    {
      nodes.push(tree);
      InsertItAVL(tree->right, val, nodes, ++depth);
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
    Balances the tree
  
  \param nodes
    a stacks of nodes that are going to be balanced
  
  \param Inserted
    used to tell if it was inserted properly

*/
/******************************************************************************/
template<typename T>
void AVLTree<T>::BalanceIt(stack & nodes, bool Inserted)
{
  while (!nodes.empty())
  {
    BinTree tre = nodes.top();
    nodes.pop();

    int lftHeight = this->tree_height(tre->left);
    int rhtHeight = this->tree_height(tre->right);

    if (lftHeight > rhtHeight + 1)
    {
      LeftHeavyBal(tre, nodes);

      if (Inserted)
      {
        return;
      }
    }
    else if (rhtHeight > lftHeight + 1)
    {
      RightHeavyBal(tre, nodes);
      if (Inserted)
      {
        return;
      }
    }
  }
}

/******************************************************************************/
/*!

  \brief
    Balances the tree if it is right heavy
  
  \param tre
    the tree being balanced
  
  \param nodes
    a stack of nodes to be balanced

*/
/******************************************************************************/
template<typename T>
void AVLTree<T>::RightHeavyBal(BinTree & tre, std::stack<BinTree>& nodes)
{
  BinTree a, b, c, nulparent = 0;

  a = tre->right;
  b = a->left;
  c = a->right;

  int bHeight = this->tree_height(b);
  int cHeight = this->tree_height(c);

  if (bHeight > cHeight)
  {
    RotRight(tre->right);
    RotLeft(tre);

    if (nodes.empty())
    {
      AttachNRot(nulparent, b);
    }
    else
    {
      AttachNRot(nodes.top(), b);
    }
  }
  else if (cHeight > bHeight || bHeight == cHeight)
  {
    RotLeft(tre);

    if (nodes.empty())
    {
      AttachNRot(nulparent, a);
    }
    else
    {
      AttachNRot(nodes.top(), a);
    }
  }
}

/******************************************************************************/
/*!

  \brief
    Balances the tree if it is left heavy
  
  \param tre
    the tree being balanced
  
  \param nodes
    a stack of nodes to be balanced

*/
/******************************************************************************/
template<typename T>
void AVLTree<T>::LeftHeavyBal(BinTree & tre, std::stack<BinTree>& nodes)
{
  BinTree a, b, c, nulparent = 0;

  a = tre->left;
  b = a->left;
  c = a->right;

  int bHeight = this->tree_height(b);
  int cHeight = this->tree_height(c);

  if (bHeight > cHeight || cHeight == bHeight)
  {
    RotRight(tre);

    if (nodes.empty())
    {
      AttachNRot(nulparent, a);
    }
    else
    {
      AttachNRot(nodes.top(), a);
    }
  }

  else if (cHeight > bHeight)
  {
    RotLeft(tre->left);
    RotRight(tre);

    if (nodes.empty())
    {
      AttachNRot(nulparent, c);
    }
    else
    {
      AttachNRot(nodes.top(), c);
    }
  }

}

/******************************************************************************/
/*!

  \brief
    Attaches a part that is rotated to the parent
  
  \param parent
    what is getting a new part attached to it
  
  \param rotate
    part being attached after it is rotated

*/
/******************************************************************************/
template<typename T>
void AVLTree<T>::AttachNRot(BinTree & parent, BinTree & rotate)
{
  if (!parent)
  {
    this->mHead = rotate;
  }
  else
  {
    if (rotate->data < parent->data)
    {
      parent->left = rotate;
    }
    else
    {
      parent->right = rotate;
    }
  }
}

/******************************************************************************/
/*!

  \brief
    counts the number of nodes
  
  \param head
    the head of the tree
  
  \return
    returns the number of nodes
*/
/******************************************************************************/
template<typename T>
unsigned int AVLTree<T>::count_nodes(BinTree & head)
{
  unsigned int count = 0;
  if (head != NULL)
  {
    count = 1 + count_nodes(head->left) + count_nodes(head->right);
  }

  return count;
}

/******************************************************************************/
/*!

  \brief
    Rotates the tree left
  
  \param tree
    the tree being rotated

*/
/******************************************************************************/
template<typename T>
void AVLTree<T>::RotLeft(BinTree & tree)
{
  BinTree tmp = tree;
  tree = tree->right;
  tmp->right = tree->left;
  tree->left = tmp;
}

/******************************************************************************/
/*!

  \brief
    Rotates the tree right
  
  \param tree
    the tree being rotated

*/
/******************************************************************************/
template<typename T>
void AVLTree<T>::RotRight(BinTree & tree)
{
  BinTree tmp = tree;
  tree = tree->left;
  tmp->left = tree->right;
  tree->right = tmp;
}

/******************************************************************************/
/*!

  \brief
    Searchs through an AVL Tree and deletes a value from it
  
  \param tree
    the tree being searched
  
  \param val
    value to be deleted
  
  \param nodes
    a stack of nodes to search through

*/
/******************************************************************************/
template<typename T>
void AVLTree<T>::DeleteItAVL(BinTree & tree, const T & val, std::stack<BinTree>& nodes)
{
  if (tree == 0)
  {
    return;
  }
  else if (val < tree->data)
  {
    nodes.push(tree);
    DeleteItAVL(tree->left, val, nodes);
  }
  else if (val > tree->data)
  {
    nodes.push(tree);
    DeleteItAVL(tree->right, val, nodes);
  }
  else
  {
    if (tree->left == 0)
    {
      BinTree tmp = tree;
      tree = tree->right;
      this->free_node(tmp);
      --(this->mNumNodes);

      BalanceIt(nodes, false);
    }
    else if (tree->right == 0)
    {
      BinTree tmp = tree;
      tree = tree->left;
      this->free_node(tmp);
      --(this->mNumNodes);

      BalanceIt(nodes, false);
    }
    else
    {
      BinTree pred = 0;
      this->find_predecessor(tree, pred);
      tree->data = pred->data;
      nodes.push(tree);
      DeleteItAVL(tree->left, tree->data, nodes);
    }
  }
}

