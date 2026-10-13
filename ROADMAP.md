# Documento técnico y roadmap — App de telemetría para Assetto Corsa

Actualizado: 2026-10-13

## Resumen

Aplicación externa en C++ para Assetto Corsa original que lee la telemetría del coche en tiempo real y la convierte en dos herramientas: un análisis de dónde pierdes tiempo y cómo se comporta tu setup, y un pit wall en vivo solo para tu coche.

La primera versión utilizable (MVP) llega al terminar la Fase 5. Las fases 6 a 11 la completan con histórico, análisis por curva, diagnóstico del setup, estrategia, servidor web y optimización. Este documento recoge lo decidido en la sesión de planificación y sirve de guía de trabajo con Claude Code: cada fase tiene alcance, tareas y criterios de terminado con casillas para ir marcándolas.

## Decisiones tomadas

Estas decisiones están cerradas; cualquier cambio debe reflejarse aquí antes de tocar el código.

| Tema | Decisión |
| --- | --- |
| Juego | Assetto Corsa original (no ACC ni Evo) |
| Tipo de app | Aplicación externa que lee la memoria compartida de AC, no una app Python dentro del juego |
| Lenguaje | C++, con código simple, pocas dependencias y estructura clara (es el primer lenguaje del autor) |
| Entorno | Windows, Visual Studio con el componente de C++ ya instalado |
| Coche inicial | RSS Formula Hybrid, la versión más reciente (Formula Hybrid 2026, a confirmar en la Fase 0) |
| Pista inicial | Spa-Francorchamps |
| Referencia de comparación | Mejor vuelta válida de la sesión |
| Alcance del pit wall | Solo el coche propio |
| Pit wall | Integrado en la app, con servidor opcional para verlo desde otros dispositivos |
| Histórico | Se guarda: vuelta cruda comprimida y versión remuestreada |
| Rendimiento | Criterio de diseño desde el primer día; stack ligero para las partes pesadas |
| Setup | Primero diagnóstico (síntomas y evidencia), sin consejos automáticos |
| Modo coach | Futuro: carreras con otra persona o con un coach |
| Yuko | Aparcado; no hay trabajo previsto en este roadmap |

## Alcance

La versión inicial (MVP) cubre lo necesario para ver tu delta en vivo en Spa con el RSS Formula Hybrid; el resto se añade por fases.

**Entra en el MVP (Fases 0 a 5)**

- Lectura en vivo de la memoria compartida de tu coche
- Detección de vueltas, sectores y validez
- Grabación de vueltas a disco
- Remuestreo por posición y delta contra la mejor vuelta de la sesión
- Interfaz local con panel en vivo

**Entra después (Fases 6 a 11)**

- Histórico en SQLite y comparación entre sesiones
- Análisis por curva y diagnóstico de conducción
- Lectura del setup y diagnóstico del setup
- Pit wall completo con estrategia, neumáticos, combustible y ERS
- Servidor local y vista web
- Optimización, pruebas de estabilidad y empaquetado

**Fuera de alcance por ahora**

- Telemetría de rivales (la memoria compartida solo da tu coche)
- Modo coach y comparación entre dos pilotos
- ACC, AC Evo y otros juegos
- Otros coches y pistas distintos de RSS Formula Hybrid y Spa
- Recomendaciones automáticas de cambios de setup
- Datos biométricos del piloto y módulo Yuko

## Datos que maneja la app

AC publica tres bloques de memoria compartida (Physics, Graphics y Static); todo lo demás se calcula o no existe. Esta lista es la referencia para el diseño del formato de grabación y de la base de datos, y la Fase 0 la contrasta con lo que el RSS Formula Hybrid expone de verdad.

### Leídos de la memoria compartida

**Mandos del piloto**

- Acelerador, freno, embrague, ángulo de volante
- Marcha, RPM, velocidad
- Reparto de frenada, ABS, control de tracción, limitador de pit

**Dinámica del coche**

- Aceleración en 3 ejes (G), velocidad local y global
- Cabeceo, balanceo, guiñada y velocidad angular local
- Altura al suelo delantera y trasera, altura del centro de gravedad
- Fuerza del force feedback

**Por rueda (×4)**

