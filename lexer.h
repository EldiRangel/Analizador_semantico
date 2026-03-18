#ifndef LEXER_PROYECTO_H
#define LEXER_PROYECTO_H

#include <string>
#include <unordered_map>

enum class TokenType {
    Keyword, Operator, Function, Delimiter, Identifier,
    Number, String, Comment, Unknown, Type, Boolean, EndOfFile
};

struct Token {
    TokenType type; 
    std::string value; 
    int line;
    int column;
};

class Lexer {
public:
    Lexer(const std::string &fuente);
    Token nextToken(); 
    static void initReservedWords();

private:
    std::string stream_datos;
    size_t cursor;
    int nro_linea;
    int nro_col;

    char ver_proximo() const;
    char extraer_char();
    void limpiar_espacios();
    static std::unordered_map<std::string, TokenType> mapa_palabras;
};
#endif
