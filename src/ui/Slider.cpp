#include "ui/Slider.h"
#include "ui/Tema.h"
#include <sstream>
#include <iomanip>
#include <cmath>

Slider::Slider(const std::string& etiq, float px, float py, float pancho,
               double minVal, double maxVal, double valInicial,
               const sf::Font& f)
    : etiqueta(etiq), x(px), y(py), ancho(pancho),
      val_min(minVal), val_max(maxVal), valor_actual(valInicial),
      fuente(f),
      pista(sf::Vector2f(pancho, 4.f)),
      relleno(sf::Vector2f(0.f, 4.f)),
      knob(7.f)
{
    txt_etiqueta.setFont(fuente);
    txt_etiqueta.setCharacterSize(13);
    txt_etiqueta.setFillColor(Tema::TEXTO_DIM);
    txt_etiqueta.setString(etiqueta);

    txt_valor.setFont(fuente);
    txt_valor.setCharacterSize(13);
    txt_valor.setFillColor(Tema::TEXTO);

    pista.setFillColor(Tema::SLIDER_PISTA);
    relleno.setFillColor(Tema::SLIDER_RELLENO);
    knob.setFillColor(Tema::SLIDER_KNOB);
    knob.setOrigin(7.f, 7.f);

    actualizarGeometria();
}

void Slider::setValor(double v)
{
    valor_actual = std::max(val_min, std::min(val_max, v));
    actualizarGeometria();
}

void Slider::actualizarGeometria()
{
    float px = valorAPosicion();

    pista.setPosition(x, y + 22.f);
    relleno.setPosition(x, y + 22.f);
    relleno.setSize(sf::Vector2f(px - x, 4.f));

    knob.setPosition(px, y + 24.f);

    txt_etiqueta.setPosition(x, y);
    txt_valor.setString(formatearValor());
    txt_valor.setPosition(x + ancho - txt_valor.getGlobalBounds().width, y);
}

void Slider::manejarEvento(const sf::Event& evento, const sf::RenderWindow& ventana)
{
    sf::Vector2i mouse = sf::Mouse::getPosition(ventana);

    if (evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left)
    {
        sf::FloatRect zona(x - 10.f, y + 14.f, ancho + 20.f, 20.f);
        if (zona.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
            arrastrando = true;
    }

    if (evento.type == sf::Event::MouseButtonReleased)
        arrastrando = false;

    if (arrastrando && evento.type == sf::Event::MouseMoved)
    {
        double nuevo = posicionAValor(static_cast<float>(mouse.x));
        valor_actual = nuevo;
        actualizarGeometria();
        if (onChange) onChange(valor_actual);
    }
}

void Slider::dibujar(sf::RenderWindow& ventana) const
{
    ventana.draw(pista);
    ventana.draw(relleno);
    ventana.draw(knob);
    ventana.draw(txt_etiqueta);
    ventana.draw(txt_valor);
}

float Slider::valorAPosicion() const
{
    double t = (valor_actual - val_min) / (val_max - val_min);
    return x + static_cast<float>(t) * ancho;
}

double Slider::posicionAValor(float px) const
{
    double t = (px - x) / ancho;
    t = std::max(0.0, std::min(1.0, t));
    double v = val_min + t * (val_max - val_min);
    // Si el rango es >= 100, redondear a entero
    if ((val_max - val_min) >= 100.0)
        v = std::round(v);
    return std::max(val_min, std::min(val_max, v));
}

std::string Slider::formatearValor() const
{
    std::ostringstream ss;
    double rango = val_max - val_min;
    if (rango >= 100.0)
    {
        long long v = static_cast<long long>(std::round(valor_actual));
        if (v >= 1'000'000)
            ss << std::fixed << std::setprecision(1) << v / 1'000'000.0 << "M";
        else if (v >= 1000)
            ss << v / 1000 << "k";
        else
            ss << v;
    }
    else
    {
        ss << std::fixed << std::setprecision(1) << valor_actual << "%";
    }
    return ss.str();
}
