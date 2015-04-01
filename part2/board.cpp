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
	heuristic = -1;
	parent = NULL;
	children = vector<Board*>();
}

Board::Board(vector< pair<int, int> > i_board, map<int, pair<int, string> > i_player_map, int i_heuristic)
{
	board = i_board;
	player_map = i_player_map;
	heuristic = i_heuristic;
	parent = NULL;
	children = vector<Board*>(); //double check (maybe need to deep copy)
}

Board::Board(Board const &other)
{
	board = other.board;
	map<int, pair<int, string> > rhs_map;
	heuristic = other.heuristic;
	parent = NULL;
	children = vector<Board*>();
	
	for(map<int, pair<int, string> >::const_iterator it = other.player_map.begin(); it!=other.player_map.end(); ++it)
	{
		pair<int, string> my_pair ((it->second).first, (it->second).second);
		rhs_map.insert(pair<int, pair<int, string> >(it->first, my_pair  )); 
	}
	player_map = rhs_map;
}

//destructor
Board::~Board()
{
	//I don't think there's a need to do anything here as no new is called in Board.cpp

	//todo: delete nodes from the tree of Boards
	// if(!children.empty() && !(isRoot()))
	// {
	// 	//assign all the children's parent pointers to point to the current board's parent
	// 	for(vector<Board*>::iterator it = children.begin(); it!=children.end(); ++it)
	// 	{
	// 		(*it)->parent = this->parent;
	// 	}
	// 	delete this;
	// }
	// else if(isRoot())
	// {
	// }
}

void Board::deleteDescendants()
{
	if(!children.empty())
	{
		for(vector<Board*>::iterator it = children.begin(); it!=children.end(); ++it)
		{
			(*it)->deleteDescendants();
		}
	}
	else
	{
		delete this;
	}
}

bool Board::isRoot()
{
	if(parent==NULL)
		return true;
	else
		return false;
}