- Deslizamiento y velocidad angular
- Carga vertical y recorrido de suspensión
- Caída (camber)
- Presión del neumático
- Temperatura interior, media, exterior y del núcleo
- Desgaste y nivel de suciedad
- Temperatura del disco de freno
- Punto de contacto y normal del neumático
- Ruedas fuera de pista (conteo)

**Motor, híbrido y daños**

- Presión del turbo y densidad del aire
- Energía y nivel del sistema híbrido (ERS/KERS), modos de despliegue y recuperación
- DRS disponible y activo
- Daños en 5 zonas
- Combustible restante

**Sesión y vuelta**

- Tiempo actual, última vuelta, mejor vuelta y parcial de sector (en ms)
- Sector actual, tiempo del último sector, vueltas completadas, posición
- Posición normalizada en pista (0 a 1), la clave para alinear vueltas
- Coordenadas XYZ, distancia recorrida, tiempo restante de sesión
- Estado de pit y pit lane, parada obligatoria hecha
- Compuesto de neumático, bandera, penalización, agarre de la superficie
- Viento (velocidad y dirección), temperatura del aire y de la pista

**Estáticos (coche y pista)**

- Modelo de coche, pista y configuración, nombre del piloto
- Potencia y par máximos, RPM máximas, capacidad del depósito
- Recorrido máximo de suspensión y radio de neumáticos
- Ayudas activas, longitud de la pista, número de sectores
- Si el coche tiene DRS, ERS o KERS

### Calculados por la app

**Dónde pierdes tiempo**

- Delta continuo contra la vuelta de referencia, por posición normalizada
- Tiempo perdido por curva y por sector
- Punto de frenada, presión máxima, tiempo de frenada
- Velocidad mínima y velocidad de apex por curva
- Punto y progresividad de aplicación del acelerador a la salida
- Trail braking: solape entre freno y volante
- Correcciones de volante y ruido del acelerador
- Cambios de marcha: RPM de cambio y tiempo perdido
- Vueltas inválidas y salidas de pista

**Entender el setup**

- Subviraje o sobreviraje: diferencia de deslizamiento delantero y trasero por fase (entrada, apex, salida)
- Equilibrio de cargas y transferencia de peso
- Ventana de neumáticos: temperaturas interior, media y exterior, y presión en caliente frente a la objetivo
- Bloqueos de frenada: deslizamiento delantero frente a trasero
- Altura y cabeceo: contacto con el suelo (bottoming) y plataforma aerodinámica
- Velocidad de amortiguador, derivada del recorrido de suspensión
- Patinaje en salida de curva, por marcha

**Estrategia y pit wall**

- Consumo por vuelta y vueltas de combustible restantes
- Degradación de neumáticos: tiempo perdido por vuelta frente a desgaste
- Ritmo por stint y consistencia (desviación típica)
- Ventana de parada y tiempo de pit

### Lo que AC no da

- **Valores del setup**: no salen en la memoria compartida; se leen del archivo de setup del coche (Fase 8)
- **Telemetría de rivales**: solo hay datos de tu coche
- **Carga aerodinámica real**: solo se puede estimar con la carga vertical y la velocidad
- **Datos biométricos del piloto**: no existen
- **ERS y aerodinámica activa del RSS 2026**: pueden no aparecer en los campos estándar; es el primer punto a validar (ver Riesgos)

## Arquitectura y stack

Un solo proceso con varios hilos, donde el hilo que lee la memoria compartida nunca espera a la interfaz, al disco ni a la red; si espera, se pierden muestras.

```
 Assetto Corsa        Lectura (hilo 1)       Análisis (hilo 2)
 memoria compartida   nunca espera a    -->  vueltas, delta,   --+--> Escritura (hilo 3): vueltas a disco y SQLite
 Physics, Graphics -->disco ni interfaz      curvas y setup        |
 y Static                                                          +--> Interfaz local (hilo 4): pit wall en pantalla
                                                                   |
                                                                   +--> Servidor (hilo 5, opcional, apagado por defecto)
                                                                        móvil, tableta u otro PC en la red local
```

La lectura entrega muestras al análisis; escritura, interfaz y servidor cuelgan del análisis, así que ninguno puede frenar la lectura. La interfaz y el servidor solo leen el último estado calculado.

### Hilos

