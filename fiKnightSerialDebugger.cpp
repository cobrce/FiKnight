#include "fiKnightSerialDebugger.h"

FiKnightSerialDebugger::FiKnightSerialDebugger(State* (*SetStateHandler)(byte ID)) : ISerialDebugger(SetStateHandler)
{
        
}

void FiKnightSerialDebugger::ExecuteSerialDebugCommand(FiKnight *machine, DebugMessage *message)
{
    switch (message->Command)
    {
    case pause:
        machine->running = false;
        break;
    case resume:
        machine->running = false;
        break;
    case current_execution_status:
        SendCurrentExecutionStatus(message->ID,machine);
        break;
    case current_state:
        SendCurrentState(message->ID,machine);
        break;
    case set_state:
        machine->SetCurrentState(this->SetStateHandler(((SetStateMessage*)message)->StateID));
        break;
    case step:
        machine->step = true;
        break;
    };
}