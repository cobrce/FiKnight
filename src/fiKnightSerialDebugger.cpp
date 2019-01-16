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

FiKnightSerialDebugger::FiKnightSerialDebugger(State *(*SetStateHandler)(byte ID))
{
    this->SetStateHandler = SetStateHandler;
}

FiKnightSerialDebugger::FiKnightSerialDebugger(State *(*SetStateHandler)(byte ID), void (*SerialReceivedHandler)(int size, byte *data))
{
    this->SerialReceivedHandler = SerialReceivedHandler;
    this->SetSerialReceivedHandler(SerialReceivedHandler);
}

bool FiKnightSerialDebugger::ReadExecuteSerialDebugCommand(FiKnight *machine)
{
    byte data[105];

    if (notificationInterval != -1)
    {
        if (!now || (millis() - now) >= notificationInterval)
        {
            now = millis();
            SendCurrentExecutionStatus(0xff, machine);
            // SendCurrentState(0xff, machine); // will be done by fiKnight
        }
    }

    if (Serial.available() > 0)
    {
        int len = Serial.readBytesUntil('>', data, 104);
        if (len < 104 && ((char)data[len - 1]) == '-')
        {
            this->ExecuteSerialDebugCommand(machine, (DebugMessage *)&data[0]);
            return true;
        }
        else if (this->SerialReceivedHandler)
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

void FiKnightSerialDebugger::SendCurrentExecutionStatus(byte ID, FiKnight *machine)
{
    DebugMessage message = {ID, current_execution_status, 1, machine->running, '-', '\n'};
    Serial.write((byte *)&message, 6);
}
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

void FiKnightSerialDebugger::ReadMemory(DumpMemoryMessage *message)
{
    if (message->memSize > 96)
        message->memSize = 96;

    byte *ptr = message->memAddress;
    byte i = 0;
    for (; i < message->memSize; i++)
    {
        message->Dump[i] = ptr[i];
    }
    message->Dump[i] = '-';
    message->Dump[i + 1] = '\n';
    message->ExtraDataLen = i + 4;
    Serial.write((byte *)message, i + 9);
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
        SendCurrentExecutionStatus(message->ID, machine);
        break;
    case current_state:
        Serial.println("#cmd get state");
        SendCurrentState(message->ID, machine);
        break;
    case set_state:
        Serial.println("#cmd set state");
        machine->SetCurrentState(this->SetStateHandler(((GetSetStateMessage *)message)->StateID));
        break;
    case step:
        Serial.println("#cmd step");
        machine->step = true;
        break;
    case dump_memory:
        Serial.println("#cmd dump");
        if ((message)->ExtraDataLen == 4)
            this->ReadMemory((DumpMemoryMessage *)message);
    };
}