/******************************************************************************/
/*!
  \file   Behaviors.cpp
  \author Sarah Janzen
  \par    c.janzen\@digipen.edu
  \date   9/20/17
  \brief
    Implementation for Behaviors Component.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Behaviors.hpp"
#include <Engine/Engine.h>
//#include <lua.hpp>
#include <iostream>
#include <string> //

#include <sstream>
#include <fstream>
//#include <cstdarg> // variadic functions

using std::cout;
using std::endl;

extern "C"
{
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}
#include <luabind/luabind.hpp>
#include <luabind/object.hpp>

namespace
{
  /**
   * \brief 
   */
  std::string LUA_FILE = "data/lua/Behaviors.lua"; // location of lua file
  /**
   * \brief 
   */
  std::string PATH_LUA = "data/lua/";

  //export GameObject class with luabind
  /**
   * \brief 
   * \param lua_state 
   */
  void BindGameObject(lua_State * lua_state)
  {
    luabind::module(lua_state)
      [
        luabind::class_<GameObject>("GameObject")
        .def(luabind::constructor<std::string>())
      //.def("FindComponent", &GameObject::FindComponent)
        .def("Destroy",       &GameObject::Destroy) //dtor
      //.def("Add",           &GameObject::Add)     //template
        .def("IsDestroyed",   &GameObject::IsDestroyed)
      //.def("GetName",       &GameObject::GetName)
      //.def("Find",          &GameObject::Find)
      ];
  }

  //calling lua functions from cpp (given a function name(obj, obj) & file)
  /**
   * \brief 
   * \param lua_fn 
   * \param obj1 
   * \param obj2 
   * \param lua_file 
   */
  void CallLua(std::string const& lua_fn,
    GameObject * obj1 = nullptr,
    GameObject * obj2 = nullptr,
    std::string const& lua_file = LUA_FILE)
  {
    lua_State *lua_state = luaL_newstate();     // new lua state
    luaL_openlibs(lua_state);                   // allow lua to use base functions
    luabind::open(lua_state);                   // connect luabind to state
    BindGameObject(lua_state);                  // bind GameObject class to lua
    luabind::globals(lua_state)["obj1"] = obj1; // export obj1 as a global 
    luabind::globals(lua_state)["obj2"] = obj2; // export obj2 as a global
    try
    {
      luaL_dofile(lua_state, lua_file.c_str());
      // may need to export obj classes to lua first
      luabind::call_function<void>(lua_state, lua_fn.c_str(), obj1, obj2);
    }
    catch (luabind::error& e)
    {
      cout << "Error calling function '" << lua_fn << "' in '" << lua_file << "'; " << e.what() << endl;
    }
    catch (std::exception& e)
    {
      cout << e.what() << endl;
    }
    lua_close(lua_state);
  }
}//!namespace

 //Messaging System calls this when an event has occured
void CallBackFn(ObjectId obj_id, const Messaging::Message& message)
{
  std::cout << "MESSAGE RECIEVED" << std::endl;
 
  //lua_State *lua_state = luaL_newstate();   // new lua state
  //luaL_openlibs(lua_state);
  //luabind::open(lua_state);                 // connect luabind to state
  //luaL_dofile(lua_state, LUA_FILE.c_str()); // read file; prepares file for function calls

  // pair.first = the object table index pair.second = the generation Data
  unsigned int index = obj_id.first; //obj table index

  GameObject obj1("player"); //for testing change later
  GameObject obj2("enemy");  //for testing change later

  switch (message.id)
  {
  case (Messaging::ID::MoveLeft):
    //call lua fn for player move left -- MoveLeft(player, control_stick_value)
    //luabind::call_function<void>(lua_state, "MoveLeft", obj1, obj2);
    std::cout << "MOVE LEFT\n";
    //CallLua("Move", &obj1/*, somehow control_stick_value*/);
    break;
  case (Messaging::ID::MoveRight):
    //call lua fn for player move right -- MoveRight(player, control_stick_value)
    //luabind::call_function<void>(lua_state, "MoveRight", obj1, obj2);
    std::cout << "MOVE RIGHT\n";
    //CallLua("Move", &obj1/*, somehow control_stick_value*/);
    break;
  case (Messaging::ID::Jump):
    //call lua fn for player jump
    //std::cout << "JUMP\n";
    if (message.message.jump.key_state == Messaging::KeyState::Pressed)
    {
      std::cout << "JUMP \n";
    }
    //CallLua("Jump", &obj1/*, somehow control_stick_value*/);
    break;
  case (Messaging::ID::MenuUp):
    //call lua fn for menu up
    CallLua("MenuUp");
    break;
  case (Messaging::ID::MenuDown):
    //call lua fn for menu down
    CallLua("MenuDown");
    break;
  case (Messaging::ID::MenuLeft):
    //call lua fn for menu left
    CallLua("MenuLeft");
    break;
  case (Messaging::ID::MenuRight):
    //call lua fn for menu right
    CallLua("MenuRight");
    break;
  case (Messaging::ID::MenuSelect):
    //call lua fn for menu select
    CallLua("MenuSelect");
    break;
  case (Messaging::ID::MenuBack):
    //call lua fn for menu back
    CallLua("MenuBack");
    break;
    // case (Messaging::ID::):
    //   //call lua fn for 
    //   break;
  default:
    break;
  }
  //lua_close(lua_state);
}

