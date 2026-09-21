#include "ui/PanelGrafica.h"
#include "ui/Tema.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

static constexpr float MARGEN_EJE_IZQ  = 52.f;
static constexpr float MARGEN_EJE_INF  = 30.f;
static constexpr float MARGEN_INT      = 10.f;

PanelGrafica::PanelGrafica(float px, float py, float pancho, float palto,
                           const sf::Font& f)
    : x(px), y(py), ancho(pancho), alto(palto), fuente(f),
      curva_S(sf::LineStrip), curva_E(sf::LineStrip),
      curva_I(sf::LineStrip), curva_R(sf::LineStrip),
      curva_D(sf::LineStrip),
      linea_dia(sf::Lines, 2)
{
    gx = x + MARGEN_EJE_IZQ;
    gy = y + MARGEN_INT;
    gw = ancho - MARGEN_EJE_IZQ - MARGEN_INT;
    gh = alto  - MARGEN_EJE_INF - MARGEN_INT;

    fondo.setSize(sf::Vector2f(ancho, alto));
    fondo.setPosition(x, y);
    fondo.setFillColor(Tema::PANEL);
    fondo.setOutlineColor(Tema::PANEL_BORDE);
    fondo.setOutlineThickness(1.f);
}

void PanelGrafica::cargarHistorial(const std::vector<PasoSimulacion>& h)
{
    historial = h;
    if (historial.empty()) return;

    N          = historial[0].N;
    total_dias = static_cast<int>(historial.back().dia);
    dia_actual = 0;
    recalcularCurvas();
}

void PanelGrafica::setDiaActual(int dia)
{
    dia_actual = dia;
    // Actualizar línea vertical
    float lx = diaAX(dia_actual);
    linea_dia[0] = sf::Vertex(sf::Vector2f(lx, gy),        sf::Color(255,255,255,80));
    linea_dia[1] = sf::Vertex(sf::Vector2f(lx, gy + gh),   sf::Color(255,255,255,80));
}

void PanelGrafica::recalcularCurvas()
{
    curva_S.clear(); curva_E.clear(); curva_I.clear();
    curva_R.clear(); curva_D.clear();

    for (const auto& paso : historial)
    {
        float fx = diaAX(paso.dia);
        curva_S.push_back(sf::Vertex(sf::Vector2f(fx, valorAY(paso.S)), Tema::COLOR_S));
        curva_E.push_back(sf::Vertex(sf::Vector2f(fx, valorAY(paso.E)), Tema::COLOR_E));
        curva_I.push_back(sf::Vertex(sf::Vector2f(fx, valorAY(paso.I)), Tema::COLOR_I));
        curva_R.push_back(sf::Vertex(sf::Vector2f(fx, valorAY(paso.R)), Tema::COLOR_R));
        curva_D.push_back(sf::Vertex(sf::Vector2f(fx, valorAY(paso.D)), Tema::COLOR_D));
    }
}

void PanelGrafica::dibujar(sf::RenderWindow& ventana) const
{
    ventana.draw(fondo);
    if (historial.empty()) return;

    int n = std::min(dia_actual + 1, (int)curva_S.size());
    if (n > 1)
    {
        ventana.draw(curva_S.data(), n, sf::LineStrip);
        ventana.draw(curva_E.data(), n, sf::LineStrip);
        ventana.draw(curva_I.data(), n, sf::LineStrip);
        ventana.draw(curva_R.data(), n, sf::LineStrip);
        ventana.draw(curva_D.data(), n, sf::LineStrip);
    }

    ventana.draw(linea_dia);
    dibujarEjes(ventana);
    dibujarLeyenda(ventana);
}

void PanelGrafica::dibujarEjes(sf::RenderWindow& ventana) const
{
    // Línea eje X
    sf::RectangleShape ejeX(sf::Vector2f(gw, 1.f));
    ejeX.setPosition(gx, gy + gh);
    ejeX.setFillColor(Tema::PANEL_BORDE);
    ventana.draw(ejeX);

    // Línea eje Y
    sf::RectangleShape ejeY(sf::Vector2f(1.f, gh));
    ejeY.setPosition(gx, gy);
    ejeY.setFillColor(Tema::PANEL_BORDE);
    ventana.draw(ejeY);

    // Etiquetas eje X (días)
    int pasos = 5;
    for (int i = 0; i <= pasos; i++)
    {
        int dia = (total_dias * i) / pasos;
        float fx = diaAX(dia);

        sf::Text t;
        t.setFont(fuente);
        t.setCharacterSize(10);
        t.setFillColor(Tema::TEXTO_DIM);
        t.setString(std::to_string(dia));
        t.setPosition(fx - t.getGlobalBounds().width / 2, gy + gh + 6);
        ventana.draw(t);
    }

    // Etiquetas eje Y (población en miles/millones)
    for (int i = 0; i <= 4; i++)
    {
        double val = N * i / 4.0;
        float fy   = valorAY(val);

        std::ostringstream ss;
        if (val >= 1'000'000)      ss << std::fixed << std::setprecision(1) << val/1e6 << "M";
        else if (val >= 1000)      ss << static_cast<int>(val/1000) << "k";
        else                       ss << static_cast<int>(val);

        sf::Text t;
        t.setFont(fuente);
        t.setCharacterSize(10);
        t.setFillColor(Tema::TEXTO_DIM);
        t.setString(ss.str());
        t.setPosition(x + 2, fy - 6);
        ventana.draw(t);
    }
}

void PanelGrafica::dibujarLeyenda(sf::RenderWindow& ventana) const
{
    struct Entrada { const char* label; sf::Color color; };
    Entrada entradas[] = {
        {"S", Tema::COLOR_S}, {"E", Tema::COLOR_E}, {"I", Tema::COLOR_I},
        {"R", Tema::COLOR_R}, {"D", Tema::COLOR_D}
    };

    float lx = gx + 8;
    float ly = gy + 8;

    for (auto& e : entradas)
    {
        sf::RectangleShape rect(sf::Vector2f(12.f, 3.f));
        rect.setPosition(lx, ly + 5);
        rect.setFillColor(e.color);
        ventana.draw(rect);

        sf::Text t;
        t.setFont(fuente);
        t.setCharacterSize(11);
        t.setFillColor(e.color);
        t.setString(e.label);
        t.setPosition(lx + 16, ly);
        ventana.draw(t);

        lx += 36;
    }
}

float PanelGrafica::diaAX(double dia) const
{
    if (total_dias == 0) return gx;
    return gx + static_cast<float>(dia / total_dias) * gw;
}

float PanelGrafica::valorAY(double val) const
{
    if (N == 0) return gy + gh;
    float t = static_cast<float>(val / N);
    return gy + gh - t * gh;
}
