#include "messageState.h"

cMessageState::cMessageState(cEngine& eng):
cState { eng, "message" }
{
    mBackground.setSize(sf::Vector2f { 800, 600 });
    mBackground.setPosition(sf::Vector2f { 200, 100 });
    mBackground.setOutlineColor(sf::Color::Green);
    mBackground.setOutlineThickness(5.0);
    mBackground.setFillColor(sf::Color(10, 20, 10, 230));
    
    tMessageOne.setFont(rFontHolder.get(FontID::GBFont));
    tMessageOne.setCharacterSize(28);
    
    tMessageTwo = tMessageOne;
}

void cMessageState::render()
{
    rWindow.draw(mBackground);
    rWindow.draw(tTitle);
    rWindow.draw(tMessageOne);
    rWindow.draw(tMessageTwo);
}

void cMessageState::init()
{
    mIsActive = true;
    
    // Check if the message to be displayed is a special
    // kind, e.g. __HISCORES__ or __GAMEOVER__.
    
    if ( rEngine.mStrParam == "__GAMEOVER__" )
    {
        tTitle = tMessageOne;
        tTitle.setColor(sf::Color::Green);
        tTitle.setString("Oh noes");
        
        std::string stmp = "Game over.\nYour score is " + std::to_string(rEngine.mCurrentScore) + "\n";
        
        if ( rEngine.mCurrentScore > rEngine.mLowestHiScore )
        {
            stmp += "\nThat is a new high score!\nEnter your name:\n";
            mNewHiScore = true;
            tMessageTwo.setString("Player1");
        }
        
        tMessageOne.setString(stmp);
        
        sf::Vector2f pos;
        pos.x = rView.getCenter().x - tMessageOne.getLocalBounds().width / 2.0;
        pos.y = rView.getCenter().y - tMessageOne.getLocalBounds().height / 2.0;
        
        tMessageOne.setPosition(pos);
        tMessageOne.setColor(sf::Color::White);

        tMessageTwo.setColor(sf::Color::White);
        tMessageTwo.setPosition(pos.x + 140, pos.y + 200);

    }
    
    if ( rEngine.mStrParam == "__HISCORES__" )
    {
        tTitle = tMessageOne;
        tTitle.setColor(sf::Color::Green);
        tTitle.setString("High scores");
        
        // Message one: names. Message two: scores. So we can format them a bit.
        std::string stmp1 {""}, stmp2 {""};
        
        for ( const auto& i : rEngine.mHiScores )
        {
            stmp1 = i.second + "\n" + stmp1;
            stmp2 = std::to_string(i.first) + "\n" + stmp2;
        }
        
        tMessageOne.setString(stmp1);
        tMessageTwo.setString(stmp2);
        
        tMessageOne.setColor(sf::Color::White);
        tMessageTwo.setColor(sf::Color::White);

        sf::Vector2f pos;
        pos.x = (rView.getCenter().x - 100) / 2.0 - tMessageOne.getLocalBounds().width / 2.0 + 200;
        pos.y = 300;
        
        tMessageOne.setPosition(pos);
        
        pos.x += rView.getCenter().x - 200;
        
        tMessageTwo.setPosition(pos);
    }

    sf::Vector2f pos;
    pos.x = (rView.getSize().x - tTitle.getLocalBounds().width) / 2.0;
    pos.y = 150;
    tTitle.setPosition(pos);
}

void cMessageState::run()
{
    sf::Event event;
    bool bkspace { false };
    while ( rWindow.pollEvent(event) )
    {
        if ( event.type == sf::Event::Closed )
        {
            clearRequest();
        }
        
        if ( mNewHiScore )
        {
            if ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace)
            {
                std::string s = tMessageTwo.getString();
                if (s.size() > 0)
                    s.pop_back();
                
                tMessageTwo.setString(s);
                bkspace = true;
            }
            if ( event.type == sf::Event::TextEntered && !bkspace )
            {
                std::string s = tMessageTwo.getString();
                if ( s.size() <= 8 ) tMessageTwo.setString(s + static_cast<char>(event.text.unicode) );
            }

            if ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return )
            {
                rEngine.mStrParam = "__RET_HISCORE__";
                rEngine.addHiScore(rEngine.mCurrentScore, tMessageTwo.getString());
                returnRequest("mainMenu");
            }
        }
        else
        if ( event.type == sf::Event::KeyPressed )
        {
            if ( rEngine.mStrParam == "__GAMEOVER__" )
            {
                returnRequest("mainMenu");
            }
            else
            {
                popRequest();
            }
        }
    }
}

void cMessageState::reset()
{
    
}

void cMessageState::cleanup()
{
    
}