/**
 * \brief 
 * \param msg_id_types 
 */
void Behaviors::registerListeners_(std::vector<Messaging::ID> const& msg_id_types)
{
  std::cout << "REGISTER LISTENERS" << std::endl;
  Messaging::MessageHandler *msg_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId const& id = GetKey();
  for (std::vector<Messaging::ID>::const_iterator it = msg_id_types.begin(); it != msg_id_types.end(); ++it)
  {
    msg_handler->Register(*it, id, CallBackFn);
  }
}

/**
 * \brief 
 * \param msg_id_types 
 */
void Behaviors::unregisterListeners_(std::vector<Messaging::ID> const& msg_id_types)
{
  std::cout << "UNREGISTER LISTENERS" << std::endl;
  Messaging::MessageHandler *msg_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId const& id = GetKey();
  for (std::vector<Messaging::ID>::const_iterator it = msg_id_types.begin(); it != msg_id_types.end(); ++it)
  {
    msg_handler->Unregister(*it, id);
  }
}

//void Behaviors::registerListeners_(void)
//{
//  Messaging::MessageHandler *msg_handler = engine.Find<Messaging::MessageHandler>();
//  ObjectId const& id = GetKey();
//  std::cout << "REGISTER LISTENERS\n";
//  msg_handler->Register(Messaging::ID::MoveLeft,   id, CallBackFn);
//  msg_handler->Register(Messaging::ID::MoveRight,  id, CallBackFn);
//  msg_handler->Register(Messaging::ID::Jump,       id, CallBackFn);
//  //msg_handler->Register(Messaging::ID::MenuUp,     id, CallBackFn);
//  //msg_handler->Register(Messaging::ID::MenuDown,   id, CallBackFn);
//  //msg_handler->Register(Messaging::ID::MenuLeft,   id, CallBackFn);
//  //msg_handler->Register(Messaging::ID::MenuRight,  id, CallBackFn);
//  //msg_handler->Register(Messaging::ID::MenuSelect, id, CallBackFn);
//  //msg_handler->Register(Messaging::ID::MenuBack,   id, CallBackFn);
//  //msg_handler_.Register(Messaging::ID::, id, &CallBackFn);
//
//  // also could have individual callback functions; which would eliminate 
//  // any switch statement but that might be gross; who knows
//}

//void Behaviors::unregisterListeners_(void)
//{
//  //ObjectId placeholder(0, 0);
//  //ObjectId &id = placeholder;
//  Messaging::MessageHandler *msg_handler = engine.Find<Messaging::MessageHandler>();
//  ObjectId const& id = GetKey();
//  //format: void Unregister(ID id, SlotMap::ObjectId obj);
//  msg_handler->Unregister(Messaging::ID::MoveLeft,   id);
//  msg_handler->Unregister(Messaging::ID::MoveRight,  id);
//  msg_handler->Unregister(Messaging::ID::Jump,       id);
//  msg_handler->Unregister(Messaging::ID::MenuUp,     id);
//  msg_handler->Unregister(Messaging::ID::MenuDown,   id);
//  msg_handler->Unregister(Messaging::ID::MenuLeft,   id);
//  msg_handler->Unregister(Messaging::ID::MenuRight,  id);
//  msg_handler->Unregister(Messaging::ID::MenuSelect, id);
//  msg_handler->Unregister(Messaging::ID::MenuBack,   id);
//}

// creates an empty behaviors component; will not listen for anything until Add() is called
/**
 * \brief 
 * \param parent 
 */
