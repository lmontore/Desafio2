#include "SolicitudAmistad.h"
#include <iostream>

SolicitudAmistad::SolicitudAmistad() {
    estado = 0; // Por defecto pendiente
    emisor = nullptr;
    receptor = nullptr;
}

SolicitudAmistad::SolicitudAmistad(const Cadena& fecha, Usuario* em, Usuario* rec) {
    fechaSolicitud = fecha;
    estado = 0; // Al crearse inicia en pendiente
    emisor = em;
    receptor = rec;
}

SolicitudAmistad::SolicitudAmistad(const SolicitudAmistad& otra) {
    fechaSolicitud = otra.fechaSolicitud;
    estado = otra.estado;
    emisor = otra.emisor;
    receptor = otra.receptor;
}

SolicitudAmistad::~SolicitudAmistad() {
    // emisor y receptor son administrados por el contenedor principal (RedLeon)
}

SolicitudAmistad& SolicitudAmistad::operator=(const SolicitudAmistad& otra) {
    if (this != &otra) {
        fechaSolicitud = otra.fechaSolicitud;
        estado = otra.estado;
        emisor = otra.emisor;
        receptor = otra.receptor;
    }
    return *this;
}

void SolicitudAmistad::aceptar() {
    estado = 1; // 1 = Aceptada
}

void SolicitudAmistad::rechazar() {
    estado = 2; // 2 = Rechazada
}

int SolicitudAmistad::getEstado() const {
    return estado;
}

const Cadena& SolicitudAmistad::getFechaSolicitud() const {
    return fechaSolicitud;
}

Usuario* SolicitudAmistad::getEmisor() const {
    return emisor;
}

Usuario* SolicitudAmistad::getReceptor() const {
    return receptor;
}