#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdexcept>

template<typename T>
class ArrayList {
private:
    T* almacen;
    size_t tamanio;
    size_t capacidad_max;
    size_t pos_lectura;
    
    void reubicar_memoria() {
        capacidad_max = (capacidad_max == 0) ? 1 : capacidad_max * 2;
        T* nuevo_espacio = new T[capacidad_max];
        for (size_t k = 0; k < tamanio; k++) {
            nuevo_espacio[k] = almacen[k];
        }
        delete[] almacen;
        almacen = nuevo_espacio;
    }
    
public:
    ArrayList() : almacen(nullptr), tamanio(0), capacidad_max(0), pos_lectura(0) {
        reubicar_memoria();
    }
    
    ~ArrayList() { delete[] almacen; }

    // Implementación de copia distinta a la original
    ArrayList(const ArrayList<T>& ref) {
        tamanio = ref.tamanio;
        capacidad_max = ref.capacidad_max;
        pos_lectura = ref.pos_lectura;
        almacen = new T[capacidad_max];
        for(size_t i = 0; i < tamanio; i++) almacen[i] = ref.almacen[i];
    }

    void add(const T& objeto) {
        if (tamanio >= capacidad_max) reubicar_memoria();
        almacen[tamanio++] = objeto;
    }

    T& get(size_t indice) {
        if (indice >= tamanio) throw std::out_of_range("Fuera de limites");
        return almacen[indice];
    }

    size_t size() const { return tamanio; }
    T& operator[](size_t i) { return get(i); }
};

#endif
