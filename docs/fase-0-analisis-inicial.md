# Fase 0 — Análisis inicial y preguntas abiertas

Fecha: 2026-10-14. Estado: **Fase 0 abierta, bloqueada en su parte de juego**.

Este documento recoge (a) el estado real del entorno medido en esta máquina, (b) qué tareas de
la Fase 0 se pueden cerrar ya y cuáles no, y (c) las preguntas que hay que contestar antes de
seguir. No sustituye al `ROADMAP.md`: cuando una respuesta se cierre, se anota también allí.

---

## 1. Estado real del entorno (medido, no supuesto)

| Comprobación | Tarea Fase 0 | Resultado | Estado |
| --- | --- | --- | --- |
| Visual Studio Community 2026 | 1 | `C:\Program Files\Microsoft Visual Studio\18\Community` | OK |
| Carga de trabajo de C++ (MSVC) | 1 | `VC\Tools\MSVC\14.51.36231`, `vcvars64.bat` presente | OK |
| CMake | 1 | 4.4.4 (winget), en el PATH | OK |
| Git | 1 | `C:\Program Files\Git\cmd\git.exe` | OK |
| GitHub CLI | 1 | `gh` 2026, sesión iniciada como `rodrago3490` (scopes `repo`, `workflow`) | OK |
| Identidad de Git (`user.name` / `user.email`) | 1 | **sin configurar** (ninguno de los dos) | Falta |
| Assetto Corsa original | 4 | **no instalado**: no existe la app 244210 en Steam | Bloqueado |
| Documentos de AC (`Documents\Assetto Corsa`) | 4 | no existe | Bloqueado |
| Content Manager | 4 | no instalado (`%LOCALAPPDATA%` y `%APPDATA%\AcTools Content Manager`) | Bloqueado |
| Custom Shaders Patch 0.2.11+ | 4 | no instalado | Bloqueado |
| RSS Formula Hybrid | 4 | no instalable (no hay AC) | Bloqueado |
| Pista Spa-Francorchamps | 4 | el DLC original no está verificado | Bloqueado |

Librería de Steam en esta máquina: una sola, `C:\Program Files (x86)\Steam`; apps instaladas
Factorio (427520), Geometry Dash (322170) y Steamworks Common Redistributables (228980).
Único disco: `C:` (1301 GB libres).

### Dos avisos derivados del entorno

1. **CMake 4.x es estricto.** Las versiones 4.x eliminaron la compatibilidad con
   `cmake_minimum_required(VERSION < 3.5)`. Cualquier fragmento de tutorial antiguo o librería
   de terceros (LZ4, Zstandard, ImGui, cpp-httplib) que declare un mínimo viejo fallará al
   configurar. Hay que fijar un mínimo moderno explícito, no copiar el `CMakeLists.txt` de un
   tutorial.
2. **El primer commit fallará** hasta que exista una identidad de Git. Es un minuto de trabajo,
   pero conviene hacerlo antes de crear el repositorio.

---

## 2. Qué se puede cerrar ya y qué no

La Fase 0 tiene 9 tareas. Se reparten así:

**Cerrables sin el juego (se pueden hacer hoy):**

- Tarea 1 — entorno. Solo falta la identidad de Git y decidir CMake frente a MSBuild.
  La carga de trabajo de C++ ya está instalada.
- Tarea 2 — repositorio, `.gitignore` y `CMakeLists.txt` mínimo que compile. **Depende de la
  pregunta Q4** (nombre de la app): el nombre decide la carpeta y el nombre del ejecutable.
- Tarea 3 — `CLAUDE.md`. El roadmap ya dicta su contenido en la sección "Cómo trabajar con
  Claude Code"; es copiar las reglas y añadir cómo compilar y cómo ejecutar las pruebas.
- Tareas de documentación (parcial) — la estructura de `docs/` y la plantilla de la tabla de
  canales de `docs/datos-disponibles.md` se pueden dejar preparadas vacías.

**Bloqueadas hasta instalar AC, CSP, el coche y la pista:**

