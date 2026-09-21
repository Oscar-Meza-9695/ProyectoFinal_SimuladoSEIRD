#pragma once
#include <SFML/Graphics.hpp>
#include "Slider.h"
#include <functional>

/**
 * PanelParametros.h
 * -----------------------------------------------------------------------
 * Panel derecho con los tres sliders de configuración y el botón Simular.
 *
 * Sliders:
 *   N          — Población total        [10,000 – 10,000,000]
 *   Cobertura  — Cobertura vacunal (%)  [0 – 100]
 *   I0         — Casos iniciales        [1 – 100]
 *
 * Emite onSimular(N, cobertura, I0) cuando el usuario presiona el botón.
 * -----------------------------------------------------------------------
 */
class PanelParametros
{
public:
    PanelParametros(float x, float y, float ancho, float alto,
                    const sf::Font& fuente);

    void manejarEvento(const sf::Event& evento, const sf::RenderWindow& ventana);
    void dibujar(sf::RenderWindow& ventana) const;

    // Callback: (poblacion, cobertura [0-1], infectados_iniciales)
    std::function<void(double, double, double)> onSimular;

private:
    float x, y, ancho, alto;
    const sf::Font& fuente;

    sf::RectangleShape fondo;
    sf::Text           titulo;

    Slider slider_N;
    Slider slider_cobertura;
    Slider slider_I0;

    sf::RectangleShape btn_fondo;
    sf::Text           btn_texto;
    bool               btn_hover = false;

    void actualizarBoton(const sf::RenderWindow& ventana);
};
