#include <fiKnight.h>
#include <fiKnightSerialDebugger.h>

// define functions
State * StateFunction();
bool AlwaysTrue(State * nextstate);
State * OnErrorCallback(State * previousstate);
State * SetStateHandler(int ID);

// define machine objects
State first = State(StateFunction,1);
State second = State(StateFunction,2);
FiKnight machine = FiKnight(&first,AlwaysTrue,OnErrorCallback);
FiKnightSerialDebugger debugger = FiKnightSerialDebugger(SetStateHandler);

void SetupMachine()
{	
  // set the debugger 
  machine.SetSerialDebugger(&debugger);
  // since the debugger uses Serial for communication
  // it's advised not to read serial directly but use
  // a SerialReceivedHandler
  // hint : to make serial communication faster, always 
  // terminate your data with '>'
  debugger.SetSerialReceivedHandler(SerialDataReceivedHandler);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);  
  pinMode(2,OUTPUT);
  SetupMachine();
};

void loop() {
  machine.MainLoop(true);
}

long int previous;
State * StateFunction()
{
  if (!previous|| (millis() - previous >=500))
  {
    previous = millis();
    digitalWrite(LED_BUILTIN,not digitalRead(LED_BUILTIN));
    Serial.println("#Led toggled");
    return SetStateHandler(machine.CurrentStateID() + 1);
  }
  return SetStateHandler(machine.CurrentStateID());
}
bool AlwaysTrue(State * nextstate)
{
  return true;
}

State * OnErrorCallback(State * previousstate)
{
  return &first;
}

State * SetStateHandler(int ID)
{
  if (ID == 2) 
    return &second;
  else
    return &first;
}

void SerialDataReceivedHandler(int size, byte * data)
{
  if (size > 0 && data[0] == 'T')
  {
    Serial.println("# Toggling Pin 2");
    digitalWrite(2,1 - digitalRead(2));
  }
}