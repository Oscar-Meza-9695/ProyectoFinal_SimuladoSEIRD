#pragma once

#include "ModeloSEIRD.h"

/*
Este archivo de cabecera es el que va a integrar númericamete el modelo
Runge-Kutta de orden 4 para resolver el sistema de ecuaciones diferenciales ordinarias (EDO) del modelo SEIRD.

RK4 calcula cuatro estimaciones de la pendiente en cada paso y luego las combina
para obtener una estimacion mas precisa de oreden 4.

    k1=f(t,             y)
    k2=f(t+dt/2,        y+k1*dt/2)
    k3=f(t+dt/2,        y+k2*dt/2)
    k4=f(t+dt,          y+k3*dt)

    y(t+dt)=y(t) + dt/6 * (k1 + 2*k2 + 2*k3 + k4)
    En este caso el error global es O(dt^4), lo que significa que es mucho mejor que Euler O(dt); 
*/

class EDOResolve 
{
    public:
    /*
    Aquí se declara la función que va a resolver el sistema de EDO utilizando RK4.
    @param model: Es una instancia del modelo SEIRD que contiene los parámetros y las funciones necesarias para calcular las derivadas.
    @param estado: Es el estado actual {S, E, I, R, D} del sistema en el tiempo t.
    @param dt: Es el paso de tiempo que se va a utilizar para avanzar la solución.
    @return: Es el nuevo estado {S, E, I, R, D} del sistema en el tiempo t+dt después de aplicar RK4.
    */
   static EstadoSEIRD pasoRK4(const ModeloSEIRD& model, const EstadoSEIRD& estado, double dt);

   private:
   //Sumar dos estados SEIRD(usado internamente para RK4)
    static EstadoSEIRD sumarEstados(const EstadoSEIRD& a, const EstadoSEIRD& b);

    //Escala un estado por un escalar (usado internamente para RK4)
    static EstadoSEIRD escalarEstado(const EstadoSEIRD& estado, double factor);
};