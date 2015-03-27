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
#include <time.h>

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
		void conquerNeighbors(int column, int row, int playerID);
		void deathBlitz(char i_column, int row, std::string playerName);
		int getPlayerScore(string playerName);
		int findHighestUnclaimedSquare();

		std::vector< std::pair <int, int> > board; //first is score, second is playerID
		
		//maps playerID with int representing score and string e.g., blue, green, etc.
		//-1 is "unassigned", default score is 0
		std::map< int, std::pair <int, std::string> > player_map;

		void parseBoard(std::string scenario);

	private:
		//helper methods
		bool checkFriendlyNeighbors(int column, int row, int playerID); //could be private
		bool checkEnemyNeighbors(int column, int row, int playerID); //could be private
		void conquerNeighbors(int column, int row, int playerID);
};


#endif
