#ifndef __sstc__board__
#define __sstc__board__

#include <SFML/Graphics.hpp>
#include "globalConstants.h"
#include <vector>
#include "tetromino.h"

class cBoard {
public:
    cBoard(size_t x, size_t y, std::map<Piece, cTetromino>& pieces);
    
    bool                newPiece(Piece, Piece);
    int                 update(Move);
    void                render(sf::RenderWindow&);
    inline int          at(size_t x, size_t y) { return valid(x,y) ? mCell[x][y] : 0; }
    
private:
    inline bool         valid(size_t x, size_t y) { return x < mSizeX && y < mSizeY; }
    inline bool         fits(const std::vector<sf::Vector2i>& vec) {
                                for ( const auto& i : vec )
                                    {
                                        if ( !valid(i.x, i.y) ) return false;
                                        if ( mCell[i.x][i.y] !=  0 ) return false;
                                    }
                                return true;
                                }
    bool                fitIfPossible(int, int, int);
    int                 newPieceArrived();
    bool                lineFull(size_t y) const;
    void                clearLine(size_t y);

private:
    std::vector<std::vector<int>>       mCell;
    std::vector<sf::Vector2i>           mActivePiece;
    std::vector<sf::Vector2i>           mNextPiece;
    Piece                               mActivePieceType;
    Piece                               mNextPieceType;
    
    int                                 mActiveRotations { 0 };
    sf::Vector2i                        mActivePos;
    size_t                              mSizeX, mSizeY;
    std::map<Piece, cTetromino>&        mPieces;
    std::vector<size_t>                 mLinesToBeCleared;
    bool                                mBlinking { false };    // blinking during line clearance
    sf::Clock                           mClock;
    sf::Time                            mBlinkTimeLeft { sf::seconds(gkLineBlinkTimeSecs) };
    bool                                mBlinkOnOff { false };
};

#endif /* defined(__sstc__board__) */
