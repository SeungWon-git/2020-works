//-----------------------------------------------------------------------------
// Monster.h
//-----------------------------------------------------------------------------
#pragma once

#include "game.h"

class Monster : public Game {
	int hp;								// ü��

public:
	Monster();

	virtual void render() const override;
};
