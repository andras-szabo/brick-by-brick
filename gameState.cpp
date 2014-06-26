#include "gameState.h"

cGameState::cGameState(cEngine& eng):
cState { eng, "game" }
{
    srand(time(NULL));  // Rand() is just fine for our purposes here
}

void cGameState::init()
{
    mIsActive = true;
    
    mNextPiece = static_cast<Piece>(1 + rand() % 7);
    mNewPieceNeeded = true;
    mTimeToMoveDown = sf::seconds((gkInitialFallTime - (mLevel * 0.2)) / 20);
    
    // Set up tetrominos in their configurations
    
    // O
    cTetromino  piece;
    std::vector<sf::Vector2i>   pos { { 0, 2 }, { 1, 2 }, { 0, 3 }, { 1, 3} };
    piece.mConfigCount = 1;
    piece.mParts.push_back(pos);
    piece.mColor = sf::Color::Blue;
    
    piece.mSprite.setTexture(rTextureHolder.get(TexID::gameBoyTex));
    piece.mSprite.setTextureRect(sf::IntRect(0,0,30,30));
    
    mPieces[Piece::O] = piece;
    
    // S
    piece.mConfigCount = 2;
    pos = std::vector<sf::Vector2i> { { 0, 3 }, { 1, 3 }, { 1, 2 }, { 2, 2 } };
    piece.mParts.clear();
    piece.mParts.push_back(pos);
    pos = std::vector<sf::Vector2i> { { 0, 1 }, { 0, 2 }, { 1, 2 }, { 1, 3 } };
    piece.mParts.push_back(pos);
    piece.mColor = sf::Color::Green;
    piece.mSprite.setTextureRect(sf::IntRect(0,31,30,30));
    
    mPieces[Piece::S] = piece;
    
    // Z
    piece.mParts.clear();
    piece.mConfigCount = 2;
    pos = std::vector<sf::Vector2i> { { 0, 2 }, { 1, 2 }, { 1, 3 }, { 2, 3 } };
    piece.mParts.push_back(pos);
    pos = std::vector<sf::Vector2i> { { 0, 3 }, { 0, 2 }, { 1, 2 }, { 1, 1 } };
    piece.mParts.push_back(pos);
    piece.mColor = sf::Color::Yellow;
    piece.mSprite.setTextureRect(sf::IntRect(0,61,30,30));
    mPieces[Piece::Z] = piece;
    
    // L
    piece.mParts.clear();
    piece.mConfigCount = 4;
    pos = std::vector<sf::Vector2i> { { 0, 1 }, { 0, 2 }, { 0, 3 }, { 1, 3 } };  // L
    piece.mParts.push_back(pos);
    pos = std::vector<sf::Vector2i> { { 0, 1 }, { 0, 2 }, { 1, 1 }, { 2, 1 } };  // I""
    piece.mParts.push_back(pos);
    pos = std::vector<sf::Vector2i> { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 1, 3 } }; //  "|
    piece.mParts.push_back(pos);
    pos = std::vector<sf::Vector2i> { { 0, 2 }, { 1, 2 }, { 2, 2 }, { 2, 1 } }; // __|
    piece.mParts.push_back(pos);
    piece.mColor = sf::Color::Cyan;
    
    piece.mSprite.setTextureRect(sf::IntRect(0,91,30,30));
    mPieces[Piece::L] = piece;
    
    // J
    piece.mParts.clear();
    piece.mConfigCount = 4;
    pos = std::vector<sf::Vector2i> { { 0, 3 }, { 1, 3 }, { 1, 2 }, { 1, 1 } };  // J
    piece.mParts.push_back(pos);
    pos = std::vector<sf::Vector2i> { { 0, 1 }, { 0, 2 }, { 1, 2 }, { 2, 2 } };  // |__
    piece.mParts.push_back(pos);
    pos = std::vector<sf::Vector2i> { { 0, 1 }, { 0, 2 }, { 0, 3 }, { 1, 1 } }; //  |"
    piece.mParts.push_back(pos);
    pos = std::vector<sf::Vector2i> { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 2, 2 } }; // ""I
    piece.mParts.push_back(pos);
    piece.mColor = sf::Color::Red;
    
    piece.mSprite.setTextureRect(sf::IntRect(0,121,30,30));
    mPieces[Piece::J] = piece;
    
    // T
    piece.mParts.clear();
    piece.mConfigCount = 4;
    pos = std::vector<sf::Vector2i> { { 0, 2 }, { 1, 2 }, { 2, 2 }, { 1, 3 } };  // T
    piece.mParts.push_back(pos);
    pos = std::vector<sf::Vector2i> { { 0, 2 }, { 1, 1 }, { 1, 2 }, { 1, 3 } };  // -|
    piece.mParts.push_back(pos);
    pos = std::vector<sf::Vector2i> { { 0, 2 }, { 1, 1 }, { 1, 2 }, { 2, 2 } }; //  _|_
    piece.mParts.push_back(pos);
    pos = std::vector<sf::Vector2i> { { 1, 1 }, { 1, 2 }, { 1, 3 }, { 2, 2 } }; // |-
    piece.mParts.push_back(pos);
    piece.mColor = sf::Color::Magenta;
    
    piece.mSprite.setTextureRect(sf::IntRect(0,150,30,30));
    
    mPieces[Piece::T] = piece;
    
    // I
    piece.mParts.clear();
    piece.mConfigCount = 2;
    pos = std::vector<sf::Vector2i> { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } };  // |
    piece.mParts.push_back(pos);
    pos = std::vector<sf::Vector2i> { { 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 } };  // --
    piece.mParts.push_back(pos);
    piece.mColor = sf::Color::White;
    
    mPieces[Piece::I] = piece;

    piece.mSprite.setTextureRect(sf::IntRect(30,0,30,30));
    mPieces[Piece::Itop] = piece;
    
    piece.mSprite.setTextureRect(sf::IntRect(30,31,30,30));
    mPieces[Piece::Iver] = piece;
    
    mPieces[Piece::Iver2] = piece;
    
    piece.mSprite.setTextureRect(sf::IntRect(30,58,30,30));
    mPieces[Piece::Ibottom] = piece;
 
    piece.mSprite.setTextureRect(sf::IntRect(61,0,30,30));
    mPieces[Piece::Ileft] = piece;

    piece.mSprite.setTextureRect(sf::IntRect(91,0,30,30));
    mPieces[Piece::Ihor] = piece;
    
    mPieces[Piece::Ihor2] = piece;
    
    piece.mSprite.setTextureRect(sf::IntRect(120,0,30,30));
    mPieces[Piece::Iright] = piece;
    
    // Setting up labels etc.
    
    tNext.setFont(rFontHolder.get(FontID::GBFont));
    tNext.setCharacterSize(24);
    tNext.setString("Next");
    tNext.setPosition(sf::Vector2f { 850, 450 });
    tNext.setColor(sf::Color::Green);

    tScore = tNext;
    tScore.setPosition(sf::Vector2f { 850, 300 });
    
    tLines = tNext;
    tLines.setPosition(sf::Vector2f { 850, 200 } );
    
    tLevel = tNext;
    tLevel.setPosition(sf::Vector2f { 850, 100 } );
    
}

