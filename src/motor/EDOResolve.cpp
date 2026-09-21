#include "motor/EDOResolve.h"

//R4
EstadoSEIRD EDOResolve::pasoRK4(const ModeloSEIRD& modelo, const EstadoSEIRD& estado, double dt){
    //k1 - pendiente en el punto actual
    EstadoSEIRD k1 = modelo.derivadas(estado);
    //k2 pendiente en el punto medio usando k1
    EstadoSEIRD k2 = modelo.derivadas(sumarEstados(estado, escalarEstado(k1, dt * 0.5)));
    //k3 pendiente en el punto medio usando k2
    EstadoSEIRD k3 = modelo.derivadas(sumarEstados(estado, escalarEstado(k2, dt * 0.5)));
    //k4 pendiente al final del intervalo usando k3
    EstadoSEIRD k4 = modelo.derivadas(sumarEstados(estado, escalarEstado(k3, dt)));

    //Combinacion ponderada: y(t+dt) = y(t) + dt/6 * (k1 + k2 + k3 + k4)
    EstadoSEIRD siguiente = estado;
    siguiente.S += dt / 6.0 * (k1.S + 2.0*k2.S + 2.0*k3.S + k4.S);
    siguiente.E += dt / 6.0 * (k1.E + 2.0*k2.E + 2.0*k3.E + k4.E);
    siguiente.I += dt / 6.0 * (k1.I + 2.0*k2.I + 2.0*k3.I + k4.I);
    siguiente.R += dt / 6.0 * (k1.R + 2.0*k2.R + 2.0*k3.R + k4.R);
    siguiente.D += dt / 6.0 * (k1.D + 2.0*k2.D + 2.0*k3.D + k4.D);
    siguiente.t  = estado.t + dt;

    return siguiente;

}

// Auxiliares
EstadoSEIRD EDOResolve::sumarEstados(const EstadoSEIRD& a, const EstadoSEIRD& b){
    EstadoSEIRD r;
    r.S = a.S + b.S;
    r.E = a.E + b.E;
    r.I = a.I + b.I;
    r.R = a.R + b.R;
    r.D = a.D + b.D;
    r.t = a.t + b.t; 
    
    return r;
}
// Multiplica un estado (que en este contexto actúa como un vector de derivadas 'k') por un factor (como dt o dt*0.5)
EstadoSEIRD EDOResolve::escalarEstado(const EstadoSEIRD& estado, double factor) {
    EstadoSEIRD r;
    r.S = estado.S * factor;
    r.E = estado.E * factor;
    r.I = estado.I * factor;
    r.R = estado.R * factor;
    r.D = estado.D * factor;
    r.t = estado.t * factor;
    
    return r;
}