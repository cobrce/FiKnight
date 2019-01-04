#include "ISerialDebugger.h"
#include "fiKnight.h"

ISerialDebugger::ISerialDebugger(State *(*SetStateHandler)(char ID))
{
    this->SetStateHandler = SetStateHandler;
}
bool ISerialDebugger::ReadExecuteSerialDebugCommand(FiKnight *machine)
{
    char data[104];

    SendCurrentExecutionStatus(0xff, machine);
    SendCurrentState(0xff, machine);

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

void ISerialDebugger::SendCurrentExecutionStatus(char ID, FiKnight *machine)
{
    DebugMessage message = {ID, current_execution_status, 1, machine->running, '-', '>'};
    Serial.write(message, 6);
}
void ISerialDebugger::SendCurrentState(char ID, FiKnight *machine)
{
    GetSetStateMessage message = {ID, current_state, 2, machine->CurrentStateID(), '-', '>'};
    Serial.write(message, 7);
}