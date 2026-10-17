#include "Album.h"
#include "MonitorRecursos.h"
#include <iostream>

void Album::redimensionar() {
    int nuevaCapacidad = (capacidadFotos == 0) ? 4 : capacidadFotos * 2;
    Foto* nuevoArreglo = new Foto[nuevaCapacidad];
    MonitorRecursos::registrarMemoria(nuevaCapacidad * sizeof(Foto));

    for (int i = 0; i < cantidadFotos; i++) {
        nuevoArreglo[i] = fotos[i];
        MonitorRecursos::contarIteracionDirecta();
    }

    if (fotos != nullptr) {
        MonitorRecursos::liberarMemoria(capacidadFotos * sizeof(Foto));
        delete[] fotos;
    }

    fotos = nuevoArreglo;
    capacidadFotos = nuevaCapacidad;
}

Album::Album() {
    privacidad = 0;
    fotos = nullptr;
    cantidadFotos = 0;
    capacidadFotos = 0;
}

Album::Album(const Cadena& nom, const Cadena& rut, int priv) {
    nombre = nom;
    ruta = rut;
    privacidad = priv;
    fotos = nullptr;
    cantidadFotos = 0;
    capacidadFotos = 0;
}

Album::Album(const Album& otro) {
    nombre = otro.nombre;
    ruta = otro.ruta;
    privacidad = otro.privacidad;
    cantidadFotos = otro.cantidadFotos;
    capacidadFotos = otro.capacidadFotos;

    if (capacidadFotos > 0) {
        fotos = new Foto[capacidadFotos];
        MonitorRecursos::registrarMemoria(capacidadFotos * sizeof(Foto));
        for (int i = 0; i < cantidadFotos; i++) {
            fotos[i] = otro.fotos[i];
            MonitorRecursos::contarIteracionDirecta();
        }
    } else {
        fotos = nullptr;
    }
}

Album::~Album() {
    if (fotos != nullptr) {
        MonitorRecursos::liberarMemoria(capacidadFotos * sizeof(Foto));
        delete[] fotos;
        fotos = nullptr;
    }
}

Album& Album::operator=(const Album& otro) {
    if (this != &otro) {
        if (fotos != nullptr) {
            MonitorRecursos::liberarMemoria(capacidadFotos * sizeof(Foto));
            delete[] fotos;
        }

        nombre = otro.nombre;
        ruta = otro.ruta;
        privacidad = otro.privacidad;
        cantidadFotos = otro.cantidadFotos;
        capacidadFotos = otro.capacidadFotos;

        if (capacidadFotos > 0) {
            fotos = new Foto[capacidadFotos];
            MonitorRecursos::registrarMemoria(capacidadFotos * sizeof(Foto));
            for (int i = 0; i < cantidadFotos; i++) {
                fotos[i] = otro.fotos[i];
                MonitorRecursos::contarIteracionDirecta();
            }
        } else {
            fotos = nullptr;
        }
    }
    return *this;
}

bool Album::agregarFoto(const Foto& nuevaFoto) {
    if (cantidadFotos >= capacidadFotos) {
        redimensionar();
    }
    fotos[cantidadFotos] = nuevaFoto;
    cantidadFotos++;
    return true;
}

bool Album::puedeVisualizarse(bool esAmigo, bool esDueno) const {
    if (esDueno) return true;
    if (privacidad == 2) return true;              // Publico
    if (privacidad == 1 && esAmigo) return true;   // Solo amigos
    return false;                                  // Privado
}

void Album::mostrarCarrusel() {
    if (cantidadFotos == 0) {
        std::cout << "\nEl album \"" << nombre << "\" no contiene fotos.\n";
        return;
    }

    int indiceActual = 0;
    char opcion = ' ';

    while (opcion != 'S' && opcion != 's') {
        fotos[indiceActual].mostrarFoto();
        std::cout << "Foto (" << (indiceActual + 1) << "/" << cantidadFotos << ")\n";
        std::cout << "Acciones: [N] Siguiente | [P] Previa | [L] Dar/Quitar Like | [S] Salir: ";
        std::cin >> opcion;
        MonitorRecursos::contarIteracionDirecta();

        if (opcion == 'N' || opcion == 'n') {
            // Ciclo circular hacia adelante
            indiceActual = (indiceActual + 1) % cantidadFotos;
        } else if (opcion == 'P' || opcion == 'p') {
            // Ciclo circular hacia atras
            indiceActual = (indiceActual - 1 + cantidadFotos) % cantidadFotos;
        } else if (opcion == 'L' || opcion == 'l') {
            int subOpc = 0;
            std::cout << "1. Dar like | 2. Quitar like: ";
            std::cin >> subOpc;
            if (subOpc == 1) {
                fotos[indiceActual].darLike();
            } else if (subOpc == 2) {
                fotos[indiceActual].quitarLike();
            }
        }
    }
}

const Cadena& Album::getNombre() const {
    return nombre;
}

int Album::getPrivacidad() const {
    return privacidad;
}

int Album::getCantidadFotos() const {
    return cantidadFotos;
}