void Board::printBoard()
{
	cout << "printing board..." << endl;
	cout << "\tA\tB\tC\tD\tE\tF" << endl;
	int counter = 0;
	int row_counter = 0;
	for(vector< pair<int, int> >::const_iterator it = board.begin(); it!=board.end(); ++it)
	{
		if(counter%6==0)
		{
			row_counter++;
			cout << endl << (row_counter) << "\t";
		}
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

int Board::getPlayerScore(string playerName)
{
	for(map< int, pair <int, string> >::const_iterator it = player_map.begin(); it!=player_map.end(); ++it)
	{	
		if( (((it)->second).second).compare(playerName) == 0)
		{	
			return ((it)->second).first;
		}
	}
	cerr << "Error: Invalid playerName for getPlayerScore. Please call addPlayer(" << playerName << ")" << endl;
	return -1;
}

int Board::getPlayerScore(int playerID)
{
	int last_player=-1;
	for(map< int, pair <int, string> >::const_iterator it = player_map.begin(); it!=player_map.end(); ++it)
	{	
		if( (it)->first == playerID)
		{	
			return ((it)->second).first;
		}
		last_player=(it)->first;
	}
	cerr << "Error: Invalid playerID for getPlayerScore. Please add another "<< playerID - last_player <<" players." <<  endl;
	return -1;	
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
	cout << "Adding player " << counter << " ("<< playerName << ") " << endl;
	pair<int, string> my_pair (0, playerName);
	player_map.insert(pair<int, pair<int, string> >(counter, my_pair));
}

bool Board::checkFriendlyNeighbors(int column, int row, int playerID)
{
	//check up
	int up_row = row-1;
	if(!(up_row<0))
	{
		//cout << "Checking up neighbor" << endl;
		if(board[up_row*6+column].second==playerID)
		{
			//cout << "There is an up neighbor" << endl;
			return true;
		}
	}

	//check right
	int right_col = column+1;
	if(!(right_col>=6))
	{
		//cout << "Checking right neighbor" << endl;
		if(board[row*6+right_col].second==playerID)
		{
			//cout << "There is a right neighbor" << endl;
			return true;
		}
	}

	//check down
	int down_row = row+1;
	if(!(down_row>=6))
	{
		//cout << "Checking down neighbor" << endl;
		if(board[down_row*6+column].second==playerID)
		{
			//cout << "There is a down neighbor" << endl;
			return true;
		}	
	}

	//check left
	int left_col = column-1;
	if(!(left_col<0))
	{
		//cout << "Checking left neighbor" << endl;
		if(board[row*6+left_col].second==playerID)
		{
			//cout << "There is a left neighbor" << endl;
			return true;
		}	
	}

	return false;
}

bool Board::checkEnemyNeighbors(int column, int row, int playerID)
{
	//check up
	int up_row = row-1;
	if(!(up_row<0))
	{
		//cout << "Checking up neighbor" << endl;
		if(board[up_row*6+column].second!=playerID && board[up_row*6+column].second!=playerID-1)
		{
			//cout << "There is an up neighbor" << endl;
			return true;
		}
	}

	//check right
	int right_col = column+1;
	if(!(right_col>=6))
	{
		//cout << "Checking right neighbor" << endl;
		if(board[row*6+right_col].second!=playerID && board[up_row*6+column].second!=playerID-1)
		{
			//cout << "There is a right neighbor" << endl;
			return true;
		}
	}

	//check down
	int down_row = row+1;
	if(!(down_row>=6))
	{
		//cout << "Checking down neighbor" << endl;
		if(board[down_row*6+column].second!=playerID && board[up_row*6+column].second!=playerID-1)
		{
			//cout << "There is a down neighbor" << endl;
			return true;
		}	
	}

	//check left
	int left_col = column-1;
	if(!(left_col<0))
	{
		//cout << "Checking left neighbor" << endl;
		if(board[row*6+left_col].second!=playerID && board[up_row*6+column].second!=playerID-1)
		{
			//cout << "There is a left neighbor" << endl;
			return true;
		}	
	}

	return false;
}

int Board::getEnemyNeighborID(int column, int row, int playerID)
{
	//check up
	int up_row = row-1;
	if(!(up_row<0))
	{
		//cout << "Checking up neighbor" << endl;
		if(board[up_row*6+column].second!=playerID && board[up_row*6+column].second!=playerID-1)
		{
			//cout << "There is an up neighbor" << endl;
			return board[up_row*6+column].second;
		}
	}

	//check right
	int right_col = column+1;
	if(!(right_col>=6))
	{
		//cout << "Checking right neighbor" << endl;
		if(board[row*6+right_col].second!=playerID && board[up_row*6+column].second!=playerID-1)
		{
			//cout << "There is a right neighbor" << endl;
			return board[row*6+right_col].second;
		}
	}

	//check down
	int down_row = row+1;
	if(!(down_row>=6))
	{
		//cout << "Checking down neighbor" << endl;
		if(board[down_row*6+column].second!=playerID && board[up_row*6+column].second!=playerID-1)
		{
			//cout << "There is a down neighbor" << endl;
			return board[down_row*6+column].second;
		}	
	}

	//check left
	int left_col = column-1;
	if(!(left_col<0))
	{
		//cout << "Checking left neighbor" << endl;
		if(board[row*6+left_col].second!=playerID && board[up_row*6+column].second!=playerID-1)
		{
			//cout << "There is a left neighbor" << endl;
			return board[row*6+left_col].second;
		}	
	}

	return -1;
}

void Board::paraDrop(char i_column, int row, string playerName)
{
	row--;
	if(row<0 || row>=6)
	{
		cerr << "Error: Invalid i_column for paraDrop. Please enter a row value from 1 to 6." << endl;
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
			cerr << "Error: Invalid column for paraDrop. Please enter a column value between A and F, inclusive." << endl;
			break;
	}

	//this should never happen
	if(column<0 || column>=6)
	{
		cerr << "Error: Invalid column for paraDrop. Please enter a column value between A and F, inclusive." << endl;
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
		cerr << "Error: Invalid paradrop - " << player_map[orig_playerID].second << " already occupies [" << i_column << "," << row+1 << "]" << endl;
		return;
	}
	else
	{
		cout << "paraDrop by " << playerName << " (" << playerID << ") on [" << i_column << "," << row+1 << "]" << endl;
		//update score
		int square_score = board[row*6+column].first;
		player_map[orig_playerID].first -= square_score;
		player_map[playerID].first += square_score;

		//update player's claim to square
		board[row*6+column].second = playerID;
		
		if(DEBUG==true)
		{
			printBoard();
			if(FAST==false)
			{
				usleep(500000);
			}
		}
	}

}

void Board::conquerNeighbors(char i_column, int column, int row, int playerID)
{
	//conquer every enemy controlled square adjacent to the square to be taken
	//up
	//if the up square is assigned and is not occupied by the player making the move
	int up_square_location = (row-1)*6+column;
	if(board[up_square_location].second!=-1 && board[up_square_location].second!=playerID)
	{
		cout << "Conquering up neighbor [" << i_column << "," << (row-1)+1 << "]" << endl;
		//conquer the square, update the score, etc.
		int enemy_playerID = board[up_square_location].second;
		int neighbor_square_score = board[up_square_location].first;
		player_map[enemy_playerID].first -= neighbor_square_score;
		player_map[playerID].first += neighbor_square_score;
		board[up_square_location].second=playerID;
	}
	//right
	//if the right square is assigned and is not occupied by the player making the move
	int right_square_location = (row)*6+(column+1);
	if(board[right_square_location].second!=-1 && board[right_square_location].second!=playerID)
	{
		cout << "Conquering right neighbor [" << (char)(i_column+1) << "," << (row)+1 << "]" << endl;
		//conquer the square, update the score, etc.
		int enemy_playerID = board[right_square_location].second;
		int neighbor_square_score = board[right_square_location].first;
		player_map[enemy_playerID].first -= neighbor_square_score;
		player_map[playerID].first += neighbor_square_score;
		board[right_square_location].second=playerID;
	}
	//down
	//if the down square is assigned and is not occupied by the player making the move
	int down_square_location = (row+1)*6+column;
	if(board[down_square_location].second!=-1 && board[down_square_location].second!=playerID)
	{
		cout << "Conquering down neighbor [" << i_column << "," << (row+1)+1 << "]" << endl;
		//conquer the square, update the score, etc.
		int enemy_playerID = board[down_square_location].second;
		int neighbor_square_score = board[down_square_location].first;
		player_map[enemy_playerID].first -= neighbor_square_score;
		player_map[playerID].first += neighbor_square_score;
		board[down_square_location].second=playerID;
	}
	//left
	//if the right square is assigned and is not occupied by the player making the move
	int left_square_location = (row)*6+(column-1);
	if(board[left_square_location].second!=-1 && board[left_square_location].second!=playerID)
	{
		cout << "Conquering left neighbor [" << (char)(i_column-1) << "," << (row)+1 << "]" << endl;
		//conquer the square, update the score, etc.
		int enemy_playerID = board[left_square_location].second;
		int neighbor_square_score = board[left_square_location].first;
		player_map[enemy_playerID].first -= neighbor_square_score;
		player_map[playerID].first += neighbor_square_score;
		board[left_square_location].second=playerID;
	}
}

void Board::deathBlitz(char i_column, int row, string playerName)
{
	row--;
	if(row<0 || row>=6)
	{
		cerr << "Error: Invalid i_column for deathBlitz. Please enter a row value from 1 to 6." << endl;
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
			cerr << "Error: Invalid column for deathBlitz. Please enter a column value between A and F, inclusive." << endl;
			break;
	}

	//this should never happen
	if(column<0 || column>=6)
	{
		cerr << "Error: Invalid column for deathBlitz. Please enter a column value between A and F, inclusive." << endl;
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
		cerr << "Error: Invalid deathBlitz - " << player_map[orig_playerID].second << " already occupies [" << i_column << "," << row+1<< "]" << endl;
		return;
	}
	if(!checkFriendlyNeighbors(column, row, playerID))
	{
		cout << "Error: invalid deathBlitz - " << playerName << " does not control a neighboring square" << endl;
		return;
	}
	else
	{
		//todo: implementation
		cout << "deathBlitz by " << playerName << " (" << playerID << ") on [" << i_column << "," << row+1 << "]" << endl;
		
		//update score
		int square_score = board[row*6+column].first;
		player_map[orig_playerID].first -= square_score;
		player_map[playerID].first += square_score;

		//update player's claim to square
		board[row*6+column].second = playerID;

		conquerNeighbors(i_column, column, row, playerID);

		if(DEBUG==true)
		{
			printBoard();
			if(FAST==false)
			{
				usleep(500000);
			}
		}
	}
}

void Board::sabotage(char i_column, int row, string playerName, double gamma)
{
	row--;
	if(row<0 || row>=6)
	{
		cerr << "Error: Invalid i_column for sabotage. Please enter a row value from 1 to 6." << endl;
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
			cerr << "Error: Invalid column for sabotage. Please enter a column value between A and F, inclusive." << endl;
			break;
	}

	//this should never happen
	if(column<0 || column>=6)
	{
		cerr << "Error: Invalid column for sabotage. Please enter a column value between A and F, inclusive." << endl;
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
		cerr << "Error: Invalid playerName for sabotage. Please call addPlayer(" << playerName << ")" << endl;
		return;
	}

	int orig_playerID = board[row*6+column].second;
	if(orig_playerID!=-1)
	{
		cerr << "Error: Invalid sabotage - " << player_map[orig_playerID].second << " already occupies [" << i_column << "," << row+1 << "]" << endl;
		return;
	}
	if(!checkEnemyNeighbors(column, row, playerID))
	{
		cout << "Error: invalid sabotage - an enemy does not control a neighboring square" << endl;
		return;
	}

	//todo: evaluate success or fail DOUBLE CHECK THIS
	bool success = false;
	if(gamma<0 || gamma>1)
	{
		cout << "Error: invalid gamma for sabotage. Please enter a value between 0 and 1, inclusive" << endl;
	}
	gamma *=100; //if gamma is initially 0.2, 20% chance of success, compare with 20. If random number <= 20, succeed. If random number > 20, fail.
	srand(time(NULL));
	int delta = rand() % 100 + 1; //returns a number between 1 and 100
	if(delta>gamma)
	{
		success = false;
	}
	else
	{
		success = true;
	}

	if(!success)
	{
		cout << "Unsuccessful sabotage by " << playerName << " (" << playerID << ") on [" << i_column << "," << row+1 << "]" << endl;
		
		int enemy_playerID = getEnemyNeighborID(column, row, playerID);
		
		//this should never happen
		if(enemy_playerID == -1)
		{
			cerr << "ERROR IN IMPLEMENTATION OF SABOTAGE (see getEnemyNeighborID call)" << endl;
		}

		//ONLY WORKS IF THERE IS ONE OTHER PLAYER
		int square_score = board[row*6+column].first;
		player_map[enemy_playerID].first += square_score;
		player_map[playerID].first -= square_score; //only affects unassigned player

		//update player's claim to square
		board[row*6+column].second = enemy_playerID;
	}
	else
	{
		cout << "successful sabotage by " << playerName << " (" << playerID << ") on [" << i_column << "," << row+1 << "]" << endl;
		//update score
		int square_score = board[row*6+column].first;
		player_map[orig_playerID].first -= square_score;
		player_map[playerID].first += square_score;

		//update player's claim to square
		board[row*6+column].second = playerID;

		conquerNeighbors(i_column, column, row, playerID);
		
		if(DEBUG==true)
		{
			printBoard();
			if(FAST==false)
			{
				usleep(500000);
			}
		}
	}

}

void Board::makeMove(int index, int playerID, int move, double gamma)
{
	int column = index%6;
	char i_column;
	int row = index/6 + 1;
	string playerName = "Unassigned";

	switch(column)
	{
		case 0:
			i_column = 'A';
			break;
		case 1:
			i_column = 'B';
			break;
		case 2:
			i_column = 'C';
			break;
		case 3:
			i_column = 'D';
			break;
		case 4:
			i_column = 'E';
			break;
		case 5:
			i_column = 'F';
			break;
		//this should never happen
		default:
			cerr << "ERROR IN IMPLEMENTATION OF makeMove (see defuault switch case) or call in respective algorithm (minimax or alphabeta)" << endl;
			i_column = 'G';
			break;
	}
	
	if(playerID<0 || playerID > (int)(player_map.size()-2))	//due to index -1 being player "Unassigned"
	{
		cerr << "Error: Invalid playerID for makeMove, please select a value between 0 and " << player_map.size()-2 << ", inclusive " << endl;
	}

	playerName = player_map[playerID].second;

	if(playerName.compare("Unassigned")==0)
	{
		cerr << "Error: Invalid playerID for makeMove, please select a value between 0 and " << player_map.size()-2 << ", inclusive" << endl;
	}

	switch(move)
	{
		case 0:
			this->paraDrop(i_column, row, playerName);
			break;
		case 1:
			this->deathBlitz(i_column, row, playerName);
			break;
		case 2:
			this->sabotage(i_column, row, playerName, gamma);
			break;
		default:
			cerr << "Error: Invalid move for makeMove, please select a value between 0 and 2, inclusive" << endl;
			break;
	}
	return;
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
			cerr << "Error: file is empty" << endl;
			return;
		}

		//process the line
		split(fields, curr_line, is_any_of("\t"), token_compress_on);
		if(fields.size()!=6)
		{
			cerr << "Error: input file has invalid number of columns for row " << i+1 << endl;
		}

		//write to the board in Board object
		for(unsigned int j = 0; j < fields.size(); j++)
		{
			temp_string = fields[j];
			temp = atoi(temp_string.c_str());
			board.push_back(pair<int, int>(temp, -1));
		}
	}
	cout << "end of parse" << endl;

	//printBoard();

}


