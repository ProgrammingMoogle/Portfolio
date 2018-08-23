/******************************************************************************/
/*!
\file   ObjectAllocator.cpp
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS280
\par    Assignment #1
\date   6/201/2017
\brief  
  This is the implementation file for all member functions
  of a class called ObjectAllocator, as specified in specification
  file ObjectAllocator.h.  
  
  Operations include:

  - Allocate               - allocates an object
  - Free                   - deallocates an object
  - DumpMemoryInUse        - dumps memory that is in use
  - ValidatePages          - validates the pages
  - FreeEmptyPages         - Frees empty pages (Extra Credit)
  - ImplementedExtraCredit - tells if extra credit was implemented or not
  - SetDebugState          - sets the debug state
  - GetFreeList            - gets the free list
  - GetPageList            - gets the page list
  - GetConfig              - gets the configuration
  - GetStats               - gets the stats
  - AllocatePage           - allocates a page
  - AddToFreeList          - adds an object to free list
  - InitPage               - initializes a page
  - SetMemNMove            - sets the memory and moves it
  - SetNonDataPattern      - sets a none data pattern
  - MoveFreeList           - moves the the free list
  - IsObjectFreeList       - checks if object is in free list
  - FreeExternalHeader     - frees the external header
  - FreePage               - frees a page
  - CheckBounds            - checks the bounds of memory
  - CheckTwoFree           - checks if it was freed twice
  - CheckCorruption        - checks if it was corrupted

*/
/******************************************************************************/
#include "ObjectAllocator.h"
#include <string.h> /* strings   */
#include <assert.h> /* bad_alloc */


/******************************************************************************/
/*!

  Creates a ObjectAllocator of size 'ObjectSize' and initializes private data
  using 'config'. It defines the size of the objects as well as defines the 
  size of Alignment on the left and right as well as the how big the pages can
  be.
          
  \param ObjectSize 
    A size_t that defines the size of the objects in a page.
    
  \param config
    A OAConfig that defines the configuration of the object and page.

*/
/******************************************************************************/
ObjectAllocator::ObjectAllocator(size_t ObjectSize, const OAConfig & config)
  : mConfig(config), mStats(), mPageList(NULL), mFreeList(NULL)
{
  mStats.ObjectSize_ = ObjectSize;

  size_t totalObj = mConfig.ObjectsPerPage_ * ObjectSize;
  
  size_t totalPadding = mConfig.ObjectsPerPage_ * mConfig.PadBytes_ * 2;
  
  size_t totalHeader = mConfig.HBlockInfo_.size_ * mConfig.ObjectsPerPage_;
  
  //Calculating the Alignment for the left and right side of memory block
  unsigned int leftTotalSize = static_cast<unsigned int>
  (mConfig.HBlockInfo_.size_ + mConfig.PadBytes_ + sizeof(void*));
  
  mConfig.LeftAlignSize_ = mConfig.Alignment_ ? (leftTotalSize 
  % mConfig.Alignment_) : 0;
  
  mLeftSize = leftTotalSize + mConfig.LeftAlignSize_;

  unsigned int interTotalSize = static_cast<unsigned int>
 (mConfig.HBlockInfo_.size_ + mConfig.PadBytes_ * 2 + ObjectSize);
 
  mConfig.InterAlignSize_ = mConfig.Alignment_ ? (interTotalSize 
  % mConfig.Alignment_): 0;
  
  mInterSize = interTotalSize + mConfig.InterAlignSize_;

  size_t totalAlignSize = mConfig.LeftAlignSize_ + mConfig.InterAlignSize_
  * (mConfig.ObjectsPerPage_ - 1);

  //Calculating the size of the page
  mStats.PageSize_ = totalObj + totalPadding + totalHeader + totalAlignSize 
  + sizeof(void*);

  //Allocates a Page
  AllocatePage();
}

/******************************************************************************/
/*!

  Deconstructs the ObjectAllocator

*/
/******************************************************************************/
ObjectAllocator::~ObjectAllocator()
{
  GenericObject* nextPage;

  //Goes through the page list and deallocates it one by one.
  while (mPageList)
  {
    if (mConfig.HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbExternal)
    {
      unsigned char* blockStart = reinterpret_cast<unsigned char*>(mPageList);
      unsigned char* blockIter = blockStart + mLeftSize;
      while (static_cast<unsigned int>(blockIter - blockStart)
        < mStats.PageSize_)
      {
        unsigned char* blockHeaderIter = blockIter - mConfig.PadBytes_
        - mConfig.HBlockInfo_.size_;
        FreeExternalHeader(blockHeaderIter);
        blockIter += mInterSize;
      }
    }
    nextPage = mPageList->Next;
    delete[] (reinterpret_cast<unsigned char*>(mPageList));
    mPageList = nextPage;
  }
}


