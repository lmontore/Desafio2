#include "RedLeon.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

// =========================================================================
// Métodos auxiliares privados para gestión de memoria dinámica
// =========================================================================

void RedLeon::redimensionarUsuarios() {
    int nuevaCap = (capacidadUsuarios == 0) ? 16 : capacidadUsuarios * 2;
    Usuario* nuevoArr = new Usuario[nuevaCap];
    MonitorRecursos::registrarMemoria(nuevaCap * sizeof(Usuario));

    for (int i = 0; i < cantidadUsuarios; i++) {
        nuevoArr[i] = usuarios[i];
        MonitorRecursos::contarIteracionDirecta();
    }

    if (usuarios != nullptr) {
        MonitorRecursos::liberarMemoria(capacidadUsuarios * sizeof(Usuario));
        delete[] usuarios;
    }

    usuarios = nuevoArr;
    capacidadUsuarios = nuevaCap;
}

void RedLeon::redimensionarMensajes() {
    int nuevaCap = (capacidadMensajes == 0) ? 32 : capacidadMensajes * 2;
    Mensaje* nuevoArr = new Mensaje[nuevaCap];
    MonitorRecursos::registrarMemoria(nuevaCap * sizeof(Mensaje));

    for (int i = 0; i < cantidadMensajes; i++) {
        nuevoArr[i] = mensajes[i];
        MonitorRecursos::contarIteracionDirecta();
    }

    if (mensajes != nullptr) {
        MonitorRecursos::liberarMemoria(capacidadMensajes * sizeof(Mensaje));
        delete[] mensajes;
    }

    mensajes = nuevoArr;
    capacidadMensajes = nuevaCap;
}

void RedLeon::redimensionarSolicitudes() {
    int nuevaCap = (capacidadSolicitudes == 0) ? 32 : capacidadSolicitudes * 2;
    SolicitudAmistad* nuevoArr = new SolicitudAmistad[nuevaCap];
    MonitorRecursos::registrarMemoria(nuevaCap * sizeof(SolicitudAmistad));

    for (int i = 0; i < cantidadSolicitudes; i++) {
        nuevoArr[i] = solicitudes[i];
        MonitorRecursos::contarIteracionDirecta();
    }

    if (solicitudes != nullptr) {
        MonitorRecursos::liberarMemoria(capacidadSolicitudes * sizeof(SolicitudAmistad));
        delete[] solicitudes;
    }

    solicitudes = nuevoArr;
    capacidadSolicitudes = nuevaCap;
}

void RedLeon::inicializarMatrizAmistades(int n) {
    liberarMatrizAmistades();
    amistades = new bool*[n];
    MonitorRecursos::registrarMemoria(n * sizeof(bool*));

    for (int i = 0; i < n; i++) {
        amistades[i] = new bool[n];
        MonitorRecursos::registrarMemoria(n * sizeof(bool));
        for (int j = 0; j < n; j++) {
            amistades[i][j] = false;
            MonitorRecursos::contarIteracionDirecta();
        }
    }
}

void RedLeon::liberarMatrizAmistades() {
    if (amistades != nullptr) {
        for (int i = 0; i < cantidadUsuarios; i++) {
            if (amistades[i] != nullptr) {
                MonitorRecursos::liberarMemoria(cantidadUsuarios * sizeof(bool));
                delete[] amistades[i];
            }
        }
        MonitorRecursos::liberarMemoria(cantidadUsuarios * sizeof(bool*));
        delete[] amistades;
        amistades = nullptr;
    }
}

// =========================================================================
// Constructor y Destructor
// =========================================================================

RedLeon::RedLeon() {
    usuarios = nullptr;
    cantidadUsuarios = 0;
    capacidadUsuarios = 0;

    amistades = nullptr;

    mensajes = nullptr;
    cantidadMensajes = 0;
    capacidadMensajes = 0;

    solicitudes = nullptr;
    cantidadSolicitudes = 0;
    capacidadSolicitudes = 0;

    srand(time(nullptr));
}