void cGameState::reset()
{
    
}

void cGameState::cleanup()
{
    
}

void cGameState::render()
{
    // Render background
    // score
    // level
    
    // Render board and next piece:
    mBoard.render(rWindow);
    rWindow.draw(tNext);
    
    tScore.setString("Score:\n\t" + std::to_string(mScore));
    rWindow.draw(tScore);
    
    tLines.setString("Lines cleared:\n\t" + std::to_string(mLinesCleared));
    rWindow.draw(tLines);
    
    tLevel.setString("Level:\n\t" + std::to_string(mLevel));
    rWindow.draw(tLevel);

}

void cGameState::run()
{
    if ( mNewPieceNeeded )
    {
        auto followingPiece = static_cast<Piece>(1 + rand() % 7);
        mNewPieceNeeded = false;
        
        if ( !mBoard.newPiece(mNextPiece, followingPiece) )
        {
            // Call game over state, pass score param through engine
            rEngine.mCurrentScore = mScore;
            pushRequest("gameOver");
            return;
        }

        mNextPiece = followingPiece;
    }
    
    sf::Event event;
    while ( rWindow.pollEvent(event) )
    {
        if ( event.type == sf::Event::Closed )
        {
            clearRequest();
        }
        if ( event.type == sf::Event::KeyPressed )
        {
            switch ( event.key.code ) {
                case sf::Keyboard::Escape: {
                    // Pause
                    pushRequest("pause");
                    break;
                }
                default: break;
            }
        }
    }
    
    mDesiredMove = Move::nothing;
    
    // At given intervals, check if keys are pressed.
    
    if ( mKeyPressCoolDown <= sf::Time::Zero )
    {
        for ( const auto& i : rKeyBindings[rControlPreset] )
        {
            if ( sf::Keyboard::isKeyPressed(i.first) )
            {
                mDesiredMove = i.second;
                mKeyPressCoolDown = sf::seconds(gkKeyPressInterval);
            }
        }
    }
   
    auto timePassed = mClock.restart();
    mAccumulatedTime += timePassed;
    if ( mKeyPressCoolDown > sf::Time::Zero ) mKeyPressCoolDown -= timePassed;
    
    // If we have to move down, a right turns into a right-and-down,
    // a left into a left-and-down, etc.
    
    if ( mAccumulatedTime >= mTimeToMoveDown && mDesiredMove != Move::fall )
    {
        switch ( mDesiredMove ) {
            case Move::moveLeft: mDesiredMove = Move::moveDownLeft; break;
            case Move::moveRight: mDesiredMove = Move::moveDownRight; break;
            case Move::rotateLeft: mDesiredMove = Move::rotateDownLeft; break;
            case Move::rotateRight: mDesiredMove = Move::rotateDownRight; break;
            default: mDesiredMove = Move::moveDown; break;
        }
        
        mAccumulatedTime = sf::Time::Zero;
    }
    
    // Update the board with the desired move; it'll return "outcome" which
    // will tell us how many lines were cleared, and whether we need to spawn
    // a new piece.
    
    auto outcome = mBoard.update(mDesiredMove);
    
    if ( outcome > 1 && outcome < 6 )
    {

        // 1, 2, 3, or 4 lines are cleared
        mScore += (outcome - 1) * 10 * mLevel;
        mClearingLines = true;
        
        mLinesCleared += outcome - 1;
        mLevel = mLinesCleared / 10 + 1;
        
        mTimeToMoveDown = sf::seconds((gkInitialFallTime - (mLevel * 0.2)) / 20);
        
    } else if ( outcome == 6 || outcome == 1 )
    {
        // Blinking finished, lines have actually been cleared (outcome == 6)
        // or: need a new piece, but no lines were cleared (outcome == 1)
        if ( outcome == 1 ) mScore += mLevel;
        mClearingLines = false;
        mNewPieceNeeded = true;
    }
    
}