#pragma once
#include <SFML/Graphics.hpp>

/**
 * Tema.h
 * Paleta de colores, fuentes y constantes de layout para toda la UI.
 * Un solo lugar para cambiar la apariencia del simulador.
 */

namespace Tema
{
    // ── Resolución de ventana ──────────────────────────────────────────
    constexpr unsigned int ANCHO  = 1400;
    constexpr unsigned int ALTO   = 800;

    // ── Paleta principal (fondo oscuro tipo laboratorio) ───────────────
    const sf::Color FONDO           = sf::Color(15,  20,  30);   // azul noche
    const sf::Color PANEL           = sf::Color(22,  30,  45);   // panel oscuro
    const sf::Color PANEL_BORDE     = sf::Color(45,  65,  95);   // borde sutil
    const sf::Color TEXTO           = sf::Color(210, 220, 235);  // blanco frío
    const sf::Color TEXTO_DIM       = sf::Color(100, 120, 150);  // texto secundario
    const sf::Color ACENTO          = sf::Color(60,  160, 240);  // azul eléctrico

    // ── Colores SEIRD ──────────────────────────────────────────────────
    const sf::Color COLOR_S         = sf::Color(60,  140, 230);  // azul   — Susceptible
    const sf::Color COLOR_E         = sf::Color(240, 190,  40);  // amarillo — Expuesto
    const sf::Color COLOR_I         = sf::Color(220,  60,  60);  // rojo   — Infeccioso
    const sf::Color COLOR_R         = sf::Color(60,  200, 120);  // verde  — Recuperado
    const sf::Color COLOR_D         = sf::Color(130, 130, 140);  // gris   — Fallecido

    // ── Color botón ───────────────────────────────────────────────────
    const sf::Color BTN_NORMAL      = sf::Color(40,  110, 200);
    const sf::Color BTN_HOVER       = sf::Color(60,  150, 255);
    const sf::Color BTN_TEXTO       = sf::Color(255, 255, 255);

    // ── Slider ────────────────────────────────────────────────────────
    const sf::Color SLIDER_PISTA    = sf::Color(45,  65,  95);
    const sf::Color SLIDER_RELLENO  = sf::Color(60,  160, 240);
    const sf::Color SLIDER_KNOB     = sf::Color(255, 255, 255);

    // ── Layout: márgenes y divisiones ─────────────────────────────────
    constexpr float MARGEN          = 12.f;
    constexpr float HEADER_H        = 44.f;
    constexpr float BARRA_H         = 110.f;  // barra inferior
    constexpr float PANEL_PARAM_W   = 220.f;  // panel derecho

    // Ancho del área central (gráfica + agentes)
    constexpr float AREA_CENTRAL_W  = ANCHO - PANEL_PARAM_W - MARGEN * 3;
    constexpr float GRAFICA_W       = AREA_CENTRAL_W * 0.58f;
    constexpr float AGENTES_W       = AREA_CENTRAL_W * 0.42f;
    constexpr float CONTENIDO_H     = ALTO - HEADER_H - BARRA_H - MARGEN * 4;
}
