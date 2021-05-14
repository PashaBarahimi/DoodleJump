#ifndef NORMAL_PLATFORM_HPP
#define NORMAL_PLATFORM_HPP

#include "Platform.hpp"

class NormalPlatform : public Platform
{
public:
	NormalPlatform(const Point location, const sizeVector size, GameBoard* board, Doodler* doodler);
	Type getType() override;
};

#endif
