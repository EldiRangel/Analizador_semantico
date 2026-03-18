#include "lexer.h"
#include <cctype>

std::unordered_map<std::string, TokenType> Lexer::mapa_palabras;

Lexer::Lexer(const std::string &fuente) : stream_datos(fuente), cursor(0), nro_linea(1), nro_col(1) {}

void Lexer::initReservedWords() {
    mapa_palabras["var"] = TokenType::Keyword;
    mapa_palabras["begin"] = TokenType::Keyword;
    mapa_palabras["end"] = TokenType::Keyword;
    mapa_palabras["if"] = TokenType::Keyword;
    mapa_palabras["then"] = TokenType::Keyword;
    mapa_palabras["while"] = TokenType::Keyword;
    mapa_palabras["do"] = TokenType::Keyword;
    mapa_palabras["integer"] = TokenType::Type;
    mapa_palabras["real"] = TokenType::Type;
    mapa_palabras["string"] = TokenType::Type;
    mapa_palabras["boolean"] = TokenType::Type;
    mapa_palabras["true"] = TokenType::Boolean;
    mapa_palabras["false"] = TokenType::Boolean;
}

void Lexer::limpiar_espacios() {
    while (cursor < stream_datos.length()) {
        char c = ver_proximo();
        if (isspace(c)) {
            extraer_char();
        } else if (c == '/' && stream_datos[cursor + 1] == '/') {
            while (cursor < stream_datos.length() && ver_proximo() != '\n') {
                extraer_char();
            }
        } else {
            break;
        }
    }
}

Token Lexer::nextToken() {
    limpiar_espacios();
    if (cursor >= stream_datos.length()) return {TokenType::EndOfFile, "", nro_linea, nro_col};

    int l = nro_linea, c = nro_col;
    char actual = ver_proximo();

    if (isalpha(actual)) {
        std::string palabra = "";
        while (isalnum(ver_proximo())) palabra += extraer_char();
        if (mapa_palabras.count(palabra)) return {mapa_palabras[palabra], palabra, l, c};
        return {TokenType::Identifier, palabra, l, c};
    }

    if (isdigit(actual)) {
        std::string num = "";
        while (isdigit(ver_proximo()) || ver_proximo() == '.') num += extraer_char();
        return {TokenType::Number, num, l, c};
    }

    if (actual == '"') {
        extraer_char(); // Quita "
        std::string cadena = "";
        while (ver_proximo() != '"' && ver_proximo() != '\0') cadena += extraer_char();
        if (ver_proximo() == '"') extraer_char();
        return {TokenType::String, cadena, l, c};
    }

    std::string simb(1, extraer_char());
    if (simb == ":" && ver_proximo() == '=') {
        simb += extraer_char();
        return {TokenType::Operator, simb, l, c};
    }
    
    return {TokenType::Unknown, simb, l, c};
}

char Lexer::ver_proximo() const { return cursor < stream_datos.length() ? stream_datos[cursor] : '\0'; }
char Lexer::extraer_char() {
    char c = stream_datos[cursor++];
    if (c == '\n') { nro_linea++; nro_col = 1; } else nro_col++;
    return c;
}
