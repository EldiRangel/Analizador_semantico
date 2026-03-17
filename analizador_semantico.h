#ifndef ANALIZADOR_SEMANTICO_H
#define ANALIZADOR_SEMANTICO_H

#include "../LibreriaDeSoportes/arraylist.h"
#include <string>
#include <unordered_map>
#include "../analizadorlexico/lexer.h"

using namespace std;

// Estructura para registrar los errores encontrados
struct DetalleError {
    int fila;
    int col;
    string mensaje_error;
};

// Información de los identificadores en la tabla
struct RegistroSimbolo {
    string tipo_dato;
    bool modificable;
};

class AnalizadorSemantico {
public:
    AnalizadorSemantico(const ArrayList<Token> &fuente_tokens);
    void ejecutar_verificacion();
    bool es_valido() const;
    const ArrayList<DetalleError> &obtener_lista_errores() const;

private:
    const ArrayList<Token> &tokens_entrada;
    unordered_map<string, RegistroSimbolo> tabla_nombres; 
    ArrayList<DetalleError> errores_detectados;

    void procesar_bloque_variables();
    void validar_tipos_datos();
    void inspeccionar_controles();

    string detectar_tipo_token(const Token &t) const;
    string evaluar_tipo_expresion(size_t index_inicio, size_t index_fin, int &l_err, int &c_err, string &m_err) const;
    size_t buscar_final_expresion(size_t inicio, size_t maximo) const;
};

#endif
