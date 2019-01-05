#include "fiKnight.h"
#include "fiKnightSerialDebugger.h"


FiKnight::FiKnight()
{
  
}
FiKnight::FiKnight(State *firstState, bool (*callback)(State *nextstate), State *(*onErrorCallback)(State *previousState))
{
  this->firstState = firstState;
  this->currentState = firstState;
  this->callback = callback;
  this->onErrorCallback = onErrorCallback;
}
void FiKnight::SetSerialDebugger(ISerialDebugger *debugger)
{
  this->debugger = debugger;
}

void FiKnight::MainLoop(bool paused)
{
  this->running = !paused;
  while (true)
  {
    if (this->debugger)
      this->debugger->ReadExecuteSerialDebugCommand(this);
    if (currentState && (running || step))
    {
      previousState = currentState;
      currentState = currentState->Run();
      if (!currentState)
        currentState = this->onErrorCallback(previousState);
      if (!currentState)
        running = false;
      running = !step && callback(currentState);
    }
    step = false;
  }
}

int FiKnight::CurrentStateID()
{
  return this-> currentState->ID;
}

void FiKnight::SetCurrentState(State *state)
{
  this -> currentState = state;
}