/******************************************************************************/
/*!

  Creates a object that will be given to the client to do what they will. Also,
  it handles different types of memory block and builds it according to it.
          
  \param label
    A char pointer that defines what type of memory block it is. 
    
  \return
    the newly created object

*/
/******************************************************************************/
void* ObjectAllocator::Allocate(const char *label)
{
  //When using the MemManager
  if (mConfig.UseCPPMemManager_)
  {
    //It will try and allocate an object
    try
    {
      void* objAllocated = ::operator new(mStats.ObjectSize_);

      ++mStats.Allocations_;
      ++mStats.ObjectsInUse_;
      --mStats.FreeObjects_;
      if (mStats.ObjectsInUse_ > mStats.MostObjects_)
      {
        mStats.MostObjects_ = mStats.ObjectsInUse_;
      }
      return objAllocated;
    }
    //if the allocation failed, then it will throw an exception
    catch (std::bad_alloc &)
    {
      throw OAException(OAException::E_NO_MEMORY, "Not enough memory.");
    }

  }
  
  //It will allocate a page if the free list is empty
  if (!mFreeList)
  {
    AllocatePage();
  }
  
  //Prepares to create the object, before giving it to the client
  GenericObject* returner = mFreeList;
  mFreeList = mFreeList->Next;

  ++mStats.Allocations_;
  ++mStats.ObjectsInUse_;
  --mStats.FreeObjects_;

  if (mStats.ObjectsInUse_ > mStats.MostObjects_)
  {
    mStats.MostObjects_ = mStats.ObjectsInUse_;
  }
  
  //Prepares the header block
  unsigned char* blockHeaderIter = reinterpret_cast<unsigned char*>(returner) 
  - mConfig.PadBytes_ - mConfig.HBlockInfo_.size_;
  unsigned short counter;

  //Depending on what kind of type of block it is, it has its own rules to go by
  switch (mConfig.HBlockInfo_.type_)
  {
    case OAConfig::HBLOCK_TYPE::hbExtended:
    {
      SetMemNMove(&blockHeaderIter, 0, mConfig.HBlockInfo_.additional_);
      memcpy(&counter, blockHeaderIter, sizeof(counter));
      ++counter;
      memcpy(blockHeaderIter, &counter, sizeof(counter));
      blockHeaderIter += sizeof(counter);
    }
    case OAConfig::HBLOCK_TYPE::hbBasic:
    {
      memcpy(blockHeaderIter, &mStats.Allocations_, sizeof(unsigned));
      blockHeaderIter += sizeof(unsigned);
      SetMemNMove(&blockHeaderIter, 1, sizeof(char));
      break;
    }
    case OAConfig::HBLOCK_TYPE::hbExternal:
    {
      MemBlockInfo* blockMem = new MemBlockInfo();
      blockMem->in_use = true;
      if (label)
      {
        blockMem->label = new char[strlen(label) + 1];
        strcpy(blockMem->label, label);
      }
      else
      {
        blockMem->label = NULL;
      }
      blockMem->alloc_num = mStats.Allocations_;
      MemBlockInfo** blockMemPtr = &blockMem;
      memcpy(blockHeaderIter, blockMemPtr, sizeof(void*));
      break;
    }
    case OAConfig::HBLOCK_TYPE::hbNone:
    {
      break;
    }
    default:
    {
      break;
    }
  }

  //if it is in debug mode, it will add a pattern at the end of the block to
  //track if the memory overflowed or underflowed in the block
  if (mConfig.DebugOn_)
  {
    memset(reinterpret_cast<unsigned char*>(returner), ALLOCATED_PATTERN,
    mStats.ObjectSize_);
  }

  //gives the object to the client
  return returner;
}

