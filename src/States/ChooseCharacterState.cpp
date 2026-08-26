#include "States/ChooseCharacterState.hpp"
#include "Core/Game.hpp"
#include "States/StateIdentifiers.hpp"
#include <iostream>

ChooseCharacterState::ChooseCharacterState(Context context) : State(context)
{

}


void ChooseCharacterState::handleInput()
{
}

// Do the input only one time.
void ChooseCharacterState::handleEvent(const sf::Event& event)
{
}

//Do the input or whatever at each frames.
void ChooseCharacterState::update(float deltaTime)
{

}

void ChooseCharacterState::draw(sf::RenderWindow& window)
{
	
}


