#ifndef REDLEON_H
#define REDLEON_H

#include "usuario.h"
#include "mensaje.h"
#include "SolicitudAmistad.h"
#include "cadena.h"
#include "MonitorRecursos.h"

class RedLeon
{
private:
    // Arreglo dinamico de usuarios
    Usuario* usuarios;
    int cantidadUsuarios;
    int capacidadUsuarios;

    // Matriz de adyacencia dinamica N x N para representar relaciones de amistad
    bool** amistades;

    // Repositorio de mensajes en memoria dinamica
    Mensaje* mensajes;
    int cantidadMensajes;
    int capacidadMensajes;

    // Repositorio de solicitudes de amistad en memoria dinamica
    SolicitudAmistad* solicitudes;
    int cantidadSolicitudes;
    int capacidadSolicitudes;

    // Metodos auxiliares internos de gestion de memoria dinamica (redimensionamiento)
    void redimensionarUsuarios();
    void redimensionarMensajes();
    void redimensionarSolicitudes();
    void inicializarMatrizAmistades(int n);
    void liberarMatrizAmistades();

public:
    RedLeon();
    ~RedLeon();

    // Consultas y busquedas rapidas
    int buscarIndiceUsuario(const Cadena& nombre) const;
    Usuario* buscarUsuario(const Cadena& nombre);
    bool agregarUsuario(const Usuario& u);

    // Operaciones sobre la red y relaciones de amistad
    bool sonAmigos(int idx1, int idx2) const;
    void establecerAmistad(int idx1, int idx2, bool estado);
    int contarAmigosEnComun(int idx1, int idx2);

    // I. Persistencia (archivos de almacenamiento permanente)
    bool cargarDatos();
    bool guardarDatos();

    // II. Autenticacion e inicio de sesion
    Usuario* iniciarSesion(const Cadena& username, const Cadena& contrasena);
    void mostrarAmigosAleatorios(Usuario* u);

    // III. Administrar amistades (enviar y aceptar/rechazar recibidas)
    void administrarAmistades(Usuario* u);

    // IV. Visitar perfil de un amigo y carrusel circular
    void visitarPerfilAmigo(Usuario* u, const Cadena& nombreAmigo);

    // V. Sugerencias de amistad rankeadas por amigos en comun
    void generarSugerencias(Usuario* u, int k);

    // VI. Administracion y consulta de mensajes
    bool enviarMensaje(Usuario* emisor, Usuario* receptor, const Cadena& texto);
    void leerNuevosMensajes(Usuario* u);
    void buscarMensajesPorFecha(Usuario* u, const Cadena& fInicio, const Cadena& fFin);
};

#endif // REDLEON_H