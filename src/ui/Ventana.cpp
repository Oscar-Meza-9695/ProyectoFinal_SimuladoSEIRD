#include "ui/Ventana.h"
#include "ui/Tema.h"
#include "motor/ModeloSEIRD.h"
#include "motor/SimulacionRunner.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// ── Constructor ───────────────────────────────────────────────────────────────
Ventana::Ventana()
    : ventana(sf::VideoMode(Tema::ANCHO, Tema::ALTO),
              "Simulador Sarampion SEIRD",
              sf::Style::Titlebar | sf::Style::Close)
{
    ventana.setFramerateLimit(60);

    // Cargar fuente
    if (!fuente.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        // Fallback: intentar fuente genérica
        std::cerr << "Advertencia: no se pudo cargar arial.ttf\n";
    }

    inicializarHeader();
    inicializarPaneles();
}

// ── Inicializar header ────────────────────────────────────────────────────────
void Ventana::inicializarHeader()
{
    header.setSize(sf::Vector2f(Tema::ANCHO, Tema::HEADER_H));
    header.setPosition(0, 0);
    header.setFillColor(sf::Color(10, 15, 25));

    titulo_header.setFont(fuente);
    titulo_header.setCharacterSize(18);
    titulo_header.setFillColor(Tema::ACENTO);
    titulo_header.setStyle(sf::Text::Bold);
    titulo_header.setString("SIMULADOR DE SARAMPION  |  Modelo SEIRD");
    titulo_header.setPosition(Tema::MARGEN * 2, Tema::HEADER_H / 2 - 10);
}

// ── Inicializar paneles ───────────────────────────────────────────────────────
void Ventana::inicializarPaneles()
{
    float m  = Tema::MARGEN;
    float py = Tema::HEADER_H + m;            // y inicio paneles superiores
    float ph = Tema::CONTENIDO_H;             // alto paneles superiores

    // Panel parámetros (derecha)
    float param_x = Tema::ANCHO - Tema::PANEL_PARAM_W - m;
    panel_params = new PanelParametros(param_x, py,
                                       Tema::PANEL_PARAM_W, ph, fuente);

    // Panel gráfica (izquierda)
    panel_grafica = new PanelGrafica(m, py, Tema::GRAFICA_W, ph, fuente);

    // Panel agentes (centro)
    float agentes_x = m + Tema::GRAFICA_W + m;
    panel_agentes = new PanelAgentes(agentes_x, py, Tema::AGENTES_W, ph, fuente);

    // Panel estado (barra inferior)
    float estado_y = py + ph + m;
    panel_estado = new PanelEstado(m, estado_y,
                                   Tema::ANCHO - m * 2, Tema::BARRA_H, fuente);

    // ── Conectar callbacks ────────────────────────────────────────────────────

    // Botón SIMULAR → correr motor
    panel_params->onSimular = [this](double N, double cobertura, double I0)
    {
        correrSimulacion(N, cobertura, I0);
    };

    // Play / Pausa
    panel_estado->onPlay  = [this]() { /* reproduciendo ya se maneja internamente */ };
    panel_estado->onPausa = [this]() { };
}

// ── Correr simulación ─────────────────────────────────────────────────────────
void Ventana::correrSimulacion(double N, double cobertura, double I0)
{
    ModeloSEIRD modelo(N, cobertura, I0);
    SimulacionRunner runner;

    historial   = runner.run(modelo);
    total_dias  = static_cast<int>(historial.back().dia);
    dia_actual  = 0;
    tiempo_acum = 0.f;
    simulado    = true;

    runner.exportCSV(historial, "output.csv");

    panel_grafica->cargarHistorial(historial);
    panel_agentes->cargarHistorial(historial);
    panel_estado->cargarHistorial(historial);

    // ── Actualizar título con parámetros del modelo ───────────────────────
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << "SIMULADOR DE SARAMPION  |  Modelo SEIRD"
       << "     R0=" << ModeloSEIRD::R0
       << "  β=" << modelo.beta
       << "  Tiempo simulado: " << total_dias << " dias";
    titulo_header.setString(ss.str());
}

// ── Loop principal ────────────────────────────────────────────────────────────
void Ventana::correr()
{
    sf::Clock reloj;

    while (ventana.isOpen())
    {
        float dt = reloj.restart().asSeconds();

        manejarEventos();
        actualizar(dt);
        dibujar();
    }
}

// ── Eventos ───────────────────────────────────────────────────────────────────
void Ventana::manejarEventos()
{
    sf::Event evento;
    while (ventana.pollEvent(evento))
    {
        if (evento.type == sf::Event::Closed)
            ventana.close();

        if (evento.type == sf::Event::KeyPressed &&
            evento.key.code == sf::Keyboard::Escape)
            ventana.close();

        panel_params->manejarEvento(evento, ventana);
        panel_estado->manejarEvento(evento, ventana);
    }
}

// ── Actualizar ────────────────────────────────────────────────────────────────
void Ventana::actualizar(float dt)
{
    if (!simulado) return;

    // Avanzar replay si está reproduciendo
    if (panel_estado->isReproduciendo() && dia_actual < (int)historial.size() - 1)
    {
        tiempo_acum += dt * panel_estado->getVelocidad();
        while (tiempo_acum >= 1.f && dia_actual < (int)historial.size() - 1)
        {
            tiempo_acum -= 1.f;
            avanzarDia();
        }
    }

    // Actualizar agentes siempre (animación continua)
    panel_agentes->actualizar(dt, dia_actual);
}

// ── Avanzar un día en el replay ───────────────────────────────────────────────
void Ventana::avanzarDia()
{
    dia_actual++;
    // dia_actual es índice del historial (0 a historial.size()-1)
    int idx = std::min(dia_actual, (int)historial.size() - 1);
    panel_grafica->setDiaActual(idx);
    panel_estado->setDiaActual(idx);
}

// ── Dibujar ───────────────────────────────────────────────────────────────────
void Ventana::dibujar()
{
    ventana.clear(Tema::FONDO);

    ventana.draw(header);
    ventana.draw(titulo_header);

    panel_grafica->dibujar(ventana);
    panel_agentes->dibujar(ventana);
    panel_params->dibujar(ventana);
    panel_estado->dibujar(ventana);

    ventana.display();
}
