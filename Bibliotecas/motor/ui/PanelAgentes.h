#pragma once
#include <SFML/Graphics.hpp>
#include "motor/SimulacionRunner.h"
#include <vector>

/**
 * PanelAgentes.h
 * 
 * Visualización de ~300 agentes como círculos animados que representan
 * proporcionalmente el estado SEIRD en cada día del replay.
 *
 * Comportamiento:
 *   - Los agentes se mueven aleatoriamente con rebote en los bordes
 *   - Su color cambia según su estado: S=azul E=amarillo I=rojo R=verde D=gris
 *   - Cuando un susceptible se infecta, el infeccioso más cercano lanza
 *     una línea de contagio breve (dura 0.4 segundos)
 */

struct Agente
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    int          estado;   // 0=S 1=E 2=I 3=R 4=D
    int          estado_anterior;
    float        timer_linea = 0.f;  // segundos que queda visible la línea
    int          objetivo_linea = -1; // índice del agente al que apunta la línea
};

class PanelAgentes
{
public:
    static constexpr int NUM_AGENTES = 300;

    PanelAgentes(float x, float y, float ancho, float alto,
                 const sf::Font& fuente);

    // Carga historial y reinicia agentes
    void cargarHistorial(const std::vector<PasoSimulacion>& historial);

    // Avanza la animación (dt en segundos) al día actual
    void actualizar(float dt, int dia_actual);

    void dibujar(sf::RenderWindow& ventana) const;

private:
    float x, y, ancho, alto;
    const sf::Font& fuente;

    std::vector<PasoSimulacion> historial;
    std::vector<Agente>         agentes;

    sf::RectangleShape fondo;

    // Reparte NUM_AGENTES entre estados según proporciones del día
    void redistribuirAgentes(int dia);

    // Devuelve el color según estado
    sf::Color colorEstado(int estado) const;

    // Encuentra el infeccioso más cercano al agente i
    int infecciosoMasCercano(int idx) const;
};