#ifndef USUARIO_H
#define USUARIO_H

#include "cadena.h"
#include "album.h"

class Usuario
{
private:
    Cadena nombreUsuario;
    Cadena fechaNacimiento;
    Cadena correo;
    Cadena fotoPerfil;
    Cadena contrasena;
    Cadena ciudad;
    Cadena pais;
    Cadena fechaInscripcion;

    Album** albumes;
    int cantidadAlbumes;
    int capacidadAlbumes;

public:
    Usuario();
    Usuario(const Cadena& nombreUsuario,
            const Cadena& fechaNacimiento,
            const Cadena& correo,
            const Cadena& fotoPerfil,
            const Cadena& contrasena,
            const Cadena& ciudad,
            const Cadena& pais,
            const Cadena& fechaInscripcion);
    Usuario(const Usuario& otro);
    ~Usuario();

    Usuario& operator=(const Usuario& otro);

    // Getters
    const Cadena& getNombreUsuario() const;
    const Cadena& getContrasena() const;
    const Cadena& getFotoPerfil() const;
    int getCantidadAlbumes() const;
    Album* getAlbum(int indice) const;

    // Métodos del UML
    void mostrarPerfil() const;
    bool agregarAlbum(Album* nuevoAlbum);
};

#endif // USUARIO_H