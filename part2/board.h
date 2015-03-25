#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#include <utility>
#include <map>

class Board {
	public:
		Board();
		Board(std::vector< std::pair<int, int> > i_board, std::map<int, std::pair<int, std::string> > i_player_map);
		Board(Board const &other);
		~Board();
		void printBoard();
		void printScores();
		void addPlayer(std::string player_name);
		void paraDrop(char i_column, int row, std::string playerName);
		void deathBlitz(char i_column, int row, std::string playerName);

		std::vector< std::pair <int, int> > board; //first is score, second is playerID
		
		//maps playerID with int representing score and string e.g., blue, green, etc.
		//-1 is "unassigned", default score is 0
		std::map< int, std::pair <int, std::string> > player_map;

		void parseBoard(std::string scenario);

	private:
		//helper methods
		bool checkNeighbors(int column, int row, int playerID); //could be private
};


#endif
