//-----------------------------------------------------------------------------
// Tree.h
//-----------------------------------------------------------------------------
#pragma once

#include "game.h"

class Tree : public Game {
	int height;							// 나무의 높이

public:
	Tree();

	virtual void render() const override;
};
