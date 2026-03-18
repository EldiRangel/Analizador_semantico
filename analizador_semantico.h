#ifndef ANALIZADOR_SEMANTICO_H
#define ANALIZADOR_SEMANTICO_H

#include "arraylist.h"      
#include "lexer.h"
#include <string>
#include <unordered_map>

struct DetalleError {
    int fila;
    int col;
    std::string mensaje_error;
};

struct RegistroSimbolo {
    std::string tipo_dato;
   
};

class AnalizadorSemantico {
public:
    AnalizadorSemantico(const ArrayList<Token> &fuente_tokens);
    void ejecutar_verificacion();
    bool es_valido() const;
    const ArrayList<DetalleError>& obtener_lista_errores() const;

private:
    const ArrayList<Token> &tokens_entrada;
    std::unordered_map<std::string, RegistroSimbolo> tabla_nombres; 
    ArrayList<DetalleError> errores_detectados;

    void procesar_bloque_variables();
    void validar_tipos_datos();
    void inspeccionar_controles();
    std::string detectar_tipo_token(const Token &t) const;
};

#endif
