#include "automaton.h"  // Inclusión de los archivos de encabezado necesarios
#include "state.h"
#include <iostream>
#include <string>
#include <fstream>
#include <queue>


// Constructor de la clase Automaton
Automaton::Automaton(): currentState("Initial state", true, false), // Se inicializan los estados del autómata
    operators("Operator", true, false),
    variable("Variable", true, false),
    lParenthesis("lParenthesis", true, false),
    rParenthesis("rParenthesis", true, false),
    floats("float", true, false),
    ints("int", true, false),
    deathState("Death state", true, true){


    // Se definen las transiciones entre los estados
    currentState.addTransition("operador", operators);  
    currentState.addTransition("variable", variable);
    currentState.addTransition("lP", lParenthesis);
    currentState.addTransition("rP", rParenthesis);
    currentState.addTransition("float", floats);
    currentState.addTransition("int", ints);

    // Se agregan las transiciones para cada estado hacia sí mismo y hacia el estado de muerte
    operators.addTransition("operador", operators);  
    operators.addTransition("variable", variable);
    operators.addTransition("lP", lParenthesis);
    operators.addTransition("rP", rParenthesis);
    operators.addTransition("float", floats);
    operators.addTransition("int", ints); 

    variable.addTransition("operador", operators); 
    variable.addTransition("variable", variable);
    variable.addTransition("lP", lParenthesis);
    variable.addTransition("rP", rParenthesis);
    variable.addTransition("float", floats);
    variable.addTransition("int", ints); 

    lParenthesis.addTransition("operador", operators);
    lParenthesis.addTransition("variable", variable);
    lParenthesis.addTransition("lP", lParenthesis);
    lParenthesis.addTransition("rP", rParenthesis);
    lParenthesis.addTransition("float", floats);
    lParenthesis.addTransition("int", ints);

    rParenthesis.addTransition("operador", operators);
    rParenthesis.addTransition("variable", variable);
    rParenthesis.addTransition("lP", lParenthesis);
    rParenthesis.addTransition("rP", rParenthesis);
    rParenthesis.addTransition("float", floats);
    rParenthesis.addTransition("int", ints);

    floats.addTransition("operador", operators);
    floats.addTransition("variable", variable);
    floats.addTransition("lP", lParenthesis);
    floats.addTransition("rP", rParenthesis);
    floats.addTransition("float", floats);
    floats.addTransition("int", ints);

    ints.addTransition("operador", operators);
    ints.addTransition("variable", variable);
    ints.addTransition("lP", lParenthesis);
    ints.addTransition("rP", rParenthesis);
    ints.addTransition("float", floats);
    ints.addTransition("int", ints);
    
    // Se añaden transiciones hacia el estado de muerte
    operators.addTransition("death", deathState);
    variable.addTransition("death", deathState);
    lParenthesis.addTransition("death", deathState);
    rParenthesis.addTransition("death", deathState);
    floats.addTransition("death", deathState);
    ints.addTransition("death", deathState);
}

// Método para intercambiar estados
State Automaton::swap(State next){
    // Se agregan las transiciones necesarias al estado pasado como argumento
    next.addTransition("operador", operators);
    next.addTransition("operador", operators);
    next.addTransition("variable", variable);
    next.addTransition("lP", lParenthesis);
    next.addTransition("rP", rParenthesis);
    next.addTransition("float", floats);
    next.addTransition("int", ints);
    next.addTransition("death", deathState);

    return next; // Se devuelve el estado modificado
}

// Método para analizar el archivo fuente
void Automaton::lexer(std::string filename){
    std::ifstream file; // Se abre el archivo
    file.open(filename); // Se abre el archivo con el nombre proporcionado
    
    if (!file) { // Si no se puede abrir el archivo, se muestra un mensaje de error
        std::cout << "Unable to open  file"; 
    } else { // Si se abre correctamente
        std::string line;
        while (std::getline(file, line)) { // Se lee línea por línea del archivo
            validate(line); // Se valida cada línea
        }
        file.close(); // Se cierra el archivo
    }
}

