#include "fiKnightSerialDebugger.h"

FiKnightSerialDebugger::FiKnightSerialDebugger() : ISerialDebugger()
{

}
FiKnightSerialDebugger::FiKnightSerialDebugger(State* (*SetStateHandler)(char ID)) : ISerialDebugger(SetStateHandler)
{
      this->SetStateHandler = SetStateHandler;  
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
        machine->SetCurrentState(this->SetStateHandler(((GetSetStateMessage*)message)->StateID));
        break;
    case step:
        machine->step = true;
        break;
    };
}