#include <iostream>
#include "RedLeon.h"
#include "MonitorRecursos.h"

int main() {
    RedLeon red;

    // 1. Carga automática de datos desde almacenamiento permanente a memoria RAM[cite: 10]
    // Esta operación no forma parte del menú presentado a los usuarios[cite: 10].
    red.cargarDatos();

    std::cout << "====================================================\n";
    std::cout << "            BIENVENIDO A UDE@BOOK (2026)            \n";
    std::cout << "====================================================\n";

    // 2. Proceso de autenticación e inicio de sesión[cite: 10]
    char uInput[64];
    char pInput[64];
    std::cout << "Nombre de usuario (Demostracion: usuario1): ";
    std::cin >> uInput;
    std::cout << "Contrasena       (Demostracion: 1234): ";
    std::cin >> pInput;

    Usuario* sesion = red.iniciarSesion(Cadena(uInput), Cadena(pInput));
    if (sesion == nullptr) {
        std::cout << "\n[!] Error: Credenciales incorrectas. Finalizando aplicacion.\n";
        return 0;
    }

    bool alternadoFotos = false; // Control para permitir alternar las fotos una única vez[cite: 10]
    char opcionMenu = ' ';

    // 3. Ciclo principal del sistema con visualización según Figura 2 del enunciado[cite: 9]
    while (opcionMenu != 'S' && opcionMenu != 's') {
        // Muestra las 3 fotos de perfil de amigos ponderadas[cite: 9, 10]
        red.mostrarAmigosAleatorios(sesion);

        std::cout << "Bienvenido a UdeaBook // " << sesion->getNombreUsuario() << "\n\n";
        std::cout << "Menu de opciones:\n";
        std::cout << "1.- Administrar amistades\n";
        std::cout << "2.- Visitar perfil de un amigo.\n";
        std::cout << "3.- Ver sugerencias de amistad\n";
        std::cout << "4.- Enviar mensaje a un amigo\n";
        std::cout << "5.- Leer nuevos mensajes recibidos\n";
        std::cout << "6.- Buscar mensajes por rango de fecha\n";
        std::cout << ".- Ingrese 'N' o 'n' para alternar el grupo de fotos de perfil de amigos.\n";
        std::cout << ".- Ingrese 'S' o 's' para cerrar su sesion.\n";
        std::cout << "\nSu opcion: ";
        std::cin >> opcionMenu;

        if (opcionMenu == 'N' || opcionMenu == 'n') {
            if (!alternadoFotos) {
                alternadoFotos = true;
                std::cout << "\n[INFO] Set de fotos de perfil alternado (cambio unico permitido):\n";
                red.mostrarAmigosAleatorios(sesion);
            } else {
                std::cout << "\n[AVISO] Ya ha utilizado su unica oportunidad para alternar fotos de perfil.\n";
            }
        }
        else if (opcionMenu == '1') {
            red.administrarAmistades(sesion);
        }
        else if (opcionMenu == '2') {
            char amigoNom[64];
            std::cout << "Ingrese el nombre de usuario del amigo (Ejemplo demostracion: usuario2): ";
            std::cin >> amigoNom;
            red.visitarPerfilAmigo(sesion, Cadena(amigoNom));
        }
        else if (opcionMenu == '3') {
            int k = 0;
            std::cout << "Ingrese cantidad de sugerencias a consultar K (1 a 20, Ej: 12): ";
            std::cin >> k;
            red.generarSugerencias(sesion, k);
        }
        else if (opcionMenu == '4') {
            char destNom[64];
            char textoMsg[512];
            std::cout << "Destinatario amigo: ";
            std::cin >> destNom;
            std::cin.ignore(); // Limpiar el buffer de entrada antes de leer la línea completa
            std::cout << "Mensaje (maximo 512 caracteres): ";
            std::cin.getline(textoMsg, 512);

            Usuario* destU = red.buscarUsuario(Cadena(destNom));
            int idxU = red.buscarIndiceUsuario(sesion->getNombreUsuario());
            int idxD = red.buscarIndiceUsuario(Cadena(destNom));

            if (destU != nullptr && red.sonAmigos(idxU, idxD)) {
                red.enviarMensaje(sesion, destU, Cadena(textoMsg));
                std::cout << "Mensaje enviado exitosamente.\n";
            } else {
                std::cout << "Error: Solo puede enviar mensajes a usuarios con quienes posea amistad mutua.\n";
            }
        }
        else if (opcionMenu == '5') {
            red.leerNuevosMensajes(sesion);
        }
        else if (opcionMenu == '6') {
            char fInicio[32];
            char fFin[32];
            std::cout << "Fecha de inicio (AAAA-MM-DD, Ej: 2026-08-01): ";
            std::cin >> fInicio;
            std::cout << "Fecha de fin    (AAAA-MM-DD, Ej: 2026-10-15): ";
            std::cin >> fFin;
            red.buscarMensajesPorFecha(sesion, Cadena(fInicio), Cadena(fFin));
        }
        else if (opcionMenu != 'S' && opcionMenu != 's') {
            std::cout << "\n[!] Opcion no valida. Seleccione una opcion del menu.\n";
        }
    }

    // 4. Sincronización y persistencia al cerrar la sesión[cite: 10]
    red.guardarDatos();
    std::cout << "\nSesion cerrada exitosamente. Almacenamiento actualizado.\n";

    return 0;
}