1. **Lectura**: lee Physics, Graphics y Static, detecta muestras nuevas con el identificador de paquete y las deja en una cola sin bloqueos.
2. **Análisis**: detecta vueltas y sectores, remuestrea por posición normalizada y calcula delta y métricas.
3. **Escritura**: guarda vueltas en disco y datos en SQLite desde su propia cola.
4. **Interfaz**: dibuja el pit wall local y solo lee el último estado calculado.
5. **Servidor (opcional)**: envía el estado a otros dispositivos. Apagado por defecto y escuchando solo en la red local.

### Stack propuesto

| Parte | Opción | Motivo |
| --- | --- | --- |
| Lectura | API de Windows para memoria compartida | Sin dependencias, muy ligera |
| Compilación | CMake con Visual Studio | Estándar, y Claude Code lo maneja bien |
| Interfaz local | Dear ImGui sobre DirectX 11 | Muy rápida, pensada para paneles de telemetría |
| Servidor | cpp-httplib o Crow con WebSocket | Librería pequeña; se decide en la Fase 10 |
| Histórico | SQLite para metadatos y vueltas, más archivos binarios comprimidos | Una sola dependencia y sin servidor de base de datos |
| Compresión | LZ4 o Zstandard | Se decide en la Fase 3 midiendo CPU y tamaño |
| Pruebas | doctest o Catch2 | Para probar el análisis con vueltas grabadas, sin abrir el juego |

### Datos en disco

- **Vuelta cruda comprimida**: todas las muestras a la frecuencia de lectura, por si quieres reanalizar.
- **Vuelta remuestreada**: unos 3.000 puntos por vuelta (cada 1 o 2 m) sobre la posición normalizada, para comparar vueltas entre sí.
- Cada vuelta guarda coche, pista, setup, compuesto y condiciones, para que las comparaciones sean justas.

### Estructura del repositorio

```
ac-telemetry/
  CMakeLists.txt
  CLAUDE.md          reglas y contexto para Claude Code
  ROADMAP.md         copia de este roadmap con las casillas
  docs/              datos-disponibles.md, formato de archivo, decisiones
  src/
    core/            tipos, lector de memoria compartida
    analysis/        vueltas, remuestreo, delta, curvas, setup
    storage/         archivos de vuelta y SQLite
    ui/              pit wall local (ImGui)
    server/          servidor y vista web
    app/             main y arranque de hilos
  tests/
  data/              curvas de Spa, configuración
```

## Riesgos y puntos a validar

El mayor riesgo es que el coche elegido es muy reciente: la Fase 0 existe para comprobar qué datos salen de verdad antes de diseñar sobre ellos.

| Riesgo | Por qué importa | Cómo se resuelve |
| --- | --- | --- |
| El "último" RSS Formula Hybrid es el Formula Hybrid 2026, publicado el 9 de octubre de 2026 | Es un coche con muy poca documentación de la comunidad; el nombre exacto de la carpeta del coche hay que confirmarlo | Fase 0: comprobar la carpeta del coche instalado y la versión |
| ERS propio, aerodinámica activa y perfiles de despliegue eléctrico del RSS 2026 | Pueden no verse en los campos estándar de ERS/KERS de la memoria compartida | Fase 0: volcado de datos real; si faltan canales, se ajusta la Fase 9 |
| El coche incluye sus propias apps (RSS Control Centre, Race Lab y Engineer) | Race Lab ya analiza la conducción, así que esta app debe aportar algo distinto: delta por curva, histórico y diagnóstico de setup | Revisar Race Lab en la Fase 0 y concretar la diferencia |
| Requiere Custom Shaders Patch 0.2.11 o posterior | Sin el CSP correcto, el coche no funciona completo | Fase 0: comprobar la versión instalada |
| Frecuencia de lectura irregular en Windows | Los temporizadores de Windows no son precisos y se pueden perder o duplicar muestras | Usar el identificador de paquete y medir la frecuencia real (Fase 1) |
| Detección de vueltas con casos límite | Salida de boxes, reset a boxes, teletransportes y vueltas inválidas estropean el delta | Pruebas con datos grabados (Fase 2) |
| Cambiar el setup a mitad de sesión | La mejor vuelta puede ser de un setup anterior y el delta engaña | Etiquetar cada vuelta con su setup y avisar (Fases 2, 4 y 8) |
| El setup no está en la memoria compartida | Sin él no se puede relacionar un síntoma con un ajuste | Leer el archivo de setup (Fase 8) |
| C++ como primer lenguaje | Curva difícil y riesgo de código difícil de mantener | Reglas simples en CLAUDE.md y explicar cada pieza al construirla |
| AC cerrado, pausado o en menú | La memoria compartida existe pero los datos no son válidos | Gestión de estados y reconexión (Fase 1) |

