#include "move.h"

Move::Move()
{
	score = 0;
	index = -1;
}
Move::Move(int i_score, int i_index)
{
	score = i_score;
	index = i_index;
}
Move::Move(Move const &rhs)
{
	score = rhs.score;
	index = rhs.index;
}
Move::~Move()
{

}