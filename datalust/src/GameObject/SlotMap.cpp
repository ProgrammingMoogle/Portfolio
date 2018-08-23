/******************************************************************************/
/*!
  \file    SlotMap.cpp
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/07/2017

  \brief
	added game_state, game_state_manager, game_object, and SlotMap
	SlotMap.cpp contains the implementation for the SlotMap data structure.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "SlotMap.h"
/******************************************************************************/
/*
  stuff used for the SlotMap class
*/
/******************************************************************************/
// initialize the data to size
SlotMap::SlotMap(size_t size) : freeListHead_(0), freeListTail_(size - 1)
{
	// loop through the index array and set up the free list so that it starts at
	// the first index in the array and goes to the last index in the array
	index_.reserve(size);
	CreateFreeList(0, size);
	objectTable_.reserve(size);
	erase_.reserve(size);
}

// copy the vectors over
SlotMap::SlotMap(const SlotMap& o) :
	objectTable_(o.objectTable_), index_(o.index_), erase_(o.erase_),
	freeListHead_(o.freeListHead_), freeListTail_(o.freeListTail_)
{
}

// member swap function swaps all of the private members
void SlotMap::Swap(SlotMap& b)
{
	index_.swap(b.index_);
	erase_.swap(b.erase_);
	objectTable_.swap(b.objectTable_);
	std::swap(freeListHead_, b.freeListHead_);
	std::swap(freeListTail_, b.freeListTail_);
}

bool SlotMap::operator==(const SlotMap& rhs) const
{
	return (objectTable_ == rhs.objectTable_);
}

bool SlotMap::operator!=(const SlotMap& rhs) const
{
	return !(objectTable_ == rhs.objectTable_);
}

void SlotMap::Clear()
{
	this->~SlotMap();
	new (this) SlotMap;
}

void swap(SlotMap& a, SlotMap& b) {
	a.Swap(b);
}

size_t SlotMap::Size()
{
	return objectTable_.size();
}

size_t SlotMap::MaxSize()
{
	return objectTable_.max_size();
}
bool SlotMap::Empty()
{
	return objectTable_.empty();
}

void SlotMap::CreateFreeList(const size_t begining, const size_t end)
{
	for (unsigned int i = begining; i < end - 1; ++i)
	{
		index_.push_back(std::make_pair(i + 1, 0));
	}
	// the last index must point to itself
	index_.push_back(std::make_pair(end - 1, 0));
}

void SlotMap::Grow()
{
	size_t i = index_.capacity();
	// call resize to add space to all three of the vectors
	index_.reserve(index_.capacity() * 2);
	objectTable_.reserve(index_.capacity() * 2);
	erase_.reserve(index_.capacity() * 2);

	i = index_.capacity();

	// store the first new element and the last element in the array
	size_t begin = index_.size(), end = index_.capacity();

	// create a new free list in the newly alocated indices
	freeListHead_ = begin;
	freeListTail_ = end - 1;
	CreateFreeList(begin, end);
}

ObjectId SlotMap::Insert(GameObject& obj)
{
	return Insert(std::move(obj));
}

ObjectId SlotMap::Insert(GameObject&& obj)
{
	// check if the free list is empty, if so allocate additional space
	if (freeListHead_ == (size_t)-1)
	{
		Grow();
	}
	// generate a key that will point into the index table
	ObjectId key;
	key.first = (freeListHead_), key.second = index_[freeListHead_].second;

	// push the free list to point somewhere else
	if (freeListHead_ == freeListTail_)
	{
		freeListHead_ = -1;
		freeListTail_ = -1;
	}

	else
		freeListHead_ = index_[freeListHead_].first;

	// change the key in the data table so that it points to the new data member
	index_[key.first].first = (objectTable_.size());

	// call pushback on the array to add the object at the end of the data table
	objectTable_.push_back(std::move(obj));

	// push back the index of the key in the index table on the erase vector
	erase_.push_back(key.first);

	return key;
}

void SlotMap::Erase(ObjectId key)
{
	// check if the user is looking for old/new data if so throw
	if (key.second != index_[key.first].second || key == nullobj)
	{
		return;
	}

	if (index_[key.first].first != objectTable_.size() - 1)
	{
		objectTable_[index_[key.first].first].swap(objectTable_[objectTable_.size() - 1]);
	}
	// decrement the size of the vector
	objectTable_.resize(objectTable_.size() - 1);

	// do the same thing to the erase table that was done with the objectTable_

	if (index_[key.first].first != erase_.size() - 1)
	{
		erase_[index_[key.first].first] = std::move(erase_[erase_.size() - 1]);
	}
	index_[erase_[index_[key.first].first]].first = index_[key.first].first;
	erase_.resize(erase_.size() - 1);

	// check if there are any spots left in the free list, and if not point it to
	// the object we just freed (this is an edge case)
	if (freeListTail_ == -1)
	{
		freeListHead_ = key.first;
		freeListTail_ = key.first;
	}
	index_[freeListTail_].first = key.first;
	freeListTail_ = key.first;
	// increment the generation data
	++index_[freeListTail_].second;
}

GameObject& SlotMap::get_object(const ObjectId key)
{
	// check if the user is looking for old/new data if so throw
	//if (!(key.first < index_.size()))
	//{
	//	throw int(-1);
	//}
	if (key.second != index_[key.first].second /*|| key == nullobj*/ )
	{
		throw int(-1);
	}
	// return the object that they were looking for
	return objectTable_.at(index_[key.first].first);
}