RedLeon::~RedLeon() {
    if (usuarios != nullptr) {
        MonitorRecursos::liberarMemoria(capacidadUsuarios * sizeof(Usuario));
        delete[] usuarios;
        usuarios = nullptr;
    }
    liberarMatrizAmistades();
    if (mensajes != nullptr) {
        MonitorRecursos::liberarMemoria(capacidadMensajes * sizeof(Mensaje));
        delete[] mensajes;
        mensajes = nullptr;
    }
    if (solicitudes != nullptr) {
        MonitorRecursos::liberarMemoria(capacidadSolicitudes * sizeof(SolicitudAmistad));
        delete[] solicitudes;
        solicitudes = nullptr;
    }
}

// =========================================================================
// Búsquedas y operaciones básicas
// =========================================================================

int RedLeon::buscarIndiceUsuario(const Cadena& nombre) const {
    for (int i = 0; i < cantidadUsuarios; i++) {
        MonitorRecursos::contarIteracionDirecta();
        if (usuarios[i].getNombreUsuario() == nombre) {
            return i;
        }
    }
    return -1;
}

Usuario* RedLeon::buscarUsuario(const Cadena& nombre) {
    int idx = buscarIndiceUsuario(nombre);
    if (idx != -1) {
        return &usuarios[idx];
    }
    return nullptr;
}

bool RedLeon::agregarUsuario(const Usuario& u) {
    if (cantidadUsuarios >= capacidadUsuarios) {
        redimensionarUsuarios();
    }
    usuarios[cantidadUsuarios] = u;
    cantidadUsuarios++;
    return true;
}

// =========================================================================
// Relaciones de Amistad (Grafo / Matriz de adyacencia)
// =========================================================================

bool RedLeon::sonAmigos(int idx1, int idx2) const {
    if (amistades == nullptr) return false;
    if (idx1 >= 0 && idx1 < cantidadUsuarios && idx2 >= 0 && idx2 < cantidadUsuarios) {
        return amistades[idx1][idx2];
    }
    return false;
}

void RedLeon::establecerAmistad(int idx1, int idx2, bool estado) {
    if (amistades != nullptr && idx1 >= 0 && idx1 < cantidadUsuarios && idx2 >= 0 && idx2 < cantidadUsuarios) {
        amistades[idx1][idx2] = estado;
        amistades[idx2][idx1] = estado;
    }
}

int RedLeon::contarAmigosEnComun(int idx1, int idx2) {
    if (amistades == nullptr || idx1 == idx2) return 0;
    int enComun = 0;
    for (int i = 0; i < cantidadUsuarios; i++) {
        MonitorRecursos::contarIteracionDirecta();
        if (amistades[idx1][i] && amistades[idx2][i]) {
            enComun++;
        }
    }
    return enComun;
}

// =========================================================================
// I. Persistencia de datos y Generador de prueba
// =========================================================================

