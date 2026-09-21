#pragma once
#include <SFML/Graphics.hpp>
#include "motor/SimulacionRunner.h"
#include <vector>
#include <functional>

/**
 * PanelEstado.h
 * 
 * Barra inferior con:
 *   - Día actual, día del pico de infecciosos, fallecidos, recuperados
 *   - Botones Play / Pausa
 *   - Slider de velocidad de replay
 *   - Barras de progreso S, E, I, R, D con valores numéricos
 * 
 */
class PanelEstado
{
public:
    PanelEstado(float x, float y, float ancho, float alto,
                const sf::Font& fuente);

    void cargarHistorial(const std::vector<PasoSimulacion>& historial);
    void setDiaActual(int dia);

    // Controles de replay
    bool isReproduciendo() const { return reproduciendo; }
    float getVelocidad()   const { return velocidad; }

    void manejarEvento(const sf::Event& evento, const sf::RenderWindow& ventana);
    void dibujar(sf::RenderWindow& ventana) const;

    std::function<void()> onPlay;
    std::function<void()> onPausa;

private:
    float x, y, ancho, alto;
    const sf::Font& fuente;

    std::vector<PasoSimulacion> historial;
    int    dia_actual  = 0;
    int    dia_pico    = 0;
    bool   reproduciendo = false;
    float  velocidad   = 1.f;   // días por segundo

    sf::RectangleShape fondo;
    sf::RectangleShape separador;

    // Slider de velocidad
    sf::RectangleShape vel_pista;
    sf::RectangleShape vel_relleno;
    sf::CircleShape    vel_knob;
    bool               vel_arrastrando = false;

    // Botones play/pausa
    sf::ConvexShape  btn_play;
    sf::RectangleShape btn_pausa_l, btn_pausa_r;

    // Barras SEIRD
    struct BarraEstado {
        char letra;
        sf::Color color;
        sf::RectangleShape fondo_barra;
        sf::RectangleShape relleno_barra;
        sf::Text texto_letra;
        sf::Text texto_valor;
    };
    std::vector<BarraEstado> barras;

    sf::Text txt_dia, txt_pico, txt_fallecidos, txt_recuperados;

    void inicializarBarras();
    void actualizarBarras(int dia);
    void dibujarControles(sf::RenderWindow& ventana) const;
    void dibujarBarras(sf::RenderWindow& ventana) const;
    void dibujarTextos(sf::RenderWindow& ventana) const;
};