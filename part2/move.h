#ifndef MOVE_H
#define MOVE_H

class Move {
	public:
		Move();
		Move(int score, int index);
		Move(Move const &rhs);
		~Move();
		void setIndex(int i_index);
		void setScore(int i_score);

		int score;
		int index;
		int moveType;
};


#endif
