#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "board.h"


using namespace std;
using namespace boost;

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

	delete b;
	return 0;
}
