// Primera prueba automatica del proyecto.
//
// De momento no prueba logica de telemetria: comprueba que la cadena de
// compilacion y de pruebas funciona y que el estandar de C++ es el esperado.
// Las pruebas de verdad (vueltas, sectores, delta) llegan en la Fase 2, usando
// el volcado real de la Fase 0 como datos de entrada.
//
// Convencion: el programa devuelve 0 si todo va bien y 1 si algo falla.

#include <iostream>
#include <string>

namespace {

// Version del estandar que usa de verdad el compilador.
// En Visual Studio hay que mirar _MSVC_LANG: sin la opcion /Zc:__cplusplus,
// __cplusplus se queda en 199711L aunque se compile con /std:c++17.
// CMakeLists.txt activa /Zc:__cplusplus, pero asi la prueba tambien vale si
// alguien compila este archivo a mano.
#if defined(_MSVC_LANG)
#define REDUSS_ESTANDAR _MSVC_LANG
#else
#define REDUSS_ESTANDAR __cplusplus
#endif

int fallos = 0;

void comprobar(bool condicion, const std::string& descripcion) {
    if (condicion) {
        std::cout << "[ OK ] " << descripcion << '\n';
    } else {
        std::cout << "[FALLO] " << descripcion << '\n';
        ++fallos;
    }
}

}  // namespace

int main() {
    // static_assert se evalua al compilar: esta comprobacion no cuesta nada
    // en tiempo de ejecucion y falla antes de que el programa llegue a correr.
    static_assert(REDUSS_ESTANDAR >= 201703L,
                  "El proyecto espera C++17 o posterior");

    std::cout << "Prueba de entorno\n";
    std::cout << "  estandar detectado: " << REDUSS_ESTANDAR << '\n';

    comprobar(sizeof(char) == 1, "char ocupa 1 byte");
    comprobar(sizeof(float) == 4, "float ocupa 4 bytes (lo asume AC)");
    comprobar(sizeof(double) == 8, "double ocupa 8 bytes");

    // La memoria compartida de AC se lee como bytes y se interpreta como
    // estructuras. Si el compilador empaquetara de otra forma, todo se
    // desplazaria. Esta comprobacion es un primer aviso; la comprobacion
    // definitiva es comparar el tamano de nuestras estructuras con el tamano
    // real que publica AC, que se anota en la Fase 0.
    struct ConRelleno {
        char  a;
        int   b;
    };
    comprobar(sizeof(ConRelleno) == 8,
              "el compilador alinea las estructuras (char + int = 8 bytes)");

    if (fallos == 0) {
        std::cout << "Todo correcto.\n";
        return 0;
    }

    std::cout << fallos << " comprobacion(es) fallida(s).\n";
    return 1;
}
