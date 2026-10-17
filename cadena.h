#ifndef CADENA_H
#define CADENA_H

#include <iostream>

class MonitorRecursos; // Declaración adelantada

class Cadena
{
private:
    char* datos;
    int longitud;

public:
    // Constructores y destructor
    Cadena();
    Cadena(const char* str);
    Cadena(const Cadena& otra);
    ~Cadena();

    // Getters
    int getLongitud() const;
    const char* c_str() const;

    // Métodos solicitados en el diagrama UML
    void asignar(const char* str);
    bool comparar(const Cadena& otra) const;

    // Sobrecarga de operadores (cumple con el requisito de al menos 2 operadores)
    Cadena& operator=(const Cadena& otra);
    bool operator==(const Cadena& otra) const;
    bool operator!=(const Cadena& otra) const;

    // Métodos utilitarios para rutas, comparaciones de fechas e I/O
    bool contiene(const Cadena& sub) const;
    int compararLexicografico(const Cadena& otra) const; // Útil para ordenar y buscar fechas
    friend std::ostream& operator<<(std::ostream& os, const Cadena& c);
    friend std::istream& operator>>(std::istream& is, Cadena& c);
};

#endif // CADENA_H