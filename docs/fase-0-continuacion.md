# Fase 0 — Guía para continuar en el PC del juego

Mensaje de contexto para pasar a la máquina que tiene Assetto Corsa. Está escrito para que
pueda leerlo otra sesión de trabajo (o el propio autor) sin conocer nada de lo anterior.

Última actualización: 2026-10-14.

---

## 1. Qué es este proyecto

Aplicación externa en **C++** para **Assetto Corsa original** que lee la telemetría del coche en
tiempo real desde la memoria compartida de Windows y la convierte en dos herramientas: análisis
de dónde pierdes tiempo y cómo se comporta tu setup, y un pit wall en vivo solo para tu coche.

- Coche: **RSS Formula Hybrid** (versión exacta por confirmar, ver preguntas).
- Pista: **Spa-Francorchamps**.
- El plan completo, con las 12 fases y las casillas, está en `ROADMAP.md`.
- Las reglas de trabajo y cómo compilar están en `CLAUDE.md`.

Repositorio: <https://github.com/rodrago3490/reduss-telemetry> (público).

## 2. Situación: por qué hay dos máquinas

Assetto Corsa **no está instalado en la máquina de desarrollo** (verificado: solo hay Factorio,
Geometry Dash y redistribuibles en Steam; no existe `Documents\Assetto Corsa` ni Content
Manager). El juego vive en **otro PC** al que el autor accede por separado.

Consecuencia: se compila y se desarrolla en un sitio y se ejecuta y se graba en otro. Esto no
estaba previsto en el roadmap, que asumía juego y desarrollo en la misma máquina. Afecta a las
Fases 0, 1, 5 y 11.

## 3. Estado actual: qué está hecho y verificado

Esto **no es una suposición, está comprobado ejecutándolo**:

| Cosa | Estado |
| --- | --- |
| Visual Studio Community 2026 (`C:\Program Files\Microsoft Visual Studio\18\Community`) | Instalado |
| Carga de trabajo de C++ (MSVC 14.51.36231, `vcvars64.bat`) | Instalada |
| CMake 4.4.4 (winget) | Instalado y en el PATH |
| Git y GitHub CLI (`gh`, sesión iniciada) | Instalados |
| Identidad de Git | Configurada (`Rodrigo Ortiz`, correo `noreply` de GitHub) |
| Repositorio con `.gitignore`, `.gitattributes`, `CMakeLists.txt`, `CLAUDE.md`, `docs/` | Creado y subido |
| Compilación en Debug | **Funciona** (`cmake --build build --config Debug`) |
| Ejecutable `reduss-telemetry.exe` | **Corre** y reporta versión 0.0.1 y C++17 |
| Pruebas (`ctest`) | **Pasan** (1 de 1) |

Tareas 1, 2 y 3 de la Fase 0 cerradas. **Quedan 6 de 9**, todas bloqueadas por la instalación del
juego.

## 4. Qué hacer en el PC del juego

Por orden.

**Paso 1 — Instalar y confirmar el juego (tarea 4).** Anotar los datos exactos, no "cumple el
mínimo":

- [ ] Assetto Corsa original, con la app de Steam 244210
- [ ] Content Manager instalado (es la vía normal de todo el ecosistema de mods)
- [ ] Custom Shaders Patch, **número de versión exacto** (el requisito es 0.2.11 o posterior)
- [ ] RSS Formula Hybrid: **nombre exacto de la carpeta** dentro de `content/cars`
- [ ] Spa-Francorchamps disponible
- [ ] Ruta de la carpeta de setups del coche

El nombre de la carpeta del coche y la versión de CSP son dos de las preguntas abiertas: son
datos que solo se pueden obtener ahí.

**Paso 2 — Ejecutar el volcado (tarea 5).** El programa todavía no está escrito. Diseñado para
que funcione **sin conocer la estructura de los datos**, ver la sección 5.

**Paso 3 — Revisar el volcado (tarea 6).** Qué campos cambian, cuáles están siempre a cero, y
cómo se ven ERS, DRS y aerodinámica activa. Rellenar `docs/datos-disponibles.md`.

**Paso 4 — Probar las apps del coche (tarea 7).** RSS Control Centre y Race Lab, para concretar
qué aporta esta app de distinto (cierra una pregunta abierta).

**Paso 5 — Cerrar la documentación (tarea 9).**

**Paso 6 — Copiar el volcado a la máquina de desarrollo**, a la carpeta `dumps/` (que está en
`.gitignore`). Ese volcado es el dato de prueba con el que se harán las pruebas automáticas de
las Fases 2, 3 y 4 sin abrir el juego. **Es el entregable más valioso de toda la Fase 0**: sin él,
el análisis no se puede probar de forma reproducible.

## 5. Cómo escribir el volcado sin conocer la estructura

Punto importante de diseño, y la razón es esta:

> El archivo `SharedMemory.h` que define las estructuras de AC (`SPageFilePhysics`,
> `SPageFileGraphic`, `SPageFileStatic`) **no viene con Visual Studio ni con el SDK de Windows**.
> Se ha comprobado: no existe en el Windows SDK 10.0.26100.0 ni en las cabeceras de MSVC. Viene
> con el SDK de modding de AC, que está en el PC del juego o hay que descargar.

Por tanto, **no se pueden escribir las estructuras todavía sin inventarse los nombres de los
campos**. Y el roadmap es explícito: no se diseña sobre campos supuestos. La solución es que el
primer volcado no necesite saber nada:

**El volcado debe ser de bytes crudos, no de campos con nombre.**

1. Abrir los tres mapeos por nombre: `acpmf_physics`, `acpmf_graphics`, `acpmf_static`
   (`OpenFileMappingA` con `FILE_MAP_READ`).
