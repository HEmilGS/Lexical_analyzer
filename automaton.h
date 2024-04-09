#ifndef AUTOMATON_H
#define AUTOMATON_H
#include <string>
#include <cctype>
#include "state.h"

class Automaton: public State{
    private: //atributos de la clase automaton, al ser un analizador lexico, necesita este tipo de estados, los cuales se asignaran sus transiciones durante la ejecucion
    State operators;
    State variable;
    State lParenthesis;
    State rParenthesis;
    State floats;
    State ints;
    State deathState;
    State currentState;

    public:
    Automaton(); //constructor por default, aqui se agregan las trancisiones para cada estado, de esta manera el usuario no tiene que crearlas, lo convierte en un analizador lexico por default
    State swap(State next); //metodo para hacer un intercambio durante la ejecucion del estado siguiente por el estado actual
    void lexer(std::string filename); //metodo que llama el usuario, recibe el nombre de su archivo y lo itera linea por linea para despues validar
    void validate(std::string line); //metodo de validacion, itera caracter por caracter con el fin de validar el tipo de caracter que se esta enviando

};



#endif