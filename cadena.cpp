#include "Cadena.h"
#include "MonitorRecursos.h"

Cadena::Cadena() {
    longitud = 0;
    datos = new char[1];
    datos[0] = '\0';
    MonitorRecursos::registrarMemoria(sizeof(char));
}

Cadena::Cadena(const char* str) {
    if (str == nullptr) {
        longitud = 0;
        datos = new char[1];
        datos[0] = '\0';
        MonitorRecursos::registrarMemoria(sizeof(char));
    } else {
        int len = 0;
        while (str[len] != '\0') {
            len++;
            MonitorRecursos::contarIteracionDirecta();
        }
        longitud = len;
        datos = new char[longitud + 1];
        MonitorRecursos::registrarMemoria((longitud + 1) * sizeof(char));

        for (int i = 0; i < longitud; i++) {
            datos[i] = str[i];
            MonitorRecursos::contarIteracionDirecta();
        }
        datos[longitud] = '\0';
    }
}

Cadena::Cadena(const Cadena& otra) {
    longitud = otra.longitud;
    datos = new char[longitud + 1];
    MonitorRecursos::registrarMemoria((longitud + 1) * sizeof(char));

    for (int i = 0; i < longitud; i++) {
        datos[i] = otra.datos[i];
        MonitorRecursos::contarIteracionDirecta();
    }
    datos[longitud] = '\0';
}

Cadena::~Cadena() {
    if (datos != nullptr) {
        MonitorRecursos::liberarMemoria((longitud + 1) * sizeof(char));
        delete[] datos;
        datos = nullptr;
    }
}

int Cadena::getLongitud() const {
    return longitud;
}

const char* Cadena::c_str() const {
    return datos;
}

void Cadena::asignar(const char* str) {
    if (datos != nullptr) {
        MonitorRecursos::liberarMemoria((longitud + 1) * sizeof(char));
        delete[] datos;
    }

    if (str == nullptr) {
        longitud = 0;
        datos = new char[1];
        datos[0] = '\0';
        MonitorRecursos::registrarMemoria(sizeof(char));
    } else {
        int len = 0;
        while (str[len] != '\0') {
            len++;
            MonitorRecursos::contarIteracionDirecta();
        }
        longitud = len;
        datos = new char[longitud + 1];
        MonitorRecursos::registrarMemoria((longitud + 1) * sizeof(char));

        for (int i = 0; i < longitud; i++) {
            datos[i] = str[i];
            MonitorRecursos::contarIteracionDirecta();
        }
        datos[longitud] = '\0';
    }
}

bool Cadena::comparar(const Cadena& otra) const {
    return (*this == otra);
}

Cadena& Cadena::operator=(const Cadena& otra) {
    if (this == &otra) {
        return *this;
    }

    if (datos != nullptr) {
        MonitorRecursos::liberarMemoria((longitud + 1) * sizeof(char));
        delete[] datos;
    }

    longitud = otra.longitud;
    datos = new char[longitud + 1];
    MonitorRecursos::registrarMemoria((longitud + 1) * sizeof(char));

    for (int i = 0; i < longitud; i++) {
        datos[i] = otra.datos[i];
        MonitorRecursos::contarIteracionDirecta();
    }
    datos[longitud] = '\0';

    return *this;
}

bool Cadena::operator==(const Cadena& otra) const {
    if (longitud != otra.longitud) {
        return false;
    }
    for (int i = 0; i < longitud; i++) {
        MonitorRecursos::contarIteracionDirecta();
        if (datos[i] != otra.datos[i]) {
            return false;
        }
    }
    return true;
}

bool Cadena::operator!=(const Cadena& otra) const {
    return !(*this == otra);
}

int Cadena::compararLexicografico(const Cadena& otra) const {
    int i = 0;
    while (i < longitud && i < otra.longitud) {
        MonitorRecursos::contarIteracionDirecta();
        if (datos[i] < otra.datos[i]) return -1;
        if (datos[i] > otra.datos[i]) return 1;
        i++;
    }
    if (longitud < otra.longitud) return -1;
    if (longitud > otra.longitud) return 1;
    return 0;
}

std::ostream& operator<<(std::ostream& os, const Cadena& c) {
    if (c.datos != nullptr) {
        os << c.datos;
    }
    return os;
}

std::istream& operator>>(std::istream& is, Cadena& c) {
    char buffer[1024];
    if (is >> buffer) {
        c.asignar(buffer);
    }
    return is;
}