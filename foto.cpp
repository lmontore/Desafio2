#include "Foto.h"
#include "MonitorRecursos.h"
#include <iostream>

Foto::Foto() {
    tamano = 0;
    cantidadLikes = 0;
}

Foto::Foto(const Cadena& fecha, long tam, const Cadena& ruta) {
    fechaCaptura = fecha;
    tamano = tam;
    rutaArchivo = ruta;
    cantidadLikes = 0;
}

Foto::Foto(const Foto& otra) {
    fechaCaptura = otra.fechaCaptura;
    tamano = otra.tamano;
    rutaArchivo = otra.rutaArchivo;
    cantidadLikes = otra.cantidadLikes;
}

Foto::~Foto() {
}

Foto& Foto::operator=(const Foto& otra) {
    if (this != &otra) {
        fechaCaptura = otra.fechaCaptura;
        tamano = otra.tamano;
        rutaArchivo = otra.rutaArchivo;
        cantidadLikes = otra.cantidadLikes;
    }
    return *this;
}

void Foto::darLike() {
    cantidadLikes++;
}

void Foto::quitarLike() {
    if (cantidadLikes > 0) {
        cantidadLikes--;
    }
}

void Foto::mostrarFoto() const {
    std::cout << "\n============================================\n";
    std::cout << "[FOTO]: " << rutaArchivo << "\n";
    std::cout << "Fecha de captura : " << fechaCaptura << "\n";
    std::cout << "Tamano de archivo: " << tamano << " KB\n";
    std::cout << "Likes            : " << cantidadLikes << "\n";
    std::cout << "============================================\n";
}

const Cadena& Foto::getRutaArchivo() const {
    return rutaArchivo;
}

int Foto::getCantidadLikes() const {
    return cantidadLikes;
}