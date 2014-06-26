#include "controlState.h"

cControlState::cControlState(cEngine& eng):
cState { eng, "controlMenu" }
{
    std::vector<std::string>    stmp;
    stmp.push_back("Default");
    stmp.push_back("Vim");
    stmp.push_back("WASD");

    mControlMenu.setup(stmp, rFontHolder.get(FontID::GBFont), 28,
                       sf::Vector2f { 250, -rView.getSize().y });
    
    mControlMenu.setTitle("Choose control preset", rFontHolder.get(FontID::GBFont),
                          30, sf::Color::Green, sf::Vector2f { -rView.getSize().x, 120 });
    
    stmp.clear();
    stmp.push_back("Move left: left arrow\nMove right: right arrow\nRotate: up arrow or\n\trShift\nQuick fall: down arrow\n\nPause: ESC");
    
    stmp.push_back("Move left: H\nMove right: L\nRotate: K or space\nQuick fall: J\n\nPause: ESC");

    stmp.push_back("Move left: A\nMove right: D\nRotate: W or space\nQuick fall: S\n\nPause: ESC");
    
    mControlMenu.setHints(stmp, rFontHolder.get(FontID::GBFont), 22, sf::Color::Green,
                          sf::Vector2f { 550, rView.getSize().y/2 - 30 } );
    
    mBackground.setSize(sf::Vector2f { 1000, 600 });
    mBackground.setPosition(sf::Vector2f { 100, 100 });
    mBackground.setOutlineColor(sf::Color::Green);
    mBackground.setOutlineThickness(5.0);
    mBackground.setFillColor(sf::Color(10, 20, 10, 230));
}

void cControlState::init()
{
    mIsActive = true;
}

void cControlState::reset()
{
    mControlMenu.reset();
}

void cControlState::render()
{
    rWindow.draw(mBackground);
    mControlMenu.render();
}

void cControlState::run()
{
    if ( mControlMenu.update() )
    {
        auto s = mControlMenu.getSelectedItem();
        if ( s == "Default" ) rControlPreset = 0;
        if ( s == "Vim" ) rControlPreset = 1;
        if ( s == "WASD" ) rControlPreset = 2;
        popRequest();
    }
}

void cControlState::cleanup()
{
    
}