Behaviors::Behaviors(GameObject& parent) :
  Component(parent),
  msg_id_types_(std::vector<Messaging::ID> ()) // no types yet
{
}

/**
 * \brief 
 * \param parent 
 * \param msg_id_types 
 */
Behaviors::Behaviors(GameObject& parent, std::vector<Messaging::ID> const& msg_id_types) :
  Component(parent),
  msg_id_types_(msg_id_types)
{
  registerListeners_(msg_id_types);
}

// assumes the user will not add duplicates (this function will not check for duplicates)
/**
 * \brief 
 * \param msg_id_types 
 */
void Behaviors::Add(std::vector<Messaging::ID> const& msg_id_types)
{
  registerListeners_(msg_id_types);
  msg_id_types_.insert(msg_id_types_.end(), msg_id_types.begin(), msg_id_types.end());
}

// this is a more careful version of the Add() function; 
// throws an exception if trying to add a duplicate
/**
 * \brief 
 * \param msg_id_type 
 */
void Behaviors::Add(Messaging::ID const& msg_id_type)
{
  if (std::find(msg_id_types_.begin(),msg_id_types_.end(),msg_id_type) != msg_id_types_.end())
  {
    // throw exception: trying to add a duplicate
  }
  Add({ msg_id_type });
}

// assumes the given message id types have already been added (will not check)
/**
 * \brief 
 * \param msg_id_types 
 */
void Behaviors::Remove(std::vector<Messaging::ID> const& msg_id_types)
{
  std::vector<Messaging::ID> rm_id_types = msg_id_types;
  unregisterListeners_(rm_id_types);
  while (!rm_id_types.empty())
  {
    Messaging::ID const& id = msg_id_types.front();
    for (std::vector<Messaging::ID>::iterator it = msg_id_types_.begin(); it != msg_id_types_.end(); ++it)
    {
      if (*it == id)
      {
        msg_id_types_.erase(it);
        break;
      }
    }
    rm_id_types.pop_back();
  }
  

  //std::sort(msg_id_types.begin(), msg_id_types.end());   //O(nlogn)
  //std::sort(msg_id_types_.begin(), msg_id_types_.end()); //O(nlogn)
  //for (std::vector<Messaging::ID>::iterator it = msg_id_types.begin(), 
  //  i = msg_id_types_.begin(); it != msg_id_types.end(), i != msg_id_types_.end();)
  //{
  //  while (*i < *it) ++i;
  //  while (*it < *i) ++it;
  //  if (*i == *it)
  //  {
  //    msg_id_types_.erase(it);
  //  }
  //}//O()

}

// this is a more careful version of the Remove() function;
// throws an exception if trying to remove something that doesn't exist
/**
 * \brief 
 * \param msg_id_type 
 */
void Behaviors::Remove(Messaging::ID const& msg_id_type)
{
  if (std::find(msg_id_types_.begin(), msg_id_types_.end(), msg_id_type) == msg_id_types_.end())
  {
    // throw exception: trying to remove type that doesn't exist on <this object>
  }
  Remove({ msg_id_type });
}

/**
 * \brief 
 */
Behaviors::~Behaviors(void)
{
  unregisterListeners_(msg_id_types_);
}

void Behaviors::Update(float dt) {}

void Behaviors::Draw(void) {}

namespace SarahSandbox
{
  //calling lua functions from cpp (given a file & function name(float, float) )
  void TestCallLuaFromCpp(std::string const& lua_file = "data/lua/Behaviors.lua",
    std::string const& lua_fn = "add", float obj1 = 0, float obj2 = 0)//, int n_param = 0, ...)
  {
    lua_State *lua_state = luaL_newstate(); //new lua state
    luabind::open(lua_state);               // connect luabind to state
                                            //va_list args;
                                            //va_start(args, n_param);
    try
    {
      luaL_dofile(lua_state, lua_file.c_str());
      float val = luabind::call_function<float>(lua_state, lua_fn.c_str(), obj1, obj2);
      cout << "Result: " << val << endl;
    }
    catch (luabind::error& e)
    {
      cout << "Error calling function '" << lua_fn << "' in '" << lua_file << "'; " << e.what() << endl;
    }
    catch (std::exception& e)
    {
      cout << e.what() << endl;
    }
    //va_end(args);
    lua_close(lua_state);
  }

