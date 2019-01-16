#include "fiKnightSerialDebugger.h"


class FiKnightSerialReader : public FiKnightSerialDebugger
{
    public:
    FiKnightSerialReader(void (*SerialReceivedHandler)(int size, byte *data));
    virtual void ExecuteSerialDebugCommand(FiKnight *machine, DebugMessage *message) override;
};