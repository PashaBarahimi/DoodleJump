#include "Pistol.hpp"

Pistol::Pistol(GameBoard* board, sizeVector size, Point topLeftLocation) : board_(board), size_(size), topLeftLocation_(topLeftLocation) { }

void Pistol::move()
{
	topLeftLocation_.y -= static_cast<int>(PISTOL_SPEED * board_->getDelay());
}

void Pistol::moveWithCamera(int pixel)
{
	topLeftLocation_.y += pixel;
}
