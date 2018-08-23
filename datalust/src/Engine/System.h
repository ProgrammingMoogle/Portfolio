/******************************************************************************/
/*!
	\file    Engine.h
	\author  Zayd Gaudet
	\par     zayd.gaudet\@digipen.edu
	\par     09/29/2017
	
	\brief
		System base class for all systems to derive from
	
	All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef SYSTEM_H
#define SYSTEM_H

class System
{
public:
  // virtual functions that all systems should implement
  virtual bool Init() {return true;}
  virtual void Update(float) {}
  virtual void Shutdown() {}
  virtual ~System() {}
};

#endif //SYSTEM_H