#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

class Other : public Actor
{
	RTTI_DECLARATIONS(Other, Actor)

public:
	Other(const Vector2& position);
};