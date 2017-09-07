#pragma once
#include "Character.h"
class Ghost :
	public Character
{
private:
	enum State {
		Hunting,
		Retreating,
		Scared
	} state;
public:
	Ghost();
	~Ghost();
};

