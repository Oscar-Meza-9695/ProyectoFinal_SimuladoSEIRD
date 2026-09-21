#include "ui/PanelEstado.h"
#include "ui/Tema.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

PanelEstado::PanelEstado(float px, float py, float pancho, float palto,
                         const sf::Font& f)
    : x(px), y(py), ancho(pancho), alto(palto), fuente(f),
      vel_pista(sf::Vector2f(100.f, 4.f)),
      vel_relleno(sf::Vector2f(30.f, 4.f)),
      vel_knob(6.f)
{
    fondo.setSize(sf::Vector2f(ancho, alto));
    fondo.setPosition(x, y);
    fondo.setFillColor(Tema::PANEL);
    fondo.setOutlineColor(Tema::PANEL_BORDE);
    fondo.setOutlineThickness(1.f);

    separador.setSize(sf::Vector2f(1.f, alto - 20.f));
    separador.setPosition(x + ancho - Tema::PANEL_PARAM_W - Tema::MARGEN, y + 10);
    separador.setFillColor(Tema::PANEL_BORDE);

    // ── Slider de velocidad ───────────────────────────────────────────────────
    float vx = x + 160.f, vy = y + alto/2 - 2;
    vel_pista.setPosition(vx, vy);
    vel_pista.setFillColor(Tema::SLIDER_PISTA);
    vel_relleno.setPosition(vx, vy);
    vel_relleno.setFillColor(Tema::SLIDER_RELLENO);
    vel_knob.setOrigin(6.f, 6.f);
    vel_knob.setPosition(vx + 30.f, vy + 2);
    vel_knob.setFillColor(Tema::SLIDER_KNOB);

    // ── Botón play (triángulo) ────────────────────────────────────────────────
    btn_play.setPointCount(3);
    btn_play.setPoint(0, sf::Vector2f(0,  0));
    btn_play.setPoint(1, sf::Vector2f(0,  16));
    btn_play.setPoint(2, sf::Vector2f(14, 8));
    btn_play.setPosition(x + 14, y + alto/2 - 8);
    btn_play.setFillColor(Tema::ACENTO);

    // ── Botón pausa (dos rectángulos) ─────────────────────────────────────────
    btn_pausa_l.setSize(sf::Vector2f(5, 16));
    btn_pausa_l.setPosition(x + 14, y + alto/2 - 8);
    btn_pausa_l.setFillColor(Tema::ACENTO);
    btn_pausa_r.setSize(sf::Vector2f(5, 16));
    btn_pausa_r.setPosition(x + 22, y + alto/2 - 8);
    btn_pausa_r.setFillColor(Tema::ACENTO);

    // ── Textos resaltados ─────────────────────────────────────────────────────

    // DÍA — grande, azul eléctrico, negrita
    txt_dia.setFont(fuente);
    txt_dia.setCharacterSize(22);
    txt_dia.setStyle(sf::Text::Bold);
    txt_dia.setFillColor(Tema::ACENTO);
    txt_dia.setPosition(x + 40, y + 8);

    // PICO — mediano, blanco
    txt_pico.setFont(fuente);
    txt_pico.setCharacterSize(14);
    txt_pico.setFillColor(Tema::TEXTO);
    txt_pico.setPosition(x + 170, y + 14);

    // FALLECIDOS — mediano, rojo
    txt_fallecidos.setFont(fuente);
    txt_fallecidos.setCharacterSize(14);
    txt_fallecidos.setStyle(sf::Text::Bold);
    txt_fallecidos.setFillColor(Tema::COLOR_I);   // rojo
    txt_fallecidos.setPosition(x + 340, y + 14);

    // RECUPERADOS — mediano, verde
    txt_recuperados.setFont(fuente);
    txt_recuperados.setCharacterSize(14);
    txt_recuperados.setStyle(sf::Text::Bold);
    txt_recuperados.setFillColor(Tema::COLOR_R);  // verde
    txt_recuperados.setPosition(x + 170, y + 36);

    inicializarBarras();
}

// ── Barras SEIRD ──────────────────────────────────────────────────────────────
void PanelEstado::inicializarBarras()
{
    struct Info { char l; sf::Color c; };
    Info infos[] = {
        {'S', Tema::COLOR_S}, {'E', Tema::COLOR_E}, {'I', Tema::COLOR_I},
        {'R', Tema::COLOR_R}, {'D', Tema::COLOR_D}
    };

    float bx  = x + ancho - Tema::PANEL_PARAM_W;
    float by  = y + 10;
    float bw  = 80.f;
    float bh  = 14.f;
    float gap = (alto - 20.f) / 5.f;

    barras.clear();
    for (int i = 0; i < 5; i++)
    {
        BarraEstado b;
        b.letra = infos[i].l;
        b.color = infos[i].c;

        b.fondo_barra.setSize(sf::Vector2f(bw, bh));
        b.fondo_barra.setPosition(bx + 30, by + i*gap + gap/2 - bh/2);
        b.fondo_barra.setFillColor(Tema::SLIDER_PISTA);

        b.relleno_barra.setSize(sf::Vector2f(0.f, bh));
        b.relleno_barra.setPosition(bx + 30, by + i*gap + gap/2 - bh/2);
        b.relleno_barra.setFillColor(infos[i].c);

        b.texto_letra.setFont(fuente);
        b.texto_letra.setCharacterSize(13);
        b.texto_letra.setStyle(sf::Text::Bold);
        b.texto_letra.setFillColor(infos[i].c);
        b.texto_letra.setString(std::string(1, infos[i].l) + ":");
        b.texto_letra.setPosition(bx + 8, by + i*gap + gap/2 - 8);

        b.texto_valor.setFont(fuente);
        b.texto_valor.setCharacterSize(12);
        b.texto_valor.setFillColor(Tema::TEXTO);
        b.texto_valor.setPosition(bx + 115, by + i*gap + gap/2 - 7);

        barras.push_back(b);
    }
}

