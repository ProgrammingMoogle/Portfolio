/******************************************************************************/
/*!
  \file    GameObject.h
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/07/2017

  \brief
    interface for the GameObject class
    
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <string>
#include "Component.h"

class SlotMap;



class GameObject
{
    public:
		GameObject(); // default ctor doesn't do anything, is used for debug purpouses
   
	/**************************************************************************/
    /*!
        Ctor for the GameObject class create and object

		\param type
			The type of an object
        \param name
            The name is a way for us to differientiate between objects, not all 
            objects need a name however, only important ones
    */
    /**************************************************************************/
    GameObject(std::string type, std::string name = "");

    /**************************************************************************/
    /*!
        Add a component to the GameObject Takes a pointer to a componenet 
		(so it doesn't slice the object) and stores so that it is easily 
		accesible

        \param component
            the component that is being added to the GameObject
        
        \return
          return a refference to the componenet Added so the call can be chained
    */
    /**************************************************************************/
    template<typename T>
    GameObject& Add(T* component);
    
    /**************************************************************************/
    /*!
        equality operator for the GameObject class

        \param GameObject
          the other object we are comparing to

		\return
			return true if the objects are the same else return false
    */      
    /**************************************************************************/
    bool operator==(const GameObject& rhs) const;
    
    /**************************************************************************/
    /*!
        copy assignment operator for the GameObject class, takes a GameObject and 
        assigns another to it (utilizes the copy swap design pattern)

        \param GameObject
            the GameObject being assigned to 
    */
    /**************************************************************************/
	GameObject& operator=(GameObject const& rhs);


	/**************************************************************************/
	/*!
		move assignment operator for the GameObject class, takes a GameObject and
		moves it to another

		\param GameObject
		the GameObject being moved to
	*/
	/**************************************************************************/
	GameObject& operator=(GameObject&& rhs) noexcept;

    /**************************************************************************/
    /*!
        Find A component of type T attached to the GameObject const version    
        
		\return
            the component that was found, if no components were found returns 
			null
    */
    /**************************************************************************/
    template<typename T>
    const T* Find() const;

	/**************************************************************************/
	/*!
		Find A component of type T attached to the GameObject const version
		
		\return
		the component that was found, if no components were found returns
		null
	*/
	/**************************************************************************/
    template<typename T>
    T* Find();

    /**************************************************************************/
    /*!
        Copy constructor for the GameObject class, this will be used when
        instantiating object cloned from an archetype 
        (specifically when using data serialization)

        \param GameObject 
            A refference to the gameobject that is being cloned
    */
    /**************************************************************************/
	GameObject(const GameObject& other);// = delete;

    /**************************************************************************/
    /*!
        Move constructor for the GameObject class
        (specifically when using data serialization)

        \param GameObject 
            A r value reference to the gameobject that is being moved
    */
    /**************************************************************************/
    GameObject(GameObject&& o) noexcept;
    
    /**************************************************************************/
    /*!
        Destructor for the GameObject class, deletes all of the comoponents 
		attached to the object
    */
    /**************************************************************************/
    ~GameObject() noexcept;

    /**************************************************************************/
    /*!
        Flag a game object for destruction, This is to avoid game objects being 
        destroyed while they are being processed. 
    */
    /**************************************************************************/
    void Destroy();

    /**************************************************************************/
    /*!
        Check whether a game object has been flagged for destruction.

        \return
          returns true if it has been flaged else it returns false
    */
    /**************************************************************************/
    bool IsDestroyed();

    // any post-initialization steps for components done here
    void Start();

    /**************************************************************************/
    /*!
        Updates any components attached to the GameObject, does nothing if the
        GameObejct isn't valid
    */
    /**************************************************************************/
    void Update(float dt);
        
	/**************************************************************************/
	/*!
		Set the name of the object

		\param name
			the name you want to rename this object to

		\return
			returns a reference to the name of the object as a std::string
	*/
	/**************************************************************************/
	std::string& SetName(std::string name);

    /**************************************************************************/
    /*!
        returns the name of the object, if the object doesn't have a name it 
        will return nulptr

		\return
			the objects name
    */
    /**************************************************************************/
    std::string& GetName();

	/**************************************************************************/
	/*!
		const version of the GetName Function above
	*/
	/**************************************************************************/
	std::string const& GetName() const;

	/**************************************************************************/
	/*!
		Return the type of the GameObject

		\return
			The GameObjects type, its return is const because its type should
			never changed
	*/
	/**************************************************************************/
	std::string const& GetType() const;

    /**************************************************************************/
    /*!
        Set the Objectid of the GameObject
		
		\param 
			take and ObjectId and stores it in the GameObject and all of its 
			components, the ObjectId is taken as a reff so it can't be null 
		
		\return
			returns a refference to the Id so that calls can be chained
    /**************************************************************************/
    ObjectId const& SetObjectId(ObjectId& key);
	
	/**************************************************************************/
	/*!
		Swap to gameobjects

		\param rhs
			One of the objects being swaped
	*/
	/**************************************************************************/
	void swap(GameObject& rhs);

	//! Public Accesor functions
	void MoveVal(float MoveVal) { 
		moveVal_ = MoveVal; 
	}

	float MoveVal() { 
		return moveVal_; 
	}

	void PInput(bool PInput) { PInput_ = PInput; }

	bool PInput() { return PInput_; }

	void LifeTime(float LifeTime) { lifetime_ = LifeTime; }

	float LifeTime() { return lifetime_; }

	ObjectId GetKey() { return key_; }

  bool IsActive() { return Active; };

  void DeActivate() { Active = false; }
  void Activate() { Active = true; }
    private:
	void LifeTime_(float dt);

	//! The name & type of the object
	std::string name_ = "";
	std::string type_;

    //! a container for all of the objects components
    std::unordered_map<std::type_index, Component*> components_;

    //! Flag to indicate that the game object is dead and should be destroyed.
    bool isDestroyed_ = false;
	
	//! Flag to indicate whether the object has input
	bool PInput_ = false;

	//! value used to determine when an object screen wraps

	//! how long an object lasts a liftime of zero is considered infinite
	float lifetime_ = 0.0f;

	//! the movement speed and jump hight of the gameobject
	float moveVal_;

	//! the key of the object
	ObjectId key_ = nullobj;

  bool Active = true;
};

#include "game_object_timpl.cpp"

#endif //GAMEOBJECT_H
