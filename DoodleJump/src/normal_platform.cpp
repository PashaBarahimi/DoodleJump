#include "include/normal_platform.hpp"

Platform::Type NormalPlatform::getType()
{
	return Type::Normal;
}

NormalPlatform::NormalPlatform(const Point location, const sizeVector size, GameBoard *board, Doodler *doodler) : Platform(location, size, board, doodler) {}
