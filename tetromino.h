#ifndef __sstc__tetromino__
#define __sstc__tetromino__

#include <vector>
#include <SFML/Graphics.hpp>
#include "globalConstants.h"

struct cTetromino {
public:
    inline const std::vector<sf::Vector2i>& rotate(unsigned int config) const
    { return mParts[ config % mConfigCount ]; }
    
public:
    std::vector<std::vector<sf::Vector2i>>  mParts; // relative coords of various parts
                                                    // in a given configuration (rotation)
    int         mConfigCount { 0 };     // how many valid configurations are there?
    sf::Color   mColor;
    sf::Sprite  mSprite;
    
};

#endif /* defined(__sstc__tetromino__) */
