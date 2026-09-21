#pragma once
#include <SFML/Graphics.hpp>
#include "motor/ModeloSEIRD.h"
#include "motor/SimulacionRunner.h"
#include "ui/PanelParametros.h"
#include "ui/PanelGrafica.h"
#include "ui/PanelAgentes.h"
#include "ui/PanelEstado.h"
#include <vector>

/**
 * Ventana.h
 * Clase principal que:
 *   1. Crea la ventana SFML (1400×800)
 *   2. Instancia y posiciona los cuatro paneles
 *   3. Contiene el loop principal (eventos → actualizar → dibujar)
 *   4. Coordina el replay: avanza el día según velocidad y tiempo
 *
 * Flujo de una simulación:
 *   PanelParametros::onSimular → correrSimulacion() →
 *   reparte historial a paneles → inicia replay automáticamente
 */
class Ventana
{
public:
    Ventana();
    void correr();  // loop principal — bloquea hasta que se cierra la ventana

private:
    sf::RenderWindow ventana;
    sf::Font         fuente;

    // Paneles
    PanelParametros* panel_params  = nullptr;
    PanelGrafica*    panel_grafica = nullptr;
    PanelAgentes*    panel_agentes = nullptr;
    PanelEstado*     panel_estado  = nullptr;

    // Estado del replay
    std::vector<PasoSimulacion> historial;
    int   dia_actual    = 0;
    int   total_dias    = 0;
    float tiempo_acum   = 0.f;   // acumulador para avanzar días
    bool  simulado      = false;

    sf::RectangleShape header;
    sf::Text           titulo_header;

    void correrSimulacion(double N, double cobertura, double I0);
    void manejarEventos();
    void actualizar(float dt);
    void dibujar();
    void avanzarDia();

    void inicializarPaneles();
    void inicializarHeader();
};
