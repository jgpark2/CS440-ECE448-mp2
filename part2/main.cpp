#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <unistd.h>
#include "board.h"
#include "move.h"
#include <ctime>


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

Move alphabeta(Board board, int depth, int orig_id, bool maximizingPlayer, int alpha, int beta, double gamma)
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
					curMove.score = (alphabeta(*paraDrop_board, depth-1, orig_id, !maximizingPlayer, alpha, beta, gamma)).score;
					if(curMove.score > bestMove.score)
						bestMove = curMove;
						bestMove.moveType = 0;
					alpha = max(alpha, bestMove.score);
					if(beta<=alpha)
						break;
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
					curMove.score = (alphabeta(*deathBlitz_board, depth-1, orig_id, !maximizingPlayer, alpha, beta, gamma)).score;
					if(curMove.score > bestMove.score)
						bestMove = curMove;
						bestMove.moveType = 1;
					alpha = max(alpha, bestMove.score);
					if(beta<=alpha)
						break;
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
					curMove.score = (alphabeta(*paraDrop_board, depth-1, orig_id, !maximizingPlayer, alpha, beta, gamma)).score;
					if(curMove.score < bestMove.score)
						bestMove = curMove;
						bestMove.moveType = 0;
					beta = min(beta, bestMove.score);
					if(beta<=alpha)
						break;
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
					curMove.score = (alphabeta(*deathBlitz_board, depth-1, orig_id, !maximizingPlayer, alpha, beta, gamma)).score;
					if(curMove.score < bestMove.score)
						bestMove = curMove;
						bestMove.moveType = 1;
					beta = min(beta, bestMove.score);
					if(beta<=alpha)
						break;
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
	
	//int totalExpansions = 0;
	double totalTime = 0.0;
	int timeCount = 0;

	while(!(d->isBoardFull())) {
		Move heuristic;
		clock_t time_start = clock();
		
		if(strategyType[curPlayer] == 0)
			heuristic = minimax(*d, 3, curPlayer, true, 1.0);
		else
			heuristic = alphabeta(*d, 3, curPlayer, true, INT_MIN, INT_MAX, 1.0); //Probably change depth to smthn more
		
		clock_t time_end = clock();
		
		totalTime+=double(time_end-time_start)/CLOCKS_PER_SEC;
		timeCount++;
		cout << "Time this move took: " << double(time_end-time_start)/CLOCKS_PER_SEC << endl;
		//cout << "Minimax heuristic score: " << heuristic.score << endl;
		//cout << "Minimax heuristic index: " << heuristic.index << endl;
		
		d->makeMove(heuristic.index,curPlayer,heuristic.moveType,1.0);
		
		curPlayer = (curPlayer == 1)?0:1;
	}

	cout << "player1 Score = " << d->getPlayerScore(0) << endl;
	cout << "player2 Score = " << d->getPlayerScore(1) << endl;
	d->printBoard();
	
	cout << "Average Time per move: " << totalTime/timeCount <<"s"<<endl;
	cout << "Average Nodes Expanded per move: " <<endl;
	

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
