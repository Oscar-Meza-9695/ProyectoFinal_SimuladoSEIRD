#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

/**
 * Slider.h
 * 
 * Widget de slider horizontal con etiqueta, valor numérico y callback.
 * 
 */
class Slider
{
public:
    /**
     * @param etiqueta  Texto que aparece encima del slider
     * @param x, y      Posición superior izquierda
     * @param ancho     Longitud de la pista
     * @param minVal    Valor mínimo
     * @param maxVal    Valor máximo
     * @param valInicial Valor inicial
     */
    Slider(const std::string& etiqueta,
           float x, float y, float ancho,
           double minVal, double maxVal, double valInicial,
           const sf::Font& fuente);

    void manejarEvento(const sf::Event& evento, const sf::RenderWindow& ventana);
    void dibujar(sf::RenderWindow& ventana) const;

    double getValor() const { return valor_actual; }
    void   setValor(double v);

    // Callback que se llama cuando el usuario mueve el knob
    std::function<void(double)> onChange;

private:
    std::string etiqueta;
    float x, y, ancho;
    double val_min, val_max, valor_actual;
    bool arrastrando = false;

    const sf::Font& fuente;

    sf::RectangleShape pista;
    sf::RectangleShape relleno;
    sf::CircleShape    knob;
    sf::Text           txt_etiqueta;
    sf::Text           txt_valor;

    void actualizarGeometria();
    float valorAPosicion() const;
    double posicionAValor(float px) const;

    // Formatear el valor según el rango (entero o decimal)
    std::string formatearValor() const;
};
