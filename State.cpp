#include "State.h"

// State::State()
// {
// }
State::State(State *(*function)(),int ID)
{
    this->function = function;
    this->ID = ID;
}
void State::SetFunction(State *(*function)())
{
    this -> function = function;
}

State * State::Run()
{
    if (function)
        return this->function();
    else
        return this;
}