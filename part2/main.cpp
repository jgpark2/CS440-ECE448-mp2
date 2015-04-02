#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <unistd.h>
#include "board.h"
#include "move.h"


using namespace std;
using namespace boost;

Move minimax(Board board, int depth, int orig_id, bool maximizingPlayer, double gamma)
{
	Move move;
	//board.printBoard();
	if(depth==0 || board.isBoardFull())
	{
		move.score = board.getPlayerScore(orig_id);
		return move;
	}
	if(maximizingPlayer)
	{
		Move bestMove(INT_MIN, -1);

		//paraDrop
		vector<int> possible_moves = board.getUnoccupiedIndices();
		if(!possible_moves.empty())
		{
			for(vector<int>::iterator it = possible_moves.begin(); it!=possible_moves.end(); ++it)
			{
				Board* paraDrop_board = new Board(board);
		  		paraDrop_board->makeMove(*it, 0, 0, gamma);
		  		Move curMove(-1, *it);

				if( !(paraDrop_board->isSameBoard(board)) )
				{
			  		paraDrop_board->parent = &board;
					board.children.push_back(paraDrop_board);
					curMove.score = (minimax(*paraDrop_board, depth-1, orig_id, !maximizingPlayer, gamma)).score;
					if(curMove.score > bestMove.score) {
						bestMove = curMove;
						bestMove.moveType = 0;
					}
				}
			}
		}

		//deathBlitz
		//possible_moves.clear();
		set<int> possible_moves_2 = board.getEmptyNeighboringSquares(0);
		//if(!possible_moves_2.empty())
		//{
			for(set<int>::iterator it = possible_moves_2.begin(); it!=possible_moves_2.end(); ++it)
			{
				Board* deathBlitz_board = new Board(board);
				deathBlitz_board->makeMove(*it, 0, 1, gamma);
				Move curMove(-1, *it);

				if( !(deathBlitz_board->isSameBoard(board)) )
				{
					deathBlitz_board->parent = &board;
					board.children.push_back(deathBlitz_board);
					curMove.score = (minimax(*deathBlitz_board, depth-1, orig_id, !maximizingPlayer, gamma)).score;
					if(curMove.score > bestMove.score){
						bestMove = curMove;
						bestMove.moveType = 1;
					}
				}
			}
		//}

		//sabotage (FOR EXPECTIMINIMAX, NOT FOR MINIMAX)
		// possible_moves.clear();
		// possible_moves = board.getEmptyNeighboringSquares(1); //want neighbors to enemy squares
		// if(!possible_moves.empty())
		// {
		// 	for(vector<int>::iterator it = possible_moves.begin(); it!=possible_moves.end(); ++it)
		// 	{
		// 		Board* sabotage_board = new Board(board);
		// 		sabotage_board->makeMove(*it, 0, 2, gamma);
		// 		if( !(sabotage_board->isSameBoard(board)) )
		// 		{
		// 			sabotage_board->parent = &board;
		// 			board.children.push_back(sabotage_board);
		// 			int test_val2 = minimax(*sabotage_board, depth-1, false, gamma);
		// 			bestValueMax = max(bestValueMax, test_val2);
		// 		}
		// 	}
		// }

		return bestMove;
	}
	else
	{
		Move bestMove(INT_MAX, -1);

		//paraDrop
		vector<int>possible_moves = board.getUnoccupiedIndices();
		if(!possible_moves.empty())
		{
			for(vector<int>::iterator it = possible_moves.begin(); it!=possible_moves.end(); ++it)
			{
				Board* paraDrop_board = new Board(board);
		  		paraDrop_board->makeMove(*it, 1, 0, gamma);
		  		Move curMove(-1, *it);

				if( !(paraDrop_board->isSameBoard(board)) )
				{
			  		paraDrop_board->parent = &board;
					board.children.push_back(paraDrop_board);
					curMove.score = (minimax(*paraDrop_board, depth-1, orig_id, !maximizingPlayer, gamma)).score;
					if(curMove.score < bestMove.score) {
						bestMove = curMove;
						bestMove.moveType = 0;
					}
				}
			}
		}

		//deathBlitz
		//possible_moves.clear();
		set<int> possible_moves_2 = board.getEmptyNeighboringSquares(1);
		//if(!possible_moves_2.empty())
		//{
			for(set<int>::iterator it = possible_moves_2.begin(); it!=possible_moves_2.end(); ++it)
			{		
				Board* deathBlitz_board = new Board(board);
				deathBlitz_board->makeMove(*it, 1, 1, gamma);
			  	Move curMove(-1, *it);

				if( !(deathBlitz_board->isSameBoard(board)) )
				{
					deathBlitz_board->parent = &board;
					board.children.push_back(deathBlitz_board);
					curMove.score = (minimax(*deathBlitz_board, depth-1, orig_id, !maximizingPlayer, gamma)).score;
					if(curMove.score < bestMove.score) {
						bestMove = curMove;
						bestMove.moveType = 1;
					}
				}
			}
		//}

		//sabotage (FOR EXPECTIMINIMAX, NOT FOR MINIMAX)
		// possible_moves.clear();
		// possible_moves = board.getEmptyNeighboringSquares(1); //want neighbors to enemy squares
		// if(!possible_moves.empty())
		// {
		// 	for(vector<int>::iterator it = possible_moves.begin(); it!=possible_moves.end(); ++it)
		// 	{
		// 		Board* sabotage_board = new Board(board);
		// 		sabotage_board->makeMove(*it, 1, 2, gamma);
		// 		if( !(sabotage_board->isSameBoard(board)) )
		// 		{
		// 			sabotage_board->parent = &board;
		// 			board.children.push_back(sabotage_board);
		// 			int test_val2 = minimax(*sabotage_board, depth-1, true, gamma);
		// 			bestValueMin = min(bestValueMin, test_val2);
		// 		}
		// 	}
		// }
		
		return bestMove;
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

	if (argc!=4) 
	{
		cout << "Error, usage: ./mp2 [player 1 stragegy a/m] [player 2 strategy a/m] [scenario file name]" << endl;
		return 1;
	}
	
	char player1Mode = argv[1][0];
	char player2Mode = argv[2][0];
	string scenario = argv[3];
	
	vector<int> strategyType;
	if(player1Mode == 'm')
		strategyType.push_back(0);
	else
		strategyType.push_back(1);
		
	if(player2Mode == 'm')
		strategyType.push_back(0);
	else
		strategyType.push_back(1);

	

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

	// b->paraDrop('D', 4, "Blue");
	// b->printBoard();
	// b->printScores();

	// b->deathBlitz('D', 4, "Blue");
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
	cout << "Running minimax" << endl;
	int curPlayer = 0;

	//minimax vs minimax

	while(!(d->isBoardFull()))
	{
		if(strategyType[curPlayer] == 0)
			Move minimax_heuristic = minimax(*d, 3, curPlayer, true, 1.0);
		else
			Move minimax_heuristic = alphabeta(*d, 3, curPlayer, true, 1.0);
		d->makeMove(minimax_hueristic.index,curPlayer,minimax_hueristic.moveTypes,1.0);
		curPlayer = (curPlayer == 1)?0:1;

	}

	//

	cout << "Minimax heuristic score: " << minimax_heuristic.score << endl;
	cout << "Minimax heuristic index: " << minimax_heuristic.index << endl;

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

	// Board* f = new Board(*d);
	// cout << "Two board are ";
	// if(!(f->isSameBoard(*d)))
	// 	cout << "not ";
	// cout << "the same" << endl;
	// f->paraDrop('D', 4, "Green");
	// cout << "Two board are ";
	// if(!(f->isSameBoard(*d)))
	// 	cout << "not ";
	// cout << "the same" << endl;

	//delete b;
	//delete c;
	delete d;
	//delete e;
	return 0;
}
