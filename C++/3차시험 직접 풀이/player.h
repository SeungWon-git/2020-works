//-----------------------------------------------------------------------------
// Player.h
//-----------------------------------------------------------------------------
#pragma once

#include "game.h"

class Player : public Game {
	int nation;							// ���� 0 - Korea,  1 - Japan, 2 - China
	int hp;								// ü��
public:
	Player(); 

	virtual void render() const override;

	void setHp( int n ) {
		hp = n;
	}

	int getHp() const {
		return hp;
	}

	int getNation() const {
		return nation;
	}
};