static void generarArchivosPruebaSiNoExisten() {
    std::ifstream test("usuarios.txt");
    if (test.good()) {
        test.close();
        return;
    }
    test.close();

    std::cout << "[CONFIG] Generando archivos planos para 1005 usuarios de prueba...\n";

    std::ofstream fUsers("usuarios.txt");
    for (int i = 1; i <= 1005; i++) {
        fUsers << "usuario" << i << ";"
               << "2000-01-15;"
               << "usuario" << i << "@udea.edu.co;"
               << "/users/udea_storage/usuario" << i << "/albums/profile.png;"
               << "1234;"
               << "Medellin;Colombia;2026-02-01\n";
    }
    fUsers.close();

    std::ofstream fAmis("amistades.txt");
    for (int i = 2; i <= 26; i++) {
        fAmis << "usuario1;usuario" << i << "\n";
    }
    for (int i = 2; i <= 16; i++) {
        fAmis << "usuario27;usuario" << i << "\n";
    }
    for (int i = 2; i <= 9; i++) {
        fAmis << "usuario28;usuario" << i << "\n";
    }
    fAmis.close();

    std::ofstream fAlb("albumes_fotos.txt");
    const char* fotosAlb1[5] = {"Malecon1", "Malecon2", "Playa1", "Playa2", "Bocagrande"};
    for (int j = 0; j < 5; j++) {
        fAlb << "usuario2;Cartagena2026;1;2026-06-0" << (j + 1) << ";1500;"
             << "/users/udea_storage/usuario2/albums/Cartagena2026/" << fotosAlb1[j] << ".png;5\n";
    }
    const char* fotosAlb2[5] = {"Bloque19", "Plazoleta", "Biblioteca", "Fuente", "Ingenieria"};
    for (int j = 0; j < 5; j++) {
        fAlb << "usuario2;CampusUdeA;2;2026-08-1" << j << ";2048;"
             << "/users/udea_storage/usuario2/albums/CampusUdeA/" << fotosAlb2[j] << ".png;10\n";
    }
    fAlb.close();

    std::ofstream fMsg("mensajes.txt");
    fMsg << "usuario2;usuario1;2026-08-20 10:15;0;Hola! Como va el avance del proyecto Ude@Book?\n";
    fMsg << "usuario3;usuario1;2026-09-12 14:00;0;Revisa los diagramas UML cuando puedas.\n";
    fMsg << "usuario2;usuario1;2026-10-14 18:30;0;Nos vemos en el bloque 19 para estudiar.\n";
    fMsg.close();

    std::ofstream fSol("solicitudes.txt");
    fSol << "usuario30;usuario1;2026-10-12;0\n";
    fSol << "usuario31;usuario1;2026-10-13;0\n";
    fSol.close();

    std::cout << "[CONFIG] Archivos iniciales listos.\n";
}

