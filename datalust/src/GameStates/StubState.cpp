// #include <iostream>
// #include <memory>
// #include <Engine/Engine.h>
// #include "StubState.h"   
// #include "DemoState.h"

// void StubState::Init()
// {
//   std::cout<< "Press Space to play or press Q to quit."<<std::endl;
// }

// void StubState::Update(float dt)
// {
//   int c = std::cin.get();
//   /*checks the stored value if it isn't '\n' then creates a loop while the enter key isn't pressed*/
//   while (c != '\n' &&  std::cin.get() != '\n')
//   ;
//   if (c == 'q' || c == 'Q')
//     std::static_pointer_cast<GameStateManager>(
//       engine.FindComponent(System::TypeEnum::GameStateManager))->Quit();
//   else if (c == ' ')
//     std::static_pointer_cast<GameStateManager>(
//       engine.FindComponent(System::TypeEnum::GameStateManager))->ChangeState(std::make_unique<DemoState>());
// }

// void StubState::Shutdown()
// {
// }