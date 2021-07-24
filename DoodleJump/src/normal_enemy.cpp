#include "include/normal_enemy.hpp"

NormalEnemy::NormalEnemy(Point topLeftLocation, sizeVector size, GameBoard *board, Doodler *doodler) : Enemy(topLeftLocation, size, board, doodler) {}

Enemy::Type NormalEnemy::getType()
{
	return Type::Normal;
}