bool RedLeon::cargarDatos() {
    generarArchivosPruebaSiNoExisten();

    MonitorRecursos::reiniciar();
    std::cout << "Cargando datos desde archivos planos a memoria RAM...\n";

    std::ifstream fUsers("usuarios.txt");
    if (!fUsers.is_open()) return false;

    char uNom[64], uNac[32], uMail[64], uFoto[256], uPass[64], uCiu[64], uPai[64], uInsc[32];
    while (fUsers.getline(uNom, 64, ';')) {
        fUsers.getline(uNac, 32, ';');
        fUsers.getline(uMail, 64, ';');
        fUsers.getline(uFoto, 256, ';');
        fUsers.getline(uPass, 64, ';');
        fUsers.getline(uCiu, 64, ';');
        fUsers.getline(uPai, 64, ';');
        fUsers.getline(uInsc, 32, '\n');

        Cadena cadNom(uNom);
        Cadena cadNac(uNac);
        Cadena cadMail(uMail);
        Cadena cadFoto(uFoto);
        Cadena cadPass(uPass);
        Cadena cadCiu(uCiu);
        Cadena cadPai(uPai);
        Cadena cadInsc(uInsc);

        Usuario u(cadNom, cadNac, cadMail, cadFoto, cadPass, cadCiu, cadPai, cadInsc);
        agregarUsuario(u);
        MonitorRecursos::contarIteracionDirecta();
    }
    fUsers.close();

    inicializarMatrizAmistades(cantidadUsuarios);

    std::ifstream fAmis("amistades.txt");
    if (fAmis.is_open()) {
        char u1[64], u2[64];
        while (fAmis.getline(u1, 64, ';')) {
            fAmis.getline(u2, 64, '\n');
            int idx1 = buscarIndiceUsuario(Cadena(u1));
            int idx2 = buscarIndiceUsuario(Cadena(u2));
            if (idx1 != -1 && idx2 != -1) {
                establecerAmistad(idx1, idx2, true);
            }
            MonitorRecursos::contarIteracionDirecta();
        }
        fAmis.close();
    }

    std::ifstream fAlb("albumes_fotos.txt");
    if (fAlb.is_open()) {
        char nomU[64], nomA[64], privStr[8], fCap[32], tamStr[16], rFoto[256], lkStr[16];
        while (fAlb.getline(nomU, 64, ';')) {
            fAlb.getline(nomA, 64, ';');
            fAlb.getline(privStr, 8, ';');
            fAlb.getline(fCap, 32, ';');
            fAlb.getline(tamStr, 16, ';');
            fAlb.getline(rFoto, 256, ';');
            fAlb.getline(lkStr, 16, '\n');

            Usuario* user = buscarUsuario(Cadena(nomU));
            if (user != nullptr) {
                Album* targetAlbum = nullptr;
                for (int a = 0; a < user->getCantidadAlbumes(); a++) {
                    if (user->getAlbum(a)->getNombre() == Cadena(nomA)) {
                        targetAlbum = user->getAlbum(a);
                        break;
                    }
                }

                if (targetAlbum == nullptr) {
                    int priv = atoi(privStr);
                    targetAlbum = new Album(Cadena(nomA), Cadena(rFoto), priv);
                    user->agregarAlbum(targetAlbum);
                }

                long tam = atol(tamStr);
                Foto f(Cadena(fCap), tam, Cadena(rFoto));
                targetAlbum->agregarFoto(f);
            }
            MonitorRecursos::contarIteracionDirecta();
        }
        fAlb.close();
    }

    std::ifstream fMsg("mensajes.txt");
    if (fMsg.is_open()) {
        char em[64], rec[64], fEnv[32], leiStr[8], cont[512];
        while (fMsg.getline(em, 64, ';')) {
            fMsg.getline(rec, 64, ';');
            fMsg.getline(fEnv, 32, ';');
            fMsg.getline(leiStr, 8, ';');
            fMsg.getline(cont, 512, '\n');

            Usuario* emUser = buscarUsuario(Cadena(em));
            Usuario* recUser = buscarUsuario(Cadena(rec));
            if (emUser != nullptr && recUser != nullptr) {
                if (cantidadMensajes >= capacidadMensajes) redimensionarMensajes();
                Mensaje m(Cadena(cont), Cadena(fEnv), emUser, recUser);
                if (leiStr[0] == '1') m.marcarLeido();
                mensajes[cantidadMensajes++] = m;
            }
            MonitorRecursos::contarIteracionDirecta();
        }
        fMsg.close();
    }

    std::ifstream fSol("solicitudes.txt");
    if (fSol.is_open()) {
        char em[64], rec[64], fSolStr[32], estStr[8];
        while (fSol.getline(em, 64, ';')) {
            fSol.getline(rec, 64, ';');
            fSol.getline(fSolStr, 32, ';');
            fSol.getline(estStr, 8, '\n');

            Usuario* emUser = buscarUsuario(Cadena(em));
            Usuario* recUser = buscarUsuario(Cadena(rec));
            if (emUser != nullptr && recUser != nullptr) {
                if (cantidadSolicitudes >= capacidadSolicitudes) redimensionarSolicitudes();
                SolicitudAmistad s(Cadena(fSolStr), emUser, recUser);
                if (estStr[0] == '1') s.aceptar();
                else if (estStr[0] == '2') s.rechazar();
                solicitudes[cantidadSolicitudes++] = s;
            }
            MonitorRecursos::contarIteracionDirecta();
        }
        fSol.close();
    }

    std::cout << "[OK] Carga finalizada: " << cantidadUsuarios << " usuarios en el sistema.\n";
    MonitorRecursos::mostrarReporte();
    return true;
}

