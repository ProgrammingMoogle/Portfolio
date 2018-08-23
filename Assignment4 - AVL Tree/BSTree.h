/******************************************************************************/
/*!
\file   BSTree.h
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS280
\par    Assignment #4
\date   7/7/2017
\brief
This is the header file for all member functions
of BSTree.cpp.

*/
/******************************************************************************/
//---------------------------------------------------------------------------
#ifndef BSTREEH
#define BSTREEH
//---------------------------------------------------------------------------
#include <string>    // std::string
#include <stdexcept> // std::exception
#include <algorithm> // Max

#include "ObjectAllocator.h"

/*!
  The exception class for the AVL/BST classes
*/
class BSTException : public std::exception
{
  public:
    /*!
      Non-default constructor

      \param ErrCode
        The kind of exception (only one currently)

      \param Message
        The human-readable reason for the exception.
    */
    BSTException(int ErrCode, const std::string& Message) :
      error_code_(ErrCode), message_(Message) {
    };

    /*! 
      Retrieve the exception code.

      \return
        E_NO_MEMORY
    */
    virtual int code() const {
      return error_code_;
    }

    /*!
      Retrieve the message string

      \return
        The human-readable message.
    */
    virtual const char *what() const throw() {
      return message_.c_str();
    }

    //! Destructor
    virtual ~BSTException() {}

    //! The kinds of exceptions (only one currently)
    enum BST_EXCEPTION{E_NO_MEMORY};

  private:  
    int error_code_;      //!< The code of the exception
    std::string message_; //!< Readable message text
};

/*!
  The definition of the BST 
*/
template <typename T>
class BSTree
{
  public:
    //! The node structure
    struct BinTreeNode
    {
      BinTreeNode *left;  //!< The left child
      BinTreeNode *right; //!< The right child
      T data;             //!< The data
      int balance_factor; //!< optional for efficient balancing
      unsigned count;     //!< nodes in this subtree for efficient indexing

      //! Default constructor
      BinTreeNode() : left(0), right(0), data(0), balance_factor(0), count(1) {};

      //! Conversion constructor
      BinTreeNode(const T& value) : left(0), right(0), data(value), balance_factor(0), count(1) {};
    };

    //! shorthand
    typedef BinTreeNode* BinTree;

    BSTree(ObjectAllocator *oa = 0, bool ShareOA = false);
    BSTree(const BSTree& rhs);
    virtual ~BSTree();
    BSTree& operator=(const BSTree& rhs);
    const BinTreeNode* operator[](int index) const; // for r-values
    virtual void insert(const T& value);
    virtual void remove(const T& value);
    void clear();
    bool find(const T& value, unsigned &compares) const;
    bool empty() const;
    unsigned int size() const;
    int height() const;
    BinTree root() const;

    static bool ImplementedIndexing();

  protected:
    BinTree& get_root();
    BinTree make_node(const T& value) const;
    void free_node(BinTree node);
    int tree_height(BinTree tree) const;
    void find_predecessor(BinTree tree, BinTree &predecessor) const;

    //would call it root, but there is also the root function so I'll just call it head;
    BinTree mHead; //!< the head of the tree or the original tree
    ObjectAllocator *mAlloc; //!< used for allocating nodes
    int mHeight; //!< the height of the tree
    unsigned int mNumNodes; //!< the number of nodes
    bool mOwnerOA; //!< determines whether or not we own the node or not
    bool mShareAlloc; //!< determines whether not it shares allocation or not

    //Used to clear the entire tree
    void ClearAll(BinTree tree);
    //inserts an item into the tree
    virtual void InsertIt(BinTree &tree, const T& val, int depth);
  private:
    // private stuff...
    //A copy helper
    void CopyHelp(BinTree &dest, const BinTree &source);
    //Deletes an item in the tree
    virtual void DeleteIt(BinTree& tree, const T& data);
    //searches for an item and returns true if found else it's false
    bool FindIt(BinTree tree, const T& data, unsigned &comp) const;
};

#include "BSTree.cpp"

#endif
//---------------------------------------------------------------------------
