#include "state.h"
#include <iostream>

State::State(){ //constructor por default, queda vacio ya que solo se usa para declarar el estado "next" en el automata, sus atributos son asignados durante la ejecucion
    
}

State::State(std::string label, bool isFinal, bool isDeath ){ //constructor de la clase  State que recibe como parametros el nombre del estado, si es final y si es estado muerto
        this->label = label;
        this->isFinal = isFinal;
        this->isDeath = isDeath;
    }

    void State::addTransition(std::string symbol, State nextState){ //metodo para agregar las transiciones del DFA dentro de un hash, recibe a donde sera la siguiente transicion y el estado al que corresponde
        transition.insert(std::make_pair(symbol, nextState));

    }

State State::nextState(std::string symbol) { //metodo para conseguir el siguiente estado durante la ejecucion del automata, comprueba que exista el simbolo que se envia y retorna su par, en este caso, el estado siguiente 
    auto it = transition.find(symbol);
    if (it != transition.end()) {
        return it->second;
    } else {
        return State("Death state", false, true);
    }
}



    void State::toString(std::string symbol, std::string stateLabel) { //metodo para imprimir  el estado actual con su simbolo y su tipo 
    //std::cout << label << " Is final " << (isFinal ? "true" : "false") << "   Is death " << (isDeath ? "true " : "false ") << " "<< symbol<< std::endl;
    std::cout<<"Token: "<< symbol << " Type: " << stateLabel << std::endl;

    }