// Método para validar una línea de código
void Automaton::validate(std::string line) {
    if (line.length() != 0) { // Si la línea no está vacía
        State next; // Se inicializa un nuevo estado
        std::string symbol; // Variable para almacenar el símbolo actual
        int cont = 0; // Contador para avanzar en la línea
        std::queue<char> colaDeValidacion; // Cola para validar
        bool DigitFound = false, realEncontrado  = false; // Variables de control

        for (int i = 0; i < line.length(); i++) { // Se recorre cada carácter de la línea
            if(line[i] != ' ') { // Se ignora el espacio en blanco
                if (std::isdigit(line[i])) { // Si el carácter es un dígito
                    next = currentState.nextState("int"); // Se obtiene el próximo estado
                    currentState = swap(next); // Se cambia al próximo estado
                    while (isdigit(line[i+cont]) || line[i+cont] == '.') { // Mientras se encuentren dígitos o puntos
                        symbol = symbol + line[i+cont];
                        size_t dotfound = symbol.find("."); // Se busca un punto en el símbolo
                        if (dotfound != std::string::npos) { // Si se encuentra un punto
                            realEncontrado = true; // Se indica que se encontró un número real
                        }
                        cont++; // Se avanza en la línea
                        DigitFound = true; // Se indica que se encontró un dígito
                    }
                    i= i+symbol.length()-1; // Se ajusta el índice
                    if(DigitFound && realEncontrado){ // Si se encontró un número real
                        next = currentState.nextState("float"); // Se obtiene el próximo estado
                        currentState = swap(next); // Se cambia al próximo estado
                        currentState.toString(symbol, currentState.label); // Se muestra el símbolo
                        std::cout<<std::endl; // Se imprime un salto de línea
                        realEncontrado = false; // Se reinicia la bandera
                        cont=0; // Se reinicia el contador
                        symbol=""; // Se reinicia el símbolo
                    } else if(DigitFound) { // Si se encontró un número entero
                        currentState.toString(symbol, currentState.label); // Se muestra el símbolo
                        std::cout<<std::endl; // Se imprime un salto de línea
                        cont=0; // Se reinicia el contador
                        symbol=""; // Se reinicia el símbolo
                    }
                } else if(std::isalpha(line[i])) { // Si el carácter es una letra
                    next = currentState.nextState("variable"); // Se obtiene el próximo estado
                    currentState = swap(next); // Se cambia al próximo estado
                    while (isalpha(line[i+cont])) { // Mientras se encuentren letras
                        symbol = symbol + line[i+cont];
                        cont++; // Se avanza en la línea
                        DigitFound = true; // Se indica que se encontró una letra
                    }
                    i= i+symbol.length()-1; // Se ajusta el índice
                    currentState.toString(symbol, currentState.label); // Se muestra el símbolo
                    std::cout<<std::endl; // Se imprime un salto de línea
                    if(DigitFound) { // Si se encontró una letra
                        cont=0; // Se reinicia el contador
                        symbol=""; // Se reinicia el símbolo
                    }
                } else if(line[i]=='=' || line[i]=='*' || line[i]=='+' || line[i]=='-' || line[i]=='/') { // si especificar// Si el carácter es un operador
                    next = currentState.nextState("operador"); // Se obtiene el próximo estado
                    currentState = swap(next); // Se cambia al próximo estado
                    std::string symbol(1, line[i]); // Se convierte el carácter a cadena
                    currentState.toString(symbol, currentState.label); // Se muestra el símbolo
                    std::cout<<std::endl; // Se imprime un salto de línea
                    symbol=""; // Se reinicia el símbolo
                } else if(line[i]=='(') { // Si el carácter es un paréntesis izquierdo
                    next = currentState.nextState("lP"); // Se obtiene el próximo estado
                    currentState = swap(next); // Se cambia al próximo estado
                    std::string symbol(1, line[i]); // Se convierte el carácter a cadena
                    currentState.toString(symbol, currentState.label); // Se muestra el símbolo
                    std::cout<<std::endl; // Se imprime un salto de línea
                    symbol=""; // Se reinicia el símbolo
                } else if(line[i]== ')') { // Si el carácter es un paréntesis derecho
                    next = currentState.nextState("rP"); // Se obtiene el próximo estado
                    currentState = swap(next); // Se cambia al próximo estado
                    std::string symbol(1, line[i]); // Se convierte el carácter a cadena
                    currentState.toString(symbol, currentState.label); // Se muestra el símbolo
                    std::cout<<std::endl; // Se imprime un salto de línea
                    symbol=""; // Se reinicia el símbolo
                } else { // Si el carácter no es reconocido
                    next = currentState.nextState("death"); // Se obtiene el próximo estado
                    currentState = swap(next); // Se cambia al próximo estado
                    std::string symbol(1, line[i]); // Se convierte el carácter a cadena
                    currentState.toString(symbol, currentState.label); // Se muestra el símbolo
                    symbol=""; // Se reinicia el símbolo
                }
            }
        }
    }
}
