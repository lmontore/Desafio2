#include "MonitorRecursos.h"

long MonitorRecursos::iteracionesDirectas = 0;
long MonitorRecursos::iteracionesIndirectas = 0;
long MonitorRecursos::memoriaTotal = 0;

void MonitorRecursos::reiniciar() {
    iteracionesDirectas = 0;
    iteracionesIndirectas = 0;
}

void MonitorRecursos::contarIteracionDirecta() {
    iteracionesDirectas++;
}

void MonitorRecursos::contarIteracionIndirecta() {
    iteracionesIndirectas++;
}

void MonitorRecursos::registrarMemoria(long bytes) {
    if (bytes > 0) {
        memoriaTotal += bytes;
    }
}

void MonitorRecursos::liberarMemoria(long bytes) {
    if (bytes > 0 && memoriaTotal >= bytes) {
        memoriaTotal -= bytes;
    }
}

void MonitorRecursos::mostrarReporte() {
    std::cout << "\n------------------ REPORTE DE EFICIENCIA ------------------" << std::endl;
    std::cout << "-> Iteraciones directas : " << iteracionesDirectas << std::endl;
    std::cout << "-> Iteraciones indirectas: " << iteracionesIndirectas << std::endl;
    std::cout << "-> Total iteraciones    : " << (iteracionesDirectas + iteracionesIndirectas) << std::endl;
    std::cout << "-> Memoria dinamica viva: " << memoriaTotal << " bytes (~"
              << (memoriaTotal / 1024.0) << " KB)" << std::endl;
    std::cout << "-----------------------------------------------------------\n" << std::endl;
}

long MonitorRecursos::getMemoriaTotal() {
    return memoriaTotal;
}