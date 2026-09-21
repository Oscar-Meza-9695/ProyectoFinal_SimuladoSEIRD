# ProyectoFinal_SimuladoSEIRD
Simulador del sarampión con el modelo SEIRD resuelto con Runge-Kutta de 4.º orden, en C++ y CMake.
# Simulador SEIRD del Sarampión

Simulación de la propagación del sarampión en una población con parámetros epidemiológicos configurables. Proyecto de Integración de la Licenciatura en Ingeniería en Computación, UAM Azcapotzalco (trimestre 26-P).

El modelo compartimental **SEIRD** se resuelve con el método numérico de **Runge-Kutta de 4.º orden (RK4)**, implementado en **C++17**, compilado con **CMake** y visualizado en tiempo real con **SFML**.

## El modelo

| Símbolo | Compartimento | Descripción |
|---|---|---|
| S | Susceptibles | Sanos, sin inmunidad |
| E | Expuestos | En incubación, aún no contagian |
| I | Infecciosos | Contagian a otros |
| R | Recuperados | Inmunes (incluye vacunados previos) |
| D | Decesos | Fallecidos por la enfermedad |

```
dS/dt = -β·S·I/N
dE/dt =  β·S·I/N - σ·E
dI/dt =  σ·E - (γ + μ)·I
dR/dt =  γ·I
dD/dt =  μ·I
```

### Parámetros epidemiológicos (fijos, de la literatura)

| Parámetro | Valor |
|---|---|
| R₀ | 15 |
| Periodo de incubación (1/σ) | 10 días |
| Periodo infeccioso (1/γ) | 8 días |
| Letalidad (CFR) | 0.15 % |
| Eficacia de la vacuna | 97 % |

Los parámetros derivados (β, σ, γ, μ) los calcula el motor a partir de estos valores.

### Parámetros configurables (desde la interfaz)

| Parámetro | Rango |
|---|---|
| Población total (N) | 10,000 – 10,000,000 |
| Cobertura vacunal | 0 – 100 % |
| Infectados iniciales (I₀) | 1 – 100 |

## Características

- Integrador RK4 con paso fijo `dt = 0.1` días. Conservación de la población total con error < 10⁻⁶.
- El brote termina cuando E + I < 1 o al llegar a 730 días.
- Gráfica de curvas S, E, I, R, D que se dibuja progresivamente.
- 300 agentes representativos animados (1 punto = N/300 personas), coloreados según el estado del modelo.
- Controles de replay: play/pausa y velocidad.
- Exportación automática a `output.csv` (un registro por día) al terminar cada simulación.
- El motor matemático no depende de SFML, así que puede compilarse y validarse por separado.

## Resultados de validación

| Escenario | Cobertura | Rₜ inicial | Pico de infecciosos | Tasa de ataque | Fallecidos |
|---|---|---|---|---|---|
| A: N = 1,000,000 | 85 % | 2.63 | día 176 | 90.8 % de S₀ | 238.7 |
| B: N = 10,000,000 | 50 % | 7.72 | día 85 | 99.96 % de S₀ | ~7,000 |

Con 85 % de cobertura el brote igual ocurre, porque la inmunidad efectiva (85 % × 97 % = 82.45 %) es menor que el umbral de inmunidad de rebaño (1 − 1/R₀ = 93.3 %).

## Estructura del proyecto

```
├── CMakeLists.txt
├── Bibliotecas/motor/          # Cabeceras
│   ├── ModeloSEIRD.h           # Parámetros y derivadas
│   ├── EDOResolve.h            # Integrador RK4
│   ├── SimulacionRunner.h      # Loop de simulación + CSV
│   └── ui/                     # Ventana, paneles y sliders (SFML)
└── src/
    ├── main.cpp
    ├── motor/
    └── ui/
```

## Requisitos

- Compilador C++17 (desarrollado con MinGW-W64 GCC 13.1.0)
- CMake 3.16 o superior
- SFML 2.6.2

## Compilación y ejecución

```bash
cmake -B build
cmake --build build
```

Ejecuta el programa desde la carpeta `build/`. El archivo `output.csv` se genera en el directorio desde donde lo ejecutes.

> La interfaz carga la fuente `C:/Windows/Fonts/arial.ttf`, por lo que por ahora está pensada para Windows.

## Autor

Oscar Meza Galicia
