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
#include "config.h"
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
    void (*SerialReceivedHandler)(int size, byte *data);
    #if defined(INCLUDE_DEBUG_FUNCTION) || defined(EXECUTE_DEBUG_COMMANDS)
    State *(*SetStateHandler)(byte ID);
    void ReadMemory(DumpMemoryMessage *message);
    #endif

  public:
    FiKnightSerialDebugger();
    FiKnightSerialDebugger(void (*SerialReceivedHandler)(int size, byte *data));
    
    #if defined(INCLUDE_DEBUG_FUNCTION) || defined(EXECUTE_DEBUG_COMMANDS) || defined(NOTIFY_ON_STATE_CHANGE)
    void SendCurrentState(byte ID, FiKnight *machine);
    #endif

    #if defined(INCLUDE_DEBUG_FUNCTION) || defined(EXECUTE_DEBUG_COMMANDS)
    FiKnightSerialDebugger(State *(*SetStateHandler)(byte ID));
    FiKnightSerialDebugger(State *(*SetStateHandler)(byte ID), void (*SerialReceivedHandler)(int size, byte *data));
    void SendCurrentExecutionStatus(byte ID, FiKnight *machine);
    void ExecuteSerialDebugCommand(FiKnight *machine, DebugMessage *message);
    #endif

    bool ReadExecuteSerialDebugCommand(FiKnight *machine);
    void SetSerialReceivedHandler(void (*SerialReceivedHandler)(int size, byte *data));
};
#endif
