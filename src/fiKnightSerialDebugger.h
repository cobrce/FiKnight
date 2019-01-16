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
    dump_memory = 'D',
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

struct DumpMemoryMessage
{
    byte ID;
    Commands command;
    byte ExtraDataLen;
    int memAddress;
    int memSize;
    byte Dump[96];
    byte Sig;
    byte End;
};

class FiKnightSerialDebugger
{
  protected:
    long int now;
    State *(*SetStateHandler)(byte ID);
    void (*SerialReceivedHandler)(int size, byte *data);
    void ReadMemory(DumpMemoryMessage *message);

  public:
    int notificationInterval = 2000;
    FiKnightSerialDebugger();
    FiKnightSerialDebugger(State *(*SetStateHandler)(byte ID));
    FiKnightSerialDebugger(State *(*SetStateHandler)(byte ID), void (*SerialReceivedHandler)(int size, byte *data));
    virtual bool ReadExecuteSerialDebugCommand(FiKnight *machine);
    virtual void SendCurrentExecutionStatus(byte ID, FiKnight *machine);
    virtual void SendCurrentState(byte ID, FiKnight *machine);
    virtual void ExecuteSerialDebugCommand(FiKnight *machine, DebugMessage *message);
    virtual void SetSerialReceivedHandler(void (*SerialReceivedHandler)(int size, byte *data));
};
#endif
