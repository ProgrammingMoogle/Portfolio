/******************************************************************************/
/*!
\file   BList.h
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS280
\par    Assignment #2
\date   6/15/2017
\brief
  This is the interface file for all member functions
  of classes called BListException and the templated class BList
  with struct BListStats.

*/
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
#ifndef BLIST_H
#define BLIST_H
////////////////////////////////////////////////////////////////////////////////

#include <string> // error strings

/*!
  The exception class for BList
*/
class BListException : public std::exception
{
  private:  
    int m_ErrCode;             //!< One of E_NO_MEMORY, E_BAD_INDEX, E_DATA_ERROR
    std::string m_Description; //!< Description of the exception

  public:
    /*!
      Constructor

      \param ErrCode
        The error code for the exception.

      \param Description
        The description of the exception.
    */
    BListException(int ErrCode, const std::string& Description) :
    m_ErrCode(ErrCode), m_Description(Description) {};

    /*!
      Get the kind of exception

      \return
        One of E_NO_MEMORY, E_BAD_INDEX, E_DATA_ERROR
    */
    virtual int code() const { 
      return m_ErrCode; 
    }

    /*!
      Get the human-readable text for the exception

      \return
        The description of the exception
    */
    virtual const char *what() const throw() {
      return m_Description.c_str();
    }

    /*!
      Destructor is "implemented" because it needs to be virtual
    */
    virtual ~BListException() {
    }

    //! The reason for the exception
    enum BLIST_EXCEPTION {E_NO_MEMORY, E_BAD_INDEX, E_DATA_ERROR};
};

/*!
  Statistics about the BList
*/
struct BListStats
{
    //!< Default constructor
  BListStats() : NodeSize(0), NodeCount(0), ArraySize(0), ItemCount(0)  {};

  /*! 
    Non-default constructor

    \param nsize
      Size of the node

    \param ncount
      Number of nodes in the list

    \param asize
      Number of elements in each node (array)

    \param count
      Number of items in the list

  */
  BListStats(size_t nsize, int ncount, int asize, int count) : 
  NodeSize(nsize), NodeCount(ncount), ArraySize(asize), ItemCount(count)  {};

  size_t NodeSize; //!< Size of a node (via sizeof)
  int NodeCount;   //!< Number of nodes in the list
  int ArraySize;   //!< Max number of items in each node
  int ItemCount;   //!< Number of items in the entire list
};  

/*!
  The BList class
*/
template <typename T, int Size = 1>
class BList
{
 
  public:
    /*!
      Node struct for the BList
    */
    struct BNode
    {
      BNode *next;    //!< pointer to next BNode
      BNode *prev;    //!< pointer to previous BNode
      int count;      //!< number of items currently in the node
      T values[Size]; //!< array of items in the node

      //!< Default constructor
      BNode() : next(0), prev(0), count(0) {}
      BNode(BNode* nextNode, BNode* prevNode) : next(nextNode), prev(prevNode), count(0) {}
    };

    BList();                            // default constructor
    BList(const BList &rhs);            // copy constructor
    ~BList();                           // destructor
    BList& operator=(const BList &rhs); // assign operator

      // arrays will be unsorted, if calling either of these
    void push_back(const T& value);
    void push_front(const T& value);

      // arrays will be sorted, if calling this
    void insert(const T& value);

    void remove(int index);
    void remove_by_value(const T& value); 

    int find(const T& value) const;       // returns index, -1 if not found

    T& operator[](int index);             // for l-values
    const T& operator[](int index) const; // for r-values

    size_t size() const;   // total number of items (not nodes)
    void clear();          // delete all nodes 

    static size_t nodesize(); // so the allocator knows the size

      // For debugging
    const BNode *GetHead() const;
    BListStats GetStats() const;

  private:
    BNode *head_; //!< points to the first node
    BNode *tail_; //!< points to the last node

    // Other private data and methods you may need ...
    BListStats mStats; //!< the statistics of the list
    //Allocates a node
    BNode* AllocateNode(BNode* next = nullptr, BNode* prev = nullptr);
    //Deallocates a node
    void DeallocateNode(BNode* toRemove);
    //Gets the value
    T& GetValue(int index) const;
    //Splits the node
    void Split(BNode* toSplit);
    //Inserts a node
    void InsertNode(BNode* toInsert, const T& value);
    //Regular insert for linked lists
    void RegularInsert(const T& value);
    //Finds the position of a node
    BNode* FindPosition(const T& value);
    //A Copy helper
    void CopyHelp(BList const& other);


};

#include "BList.cpp"

#endif // BLIST_H
