#ifndef MENSAJE_H
#define MENSAJE_H

#include "cadena.h"

class Usuario; // Declaracion adelantada

class Mensaje
{
private:
    Cadena contenido;
    Cadena fechaEnvio;
    bool leido;
    Usuario* emisor;
    Usuario* receptor;

public:
    Mensaje();
    Mensaje(const Cadena& cont, const Cadena& fecha, Usuario* em, Usuario* rec);
    Mensaje(const Mensaje& otro);
    ~Mensaje();

    Mensaje& operator=(const Mensaje& otro);

    void marcarLeido();
    void mostrarMensaje() const;

    bool esLeido() const;
    const Cadena& getFechaEnvio() const;
    const Cadena& getContenido() const;
    Usuario* getEmisor() const;
    Usuario* getReceptor() const;
};

#endif // MENSAJE_H