  //call cpp function(std::string) from lua
  void TestCallCppFromLua(std::string const& param, void(*cpp_fn)(std::string const&))
  {
    lua_State *lua_state = luaL_newstate();
    luaL_openlibs(lua_state);
    luabind::open(lua_state);
    // add function to the state's global scope
    luabind::module(lua_state)
      [
        luabind::def("lua_call_to_cpp", //cpp function name alias used in lua file
        *cpp_fn) //cpp function
      ];
    luaL_dostring(
      lua_state,
      ("lua_call_to_cpp(\"" + param + "\")\n").c_str()
    );
    lua_close(lua_state);
  }

  class NumberPrinter {
  public:
    NumberPrinter(int number) :
      m_number(number) {}

    void print() {
      cout << m_number << endl;
    }

  private:
    int m_number;
  };

  //exporting classes to lua
  void TestExportClassToLua()
  {
    // Create a new lua state
    lua_State *lua_state = luaL_newstate();

    // Connect LuaBind to this lua state
    luabind::open(lua_state);

    // Export our class with LuaBind
    luabind::module(lua_state)[
      luabind::class_<NumberPrinter>("NumberPrinter")
        .def(luabind::constructor<int>())
        .def("print", &NumberPrinter::print)
    ];

    // Now use this class in a lua script
    luaL_dostring(
      lua_state,
      "Print2000 = NumberPrinter(2000)\n"
      "Print2000:print()\n"
    );


    lua_close(lua_state);
  }

  template<typename T>
  struct Point {
    Point(T x, T y) :
      x_(x), y_(y) {}

    T x_, y_;
  };

  template<typename T>
  struct Box {
    Box(Point<T> upper_left, Point<T> lower_right) :
      upper_left_(upper_left), lower_right_(lower_right) {}

    Point<T> upper_left_, lower_right_;
  };

  // exporting class attributes & properties to lua
  void TestExportClassAttributesToLua()
  {
    lua_State *lua_state = luaL_newstate();
    luaL_openlibs(lua_state);
    luabind::open(lua_state);
    // export class with luabind
    luabind::module(lua_state)
      [
        // export Point
        luabind::class_<Point<float> >("Point")
        .def(luabind::constructor<float, float>())
      .def_readwrite("x_", &Point<float>::x_)
      .def_readwrite("y_", &Point<float>::y_),
      // export Box
      luabind::class_<Box<float> >("Box")
      .def(luabind::constructor<Point<float>, Point<float> >())
      .def_readwrite("upper_left_", &Box<float>::upper_left_)
      //.def_readwrite("lower_right_", &Box<float>::upper_right_)
      ];

    // use the class in a lua script
    luaL_dostring
    (
      lua_state,
      "MyBox = Box(Point(10, 20), Point(30, 40))\n"
      "MyBox.upper_left_.x_ = MyBox.lower_right_.y_\n"
    );
    lua_close(lua_state);
  }

  struct Count
  {
    Count()
      : count_(0) {}
    void IncrementCount()
    {
      ++count_;
    }
    unsigned GetCount() const
    {
      return count_;
    }

    unsigned count_;
  };

  //catching lua errors
  void TestErrors()
  {
    lua_State *lua_state = luaL_newstate();
    luaL_openlibs(lua_state);
    luabind::open(lua_state);
    // export class with LuaBind
    luabind::module(lua_state)
      [
        luabind::class_<Count>("Count")
        .def("IncrementCount", &Count::IncrementCount)
      .property("GetCount", &Count::GetCount)
      ];

    try
    {
      Count count;

      // assign count to a global in lua
      luabind::globals(lua_state)["count"] = &count;
      // execute a script to increment counts
      luaL_dostring
      (
        lua_state,
        "count:IncrementCount()\n"
        "count:IncrementCount()\n"
        "\n"
        "count_ = count.count_\n"
      );

      // read a global from the lua script
      unsigned count_ = luabind::object_cast<unsigned>
        (
          luabind::globals(lua_state)["count_"]
          );
      cout << count_ << endl;
    }
    catch (const std::exception &TheError)
    {
      //std::cout << TheError.what() << endl;
      std::cerr << TheError.what() << endl;
    }

    lua_close(lua_state);
  }

  static bool QUIT = false; //for testing

