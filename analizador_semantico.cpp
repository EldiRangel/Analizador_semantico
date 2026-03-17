#include "analizador_semantico.h"

AnalizadorSemantico::AnalizadorSemantico(const ArrayList<Token> &fuente_tokens)
    : tokens_entrada(fuente_tokens) {}

void AnalizadorSemantico::ejecutar_verificacion() {
    tabla_nombres.clear();
    errores_detectados = ArrayList<DetalleError>();

    procesar_bloque_variables(); //
    validar_tipos_datos();       //
    inspeccionar_controles();    //
}

bool AnalizadorSemantico::es_valido() const {
    return errores_detectados.size() == 0;
}

const ArrayList<DetalleError> &AnalizadorSemantico::obtener_lista_errores() const {
    return errores_detectados;
}

// Lógica para registrar variables en el diccionario
void AnalizadorSemantico::procesar_bloque_variables() {
    bool activo_var = false;
    for (size_t idx = 0; idx < tokens_entrada.size(); ++idx) {
        if (tokens_entrada[idx].value == "var") {
            activo_var = true;
            continue;
        }
        if (tokens_entrada[idx].value == "begin") break;
        if (!activo_var) continue;

        if (tokens_entrada[idx].type == TokenType::Identifier) {
            ArrayList<string> nombres_encontrados;
            size_t cursor = idx;

            while (cursor < tokens_entrada.size() && tokens_entrada[cursor].type == TokenType::Identifier) {
                nombres_encontrados.add(tokens_entrada[cursor].value);
                cursor++;
                if (cursor < tokens_entrada.size() && tokens_entrada[cursor].value == ",") cursor++;
                else break;
            }

            if (cursor >= tokens_entrada.size() || tokens_entrada[cursor].value != ":") {
                idx = cursor;
                continue;
            }

            cursor++;
            string tipo_traducido = tokens_entrada[cursor].value;
            // Traducción de tipos Pascal a internos
            if (tipo_traducido == "integer") tipo_traducido = "i32";
            else if (tipo_traducido == "real") tipo_traducido = "f64";
            else if (tipo_traducido == "string") tipo_traducido = "String";
            else if (tipo_traducido == "boolean") tipo_traducido = "bool";

            for (size_t n = 0; n < nombres_encontrados.size(); ++n) {
                const string &nom = nombres_encontrados.get(n);
                if (tabla_nombres.count(nom)) {
                    errores_detectados.add({tokens_entrada[idx].line, tokens_entrada[idx].column, "Identificador duplicado: " + nom});
                } else {
                    tabla_nombres[nom] = RegistroSimbolo{tipo_traducido, true};
                }
            }
            idx = cursor;
        }
    }
}
// ... resto de métodos (validar_tipos_datos e inspeccionar_controles) siguiendo este estilo