/******************************************************************************/
/*!

  Deallocates an object, but before it deallocates it runs through some checks
  to make sure that the object isn't being freed again, it isn't corrupted, or
  it isn't running into another if these occur, it will throw an exception
  instead of deallocating it.
          
  \param Object 
    An object that will be deleted.

*/
/******************************************************************************/
void ObjectAllocator::Free(void * Object)
{
  //if MemManager is on, delete the object.
  if (mConfig.UseCPPMemManager_)
  {
    ::operator delete(Object);
  }
  else
  {
    //Prepare to delete the object
    unsigned char* memPtr = reinterpret_cast<unsigned char*>(Object);
    if (mConfig.DebugOn_)
    {
      //Checks if it corrupted, its out of bounds, or if it was already freed
      CheckTwoFree(memPtr);
      CheckCorruption(memPtr);
      CheckBounds(memPtr);
      
      //Tells the client that it will be cleared, if the above didn't fail
      memset(memPtr, FREED_PATTERN, mStats.ObjectSize_);
    }
    
    unsigned char* blockHeaderIter = memPtr - mConfig.PadBytes_
    - mConfig.HBlockInfo_.size_;
    
    //Based on the type of the memory block, it will be freed a certain way
    switch (mConfig.HBlockInfo_.type_)
    {
      case OAConfig::HBLOCK_TYPE::hbExtended:
      {
        SetMemNMove(&blockHeaderIter, 0, mConfig.HBlockInfo_.additional_); 
        blockHeaderIter += sizeof(unsigned short); 
      }
      case OAConfig::HBLOCK_TYPE::hbBasic:
      {
        SetMemNMove(&blockHeaderIter, 0, sizeof(unsigned int)); 
        SetMemNMove(&blockHeaderIter, 0, sizeof(char)); 
        break;
      }
      case OAConfig::HBLOCK_TYPE::hbExternal:
      {
        FreeExternalHeader(blockHeaderIter);
        memset(blockHeaderIter, 0, mConfig.HBlockInfo_.size_);
        break;
      }
      case OAConfig::HBLOCK_TYPE::hbNone:
      {
      }
      default:
      {
        break;
      }
    }

    //After doing the deallocation, it will put the object back in the free list
    AddToFreeList(Object);
  }
  ++mStats.Deallocations_;
  ++mStats.FreeObjects_;
  --mStats.ObjectsInUse_;
}

/******************************************************************************/
/*!

  Dumps memory that is in use.
          
  \param fn
    A DUMPCALLBACK variable that is used to dump memory. 
    
  \return
    the count of how many were dumped.

*/
/******************************************************************************/
unsigned ObjectAllocator::DumpMemoryInUse(DUMPCALLBACK fn) const
{
  unsigned count = 0;
  GenericObject* pageListIter = mPageList;
  unsigned char * pageIter;
  unsigned char * pageStart;
  while (pageListIter) 
  {
    pageStart = reinterpret_cast<unsigned char*>(pageListIter);
    pageIter = pageStart + mLeftSize;

    while (static_cast<unsigned int>(pageIter - pageStart) < mStats.PageSize_)
    {
      if (!IsObjectFreeList(pageIter))
      {
        fn(pageIter, mStats.ObjectSize_);
        ++count;
      }
      pageIter += mInterSize;
    }
    pageListIter = pageListIter->Next;
  }

  return count;
}

/******************************************************************************/
/*!

  Validate the pages of the page list to ensure that they are valid.
          
  \param fn 
    A VALIDATECALLBACK used to validate a page.
    
  \return 
    the number of valid pages.

*/
/******************************************************************************/
unsigned ObjectAllocator::ValidatePages(VALIDATECALLBACK fn) const
{
  if (!mConfig.DebugOn_ || mConfig.PadBytes_ == 0)
    return 0;


  unsigned count = 0;
  GenericObject* pageListIter = mPageList;
  unsigned char * pageIter;
  unsigned char * pageStart;
  while (pageListIter) 
  {
    pageStart = reinterpret_cast<unsigned char*>(pageListIter);
    pageIter = pageStart + mLeftSize;

    while (static_cast<unsigned int>(pageIter - pageStart) < mStats.PageSize_) 
    {
      try 
      {
        CheckCorruption(pageIter);
      }
      catch (OAException &)
      {
        fn(pageIter, mStats.ObjectSize_);
        ++count;
      }
      pageIter += mInterSize;
    }
    pageListIter = pageListIter->Next;
  }

  return count;
}