bool RedLeon::guardarDatos() {
    MonitorRecursos::reiniciar();
    std::cout << "Guardando datos actualizados en almacenamiento permanente...\n";

    std::ofstream fAmis("amistades.txt");
    for (int i = 0; i < cantidadUsuarios; i++) {
        for (int j = i + 1; j < cantidadUsuarios; j++) {
            MonitorRecursos::contarIteracionDirecta();
            if (amistades[i][j]) {
                fAmis << usuarios[i].getNombreUsuario() << ";"
                      << usuarios[j].getNombreUsuario() << "\n";
            }
        }
    }
    fAmis.close();

    std::ofstream fSol("solicitudes.txt");
    for (int i = 0; i < cantidadSolicitudes; i++) {
        MonitorRecursos::contarIteracionDirecta();
        fSol << solicitudes[i].getEmisor()->getNombreUsuario() << ";"
             << solicitudes[i].getReceptor()->getNombreUsuario() << ";"
             << solicitudes[i].getFechaSolicitud() << ";"
             << solicitudes[i].getEstado() << "\n";
    }
    fSol.close();

    std::ofstream fMsg("mensajes.txt");
    for (int i = 0; i < cantidadMensajes; i++) {
        MonitorRecursos::contarIteracionDirecta();
        fMsg << mensajes[i].getEmisor()->getNombreUsuario() << ";"
             << mensajes[i].getReceptor()->getNombreUsuario() << ";"
             << mensajes[i].getFechaEnvio() << ";"
             << (mensajes[i].esLeido() ? 1 : 0) << ";"
             << mensajes[i].getContenido() << "\n";
    }
    fMsg.close();

    std::cout << "[OK] Almacenamiento sincronizado.\n";
    MonitorRecursos::mostrarReporte();
    return true;
}

// =========================================================================
// II. Autenticación y Visualización de Fotos de Amigos
// =========================================================================

Usuario* RedLeon::iniciarSesion(const Cadena& username, const Cadena& contrasena) {
    MonitorRecursos::reiniciar();
    for (int i = 0; i < cantidadUsuarios; i++) {
        MonitorRecursos::contarIteracionDirecta();
        if (usuarios[i].getNombreUsuario() == username && usuarios[i].getContrasena() == contrasena) {
            MonitorRecursos::mostrarReporte();
            return &usuarios[i];
        }
    }
    MonitorRecursos::mostrarReporte();
    return nullptr;
}

void RedLeon::mostrarAmigosAleatorios(Usuario* u) {
    MonitorRecursos::reiniciar();
    int idxU = buscarIndiceUsuario(u->getNombreUsuario());
    if (idxU == -1) return;

    int numAmigos = 0;
    for (int i = 0; i < cantidadUsuarios; i++) {
        MonitorRecursos::contarIteracionDirecta();
        if (amistades[idxU][i]) numAmigos++;
    }

    if (numAmigos == 0) {
        std::cout << "\nNo tienes amigos agregados aun para mostrar en portada.\n";
        MonitorRecursos::mostrarReporte();
        return;
    }

    int* indicesAmigos = new int[numAmigos];
    float* pesos = new float[numAmigos];
    int pos = 0;

    for (int i = 0; i < cantidadUsuarios; i++) {
        MonitorRecursos::contarIteracionDirecta();
        if (amistades[idxU][i]) {
            indicesAmigos[pos] = i;
            int enComun = contarAmigosEnComun(idxU, i);
            if (enComun <= 10) {
                pesos[pos] = 1.0f;
            } else if (enComun <= 20) {
                pesos[pos] = 2.5f;
            } else {
                pesos[pos] = 3.5f;
            }
            pos++;
        }
    }

    int aMostrar = (numAmigos < 3) ? numAmigos : 3;
    bool* seleccionados = new bool[numAmigos];
    for (int i = 0; i < numAmigos; i++) seleccionados[i] = false;

    std::cout << "\n";
    for (int k = 0; k < aMostrar; k++) {
        float sumaPesos = 0;
        for (int i = 0; i < numAmigos; i++) {
            if (!seleccionados[i]) sumaPesos += pesos[i];
        }

        float r = ((float)rand() / RAND_MAX) * sumaPesos;
        float acumulado = 0;
        int elegido = -1;

        for (int i = 0; i < numAmigos; i++) {
            if (!seleccionados[i]) {
                acumulado += pesos[i];
                if (r <= acumulado) {
                    elegido = i;
                    break;
                }
            }
        }

        if (elegido == -1) {
            for (int i = 0; i < numAmigos; i++) {
                if (!seleccionados[i]) { elegido = i; break; }
            }
        }

        seleccionados[elegido] = true;
        Usuario* amigo = &usuarios[indicesAmigos[elegido]];
        std::cout << "Foto perfil de amigo " << (k + 1) << ": " << amigo->getFotoPerfil() << "\n";
    }

    delete[] indicesAmigos;
    delete[] pesos;
    delete[] seleccionados;
    MonitorRecursos::mostrarReporte();
}

