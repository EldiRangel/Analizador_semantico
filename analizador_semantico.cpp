#include "analizador_semantico.h"

using namespace std;

AnalizadorSemantico::AnalizadorSemantico(const ArrayList<Token> &fuente_tokens) 
    : tokens_entrada(fuente_tokens) {}

void AnalizadorSemantico::ejecutar_verificacion() {
    tabla_nombres.clear();
    // Reiniciar lista de errores
    errores_detectados = ArrayList<DetalleError>(); 
    
    procesar_bloque_variables();
    validar_tipos_datos();
    inspeccionar_controles();
}

bool AnalizadorSemantico::es_valido() const { return errores_detectados.size() == 0; }
const ArrayList<DetalleError>& AnalizadorSemantico::obtener_lista_errores() const { return errores_detectados; }

void AnalizadorSemantico::procesar_bloque_variables() {
    bool en_var = false;
    for (size_t i = 0; i < tokens_entrada.size(); ++i) {
        if (tokens_entrada[i].value == "var") { en_var = true; continue; }
        if (tokens_entrada[i].value == "begin") break;
        if (!en_var) continue;

        if (tokens_entrada[i].type == TokenType::Identifier) {
            ArrayList<string> nombres;
            size_t j = i;
            while (j < tokens_entrada.size() && (tokens_entrada[j].type == TokenType::Identifier || tokens_entrada[j].value == ",")) {
                if (tokens_entrada[j].type == TokenType::Identifier) nombres.add(tokens_entrada[j].value);
                j++;
            }
            if (j < tokens_entrada.size() && tokens_entrada[j].value == ":") {
                j++;
                string tipo_leido = tokens_entrada[j].value;
                for (size_t n = 0; n < nombres.size(); ++n) {
                    if (tabla_nombres.count(nombres[n])) {
                        DetalleError err;
                        err.fila = tokens_entrada[i].line;
                        err.col = tokens_entrada[i].column;
                        err.mensaje_error = "Variable duplicada: " + nombres[n];
                        errores_detectados.add(err);
                    } else {
                        RegistroSimbolo reg;
                        reg.tipo_dato = tipo_leido;
                        tabla_nombres[nombres[n]] = reg;
                    }
                }
            }
            i = j;
        }
    }
}

void AnalizadorSemantico::validar_tipos_datos() {
    for (size_t i = 0; i < tokens_entrada.size(); ++i) {
        if (tokens_entrada[i].value == ":=") {
            string nombre_var = tokens_entrada[i-1].value;
            if (tabla_nombres.find(nombre_var) == tabla_nombres.end()) {
                DetalleError err;
                err.fila = tokens_entrada[i-1].line;
                err.col = tokens_entrada[i-1].column;
                err.mensaje_error = "Variable no declarada: " + nombre_var;
                errores_detectados.add(err);
                continue;
            }
            string t_esp = detectar_tipo_token(tokens_entrada[i-1]);
            string t_real = detectar_tipo_token(tokens_entrada[i+1]);

            if (t_esp == "integer" && t_real == "real") {
                DetalleError err;
                err.fila = tokens_entrada[i+1].line;
                err.col = tokens_entrada[i+1].column;
                err.mensaje_error = "Incompatibilidad: REAL a INTEGER";
                errores_detectados.add(err);
            }
            else if ((t_esp == "integer" || t_esp == "real") && t_real == "string") {
                DetalleError err;
                err.fila = tokens_entrada[i+1].line;
                err.col = tokens_entrada[i+1].column;
                err.mensaje_error = "Incompatibilidad: STRING a NUMERO";
                errores_detectados.add(err);
            }
        }
    }
}

void AnalizadorSemantico::inspeccionar_controles() {
    for (size_t i = 0; i < tokens_entrada.size(); ++i) {
        if (tokens_entrada[i].value == "if" || tokens_entrada[i].value == "while") {
            string t_cond = detectar_tipo_token(tokens_entrada[i+1]);
            if (t_cond != "boolean") {
                DetalleError err;
                err.fila = tokens_entrada[i+1].line;
                err.col = tokens_entrada[i+1].column;
                err.mensaje_error = "La condicion debe ser BOOLEAN";
                errores_detectados.add(err);
            }
        }
    }
}

string AnalizadorSemantico::detectar_tipo_token(const Token &t) const {
    if (t.type == TokenType::Number) return (t.value.find('.') != string::npos) ? "real" : "integer";
    if (t.type == TokenType::String) return "string";
    if (t.type == TokenType::Boolean) return "boolean";
    if (tabla_nombres.count(t.value)) {
        string t_raw = tabla_nombres.at(t.value).tipo_dato;
        if (t_raw == "i32" || t_raw == "integer") return "integer";
        if (t_raw == "f64" || t_raw == "real") return "real";
        return t_raw;
    }
    return "desconocido";
}