2. **Preguntar el tamaño a Windows** con `VirtualQuery` sobre la vista mapeada. Esto da el tamaño
   real que publica AC, que es el número que necesita la Fase 1 para comparar. No hace falta
   `sizeof` de nada.
3. Leer los bytes tal cual y escribirlos a un archivo, con marca de tiempo y el identificador de
   paquete delante de cada muestra.
4. Repetir mientras se dan 2 o 3 vueltas en Spa, a la máxima frecuencia que se pueda.

Con eso se obtienen dos cosas a la vez: el volcado crudo y **el tamaño real de cada bloque**.

Después, el análisis del volcado (que sí se puede hacer ya en la máquina de desarrollo) es lo que
descubre dónde está cada campo: se mira **qué desplazamientos cambian** y se correlacionan con lo
que se sabe del juego (la velocidad sube y baja de forma reconocible, las RPM siguen al motor, la
marcha es un entero pequeño que salta, los pedales van de 0 a 1). De ahí salen los campos con
nombre, ya verificados, y con eso se escriben las estructuras de la Fase 1 **contra datos reales**.

El identificador de paquete va delante de cada muestra por una razón: es lo que permitirá medir la
frecuencia real de lectura y detectar muestras perdidas o duplicadas (Fase 1), y también saber si
el volcado se saltó muestras.

Cuando el volcado ya esté analizado, conviene además guardar un `.h` con las estructuras
deducidas, para que la Fase 1 empiece con los desplazamientos ya confirmados.

## 6. Preguntas abiertas que se cierran en el PC del juego

| # | Pregunta | Cómo se cierra |
| --- | --- | --- |
| 1 | ¿Es el Formula Hybrid 2026 el coche exacto y cuál es su carpeta? | Mirar `content/cars`; y el campo `carName` del bloque Static |
| 2 | ¿Qué canales de ERS y aerodinámica activa expone el RSS 2026? | Volcado real. **Ojo: la pregunta está mal planteada**, ver abajo |
| 3 | ¿Qué aporta esta app frente a RSS Race Lab? | Probar Race Lab y anotar la diferencia |
| 4 | ¿Dónde se instala AC? | Contestada: en otro PC, este documento es la consecuencia |
| 5 | ¿Content Manager o launcher original? | Decidir al instalar |
| 6 | ¿Qué versión exacta de CSP? | Anotar el número, no "cumple el mínimo" |
| 7 | ¿Cómo se obtiene el coche y con qué versión? | Al descargarlo |

**Sobre la pregunta 2, que es la más delicada:** los campos estándar de KERS y DRS de AC
**existen siempre** en la memoria compartida, porque los publica el juego, no el coche. La duda
real no es si existen, sino **si el RSS 2026 los rellena o los deja a cero**, porque tiene su
propio ERS y su aerodinámica activa gobernados por sus apps.

Por eso cada canal hay que clasificarlo en **tres** estados, no dos:

- **Existe y varía** → utilizable.
- **Existe y siempre a cero** → el campo está pero el coche no lo rellena. No se puede mostrar
  como si fuera un dato: engaña más que un canal ausente, porque parece que funciona.
- **No existe** → no está en el bloque.

Esto afecta directamente al alcance de la Fase 9.

## 7. Trampas ya encontradas (no repetirlas)

1. **MSVC no dice la verdad sobre C++ por defecto.** Sin la opción `/Zc:__cplusplus`, Visual
   Studio deja `__cplusplus` en `199711L` aunque se compile con `/std:c++17`. Esto ya hizo fallar
   una prueba en este proyecto. Está resuelto en `CMakeLists.txt` y en `tests/prueba_entorno.cpp`,
   donde se mira `_MSVC_LANG`. **No quitar esa opción.**
2. **CMake 4.x es estricto.** Las versiones 4.x eliminaron el soporte de
   `cmake_minimum_required(VERSION < 3.5)`. Cualquier librería de terceros que se añada (LZ4,
   Zstandard, ImGui, cpp-httplib) puede fallar al configurar por este motivo. El proyecto fija
   3.20.
3. **`cl.exe` no está en el PATH.** Es normal: se usa desde el entorno de Visual Studio. No pasar
   `vcvars64.bat` a mano: CMake encuentra el compilador solo.
4. **Comparar tamaños no basta.** Que nuestra estructura mida lo que creemos no demuestra que
   coincida con la de AC. Un desajuste de relleno (*padding*) desplaza todos los campos siguientes
   y produce datos basura sin ningún error visible. Por eso el volcado mide el tamaño real con
   `VirtualQuery` y se anota en `docs/datos-disponibles.md`.
5. **El dominio del fabricante que se citó en la planificación no existe.**
   `race-sim-studio.com` no resuelve; el real parece ser `racesimstudio.com`. La fecha de
   lanzamiento del coche, sus apps y el requisito de CSP salen de una sola noticia sin verificar.

## 8. Comandos que funcionan hoy (máquina de desarrollo)

```powershell
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
.\build\Debug\reduss-telemetry.exe
```

El ejecutable no hace nada útil todavía: solo confirma que el entorno compila y se ejecuta. La
lectura real del juego empieza en la Fase 1.

## 9. Siguiente paso concreto

En la máquina de desarrollo, sin necesidad del juego:

1. Escribir el programa de volcado crudo descrito en la sección 5 (código de la tarea 5).
2. Dejarlo listo para copiar al PC del juego y ejecutarlo allí durante 2 o 3 vueltas.

En el PC del juego: instalar y confirmar todo lo del paso 1 de la sección 4, ejecutar el volcado y
traerlo de vuelta.
