#include "board.h"

using namespace std;
using namespace boost;

Board::Board()
{
	board = vector< pair<int, int> >();
	map<int, pair<int, string> > my_map;
	pair<int, string> my_pair (0, "Unassigned"); //score of 0 and name is "Unassigned", printed as "U" on the board
	my_map.insert(pair<int, pair<int, string> >(-1, my_pair)); //score initial score of -1
	player_map = my_map;
}

Board::Board(vector< pair<int, int> > i_board, map<int, pair<int, string> > i_player_map)
{
	board = i_board;
	player_map = i_player_map;
}

Board::Board(Board const &other)
{
	board = other.board;
	player_map = other.player_map;
}

//destructor
Board::~Board()
{

}

void Board::printBoard()
{
	cout << "printing board..." << endl;
	int counter = 0;
	for(vector< pair<int, int> >::const_iterator it = board.begin(); it!=board.end(); ++it)
	{
		if(counter%6==0 && counter!=0)
			cout << endl;
		cout << (it)->first << "-";
		string playerName = (player_map[(it)->second]).second;
		cout << playerName.at(0) << "\t";
		counter++;
	}
	cout << endl;
	cout << "board printed" << endl;
}

void Board::printScores()
{	
	cout << "printing scores..." << endl;

	for(map< int, pair <int, string> >::const_iterator it = player_map.begin(); it!=player_map.end(); ++it)
	{	
		cout << "Player " << (it)->first << " (" << ((it)->second).second << "):\t" << ((it)->second).first << endl;
	}

	cout << "scores printed" << endl;
}

void Board::addPlayer(string playerName)
{
	int counter = -1; //Unassigned is a default player

	for(map< int, pair <int, string> >::const_iterator it = player_map.begin(); it!=player_map.end(); ++it)
	{
		if( (((it)->second).second).compare(playerName) == 0)
		{	
			cerr << "Player " << playerName << " already exists" << endl;
			return;
		}
		counter++;
	}

	pair<int, string> my_pair (0, playerName);
	player_map.insert(pair<int, pair<int, string> >(counter, my_pair));
}

bool Board::checkNeighbors(int column, int row, int playerID)
{
	//check up
	int up_row = row-1;
	if(!up_row<0)
	{
		if(board[up_row*6+column].second==playerID)
			return true;
	}

	//check right
	int right_col = column+1;
	if(!right_col>=6)
	{
		if(board[row*6+right_col].second==playerID)
			return true;
	}

	//check down
	int down_row = row+1;
	if(!down_row>=6)
	{
		if(board[down_row*6+column].second==playerID)
			return true;
	}

	//check left
	int left_col = column-1;
	if(!left_col<0)
	{
		if(board[row*6+left_col].second==playerID)
			return true;
	}

	return false;
}

void Board::paraDrop(char i_column, int row, string playerName)
{
	row--;
	if(row<0 || row>=6)
	{
		cerr << "Error: Invalid i_column for paraDrop" << endl;
	}

	int column = -1;

	switch(i_column)
	{
		case 'A':
		case 'a':
			column = 0;
			break;
		case 'B':
		case 'b':
			column = 1;
			break;
		case 'C':
		case 'c':
			column = 2;
			break;
		case 'D':
		case 'd':
			column = 3;
			break;
		case 'E':
		case 'e':
			column = 4;
			break;
		case 'F':
		case 'f':
			column = 5;
			break;
		default:
			cerr << "Error: Invalid column for paraDrop" << endl;
			break;
	}

	//this should never happen
	if(column<0 || column>=6)
	{
		cerr << "Error: Invalid column for paraDrop" << endl;
		return;
	}
	
	int playerID = -1;
	for(map< int, pair <int, string> >::const_iterator it = player_map.begin(); it!=player_map.end(); ++it)
	{
		if( (((it)->second).second).compare(playerName) == 0)
		{	
			playerID = (it)->first;
			break;
		}
	}
	if(playerID==-1)
	{
		cerr << "Error: Invalid playerName for paraDrop. Please call addPlayer(" << playerName << ")" << endl;
		return;
	}

	int orig_playerID = board[row*6+column].second;
	if(orig_playerID!=-1)
	{
		cerr << "Error: Invalid paradrop - " << player_map[orig_playerID].second << " already occupies [" << i_column << "," << row << "]" << endl;
		return;
	}
	else
	{
		cout << "Moving " << playerName << " (" << playerID << ") to " << i_column << "," << row << endl;
		//update score
		int square_score = board[row*6+column].first;
		player_map[orig_playerID].first -= square_score;
		player_map[playerID].first += square_score;

		//update player's claim to square
		board[row*6+column].second = playerID;
		printBoard();
	}

}

