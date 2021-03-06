#include "move.h"

Move::Move()
{
	score = 0;
	index = -1;
	moveType = -1;
}
Move::Move(int i_score, int i_index)
{
	score = i_score;
	index = i_index;
	moveType = -1;
}
Move::Move(Move const &rhs)
{
	score = rhs.score;
	index = rhs.index;
	moveType = rhs.moveType;
}
Move::~Move()
{

}
void Move::setIndex(int i_index)
{
	if(index<0)
		index = i_index;
}
void Move::setScore(int i_score)
{
	score = i_score;
}