Fuentes: [Traxion, 9 de octubre de 2026](https://traxion.gg/race-sim-studio-releases-2026-spec-formula-1-style-mod-for-assetto-corsa/) para el lanzamiento, las apps incluidas y el requisito de CSP.

## Cómo trabajar con Claude Code

Cada fase se trabaja en pasos pequeños: una tarea, compilar, probar, marcar la casilla y hacer commit.

**Al empezar cada sesión**

1. Pedir a Claude Code que lea CLAUDE.md y la fase en curso del roadmap.
2. Acordar qué tareas de la fase se harán en esa sesión.
3. Pedir que explique cada pieza nueva de C++ en lenguaje sencillo, porque el código lo vas a mantener tú.

**Reglas para el CLAUDE.md del repositorio**

- Objetivo y decisiones de este documento, en un párrafo corto
- Cómo compilar y cómo ejecutar las pruebas
- C++ simple: pocas dependencias, sin plantillas complicadas, nombres claros y comentarios donde la intención no sea obvia
- El hilo de lectura nunca espera a la interfaz, al disco ni a la red
- Una tarea por commit, con mensaje claro; no mezclar fases
- No cambiar el formato de archivo de vuelta sin subir su versión

**Cómo marcar el progreso**

- Una tarea se marca solo cuando está hecha y probada, no cuando está escrita.
- Una fase se da por terminada cuando todos sus criterios de terminado están marcados; después, un commit etiquetado como `fase-N`.
- Mantén una sola copia viva del roadmap: o el documento original, o este ROADMAP.md del repositorio. Si Claude Code va a marcar las casillas, la copia viva debe ser esta.

## Progreso global

Marca cada fase solo cuando todos sus criterios de terminado estén marcados. El MVP se cierra con la Fase 5.

- [ ] Fase 0: Preparación del entorno y validación de datos
- [ ] Fase 1: Lector de memoria compartida en consola
- [ ] Fase 2: Vueltas, sectores y sesión
- [ ] Fase 3: Grabación de vueltas a disco
- [ ] Fase 4: Remuestreo y delta contra la mejor vuelta
- [ ] Fase 5: Interfaz local, pit wall básico (hito MVP)
- [ ] Fase 6: Histórico en SQLite
- [ ] Fase 7: Análisis por curva y diagnóstico de conducción
- [ ] Fase 8: Lectura del setup y diagnóstico del setup
- [ ] Fase 9: Pit wall completo, estrategia y ERS
- [ ] Fase 10: Servidor local y vista web
- [ ] Fase 11: Rendimiento, estabilidad y empaquetado

## Fase 0: Preparación y validación de datos

**Objetivo:** dejar el entorno listo y confirmar, con el coche y la pista reales, qué datos salen de la memoria compartida antes de diseñar nada. **Depende de:** nada.

**Qué cubre**

- [ ] Instalar Visual Studio con la carga de trabajo de C++ para escritorio, CMake y Git
- [ ] Crear el repositorio, el .gitignore y un CMakeLists mínimo que compile un programa de prueba
- [ ] Escribir el CLAUDE.md con objetivo, decisiones y reglas (sección Cómo trabajar con Claude Code)
- [ ] Confirmar la instalación: AC original, Custom Shaders Patch 0.2.11 o posterior, RSS Formula Hybrid (nombre exacto de la carpeta del coche y versión) y la pista Spa-Francorchamps
- [ ] Escribir un programa mínimo que vuelque los tres bloques de memoria compartida a un archivo mientras das 2 o 3 vueltas en Spa
- [ ] Revisar el volcado: qué campos cambian, cuáles están siempre a cero y cómo se ven ERS, DRS y aerodinámica activa
- [ ] Probar las apps incluidas con el coche (Control Centre y Race Lab) para saber qué hacen ya y qué aportará esta app
- [ ] Localizar la carpeta de setups del coche y pista, y anotar la ruta
- [ ] Documentar los hallazgos en docs/datos-disponibles.md, con una tabla de canal, disponible (sí/no) y observaciones

**Qué se espera al terminar**

- [ ] El proyecto compila y está subido al repositorio
- [ ] Existe un volcado real de al menos 3 vueltas en Spa con el RSS Formula Hybrid, guardado para usarlo como datos de prueba
- [ ] Sabes qué canales de ERS y aerodinámica activa existen y cuáles no, y el roadmap se ha ajustado si falta alguno
- [ ] Está decidido el nombre exacto del coche y la versión de CSP
- [ ] Está anotada la diferencia entre esta app y Race Lab

## Fase 1: Lector de memoria compartida en consola

**Objetivo:** leer en vivo los tres bloques de memoria compartida desde un hilo propio y mostrarlos en consola. **Depende de:** Fase 0.

**Qué cubre**

- [ ] Definir en C++ las estructuras de Physics, Graphics y Static con el mismo orden y empaquetado que AC, y comprobar sus tamaños en tiempo de compilación
- [ ] Abrir los tres mapeos de memoria con la API de Windows y cerrarlos limpiamente
- [ ] Convertir las cadenas de texto del juego (caracteres anchos) a UTF-8
- [ ] Bucle de lectura en un hilo propio, con detección de muestra nueva por el identificador de paquete
- [ ] Gestionar estados: AC cerrado, en menú, en pausa o en replay, con reconexión automática
- [ ] Mostrar en consola velocidad, marcha, RPM, pedales, G, presiones y temperaturas de neumáticos, combustible, ERS y tiempo de vuelta
- [ ] Medir la frecuencia real de lectura y contar las muestras perdidas o duplicadas

**Qué se espera al terminar**

- [ ] Los datos en consola coinciden con lo que ves en el juego
- [ ] La app se reconecta sola si cierras y abres AC
- [ ] La frecuencia de lectura está medida y anotada, sin muestras duplicadas
- [ ] El consumo de CPU del lector está medido y es bajo (objetivo inicial: menos del 1 % de un núcleo)

## Fase 2: Vueltas, sectores y sesión

**Objetivo:** convertir el flujo de muestras en vueltas completas, con tiempos, sectores y validez fiables. **Depende de:** Fase 1.

**Qué cubre**

- [ ] Modelo de sesión: coche, pista, fecha y condiciones (temperatura del aire y de la pista, agarre)
- [ ] Detectar inicio y fin de vuelta con las vueltas completadas y la posición normalizada (cruce de la línea de meta)
- [ ] Registrar sectores y tiempos parciales
- [ ] Clasificar cada vuelta: válida, inválida (salida de pista o penalización), vuelta de salida de boxes o vuelta de entrada a boxes
- [ ] Etiquetar cada vuelta con su setup (de momento, una etiqueta manual de sesión; en la Fase 8, una huella del archivo de setup)
- [ ] Calcular la mejor vuelta válida de la sesión
- [ ] Cubrir casos límite: reset a boxes, teletransporte, primera vuelta y reinicio de sesión
- [ ] Escribir pruebas automáticas usando el volcado de la Fase 0

**Qué se espera al terminar**

- [ ] Una sesión en Spa genera una lista de vueltas con tiempo, sectores y validez
- [ ] Los tiempos de la app coinciden con los que muestra el juego
- [ ] Las vueltas de salida y de entrada a boxes nunca se usan como referencia
- [ ] Las pruebas automáticas pasan sin abrir el juego

## Fase 3: Grabación de vueltas a disco

**Objetivo:** guardar cada vuelta sin perder muestras y poder reabrirla después, sin el juego. **Depende de:** Fase 2.

**Qué cubre**

- [ ] Diseñar el formato de archivo binario por vuelta: cabecera con versión del formato, coche, pista, setup, número de vuelta y tiempo, seguida de las muestras
- [ ] Elegir qué canales se guardan y con qué precisión (decimales de 32 bits salvo que haga falta más)
- [ ] Hilo escritor con cola, para que el hilo de lectura nunca espere al disco
- [ ] Elegir la compresión (LZ4 o Zstandard) midiendo CPU y tamaño
- [ ] Lector de archivos para reanalizar vueltas sin el juego (modo offline)
- [ ] Definir dónde se guardan los datos, cuánto ocupan y cómo se limpian
- [ ] Documentar el formato en docs/ para poder cambiarlo con control de versiones

**Qué se espera al terminar**

- [ ] Cada vuelta queda guardada y, al leerla de nuevo, es idéntica a la original
- [ ] Está medido y anotado el tamaño de una hora de sesión
- [ ] No se pierden muestras mientras se guarda
- [ ] El modo offline funciona con AC cerrado
- [ ] El formato está documentado y lleva número de versión

## Fase 4: Remuestreo y delta

**Objetivo:** poner todas las vueltas en un eje común (la posición en pista) para compararlas y saber, punto a punto, dónde ganas o pierdes tiempo. **Depende de:** Fase 3.

**Qué cubre**

- [ ] Remuestrear cada vuelta por posición normalizada, cada 1 o 2 m (unos 3.000 puntos por vuelta, según la longitud de la pista)
- [ ] Interpolar los canales continuos; para la marcha usar el valor más cercano y para los booleanos el último válido
- [ ] Definir la vuelta de referencia como la mejor vuelta válida de la sesión y actualizarla cuando se supere
- [ ] Calcular el delta continuo (tiempo acumulado frente a la referencia) a lo largo de la vuelta
- [ ] Calcular el delta por sector y el proyectado a final de vuelta
- [ ] Avisar cuando la referencia y la vuelta actual tienen setups distintos
- [ ] Comparar canales alineados por posición: frenada, velocidad y acelerador
- [ ] Pruebas automáticas con vueltas grabadas

**Qué se espera al terminar**

- [ ] Al acabar una vuelta, la app muestra el delta y los puntos donde ganas o pierdes más
- [ ] El delta final coincide con la diferencia real de tiempos de vuelta, con una tolerancia definida en las pruebas
- [ ] El cálculo en tiempo real no afecta a la lectura (medido)
- [ ] La referencia cambia sola al mejorar la mejor vuelta

## Fase 5: Interfaz local, pit wall básico (hito MVP)

**Objetivo:** ver el estado de tu coche y tu delta en una ventana mientras conduces, sin perjudicar al juego. **Depende de:** Fase 4.

**Qué cubre**

- [ ] Ventana con Dear ImGui sobre DirectX 11, con el dibujo en un hilo independiente
- [ ] Panel en vivo: velocidad, marcha, RPM, pedales, delta, tiempo de vuelta y sectores
- [ ] Gráfica de la vuelta actual frente a la referencia (velocidad y pedales por posición)
- [ ] Mapa de pista sencillo a partir de las coordenadas XYZ, con la posición del coche
- [ ] Lista de vueltas de la sesión con tiempo y validez
- [ ] Configuración mínima: unidades, canales visibles y ventana siempre visible
- [ ] Medir el impacto en los FPS del juego, con y sin la app

**Qué se espera al terminar**

- [ ] Puedes conducir en Spa con la app abierta y ver el delta en vivo
- [ ] La diferencia de FPS del juego con y sin la app está medida y anotada
- [ ] La interfaz no bloquea ni retrasa la lectura de datos
- [ ] Hito MVP: primera versión utilizable, etiquetada como `v0.1`

## Fase 6: Histórico en SQLite

**Objetivo:** conservar todas las sesiones y poder compararlas a lo largo del tiempo. **Depende de:** Fase 3 (puede hacerse en paralelo a la Fase 5).

**Qué cubre**

- [ ] Diseñar el esquema: sesiones, vueltas, setups, pistas, coches y referencias a los archivos de vuelta
- [ ] Guardar automáticamente al cerrar cada vuelta y cada sesión, desde el hilo escritor
- [ ] Versionar el esquema y escribir migraciones
- [ ] Calcular la mejor vuelta histórica por coche, pista y compuesto, como referencia opcional además de la mejor vuelta de la sesión
- [ ] Pantalla de histórico: sesiones, vueltas y filtros
- [ ] Comparar dos vueltas cualesquiera de sesiones distintas, con aviso si el setup o las condiciones no son comparables
- [ ] Exportar a CSV

**Qué se espera al terminar**

- [ ] Al cerrar AC y volver a abrir la app, las sesiones anteriores siguen disponibles
- [ ] Se puede elegir como referencia la mejor vuelta histórica
- [ ] Una actualización de la app no rompe las bases de datos antiguas
- [ ] La exportación a CSV abre correctamente en una hoja de cálculo

## Fase 7: Análisis por curva y diagnóstico de conducción

**Objetivo:** pasar de "pierdo 0,4 s en esta vuelta" a "pierdo 0,15 s en Pouhon por frenar pronto". **Depende de:** Fases 4 y 6.

**Qué cubre**

- [ ] Definir las curvas de Spa en un archivo de datos, con inicio, apex y fin por posición normalizada (por ejemplo La Source, Eau Rouge y Raidillon, Pouhon, Blanchimont y Bus Stop)
- [ ] Dividir cada curva en entrada, apex y salida
- [ ] Métricas por curva: punto de frenada, presión máxima de freno, velocidad mínima, punto de aplicación del acelerador y tiempo perdido frente a la referencia
- [ ] Trail braking: solape entre freno y volante
- [ ] Correcciones de volante y ruido del acelerador como medida de inestabilidad
- [ ] Cambios de marcha: RPM de cambio y tiempo perdido
- [ ] Ranking de las curvas donde más tiempo pierdes, con la causa probable
- [ ] Informe por vuelta y por sesión, visible en la interfaz

**Qué se espera al terminar**

- [ ] Tras una sesión, la app señala las tres curvas donde más tiempo pierdes, con el número y la causa
- [ ] Las métricas por curva se calculan igual con datos de la sesión actual y con datos del histórico
- [ ] Las curvas se pueden editar sin recompilar
- [ ] Un conjunto de vueltas de prueba tiene resultados verificados a mano

## Fase 8: Lectura del setup y diagnóstico del setup

**Objetivo:** relacionar los síntomas que muestran los datos con el setup con el que se hicieron. **Depende de:** Fases 4 y 6.

**Qué cubre**

- [ ] Leer el archivo de setup del coche y pista desde la ruta anotada en la Fase 0
- [ ] Calcular una huella del setup y asociarla a cada vuelta; detectar cuándo cambia
- [ ] Subviraje y sobreviraje: diferencia de deslizamiento entre eje delantero y trasero en entrada, apex y salida
- [ ] Equilibrio de cargas verticales y transferencia de peso
- [ ] Ventana de neumáticos: temperaturas interior, media y exterior, y presión en caliente frente a la objetivo
- [ ] Bloqueos de frenada, comparando deslizamiento delantero y trasero al frenar
- [ ] Contacto con el suelo (bottoming) y comportamiento de la plataforma aerodinámica
- [ ] Patinaje en salida de curva, por marcha
- [ ] Comparar dos setups: vueltas, métricas y diferencias entre archivos de setup
- [ ] Informe de setup con síntomas y evidencia (gráficas), sin recomendaciones automáticas

**Qué se espera al terminar**

- [ ] Cada vuelta guardada indica con qué setup se hizo
- [ ] La app compara dos setups mostrando qué cambió en el archivo y qué cambió en los datos
- [ ] El informe señala síntomas con evidencia que se pueda comprobar a mano
- [ ] Está decidido si, y cómo, se añaden recomendaciones de cambios para el RSS (queda para después)

## Fase 9: Pit wall completo, estrategia y ERS

**Objetivo:** convertir el panel básico en un pit wall que ayude a tomar decisiones de carrera con tu propio coche. **Depende de:** Fases 5 y 6; el alcance de ERS y aerodinámica depende de lo hallado en la Fase 0.

**Qué cubre**

- [ ] Combustible: consumo por vuelta y vueltas restantes
- [ ] Neumáticos: degradación (tiempo perdido por vuelta frente a desgaste), temperaturas y presiones frente a la ventana objetivo
- [ ] Ritmo por stint y consistencia (desviación típica de las vueltas válidas)
- [ ] ERS: nivel de batería, despliegue, recuperación y perfil activo, con los canales que existan realmente
- [ ] Aerodinámica activa y DRS, si el coche los expone
- [ ] Daños y estado de frenos
- [ ] Ventana de parada y tiempo de pit
- [ ] Alertas configurables: combustible, temperatura de frenos y neumáticos fuera de ventana
- [ ] Paneles personalizables: elegir qué se ve y dónde

**Qué se espera al terminar**

- [ ] En una carrera de varias vueltas, la app indica con qué vuelta termina el combustible y cómo evolucionan los neumáticos
- [ ] Los datos de ERS mostrados coinciden con lo que muestra el coche
- [ ] Las alertas se disparan en los casos de prueba
- [ ] Los canales que no existen en el coche están documentados y no aparecen en la interfaz

## Fase 10: Servidor local y vista web

**Objetivo:** ver el pit wall desde otro dispositivo (móvil, tableta u otro PC) sin cargar al PC del juego. **Depende de:** Fase 9.

**Qué cubre**

- [ ] Elegir la librería de servidor (cpp-httplib o Crow) y comprobar que añade poca carga
- [ ] Servidor HTTP con WebSocket que envía el último estado calculado, con una frecuencia menor que la de lectura
- [ ] Apagado por defecto, con interruptor en la interfaz
- [ ] Escucha solo en la red local y con una contraseña o token sencillo
- [ ] Documentar la regla del firewall de Windows necesaria
- [ ] Página web ligera con los mismos paneles principales
- [ ] Reconexión automática del navegador si se corta la conexión
- [ ] Probar en móvil y tableta, con varios dispositivos conectados

**Qué se espera al terminar**

- [ ] Un móvil en la misma red ve el pit wall en vivo
- [ ] Con el servidor encendido, la bajada de FPS del juego está medida y es aceptable
- [ ] Con el servidor apagado no hay ningún puerto abierto
- [ ] Sin la contraseña correcta no se accede a los datos

## Fase 11: Rendimiento, estabilidad y empaquetado

**Objetivo:** que la app sea ligera, estable en sesiones largas y fácil de instalar. **Depende de:** Fases 1 a 10.

**Qué cubre**

- [ ] Perfilar CPU, memoria y uso de disco en una sesión real
- [ ] Prueba de estabilidad de una hora o más, con el servidor encendido y la grabación activa
- [ ] Registro de errores (log) y mensajes claros cuando falla algo
- [ ] Pruebas de regresión con vueltas grabadas
- [ ] Revisar y reducir dependencias
- [ ] Empaquetado: carpeta portable o instalador, con la configuración por defecto
- [ ] README de usuario: instalación, primer uso y solución de problemas
- [ ] Versionado y notas de cada versión

**Qué se espera al terminar**

- [ ] La app funciona una hora o más sin errores, fugas de memoria ni muestras perdidas
- [ ] Están anotados los consumos de CPU, memoria y disco, y el impacto en los FPS
- [ ] Se puede instalar en otro PC con Windows siguiendo solo el README
- [ ] Versión `v1.0` etiquetada

## Futuro

Nada de esto entra en las fases 0 a 11; se recoge para dejar la arquitectura preparada.

- [ ] **Modo coach**: carreras con otra persona o con un coach, con comparación de vueltas entre dos pilotos y vista del coach. Requiere el servidor de la Fase 10 y decidir cómo se obtienen los datos del otro piloto.
- [ ] **Módulo Yuko**: aparcado a petición del autor; sin diseño ni trabajo previsto.
- [ ] **Recomendaciones de setup**: reglas específicas del RSS Formula Hybrid, una vez que el diagnóstico de la Fase 8 sea fiable.
- [ ] **Otros coches y pistas**: parametrizar curvas y canales para no depender de Spa ni del RSS.
- [ ] **Telemetría de rivales**: solo si hay una fuente de datos fiable.
- [ ] **ACC y AC Evo**: requerirían un lector distinto, porque sus datos son otros.

## Preguntas abiertas

Cada una se cierra en la fase indicada; anota la respuesta aquí cuando se decida.

- [ ] ¿Es el Formula Hybrid 2026 el coche exacto y cuál es el nombre de su carpeta? (Fase 0)
- [ ] ¿Qué canales de ERS y aerodinámica activa expone realmente el RSS 2026? (Fase 0)
- [ ] ¿Qué aporta esta app frente a RSS Race Lab? (Fase 0)
- [ ] ¿Cómo se llamará la aplicación? (antes de crear el repositorio)
- [ ] ¿El repositorio será público o privado? (Fase 0)
- [ ] ¿LZ4 o Zstandard para comprimir las vueltas? (Fase 3)
- [ ] ¿Las curvas de Spa se definen a mano o se detectan automáticamente? (Fase 7)
- [ ] ¿El pit wall se verá en un segundo monitor, en otro PC o en el móvil? (Fases 5 y 10)
- [ ] ¿Cuál es la bajada máxima de FPS aceptable con la app abierta? (Fase 5)
