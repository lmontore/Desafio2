cmake_minimum_required(VERSION 3.16)

project(UdeBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(UdeBook main.cpp
    usuario.h
    usuario.cpp
    album.h
    album.cpp
    foto.h
    foto.cpp
    mensaje.h
    mensaje.cpp


    SolicitudAmistad.h
    SolicitudAmistad.cpp


    RedLeon.h
    RedLeon.cpp
    cadena.h
    cadena.cpp
    MonitorRecursos.h
    MonitorRecursos.cpp)

include(GNUInstallDirs)
install(TARGETS UdeBook
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
