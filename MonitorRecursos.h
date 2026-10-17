#ifndef MONITORRECURSOS_H
#define MONITORRECURSOS_H

#include <iostream>

class MonitorRecursos
{
private:
    static long iteracionesDirectas;
    static long iteracionesIndirectas;
    static long memoriaTotal; // En bytes

public:
    static void reiniciar();
    static void contarIteracionDirecta();
    static void contarIteracionIndirecta();
    static void registrarMemoria(long bytes);
    static void liberarMemoria(long bytes);
    static void mostrarReporte();
    static long getMemoriaTotal();
};

#endif // MONITORRECURSOS_H