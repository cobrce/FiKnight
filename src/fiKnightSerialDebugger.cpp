#include "config.h"
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

FiKnightSerialDebugger::FiKnightSerialDebugger(void (*SerialReceivedHandler)(int size, byte *data))
{
    this->SetSerialReceivedHandler(SerialReceivedHandler);
}

#if defined(INCLUDE_DEBUG_FUNCTION) || defined(EXECUTE_DEBUG_COMMANDS)
FiKnightSerialDebugger::FiKnightSerialDebugger(State *(*SetStateHandler)(byte ID))
{
    this->SetStateHandler = SetStateHandler;
}

FiKnightSerialDebugger::FiKnightSerialDebugger(State *(*SetStateHandler)(byte ID), void (*SerialReceivedHandler)(int size, byte *data))
{
    this->SerialReceivedHandler = SerialReceivedHandler;
    this->SetSerialReceivedHandler(SerialReceivedHandler);
}
#endif

bool FiKnightSerialDebugger::ReadExecuteSerialDebugCommand(FiKnight *machine)
{
    byte data[105];

    #ifdef NOTIFY_EXECUTION_STATE
    if (!now || (millis() - now) >= NOTIFICATION_INTERVAL)
    {
        now = millis();
        SendCurrentExecutionStatus(0xff, machine);
        // SendCurrentState(0xff, machine); // will be done by fiKnight
    }
    #endif
    if (Serial.available() > 0)
    {
        int len = Serial.readBytesUntil('>', data, 104);
        #if defined(FILTER_DEBUG_COMMANDS) || defined(EXECUTE_DEBUG_COMMANDS)
        if (len < 104 && ((char)data[len - 1]) == '-')
        {
            #ifdef EXECUTE_DEBUG_COMMANDS
            this->ExecuteSerialDebugCommand(machine, (DebugMessage *)&data[0]);
            #endif
            return true;
        }
        else if (this->SerialReceivedHandler)
        #endif
        {
            this->SerialReceivedHandler(len, &data[0]);
        }
    }
    return false;
}

void FiKnightSerialDebugger::SetSerialReceivedHandler(void (*SerialReceivedHandler)(int size, byte *data))
{
    this->SerialReceivedHandler = SerialReceivedHandler;
}

#if defined(INCLUDE_DEBUG_FUNCTION) || defined(EXECUTE_DEBUG_COMMANDS) || defined(NOTIFY_ON_STATE_CHANGE)
void FiKnightSerialDebugger::SendCurrentState(byte ID, FiKnight *machine)
{
    GetSetStateMessage message = {
        ID,
        current_state,
        2,
        machine->CurrentStateID(),
        '-',
        '\n',
    };
    Serial.write((byte *)&message, 7);
}
#endif

#if defined(INCLUDE_DEBUG_FUNCTION) || defined(EXECUTE_DEBUG_COMMANDS)

void FiKnightSerialDebugger::SendCurrentExecutionStatus(byte ID, FiKnight *machine)
{
    DebugMessage message = {ID, current_execution_status, 1, machine->running, '-', '\n'};
    Serial.write((byte *)&message, 6);
}
void FiKnightSerialDebugger::ReadMemory(DumpMemoryMessage *msg)
{
    byte i;
    DumpMemoryMessage message;

    for (i = 0;i < 7;i++ )
        ((byte *)(&message))[i] = ((byte*)msg)[i];

    if (message.memSize > 96)
        message.memSize = 96;

    byte *ptr = message.memAddress;
    for (i = 0; i < message.memSize; i++)
    {
        message.Dump[i] = ptr[i];
    }
    message.Dump[i] = '-';
    message.Dump[i + 1] = '\n';
    message.ExtraDataLen = i + 4;
    Serial.write((byte *)&message, i + 9);
}

void FiKnightSerialDebugger::ExecuteSerialDebugCommand(FiKnight *machine, DebugMessage *message)
{
    switch (message->Command)
    {
    case pause:
        // Serial.println("#cmd pause");
        machine->running = false;
        break;
    case resume:
        // Serial.println("#cmd resume");
        machine->running = true;
        break;
    case current_execution_status:
        // Serial.println("#cmd get run/pause");
        SendCurrentExecutionStatus(message->ID, machine);
        break;
    case current_state:
        // Serial.println("#cmd get state");
        SendCurrentState(message->ID, machine);
        break;
    case set_state:
        // Serial.println("#cmd set state");
        machine->SetCurrentState(this->SetStateHandler(((GetSetStateMessage *)message)->StateID));
        break;
    case step:
        // Serial.println("#cmd step");
        machine->step = true;
        break;
    case dump_memory:
        // Serial.println("#cmd dump");
        if ((message)->ExtraDataLen == 4)
            this->ReadMemory((DumpMemoryMessage *)message);
        break;
    case read_IO:
        // Serial.println("#cmd read IO");
        SendIoState(message->ID);
        break;
    };
}
#include "ports.h"
void FiKnightSerialDebugger::SendIoState(byte ID)
{
    ReadIOMessage message =
    {
        ID,
        read_IO,
        12,
        PORTA_,
        PORTB_,
        PORTC_,
        PORTD_,
        PINA_,
        PINB_,
        PINC_,
        PIND_,
        DDRA_,
        DDRB_,
        DDRC_,
        DDRD_,
        '-',
        '\n'
    };
    Serial.write((byte*) & message,sizeof(ReadIOMessage));    
}
#endif