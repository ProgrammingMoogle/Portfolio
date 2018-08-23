/******************************************************************************/
/*!
\file   AVLTree.h
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS280
\par    Assignment #4
\date   7/7/2017
\brief
This is the header file for all member functions
of AVLTree.cpp.

*/
/******************************************************************************/
//---------------------------------------------------------------------------
#ifndef AVLTREEH
#define AVLTREEH
//---------------------------------------------------------------------------
#include <stack>
#include "BSTree.h"
//#include <iostream>

/*!
  Definition for the AVL Tree 
*/
template <typename T>
class AVLTree : public BSTree<T>
{
  public:
    typedef typename BSTree<T>::BinTreeNode* BinTree;

    typedef std::stack<BinTree> stack;

    AVLTree(ObjectAllocator *oa = 0, bool ShareOA = false);
    virtual ~AVLTree();
    virtual void insert(const T& value) override;
    virtual void remove(const T& value) override;

  private:
    // private stuff...
    //Inserts an item into the AVL tree
    void InsertItAVL(BinTree &tree, const T& val, stack &nodes, int depth);
    //Used to balance the tree
    void BalanceIt(stack & nodes, bool Inserted = true);
    //Rotates the tree left
    void RotLeft(BinTree &tree);
    //Rotates the tree right
    void RotRight(BinTree &tree);
    //Deletes an item in the AVL tree
    void DeleteItAVL(BinTree &tree, const T& val, std::stack<BinTree> &nodes);
    //Attaches parts of a tree and rotates it accordingly
    void AttachNRot(BinTree &parent, BinTree &rotate);
    //Used to balance a tree if it is heavy on the left side
    void LeftHeavyBal(BinTree &tre, std::stack<BinTree> &nodes);
    //Used to balance a tree if it is heavy on the right side
    void RightHeavyBal(BinTree &tre, std::stack<BinTree> &nodes);
    //Counts the nodes of the tree
    unsigned int count_nodes(BinTree &head);

};

#include "AVLTree.cpp"

#endif
//---------------------------------------------------------------------------