/******************************************************************************/
/*!

  Frees empty pages. (Extra Credit. Tried, but doesn't work)
  
  \return
    the count of how many empty pages were freed
    
*/
/******************************************************************************/
unsigned ObjectAllocator::FreeEmptyPages()
{
  GenericObject* prevPage = NULL;
  GenericObject* currPage = mPageList;
  unsigned char * pageIter;
  unsigned char * pageStart;
  unsigned int count = 0;

  bool isEmpty = true;
  while (currPage) 
  {
    pageStart = reinterpret_cast<unsigned char*>(currPage);
    pageIter = pageStart + mLeftSize;

    while (static_cast<unsigned int>(pageIter - pageStart) < mStats.PageSize_)
    {
      if (!IsObjectFreeList(pageIter)) 
      {
        isEmpty = false;
        break;
      }
      pageIter += mInterSize;
    }

    if (isEmpty) 
    {
      GenericObject* toDelete = currPage;
      if (currPage == mPageList)
      {
        mPageList = currPage->Next;
      }
      currPage = currPage->Next;
      if (prevPage)
      {
        prevPage->Next = currPage;
      }
      FreePage(toDelete);
      isEmpty = true;
      ++count;
    }
    else
    {
      prevPage = currPage;
      currPage = currPage->Next;
    }


  }

  return count;
}

/******************************************************************************/
/*!

  Used to determine whether or not extra credit was done.
  
  \return
    Gives false if it wasn't, true if it was implemented.

*/
/******************************************************************************/
bool ObjectAllocator::ImplementedExtraCredit()
{
  return false;
}

/******************************************************************************/
/*!

  Set the state of the debug mode.
  
  \param State
    The state of debug if it is on or not.

*/
/******************************************************************************/
void ObjectAllocator::SetDebugState(bool State)
{
  mConfig.DebugOn_ = State;
}

/******************************************************************************/
/*!

  Gets the free list.
  
  \return
    Gives the free list.

*/
/******************************************************************************/
const void * ObjectAllocator::GetFreeList() const
{
  return mFreeList;
}

/******************************************************************************/
/*!

  Gets the page list.
  
  \return
    Gives the page list.

*/
/******************************************************************************/
const void * ObjectAllocator::GetPageList() const
{
  return mPageList;
}

/******************************************************************************/
/*!

  Gets the configuration.
  
  \return
    Gives the configuration.

*/
/******************************************************************************/
OAConfig ObjectAllocator::GetConfig() const
{
  return mConfig;
}

/******************************************************************************/
/*!

  Gets the stats.
  
  \return
    Gives the stats.

*/
/******************************************************************************/
OAStats ObjectAllocator::GetStats() const
{
  return mStats;
}

//Helper Functions


/******************************************************************************/
/*!

  Allocates a page.

*/
/******************************************************************************/
void ObjectAllocator::AllocatePage(void)
{
  try
  {
    if (mConfig.MaxPages_ != 0 && mStats.PagesInUse_ == mConfig.MaxPages_)
    {
      throw OAException(OAException::E_NO_PAGES, 
      "Cannot allocate new page. Max pages has been reached.");
    }
    unsigned char* newPage = new unsigned char[mStats.PageSize_];

    if (mConfig.DebugOn_)
    {
      memset(newPage, UNALLOCATED_PATTERN, mStats.PageSize_);
    }
    GenericObject* oldPage = mPageList;
    mPageList = reinterpret_cast<GenericObject*>(newPage);
    mPageList->Next = oldPage;

    InitPage(mPageList);
  }
  catch (std::bad_alloc &)
  {
    throw OAException(OAException::E_NO_MEMORY, "Not enough memory.");
  }
  ++mStats.PagesInUse_;
  mStats.FreeObjects_ += mConfig.ObjectsPerPage_;
}



/******************************************************************************/
/*!

  Adds an object to the free list mostly used after it was freed.
  
  \param Object
    the object that is being placed back on the list.

*/
/******************************************************************************/
void ObjectAllocator::AddToFreeList(void * Object)
{
  GenericObject* nextObj = mFreeList;
  mFreeList = reinterpret_cast<GenericObject*>(Object);
  mFreeList->Next = nextObj;
}



