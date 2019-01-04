#ifndef ISERIALDEBUGGER_H_
#define ISERIALDEBUGGER_H_

class FiKnight;
#include "fiKnight.h"

enum Commands
{
    pause = 'P',
    resume = 'R',
    current_execution_status = 'E',
    current_state = 'C',
    set_state = 'S',
    step = '+',
};


struct DebugMessage
{
    char ID;
    Commands Command;
    char ExtraDataLen;
    char ExtraData[100];
    char Sig; // should be "-"
    char End; // should be ">"
};
struct GetSetStateMessage
{
    char ID;
    Commands Command;
    char ExtraDataLen;
    int StateID;
    char Sig;
    char End;
};

class ISerialDebugger
{
    protected:
    State * (*SetStateHandler)(char ID);
    public:
    ISerialDebugger(State * (*SetStateHandler)(char ID));
    virtual bool ReadExecuteSerialDebugCommand(FiKnight * machine);
    
    virtual void SendCurrentExecutionStatus(char ID,FiKnight * machine);
    virtual void SendCurrentState(char ID,FiKnight * machine);
    virtual void ExecuteSerialDebugCommand(FiKnight * machine,DebugMessage * message) = 0;
};
#endif
