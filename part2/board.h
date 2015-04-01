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
#include <set>
#include <time.h>
#include <unistd.h>

#define DEBUG false
#define FAST true

class Board {
	public:
		Board();
		Board(std::vector< std::pair<int, int> > i_board, std::map<int, std::pair<int, std::string> > i_player_map, int heuristic);
		Board(Board const &other);
		~Board();
		void printBoard();
		void printScores();
		void addPlayer(std::string player_name);
		void paraDrop(char i_column, int i_row, std::string playerName);
		void deathBlitz(char i_column, int i_row, std::string playerName);
		void sabotage(char i_column, int i_row, std::string playerName, double gamma);
		void makeMove(int index, int playerID, int move, double gamma);
		int getPlayerScore(std::string playerName);
		int getPlayerScore(int playerID);
		int findHighestUnclaimedSquare();
		int findLowestUnclaimedSquare();
		bool isBoardFull();
		bool isRoot();
		bool isSameBoard(Board rhs);
		void deleteDescendants();
		std::set<int> getEmptyNeighboringSquares(int playerID);
		std::vector<int> getUnoccupiedIndices();
		std::vector< std::pair <int, int> > board; //first is score, second is playerID
		//maps playerID with int representing score and string e.g., blue, green, etc.
		//-1 is "unassigned", default score is 0
		std::map< int, std::pair <int, std::string> > player_map;
		int heuristic;
		void parseBoard(std::string scenario);

		Board* parent;
		std::vector<Board*>children;

	private:
		//helper methods
		bool checkFriendlyNeighbors(int column, int row, int playerID); //could be private
		bool checkEnemyNeighbors(int column, int row, int playerID); //could be private
		void conquerNeighbors(char i_column, int column, int row, int playerID);
		int getEnemyNeighborID(int column, int row, int playerID);
		int player_stoi(std::string playerName) ;
		std::vector<int> findNeighborIDs(int col, int row);
};


#endif
