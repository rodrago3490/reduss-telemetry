# CLAUDE.md — contexto y reglas del proyecto

Este archivo es lo primero que hay que leer al empezar una sesión de trabajo.
El plan completo está en `ROADMAP.md`; la copia viva es la del repositorio.

## Objetivo

Aplicación externa en C++ para **Assetto Corsa original** que lee la telemetría del coche en
tiempo real desde la memoria compartida de Windows y la convierte en dos herramientas: análisis
de dónde pierdes tiempo y cómo se comporta tu setup, y un pit wall en vivo solo para tu coche.

Coche inicial: **RSS Formula Hybrid (versión a confirmar en la Fase 0)**.
Pista inicial: **Spa-Francorchamps**.
El MVP se cierra al terminar la **Fase 5** (`v0.1`); la `v1.0` es la Fase 11.

## Cómo compilar

Requiere Visual Studio con la carga de trabajo de C++ y CMake (los dos ya están instalados en
la máquina de desarrollo).

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

El ejecutable queda en `build\Debug\reduss-telemetry.exe`. Todavía no lee nada del juego: solo
confirma que el entorno funciona.

## Cómo ejecutar las pruebas

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

`--output-on-failure` es obligatorio en la práctica: sin él, CTest solo dice que algo falló y no
enseña qué comprobación.

Las pruebas se ejecutan **sin abrir el juego**, con vueltas grabadas, a partir de la Fase 2.

## Reglas del código

- **Una tarea por commit**, con mensaje claro en imperativo. No mezclar fases en un mismo commit.
- **C++ simple.** Pocas dependencias, sin plantillas complicadas, sin trucos. Nombres claros y
  comentarios donde la intención no sea obvia. El autor mantiene este código: si una pieza no se
  entiende al leerla, está mal escrita.
- **El hilo de lectura nunca espera.** No espera a la interfaz, ni al disco, ni a la red. Si
  espera, se pierden muestras. Es la regla de arquitectura más importante del proyecto.
- **No cambiar el formato de archivo de vuelta sin subir su número de versión** y documentarlo
  en `docs/formato-vuelta.md`.
- **Cada canal que se lea del juego tiene que estar verificado** en `docs/datos-disponibles.md`
  como disponible. No se diseña sobre campos supuestos.
- Los datos locales (vueltas grabadas, volcados, bases de datos) no se versionan: viven en
  `data/` y `dumps/`, que están en `.gitignore`.

## Cómo explicar cada pieza

El autor está aprendiendo C++ (es su primer lenguaje). Al construir algo nuevo:

- Explicar en lenguaje sencillo qué hace la pieza y por qué se hace así.
- Señalar lo específico de Windows cuando aparezca (memoria compartida, UTF-16, hilos).
- Evitar C++ "de experto" cuando haya una forma más larga pero más legible.

## Estructura del repositorio

```
CMakeLists.txt
CLAUDE.md            este archivo
ROADMAP.md           el plan, con las casillas para marcar
docs/                datos-disponibles.md, formato-vuelta.md, decisiones
src/
  app/               main y arranque de hilos
  core/              tipos y lector de memoria compartida
  analysis/          vueltas, remuestreo, delta, curvas, setup
  storage/           archivos de vuelta y SQLite
  ui/                pit wall local (ImGui)
  server/            servidor y vista web
tests/               pruebas automaticas (se ejecutan sin el juego)
data/                curvas de Spa y configuracion (lo demas no se versiona)
dumps/               volcados de memoria compartida de la Fase 0 (no se versiona)
```

Las carpetas `src/core`, `src/analysis`, `src/storage`, `src/ui` y `src/server` se crean en la
fase que las necesita. Hoy solo existe `src/app` con un `main` de comprobación.

## Estado actual

Fase 0 en curso. Hecho: entorno, repositorio, `CMakeLists.txt` mínimo, este archivo y las
plantillas de `docs/`. Pendiente: todo lo que necesita el juego instalado (volcado real,
validación de canales). Ver la sección "Preguntas abiertas" de `ROADMAP.md` y
`docs/fase-0-analisis-inicial.md`.

## Al empezar cada sesión

1. Leer este archivo y la fase en curso del `ROADMAP.md`.
2. Acordar qué tareas de la fase se hacen en esa sesión.
3. Trabajar en pasos pequeños: una tarea, compilar, probar, marcar la casilla, commit.