void Board::deathBlitz(char i_column, int row, string playerName)
{
	row--;
	if(row<0 || row>=6)
	{
		cerr << "Error: Invalid i_column for deathBlitz" << endl;
	}

	int column = -1;

	switch(i_column)
	{
		case 'A':
		case 'a':
			column = 0;
			break;
		case 'B':
		case 'b':
			column = 1;
			break;
		case 'C':
		case 'c':
			column = 2;
			break;
		case 'D':
		case 'd':
			column = 3;
			break;
		case 'E':
		case 'e':
			column = 4;
			break;
		case 'F':
		case 'f':
			column = 5;
			break;
		default:
			cerr << "Error: Invalid column for deathBlitz" << endl;
			break;
	}

	//this should never happen
	if(column<0 || column>=6)
	{
		cerr << "Error: Invalid column for deathBlitz" << endl;
		return;
	}

	int playerID = -1;
	for(map< int, pair <int, string> >::const_iterator it = player_map.begin(); it!=player_map.end(); ++it)
	{
		if( (((it)->second).second).compare(playerName) == 0)
		{
			playerID = (it)->first;
			break;
		}
	}
	if(playerID==-1)
	{
		cerr << "Error: Invalid playerName for deathBlitz. Please call addPlayer(" << playerName << ")" << endl;
		return;
	}

	int orig_playerID = board[row*6+column].second;
	if(orig_playerID!=-1)
	{
		cerr << "Error: Invalid deathBlitz - " << player_map[orig_playerID].second << " already occupies [" << i_column << "," << row << "]" << endl;
		return;
	}
	else if(!checkNeighbors(column, row, playerID))
	{
		cout << "Error: invalid deathBlitz - " << playerName << " does not control a neighboring square" << endl;
		return;
	}
	else
	{
		//todo: implementation
		cout << "Moving " << playerName << " (" << playerID << ") to " << i_column << "," << row << endl;
		
		//todo: conquer every enemy controlled square adjacent to the square to be taken

		//update score
		int square_score = board[row*6+column].first;
		player_map[orig_playerID].first -= square_score;
		player_map[playerID].first += square_score;

		//update player's claim to square
		board[row*6+column].second = playerID;

		printBoard();
	}
}

void Board::parseBoard(string scenario)
{
	cout << "parsing board..." << endl;

	//to parse file
	string curr_line;
	ifstream myfile(scenario.c_str());

	//to parse each line
	vector<string> fields;
	string temp_string;
	int temp;

	if(!myfile.is_open())
	{
		cerr << "Error: unable to open file" << endl;
		return;
	}

	//each line
	for(unsigned int i = 0; i < 6; i++)
	{
		//get the line
		if(getline(myfile, curr_line)==NULL)
		{
			cerr << "Error: invalid line in input file" << endl;
			return;
		}

		//process the line
		split(fields, curr_line, is_any_of("\t"), token_compress_on);
		if(fields.size()!=6)
		{
			cerr << "Error: input file has invalid number of columns" << endl;
		}

		//write to the board in Parse object
		for(unsigned int j = 0; j < fields.size(); j++)
		{
			temp_string = fields[j];
			temp = atoi(temp_string.c_str());
			board.push_back(pair<int, int>(temp, -1));
		}
	}
	cout << "end of parse" << endl;

	printBoard();

}
