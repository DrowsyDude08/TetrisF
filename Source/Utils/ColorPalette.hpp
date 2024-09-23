#ifndef COLORPALETTE_HPP
#define COLORPALETTE_HPP

#include <SFML/Graphics.hpp>
#include <array>

struct ColorPalette{
    ColorPalette()
        : defaultButtonColor(183, 180, 242),
        selectedButtonColor(159, 110, 227),
        defaultOutlineColor(178, 73, 174),
        selectedOutlineColor(200, 100, 150),
        defaultTextColor(75, 0, 130),
        selectedTextColor(0, 0, 0),
        background(199, 222, 252),
        textField(250, 250, 250),
        saveNameColor(46, 139, 87){}
        

    sf::Color defaultButtonColor;
    sf::Color selectedButtonColor;
    sf::Color defaultOutlineColor;
    sf::Color selectedOutlineColor;
    sf::Color defaultTextColor;
    sf::Color selectedTextColor;
    sf::Color background;
    sf::Color textField;
    sf::Color saveNameColor;
};

#endif //COLORPALETTE_HPP
