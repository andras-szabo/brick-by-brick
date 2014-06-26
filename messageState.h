#ifndef __sstc__messageState__
#define __sstc__messageState__

#include "state.h"
#include <SFML/Graphics.hpp>

class cMessageState : public cState {
public:
    cMessageState(cEngine&);
    
    virtual void        init();
    virtual void        run();
    virtual void        reset();
    virtual void        cleanup();
    virtual void        render();
    
private:
    sf::RectangleShape      mBackground;
    sf::Text                tMessageOne;
    sf::Text                tMessageTwo;
    sf::Text                tTitle;
    bool                    mNewHiScore { false };
    
};

#endif /* defined(__sstc__messageState__) */
