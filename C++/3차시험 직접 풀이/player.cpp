//-----------------------------------------------------------------------------
// Player.cpp
//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <random>
#include "game.h"
#include "player.h"

//enum class Nation { Korea = 0, Japan, China };
enum Nation { Korea = 0, Japan, China };
std::string nations[3]{ "�ѱ�", "�Ϻ�", "�߱�" };

std::default_random_engine drePlayer;
std::uniform_int_distribution<> uiHpPlayer( 50, 100 );
std::uniform_int_distribution<> uiNation( 0, 2 );

Player::Player() 
{
	nation = uiNation( drePlayer );
	hp = uiHpPlayer( drePlayer );
}

void Player::render() const 
{
	std::cout << "�÷��̾�, ���� - " << nations[nation] << ", ü�� - " << hp << std::endl;
}

