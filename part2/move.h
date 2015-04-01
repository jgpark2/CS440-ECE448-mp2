#ifndef MOVE_H
#define MOVE_H

class Move {
	public:
		Move();
		Move(int score, int index);
		Move(Move const &rhs);
		~Move();
		void setIndex(int i_index);

		int score;
		int index;
};


#endif
