//-----------------------------------------------------------------------------
// Game.h
//-----------------------------------------------------------------------------
#pragma once

#include <iostream>

class Game {
public:
	Game() = default;

	Game( const Game& ) {
		std::cout << "�� ������ ��µǸ� ���캸��" << std::endl;
	}

	virtual void render() const = 0;

	friend Player;
};

