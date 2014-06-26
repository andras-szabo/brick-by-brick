#ifndef __sstc__gameState__
#define __sstc__gameState__

#include "state.h"
#include "globalConstants.h"
#include <SFML/Graphics.hpp>
#include "board.h"
#include <random>
#include "tetromino.h"
#include <map>


class cGameState : public cState {
public:
    cGameState(cEngine&);
    
    virtual void        init();
    virtual void        run();
    virtual void        cleanup();
    virtual void        reset();
    virtual void        render();

private:
    std::map<Piece, cTetromino> mPieces;
    cBoard              mBoard { 10, 22, mPieces };
    unsigned int        mLevel { 1 };
    unsigned int        mScore { 0 };
    Piece               mNextPiece;
    bool                mNewPieceNeeded { true };
    Move                mDesiredMove;
    sf::Clock           mClock;
    sf::Time            mAccumulatedTime;
    sf::Time            mTimeToMoveDown;
    bool                mClearingLines { false };
    unsigned int        mLinesCleared { 0 };

    sf::Time            mKeyPressCoolDown { sf::Time::Zero };
    
    sf::Text            tNext;
    sf::Text            tScore;
    sf::Text            tLines;
    sf::Text            tLevel;

};

#endif /* defined(__sstc__gameState__) */
