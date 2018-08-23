/******************************************************************************/
/*!
\file    GameObject.cpp
\author  Zayd Gaudet
\par     zayd.gaudet\@digipen.edu
\par     09/07/2017

\brief
	Template implementation for the GameObject

All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <algorithm>



// find the componenet of Type type
template<typename T>
T* GameObject::Find() 
{
  if (components_.empty())
    return nullptr;
	// find the component at type_index  T
	auto component = components_.find(std::type_index(typeid(T)));
	
	// check if the value found was at the end of the map if so return null, else return the component found 
	return (component == components_.end()) ? nullptr : static_cast<T*>(component->second);
}

// find the component of Type type, but const
template<typename T>
T const * GameObject::Find() const 
{
	// find the component at type_index  T
	auto component = components_.find(std::type_index(typeid(T)));
	
	// check if the value found was at the end of the map if so return null, else return the component found 
	return (component == components_.end()) ? nullptr : static_cast<T const *>(component->second);
}

template<typename T>
GameObject& GameObject::Add(T* component) {
  components_[std::type_index(typeid(T))] = static_cast<Component*>(component);
  return *this;
}