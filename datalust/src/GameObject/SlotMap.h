/******************************************************************************/
/*!
  \file    slot_map.h
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/07/2017
  
  \brief
  slot_map.h contains the definition for the slot_map and assosciated 
  iterator classes. The slot_map will be used as a container for the 
  gameobject manager.
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef SLOT_MAP_H
#define SLOT_MAP_H

#include <vector>
#include <utility>
#include <string>
#include <Util/Util.hpp>
#include "GameObject.h"

/******************************************************************************/
/*!
  The slot_map consists of three vectors wich contain the data, the index of the
  data, and a list of valid indices, this is used to keeps track of erased data
  in order to avoid unecesary allocations and to add safety gaurentees. 
  furthermore it is compatible with standard library algorithms
*/
/******************************************************************************/
class SlotMap
  {
  public:

      // declare ConstIterator class so the compiler realizes that it exists
      class ConstIterator;
      
      /************************************************************************/
      /*!
        A bidirectionaly iterator class used to travers the data table of the 
        container
      */
      /************************************************************************/
      class Iterator 
      {
        public:
          friend class ConstIterator;          
          /********************************************************************/
          /*!
            Non-default constructor for the slot_map iterator class, this is 
            mean't to be used by systems that don't have acces to private members

            \param const object_id
              the key that is used to determine where the iterator should be 
              pointing to when it has been constructed, it will also test to 
              make sure the generation data matches to ensure the data is right
            
            \param slot_map
              the container that the iterator will be assosciated with once it 
              has be constructed
          */
          /********************************************************************/
          Iterator(const ObjectId& key, SlotMap& container);

          /********************************************************************/
          /*!
            Overloaded Non-default constructor for the slot_map iterator class, 
            this is mean't to be used by systems that have acces to private 
            members

            \param game_object
              this is a pointer that points to where the iterator will point, it 
              will also test to make sure the generation data matches to ensure 
              the data is right
            
            \param slot_map
              the container that the iterator will be assosciated with once it 
              has be constructed
          */
          /********************************************************************/
          Iterator( GameObject* raw_pointer, SlotMap& container);

          /********************************************************************/
          /*!
            assignment operator for the slot_map iterator class, assigns
            one iterator to another

            \param iterator
              the iterator whos values are being used to change another
        
            \return
              returns a reference to a iterator so that assignments can be 
              chained
          */
          /********************************************************************/
          Iterator& operator=(const Iterator& rhs); 
          
          /********************************************************************/
          /*!
            != operater that compares to iterators by the pointers inside

            \param const iterator
              the r value iterator that is being compared  

            \return
              returns true if the iterators aren't = and false if they are
          */
          /********************************************************************/
          bool operator!=(const Iterator& rhs) const;

          /********************************************************************/
          /*!
            != operater that compares to iterators by the pointers inside

            \param const iterator
              the r value iterator that is being compared  

            \return
              returns true if the iterators aren't = and false if they are
          */
          /********************************************************************/
          bool operator<(const Iterator& rhs) const;

          /********************************************************************/
          /*!
            != operater that compares to iterators by the pointers inside

            \param const iterator
              the r value iterator that is being compared  

            \return
              returns true if the iterators aren't = and false if they are
          */
          /********************************************************************/
          bool operator<(ConstIterator& rhs) const;

          /********************************************************************/
          /*!
            derefference operater used to grab data from the data table

            \return
              returns a refference to the game_object the iterator is poiting to
          */
          /********************************************************************/          
          GameObject& operator*() const;
          
          /********************************************************************/
          /*!
            arrow operator used to grab members of the game_object class
         
            \return
              returns a pointer to the game_object the iterator is poiting to
          */
          /********************************************************************/
          GameObject* operator->() const;

          /********************************************************************/
          /*!
            pre-increment operator increments the pointer (not the data inside)
         
            \return
              returns a refferece to the iterator that is pointing to a new 
              location
          */
          /********************************************************************/
          Iterator& operator++();
          
          /********************************************************************/
          /*!
            post-increment operator creates a temp value, increments the pointer
            (not the data inside) and the gives the back the temp value
         
            \return
              returns a temp refferece to the iterator that hasn't been 
              incremented
          */
          /********************************************************************/
          Iterator operator++(int);

          /********************************************************************/
          /*!
            pre-decrement operator decrements the pointer (not the data inside)
         
            \return
              returns a refferece to the iterator that is pointing to a new 
              location
          */
          /********************************************************************/
          Iterator& operator--();

          /********************************************************************/
          /*!
            post-decrement operator creates a temp value, decrements the pointer
            (not the data inside) and the gives the back the temp value
         
            \return
              returns a temp refferece to the iterator that hasn't been 
              decremented
          */
          /********************************************************************/
          Iterator operator--(int);
        private:
          //! the pointer the iterator is wraping around, points to game_objects
          GameObject* rawPointer_;
          //! the container assosciated with the iterator
          SlotMap& container_;
      };

      /************************************************************************/
      /*!
        A bidirectionaly const iterator class used to travers the data table of 
        the container so that the user can look at the data with their eyes, but
        not their hands
      */
      /************************************************************************/
      class ConstIterator 
      {
        public:
          friend class Iterator;
          /********************************************************************/
          /*!
            const version of the Non-default constructor found above

            \param const object_id
              the key that is used to determine where the iterator should be 
              pointing to when it has been constructed, it will also test to 
              make sure the generation data matches to ensure the data is right
            
            \param slot_map
              the container that the iterator will be assosciated with once it 
              has be constructed
          */
          /********************************************************************/
          ConstIterator(const ObjectId& key, SlotMap& container);

          /********************************************************************/
          /*!
             const version of the Overloaded Non-default constructor found above

            \param game_object
              this is a pointer that points to where the iterator will point, it 
              will also test to make sure the generation data matches to ensure 
              the data is right
            
            \param slot_map
              the container that the iterator will be assosciated with once it 
              has be constructed
          */
          /********************************************************************/
          ConstIterator(GameObject* rawPointer, SlotMap& container);

          /********************************************************************/
          /*!
            assignment operator for the slot_map const_iterator class, assigns
            one const_iterator to another

            \param const_iterator
              the const_iterator whos values are being used to change another
        
            \return
              returns a reference to a const_iterator so that assignments can be 
              chained
          */
          /********************************************************************/
          ConstIterator& operator=(ConstIterator& rhs); 
          
          /********************************************************************/
          /*!
            assignment operator for the slot_map const_iterator class, assigns
            an iterator to a const_iterator

            \param iterator
              the const_iterator whos values are being used to change another
        
            \return
              returns a reference to a const_iterator so that assignments can be 
              chained
          */
          /********************************************************************/
          ConstIterator& operator=(const SlotMap::Iterator& rhs); 
          
          /********************************************************************/
          /*!
            const version of the != operater found above

            \param const iterator
              the r value iterator that is being compared  

            \return
              returns true if the iterators aren't = and false if they are
          */
          /********************************************************************/
          bool operator!=(ConstIterator& rhs) const;

          /********************************************************************/
          /*!
            != operater that compares to iterators by the pointers inside

            \param const iterator
              the r value iterator that is being compared  

            \return
              returns true if the iterators aren't = and false if they are
          */
          /********************************************************************/
          bool operator<(const Iterator& rhs) const;

          /********************************************************************/
          /*!
            != operater that compares to iterators by the pointers inside

            \param const iterator
              the r value iterator that is being compared  

            \return
              returns true if the iterators aren't = and false if they are
          */
          /********************************************************************/
          bool operator<(ConstIterator& rhs) const;
          /********************************************************************/
          /*!
            const version of the derefference operater found above

            \return
              returns a refference to the game_object the iterator is poiting to
          */
          /********************************************************************/
          const GameObject& operator*() const; 

          /********************************************************************/
          /*!
            const version of the arrow operator found above
         
            \return
              returns a pointer to the game_object the iterator is poiting to
          */
          /********************************************************************/
          const GameObject* operator->() const;

          /********************************************************************/
          /*!
            const version of the pre-increment operator found above
         
            \return
              returns a refferece to the iterator that is pointing to a new 
              location
          */
          /********************************************************************/
          ConstIterator& operator++();

          /********************************************************************/
          /*!
            const version of the post-increment operator found above
         
            \return
              returns a temp refferece to the iterator that hasn't been 
              incremented
          */
          /********************************************************************/
          ConstIterator operator++(int);

          /********************************************************************/
          /*!
            const version of the pre-decrement operator found above

            \return
              returns a refferece to the iterator that is pointing to a new 
              location
          */
          /********************************************************************/
          ConstIterator& operator--();

          /********************************************************************/
          /*!
            const version of the post-decrement operator found above
            (not the data inside) and the gives the back the temp value
         
            \return
              returns a temp refferece to the iterator that hasn't been 
              decremented
          */
          /********************************************************************/
          ConstIterator operator--(int);
        private:
          //! the pointer the iterator is wraping around, points to game_objects
          GameObject* rawPointer_;
          //! the container assosciated with the iterator
          SlotMap& container_;
      };
      //! make the iterator class a friend of the slot_map so that it can acces 
      //! the data table
      friend class iterator;

      /************************************************************************/
      /*!
        default constructor for the slot_map, initializes the slot_map so that
        all of the vectors start with a size of 12 unless otherwise specified,
        it also initializes the free list

        \param const size_t
          the size to initialize the vectors to, defaults to 12
      */
      /************************************************************************/
      SlotMap(const size_t size = 1000);

      /************************************************************************/
      /*!
        copy constructor for the slot_map, coppies all of the vectors and the 
        free list head/tail 

        \param const slot_map
          the slot map that is being copied
      */
      /************************************************************************/
      SlotMap(const SlotMap& o);

	  /************************************************************************/
	  /*!
			Default destructor for the SlotMap class
	  */
	  /************************************************************************/
	  ~SlotMap() = default;

      /************************************************************************/
      /*!
        assignment operator for the slot_map class, assigns all of the value of 
        the slot_map on the right to the slot_map on the left

        \param const slot_map
          the slot_map whos values are being used to change another
        
        \return
          returns a reference to a slot_map so that assignments can be chained
      */
      /************************************************************************/
      SlotMap& operator=(const SlotMap& rhs);

      /************************************************************************/
      /*!
        creates a new iterator

        \return
          returns an iterator pointing to the begining of the data table  
      */
      /************************************************************************/      
      Iterator Begin();

      /************************************************************************/
      /*!
        creates a new iterator

        \return
          returns an iterator pointing to the end of the data table  
      */
      /************************************************************************/
      Iterator End();

      /************************************************************************/
      /*!
        creates a new constant iterator

        \return
          returns a const iterator pointing to the begining of the data table  
      */
      /************************************************************************/
      ConstIterator Cbegin();

      /************************************************************************/
      /*!
        creates a new constant iterator

        \return
          returns a const iterator pointing to the end of the data table  
      */
      /************************************************************************/
      ConstIterator Cend();

      /************************************************************************/
      /*!
        equality operator, if the data tables in both slot_maps are identicle 
        they are deemed equal

        \param slot_map
          the rvalue slot_map being compared

        \return
          returns true if the data tables are the same, otherwise returns false  
      */
      /************************************************************************/
      bool operator==(const SlotMap& rhs) const;

      /************************************************************************/
      /*!
        inequality operator, if the data tables in both slot_maps aren't 
        identicle they are deemed inequal

        \param slot_map
          the rvalue slot_map being compared

        \return
          returns true if the data tables aren't the same, otherwise returns 
          false  
      */
      /************************************************************************/
      bool operator!=(const SlotMap& rhs) const;

      /************************************************************************/
      /*!
        swap member function swaps the values of the l and r slot_maps

        \param slot_map
          the rvalue slot_map being compared 
      */
      /************************************************************************/
      void Swap(SlotMap& b);

      /************************************************************************/
      /*!
        finds the number of elements in the data table, and the erase table as 
        they are gaurenteed to be contiguos at all times

        \return
          returns true if the data tables are the same, otherwise returns false  
      */
      /************************************************************************/
      size_t Size();

      /************************************************************************/
      /*!
        Finds the maximum number of elements the container is able to hold due 
        to system or library implementation limitations

        \return
          returns the size found 
      */
      /************************************************************************/
      size_t MaxSize();

      /************************************************************************/
      /*!
        Checks if the container has no elements

        \return
          true if the container is empty, false otherwise 
      */
      /************************************************************************/
      bool Empty();

      /************************************************************************/
      /*!
        Inserts an element at a location determined by the slot_map based on 
        available space. 

        \param game_object
          the object that is being inserted into the slot_map

        \return
          an object key the points in to the index vector
      */
      /************************************************************************/
      ObjectId Insert(GameObject&& rhs);

      /************************************************************************/
      /*!
        const l value version of the insert function above

        \param game_object
          the object that is being inserted into the slot_map

        \return
          an object key the points in to the index vector
      */
      /************************************************************************/
      ObjectId Insert(GameObject& rhs);

      /************************************************************************/
      /*!
        erases an element then moves the element at the end to fill the gap. 
        it keeps track of the space that was erased so it can be reused later

        \param object_id
          the key pointing to the index that points to the data to be erased
      */
      /************************************************************************/
      void Erase(const ObjectId key);

      /************************************************************************/
      /*!
        given a key into the index table it checks if the generation is the same
        and if so finds the object

        \param object_id
          the name of the object we are looking for
        \return
          returns the object that was found
      */
      /************************************************************************/
      GameObject& get_object(const ObjectId key);

      /************************************************************************/
      /*!
        given the name of an object it searches through the object table untill
        it finds an object with a matching name, this will not work if multiple
        objects have the same name

        \param std::sting
          the key pointing to the index that points to the data to be erased

        \return
          returns the object that was found
      */
      /***********************************************************************/
      GameObject& get_object(const std::string name);

	  /***********************************************************************/
	  /*!
			calls the slot map destructor and then reconstructs it as a new 
			slot_map
	  */
	  /***********************************************************************/
	  void Clear();
    private:
      //! private member function to init the free list within a given range
      void CreateFreeList(const size_t begining, const size_t end);
      //! grow the slot map so that it can fit more objects
      void Grow();
      //! the table where all the objects are sSlotMaptored in
      std::vector<GameObject> objectTable_;
      //! the vector that contains generation data and idicies of the objects
      std::vector<ObjectId> index_;
      //! vector that contains all of the indices of all the valid indices in 
      //! the index table
      std::vector<int> erase_;
      //! the head of the free list, points into the index table
      size_t freeListHead_;
      //! the tail of the free list, points to index of the object most recently 
      //! erased
      size_t freeListTail_;
  };

  //! non member non friend swap function, calls the member function
  void Swap(SlotMap& a, SlotMap& b);

#endif
