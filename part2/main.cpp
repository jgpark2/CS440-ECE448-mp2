#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <unistd.h>
#include "board.h"


using namespace std;
using namespace boost;

int minimax(Board board, int depth, bool maximizingPlayer, double gamma)
{
	board.printBoard();
	if(depth==0 || board.isBoardFull())
	{
		//cout << "Base case for ";
		int retval;
		if(maximizingPlayer)
		{
			//cout << " maximizingPlayer" << endl;
			retval = board.getPlayerScore(0);
			return retval;
		}
		else
		{
			//cout << " minimizingPlayer" << endl;
			retval = board.getPlayerScore(1);
			return retval;
		}
	}
	if(maximizingPlayer)
	{
		int bestValueMax = INT_MIN;
		//vector<int> unoccupied_squares = board.getUnoccupiedIndices();
		// for(vector<int>::const_iterator it = unoccupied_squares.begin(); it!=unoccupied_squares.end(); ++it)
		// {
			//paraDrop
			Board* paraDrop_board = new Board(board);
 			paraDrop_board->parent = &board;
  			board.children.push_back(paraDrop_board);
   			//paraDrop_board->makeMove(*it, 0, 0, gamma);
      		paraDrop_board->makeMove(paraDrop_board->findHighestUnclaimedSquare(), 0, 0, gamma);
   			int test_val0 = minimax(*paraDrop_board, depth-1, false, gamma);
 			bestValueMax = max(bestValueMax, test_val0);

 		// 	//deathBlitz
			// Board* deathBlitz_board = new Board(board);
 		// 	deathBlitz_board->parent = &board;
  	// 		board.children.push_back(deathBlitz_board);
   // 			deathBlitz_board->makeMove(*it, 0, 1, gamma);
   // 			int test_val1 = minimax(*deathBlitz_board, depth-1, false, gamma);
 		// 	bestValueMax = max(bestValueMax, test_val1);

 			// //sabotage
 			// Board* sabotage_board = new Board(board);
 			// sabotage_board->parent = &board;
  		// 	board.children.push_back(sabotage_board);
   	// 		sabotage_board->makeMove(*it, 0, 2, gamma);
   	// 		int test_val2 = minimax(*sabotage_board, depth-1, false, gamma);
 			// bestValueMax = max(bestValueMax, test_val2);

		//}
		return bestValueMax;
	}
	else
	{
		int bestValueMin = INT_MAX;
		// vector<int> unoccupied_squares = board.getUnoccupiedIndices();
		// for(vector<int>::const_iterator it = unoccupied_squares.begin(); it!=unoccupied_squares.end(); ++it)
		// {
			//paraDrop
			Board* paraDrop_board = new Board(board);
 			paraDrop_board->parent = &board;
  			board.children.push_back(paraDrop_board);
   			//paraDrop_board->makeMove(*it, 1, 0, gamma);
      		paraDrop_board->makeMove(paraDrop_board->findLowestUnclaimedSquare(), 1, 0, gamma);
   			int test_val0 = minimax(*paraDrop_board, depth-1, true, gamma);
 			bestValueMin = min(bestValueMin, test_val0);

 		// 	//deathBlitz
			// Board* deathBlitz_board = new Board(board);
 		// 	deathBlitz_board->parent = &board;
  	// 		board.children.push_back(deathBlitz_board);
   // 			deathBlitz_board->makeMove(*it, 1, 1, gamma);
   // 			int test_val1 = minimax(*deathBlitz_board, depth-1, true, gamma);
 		// 	bestValueMin = min(bestValueMin, test_val1);

 			// //sabotage
 			// Board* sabotage_board = new Board(board);
 			// sabotage_board->parent = &board;
  		// 	board.children.push_back(sabotage_board);
   	// 		sabotage_board->makeMove(*it, 1, 2, gamma);
   	// 		int test_val2 = minimax(*sabotage_board, depth-1, true, gamma);
 			// bestValueMin = min(bestValueMin, test_val2);
		//}
		return bestValueMin;	
	}
}