  void loop()
  {
    float dt = 0;
    //GameObject player("player");
    //GameObject enemy("enemy");
    //Behaviors b;
   // cout << player.GetName() << endl;
    while (!QUIT)
    {
      //testing
      TestCallLuaFromCpp();
      BehaviorsDebug::press_anykey();
      QUIT = true;
      //b.Update(dt);
    }
  }
  void BehaviorsTest(void)
  {
    TestCallLuaFromCpp(LUA_FILE, "add", 5.5f, 4.2f);
    //TestCallCppFromLua("data/lua/Behaviors.lua", &BehaviorsDebug::PrintFile);
    //TestExportClassToLua();
    //TestExportClassAttributesToLua();

    //BehaviorsDebug::LuaHasErrors(LUA_FILE);
    //BehaviorsDebug::PrintFile(LUA_FILE);

    lua_State *lua_state = luaL_newstate();  // new lua state
    luaL_openlibs(lua_state);
    luabind::open(lua_state);                // connect luabind to state

    GameObject go1("player");
    GameObject go2("enemy");

    // export class with luabind
    luabind::module(lua_state)
    [
      //export GameObject
      luabind::class_<GameObject>("GameObject")
      .def(luabind::constructor<std::string>()) //ctor
      .def("Destroy",     &GameObject::Destroy) //dtor
      //.def("Add",         &GameObject::Add)   //template
      .def("IsDestroyed", &GameObject::IsDestroyed)
      //.def("GetName",     static_cast<std::string (*)()>(&GameObject::GetName))
      //.def("Find",        &GameObject::Find)
    ];

    // fix these -_-"
    //luabind::globals(lua_state)["obj1"] = go1;
    //luabind::globals(lua_state)["obj2"] = go2;

    /*
      Another way to pass this data to Lua is to call a Lua function 
      and pass it as a parameter. You can use the luabind::object 
      interface to get a function as an object, then use 
      luabind::call_function to call it. You can then just pass x as 
      a parameter to the function. 
    */

    //// use the class in a lua script
    //luaL_dostring
    //(
    //  lua_state,
    //  "MyBox = Box(Point(10, 20), Point(30, 40))\n"
    //  "MyBox.upper_left_.x_ = MyBox.lower_right_.y_\n"
    //);

    try
    {
      luaL_dofile(lua_state, LUA_FILE.c_str());
      // may need to export obj classes to lua first
      std::string message = luabind::call_function<std::string>(lua_state, "test");//, go1, go2);
      std::cout << message << std::endl;
    }
    catch (luabind::error& e)
    {
      cout << "Error calling function '" << "test" << "' in '" << LUA_FILE << "'; " << e.what() << endl;
    }
    catch (std::exception& e)
    {
      cout << e.what() << endl;
    }
    lua_close(lua_state);
  }
 
}//!namespace SarahSandbox

namespace BehaviorsDebug
{
  // output operator for a vector of Messaging::ID
  // will look something like <0,1,2,3,5,7,10>
  std::ostream &operator<<(std::ostream & os, std::vector<Messaging::ID> const& msg_id_types)
  {
    os << "<";
    std::vector<Messaging::ID>::const_iterator end = msg_id_types.end();
    --end;
    for (std::vector<Messaging::ID>::const_iterator it = msg_id_types.begin(); it != end; ++it)
    {
      os << static_cast<unsigned int>(*it) << ", ";
    }
    os << static_cast<unsigned int>(*end) << ">";
    return os;
  }

  // check lua file for syntax errors & memory allocation errors
  // returns true if errors
  // returns false if no errors 
  // only use for debug
  bool LuaHasErrors(std::string const& lua_file_name)
  {
    lua_State *lua_state = luaL_newstate();
    luabind::open(lua_state); //connect luabind to state
    bool has_errors = false;
    int err = luaL_loadfile(lua_state, lua_file_name.c_str());
    if (err == LUA_ERRSYNTAX)
    {
      cout << "ERROR: lua error in " << lua_file_name << " syntax" << endl;
      //alternatively throw an exception (lua syntax error)
      has_errors = true;
    }
    if (err == LUA_ERRMEM)
    {
      cout << "ERROR [" << lua_file_name << "]: lua error in memory allocation." << endl;
      //alternatively throw an exception (lua mem error)
      has_errors = true;
    }
    lua_close(lua_state);
    return has_errors;
  }

  // way of checking that you have given the correct filename
  // only use for debug
  void PrintFile(std::string const& file_name)
  {
    cout << "[printing " << file_name << "]" << endl;
    system(("cat " + file_name).c_str()); // sys calls are bad
    cout << "[finished printing " << file_name << "]" << endl;
  }
  //only use for debug
  void press_anykey(void)
  {
    system("pause"); //sys calls are bad; change later
  }
}

/**
 * \brief 
 * \return 
 */
Component* Behaviors::Clone_() const
{
  return new Behaviors(*this);
}

