#include <fiKnight.h>
#include <fiKnightSerialDebugger.h>

// define functions
State * StateFunction();
bool AlwaysTrue(State * nextstate);
State * OnErrorCallback(State * previousstate);
State * SetStateHandler(int ID);
void SerialDataReceivedHandler(int size, byte * data);

// define machine objects
State first = State(StateFunction,1);
State second = State(StateFunction,2);

// Reading serial data directly disturbs the functioning of FiKnightSerialDebugger
// Instead a SerialDataReceivedHandler should be used, this way when an unknown
// message is received it's sent to the SerialDataReceivedHandler
FiKnightSerialDebugger debugger = FiKnightSerialDebugger(SetStateHandler,SerialDataReceivedHandler);
FiKnight machine = FiKnight(&first,AlwaysTrue,OnErrorCallback,&debugger);


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);  
  // sending T> from serial monitor with toggle this pin
  pinMode(2,OUTPUT);
};

void loop() {
  machine.MainLoop(true);
}

long int previous;
State * StateFunction()
{
  if (!previous || (millis() - previous >=500))
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
