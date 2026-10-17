#include "usuario.h"
#include "MonitorRecursos.h"
#include <iostream>

Usuario::Usuario() {
    albumes = nullptr;
    cantidadAlbumes = 0;
    capacidadAlbumes = 0;
}

Usuario::Usuario(const Cadena& nombre,
                 const Cadena& fechaNac,
                 const Cadena& mail,
                 const Cadena& foto,
                 const Cadena& pass,
                 const Cadena& ciu,
                 const Cadena& pai,
                 const Cadena& fechaInsc) {
    nombreUsuario = nombre;
    fechaNacimiento = fechaNac;
    correo = mail;
    fotoPerfil = foto;
    contrasena = pass;
    ciudad = ciu;
    pais = pai;
    fechaInscripcion = fechaInsc;

    albumes = nullptr;
    cantidadAlbumes = 0;
    capacidadAlbumes = 0;
}

Usuario::Usuario(const Usuario& otro) {
    nombreUsuario = otro.nombreUsuario;
    fechaNacimiento = otro.fechaNacimiento;
    correo = otro.correo;
    fotoPerfil = otro.fotoPerfil;
    contrasena = otro.contrasena;
    ciudad = otro.ciudad;
    pais = otro.pais;
    fechaInscripcion = otro.fechaInscripcion;
    cantidadAlbumes = otro.cantidadAlbumes;
    capacidadAlbumes = otro.capacidadAlbumes;

    if (capacidadAlbumes > 0) {
        albumes = new Album*[capacidadAlbumes];
        MonitorRecursos::registrarMemoria(capacidadAlbumes * sizeof(Album*));
        for (int i = 0; i < cantidadAlbumes; i++) {
            albumes[i] = new Album(*(otro.albumes[i]));
            MonitorRecursos::registrarMemoria(sizeof(Album));
            MonitorRecursos::contarIteracionDirecta();
        }
    } else {
        albumes = nullptr;
    }
}

Usuario::~Usuario() {
    if (albumes != nullptr) {
        for (int i = 0; i < cantidadAlbumes; i++) {
            if (albumes[i] != nullptr) {
                MonitorRecursos::liberarMemoria(sizeof(Album));
                delete albumes[i];
            }
        }
        MonitorRecursos::liberarMemoria(capacidadAlbumes * sizeof(Album*));
        delete[] albumes;
        albumes = nullptr;
    }
}

Usuario& Usuario::operator=(const Usuario& otro) {
    if (this != &otro) {
        if (albumes != nullptr) {
            for (int i = 0; i < cantidadAlbumes; i++) {
                if (albumes[i] != nullptr) {
                    MonitorRecursos::liberarMemoria(sizeof(Album));
                    delete albumes[i];
                }
            }
            MonitorRecursos::liberarMemoria(capacidadAlbumes * sizeof(Album*));
            delete[] albumes;
            albumes = nullptr;
        }

        nombreUsuario = otro.nombreUsuario;
        fechaNacimiento = otro.fechaNacimiento;
        correo = otro.correo;
        fotoPerfil = otro.fotoPerfil;
        contrasena = otro.contrasena;
        ciudad = otro.ciudad;
        pais = otro.pais;
        fechaInscripcion = otro.fechaInscripcion;
        cantidadAlbumes = otro.cantidadAlbumes;
        capacidadAlbumes = otro.capacidadAlbumes;

        if (capacidadAlbumes > 0) {
            albumes = new Album*[capacidadAlbumes];
            MonitorRecursos::registrarMemoria(capacidadAlbumes * sizeof(Album*));
            for (int i = 0; i < cantidadAlbumes; i++) {
                albumes[i] = new Album(*(otro.albumes[i]));
                MonitorRecursos::registrarMemoria(sizeof(Album));
                MonitorRecursos::contarIteracionDirecta();
            }
        } else {
            albumes = nullptr;
        }
    }
    return *this;
}

const Cadena& Usuario::getNombreUsuario() const {
    return nombreUsuario;
}

const Cadena& Usuario::getContrasena() const {
    return contrasena;
}

const Cadena& Usuario::getFotoPerfil() const {
    return fotoPerfil;
}

int Usuario::getCantidadAlbumes() const {
    return cantidadAlbumes;
}

Album* Usuario::getAlbum(int indice) const {
    if (indice >= 0 && indice < cantidadAlbumes) {
        return albumes[indice];
    }
    return nullptr;
}

void Usuario::mostrarPerfil() const {
    std::cout << "\n============================================\n";
    std::cout << "USUARIO: " << nombreUsuario << "\n";
    std::cout << "Foto de perfil (Ruta): " << fotoPerfil << "\n";
    std::cout << "Correo: " << correo << " | Ciudad: " << ciudad << ", " << pais << "\n";
    std::cout << "Fecha de Nacimiento: " << fechaNacimiento << "\n";
    std::cout << "Inscripcion: " << fechaInscripcion << "\n";
    std::cout << "Total de albumes: " << cantidadAlbumes << "\n";
    std::cout << "============================================\n";
}

bool Usuario::agregarAlbum(Album* nuevoAlbum) {
    if (nuevoAlbum == nullptr) return false;

    if (cantidadAlbumes >= capacidadAlbumes) {
        int nuevaCap = (capacidadAlbumes == 0) ? 2 : capacidadAlbumes * 2;
        Album** nuevoArreglo = new Album*[nuevaCap];
        MonitorRecursos::registrarMemoria(nuevaCap * sizeof(Album*));

        for (int i = 0; i < cantidadAlbumes; i++) {
            nuevoArreglo[i] = albumes[i];
            MonitorRecursos::contarIteracionDirecta();
        }

        if (albumes != nullptr) {
            MonitorRecursos::liberarMemoria(capacidadAlbumes * sizeof(Album*));
            delete[] albumes;
        }

        albumes = nuevoArreglo;
        capacidadAlbumes = nuevaCap;
    }

    albumes[cantidadAlbumes] = nuevoAlbum;
    cantidadAlbumes++;
    return true;
}