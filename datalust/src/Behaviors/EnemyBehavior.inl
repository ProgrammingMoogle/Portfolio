#pragma once

template<typename T>
void EnemyBehavior::OnCollision(ObjectId obj, Messaging::Message const & m)
{
  KPE::RigidBody *body = m.message.collision.body_;
  KPE::RigidBody *otherBody = m.message.collision.otherBody_;
  if (body == nullptr || otherBody == nullptr)
    return;

  if (otherBody->GetParent().GetKey() == obj)
    otherBody->GetParent().Find<T>()->OnCollisionEnter(*otherBody, *body, m.message.collision);
  else if (body->GetParent().GetKey() == obj)
    body->GetParent().Find<T>()->OnCollisionEnter(*body, *otherBody, m.message.collision);
}