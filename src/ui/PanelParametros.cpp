#include "ui/PanelParametros.h"
#include "ui/Tema.h"

PanelParametros::PanelParametros(float px, float py, float pancho, float palto,
                                 const sf::Font& f)
    : x(px), y(py), ancho(pancho), alto(palto), fuente(f),
      slider_N      ("Población",  px+14, py+52,  pancho-28, 10000, 10000000, 1000000, f),
      slider_cobertura("Cobertura", px+14, py+100, pancho-28, 0,     100,       85,      f),
      slider_I0     ("Infectados iniciales", px+14, py+148, pancho-28, 1, 100, 1, f)
{
    fondo.setSize(sf::Vector2f(ancho, alto));
    fondo.setPosition(x, y);
    fondo.setFillColor(Tema::PANEL);
    fondo.setOutlineColor(Tema::PANEL_BORDE);
    fondo.setOutlineThickness(1.f);

    titulo.setFont(fuente);
    titulo.setCharacterSize(14);
    titulo.setFillColor(Tema::ACENTO);
    titulo.setString("PARAMETROS");
    titulo.setStyle(sf::Text::Bold);
    titulo.setPosition(px + 14, py + 14);

    // Botón SIMULAR
    float bx = px + 14;
    float by = py + 200;
    float bw = pancho - 28;
    float bh = 36;

    btn_fondo.setSize(sf::Vector2f(bw, bh));
    btn_fondo.setPosition(bx, by);
    btn_fondo.setFillColor(Tema::BTN_NORMAL);
    btn_fondo.setOutlineThickness(0.f);

    btn_texto.setFont(fuente);
    btn_texto.setCharacterSize(14);
    btn_texto.setFillColor(Tema::BTN_TEXTO);
    btn_texto.setString("SIMULAR");
    btn_texto.setStyle(sf::Text::Bold);
    // centrar texto en botón
    sf::FloatRect tb = btn_texto.getGlobalBounds();
    btn_texto.setPosition(bx + bw/2 - tb.width/2, by + bh/2 - tb.height/2 - 2);
}

void PanelParametros::manejarEvento(const sf::Event& evento,
                                    const sf::RenderWindow& ventana)
{
    slider_N.manejarEvento(evento, ventana);
    slider_cobertura.manejarEvento(evento, ventana);
    slider_I0.manejarEvento(evento, ventana);

    // Detectar hover y clic en botón
    sf::Vector2i mouse = sf::Mouse::getPosition(ventana);
    sf::FloatRect rb = btn_fondo.getGlobalBounds();
    btn_hover = rb.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
    btn_fondo.setFillColor(btn_hover ? Tema::BTN_HOVER : Tema::BTN_NORMAL);

    if (evento.type == sf::Event::MouseButtonReleased &&
        evento.mouseButton.button == sf::Mouse::Left && btn_hover)
    {
        if (onSimular)
            onSimular(slider_N.getValor(),
                      slider_cobertura.getValor() / 100.0,
                      slider_I0.getValor());
    }
}

void PanelParametros::dibujar(sf::RenderWindow& ventana) const
{
    ventana.draw(fondo);
    ventana.draw(titulo);
    slider_N.dibujar(ventana);
    slider_cobertura.dibujar(ventana);
    slider_I0.dibujar(ventana);
    ventana.draw(btn_fondo);
    ventana.draw(btn_texto);
}