//only paraDrop
int alphabeta(Board board, int depth, bool maximizingPlayer, int alpha, int beta, double gamma)
{
	if(depth==0 || board.isBoardFull())
	{
		int retval;
		if(maximizingPlayer)
		{
			retval = board.getPlayerScore(0);
			return retval;
		}
		else
		{
			retval = board.getPlayerScore(1);
			return retval;
		}
	}
	if(maximizingPlayer)
	{
		int vMax = INT_MIN;
		vector<int> unoccupied_squares = board.getUnoccupiedIndices();
		for(vector<int>::const_iterator it = unoccupied_squares.begin(); it!=unoccupied_squares.end(); ++it)
		{
			//paraDrop
			Board* paraDrop_board = new Board(board);
 			paraDrop_board->parent = &board;
  			board.children.push_back(paraDrop_board);
   			paraDrop_board->makeMove(*it, 0, 0, gamma);
   			int test_val0 = alphabeta(*paraDrop_board, depth-1, false, alpha, beta, gamma);
 			vMax = max(vMax, test_val0);
 			alpha = max(alpha, vMax);
 			if(beta <= alpha)
 				break;

 			//deathBlitz
			Board* deathBlitz_board = new Board(board);
 			deathBlitz_board->parent = &board;
  			board.children.push_back(deathBlitz_board);
   			deathBlitz_board->makeMove(*it, 0, 1, gamma);
   			int test_val1 = alphabeta(*deathBlitz_board, depth-1, false, alpha, beta, gamma);
 			vMax = max(vMax, test_val1);
 			alpha = max(alpha, vMax);
 			if(beta <= alpha)
 				break;

 			// //sabotage
 			// Board* sabotage_board = new Board(board);
 			// sabotage_board->parent = &board;
  		// 	board.children.push_back(sabotage_board);
   	// 		sabotage_board->makeMove(*it, 0, 2, gamma);
   	// 		int test_val2 = alphabeta(*sabotage_board, depth-1, false, alpha, beta, gamma);
 			// vMax = max(vMax, test_val2);
 			// alpha = max(alpha, vMax);
 			// if(beta <= alpha)
 			// 	break;

		}
		return vMax;
	}
	else
	{
		int vMin = INT_MAX;
		vector<int> unoccupied_squares = board.getUnoccupiedIndices();
		for(vector<int>::const_iterator it = unoccupied_squares.begin(); it!=unoccupied_squares.end(); ++it)
		{
			//paraDrop
			Board* paraDrop_board = new Board(board);
 			paraDrop_board->parent = &board;
  			board.children.push_back(paraDrop_board);
   			paraDrop_board->makeMove(*it, 1, 0, gamma);
   			int test_val0 = alphabeta(*paraDrop_board, depth-1, true, alpha, beta, gamma);
 			vMin = min(vMin, test_val0);
 			beta = min(beta, vMin);
 			if(beta <= alpha)
 				break;

 			//deathBlitz
			Board* deathBlitz_board = new Board(board);
 			deathBlitz_board->parent = &board;
  			board.children.push_back(deathBlitz_board);
   			deathBlitz_board->makeMove(*it, 1, 1, gamma);
   			int test_val1 = alphabeta(*deathBlitz_board, depth-1, true, alpha, beta, gamma);
 			vMin = min(vMin, test_val1);
  			beta = min(beta, vMin);
 			if(beta <= alpha)
 				break;

 			// //sabotage
 			// Board* sabotage_board = new Board(board);
 			// sabotage_board->parent = &board;
  		// 	board.children.push_back(sabotage_board);
   	// 		sabotage_board->makeMove(*it, 1, 2, gamma);
   	// 		int test_val2 = alphabeta(*sabotage_board, depth-1, true, alpha, beta, gamma);
 			// vMin = min(vMin, test_val2);
  		// 	beta = min(beta, vMin);
 			// if(beta <= alpha)
 			// 	break;
		}
		return vMin;	
	}
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

	// Board* b = new Board();
	// b->parseBoard(scenario);
	// b->addPlayer("Blue");
	// b->addPlayer("Green");

	// b->paraDrop('D', 1, "Blue");
	// b->paraDrop('C', 2, "Blue");
	// b->paraDrop('D', 2, "Blue");
	// b->paraDrop('E', 2, "Blue");
	// b->paraDrop('C', 4, "Green");
	// b->paraDrop('D', 4, "Green");
	// b->printBoard();
	// b->printScores();

	// b->deathBlitz('D', 3, "Green");
	// b->printBoard();
	// b->printScores();

	// b->deathBlitz('C', 3, "Blue");
	// b->printBoard();
	// b->printScores();

	// b->sabotage('C', 1, "Green", 1.0);
	// b->printBoard();
	// b->printScores();
	// cout << "Player 0 score: " << b->getPlayerScore(0) << endl;
	// cout << "Blue's score " << b->getPlayerScore("Blue") << endl;
	// cout << "Player 1 score: " << b->getPlayerScore(1) << endl;
	// cout << "Blue's score " << b->getPlayerScore("Green") << endl;

	// Board* c = new Board(*b);
	// c->printBoard();

	Board* d = new Board();
	d->parseBoard(scenario);
	d->addPlayer("Blue");
	d->addPlayer("Green");
	//d->printBoard();
	//d->printScores();
	cout << "Running minimax" << endl;
	int minimax_heuristic = minimax(*d, 3, false, 1.0);
	cout << "Minimax heuristic: " << minimax_heuristic << endl;
	//vector<int> minimax_val = minimax(*d, 3, true, 1.0, 0, false);
	//cout << "Index: " << minimax_val[0] << "\tHeuristic Value: " << minimax_val[1] << endl;

	// cout << endl << "\n\npausing for 5 seconds..." << endl;
	// usleep(5000000);

	// Board* e = new Board();
	// e->parseBoard(scenario);
	// e->addPlayer("Blue");
	// e->addPlayer("Green");
	// e->printBoard();
	// // cout << "Running alphabeta" << endl;
	// // vector<int> alphabeta_val = alphabeta(*e, 3, INT_MIN, INT_MAX, true, 1.0, 0, false);
	// // cout << "Index: " << alphabeta_val[0] << "\tHeuristic Value: " << alphabeta_val[1] << endl;

	//delete b;
	//delete c;
	delete d;
	//delete e;
	return 0;
}
