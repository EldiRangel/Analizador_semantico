#include "analizador_semantico.h"
#include "lexer.h"
#include "arraylist.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    std::ifstream archivo("codigo_semantico.txt");
    if (!archivo) { std::cerr << "No se encontro el archivo." << std::endl; return 1; }

    std::stringstream buffer;
    buffer << archivo.rdbuf();
    
    Lexer::initReservedWords();
    Lexer lex(buffer.str());
    ArrayList<Token> lista;
    Token t;
    do {
        t = lex.nextToken();
        lista.add(t);
    } while (t.type != TokenType::EndOfFile);

    AnalizadorSemantico sem(lista);
    sem.ejecutar_verificacion();

    if (sem.es_valido()) {
        std::cout << "\n>>> ANALISIS EXITOSO: Sin errores semanticos.\n";
    } else {
        std::cout << "\n>>> ERRORES SEMANTICOS DETECTADOS:\n";
        const ArrayList<DetalleError>& errs = sem.obtener_lista_errores();
        for (size_t i = 0; i < errs.size(); ++i) {
            std::cout << " [Fila " << errs[i].fila << ", Col " << errs[i].col << "] -> " << errs[i].mensaje_error << "\n";
        }
    }
    return 0;
}
