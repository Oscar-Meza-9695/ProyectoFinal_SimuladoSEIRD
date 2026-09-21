#pragma once
#include <SFML/Graphics.hpp>
#include "motor/SimulacionRunner.h"
#include <vector>

/**
 * PanelGrafica.h
 * -----------------------------------------------------------------------
 * Dibuja las cinco curvas SEIRD (S, E, I, R, D) en función del día.
 *
 * - Ejes X (días) e Y (personas) con etiquetas
 * - Línea vertical que indica el día actual del replay
 * - Leyenda de colores en la esquina superior
 * -----------------------------------------------------------------------
 */
class PanelGrafica
{
public:
    PanelGrafica(float x, float y, float ancho, float alto,
                 const sf::Font& fuente);

    // Carga el historial completo (llamar después de cada simulación)
    void cargarHistorial(const std::vector<PasoSimulacion>& historial);

    // Actualiza la línea del día actual (llamar en cada frame del replay)
    void setDiaActual(int dia);

    void dibujar(sf::RenderWindow& ventana) const;

private:
    float x, y, ancho, alto;
    const sf::Font& fuente;

    // Área interna de dibujo (dentro de los márgenes de ejes)
    float gx, gy, gw, gh;  // grafica x, y, width, height

    std::vector<PasoSimulacion> historial;
    int dia_actual = 0;
    double N = 1.0;
    int total_dias = 0;

    // Vértices de cada curva (precalculados en cargarHistorial)
    std::vector<sf::Vertex> curva_S, curva_E, curva_I, curva_R, curva_D;
    sf::VertexArray linea_dia;

    sf::RectangleShape fondo;
    sf::RectangleShape borde;

    void recalcularCurvas();
    void dibujarEjes(sf::RenderWindow& ventana) const;
    void dibujarLeyenda(sf::RenderWindow& ventana) const;

    // Convierte día y valor a coordenada de pantalla
    float diaAX(double dia)   const;
    float valorAY(double val) const;
};