// =========================================================================
// III. Administrar Amistades
// =========================================================================

void RedLeon::administrarAmistades(Usuario* u) {
    MonitorRecursos::reiniciar();
    int idxU = buscarIndiceUsuario(u->getNombreUsuario());
    int opc = 0;

    std::cout << "\n--- ADMINISTRAR AMISTADES ---\n";
    std::cout << "1. Enviar solicitud de amistad\n";
    std::cout << "2. Ver y responder solicitudes recibidas\n";
    std::cout << "Seleccione: ";
    std::cin >> opc;

    if (opc == 1) {
        char destinoStr[128];
        std::cout << "Ingrese el nombre de usuario a agregar: ";
        std::cin >> destinoStr;
        Cadena dest(destinoStr);

        int idxDest = buscarIndiceUsuario(dest);
        if (idxDest == -1 || idxDest == idxU) {
            std::cout << "Usuario no valido o inexistente.\n";
        } else if (sonAmigos(idxU, idxDest)) {
            std::cout << "Ya son amigos en la plataforma.\n";
        } else {
            bool pendiente = false;
            for (int i = 0; i < cantidadSolicitudes; i++) {
                MonitorRecursos::contarIteracionDirecta();
                if (solicitudes[i].getEstado() == 0) {
                    if ((solicitudes[i].getEmisor() == u && solicitudes[i].getReceptor() == &usuarios[idxDest]) ||
                        (solicitudes[i].getEmisor() == &usuarios[idxDest] && solicitudes[i].getReceptor() == u)) {
                        pendiente = true;
                        break;
                    }
                }
            }

            if (pendiente) {
                std::cout << "Ya existe una solicitud pendiente entre ambos.\n";
            } else {
                if (cantidadSolicitudes >= capacidadSolicitudes) redimensionarSolicitudes();
                Cadena fechaActual("2026-10-15");
                solicitudes[cantidadSolicitudes] = SolicitudAmistad(fechaActual, u, &usuarios[idxDest]);
                cantidadSolicitudes++;
                std::cout << "Solicitud de amistad enviada con exito.\n";
            }
        }
    } else if (opc == 2) {
        std::cout << "\n--- SOLICITUDES PENDIENTES RECIBIDAS ---\n";
        int recibidas[100];
        int totalRec = 0;

        for (int i = 0; i < cantidadSolicitudes; i++) {
            MonitorRecursos::contarIteracionDirecta();
            if (solicitudes[i].getReceptor() == u && solicitudes[i].getEstado() == 0) {
                recibidas[totalRec] = i;
                std::cout << (totalRec + 1) << ". De: "
                          << solicitudes[i].getEmisor()->getNombreUsuario()
                          << " (Fecha: " << solicitudes[i].getFechaSolicitud() << ")\n";
                totalRec++;
            }
        }

        if (totalRec == 0) {
            std::cout << "No tienes solicitudes de amistad pendientes.\n";
        } else {
            int numSol = 0, decision = 0;
            std::cout << "Seleccione el numero de solicitud a gestionar (0 para salir): ";
            std::cin >> numSol;
            if (numSol >= 1 && numSol <= totalRec) {
                std::cout << "1. Aceptar | 2. Rechazar: ";
                std::cin >> decision;
                int idxReal = recibidas[numSol - 1];
                if (decision == 1) {
                    solicitudes[idxReal].aceptar();
                    int idxAmigo = buscarIndiceUsuario(solicitudes[idxReal].getEmisor()->getNombreUsuario());
                    establecerAmistad(idxU, idxAmigo, true);
                    std::cout << "Solicitud aceptada. Ahora son amigos.\n";
                } else if (decision == 2) {
                    solicitudes[idxReal].rechazar();
                    std::cout << "Solicitud rechazada.\n";
                }
            }
        }
    }
    MonitorRecursos::mostrarReporte();
}

