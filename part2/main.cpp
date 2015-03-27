#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include "board.h"


using namespace std;
using namespace boost;


//returns index of board for next move i.e. board[index] s.t. index/6 = row-1 and index%6=column
//ONLY WORKS FOR 2 PLAYERS - assuming maximizingPlayer is playerID 0 and minimizingPlayer is playerID 1
//move: 0 = paraDrop, 1 = deathBlitz, 2 = sabotage
//nodes_expanded: put in 0 for initial call
//returns a vector that contains heuristic at 0, number of nodes expanded at 1
vector<int> minimax(Board board, int depth, bool maximizingPlayer, double gamma, int nodes_expanded)
{
	if(depth==0 || board.isBoardFull())
	{
		if(maximizingPlayer)
		{
			vector <int> retval;
			retval.push_back(board.getPlayerScore(0));
			retval.push_back(nodes_expanded);
			return retval;
		}
		else
		{
			vector <int> retval;
			retval.push_back(board.getPlayerScore(1));
			retval.push_back(nodes_expanded);
			return retval;		
		}
	}

	if(maximizingPlayer)
	{
		int bestValue = INT_MIN;
		
		//DOUBLE CHECK THIS:
		//for each child in vector of children from getChildrenIndices
		//try all three moves: paraDrop, deathBlitz, sabotage
		//compare heuristic with bestValue and store max as bestValue

		vector<int> maxNeighbors = board.getChildrenIndices(0);
		for(vector<int>::iterator it = maxNeighbors.begin(); it!=maxNeighbors.end(); ++it)
		{
			int test_val;

			Board* paraDrop_board = new Board(board);
			paraDrop_board->makeMove(*it, 0, 0, gamma);//fourth param is gamma, any double will work
			test_val = (minimax(*paraDrop_board, depth-1, false, gamma, nodes_expanded+1))[0];
			test_val = max(bestValue, test_val);

			Board* deathBlitz_board = new Board(board);
			deathBlitz_board->makeMove(*it, 0, 1, gamma);//fourth param is gamma, any double will work
			test_val = (minimax(*deathBlitz_board, depth-1, false, gamma, nodes_expanded+1))[0];
			test_val = max(bestValue, test_val);

			Board* sabotage_board = new Board(board);
			sabotage_board->makeMove(*it, 0, 2, gamma);
			test_val = (minimax(*sabotage_board, depth-1, false, gamma, nodes_expanded+1))[0];
			test_val = max(bestValue, test_val);
		}
		vector<int> retval;
		retval.push_back(bestValue);
		retval.push_back(nodes_expanded);
		return retval;
	}
	else
	{
		int bestValue = INT_MAX;
		//DOUBLE CHECK THIS:
		//for each child in vector of children from getChildrenIndices
		//try all three moves: paraDrop, deathBlitz, sabotage
		//compare heuristic with bestValue and store max as bestValue

		vector<int> maxNeighbors = board.getChildrenIndices(0);
		for(vector<int>::iterator it = maxNeighbors.begin(); it!=maxNeighbors.end(); ++it)
		{
			int test_val;

			Board* paraDrop_board = new Board(board);
			paraDrop_board->makeMove(*it, 1, 0, gamma);//fourth param is gamma, any double will work
			test_val = (minimax(*paraDrop_board, depth-1, true, gamma, nodes_expanded))[0];
			test_val = max(bestValue, test_val);

			Board* deathBlitz_board = new Board(board);
			deathBlitz_board->makeMove(*it, 1, 1, gamma);//fourth param is gamma, any double will work
			test_val = (minimax(*deathBlitz_board, depth-1, true, gamma, nodes_expanded))[0];
			test_val = max(bestValue, test_val);

			Board* sabotage_board = new Board(board);
			sabotage_board->makeMove(*it, 1, 2, gamma);
			test_val = (minimax(*sabotage_board, depth-1, true, gamma, nodes_expanded))[0];
			test_val = max(bestValue, test_val);
		}

		vector<int> retval;
		retval.push_back(bestValue);
		retval.push_back(nodes_expanded);
		return retval;
	}

	vector<int> retval;
	retval.push_back(-1);
	retval.push_back(nodes_expanded);
	return retval;
}


