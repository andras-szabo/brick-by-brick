#ifndef __sstc__menuState__
#define __sstc__menuState__

#include "engine.h"
#include "guiMenu.h"

class cMenuState : public cState {
public:
    cMenuState(cEngine&);
    
    virtual void    init();
    virtual void    run();
    virtual void    cleanup();
    virtual void    reset();
    virtual void    render();
    
private:
    cMenu           mMainMenu { rWindow };
    sf::Text        tTitle;
    sf::Text        tCredits;
};

#endif /* defined(__sstc__menuState__) */
