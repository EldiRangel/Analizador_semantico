#ifndef LEXER_PROYECTO_H
#define LEXER_PROYECTO_H

#include <string>
#include <unordered_map>

// Usamos tu propia estructura en lugar de vectores
#include "arraylist.h"

using namespace std;

enum class TokenType {
    Keyword,    // Palabras como 'begin', 'if', 'then'
    Operator,   // '+', '-', ':=', etc.
    Function,   // Funciones del lenguaje
    Delimiter,  // ';', '(', ')', ','
    Identifier, // Nombres de variables
    Number,     // Valores numéricos
    String,     // Texto entre comillas
    Comment,    // Comentarios del código
    Unknown,    // Caracteres no reconocidos
    Type,       // 'integer', 'boolean'
    Boolean,    // 'true', 'false'
    EndOfFile   // Fin del archivo
};

struct Token {
    TokenType type; 
    string value; 
    int line;
    int column;
};

class Lexer {
public:
    // Constructor con el código fuente
    Lexer(const string &src); 
    
    // Obtiene el siguiente token del flujo
    Token nextToken(); 
    
    // Manejo de palabras reservadas
    static void loadReservedWords(const string &filename); 
    static void initReservedWords();

private:
    string input; 
    size_t pos = 0; 
    int line = 1, column = 1;

    // Métodos de apoyo para el análisis
    char peekNext() const; 
    char peek() const; 
    char get(); 
    void skipWhitespace();
    bool isDelimiter(char c) const; 
    static TokenType stringToTokenType(const string &str); 

    // Mapa de búsqueda rápida para palabras clave
    static unordered_map<string, TokenType> reservedWords;
};

#endif
