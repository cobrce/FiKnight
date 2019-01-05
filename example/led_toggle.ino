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
  machine.SetSerialDebugger(&debugger);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);  
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
