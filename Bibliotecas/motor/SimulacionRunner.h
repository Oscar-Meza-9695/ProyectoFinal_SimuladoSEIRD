#pragma once
#include "ModeloSEIRD.h"
#include <vector>
#include <string>

/*
Orquesta la simulación del modelo SEIRD.

Responsabilidades:
    1. Ejecuta el loop de integracion llamado EDOResolve::pasoRK4 para avanzar el estado del sistema en el tiempo.
    2.Aplicar la condiccion de parada doble:
        -Natural: Cuando el número de infectados llega a cero. -> E + I < 1.0
        -Forzada: Cuando se alcanza un tiempo máximo definido por el usuario. -> t >= T_MAX 
    3. Almacenar el historial de estados para su posterior análisis y visualización.
    4. Exportar los resultados a CSV

Flujo:
    SimulacionRunner runner;
    auto history = runner.run(modelo);
    runner.exportsCSV(historial, "Salida.csv")
*/

// Snapshop de un instante en la simulacion
struct PasoSimulacion
{
    double dia;     //Dias de la simulacion (Puede ser fraccionario por dt)
    double S;       //Suceptibles
    double E;       // Expuestos
    double I;       // Infecciosos
    double R;       // Recuperados
    double D;       // Fallecidos
    double N;       // Poblacion total (constante, Util para normalizar en UI)
};

class SimulacionRunner
{
public:
    //Configuracion del runner
    static constexpr double DT      = 0.1;    // Paso de tiempo en di­as
    static constexpr double T_MAX   = 730.0;  // Tope de seguridad (2 años)
    static constexpr double EI_MIN  = 1.0;    // Umbral de extincion (E+I < 1)

    /**
     * Ejecuta la simulacion completa.
     *
     * @param model  Modelo SEIRD configurado con parametros de usuario
     * @return       Vector con el estado del sistema en cada paso dt
     */
    std::vector<PasoSimulacion> run(const ModeloSEIRD& model) const;

    /**
     * Exporta el historial de simulacion a un archivo CSV.
     *
     * Columnas: day, S, E, I, R, D, N
     *
     * @param history   Resultado de run()
     * @param filepath  Ruta del archivo de salida (ej. "output/results.csv")
     * @return          true si la exportacion fue exitosa
     */
    bool exportCSV(const std::vector<PasoSimulacion>& history, const std::string& filepath) const;
};