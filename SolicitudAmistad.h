
#ifndef SOLICITUDAMISTAD_H
#define SOLICITUDAMISTAD_H

#include "cadena.h"

class Usuario; // Declaracion adelantada para evitar dependencias circulares

class SolicitudAmistad
{
private:
    Cadena fechaSolicitud;
    int estado; // 0: Pendiente, 1: Aceptada, 2: Rechazada
    Usuario* emisor;
    Usuario* receptor;

public:
    SolicitudAmistad();
    SolicitudAmistad(const Cadena& fecha, Usuario* em, Usuario* rec);
    SolicitudAmistad(const SolicitudAmistad& otra);
    ~SolicitudAmistad();

    SolicitudAmistad& operator=(const SolicitudAmistad& otra);

    void aceptar();
    void rechazar();

    // Getters necesarios para que RedLeon/RedSocial pueda filtrar y consultar
    int getEstado() const;
    const Cadena& getFechaSolicitud() const;
    Usuario* getEmisor() const;
    Usuario* getReceptor() const;
};

#endif // SOLICITUDAMISTAD_H