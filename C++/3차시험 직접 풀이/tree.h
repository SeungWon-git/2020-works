//-----------------------------------------------------------------------------
// Tree.h
//-----------------------------------------------------------------------------
#pragma once

#include "game.h"

class Tree : public Game {
	int height;							// ������ ����

public:
	Tree();

	virtual void render() const override;
};
