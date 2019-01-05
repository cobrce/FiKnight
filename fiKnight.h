#ifndef FIKNIGHT_H_
#define FIKNIGHT_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif

class FiKnightSerialDebugger;
class State;
#include "State.h"
#include "fiKnightSerialDebugger.h"
class FiKnight
{
  private:
    State *firstState;
    State *currentState;
    State *previousState;
    FiKnightSerialDebugger * debugger;
    bool (*callback)(State *nextstate);
    State * (*onErrorCallback)(State *previousState);


  public:
    bool step;
    bool running;
    FiKnight();
    // callback function is the function that decides wether continue execution or not
    // on error callback is a function called when currentstate::Run doesn't give the next state to execute, this function should return the next state, if not, the execution is aborted
    FiKnight(State *firstState, bool (*callback)(State *nextstate), State * (*onErrorCallback)(State *previousState));
    void SetSerialDebugger(FiKnightSerialDebugger * debugger);
    //should run endlessly unless :
    //  • no next state is provided from both the currentstate::Run and the onErrorCallBack
    //  • the callback returns false
    void MainLoop(bool paused = false);
    int CurrentStateID();
    void SetCurrentState(State * state);
};
#endif