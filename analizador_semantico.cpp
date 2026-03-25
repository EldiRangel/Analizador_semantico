#include "analizador_semantico.h"

using namespace std;

AnalizadorSemantico::AnalizadorSemantico(const ArrayList<Token> &fuente_tokens) 
    : tokens_entrada(fuente_tokens) {}

void AnalizadorSemantico::ejecutar_verificacion() {
    tabla_nombres.clear();
    errores_detectados = ArrayList<DetalleError>(); 
    
    // Lee declaraciones (var ... begin)
    procesar_bloque_variables();
    
    // Revisa la lógica de asignaciones y controles
    validar_tipos_datos();
    inspeccionar_controles();
}

void AnalizadorSemantico::procesar_bloque_variables() {
    bool en_var = false;
    for (size_t i = 0; i < tokens_entrada.size(); ++i) {
        if (tokens_entrada[i].value == "var") { en_var = true; continue; }
        if (tokens_entrada[i].value == "begin") break; 
        if (!en_var) continue;

        // Si encuentra un identificador (nombre de variable)
        if (tokens_entrada[i].type == TokenType::Identifier) {
            ArrayList<string> nombres_temporales;
            size_t j = i;
            
            // Leemos todos los nombres antes de los dos puntos (ej: a, b, c : integer)
            while (j < tokens_entrada.size() && (tokens_entrada[j].type == TokenType::Identifier || tokens_entrada[j].value == ",")) {
                if (tokens_entrada[j].type == TokenType::Identifier) nombres_temporales.add(tokens_entrada[j].value);
                j++;
            }

            // Busca el tipo después de los ":"
            if (j < tokens_entrada.size() && tokens_entrada[j].value == ":") {
                j++;
                string tipo_encontrado = tokens_entrada[j].value;
                
                for (size_t n = 0; n < nombres_temporales.size(); ++n) {
                    
                    if (tabla_nombres.count(nombres_temporales[n])) {
                        DetalleError err = {tokens_entrada[i].line, tokens_entrada[i].column, "Error: Variable duplicada '" + nombres_temporales[n] + "'"};
                        errores_detectados.add(err);
                    } else {
                        // Guarda en la tabla de símbolos
                        RegistroSimbolo reg;
                        reg.tipo_dato = tipo_encontrado;
                        tabla_nombres[nombres_temporales[n]] = reg;
                    }
                }
            }
            i = j; 
        }
    }
}

void AnalizadorSemantico::validar_tipos_datos() {
    for (size_t i = 0; i < tokens_entrada.size(); ++i) {
        // Busca el símbolo de asignación ":="
        if (tokens_entrada[i].value == ":=") {
            string variable_izquierda = tokens_entrada[i-1].value;

            // La variable debe existir en la tabla
            if (tabla_nombres.find(variable_izquierda) == tabla_nombres.end()) {
                DetalleError err = {tokens_entrada[i-1].line, tokens_entrada[i-1].column, "Error: Variable '" + variable_izquierda + "' no declarada"};
                errores_detectados.add(err);
                continue;
            }

            // Obtiene ambos tipos de tokens de ambos lados para comparar
            string tipo_esperado = detectar_tipo_token(tokens_entrada[i-1]);
            string tipo_recibido = detectar_tipo_token(tokens_entrada[i+1]);

            
            if (tipo_esperado == "integer" && tipo_recibido == "real") {
                DetalleError err = {tokens_entrada[i+1].line, tokens_entrada[i+1].column, "Incompatibilidad: No puedes asignar un REAL a un INTEGER"};
                errores_detectados.add(err);
            }
            //  No meter texto en variables numéricas
            else if ((tipo_esperado == "integer" || tipo_esperado == "real") && tipo_recibido == "string") {
                DetalleError err = {tokens_entrada[i+1].line, tokens_entrada[i+1].column, "Incompatibilidad: No puedes asignar STRING a una variable numerica"};
                errores_detectados.add(err);
            }
        }
    }
}

void AnalizadorSemantico::inspeccionar_controles() {
    for (size_t i = 0; i < tokens_entrada.size(); ++i) {
        // Revisamos IF y WHILE
        if (tokens_entrada[i].value == "if" || tokens_entrada[i].value == "while") {
            string t_condicion = detectar_tipo_token(tokens_entrada[i+1]);

            // RLa condición debe ser booleana (true, false o variable boolean)
            if (t_condicion != "boolean") {
                DetalleError err = {tokens_entrada[i+1].line, tokens_entrada[i+1].column, "Error: La condicion del '" + tokens_entrada[i].value + "' debe ser BOOLEAN"};
                errores_detectados.add(err);
            }
        }
    }
}

string AnalizadorSemantico::detectar_tipo_token(const Token &t) const {
    // Si es un número, revisamos si tiene punto decimal
    if (t.type == TokenType::Number) return (t.value.find('.') != string::npos) ? "real" : "integer";
    
    // Si es texto directo
    if (t.type == TokenType::String) return "string";
    
    // Si es true/false
    if (t.type == TokenType::Boolean) return "boolean";
    
    // Si es una variable, buscamos tipo real en la tabla
    if (tabla_nombres.count(t.value)) {
        string t_raw = tabla_nombres.at(t.value).tipo_dato;
       
        if (t_raw == "i32" || t_raw == "integer") return "integer";
        if (t_raw == "f64" || t_raw == "real") return "real";
        if (t_raw == "bool" || t_raw == "boolean") return "boolean";
        return t_raw;
    }
    return "desconocido";
}