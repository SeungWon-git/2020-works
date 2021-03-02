//-----------------------------------------------------------------------------
// Monster.h
//-----------------------------------------------------------------------------
#pragma once

#include "game.h"

class Monster : public Game {
	int hp;								// Ã¼·Â

public:
	Monster();

	virtual void render() const override;
};
