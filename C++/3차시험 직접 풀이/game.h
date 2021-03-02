//-----------------------------------------------------------------------------
// Game.h
//-----------------------------------------------------------------------------
#pragma once

#include <iostream>

class Game {
public:
	Game() = default;

	Game( const Game& ) {
		std::cout << "이 문장이 출력되면 살펴보자" << std::endl;
	}

	virtual void render() const = 0;

	friend Player;
};

