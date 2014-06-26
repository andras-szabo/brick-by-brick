#include "engine.h"
#include "introState.h"
#include "menuState.h"
#include "gameState.h"
#include "pauseState.h"
#include "controlState.h"
#include "messageState.h"
#include "ResourcePath.hpp"
#include <fstream>

cEngine::cEngine()
{
    mWindow.create(sf::VideoMode(gkWindowSize.x, gkWindowSize.y), " Brick by brick " + gkVersion);
    mWindow.setVerticalSyncEnabled(true);
    mWindow.setKeyRepeatEnabled(false);
    
    mFontHolder.load(FontID::GBFont, "Super-Mario-Land-Game-Boy.ttf");
    mTextureHolder.load(TexID::introScreen, "sisu_splash.png");
    mTextureHolder.load(TexID::gameBoyTex, "gbttrs.png");
    
    mView.setSize(gkViewSize.x, gkViewSize.y);
    mView.setCenter(gkViewSize.x / 2, gkViewSize.y / 2);
    mView.setViewport(sf::FloatRect(0,0,1,1));

    mWindow.setView(mView);
    
    // Set up key bindings
    
    std::map<sf::Keyboard::Key, Move>   kb;
    
    kb[sf::Keyboard::Key::Left] = Move::moveLeft;
    kb[sf::Keyboard::Key::Right] = Move::moveRight;
    kb[sf::Keyboard::Key::Up] = Move::rotateRight;
    kb[sf::Keyboard::Key::RShift] = Move::rotateLeft;
    kb[sf::Keyboard::Key::Down] = Move::fall;
    
    mKeyBindings.push_back(kb);
    
    kb.clear();
    
    kb[sf::Keyboard::Key::H] = Move::moveLeft;
    kb[sf::Keyboard::Key::L] = Move::moveRight;
    kb[sf::Keyboard::Key::K] = Move::rotateRight;
    kb[sf::Keyboard::Key::Space] = Move::rotateLeft;
    kb[sf::Keyboard::Key::J] = Move::fall;
    
    mKeyBindings.push_back(kb);
    
    kb.clear();

    kb[sf::Keyboard::Key::A] = Move::moveLeft;
    kb[sf::Keyboard::Key::D] = Move::moveRight;
    kb[sf::Keyboard::Key::Space] = Move::rotateRight;
    kb[sf::Keyboard::Key::W] = Move::rotateLeft;
    kb[sf::Keyboard::Key::S] = Move::fall;

    mKeyBindings.push_back(kb);
    
    // Load hi scores
    std::ifstream in { resourcePath() + "hiscore.dat" };
    if ( in.fail() )
    {
        // No hiscore file just yet, so let's create it
        in.close();
        
        mHiScores[100] = "Fran";
        mHiScores[200] = "Liam";
        mHiScores[400] = "Noel";
        mHiScores[600] = "Alex";
        mHiScores[800] = "Ringo";
        mHiScores[1000] = "George";
        mHiScores[1200] = "Paul";
        mHiScores[1400] = "John";
        
        std::ofstream out { resourcePath() + "hiscore.dat" };
        for ( const auto& i : mHiScores )
        {
            out << i.first << i.second << "\n";
        }
        out.close();
        
    } else  // hiScore file found
    {
        mHiScores.clear();
        unsigned int utmp;
        std::string stmp;
        while ( in >> utmp >> stmp )
        {
            mHiScores[utmp] = stmp;
        }
        in.close();
    }
    
    mLowestHiScore = std::begin(mHiScores)->first;  // That's all you have to beat
                                                    // to make it onto the hall of fame :)
}

void cEngine::addHiScore(unsigned int score, const std::string& name)
{
    mHiScores[score] = name;
    mHiScores.erase(std::begin(mHiScores)); // When adding new hiscore, remove previous lowest
    
    std::ofstream out { resourcePath() + "hiscore.dat" };
    for ( const auto& i : mHiScores )
    {
        out << i.first << i.second << "\n";
    }
    out.close();
    
}

template <typename T>
void cEngine::launchState()
{

    // Create new state ( initializing through constructor to which "*this,"
    // i.e. ref to the engine, is passed.
    
    std::unique_ptr<cState> p { new T { *this } };
    p->init();
    mStateStack.push_back(std::move(p));
}

void cEngine::request(SAID action, const std::string& s)
{
    if ( mCurrentStackAction == SAID::nothing )
    {
        mCurrentStackAction = action;
        mStackParam = s;
    }
}

void cEngine::executeStackChanges()
{
    // Parameters expected in mCurrentStackAction and mStackParam
    switch ( mCurrentStackAction ) {
        case SAID::pop:
        {
            popState();
            break;
        }
        case SAID::push:
        {
            pushState(mStackParam);
            break;
        }
        case SAID::swap:
        {
            swapState(mStackParam);
            break;
        }
        case SAID::returnTo:
        {
            returnToState(mStackParam);
            break;
        }
        case SAID::clear:
        {
            clearStateStack();
            break;
        }
        default: break;
    }
    
    // Reset stack requests:
    
    mCurrentStackAction = SAID::nothing;
}

void cEngine::pushState(const std::string& s)
{
    if ( !mStateStack.empty() ) ( *mStateStack.rbegin())->deactivate();
    
    if ( s == "mainMenu" ) launchState<cMenuState>();
    if ( s == "game" ) launchState<cGameState>();
    if ( s == "pause" ) launchState<cPauseState>();
    if ( s == "controlMenu" ) launchState<cControlState>();
    if ( s == "hiScore" ) { mStrParam = "__HISCORES__"; launchState<cMessageState>(); }
    if ( s == "gameOver" ) { mStrParam = "__GAMEOVER__"; launchState<cMessageState>(); }
}

void cEngine::popState()
{
    if ( mStateStack.empty() ) return;
 
    // If state stack not empty: clean up after state that's being popped,
    // then simply pop it. mStateStack is a vector of unique_ptrs, so
    // no need to worry about memory leaks.
    
    (*mStateStack.rbegin())->cleanup();
    mStateStack.pop_back();
    if ( !mStateStack.empty() )
        (*mStateStack.rbegin())->reset();
}

void cEngine::swapState(const std::string& s)
{
    // Pops the current state and pushes onto the stack a new one to
    // replace it.
    
    popState();
    if ( !mStateStack.empty() ) (*mStateStack.rbegin())->deactivate();
    pushState(s);
}

void cEngine::returnToState(const std::string& s)
{
    
    // Pop states until finding requested one. If no such state,
    // this will terminate the program.
    
    while (!mStateStack.empty() && (*mStateStack.rbegin())->mName != s )
        popState();
}

void cEngine::clearStateStack()
{
    // Clear state stack: terminate program
    
    while ( !mStateStack.empty() )
        popState();
}

void cEngine::run()
{
    launchState<cIntroState>();
    
    // Main loop
    
    while ( !mStateStack.empty() )
    {
        mWindow.clear();
        
        for ( const auto& i : mStateStack )
            i->render();

        mWindow.display();
        (*mStateStack.rbegin())->run();
        
        executeStackChanges();
    }
}