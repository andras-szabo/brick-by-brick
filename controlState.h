#ifndef __sstc__controlState__
#define __sstc__controlState__

#include "state.h"
#include "guiMenu.h"

class cControlState : public cState {
public:
    cControlState(cEngine&);
    
    virtual void        init();
    virtual void        run();
    virtual void        reset();
    virtual void        cleanup();
    virtual void        render();

private:
    cMenu               mControlMenu { rWindow };
    sf::RectangleShape  mBackground;
    
};

#endif /* defined(__sstc__controlState__) */
