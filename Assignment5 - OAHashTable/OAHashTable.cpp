#include <cmath>
#include <cstring>

#include "OAHashTable.h"
#include "Support.h"

template<typename T>
OAHashTable<T>::OAHashTable(const OAHTConfig& Config)
  :mConfig(Config), mTable(NULL), mStats()
{
  // Puts stats struct in a useful state.
  mStats.TableSize_ = mConfig.InitialTableSize_;
  mStats.PrimaryHashFunc_ = mConfig.PrimaryHashFunc_;
  mStats.SecondaryHashFunc_ = mConfig.SecondaryHashFunc_;

  InitTable();
}

template<typename T>
OAHashTable<T>::~OAHashTable()
{
  clear();
  delete[] mTable;
}

template<typename T>
void OAHashTable<T>::insert(const char *key, const T& data)
throw(OAHashTableException)
{
  unsigned hash = AlphaHash(key);

  unsigned step = 1;

  if (mConfig.SecondaryHashFunc_ != NULL)
  {
    step = BetaHash(key);
  }

  for (unsigned i = 0; i < mStats.TableSize_; ++i)
  {
    ++mStats.Probes_;

    GrowTable();

    hash = AlphaHash(key);


    if (mTable[IndexHelp(hash, step, i)].State !=
      OAHashTable::OAHTSlot::OCCUPIED)
    {
      mTable[IndexHelp(hash, step, i)].State =
        OAHTSlot::OCCUPIED;
      if (mTable[IndexHelp(hash, step, i)].Key != key)
      {
        mTable[IndexHelp(hash, step, i)].Data = data;
        strcpy(mTable[IndexHelp(hash, step, i)].Key, key);
      }
      ++mStats.Count_;

      return;
    }
    else
    {
      if (strcmp(key, mTable[IndexHelp(hash, step, i)].Key) == 0)
      {
        throw(OAHashTableException(OAHashTableException::E_DUPLICATE,
          "Key is already in table."));

      }
    }
  }
}

template<typename T>
void OAHashTable<T>::remove(const char *Key)
throw(OAHashTableException)
{
  OAHTSlot *tmp;
  int index = IndexOf(Key, tmp);

  if (index != -1)
  {
    RemoveHelp(*tmp);

    if (mConfig.DeletionPolicy_ == PACK)
    {
      // put everything back after removal
      for(int i = 0;
        static_cast<unsigned>(i) < mStats.TableSize_ - 1 &&
        mTable[IndexHelp(index + 1, 1, i)].State == OAHTSlot::OCCUPIED;
        ++i)
      {
        mTable[IndexHelp(index + 1, 1, i)].State = OAHTSlot::UNOCCUPIED;
        --mStats.Count_;
        insert(mTable[IndexHelp(index + 1, 1, i)].Key,
          mTable[IndexHelp(index + 1, 1, i)].Data);
      }
    }
  }
  else
  {
    throw(OAHashTableException(
      OAHashTableException::E_ITEM_NOT_FOUND,
      "Key is not in table."));
  }
}

template<typename T>
const T& OAHashTable<T>::find(const char *Key) const
throw(OAHashTableException)
{
  OAHTSlot *tmp;
  int index = IndexOf(Key, tmp);

  if (index != -1)
  {
    return(tmp->Data);
  }
  else
  {
    throw(OAHashTableException(
      OAHashTableException::E_ITEM_NOT_FOUND,
      "Item could not be found in table."));
  }
}

template<typename T>
void OAHashTable<T>::clear()
{
  for (unsigned i = 0; i < mStats.TableSize_; ++i)
  {
    if (mTable[i].State == OAHTSlot::OCCUPIED)
    {
      RemoveHelp(mTable[i]);
    }
    mTable[i].State = OAHTSlot::UNOCCUPIED;
  }
}

template<typename T>
OAHTStats OAHashTable<T>::GetStats() const
{
  return(mStats);
}

template<typename T>
const typename OAHashTable<T>::OAHTSlot *OAHashTable<T>::GetTable() const
{
  return(mTable);
}

template<typename T>
void OAHashTable<T>::InitTable()
{
  try
  {
    mTable = new OAHTSlot[mStats.TableSize_];
  }
  catch (std::bad_alloc)
  {
    throw(OAHashTableException(
      OAHashTableException::E_NO_MEMORY,
      "Failed to allocate table. Not enough memory."));
  }

  for (unsigned i = 0; i < mStats.TableSize_; ++i)
  {
    mTable[i].State = OAHTSlot::UNOCCUPIED;
  }

  mStats.Count_ = 0;
}

template<typename T>
void OAHashTable<T>::GrowTable() throw(OAHashTableException)
{
  double factor = std::ceil(mStats.TableSize_ * mConfig.GrowthFactor_);
  unsigned old_size = mStats.TableSize_;

  mStats.TableSize_ = GetClosestPrime(static_cast<unsigned>(factor));

  OAHTSlot* temp = mTable;

  InitTable();

  // Insert everything in old table into new table...
  for (unsigned i = 0; i < old_size; ++i)
  {
    if (temp[i].State == OAHTSlot::OCCUPIED)
    {
      insert(temp[i].Key, temp[i].Data);
    }
  }

  delete[] temp;

  ++mStats.Expansions_;
}

template<typename T>
int OAHashTable<T>::IndexOf(const char *Key, OAHTSlot* &Slot) const
{
  unsigned hash = AlphaHash(Key);
  unsigned stride = 1;

  if (mStats.SecondaryHashFunc_ != NULL)
  {
    stride = BetaHash(Key);
  }

  for (unsigned i = 0; i < mStats.TableSize_; ++i)
  {
    ++mStats.Probes_;
    if (mTable[IndexHelp(hash, stride, i)].State == OAHTSlot::OCCUPIED
      && strcmp(Key, mTable[IndexHelp(hash, stride, i)].Key) == 0)
    {
      Slot = &mTable[IndexHelp(hash, stride, i)];

      return(static_cast<int>(IndexHelp(hash, stride, i)));
    }

    // What if we hit an unoccupied spot?
    if (mTable[IndexHelp(hash, stride, i)].State == OAHTSlot::UNOCCUPIED)
    {
      break;
    }
  }

  return(-1);
}

template<typename T>
void OAHashTable<T>::RemoveHelp(OAHTSlot &to_remove) throw(OAHashTableException)
{
  if (NULL != mConfig.FreeProc_)
  {
    (*mConfig.FreeProc_)(to_remove.Data);
  }

  to_remove.State = mConfig.DeletionPolicy_ == MARK ? OAHTSlot::DELETED : OAHTSlot::UNOCCUPIED;

  --mStats.Count_;
}

template<typename T>
int OAHashTable<T>::IndexHelp(const unsigned &hash, const unsigned &stride, const unsigned& index) const
{
  return((hash + index * stride) % mStats.TableSize_);
}

template<typename T>
unsigned OAHashTable<T>::AlphaHash(const char* key) const
{
  return((*mStats.PrimaryHashFunc_)(key, mStats.TableSize_));
}

template<typename T>
unsigned OAHashTable<T>::BetaHash(const char* key) const
{
  return((*mStats.SecondaryHashFunc_)(key, mStats.TableSize_ - 1) + 1);
}