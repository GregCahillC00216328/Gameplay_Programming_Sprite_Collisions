#include <iostream>
#include <NPC.h>
#include <Idle.h>
#include <Debug.h>

NPC::NPC() : GameObject()
{
	m_animation.setCurrent(new Idle());
	m_animation.setPrevious(new Idle());
}

NPC::NPC(const AnimatedSprite& s) : GameObject(s)
{
	m_animation.setCurrent(new Idle());
	m_animation.setPrevious(new Idle());
}

NPC::~NPC()
{
}

AnimatedSprite& NPC::getAnimatedSprite()
{
	int frame = m_animated_sprite.getCurrentFrame();
	m_animated_sprite.setTextureRect(m_animated_sprite.getFrame(frame));
	return m_animated_sprite;
}

void NPC::handleInput(Input in)
{
	DEBUG_MSG("Handle Input");

	switch (in.getCurrent())
	{
	case Input::Action::IDLE:
		//std::cout << "Player Idling" << std::endl;
		m_animation.idle();
		break;
	case Input::Action::UP:
		//std::cout << "Player Up" << std::endl;
		m_animation.climbing();
		break;
	case Input::Action::LEFT:
		//std::cout << "Player Left" << std::endl;
		m_animation.jumping();
		break;
	case Input::Action::RIGHT:
		//std::cout << "Player Idling" << std::endl;
		m_animation.jumping();
		break;
	default:
		break;
	}
}

void NPC::update()
{
	//std::cout << "Handle Update" << std::endl;
	m_animated_sprite.update();
}