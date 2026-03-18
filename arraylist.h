#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <cstddef>
#include <stdexcept>

using namespace std;

template<typename T>
class ArrayList {
private:
    T* data;
    size_t count;
    size_t capacity;
    size_t it_index; // Renombrado para diferenciarlo de funciones
    
    void expand(); // Función para redimensionar el arreglo
    
public:
    ArrayList();
    ~ArrayList();
    
    // Constructor de copia para evitar errores de memoria
    ArrayList(const ArrayList<T>& other) {
        count = other.count;
        capacity = other.capacity;
        it_index = other.it_index;
        data = new T[capacity];
        for(size_t i = 0; i < count; i++) data[i] = other.data[i];
    }

    // Operador de asignación
    ArrayList<T>& operator=(const ArrayList<T>& other) {
        if (this != &other) {
            delete[] data;
            count = other.count;
            capacity = other.capacity;
            it_index = other.it_index;
            data = new T[capacity];
            for(size_t i = 0; i < count; i++) data[i] = other.data[i];
        }
        return *this;
    }

    void add(const T& element);
    T& get(size_t index);
    const T& get(size_t index) const;
    T& operator[](size_t index) { return get(index); }
    const T& operator[](size_t index) const { return get(index); }
    
    void remove(size_t index);
    size_t size() const { return count; }
    bool empty() const { return count == 0; }
    
    // Iteración simple
    T* next();
    void reset_iterator() { it_index = 0; }
};

template<typename T>
ArrayList<T>::ArrayList() : count(0), capacity(10), it_index(0) {
    data = new T[capacity];
}

template<typename T>
ArrayList<T>::~ArrayList() {
    delete[] data;
}

template<typename T>
void ArrayList<T>::expand() {
    capacity *= 2;
    T* newData = new T[capacity];
    for (size_t i = 0; i < count; i++) newData[i] = data[i];
    delete[] data;
    data = newData;
}

template<typename T>
void ArrayList<T>::add(const T& element) {
    if (count >= capacity) expand();
    data[count++] = element;
}

template<typename T>
T& ArrayList<T>::get(size_t index) {
    if (index >= count) throw out_of_range("Fuera de rango");
    return data[index];
}

template<typename T>
const T& ArrayList<T>::get(size_t index) const {
    if (index >= count) throw out_of_range("Fuera de rango");
    return data[index];
}

template<typename T>
void ArrayList<T>::remove(size_t index) {
    if (index >= count) throw out_of_range("Fuera de rango");
    for (size_t i = index; i < count - 1; i++) data[i] = data[i + 1];
    count--;
    if (it_index > index) it_index--;
}

template<typename T>
T* ArrayList<T>::next() {
    return (it_index >= count) ? nullptr : &data[it_index++];
}

#endif
