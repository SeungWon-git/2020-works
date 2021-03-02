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
std::string nations[3]{ "한국", "일본", "중국" };

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
	std::cout << "플레이어, 국가 - " << nations[nation] << ", 체력 - " << hp << std::endl;
}

