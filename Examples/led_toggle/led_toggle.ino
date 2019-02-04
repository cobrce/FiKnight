/// this is the example of how to use FiKnight library
/// you notice the use of "#ifdef USE_PRODUCTION_CONFIG",
/// this is token (un)defined in config.h (in the same
/// directory of the library), it's purpose is to include/remove
/// functions and variables depending on wether you are wrtining
/// a debug version of production version of the program because : 
/// - in debug you can command the program to run,pause,get/set state,
///   read memory...etc
/// - in production the debugger is only used to filter serial data
///   and send non command data to the SerialDataReceivedHandler
///
/// Q : why using #define/#undef instead of parameters:
/// A : memory
/// Q : why keep using a debugger if it doesn't debug?
/// A : if the debugger is removed entirely, the way how serial data
///     are received should be rewritten, unless no debugger was used
///     and the serial data are already read directly


/// State 1 and 2 share the same function that toggles LED_BUILTIN
/// State 4 is accessed only with set state from debugger and sets
/// State 4 toggles PIN 2 and sets next state as State 1
/// In the json file, State 1 and 2 are named LED OFF and LED ON resp.,
/// so after executing State 4 those names get switched


#include <fiKnight.h>
#include <fiKnightSerialDebugger.h>



// define functions
State *StateFunction();
State * FourthStateFunction();
bool AlwaysTrue(State *nextstate);
State *OnErrorCallback(State *previousstate);
State *SetStateHandler(int ID);
void SerialDataReceivedHandler(int size, byte *data);

// define machine objects
State first = State(StateFunction, 1);
State second = State(StateFunction, 2);
State fourth = State(FourthStateFunction,4);

// Reading serial data directly disturbs the functioning of FiKnightSerialDebugger
#ifdef USE_PRODUCTION_CONFIG
FiKnightSerialDebugger debugger = FiKnightSerialDebugger(SerialDataReceivedHandler);
#else
FiKnightSerialDebugger debugger = FiKnightSerialDebugger(SetStateHandler, SerialDataReceivedHandler);
#endif

FiKnight machine = FiKnight(&first, AlwaysTrue, OnErrorCallback, &debugger);

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    // sending "T>"" from serial monitor will toggle this pin
    pinMode(2, OUTPUT);
};

void loop()
{
    #ifdef USE_PRODUCTION_CONFIG
        #define PAUSED false
    #else
        #define PAUSED true
    #endif
    // if debug is disabled and MainLoop is called with paused=true,
    // there is noway to make it continue, unless you modify config.h
    // and you undef "INFINITE_MAIN_LOOP" 
    machine.MainLoop(PAUSED);
}

long int previous;
State *StateFunction()
{
    if (!previous || (millis() - previous >= 500))
    {
        previous = millis();
        digitalWrite(LED_BUILTIN, not digitalRead(LED_BUILTIN));
        Serial.println("#Led toggled");
        return SetStateHandler(machine.CurrentStateID() + 1);
    }
    return SetStateHandler(machine.CurrentStateID());
}

State * FourthStateFunction()
{
    TogglePin2();
    return SetStateHandler(1);
}

bool AlwaysTrue(State *nextstate)
{
    return true;
}

State *OnErrorCallback(State *previousstate)
{
    return &first;
}

State *SetStateHandler(int ID)
{
    if (ID == 2)
        return &second;
    else if (ID == 4)
        return &fourth;
    else
        return &first;
}

void TogglePin2()
{
    Serial.println("# Toggling Pin 2");
    digitalWrite(2, 1 - digitalRead(2));
}

void SerialDataReceivedHandler(int size, byte *data)
{
    if (size > 0 && data[0] == 'T')
    {
        TogglePin2();
    }
}
