#include "fiKnight.h"
#include "fiKnightSerialDebugger.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif

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
void FiKnight::SetSerialDebugger(FiKnightSerialDebugger *debugger)
{
  this->debugger = debugger;
}

void FiKnight::MainLoop(bool paused)
{
  Serial.println("#Mainloop");
  Serial.println(this->debugger ? "#debugger enabled" : "#debugger disabled");
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
    delay(500);
  }
}

int FiKnight::CurrentStateID()
{
  return this->currentState->ID;
}

void FiKnight::SetCurrentState(State *state)
{
  this->currentState = state;
}