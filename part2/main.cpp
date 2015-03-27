#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include "board.h"


using namespace std;
using namespace boost;


//returns index of board for next move i.e. board[index] s.t. index/6 = row-1 and index%6=column
//ONLY WORKS FOR 2 PLAYERS - assuming maximizingPlayer is playerID 0 and minimizingPlayer is playerID 1
int minimax(Board board, int depth, bool maximizingPlayer)
{
	if(depth==0 || board.isBoardFull())
	{
		if(maximizingPlayer)
		{
			return board.getPlayerScore(0);
		}
		else
		{
			return board.getPlayerScore(1);
		}
	}

	if(maximizingPlayer)
	{
		int bestValue = INT_MIN;
		//todo:
		//for each child in vector of children from getChildrenIndices
		//try all three moves: paraDrop, deathBlitz, sabotage
		//compare heuristic with bestValue and store max as bestValue

		vector<int> maxNeighbors = board.getChildrenIndices(0);
		for(vector<int>::iterator it = maxNeighbors.begin(); it!=maxNeighbors.end(); ++it)
		{

		}

		return bestValue;
	}
	else
	{
		int bestValue = INT_MAX;
		//todo:
		//for each child in vector of children from getChildrenIndices
		//try all three moves: paraDrop, deathBlitz, sabotage
		//compare heuristic with bestValue and store max as bestValue

		vector<int> maxNeighbors = board.getChildrenIndices(0);
		for(vector<int>::iterator it = maxNeighbors.begin(); it!=maxNeighbors.end(); ++it)
		{
			
		}

		return bestValue;
	}

	return -1;
}

int main(int argc, char* argv[])
{

	if (argc!=3) 
	{
		cout << "Error, usage: ./mp2 [search flag] [scenario file name]" << endl;
		return 1;
	}
	
	char mode = argv[1][0];
	string scenario = argv[2];
	
	switch(mode)
	{
		default:
			cout << "Search flag mode: " << mode << endl;
			break;
	}

	Board* b = new Board();
	b->parseBoard(scenario);
	b->addPlayer("Blue");
	b->addPlayer("Green");

	b->paraDrop('D', 1, "Blue");
	b->paraDrop('C', 2, "Blue");
	b->paraDrop('D', 2, "Blue");
	b->paraDrop('E', 2, "Blue");
	b->paraDrop('C', 4, "Green");
	b->paraDrop('D', 4, "Green");
	b->printBoard();
	b->printScores();

	b->deathBlitz('D', 3, "Green");
	b->printBoard();
	b->printScores();

	b->deathBlitz('C', 3, "Blue");
	b->printBoard();
	b->printScores();

	b->sabotage('C', 1, "Green", 1.0);
	b->printBoard();
	b->printScores();
	cout << "Player 0 score: " << b->getPlayerScore(0) << endl;
	cout << "Blue's score " << b->getPlayerScore("Blue") << endl;
	cout << "Player 1 score: " << b->getPlayerScore(1) << endl;
	cout << "Blue's score " << b->getPlayerScore("Green") << endl;

	Board* c = new Board(*b);
	c->printBoard();

	delete b;
	delete c;
	return 0;
}
