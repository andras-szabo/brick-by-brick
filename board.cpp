#include "board.h"
#include <iostream>

cBoard::cBoard(size_t x, size_t y, std::map<Piece, cTetromino>& pieces):
mSizeX { x },
mSizeY { y },
mPieces { pieces }
{
    // Set up board, fill it up with 0s
    for ( auto i = 0; i < mSizeX; ++i )
    {
        mCell.push_back(std::vector<int>(mSizeY, 0));
    }
        
    // Set up active & next piece representant
    sf::Vector2i v { 0, 0 };
    for ( auto i = 0; i < 4; ++i )
    {
        mActivePiece.push_back(v);
        mNextPiece.push_back(v);
    }
}

bool cBoard::newPiece(Piece p, Piece next)
{
    sf::Vector2i v { static_cast<int>(mSizeX / 2) - 1, 0 };

    int tmp = 0;
    for ( const auto& i : mPieces[p].rotate(0) )
    {
        mActivePiece[tmp++] = v + i;
    }
    
    tmp = 0;
    for ( const auto& i : mPieces[next].rotate(0) )
    {
        mNextPiece[tmp++] = i;
    }
    
    
    // Check if piece could be inserted into map

    mNextPieceType = next;
    mActivePieceType = p;
    mActiveRotations = 0;
    
    if ( fits(mActivePiece) )
    {
        mActivePos = v;
        return true;
    };
    
    return false;
}

bool cBoard::lineFull(size_t y) const
{
    for ( auto i = 0; i < mSizeX; ++i )
    {
        if ( mCell[i][y] == 0 ) return false;
    }
    return true;
}

int cBoard::newPieceArrived()
{
    // Returns the number of lines that this piece is going to clear

    // Put the piece on board, check if any of the lines are full
    mLinesToBeCleared.clear();
    
    bool newClearing { false };
    
    int count = 0;
    
    for ( auto& i : mActivePiece )
    {
        if ( mActivePieceType != Piece::I )
        {
            mCell[i.x][i.y] = static_cast<int>(mActivePieceType);
        } else
        {
            mCell[i.x][i.y] = 8 + (abs(mActiveRotations % 2) * 4) + count++;
        }
        
        if ( lineFull(i.y) )
        {
            mLinesToBeCleared.push_back(i.y);
            newClearing = true;
        }
        i.x = 0; i.y = 0;
    }
    
    if ( newClearing )
    {
        mClock.restart();
        mBlinkTimeLeft = sf::seconds(gkLineBlinkTimeSecs);
        mBlinking = true;

        for ( const auto& i : mLinesToBeCleared )
            for ( auto j = 0; j < mSizeX; ++j )
            {
                mCell[j][i] *= -1;
            }
    }
    
    return mLinesToBeCleared.size();
}

bool cBoard::fitIfPossible(int dx, int dy, int conf)
{
    auto tmp = mActivePiece;
    
    sf::Vector2i dv { dx, dy };
    
    int count = 0;
    for ( const auto& i : mPieces[mActivePieceType].rotate(mActiveRotations + conf) )
    {
        mActivePiece[count++] = mActivePos + i + dv;
    }
    
    if ( fits(mActivePiece) )
    {
        mActiveRotations += conf;
        mActivePos += dv;
        return true;
    }
                                            // If fits, leave it as it is and return true
    mActivePiece = tmp;                     // Otherwise load previous state, return false
    return false;
    
}

