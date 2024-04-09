#ifndef STATES_H
#define STATES_H

#include <string>
#include <unordered_map>
 
class State{
    public:
    bool isFinal, isDeath;
    std::string label;
    std::unordered_map<std::string, State> transition;
    
    State();

    State(std::string label, bool isFinal, bool isDeath);
    void addTransition(std::string symbol, State nextState);
    State nextState(std::string  symbol);
    void toString(std::string symbol, std::string stateLabel);
};

#endif