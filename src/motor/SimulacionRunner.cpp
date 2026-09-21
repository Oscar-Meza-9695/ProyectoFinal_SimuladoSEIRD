#include "motor/EDOResolve.h"
#include "motor/SimulacionRunner.h"
#include <fstream>
#include <iomanip>

//Ejecucion
std::vector<PasoSimulacion> SimulacionRunner::run(const ModeloSEIRD& modelo) const{
    std::vector<PasoSimulacion> historial;
    //Se reserva una estimacion de memoria para (730 dias / 0.1 dt = 7300 paso max)
    historial.reserve(static_cast<size_t>(T_MAX / DT) + 1);

    EstadoSEIRD estado = modelo.estado_inicial;
    
    //Registrar el estado inicial (t=0)
    historial.push_back({estado.t, estado.S, estado.E, estado.I, estado.R, estado.D, modelo.N});

    //Loop de integracion
    while(true){
        //Condicion de parada Natural: Extincion del brote
        if(estado.E + estado.I < EI_MIN)
            break;
        //Condicion de parada por tope de seguridad
        if(estado.t >= T_MAX)
            break;
        //Avanzar un paso de RK4
        estado = EDOResolve::pasoRK4(modelo, estado, DT);

        //Guardar snaphost
        historial.push_back({estado.t, estado.S, estado.E, estado.I, estado.R, estado.D, modelo.N});
    }

    return historial;
}

//Exportacion CSV()
bool SimulacionRunner::exportCSV(const std::vector<PasoSimulacion>& historial, const std::string& filepath) const{
    std:: ofstream file(filepath);
    if(!file.is_open())
        return false;
    //Cabecera
    file << "dia,S,E,I,R,D,N\n";
    //Fiar precision decimal para los valores del modelo
    file << std::fixed << std::setprecision(4);
    //Solo escribir un snaphost por dia entero para mantener el CSV legible
    //La simulacion internamente corre con dt = 0.1;
    double ultimo_dia_escrito = -1.0;

    for(const auto& paso : historial){
        double paso_dia = static_cast<int>(paso.dia);

        if(paso_dia != ultimo_dia_escrito){
            file << paso.dia << ","
                 << paso.S <<","
                 << paso.E <<","
                 << paso.I <<","
                 << paso.R <<","
                 << paso.D <<","
                 << paso.N <<",\n";
            ultimo_dia_escrito = paso_dia;
        }
    }
    file.close();
    return true;
}