#include "menuState.h"

cMenuState::cMenuState(cEngine& eng):
cState { eng, "mainMenu" }
{
    std::vector<std::string> stmp;
    stmp.push_back("New game");
    stmp.push_back("Controls");
    stmp.push_back("High scores");
    stmp.push_back("Quit");
    
    
    mMainMenu.setup( stmp, rFontHolder.get(FontID::GBFont), 36, sf::Vector2f(-rView.getSize().x, 390));

    tTitle.setString("Brick\n\t  by\n\t\t brick");
    tTitle.setFont(rFontHolder.get(FontID::GBFont));
    tTitle.setCharacterSize(50);
    tTitle.setColor(sf::Color::Green);
    tTitle.setPosition(260, 100);
    
    tCredits = tTitle;
    tCredits.setCharacterSize(12);
   
    std::string s = "2014. Andras Szabo - SFML: sfml-dev.org - Font design by Patrick Lauke";
    tCredits.setString(s);
    tCredits.setColor(sf::Color(10,50,10));
    tCredits.setPosition(rView.getCenter().x - tCredits.getLocalBounds().width / 2.0, 760);
}

void cMenuState::init()
{
    mIsActive = true;
}

void cMenuState::reset()
{
    mIsActive = true;
    mMainMenu.reset();
}

void cMenuState::render()
{
    rWindow.draw(tCredits);
    if ( !mIsActive ) return;
    rWindow.draw(tTitle);
    mMainMenu.render();
}

void cMenuState::run()
{
    if ( mMainMenu.update() )
    {
        auto s = mMainMenu.getSelectedItem();

        if ( s == "Quit" )
        {
            clearRequest();
        } else
        if ( s == "New game")
        {
            pushRequest("game");
        }
        else
        if ( s == "Controls" )
        {
            pushRequest("controlMenu");
        }
        else
        if ( s == "High scores")
        {
            pushRequest("hiScore");
        }
        else
        {
            mMainMenu.reset();
        }
    }
}

void cMenuState::cleanup()
{
    
}