#ifndef STATE_H_
#define STATE_H_
class State
{
  private:
    State *(*function)();
  public:
    // not null
    int ID;

    // State();
    State(State *(*function)(),int ID);
    void SetFunction(State *(*function)());
    State *Run();
};
#endif