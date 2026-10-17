#ifndef ALBUM_H
#define ALBUM_H

#include "Cadena.h"
#include "Foto.h"

class Album
{
private:
    Cadena nombre;
    Cadena ruta;
    int privacidad; // 0: Privado, 1: Solo amigos, 2: Publico
    Foto* fotos;
    int cantidadFotos;
    int capacidadFotos;

    void redimensionar();

public:
    Album();
    Album(const Cadena& nom, const Cadena& rut, int priv);
    Album(const Album& otro);
    ~Album();

    Album& operator=(const Album& otro);

    bool agregarFoto(const Foto& nuevaFoto);
    void mostrarCarrusel();
    bool puedeVisualizarse(bool esAmigo, bool esDueno) const;

    const Cadena& getNombre() const;
    int getPrivacidad() const;
    int getCantidadFotos() const;
};

#endif // ALBUM_H