#include "fiKnightSerialDebugger.h"
#include "fiKnight.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif

FiKnightSerialDebugger::FiKnightSerialDebugger()
{

}
FiKnightSerialDebugger::FiKnightSerialDebugger(State* (*SetStateHandler)(byte ID))
{
      this->SetStateHandler = SetStateHandler;  
}

bool FiKnightSerialDebugger::ReadExecuteSerialDebugCommand(FiKnight *machine)
{
    byte data[104];
    if (!now || (millis() - now) >=2000)
    {
        now = millis();
        SendCurrentExecutionStatus(0xff, machine);
        SendCurrentState(0xff, machine);
    }
    if (Serial.available() > 0)
    {
        int len = Serial.readBytesUntil('>', data, 104);
        if (len < 104 && ((char)data[len - 1]) == '-')
        {
            ExecuteSerialDebugCommand(machine, (DebugMessage *)&data[0]);
            return true;
        }
    }
    return false;
}

void FiKnightSerialDebugger::SendCurrentExecutionStatus(byte ID, FiKnight *machine)
{
    DebugMessage message = {ID, current_execution_status, 1, machine->running, '-', '\n'};
    Serial.write((byte*)&message, 6);
}
void FiKnightSerialDebugger::SendCurrentState(byte ID, FiKnight *machine)
{
    GetSetStateMessage message = {ID, current_state, 2, machine->CurrentStateID(), '-', '\n',};
    Serial.write((byte*)&message, 7);
}

void FiKnightSerialDebugger::ExecuteSerialDebugCommand(FiKnight *machine, DebugMessage *message)
{
    switch (message->Command)
    {
    case pause:
        Serial.println("#cmd pause");
        machine->running = false;
        break;
    case resume:
        Serial.println("#cmd resume");
        machine->running = true;
        break;
    case current_execution_status:
        Serial.println("#cmd get run/pause");
        SendCurrentExecutionStatus(message->ID,machine);
        break;
    case current_state:
        Serial.println("#cmd get state");
        SendCurrentState(message->ID,machine);
        break;
    case set_state:
        Serial.println("#cmd set state");
        machine->SetCurrentState(this->SetStateHandler(((GetSetStateMessage*)message)->StateID));
        break;
    case step:
        Serial.println("#cmd step");
        machine->step = true;
        break;
    };
}