// =========================================================================
// IV. Visitar Perfil de un Amigo y Carrusel Circular
// =========================================================================

void RedLeon::visitarPerfilAmigo(Usuario* u, const Cadena& nombreAmigo) {
    MonitorRecursos::reiniciar();
    int idxU = buscarIndiceUsuario(u->getNombreUsuario());
    int idxAmigo = buscarIndiceUsuario(nombreAmigo);

    if (idxAmigo == -1) {
        std::cout << "Usuario no encontrado.\n";
        MonitorRecursos::mostrarReporte();
        return;
    }

    bool amigos = sonAmigos(idxU, idxAmigo);
    if (!amigos && idxU != idxAmigo) {
        std::cout << "Solo puedes visitar perfiles de usuarios con los que tengas amistad mutua.\n";
        MonitorRecursos::mostrarReporte();
        return;
    }

    Usuario* amigo = &usuarios[idxAmigo];
    amigo->mostrarPerfil();

    std::cout << "\n--- ALBUMES DISPONIBLES ---\n";
    int albumesVisibles[50];
    int totalVis = 0;

    for (int i = 0; i < amigo->getCantidadAlbumes(); i++) {
        MonitorRecursos::contarIteracionDirecta();
        Album* alb = amigo->getAlbum(i);
        if (alb != nullptr && alb->puedeVisualizarse(amigos, idxU == idxAmigo)) {
            albumesVisibles[totalVis] = i;
            std::cout << (totalVis + 1) << ". " << alb->getNombre()
                      << " (" << alb->getCantidadFotos() << " fotos)\n";
            totalVis++;
        }
    }

    if (totalVis == 0) {
        std::cout << "No hay albumes visibles para tu cuenta.\n";
    } else {
        int sel = 0;
        std::cout << "Seleccione el album a explorar (0 para volver): ";
        std::cin >> sel;
        if (sel >= 1 && sel <= totalVis) {
            Album* albSeleccionado = amigo->getAlbum(albumesVisibles[sel - 1]);
            albSeleccionado->mostrarCarrusel();
        }
    }
    MonitorRecursos::mostrarReporte();
}

// =========================================================================
// V. Sugerencias de Amistad
// =========================================================================

