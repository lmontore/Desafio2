#ifndef FOTO_H
#define FOTO_H

#include "Cadena.h"

class Foto
{
private:
    Cadena fechaCaptura;
    long tamano;
    Cadena rutaArchivo;
    int cantidadLikes;

public:
    Foto();
    Foto(const Cadena& fecha, long tam, const Cadena& ruta);
    Foto(const Foto& otra);
    ~Foto();

    Foto& operator=(const Foto& otra);

    void darLike();
    void quitarLike();
    void mostrarFoto() const;

    const Cadena& getRutaArchivo() const;
    int getCantidadLikes() const;
};

#endif // FOTO_H