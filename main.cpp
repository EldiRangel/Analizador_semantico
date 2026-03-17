#include "analizador_semantico.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    // Lectura del archivo de texto solicitado
    ifstream archivo_fuente("codigo_semantico.txt");
    if (!archivo_fuente) {
        cerr << "Error: No se pudo localizar el archivo codigo_semantico.txt" << endl;
        return 1;
    }

    stringstream contenido;
    contenido << archivo_fuente.rdbuf();
    string texto_codigo = contenido.str();

    Lexer::initReservedWords();
    Lexer motor_lexico(texto_codigo);
    ArrayList<Token> lista_tokens;
    Token tkn;

    do {
        tkn = motor_lexico.nextToken();
        lista_tokens.add(tkn);
    } while (tkn.type != TokenType::EndOfFile);

    // Se asume que el Parser ya validó la sintaxis
    AnalizadorSemantico pro_semantico(lista_tokens);
    pro_semantico.ejecutar_verificacion();

    if (!pro_semantico.es_valido()) {
        cout << "Se encontraron discrepancias semanticas:\n";
        for (size_t i = 0; i < pro_semantico.obtener_lista_errores().size(); ++i) {
            const auto &err = pro_semantico.obtener_lista_errores().get(i);
            cout << "[Linea " << err.fila << ", Col " << err.col << "] -> " << err.mensaje_error << "\n";
        }
    } else {
        cout << "Analisis semantico completado exitosamente.\n";
    }

    return 0;
}
