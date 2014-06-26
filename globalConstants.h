#ifndef sstc_globalConstants_h
#define sstc_globalConstants_h

#include <SFML/Graphics.hpp>

enum class FontID   : unsigned int { GBFont };
enum class TexID    : unsigned int { introScreen, gameBoyTex };
enum class SAID     : unsigned int { nothing, push, pop, swap, returnTo, clear };   // Stack Action ID;
enum class Piece    : unsigned int { empty = 0, O = 1, S = 2, Z = 3, T = 4, J = 5, L = 6 , I = 7,
    Itop = 8, Iver = 9, Iver2 = 10, Ibottom = 11, Ileft = 12, Ihor = 13, Ihor2 = 14, Iright = 15 };
enum class Move     : unsigned int { nothing, moveLeft, moveRight, moveDownLeft, moveDown, moveDownRight,
    rotateLeft, rotateRight, rotateDownLeft, rotateDownRight, fall };

const std::string   gkVersion { "0.1" };

const sf::Vector2f  gkWindowSize { 800, 600 };
const sf::Vector2f  gkViewSize { 1200, 800 };

const float         gkIntroFadeInSecs { 1.5 };
const float         gkIntroProceedSecs { 5.0 };
const float         gkInitialFallTime { 6.0 };
const float         gkLineBlinkTimeSecs { 0.5 };
const float         gkKeyPressInterval { 1.0 / 10 };

#endif
