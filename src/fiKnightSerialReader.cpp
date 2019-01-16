#include "fiKnightSerialReader.h"

FiKnightSerialReader::FiKnightSerialReader(void (*SerialReceivedHandler)(int size, byte *data)) : FiKnightSerialDebugger()
{
    this->SetSerialReceivedHandler(SerialReceivedHandler);
}
void FiKnightSerialReader::ExecuteSerialDebugCommand(FiKnight *machine, DebugMessage *message)
{

}