GameObject& SlotMap::get_object(const std::string name)
{
	// check if the user is looking for old/new data if so throw
	for (auto it = objectTable_.begin(); it != objectTable_.end(); ++it)
	{
		if (it->GetName() == name)
			return *it;
	}
	throw int(-1);
}

// assign all of the private variables
SlotMap& SlotMap::operator=(const SlotMap& rhs)
{
	objectTable_ = rhs.objectTable_;
	erase_ = rhs.erase_;
	index_ = rhs.index_;
	freeListHead_ = rhs.freeListHead_;
	freeListTail_ = rhs.freeListTail_;
	return *this;
}

SlotMap::Iterator SlotMap::Begin()
{
	return Iterator(&objectTable_[0], *this);
}

SlotMap::Iterator SlotMap::End()
{
	return Iterator(&objectTable_[objectTable_.size()], *this);
}

SlotMap::ConstIterator SlotMap::Cbegin()
{
	return ConstIterator(&objectTable_[0], *this);
}

SlotMap::ConstIterator SlotMap::Cend()
{
	return ConstIterator(&objectTable_[objectTable_.size()], *this);
}
/******************************************************************************/
/*
  stuff used for the Iterator class
*/
/******************************************************************************/
SlotMap::Iterator::Iterator(GameObject* rawPointer, SlotMap& container) :
	container_(container)
{
	rawPointer_ = rawPointer;
}

SlotMap::Iterator::Iterator(const ObjectId& key, SlotMap& container) :
	container_(container)
{
	if (key == nullobj)
		*this = container.End();
	else
		rawPointer_ = &container.objectTable_[container.index_[key.first].first];
}

bool SlotMap::Iterator::operator!=(const Iterator& rhs) const
{
	return !(rawPointer_ == rhs.rawPointer_);
}

GameObject& SlotMap::Iterator::operator*() const
{
	return *rawPointer_;
}

GameObject* SlotMap::Iterator::operator->() const
{
	return rawPointer_;
}

SlotMap::Iterator& SlotMap::Iterator::operator++()
{
	++rawPointer_;
	return *this;
}

SlotMap::Iterator SlotMap::Iterator::operator++(int)
{
	Iterator temp = *this;
	++(*this);
	return temp;
}

SlotMap::Iterator& SlotMap::Iterator::operator--()
{
	--rawPointer_;
	return *this;
}

SlotMap::Iterator SlotMap::Iterator::operator--(int)
{
	Iterator temp = *this;
	--(*this);
	return temp;
}

SlotMap::Iterator& SlotMap::Iterator::operator=(const SlotMap::Iterator& rhs)
{
	rawPointer_ = rhs.rawPointer_;
	container_ = rhs.container_;
	return *this;
}

bool SlotMap::Iterator::operator<(const SlotMap::Iterator& rhs) const
{
	return rawPointer_ < rhs.rawPointer_;
}

bool SlotMap::Iterator::operator<(SlotMap::ConstIterator& rhs) const
{
	return rawPointer_ < rhs.rawPointer_;
}
/******************************************************************************/
/*
  stuff used for the ConstIterator class
*/
/******************************************************************************/
SlotMap::ConstIterator::ConstIterator(
	GameObject* rawPointer, SlotMap& container) : container_(container)
{
	rawPointer_ = rawPointer;
}

SlotMap::ConstIterator::ConstIterator(
	const ObjectId& key, SlotMap& container) : container_(container)
{
	if (key == nullobj)
		*this = container.End();
	else
		rawPointer_ = &container.objectTable_[container.index_[key.first].first];
}

bool SlotMap::ConstIterator::operator!=(ConstIterator& rhs) const
{
	return !(rawPointer_ == rhs.rawPointer_);
}

const GameObject& SlotMap::ConstIterator::operator*() const
{
	return *rawPointer_;
}

const GameObject* SlotMap::ConstIterator::operator->() const
{
	return rawPointer_;
}

SlotMap::ConstIterator& SlotMap::ConstIterator::operator++()
{
	++rawPointer_;
	return *this;
}

SlotMap::ConstIterator SlotMap::ConstIterator::operator++(int)
{
	ConstIterator temp = *this;
	++(*this);
	return temp;
}

SlotMap::ConstIterator& SlotMap::ConstIterator::operator--()
{
	--rawPointer_;
	return *this;
}

SlotMap::ConstIterator SlotMap::ConstIterator::operator--(int)
{
	ConstIterator temp = *this;
	--(*this);
	return temp;
}

SlotMap::ConstIterator& SlotMap::ConstIterator::operator=(
	SlotMap::ConstIterator& rhs)
{
	rawPointer_ = rhs.rawPointer_;
	container_ = rhs.container_;
	return *this;
}

SlotMap::ConstIterator& SlotMap::ConstIterator::operator=(
	const SlotMap::Iterator& rhs)
{
	rawPointer_ = rhs.rawPointer_;
	container_ = rhs.container_;
	return *this;
}

bool SlotMap::ConstIterator::operator<(const SlotMap::Iterator& rhs) const
{
	return rawPointer_ < rhs.rawPointer_;
}

bool SlotMap::ConstIterator::operator<(SlotMap::ConstIterator& rhs) const
{
	return rawPointer_ < rhs.rawPointer_;
}
