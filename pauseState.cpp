#include "pauseState.h"

cPauseState::cPauseState(cEngine& eng):
cState { eng, "pauseMenu" }
{
    std::vector<std::string>    stmp; 
    stmp.push_back("Controls");
    stmp.push_back("Quit");
    stmp.push_back("Resume");
    
    mPauseMenu.setup(stmp, rFontHolder.get(FontID::GBFont), 28,
                     -rView.getSize());
    
    mPauseMenu.setTitle("Pause", rFontHolder.get(FontID::GBFont), 30, sf::Color::Green,
                        sf::Vector2f { -rView.getSize().x, 220 }  );
    
    // Set up background shape
    
    mBackground.setPosition(sf::Vector2f { 200, 200 });
    mBackground.setSize(sf::Vector2f { 800, 400 });
    mBackground.setOutlineColor(sf::Color::Green);
    mBackground.setOutlineThickness(5.0);
    mBackground.setFillColor(sf::Color(10, 20, 10, 230));
}

void cPauseState::init()
{
    mIsActive = true;
}

void cPauseState::reset()
{
    mPauseMenu.reset();
}

void cPauseState::render()
{
    // Pause state is rendered also when not active!
    rWindow.draw(mBackground);
    mPauseMenu.render();
}

void cPauseState::run()
{
    if ( mPauseMenu.update() )
    {
        auto s = mPauseMenu.getSelectedItem();
        if ( s == "Quit" )
        {
            returnRequest("mainMenu");
        } else
            if ( s == "Resume" || s == "__BACK__" )
            {
                popRequest();
            }
        else
            if ( s == "Controls" )
            {
                pushRequest("controlMenu");
            }
    }
}

void cPauseState::cleanup()
{
    
}