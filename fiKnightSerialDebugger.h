#ifndef FIKNIGHTSERIALDEBUGGER_H_
#define FIKNIGHTSERIALDEBUGGER_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif
class FiKnight;
#include "fiKnight.h"

enum Commands : byte
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
    byte ID;
    Commands Command;
    byte ExtraDataLen;
    byte ExtraData[100];
    byte Sig; // should be "-"
    byte End; // should be ">" if sent to arduino, and '\n' if from arduino
};
struct GetSetStateMessage
{
    byte ID;
    Commands Command;
    byte ExtraDataLen;
    int StateID;
    byte Sig;
    byte End;
};
class FiKnightSerialDebugger
{
    private:
    long int now;
    State * (*SetStateHandler)(byte ID);
    public:
    FiKnightSerialDebugger();
    FiKnightSerialDebugger(State* (*SetStateHandler)(byte ID));
    virtual bool ReadExecuteSerialDebugCommand(FiKnight * machine);
    virtual void SendCurrentExecutionStatus(byte ID,FiKnight * machine);
    virtual void SendCurrentState(byte ID,FiKnight * machine);
    virtual void ExecuteSerialDebugCommand(FiKnight * machine,DebugMessage * message);
};
#endif