/******************************************************************************/
/*!

  Initializes a page.
  
  \param StartPage
    the starting page that will be initialized

*/
/******************************************************************************/
void ObjectAllocator::InitPage(GenericObject * StartPage)
{
  unsigned char* pageStart = reinterpret_cast<unsigned char*>(StartPage);
  unsigned char* pageIter = pageStart;
  if (mConfig.DebugOn_)
  {
    pageIter += sizeof(void*);
    SetNonDataPattern(&pageIter, mConfig.LeftAlignSize_);
    MoveFreeList(pageIter);
  }
  else
  {
    MoveFreeList(pageIter += mLeftSize);
  }

  if (mConfig.DebugOn_)
  {
    while (static_cast<unsigned int>((pageIter + mInterSize) - pageStart)
      < mStats.PageSize_)
    {
      SetMemNMove(&(pageIter += mStats.ObjectSize_), PAD_PATTERN, 
      mConfig.PadBytes_);
      SetNonDataPattern(&pageIter, mConfig.InterAlignSize_);
      MoveFreeList(pageIter);
    }
  }
  else
  {
    while (static_cast<unsigned int>((pageIter + mInterSize) - pageStart)
      < mStats.PageSize_)
    {
      MoveFreeList(pageIter += mInterSize);
    }
  }
  if (mConfig.DebugOn_)
  {
    SetMemNMove(&(pageIter += mStats.ObjectSize_), PAD_PATTERN,
    mConfig.PadBytes_);
  }
}



/******************************************************************************/
/*!

  Sets the memory and moves it to its desired location.
  
  \param Begin
    A char pointer to a pointer that is the start of the memory.
    
  \param Value
    the value of the memory.
    
  \param Size
    the size of the memory.

*/
/******************************************************************************/
void ObjectAllocator::SetMemNMove(unsigned char** Begin, int Value, size_t Size)
{
  memset(*Begin, Value, Size);
  *Begin += Size;
}



/******************************************************************************/
/*!

  Sets a non data pattern in memory.

  \param Begin
    A char pointer to a pointer that is the start of the memory.
    
  \param AlignSizeSize
    the size of the alignment.
    
*/
/******************************************************************************/
void ObjectAllocator::SetNonDataPattern(unsigned char** Begin, size_t AlignSize)
{
  SetMemNMove(Begin, ALIGN_PATTERN, AlignSize);

  SetMemNMove(Begin, 0, mConfig.HBlockInfo_.size_);

  SetMemNMove(Begin, PAD_PATTERN, mConfig.PadBytes_);
}

/******************************************************************************/
/*!

  Moves block of memory to the free list.

  \param Position
    A pointer to the position of the block in memory.
    
*/
/******************************************************************************/
void ObjectAllocator::MoveFreeList(unsigned char * Position)
{
  GenericObject* prevBlock = mFreeList;
  mFreeList = reinterpret_cast<GenericObject*>(Position);
  mFreeList->Next = prevBlock;
}

/******************************************************************************/
/*!

  Checks if an object is in the the free list.

  \param Object
    A pointer to an object in the free list.
    
  \return 
    returns if the block is in use or not and if it is in the list or not.
    
*/
/******************************************************************************/
bool ObjectAllocator::IsObjectFreeList(void * Object) const
{
  if (mConfig.HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbBasic 
  || mConfig.HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbExtended)
  {
    unsigned char* blockIter = reinterpret_cast<unsigned char*>(Object)
    - mConfig.PadBytes_ - sizeof(char);
    return *blockIter == 0;
  }
  else if (mConfig.HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbExternal)
  {
    unsigned char* blockIter = reinterpret_cast<unsigned char*>(Object) 
    - mConfig.PadBytes_ - mConfig.HBlockInfo_.size_;
    MemBlockInfo** blockInfoPtr = reinterpret_cast<MemBlockInfo**>(blockIter);
    MemBlockInfo* blockInfo = *blockInfoPtr;
    if (blockInfo)
    {
      return blockInfo->in_use;
    }
  }

  GenericObject* currObjInList = mFreeList;
  while (currObjInList)
  {
    if (currObjInList == Object)
    {
      return true;
    }
    currObjInList = currObjInList->Next;
  }
  return false;
}

