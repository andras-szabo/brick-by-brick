#ifndef __sstc__pauseState__
#define __sstc__pauseState__

#include "state.h"
#include "guiMenu.h"

class cPauseState : public cState {
public:
    cPauseState(cEngine&);
    
    virtual void        init();
    virtual void        run();
    virtual void        reset();
    virtual void        cleanup();
    virtual void        render();
    
private:
    cMenu               mPauseMenu { rWindow };
    sf::RectangleShape  mBackground;
    
};

#endif /* defined(__sstc__pauseState__) */