// ── Cargar historial ──────────────────────────────────────────────────────────
void PanelEstado::cargarHistorial(const std::vector<PasoSimulacion>& h)
{
    historial = h;
    if (historial.empty()) return;

    // Calcular día pico usando el campo dia real
    dia_pico = 0;
    double maxI = 0;
    for (int i = 0; i < (int)historial.size(); i++)
        if (historial[i].I > maxI)
        {
            maxI     = historial[i].I;
            dia_pico = static_cast<int>(historial[i].dia);
        }

    setDiaActual(0);
}

// ── Actualizar día ────────────────────────────────────────────────────────────
void PanelEstado::setDiaActual(int idx)
{
    dia_actual = idx;
    if (historial.empty()) return;

    idx = std::min(idx, (int)historial.size() - 1);
    const auto& p = historial[idx];

    auto fmt = [](double v) -> std::string {
        std::ostringstream ss;
        if (v >= 1e6)       ss << std::fixed << std::setprecision(2) << v/1e6 << "M";
        else if (v >= 1000) ss << static_cast<int>(v/1000) << "k";
        else                ss << static_cast<int>(v);
        return ss.str();
    };

    txt_dia.setString("Dia: " + std::to_string(static_cast<int>(p.dia)));
    txt_pico.setString("Pico: dia " + std::to_string(dia_pico));
    txt_fallecidos.setString("Fallecidos: " + fmt(p.D));
    txt_recuperados.setString("Recuperados: " + fmt(p.R));

    actualizarBarras(idx);
}

// ── Actualizar barras ─────────────────────────────────────────────────────────
void PanelEstado::actualizarBarras(int dia)
{
    if (historial.empty() || barras.empty()) return;

    const auto& p    = historial[dia];
    double vals[]    = { p.S, p.E, p.I, p.R, p.D };
    double total     = p.N;

    auto fmt = [](double v) -> std::string {
        std::ostringstream ss;
        if (v >= 1e6)       ss << std::fixed << std::setprecision(1) << v/1e6 << "M";
        else if (v >= 1000) ss << static_cast<int>(v/1000) << "k";
        else                ss << static_cast<int>(v);
        return ss.str();
    };

    for (int i = 0; i < 5; i++)
    {
        float t  = (total > 0) ? static_cast<float>(vals[i] / total) : 0.f;
        float bw = 80.f * t;
        barras[i].relleno_barra.setSize(sf::Vector2f(bw, 14.f));
        barras[i].texto_valor.setString(fmt(vals[i]));
    }
}

// ── Eventos ───────────────────────────────────────────────────────────────────
void PanelEstado::manejarEvento(const sf::Event& evento,
                                const sf::RenderWindow& ventana)
{
    sf::Vector2i mouse = sf::Mouse::getPosition(ventana);
    float mx = static_cast<float>(mouse.x);
    float my = static_cast<float>(mouse.y);

    // Clic en play/pausa
    if (evento.type == sf::Event::MouseButtonReleased &&
        evento.mouseButton.button == sf::Mouse::Left)
    {
        sf::FloatRect zona_play(x + 10, y + alto/2 - 12, 40, 24);
        if (zona_play.contains(mx, my))
        {
            reproduciendo = !reproduciendo;
            if (reproduciendo && onPlay)   onPlay();
            if (!reproduciendo && onPausa) onPausa();
        }
    }

    // Slider velocidad
    sf::FloatRect zona_vel(x + 150, y + alto/2 - 10, 110, 20);
    if (evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left &&
        zona_vel.contains(mx, my))
        vel_arrastrando = true;

    if (evento.type == sf::Event::MouseButtonReleased)
        vel_arrastrando = false;

    if (vel_arrastrando && evento.type == sf::Event::MouseMoved)
    {
        float t   = std::max(0.f, std::min(1.f, (mx - (x + 160.f)) / 100.f));
        velocidad = 1.f + t * 29.f;
        vel_knob.setPosition(x + 160.f + t * 100.f, y + alto/2 - 2 + 2);
        vel_relleno.setSize(sf::Vector2f(t * 100.f, 4.f));
    }
}

// ── Dibujar ───────────────────────────────────────────────────────────────────
void PanelEstado::dibujar(sf::RenderWindow& ventana) const
{
    ventana.draw(fondo);
    ventana.draw(separador);

    ventana.draw(txt_dia);
    ventana.draw(txt_pico);
    ventana.draw(txt_fallecidos);
    ventana.draw(txt_recuperados);

    // Play o pausa según estado
    if (!reproduciendo)
        ventana.draw(btn_play);
    else
    {
        ventana.draw(btn_pausa_l);
        ventana.draw(btn_pausa_r);
    }

    // Etiqueta velocidad
    sf::Text lbl_vel;
    lbl_vel.setFont(fuente);
    lbl_vel.setCharacterSize(11);
    lbl_vel.setFillColor(Tema::TEXTO_DIM);
    lbl_vel.setString("Vel:");
    lbl_vel.setPosition(x + 115, y + alto/2 - 8);
    ventana.draw(lbl_vel);

    ventana.draw(vel_pista);
    ventana.draw(vel_relleno);
    ventana.draw(vel_knob);

    // Barras SEIRD
    for (const auto& b : barras)
    {
        ventana.draw(b.fondo_barra);
        ventana.draw(b.relleno_barra);
        ventana.draw(b.texto_letra);
        ventana.draw(b.texto_valor);
    }
}