/**
  \file   PunchBehaviors.hpp
  \author Zayd Gaudet

  \brief Behavior component for punches

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <GameObject\Component.h>
#include <Messaging\Messaging.hpp>


/**
 * \brief 
 */
class PunchBehavior : public Component {
public:
	PunchBehavior(GameObject &parent, float dmg, float scalar);
	~PunchBehavior();
	void Update(float dt);
	virtual Component* Clone_() const { return new PunchBehavior(*this); }
	void AttachPlayer(ObjectId player);
	static void Punch(ObjectId, const Messaging::Message&);
	static void Collision(ObjectId, const Messaging::Message&);
private:
	ObjectId player_ = nullobj;
	Transform* ptransform;
	float dmg_, scalar_;
	int frame = 0;
	void onPunch(const Messaging::Message&);
	void onCollision(const Messaging::Message&);
	bool collided = false;
};