//returns index i.e. board[index] containing highest unclaimed square location
//returns -1 if there is no unclaimed square
int Board::findHighestUnclaimedSquare()
{
	int maxScore = INT_MIN;
	int maxScoreLocation = -1;
	int counter = 0;
	for(vector< pair<int, int> >::const_iterator it = board.begin(); it!=board.end(); ++it)
	{
		//if unclaimed and score greater than maxScore
		if( it->second==-1 && it->first > maxScore)
		{
			maxScore = it->first;
			maxScoreLocation = counter;
		}
		counter++;
	}
	return maxScoreLocation;
}

int Board::findLowestUnclaimedSquare()
{
	int minScore = INT_MAX;
	int minScoreLocation = -1;
	int counter = 0;
	for(vector< pair<int, int> >::const_iterator it = board.begin(); it!=board.end(); ++it)
	{
		//if unclaimed and score greater than maxScore
		if( it->second==-1 && it->first < minScore)
		{
			minScore = it->first;
			minScoreLocation = counter;
		}
		counter++;
	}
	return minScoreLocation;
}

vector<int> Board::getUnoccupiedIndices()
{
	vector<int> retval;
	int counter = 0;
	for(vector<pair<int, int> >::const_iterator it = board.begin(); it!=board.end(); ++it)
	{
		if(it->second==-1)
		{
			retval.push_back(counter);
		}
		counter++;
	}
	return retval;
}

