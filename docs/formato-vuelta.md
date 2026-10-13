# Formato del archivo de vuelta

Estado: **borrador**. El diseño definitivo es la tarea 1 de la Fase 3; aquí solo se fijan las
reglas y se deja el hueco para la especificación, para no improvisarla al llegar.

## Reglas (no negociables)

1. **Todo archivo de vuelta lleva número de versión de formato en la cabecera.** Sin excepción.
2. **No se cambia el formato sin subir el número de versión** y anotarlo aquí, en el historial
   del final de este documento.
3. El lector tiene que poder abrir archivos de versiones anteriores, o rechazarlos con un
   mensaje claro. Nunca debe interpretar mal un archivo antiguo.
4. El formato se documenta **antes** de escribir el código que lo genera.

## Qué tiene que guardar cada vuelta (borrador)

Según el roadmap, cada vuelta guarda coche, pista, setup, compuesto y condiciones, para que las
comparaciones entre vueltas sean justas.

**Cabecera**

| Campo | Para qué |
| --- | --- |
| Versión del formato | Poder cambiar el formato con control |
| Coche (`carName`) y pista (`trackName` + configuración) | Comparar solo lo comparable |
| Setup (etiqueta manual en la Fase 2; huella del archivo en la Fase 8) | Detectar que la mejor vuelta es de otro setup |
| Compuesto de neumático | La degradación depende de él |
| Condiciones: temperatura de aire y pista, agarre | Dos vueltas con distinta temperatura no se comparan igual |
| Número de vuelta y tiempo de vuelta | Identificarla |
| Frecuencia de muestreo | Poder reanalizar |

**Muestras**

Dos versiones de la misma vuelta, según el roadmap:

- **Cruda comprimida**: todas las muestras a la frecuencia de lectura. Sirve para reanalizar.
- **Remuestreada**: unos 3.000 puntos por vuelta (cada 1 o 2 m) sobre la posición normalizada.
  Es la que se usa para comparar vueltas entre sí (Fase 4).

Precisión: decimales de 32 bits (`float`) salvo que algún canal necesite más.

## Decisiones pendientes

- Compresión: LZ4 o Zstandard. Se decide en la Fase 3 midiendo CPU y tamaño.
- Qué canales exactos se guardan y cuáles se pueden recalcular al leer.
- Dónde se guardan los datos, cuánto ocupan y cómo se limpian.
- Si la vuelta remuestreada se guarda o se recalcula al leer (afecta al tamaño en disco).

## Historial de versiones del formato

| Versión | Fecha | Cambio |
| --- | --- | --- |
| — | — | Todavía no existe el formato |