- Tarea 4 — confirmar la instalación (AC, CSP ≥ 0.2.11, carpeta y versión del coche, Spa).
- Tarea 5 — el programa que vuelca los tres bloques mientras das 2 o 3 vueltas en Spa.
- Tarea 6 — revisar el volcado.
- Tarea 7 — probar RSS Control Centre y Race Lab.
- Tarea 8 — localizar la carpeta de setups del coche y la pista.
- Tarea 9 — cerrar `docs/datos-disponibles.md` con la tabla de canales real.

Las tres condiciones de terminado que hablan de "volcado real", "canales de ERS y aerodinámica"
y "diferencia frente a Race Lab" dependen de esas tareas bloqueadas. **La Fase 0 no se puede
cerrar sin AC instalado en algún sitio accesible.**

### Lo que sí se puede adelantar sin el juego

El código de la tarea 5 (el volcado) no necesita el juego para escribirse, solo para ejecutarse.
Escribirlo ahora y probarlo contra una memoria compartida falsa deja la Fase 0 a un solo paso
en cuanto haya AC: instalar, lanzar el volcado, conducir y revisar. Ver Q1.

---

## 3. Preguntas abiertas

### 3.1 Las cinco del roadmap asignadas a la Fase 0

| # | Pregunta | Estado | Cómo se cierra |
| --- | --- | --- | --- |
| Q1 | ¿Es el Formula Hybrid 2026 el coche exacto y cuál es el nombre de su carpeta? | Abierta | Instalar y leer `carName` del bloque Static; se puede además listar `content/cars`. El nombre de carpeta es el dato que necesitan las Fases 2, 3 y 8 |
| Q2 | ¿Qué canales de ERS y aerodinámica activa expone realmente el RSS 2026? | Abierta | Solo con volcado real. Ver el detalle técnico en 3.3, porque la duda es más concreta de lo que dice el roadmap |
| Q3 | ¿Qué aporta esta app frente a RSS Race Lab? | Abierta | Probar Race Lab (tarea 7) y anotar la diferencia. Decisión de producto, no de código |
| Q4 | ¿Cómo se llamará la aplicación? | Abierta | **Bloquea la tarea 2 del roadmap.** La pregunta dice "antes de crear el repositorio", así que en la práctica es de Fase 0 |
| Q5 | ¿El repositorio será público o privado? | Abierta | La cuenta `gh` ya está autenticada; es una elección, se puede decidir en un minuto |

**Q4 y Q5 bloquean la tarea 2, que es la siguiente tarea ejecutable del roadmap.** La carpeta
actual se llama `telemetry-reduss` y el roadmap abre con `ac-telemetry/`: ninguno de los dos
está confirmado como nombre de la aplicación.

### 3.2 Preguntas que la Fase 0 necesita y el roadmap no recoge

Ninguna de estas está escrita en el roadmap, pero la Fase 0 no se puede ejecutar sin decidirlas:

| # | Pregunta | Por qué importa |
| --- | --- | --- |
| Q6 | ¿Dónde se instala AC: en esta máquina o en otro PC? | El roadmap asume "Windows, Visual Studio ya instalado" y da por hecho que juego y desarrollo conviven. Aquí no hay AC. Si el juego vive en otro PC, cambian las Fases 0, 1, 5 y 11: se compila aquí, se copia el ejecutable allí y se traen los volcados de vuelta |
| Q7 | ¿Se usa Content Manager o el launcher original? | Prácticamente todo el ecosistema de AC (CSP, RSS, mods) se instala y se lanza por Content Manager. Afecta a dónde vive la carpeta de setups de la tarea 8 y a las rutas que leerá la Fase 8 |
| Q8 | ¿Qué versión exacta de CSP se instala? | El roadmap exige "0.2.11 o posterior" pero no fija una. CSP es un mod en desarrollo continuo y la versión cambia campos disponibles, que es justo lo que valida la Fase 0. Hay que anotar el número exacto, no solo "cumple el mínimo" |
| Q9 | ¿Cómo se compra/obtiene el coche y con qué versión? | El roadmap cita una noticia de Traxion pero no la vía de descarga ni si el paquete es gratuito o de pago. Esto además decide si hace falta compartir cuenta entre máquinas |

