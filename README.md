# MASTERPIECE
**¡Extermina a las razas inferiores!**

## CAPÍTULO 1 - GDD

### 1.1. Resumen del juego
Te enfrentas a otro jugador en un duelo a muerte con distintos tipos de armas por turnos, en los que elegirás tu nueva posición y tu próxima trayectoria de disparo prediciendo a tu rival y haciendo jugadas inteligentes o arriesgadas. Con partidas rápidas y mucha tensión.

### 1.2. Género y público objetivo
- **Género:** PvP, duelo por turnos  
- **Enfoque visual:** 2D con sprites varios  
- **Público objetivo:** Jugadores casuales o competitivos interesados en juegos locales de duelo o estrategia rápida

### 1.3. Plataforma y tecnología
- **Plataforma objetivo:** PC  
- **Motor de juego:** Raylib  
- **Lenguaje / herramientas:** C++

### 1.4. Historia y ambientación
- No hay una narrativa profunda: el foco está en la jugabilidad.  
- El entorno puede variar según el mapa o la arena de combate (campo, desierto, montaña, etc.).

### 1.5. Objetivo del jugador
- Vencer al oponente reduciendo su vida a cero mediante disparos precisos y buena estrategia de movimiento.  
- Cada jugador tiene 3 puntos de vida y pierde uno cada vez que recibe un impacto.  
- El primer jugador que pierda todos sus puntos de vida es derrotado.

### 1.6. Mecánicas principales
- **Turnos alternos:**  
  - En su turno, el jugador selecciona una nueva posición dentro de un rango limitado y el ángulo de disparo.  
  - Cuando ambos jugadores confirman sus decisiones, el turno se resuelve simultáneamente.  
- **Movimiento táctico:** Cada jugador solo puede moverse una cierta distancia por turno.  
- **Sistema de disparo:** Los proyectiles siguen trayectorias parabólicas que cambian dependiendo del proyectil usado.  
- **Colisiones:** Si un proyectil impacta en el oponente, se resta vida.  
- **Tipos de arma:** (en desarrollo)  
- **Condición de victoria:** El primer jugador que logre reducir la vida del oponente a 0 gana la partida.

### 1.7. Componentes y sistemas
**HUD**
- Barras de vida de cada jugador.  
- Indicador de turno o fase de resolución.

**Interfaz**
- Menú inicial con opción “Jugar” y “Salir”.  
- Selector de armas (en desarrollo).

**Física**
- Trayectorias de los disparos con gravedad.

**Resolución de turnos**
- Se muestra una animación de ambos jugadores moviéndose y disparando.

**Audio**
- Efectos de disparo e impacto.  
- Música de fondo durante el duelo.

**Arte**
- Personajes y escenario en estilo simple 2D.  
- Animaciones básicas de movimiento y disparo.

### 1.8. Inspiraciones y referencias
- *Worms* (Team17): por su sistema de turnos y disparos estratégicos.

### 1.9. Equipo de desarrollo
- Adrián: Encargado de las mecánicas jugables.
- Marcos: Encargado de los gráficos.
- Esteban: Encargado de los gráficos.
- Luis: Encargado de las físicas.

### 1.10. Planificación inicial
**Versión prototipo (primera entrega):**
- Un escenario básico.  
- Movimiento por turnos y disparo funcional.  
- Detección de impactos y sistema de vida.  
- Interfaz mínima (vida y turno).

**Futuras ampliaciones:**
- Nuevos tipos de armas.  
- Habilidades únicas y personajes.  
- Escenarios más elaborados con obstáculos destructibles.  
- Animaciones y efectos visuales.  
- Modo “mejor de 3” o marcador de victorias.

---

## CAPÍTULO 2 - BUGTRACKING

### 2.1. Elección de Sistema de Bugtracking
Se ha elegido **GitHub Issues** como sistema de bugtracking, ya que se integra de forma predeterminada con el repositorio del proyecto, lo que permite:
- Crear incidencias (issues) directamente vinculadas a commits y ramas  
- Asignar responsables y etiquetas a cada bug  
- Realizar seguimiento del progreso mediante milestones  
- Almacenar los Issues en el propio repositorio sin tener que crear una base de datos

Esta elección resulta especialmente adecuada porque el proyecto se desarrolla y versiona íntegramente en GitHub, facilitando un flujo de trabajo unificado para código, incidencias y releases.

### 2.2. Plantilla Bugtracking
Para simplificar el proceso se ha creado una plantilla predeterminada que se selecciona automáticamente. Esta plantilla debe incluir:
- Una **descripción**, explicando qué ocurre.  
- Los **pasos para reproducir** el bug.  
- El **comportamiento esperado**, es decir, lo que debería ocurrir realmente.  
- La **gravedad** del bug con las etiquetas, o si es una sugerencia.  
- **Responsable** de solucionarlo.

#### Ejemplo de estructura

**Título:** [BUG] Descripción breve del problema

**Descripción**
Explica claramente qué ocurre, en qué contexto y con qué frecuencia.

**Pasos para reproducir**
1. Ir a …
2. Hacer clic en …
3. Observar …

**Comportamiento esperado**
Describe qué debería ocurrir realmente.

**Gravedad / etiquetas**
- Gravedad: `low` | `medium` | `high` | `critical`
- Etiquetas: `bug`, `enhancement`, `ui`, etc.

---

## CAPÍTULO 3 - SISTEMA DE GESTIÓN DE RAMAS

### 3.1. Estrategias Evaluadas
Se analizaron cuatro estrategias de ramas vistas en clase: **GitFlow**, **GitHub Flow**, **Ship/Show/Ask** y **Trunk Based Development**.

- **GitHub Flow:** simple pero sin ramas de integración ni proceso formal de releases.  
- **Ship/Show/Ask:** promueve la autonomía, pero exige experiencia avanzada.  
- **Trunk Based Development:** requiere integraciones diarias y uso de *feature flags*, añadiendo complejidad.  
- **GitFlow:** estructura clara con ramas principales (*main*, *develop*) y de soporte (*feature*, *release*, *hotfix*), ideal para el ritmo del proyecto.

### 3.2. Justificación de GitFlow
- **Alineación con el ciclo de releases:** Las ramas *release* permiten estabilizar versiones, aplicar correcciones y etiquetar entregas sin detener el desarrollo.  
- **Gestión de hotfixes:** Las ramas *hotfix* corrigen errores críticos en producción manteniendo coherencia con *develop*.  
- **Trabajo paralelo y coordinado:** *Develop* centraliza la integración y evita conflictos antes de cada entrega.

### 3.3. Implementación
- **main:** versiones estables y etiquetadas (v1.0, v2.0, etc.)  
- **develop:** integración de características completadas  
- **feature/***: desarrollo de funcionalidades específicas  
- **release/***: preparación y estabilización de entregas  
- **hotfix/***: correcciones urgentes en versiones publicadas

**Flujo de trabajo:**
1. Los desarrolladores crean ramas *feature* desde *develop*.  
2. Al finalizar, integran mediante *pull request*.  
3. Antes de entregar, se genera una rama *release* para ajustes finales.  
4. Las *hotfix* corrigen errores en producción.

### Conclusión
GitFlow equilibra organización, control y flexibilidad. Es la opción más adecuada para un proyecto académico colaborativo, garantizando estabilidad, trazabilidad y buenas prácticas en ingeniería de software.

### Idioma
Para cambiar de idioma y ejecutar desde en source, hacer make clean -> make ->LANGUAGE=en_US ./bin/masterpiece

Idiomas disponibles ahora mismo:Español, inglés y japonés(romanji)
es_ES, en_US, ja_JP respectivamente.