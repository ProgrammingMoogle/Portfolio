#pragma once
#include <GameObject\Component.h>
#include <Messaging\Messaging.hpp>


class UIDmgCounterBehavior : public Component {
public:
	UIDmgCounterBehavior(GameObject &parent, float dmg, float scalar);
	~UIDmgCounterBehavior();
	void Update(float dt);
	virtual Component* Clone_() const { return new UIDmgCounterBehavior(*this); }
	void AtachPlayer(ObjectId player);
	static void Punch(ObjectId, const Messaging::Message&);
	static void Collision(ObjectId, const Messaging::Message&);
private:
	
  
};