//see description for minimax
vector<int> alphabeta(Board board, int depth, int alpha, int beta, bool maximizingPlayer, double gamma, int nodes_expanded)
{
	if(depth==0 || board.isBoardFull())
	{
		if(maximizingPlayer)
		{
			vector <int> retval;
			retval.push_back(board.getPlayerScore(0));
			retval.push_back(nodes_expanded);
			return retval;
		}
		else
		{
			vector <int> retval;
			retval.push_back(board.getPlayerScore(1));
			retval.push_back(nodes_expanded);
			return retval;		
		}
	}

	if(maximizingPlayer)
	{
		int v = INT_MIN;
		
		//DOUBLE CHECK THIS:
		//for each child in vector of children from getChildrenIndices
		//try all three moves: paraDrop, deathBlitz, sabotage
		//compare heuristic with bestValue and store max as bestValue

		vector<int> maxNeighbors = board.getChildrenIndices(0);
		for(vector<int>::iterator it = maxNeighbors.begin(); it!=maxNeighbors.end(); ++it)
		{
			Board* paraDrop_board = new Board(board);
			paraDrop_board->makeMove(*it, 0, 0, gamma);//fourth param is gamma, any double will work
			v = (alphabeta(*paraDrop_board, depth-1, alpha, beta, false, gamma, nodes_expanded+1))[0];
			alpha = max(alpha, v);
			if(beta <= alpha)
				break;	//beta cut-off

			Board* deathBlitz_board = new Board(board);
			deathBlitz_board->makeMove(*it, 0, 1, gamma);//fourth param is gamma, any double will work
			v = (alphabeta(*deathBlitz_board, depth-1, alpha, beta, false, gamma, nodes_expanded+1))[0];
			alpha = max(alpha, v);
			if(beta <= alpha)
				break;	//beta cut-off

			Board* sabotage_board = new Board(board);
			sabotage_board->makeMove(*it, 0, 2, gamma);
			v = (alphabeta(*sabotage_board, depth-1, alpha, beta, false, gamma, nodes_expanded+1))[0];
			alpha = max(alpha, v);
			if(beta <= alpha)
				break;	//beta cut-off
		}
		vector<int> retval;
		retval.push_back(v);
		retval.push_back(nodes_expanded);
		return retval;
	}
	else
	{
		int v = INT_MAX;
		//DOUBLE CHECK THIS:
		//for each child in vector of children from getChildrenIndices
		//try all three moves: paraDrop, deathBlitz, sabotage
		//compare heuristic with bestValue and store max as bestValue

		vector<int> maxNeighbors = board.getChildrenIndices(0);
		for(vector<int>::iterator it = maxNeighbors.begin(); it!=maxNeighbors.end(); ++it)
		{
			Board* paraDrop_board = new Board(board);
			paraDrop_board->makeMove(*it, 1, 0, gamma);//fourth param is gamma, any double will work
			v = (alphabeta(*paraDrop_board, depth-1, alpha, beta, true, gamma, nodes_expanded))[0];
			beta = max(beta, v);
			if(beta <= alpha)
				break;	//alpha cut-off

			Board* deathBlitz_board = new Board(board);
			deathBlitz_board->makeMove(*it, 1, 1, gamma);//fourth param is gamma, any double will work
			v = (alphabeta(*deathBlitz_board, depth-1, alpha, beta, true, gamma, nodes_expanded))[0];
			beta = max(beta, v);
			if(beta <= alpha)
				break;	//alpha cut-off

			Board* sabotage_board = new Board(board);
			sabotage_board->makeMove(*it, 1, 2, gamma);
			v = (alphabeta(*sabotage_board, depth-1, alpha, beta, true, gamma, nodes_expanded))[0];
			beta = max(beta, v);
			if(beta <= alpha)
				break;	//alpha cut-off
		}

		vector<int> retval;
		retval.push_back(v);
		retval.push_back(nodes_expanded);
		return retval;
	}

	vector<int> retval;
	retval.push_back(-1);
	retval.push_back(nodes_expanded);
	return retval;
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
