#include "mensaje.h"
#include "usuario.h"
#include <iostream>

Mensaje::Mensaje() {
    leido = false;
    emisor = nullptr;
    receptor = nullptr;
}

Mensaje::Mensaje(const Cadena& cont, const Cadena& fecha, Usuario* em, Usuario* rec) {
    contenido = cont;
    fechaEnvio = fecha;
    leido = false;
    emisor = em;
    receptor = rec;
}

Mensaje::Mensaje(const Mensaje& otro) {
    contenido = otro.contenido;
    fechaEnvio = otro.fechaEnvio;
    leido = otro.leido;
    emisor = otro.emisor;
    receptor = otro.receptor;
}

Mensaje::~Mensaje() {
    // emisor y receptor son punteros administrados por el controlador principal
}

Mensaje& Mensaje::operator=(const Mensaje& otro) {
    if (this != &otro) {
        contenido = otro.contenido;
        fechaEnvio = otro.fechaEnvio;
        leido = otro.leido;
        emisor = otro.emisor;
        receptor = otro.receptor;
    }
    return *this;
}

void Mensaje::marcarLeido() {
    leido = true;
}

void Mensaje::mostrarMensaje() const {
    std::cout << "\n--------------------------------------------------\n";
    std::cout << "De     : " << (emisor != nullptr ? emisor->getNombreUsuario().c_str() : "Desconocido") << "\n";
    std::cout << "Para   : " << (receptor != nullptr ? receptor->getNombreUsuario().c_str() : "Desconocido") << "\n";
    std::cout << "Fecha  : " << fechaEnvio << "\n";
    std::cout << "Estado : " << (leido ? "Leido" : "No leido") << "\n";
    std::cout << "Mensaje: " << contenido << "\n";
    std::cout << "--------------------------------------------------\n";
}

bool Mensaje::esLeido() const {
    return leido;
}

const Cadena& Mensaje::getFechaEnvio() const {
    return fechaEnvio;
}

const Cadena& Mensaje::getContenido() const {
    return contenido;
}

Usuario* Mensaje::getEmisor() const {
    return emisor;
}

Usuario* Mensaje::getReceptor() const {
    return receptor;
}