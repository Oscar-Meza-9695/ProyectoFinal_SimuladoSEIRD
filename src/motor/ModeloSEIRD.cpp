#include "motor/ModeloSEIRD.h"

//Constructor
ModeloSEIRD::ModeloSEIRD(double Poblacion, double Cobertura_vacunacion, double Infectados_Iniciales)
: N(Poblacion), Vacunacion(Cobertura_vacunacion), Infectados_Iniciales(Infectados_Iniciales){
    //Calcular parametros derivados
    sigma = 1.0 / Dias_Incubacion;  //tasa E -> I
    gamma = 1.0 / Dias_Infecciosos; //tasa I -> R
    mu = CFR * gamma;               //tasa I -> D
    beta = R0 * gamma;              //tasa de transmision

    //Calcular las condiciones iniciales 
    //Individuos inmunes por la vacunacion antes del brote
    double P_vacunada = N * Vacunacion * Eficacia_Vacuna;
    
    estado_inicial.t = 0.0;
    estado_inicial.I = Infectados_Iniciales;
    estado_inicial.E = 0.0;
    estado_inicial.R = P_vacunada;
    estado_inicial.D = 0.0;
    estado_inicial.S = N - P_vacunada - Infectados_Iniciales;
}

//Derivadas del sistema
EstadoSEIRD ModeloSEIRD::derivadas(const EstadoSEIRD& s) const{
    EstadoSEIRD d;

    double nueva_exposicion = beta * s.S * s.I / N; //Flujo de los S -> E

    d.S = -nueva_exposicion;                        //dS / dt
    d.E = nueva_exposicion - sigma * s.E;           //dE / dt
    d.I = sigma * s.E - gamma * s.I - mu * s.I;     //dI / dt
    d.R = gamma * s.I;                              //dR / dt
    d.D = mu * s.I;                                 //dD / dt
    d.t = 1.0;                                      //dt / dt = 1 (avance del tiempo)

    return d;
}