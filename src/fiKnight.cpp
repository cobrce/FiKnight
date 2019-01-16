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
    this->CommmonConstructor(firstState, callback, onErrorCallback);
}

FiKnight::FiKnight(State *firstState, bool (*callback)(State *nextstate), State *(*onErrorCallback)(State *previousState), FiKnightSerialDebugger *debugger)
{
    this->CommmonConstructor(firstState, callback, onErrorCallback);
    this->SetSerialDebugger(debugger);
}
void FiKnight::CommmonConstructor(State *firstState, bool (*callback)(State *nextstate), State *(*onErrorCallback)(State *previousState))
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

void FiKnight::MainLoop(bool paused, bool infinite)
{
    //   Serial.println("#Mainloop");
    //   Serial.println(this->debugger ? "#debugger enabled" : "#debugger disabled");
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

            if (notifyOnStateChange &&
                currentState != previousState &&
                debugger)
                debugger->SendCurrentState(0xff, this);
            running = !step && callback(currentState);
        }
        step = false;
        if (!infinite)
            break;
        delay(100);
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