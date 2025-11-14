# Entregable 1: Makefiles y ccache

## Información del Proyecto

- **Nombre del proyecto:** MasterPiece  
- **Equipo:** Adrián, Luis, Esteban, Marcos  
- **Fecha:** [Fecha de entrega]

---

## 1. Estructura del Proyecto

El proyecto sigue una estructura multidirectorio con las siguientes carpetas principales:

```
src/
├── core/         # Clases fundamentales (GameState, main y StateMachine)
└── states/       # Estados del juego (GameOverState, InicioState, MainGameState, PersonajesState)
```

### Estrategia de Compilación Multidirectorio

Se ha implementado una búsqueda recursiva de archivos fuente mediante las siguientes líneas en el Makefile:

```makefile
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
```

Esta estrategia permite añadir nuevos directorios y archivos sin modificar el Makefile, facilitando la escalabilidad del proyecto.

---

## 2. Análisis de Compilación Paralela

### 2.1. Tiempos de compilación con diferentes valores de -jN

| Número de hilos (-jN) | Tiempo de compilación |
|------------------------|----------------------|
| -j1                    | 0:02.76              |
| -j2                    | 0:01.18              |
| -j4                    | 0:00.77              |
| -j6                    | 0:00.67              |
| -j8                    | 0:00.60              |
| -j12                   | 0:00.62              |
| -j14                   | 0:00.67              |

**Comando utilizado para mediciones:**

```bash
time make -jN
```

### 2.2. Análisis de resultados

Se observa una mejora notable en el tiempo de compilación al aumentar el número de hilos, especialmente hasta `-j8`.  
A partir de este valor, el beneficio adicional se vuelve marginal, e incluso el tiempo aumenta ligeramente con `-j12` y `-j14`.  
Esto indica que el límite de paralelización efectiva del sistema se alcanza alrededor de los **8 hilos**, coincidiendo con el número de núcleos lógicos disponibles.

**Conclusión:**  
El valor óptimo de paralelismo para este proyecto se sitúa entre **-j8 y -j12**, proporcionando el mejor equilibrio entre rendimiento y estabilidad.

---

## 3. Errores en Compilación Paralela

### 3.1. Pruebas realizadas

Se realizaron múltiples compilaciones paralelas utilizando diferentes niveles de concurrencia:

```bash
make clean && make -jN
```

### 3.2. Resultados

- **Estado:** ✓ Sin errores detectados  
- **Descripción:** Todas las compilaciones finalizaron correctamente en paralelo.

El Makefile gestiona correctamente las dependencias entre archivos fuente.  
La regla de patrón utilizada:

```makefile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
```

garantiza que se creen los directorios necesarios y evita condiciones de carrera en compilaciones concurrentes.

**Conclusión:**  
El sistema de compilación es estable y soporta sin problemas la ejecución en paralelo.

---

## 4. Compilación con ccache

### 4.1. Instalación y configuración

```bash
sudo apt install ccache
ccache --version
```

Integración en el Makefile:

```makefile
CXX := ccache g++
```

### 4.2. Comparación de tiempos

| Escenario                       | Tiempo de compilación |
|---------------------------------|-----------------------|
| Sin ccache (compilación normal) | 0:00.67               |
| Con ccache (primera vez)        | 0:01.00               |
| Con ccache (segunda vez)        | 0:00.37               |

**Comandos utilizados:**

```bash
# Sin ccache
make clean && time make -j4

# Con ccache (primera compilación)
ccache -C && make clean && time make -j4

# Con ccache (segunda compilación)
make clean && time make -j4
```

**Análisis:**  
La primera compilación con `ccache` es ligeramente más lenta debido a la creación inicial de la caché.  
En la segunda ejecución, el tiempo se reduce en torno a un **45%**, demostrando la eficacia de `ccache` para acelerar compilaciones repetidas sin cambios en el código.

---

## 5. Análisis de Estadísticas de ccache

### 5.1. Limpieza inicial

```bash
ccache -C
ccache -z
```

### 5.2. Primera compilación (poblar cache)

```bash
make clean && make -j4
ccache -s
```

**Estadísticas primera ejecución:**

```
Summary:
  Hits:             189 /  406 (46.55 %)
  Misses:           217
  Uncacheable:       58
Primary storage:
  Cache size (GB): 0.00 / 5.00 (0.00 %)
  Cleanups:         300
```

### 5.3. Segunda compilación (usar cache)

```bash
make clean && make -j4
ccache -s
```

**Estadísticas segunda ejecución:**

```
Summary:
  Hits:             196 /  413 (47.46 %)
  Misses:           217
  Uncacheable:       59
Primary storage:
  Cache size (GB): 0.00 / 5.00 (0.00 %)
  Cleanups:         300
```

### 5.4. Análisis de resultados

- **Primera compilación:** La mayoría de los archivos fueron “miss” al no existir aún en la caché.  
- **Segunda compilación:** Aumenta el porcentaje de *hits*, confirmando que los objetos compilados se reutilizan correctamente.  
- **Eficiencia:** El rendimiento mejora con el uso sucesivo de `ccache`, reduciendo tiempos y carga de CPU.  

---

## 6. Conclusiones

1. **Compilación paralela:** La mejora es notable hasta `-j8`; más allá de ese valor el rendimiento se estabiliza.  
2. **Estabilidad:** No se produjeron errores de compilación en paralelo.  
3. **ccache:** Proporciona un ahorro significativo de tiempo en recompilaciones, especialmente útiles en entornos de desarrollo continuo.  
4. **Makefile genérico:** La detección automática de archivos facilita la ampliación del proyecto sin cambios manuales en el Makefile.  

---

## 7. Comandos de Compilación

```bash
# Compilación normal
make

# Compilación paralela (recomendado)
make -j8

# Compilación con ccache
make USE_CCACHE=1 -j8

# Limpiar proyecto
make clean

# Ver información del proyecto
make info
```

---

**Nota:** Todos los tiempos fueron medidos en un entorno **Ubuntu 22.04.5 LTS** con un procesador **12th Gen Intel(R) Core(TM) i7-12700H (14 núcleos / 20 hilos)** y **16 GB de RAM**.