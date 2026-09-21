#include "ui/PanelAgentes.h"
#include "ui/Tema.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <sstream>

static float randf(float lo, float hi) {
    return lo + (hi - lo) * (static_cast<float>(rand()) / RAND_MAX);
}

PanelAgentes::PanelAgentes(float px, float py, float pancho, float palto,
                           const sf::Font& f)
    : x(px), y(py), ancho(pancho), alto(palto), fuente(f)
{
    fondo.setSize(sf::Vector2f(ancho, alto));
    fondo.setPosition(x, y);
    fondo.setFillColor(sf::Color(18, 25, 38));
    fondo.setOutlineColor(Tema::PANEL_BORDE);
    fondo.setOutlineThickness(1.f);

    // Inicializar agentes con posiciones y velocidades aleatorias
    agentes.resize(NUM_AGENTES);
    float margen = 10.f;
    for (auto& a : agentes)
    {
        a.pos       = { randf(x + margen, x + ancho - margen),
                        randf(y + margen, y + alto  - margen) };
        float speed = randf(20.f, 55.f);
        float angle = randf(0.f, 6.2832f);
        a.vel       = { std::cos(angle) * speed, std::sin(angle) * speed };
        a.estado    = 0;  // todos susceptibles al inicio
        a.estado_anterior = 0;
    }
}

void PanelAgentes::cargarHistorial(const std::vector<PasoSimulacion>& h)
{
    historial = h;
    if (!historial.empty())
        redistribuirAgentes(0);
}

void PanelAgentes::actualizar(float dt, int dia_actual)
{
    if (historial.empty()) return;

    int dia = std::min(dia_actual, static_cast<int>(historial.size()) - 1);
    redistribuirAgentes(dia);

    float margen = 8.f;
    float xMin = x + margen, xMax = x + ancho - margen;
    float yMin = y + margen, yMax = y + alto  - margen;

    for (auto& a : agentes)
    {
        // Mover
        a.pos += a.vel * dt;

        // Rebotar en bordes
        if (a.pos.x < xMin) { a.pos.x = xMin; a.vel.x = std::abs(a.vel.x); }
        if (a.pos.x > xMax) { a.pos.x = xMax; a.vel.x = -std::abs(a.vel.x); }
        if (a.pos.y < yMin) { a.pos.y = yMin; a.vel.y = std::abs(a.vel.y); }
        if (a.pos.y > yMax) { a.pos.y = yMax; a.vel.y = -std::abs(a.vel.y); }

        // Actualizar timer de línea de contagio
        if (a.timer_linea > 0.f)
            a.timer_linea -= dt;
    }
}

void PanelAgentes::redistribuirAgentes(int dia)
{
    const auto& paso = historial[dia];
    double total     = paso.N;

    // Calcular cuántos agentes por estado (proporcional)
    int nS = static_cast<int>(NUM_AGENTES * paso.S / total);
    int nE = static_cast<int>(NUM_AGENTES * paso.E / total);
    int nI = static_cast<int>(NUM_AGENTES * paso.I / total);
    int nR = static_cast<int>(NUM_AGENTES * paso.R / total);
    int nD = NUM_AGENTES - nS - nE - nI - nR;
    nD = std::max(0, nD);

    // Asignar estados en orden
    int idx = 0;
    auto asignar = [&](int n, int estado) {
        for (int i = 0; i < n && idx < NUM_AGENTES; i++, idx++)
        {
            int prev = agentes[idx].estado;
            agentes[idx].estado = estado;

            // Si recién se infectó, lanzar línea de contagio
            if (prev == 0 && estado == 2)
            {
                agentes[idx].objetivo_linea = infecciosoMasCercano(idx);
                agentes[idx].timer_linea = 0.4f;
            }
        }
    };

    asignar(nS, 0);
    asignar(nE, 1);
    asignar(nI, 2);
    asignar(nR, 3);
    asignar(nD, 4);
}

void PanelAgentes::dibujar(sf::RenderWindow& ventana) const
{
    ventana.draw(fondo);

    // Dibujar líneas de contagio primero (debajo de los agentes)
    for (int i = 0; i < NUM_AGENTES; i++)
    {
        const auto& a = agentes[i];
        if (a.timer_linea > 0.f && a.objetivo_linea >= 0 &&
            a.objetivo_linea < NUM_AGENTES)
        {
            float alpha = std::min(1.f, a.timer_linea / 0.4f);
            sf::Color col(220, 60, 60, static_cast<sf::Uint8>(alpha * 180));

            sf::Vertex linea[2] = {
                sf::Vertex(agentes[a.objetivo_linea].pos, col),
                sf::Vertex(a.pos, col)
            };
            ventana.draw(linea, 2, sf::Lines);
        }
    }

    // Dibujar agentes
    sf::CircleShape circulo(4.f);
    circulo.setOrigin(4.f, 4.f);

    for (const auto& a : agentes)
    {
        circulo.setPosition(a.pos);
        circulo.setFillColor(colorEstado(a.estado));
        ventana.draw(circulo);
    }

    // ── Etiqueta de muestra representativa ───────────────────────────────────
    sf::Text txt_muestra;
    txt_muestra.setFont(fuente);
    txt_muestra.setCharacterSize(10);
    txt_muestra.setFillColor(Tema::TEXTO_DIM);

    double escala = historial.empty() ? 1.0 : historial[0].N / NUM_AGENTES;
    std::ostringstream ss;
    ss << NUM_AGENTES << " agentes de muestra  |  1 punto = "
       << static_cast<int>(escala) << " personas";
    txt_muestra.setString(ss.str());
    txt_muestra.setPosition(x + 8, y + alto - 18);
    ventana.draw(txt_muestra);
}

sf::Color PanelAgentes::colorEstado(int estado) const
{
    switch (estado)
    {
        case 0: return Tema::COLOR_S;
        case 1: return Tema::COLOR_E;
        case 2: return Tema::COLOR_I;
        case 3: return Tema::COLOR_R;
        case 4: return Tema::COLOR_D;
        default: return sf::Color::White;
    }
}

int PanelAgentes::infecciosoMasCercano(int idx) const
{
    float distMin = 1e9f;
    int   mejor   = -1;

    for (int i = 0; i < NUM_AGENTES; i++)
    {
        if (i == idx || agentes[i].estado != 2) continue;
        sf::Vector2f d = agentes[i].pos - agentes[idx].pos;
        float dist = std::sqrt(d.x*d.x + d.y*d.y);
        if (dist < distMin) { distMin = dist; mejor = i; }
    }
    return mejor;
}
