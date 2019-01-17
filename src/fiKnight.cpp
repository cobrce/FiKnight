#include "config.h"
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
    //   Serial.println("#Mainloop");
    //   Serial.println(this->debugger ? "#debugger enabled" : "#debugger disabled");
    this->running = !paused;
    State *previousState;
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

            #ifdef NOTIFY_ON_STATE_CHANGE
            if (currentState != previousState &&
                debugger)
                debugger->SendCurrentState(0xff, this);
            #endif
            running = !step && callback(currentState);
        }
        step = false;
        #ifndef INFINITE_MAIN_LOOP
            break;
        #else
        delay(MAIN_LOOP_DELAY);
        #endif
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