bool Board::isBoardFull()
{
	for(vector< pair<int, int> >::const_iterator it = board.begin(); it!=board.end(); ++it)
	{
		//if claimed
		if( it->second==-1)
		{
			return false;
		}
	}
	return true;
}

vector<int> Board::getEmptyNeighboringSquares(int playerID)
{
	vector<int> indices;
	int counter = 0;
	//search the board for matching playerID
	for(vector< pair<int, int> >::const_iterator it = board.begin(); it!=board.end(); ++it)
	{	
		//for every board location occupied by playerID
		if( (it)->first == playerID)
		{	
			//check to see if there are unoccupied neighbors and push their indices to the vector
			int up_index = counter-6;
			if(up_index >= 0)
			{
				if(board[up_index].second==-1)
				{
					if(find(indices.begin(), indices.end(), up_index)==indices.end()) //don't push repeats
					{
						indices.push_back(up_index);
					}
				}
			}

			int right_index = counter+1;
			if(right_index < 36)
			{
				if(board[right_index].second==-1)
				{
					if(find(indices.begin(), indices.end(), right_index)==indices.end())
					{
						indices.push_back(right_index);
					}			
				}
			}

			int down_index = counter+6;
			if(down_index < 36)
			{
				if(board[down_index].second==-1)
				{
					if(find(indices.begin(), indices.end(), down_index)==indices.end())
					{
						indices.push_back(down_index);
					}
				}
			}

			int left_index = counter-1;
			if(left_index >=0)
			{
				if(board[left_index].second==-1)
				{
					if(find(indices.begin(), indices.end(), left_index)==indices.end())
					{
						indices.push_back(left_index);
					}	
				}		
			}
		}
		counter++;
	}
	return indices;
}

bool Board::isSameBoard(Board rhs)
{
	//compare player scores
	int num_players = player_map.size();
	for(int i = -1; i<num_players-1; i++)
	{
		if(getPlayerScore(i)!=rhs.getPlayerScore(i))
			return false;	
	}

	//compare unoccupied indices
	vector<int> lhs_unoccupied = getUnoccupiedIndices();
	vector<int> rhs_unoccupied = rhs.getUnoccupiedIndices();
	if(lhs_unoccupied!=rhs_unoccupied)
		return false;	

	return true;
}