void RedLeon::generarSugerencias(Usuario* u, int k) {
    MonitorRecursos::reiniciar();
    int idxU = buscarIndiceUsuario(u->getNombreUsuario());
    if (idxU == -1) return;

    if (k < 1) k = 1;
    if (k > 20) k = 20;

    int* candidatos = new int[cantidadUsuarios];
    int* coincidencias = new int[cantidadUsuarios];
    int totalCandidatos = 0;

    for (int i = 0; i < cantidadUsuarios; i++) {
        MonitorRecursos::contarIteracionDirecta();
        if (i != idxU && !sonAmigos(idxU, i)) {
            int comun = contarAmigosEnComun(idxU, i);
            if (comun > 0) {
                candidatos[totalCandidatos] = i;
                coincidencias[totalCandidatos] = comun;
                totalCandidatos++;
            }
        }
    }

    for (int i = 0; i < totalCandidatos - 1; i++) {
        for (int j = 0; j < totalCandidatos - i - 1; j++) {
            MonitorRecursos::contarIteracionDirecta();
            if (coincidencias[j] < coincidencias[j + 1]) {
                int tempC = coincidencias[j];
                coincidencias[j] = coincidencias[j + 1];
                coincidencias[j + 1] = tempC;

                int tempU = candidatos[j];
                candidatos[j] = candidatos[j + 1];
                candidatos[j + 1] = tempU;
            }
        }
    }

    int lim = (totalCandidatos < k) ? totalCandidatos : k;
    std::cout << "\n--- TOP " << lim << " SUGERENCIAS DE AMISTAD ---\n";
    for (int i = 0; i < lim; i++) {
        std::cout << (i + 1) << ". " << usuarios[candidatos[i]].getNombreUsuario()
        << " (" << coincidencias[i] << " amigos en comun)\n";
    }

    if (lim > 0) {
        int sel = 0;
        std::cout << "¿Desea enviar solicitud a alguno? (1 a " << lim << ", 0 para volver): ";
        std::cin >> sel;
        if (sel >= 1 && sel <= lim) {
            int idxDest = candidatos[sel - 1];
            if (cantidadSolicitudes >= capacidadSolicitudes) redimensionarSolicitudes();
            Cadena fechaActual("2026-10-15");
            solicitudes[cantidadSolicitudes] = SolicitudAmistad(fechaActual, u, &usuarios[idxDest]);
            cantidadSolicitudes++;
            std::cout << "Solicitud enviada a " << usuarios[idxDest].getNombreUsuario() << ".\n";
        }
    } else {
        std::cout << "No hay sugerencias con amigos en comun disponibles.\n";
    }

    delete[] candidatos;
    delete[] coincidencias;
    MonitorRecursos::mostrarReporte();
}

// =========================================================================
// VI. Administración de Mensajes
// =========================================================================

bool RedLeon::enviarMensaje(Usuario* emisor, Usuario* receptor, const Cadena& texto) {
    if (emisor == nullptr || receptor == nullptr) return false;
    if (texto.getLongitud() == 0 || texto.getLongitud() > 512) {
        std::cout << "Error: El mensaje no puede estar vacio ni superar los 512 caracteres.\n";
        return false;
    }

    if (cantidadMensajes >= capacidadMensajes) {
        redimensionarMensajes();
    }

    Cadena fechaHoy("2026-10-15 15:30");
    mensajes[cantidadMensajes] = Mensaje(texto, fechaHoy, emisor, receptor);
    cantidadMensajes++;
    return true;
}

void RedLeon::leerNuevosMensajes(Usuario* u) {
    MonitorRecursos::reiniciar();
    std::cout << "\n--- BANDEJA DE MENSAJES NUEVOS (NO LEIDOS) ---\n";
    int leidosCount = 0;

    for (int i = 0; i < cantidadMensajes; i++) {
        MonitorRecursos::contarIteracionDirecta();
        if (mensajes[i].getReceptor() == u && !mensajes[i].esLeido()) {
            mensajes[i].mostrarMensaje();
            mensajes[i].marcarLeido();
            leidosCount++;
        }
    }

    if (leidosCount == 0) {
        std::cout << "No tienes nuevos mensajes.\n";
    } else {
        std::cout << "Se han marcado " << leidosCount << " mensaje(s) como leidos.\n";
    }
    MonitorRecursos::mostrarReporte();
}

void RedLeon::buscarMensajesPorFecha(Usuario* u, const Cadena& fInicio, const Cadena& fFin) {
    MonitorRecursos::reiniciar();
    std::cout << "\n--- BUSQUEDA DE MENSAJES RECIBIDOS (" << fInicio << " a " << fFin << ") ---\n";
    int encontrados = 0;

    for (int i = 0; i < cantidadMensajes; i++) {
        MonitorRecursos::contarIteracionDirecta();
        if (mensajes[i].getReceptor() == u) {
            if (mensajes[i].getFechaEnvio().compararLexicografico(fInicio) >= 0 &&
                mensajes[i].getFechaEnvio().compararLexicografico(fFin) <= 0) {
                mensajes[i].mostrarMensaje();
                encontrados++;
            }
        }
    }

    if (encontrados == 0) {
        std::cout << "No se encontraron mensajes en ese rango de fechas.\n";
    }
    MonitorRecursos::mostrarReporte();
}