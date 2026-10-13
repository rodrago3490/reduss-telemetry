# Datos disponibles en la memoria compartida

Estado: **plantilla sin rellenar**. Esta tabla es la tarea 9 de la Fase 0 y no se puede cerrar
hasta tener Assetto Corsa con el RSS Formula Hybrid y CSP instalados en la máquina de juego.

## Cómo se rellena

El programa de volcado de la Fase 0 (tarea 5) guarda los tres bloques de memoria compartida
(Physics, Graphics y Static) mientras se dan 2 o 3 vueltas en Spa. Después, para cada canal:

1. Se mira si el valor **cambia** durante las vueltas.
2. Si no cambia, se mira si está **siempre a cero** o si es una constante razonable.
3. Se anota el resultado en la columna "Estado".

Los tres estados posibles y qué significan para el resto del roadmap:

| Estado | Significado | Consecuencia |
| --- | --- | --- |
| **Existe y varía** | El canal es utilizable | Se puede diseñar sobre él |
| **Existe y siempre a cero** | El campo está en la estructura pero el coche no lo rellena | Existe, pero no es fiable: no se muestra como si fuera un dato |
| **No existe** | El canal no está en el bloque | No aparece en la interfaz y queda documentado |

El tercer caso es el que el roadmap ya preveía ("cuáles están siempre a cero"). El segundo es
igual de importante y más fácil de pasar por alto: un canal presente pero muerto engaña más que
uno ausente, porque parece que funciona.

## Tabla de canales

Rellenar una fila por canal. Los bloques son `Physics` (`acpmf_physics`), `Graphics`
(`acpmf_graphics`) y `Static` (`acpmf_static`).

| Bloque | Campo | Unidad | Estado | Observaciones |
| --- | --- | --- | --- | --- |
| Physics | `speedKmh` | km/h | por verificar | |
| Physics | `rpms` | rpm | por verificar | |
| Physics | `gear` | — | por verificar | |
| Physics | `gas` / `brake` / `clutch` | 0..1 | por verificar | |
| Physics | `steerAngle` | grados | por verificar | |
| Physics | `kersCharge` | 0..1 | **por verificar** | Pregunta Q2: ¿lo rellena el RSS 2026? |
| Physics | `kersInput` | 0..1 | **por verificar** | Pregunta Q2 |
| Physics | `kersCurrentKJ` | kJ | **por verificar** | Pregunta Q2 |
| Physics | `drsAvailable` | bool | **por verificar** | Pregunta Q2 |
| Physics | `drsActive` | bool | **por verificar** | Pregunta Q2 |
| Physics | `tyreWear` ×4 | 0..1 | por verificar | |
| Physics | `tyreDirtLevel` ×4 | 0..1 | por verificar | |
| Physics | `discTemperature` ×4 | °C | por verificar | |
| Physics | `padLife` ×4 | 0..1 | por verificar | |
| Physics | `fuel` | litros | por verificar | |
| Physics | `turboBoost` | bar | por verificar | |
| Static | `carName` | texto | por verificar | Cierra Q1: nombre exacto del coche |
| Static | `trackName` / `trackConfiguration` | texto | por verificar | Confirma Spa y su configuración |
| Static | `maxRpm`, `maxPower`, `maxTorque` | — | por verificar | |
| Graphics | `completedLaps` | — | por verificar | |
| Graphics | `currentTime` / `lastTime` / `bestTime` | ms | por verificar | |
| Graphics | `normalizedCarPosition` | 0..1 | por verificar | Clave para alinear vueltas |

Añadir filas hasta cubrir la lista de canales de la sección "Datos que maneja la app" del
roadmap, **incluidos los que resulten no existir**: saber qué no hay es tan útil como saber qué
hay, y la Fase 9 depende de ello.

## Tamaños reales de las estructuras

Anotar aquí el tamaño que publica AC de verdad para cada bloque, medido desde los manejadores
de memoria compartida. Es el número contra el que la Fase 1 comparará sus estructuras.

Comprobar que nuestra estructura mide lo que creemos **no** demuestra que coincida con la de AC.
Un desajuste de relleno (*padding*) desplaza todos los campos siguientes y produce datos basura
sin ningún error. Comparar contra el tamaño real sí lo demuestra.

| Bloque | Tamaño real publicado por AC |
| --- | --- |
| Physics | por medir |
| Graphics | por medir |
| Static | por medir |

## Apps del coche

Tarea 7 de la Fase 0. Se prueban las apps incluidas con el coche (RSS Control Centre y Race Lab)
y se anota qué hacen ya, para concretar qué aporta esta app (cierra Q3).

| App | Qué hace | Qué aporta esta app de distinto |
| --- | --- | --- |
| RSS Control Centre | por revisar | |
| Race Lab | por revisar | |
