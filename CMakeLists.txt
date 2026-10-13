cmake_minimum_required(VERSION 3.20)

project(reduss_telemetry
    VERSION 0.0.1
    DESCRIPTION "Telemetria para Assetto Corsa: analisis de conduccion y pit wall"
    LANGUAGES CXX)

# --- Estandar de C++ ---
# C++17 es suficiente y es el mas sencillo de mantener con Visual Studio.
# CXX_EXTENSIONS OFF pide el estandar puro (sin las extensiones de Microsoft).
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Los ejecutables van a la raiz de la carpeta de compilacion, no dentro de
# su subcarpeta: asi es facil encontrar el .exe sin buscar por el arbol.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# --- Avisos del compilador ---
# /W4 avisa de casi todo. No se usa /WX (avisos como error): al empezar,
# un aviso no debe impedirte compilar y probar.
if(MSVC)
    # /Zc:__cplusplus es necesario: sin el, MSVC define __cplusplus como 199711L
    # aunque se compile con /std:c++17, y no se puede saber que estandar se esta
    # usando desde el codigo. Es una rareza conocida de Visual Studio.
    add_compile_options(/W4 /permissive- /utf-8 /Zc:__cplusplus)
else()
    add_compile_options(-Wall -Wextra)
endif()

# --- Programa principal ---
# De momento solo comprueba que el entorno funciona (Fase 0, tarea 2).
# En la Fase 1, aqui empezara el lector de memoria compartida.
add_executable(reduss-telemetry
    src/app/main.cpp
)

# La version del proyecto se puede leer desde el codigo con REDUSS_VERSION.
target_compile_definitions(reduss-telemetry
    PRIVATE REDUSS_VERSION="${PROJECT_VERSION}"
)

# --- Pruebas ---
enable_testing()

add_executable(prueba-entorno
    tests/prueba_entorno.cpp
)

# Esta prueba solo comprueba que el proyecto compila y que el estandar de C++
# es el esperado. Las pruebas del analisis llegaran en la Fase 2.
add_test(NAME entorno COMMAND prueba-entorno)
