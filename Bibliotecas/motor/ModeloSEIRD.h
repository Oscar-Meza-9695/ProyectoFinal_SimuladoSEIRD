#pragma once 

/*
Define los parametros del modelo SEIRD para el sarampion y el estado
del sistema en un momento dado.

Compartimentos:
    S: Susceptibles
    E: Expuestos
    I: Infectados
    R: Recuperados
    D: Fallecidos
Ecuaciones diferenciales del modelo SEIRD:
    dS/dt = -beta * S * I / N
    dE/dt = beta * S * I / N - sigma * E
    dI/dt = sigma * E - gamma * I - mu * I
    dR/dt = gamma * I
    dD/dt = mu * I

En donde:
    beta: Tasa de transmisión
    sigma: Tasa de progresión de expuestos a infectados
    gamma: Tasa de recuperación
    mu: Tasa de mortalidad diaria
    N: Población total (S + E + I + R + D)
*/

//Estado del sistema en un instante dado
struct EstadoSEIRD {
    double S = 0.0; //Susceptibles
    double E = 0.0; //Expuestos
    double I = 0.0; //Infectados
    double R = 0.0; //Recuperados
    double D = 0.0; //Fallecidos
    double t = 0.0; //Tiempo en dias
};

// Modelo completo con sus parametros
class ModeloSEIRD {
    public:
    //Parametros fijos (biológicos del Sarampion)
    static constexpr double R0                  = 15.0; //Número básico de reproducción
    static constexpr double Dias_Incubacion     = 10.0; //Días de incubación
    static constexpr double Dias_Infecciosos    = 8.0; //Días de periodo infeccioso
    static constexpr double CFR                 = 0.0015; //Caso fatality rate (tasa de mortalidad)
    static constexpr double Eficacia_Vacuna     = 0.97; //Efictividad MMR (este es fijo)

    //Pararmetros derivados (calculados a partir de los anteriores)
    double sigma;            //1 / Dias_Incubacion
    double gamma;            //1 / Dias_Infecciosos
    double mu;               //CFR * gamma
    double beta;             //R0 * gamma

    //Parametros del escenario (Configurables por el usuario)
    double N;                   //Población total
    double Vacunacion;          //Porcentaje de la población vacunada (0.0 a 1.0)
    double Infectados_Iniciales; //Número inicial de infectados

    //Estado inicial del sistema
    EstadoSEIRD estado_inicial;

    /*
    Constructor
    @param N: Población total
    @param Vacunacion: Porcentaje de la población vacunada (0.0 a 1.0)
    @param Infectados_Iniciales: Número inicial de infectados
    */
    ModeloSEIRD(double poblacion, double Cobertura_vacunacion, double Infectados_Iniciales = 1.0);

    /*
    Evalua las derivadas del sistema SEIRD en un estado dado.
    Devuelve dS, dE, dI, dR, dD como un nuevo EstadoSEIRD.(Usando los campos
    S/E/I/R/D como deltas; el capo t no se usa en este modelo pero se incluye para compatibilidad con RK4)

    @param estado: El estado actual {S, E, I, R, D} del sistema.
    @return: Derivadas {dS, dE, dI, dR, dD} del sistema en el estado dado.
    */
    EstadoSEIRD derivadas(const EstadoSEIRD& estado) const;
};