#ifndef FIKNIGHTSERIALDEBUGGER_H_
#define FIKNIGHTSERIALDEBUGGER_H_

class FiKnight;
#include "fiKnight.h"
#include "ISerialDebugger.h"

class FiKnightSerialDebugger : ISerialDebugger
{
    public:
    virtual void SendCurrentExecutionStatus(char ID,FiKnight * machine);
    virtual bool ReadExecuteSerialDebugCommand(FiKnight * machine);
    virtual void SendCurrentState(char ID,FiKnight * machine);
    FiKnightSerialDebugger();
    FiKnightSerialDebugger(State* (*SetStateHandler)(char ID));
    virtual void ExecuteSerialDebugCommand(FiKnight * machine,DebugMessage * message);
};
#endif