### 3.3 Detalle técnico de Q2: la pregunta es más fina de lo que parece

El roadmap dice "qué canales de ERS y aerodinámica activa expone el coche", pero eso mezcla dos
cosas distintas que hay que separar en el volcado:

1. **Los canales estándar de la memoria compartida sí existen siempre.** La estructura de física
   de AC que publica Kunos incluye campos de KERS (carga, aporte del piloto, energía consumida
   por vuelta, límite por vuelta, si KERS está disponible) y de DRS (disponible, activo).
   El volcado los va a mostrar con valores, no en blanco.
2. **La duda real es si el coche los rellena con su propio sistema o los deja a cero**, porque
   el RSS 2026 tiene su propio ERS y su aerodinámica activa, probablemente gobernados por sus
   apps (Control Centre). Un campo puede existir y estar siempre a cero: eso es lo que la
   tarea 6 tiene que detectar, y el roadmap ya lo pide ("cuáles están siempre a cero").

Por tanto la tarea 6 debe distinguir tres estados por canal, no dos: **existe y varía**,
**existe y está siempre a cero**, y **no existe**. Y la consecuencia para la Fase 9 no es solo
"faltan canales", sino también "hay canales presentes que no son fiables". El criterio de
terminado de la Fase 9 ("los datos de ERS mostrados coinciden con lo que muestra el coche")
es el que separa un caso del otro.

### 3.4 Un hueco del roadmap que conviene cerrar en esta fase

La primera tarea de la Fase 1 pide "definir las estructuras con el mismo orden y empaquetado que
AC, y comprobar sus tamaños en tiempo de compilación". Esa comprobación solo dice que tu
estructura mide lo que tú crees; no dice que coincida con lo que AC publica de verdad. Un
desajuste de relleno (*padding*) desplaza todos los campos siguientes y produce datos basura
sin ningún error.

Los manejadores de memoria compartida ya conocen el tamaño real que publica AC, así que **la
Fase 0 debería dejar anotado ese tamaño real por bloque** para que la Fase 1 compare contra un
número medido y no contra una suposición. Es barato de añadir al volcado de la tarea 5 y ahorra
una depuración muy dolorosa.

---

## 4. Orden propuesto para desbloquear la fase

1. Decidir **Q4** (nombre) y **Q5** (visibilidad): desbloquean la tarea 2.
2. Configurar la identidad de Git y crear el repositorio con `.gitignore` y el `CMakeLists.txt`
   mínimo que compile (tareas 1 y 2, con el mínimo de CMake moderno por el aviso de CMake 4.x).
3. Escribir el `CLAUDE.md` (tarea 3).
4. Decidir **Q6 a Q9** e instalar AC, Content Manager, CSP, el coche y Spa (tarea 4).
5. Escribir el volcado y ejecutarlo durante 2 o 3 vueltas (tarea 5).
6. Revisar el volcado, cerrar Q1 y Q2, probar Race Lab y cerrar Q3 (tareas 6 y 7).
7. Anotar la carpeta de setups (tarea 8) y cerrar `docs/datos-disponibles.md` (tarea 9).

Los pasos 1 a 3 no dependen de tener AC. Los pasos 4 a 7, sí.

---

## 5. Nota sobre las fuentes

El roadmap se apoya en una noticia de Traxion para el lanzamiento del coche, sus apps incluidas
y el requisito de CSP. Al comprobar el dominio del fabricante, `race-sim-studio.com` no existe
(la resolución DNS falla); el dominio real parece ser `racesimstudio.com`. Conviene confirmar la
URL del fabricante y su página del coche antes de darla por buena en la documentación, y anotar
de dónde sale cada afirmación sobre requisitos y contenido del paquete.