int cBoard::update(Move m)
{
    if ( !mBlinking )
    {
        switch ( m ) {
            case Move::nothing: return 0; break;
            case Move::moveLeft: fitIfPossible(-1, 0, 0); return 0; break;
            case Move::moveRight: fitIfPossible(1, 0, 0); return 0; break;
            case Move::moveDown: {
                if ( fitIfPossible(0, 1, 0) ) {
                    return 0;
                }
                else {
                    return newPieceArrived() + 1;
                }
                break;
            }
            case Move::moveDownLeft:
            {
                if ( fitIfPossible(-1, 1, 0) ) { return 0; }
                else if ( fitIfPossible(0, 1, 0) ) { return 0; }
                else return newPieceArrived() + 1;
                break;
            }
            case Move::moveDownRight:
            {
                if ( fitIfPossible(1, 1, 0) ) { return 0; }
                else if ( fitIfPossible(0, 1, 0) ) { return 0; }
                else return newPieceArrived() + 1;
                break;
            }
            
            case Move::rotateLeft:
            {
                fitIfPossible(0, 0, -1); return 0; break;
            }
                
            case Move::rotateRight:
            {
                fitIfPossible(0, 0, 1); return 0; break;
            }

            case Move::rotateDownLeft:
            {
                if ( fitIfPossible(0, 1, -1) ) { return 0; }
                else if ( fitIfPossible(0, 1, 0) ) { return 0; }
                else return newPieceArrived() + 1; break;
            }

            case Move::rotateDownRight:
            {
                if ( fitIfPossible(0, 1, 1) ) { return 0; }
                else if ( fitIfPossible(0, 1, 0) ) { return 0; }
                else return newPieceArrived() + 1; break;
            }
                
            case Move::fall:
            {
                while ( fitIfPossible(0, 1, 0) )
                {
                    
                }
                return newPieceArrived() + 1 ;
                break;
            }
        }
        
    }
    else    // blinking
    {
        mBlinkTimeLeft -= mClock.restart();
        if ( mBlinkTimeLeft > sf::Time::Zero )
        {
            // Do the actual blinking by flipping the sign bit of the
            // affected cells
            mBlinkOnOff = (mBlinkTimeLeft.asMilliseconds() / 100) % 2;
            return 0;   // things are as usual, no worries
            
        } else
        {
            // Have to remove things now.
            mBlinking = false;
            std::sort(std::begin(mLinesToBeCleared), std::end(mLinesToBeCleared));
            for ( const auto& i : mLinesToBeCleared )
            {
                clearLine(i);
            }
            
            return 6;   // end of blinking, new piece may proceed!
        }
    }
    
    return 0;   // only here to appease Xcode
}

void cBoard::clearLine(size_t y)
{
    for ( auto i = 0; i < mSizeX; ++i )
        for ( auto j = y; j > 1; --j )
            mCell[i][j] = mCell[i][j-1];
}

void cBoard::render(sf::RenderWindow& window)
{
    sf::RectangleShape shape;
    
    // First: stack.
    
    shape.setFillColor(sf::Color::Black);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.0);
    shape.setSize(sf::Vector2f(300, 600));
    shape.setPosition(450, 100);
    
    window.draw(shape);
    
    // Then existing cells, but only ones which we
    // can actually see, e.g. from row 2 onward
    
    for ( auto i = 0; i < mSizeX; ++i )
        for ( auto j = 2; j < mSizeY; ++j )
        {
            auto id = mCell[i][j];
            if ( id > 0 || ( id < 0 && !mBlinkOnOff) )
            {
                auto sprite = mPieces[static_cast<Piece>(abs(id))].mSprite;
                sprite.setPosition(450 + i * 30, 100 + (j-2) * 30);
                window.draw(sprite);
            }
        }
    
    // Then active piece, if not blinking
    
    if ( !mBlinking )
    {
        int count = 0;
        for ( const auto& i : mActivePiece )
        {
            if ( i.y > 1 )
            {
                if ( mActivePieceType != Piece::I )
                {
                    auto sprite = mPieces[mActivePieceType].mSprite;
                    sprite.setPosition(450 + i.x * 30, 100 + (i.y-2) * 30);
                    window.draw(sprite);
                }
                else
                {
                    // OK now careful with I:
                    auto sprite = mPieces[static_cast<Piece>(8 + (abs(mActiveRotations % 2) * 4) + count++)].mSprite;
                    
                    sprite.setPosition(450 + i.x * 30, 100 + (i.y-2) * 30);
                    window.draw(sprite);
                }
            }
        }
    }
    
    // Finally, render next piece
    
    int count = 0;
    for ( const auto& i : mNextPiece )
    {
        if ( mNextPieceType != Piece::I )
        {
            auto sprite = mPieces[mNextPieceType].mSprite;
            sprite.setPosition(875 + i.x * 30, 540 + (i.y-2) * 30);
            window.draw(sprite);
        }
        else
        {
            // OK now careful with I:
            auto sprite = mPieces[static_cast<Piece>(8 + count++)].mSprite;
            
            sprite.setPosition(875 + i.x * 30, 540 + (i.y-2) * 30);
            window.draw(sprite);
        }
    }
}