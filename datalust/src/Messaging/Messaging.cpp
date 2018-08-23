#pragma once
/**
  \file   Messaging.cpp
  \author Taylor Osmond

  \brief Implementation of message passing system

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Messaging.hpp"
#include <algorithm>

namespace Messaging {
  
  namespace{

    //Removes stuff
    class Remover{
    public:
      bool operator()(const std::pair<ObjectId, void(*)(ObjectId, const Message&)>& o)
      {
        if(o.first != obj_)
          return false;

        return true;
      } 

      Remover(ObjectId obj) : obj_(obj) {}
    private:
      ObjectId obj_;
    };

    //Posts stuff
    class Poster{
    public:
      void operator()(const std::pair<ObjectId, void(*)(ObjectId, const Message&)>& o)
      {
        o.second(o.first, msg_);
      }

      Poster(const Message& msg) : msg_(msg) {}
    private:
      const Message& msg_;
    };
  }

  void MessageHandler::Post(Message msg)
  {
    //post stuff
    Poster poster(msg);
    std::for_each(callbacks_[(size_t)msg.id].begin(), callbacks_[(size_t)msg.id].end(), poster);
  }

  void MessageHandler::Register(ID id, ObjectId obj, void(* handler)(ObjectId, const Message&))
  {
    //make sure function pointer is valid
    if(!handler)
     return;

    callbacks_[(size_t)id].push_back(std::make_pair(obj, handler));
  }


  void MessageHandler::Unregister(ID id, ObjectId obj)
  {
    //remove stuff
    Remover remover(obj);
    callbacks_[(size_t)id].erase(std::remove_if(callbacks_[(size_t)id].begin(), callbacks_[(size_t)id].end(), remover), callbacks_[(size_t)id].end());
  }
}