/******************************************************************************/
/*!

  Deallocates the External Header.

  \param Object
    An object to be deallocated
    
*/
/******************************************************************************/
void ObjectAllocator::FreeExternalHeader(unsigned char * Object)
{
  MemBlockInfo** blockInfoPtr = reinterpret_cast<MemBlockInfo**>(Object);
  MemBlockInfo* blockInfo = *blockInfoPtr;
  if (blockInfo && blockInfo->label)
  {
    delete[](blockInfo->label);
  }
  delete blockInfo;
}

/******************************************************************************/
/*!

  Frees a page.

  \param pageHead
    the head of the page to be freed.
    
*/
/******************************************************************************/
void ObjectAllocator::FreePage(GenericObject * pageHead)
{
  unsigned char * pageStart = reinterpret_cast<unsigned char*>(pageHead);
  unsigned char * pageIter;
  GenericObject* currBlock = mFreeList;
  GenericObject* prevBlock = NULL;
  GenericObject* toDelete;
  bool isDeleted = false;
  while (currBlock)
  {
    pageIter = pageStart + mLeftSize;
    while (static_cast<unsigned int>(pageIter - pageStart) < mStats.PageSize_) 
    {
      toDelete = reinterpret_cast<GenericObject*>(pageIter);
      if (toDelete == currBlock)
      {
        if (mConfig.HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbExternal)
        {
          FreeExternalHeader(pageIter);
        }
        isDeleted = true;
        break;
      }

      pageIter += mInterSize;
    }

    if (isDeleted)
    {
      if (currBlock == mFreeList)
      {
        mFreeList = currBlock->Next;
      }
      currBlock = currBlock->Next;
      if (prevBlock)
      {
        prevBlock->Next = currBlock;
      }
      isDeleted = false;
    }
    else 
    {
      prevBlock = currBlock;
      currBlock = currBlock->Next;
    }

  }

  delete[](reinterpret_cast<unsigned char*>(pageHead));
}

/******************************************************************************/
/*!

  Checks the bounds of an object.

  \param Object
    A char pointer to a pointer that is the start of the memory.
    
*/
/******************************************************************************/
void ObjectAllocator::CheckBounds(unsigned char * Object) const
{
  GenericObject* currPage = mPageList;
  unsigned char* currPageStart = 0;

  while (currPage)
  {
    currPageStart = reinterpret_cast<unsigned char*>(currPage);
    if (Object > currPageStart && Object < (currPageStart + mStats.PageSize_))
    {
      break;
    }
    currPage = currPage->Next;
  }

  if (currPage)
  {
    unsigned char* firstBlock = currPageStart + mLeftSize;
    size_t blockDist = Object - firstBlock;
    if (blockDist % mInterSize != 0)
    {
      throw OAException(OAException::E_BAD_BOUNDARY,
      "Object is out of bounds.");
    }
  }
}

/******************************************************************************/
/*!

  Checks if an object has already been freed.

  \param Object
    the object that is being checked.
    
*/
/******************************************************************************/
void ObjectAllocator::CheckTwoFree(unsigned char * Object) const
{
  if (mStats.ObjectSize_ > sizeof(void*))
  {
    if (*(Object + sizeof(void*)) == FREED_PATTERN)
    {
      throw OAException(OAException::E_MULTIPLE_FREE,
      "Object was already freed.");
    }
  }
  else if (IsObjectFreeList(Object))
  {
    throw OAException(OAException::E_MULTIPLE_FREE,
    "Object was already freed.");
  }
}

/******************************************************************************/
/*!

  Checks if an object is corrupted.

  \param Object
    the object that is being checked.
    
*/
/******************************************************************************/
void ObjectAllocator::CheckCorruption(unsigned char * Object) const
{
  if (mConfig.PadBytes_ == 0)
  {
    return;
  }
  unsigned char* objStart = reinterpret_cast<unsigned char*>(Object);
  unsigned char* objEnd = objStart + mStats.ObjectSize_;

  unsigned char* padIter = objStart - mConfig.PadBytes_;

  while (padIter != objStart)
  {
    if (*(padIter++) != PAD_PATTERN)
    {
      throw OAException(OAException::E_CORRUPTED_BLOCK,
      "The front of this block is corrupted.");
    }
  }

  padIter = objEnd + mConfig.PadBytes_ - 1;
  while (padIter != objEnd)
  {
    if (*(padIter--) != PAD_PATTERN)
    {
      throw OAException(OAException::E_CORRUPTED_BLOCK, "The end of this block is corrupted.");
    }
  }
}
