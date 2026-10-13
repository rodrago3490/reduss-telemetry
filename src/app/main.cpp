// Punto de entrada de Reduss Telemetry.
//
// Fase 0, tarea 2: este programa todavia no lee nada del juego. Solo existe
// para comprobar que el entorno compila, enlaza y se ejecuta. En la Fase 1
// este mismo archivo arrancara el hilo que lee la memoria compartida de AC.
//
// Todavia no se usa la API de Windows a proposito: asi el proyecto compila
// tambien fuera de Windows y el esqueleto se puede probar en cualquier sitio.

#include <iostream>

#ifndef REDUSS_VERSION
// Permite compilar el archivo suelto, sin pasar por CMake.
#define REDUSS_VERSION "desconocida"
#endif

namespace {

// Version del estandar que esta usando de verdad el compilador.
//
// En Visual Studio hay que mirar _MSVC_LANG en vez de __cplusplus: sin la
// opcion /Zc:__cplusplus, MSVC deja __cplusplus en 199711L pase lo que pase.
// Los valores son los que documenta el estandar: 201703L es C++17.
// CMakeLists.txt activa /Zc:__cplusplus, asi que los dos coinciden, pero el
// codigo funciona tambien si alguien compila este archivo a mano.
#if defined(_MSVC_LANG)
#define REDUSS_ESTANDAR _MSVC_LANG
#else
#define REDUSS_ESTANDAR __cplusplus
#endif

const char* nombre_del_estandar() {
#if REDUSS_ESTANDAR >= 202002L
    return "C++20 o posterior";
#elif REDUSS_ESTANDAR >= 201703L
    return "C++17";
#elif REDUSS_ESTANDAR >= 201402L
    return "C++14";
#else
    return "anterior a C++14";
#endif
}

}  // namespace

int main() {
    std::cout << "Reduss Telemetry\n";
    std::cout << "  version        : " << REDUSS_VERSION << '\n';
    std::cout << "  estandar de C++: " << nombre_del_estandar()
              << " (REDUSS_ESTANDAR = " << REDUSS_ESTANDAR << ")\n";
    std::cout << "  estado         : esqueleto de la Fase 0, aun no lee el juego\n";
    return 0;
}