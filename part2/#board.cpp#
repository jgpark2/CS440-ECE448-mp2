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
	heuristic = other.heuristic;
	parent = NULL;
	children = vector<Board*>();
	
	for(map<int, pair<int, string> >::const_iterator it = other.player_map.begin(); it!=other.player_map.end(); ++it)
	{
		pair<int, string> my_pair ((it->second).first, (it->second).second);
		player_map.insert(pair<int, pair<int, string> >(it->first, my_pair  )); 
	}
}

//destructor
Board::~Board()
{
	//I don't think there's a need to do anything here as no new is called in Board.cpp
}

int col_ctoi(char i_column) {
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
	}
	return column;
}

//Checks if [coordinate index] within boundaries
bool inBounds(int num) {
	if(num>=0 && num<=5)
		return true;
	//cout << "Error: Invalid coordinates." << endl;
	return false;
}

//Checks if [coordinate index] within boundaries- COLUMN THEN ROW
bool inBounds(int col, int row) {
	if(col>=0 && col<=5 && row>=0 && row<=5)
		return true;
	//cout << "Error: Invalid coordinates. A-F and 1-6 Please." << endl;
	return false;
}

int cIdx(int index){
	return index%6;
}

int rIdx(int index){
	return index/6;
}

void Board::deleteDescendants()
{
	for(vector<Board*>::iterator it = children.begin(); it!=children.end(); ++it)
		(*it)->deleteDescendants();

	delete this;
}

bool Board::isRoot()
{
	if(parent==NULL)
		return true;
	return false;
}

bool Board::isBoardFull()
{
	for(vector< pair<int, int> >::const_iterator it = board.begin(); it!=board.end(); ++it)
		if( it->second==-1) //if empty slot
			return false;
	return true;
}

int Board::getPlayerScore(int playerID)
{
	int last_player=-1;
	for(map< int, pair <int, string> >::const_iterator it = player_map.begin(); it!=player_map.end(); ++it) {	
		if( (it)->first == playerID)	
			return ((it)->second).first;
			
		last_player=(it)->first;
	}
	cerr << "Error: Invalid playerID for getPlayerScore. Please add another "<< playerID - last_player <<" players." <<  endl;
	return -1;	
}

void Board::addPlayer(string playerName)
{
	int counter = -1; //Unassigned is a default player

	for(map< int, pair <int, string> >::const_iterator it = player_map.begin(); it!=player_map.end(); ++it) {
		if( (((it)->second).second).compare(playerName) == 0) {	
			cerr << "Player " << playerName << " already exists" << endl;
			return;
		}
		counter++;
	}
	cout << "Adding player " << counter << " ("<< playerName << ") " << endl;
	pair<int, string> my_pair (0, playerName);
	player_map.insert(pair<int, pair<int, string> >(counter, my_pair));
}

vector<int> Board::findNeighborIDs(int col, int row) {
	vector<int> ret;
	
	if(row-1 > -1)//check up
		ret.push_back(board[(row-1)*6+col].second);
	
	if(col+1 < 6)//check right
		ret.push_back(board[row*6+(col+1)].second);
	
	if(row+1 < 6)//check down
		ret.push_back(board[(row+1)*6+col].second);
	
	if(col-1 > -1)//check left
		ret.push_back(board[row*6+(col-1)].second);
		
	return ret;
}

bool Board::checkFriendlyNeighbors(int column, int row, int playerID)
{
	vector<int> neighborIDs = findNeighborIDs(column, row);
	
	for(unsigned int i=0; i<neighborIDs.size(); ++i)
		if(neighborIDs[i]==playerID)
			return true;
	
	return false;
}

bool Board::checkEnemyNeighbors(int column, int row, int playerID)
{
	vector<int> neighborIDs = findNeighborIDs(column, row);
	
	for(unsigned int i=0; i<neighborIDs.size(); ++i)
		if(neighborIDs[i]!=-1 && neighborIDs[i]!=playerID)
			return true;
	
	return false;
}

int Board::getEnemyNeighborID(int column, int row, int playerID)
{
	vector<int> neighborIDs = findNeighborIDs(column, row);
	
	for(unsigned int i=0; i<neighborIDs.size(); ++i)
		if(neighborIDs[i]!=-1 && neighborIDs[i]!=playerID)
			return neighborIDs[i];
	
	return -1;
}

int Board::player_stoi(string playerName) {
	for(map< int, pair <int, string> >::const_iterator it = player_map.begin(); it!=player_map.end(); ++it) {
		if( (((it)->second).second).compare(playerName) == 0)
			return (it)->first;
	}
	
	cerr << "Error: Invalid playerName. Please call addPlayer(" << playerName << ")" << endl;
	return -1;
}

void Board::paraDrop(char i_column, int i_row, string playerName)
{
	/* Bad Input Checking */
	int row = i_row-1;
	int col = col_ctoi(i_column);
	if(!inBounds(col,row))
		return;
	
	int orig_playerID = board[row*6+col].second;
	if(orig_playerID!=-1) {
		cerr << "Error: Invalid paradrop - " << player_map[orig_playerID].second << " already occupies [" << i_column << "," << row+1 << "]" << endl;
		return;
	}
	
	int playerID = player_stoi(playerName);
	
	//***INITIATE PARADROP***//
	cout << "paraDrop by " << playerName << " (" << playerID << ") on [" << i_column << "," << i_row << "]" << endl;
	
	//update score
	int square_score = board[row*6+col].first;
	player_map[orig_playerID].first -= square_score;
	player_map[playerID].first += square_score;

	//update player's claim to square
	board[row*6+col].second = playerID;
	
	
	if(DEBUG) {
		printBoard();
		if(!FAST)
			usleep(500000);
	}

}

void Board::conquer(int index, int playerID) {
	if(!inBounds(cIdx(index),rIdx(index)))
		return;
	char i_col = 'A'+cIdx(index);
	int i_row = rIdx(index)+1;
	cout << "Conquering neighbor [" << i_col << "," << i_row << "]" << endl;
	//conquer the square, update the score, etc.
	int enemy_playerID = board[index].second;
	int neighbor_square_score = board[index].first;
	player_map[enemy_playerID].first -= neighbor_square_score;
	player_map[playerID].first += neighbor_square_score;
	board[index].second=playerID;
}

void Board::conquerNeighbors(int col, int row, int playerID)
{
	//conquer every enemy controlled square adjacent to the square to be taken
	
	//if the square is assigned and is not occupied by the player making the move,
	int up = (row-1)*6+col;
	if(board[up].second!=-1 && board[up].second!=playerID) {
		conquer(up, playerID);
	}
	
	int right = (row)*6+(col+1);
	if(board[right].second!=-1 && board[right].second!=playerID) {
		conquer(right, playerID);
	}
	
	int down = (row+1)*6+col;
	if(board[down].second!=-1 && board[down].second!=playerID) {
		conquer(down, playerID);
	}
	
	int left = (row)*6+(col-1);
	if(board[left].second!=-1 && board[left].second!=playerID) {
		conquer(left, playerID);
	}
}

void Board::deathBlitz(char i_column, int i_row, string playerName)
{
	/* Bad Input Checking */
	int row = i_row-1;
	int col = col_ctoi(i_column);
	if(!inBounds(col,row))
		return;
	
	int orig_playerID = board[row*6+col].second;
	if(orig_playerID!=-1) {
		cerr << "Error: Invalid paradrop - " << player_map[orig_playerID].second << " already occupies [" << i_column << "," << row+1 << "]" << endl;
		return;
	}
	
	int playerID = player_stoi(playerName);
	
	if(!checkFriendlyNeighbors(col, row, playerID)) {
		////////////////cout << "Error: invalid deathBlitz - " << playerName << " does not control a neighboring square" << endl;
		return;
	}
	
	//*******INITIATE DEATHBLITZ*******//
	//todo: implementation
	cout << "deathBlitz by " << playerName << " (" << playerID << ") on [" << i_column << "," << row+1 << "]" << endl;
	
	//update score
	int square_score = board[row*6+col].first;
	player_map[orig_playerID].first -= square_score;
	player_map[playerID].first += square_score;

	//update player's claim to square
	board[row*6+col].second = playerID;

	conquerNeighbors(col, row, playerID);

	if(DEBUG) {
		printBoard();
		if(!FAST)
			usleep(500000);
	}
}

void Board::sabotage(char i_column, int i_row, string playerName, double gamma)
{
	/* Bad Input Checking */
	int row = i_row-1;
	int col = col_ctoi(i_column);
	if(!inBounds(col,row))
		return;
	
	int orig_playerID = board[row*6+col].second;
	if(orig_playerID!=-1) {
		cerr << "Error: Invalid paradrop - " << player_map[orig_playerID].second << " already occupies [" << i_column << "," << row+1 << "]" << endl;
		return;
	}
	
	int playerID = player_stoi(playerName);
	
	if(!checkFriendlyNeighbors(col, row, playerID)) {
		cout << "Error: invalid deathBlitz - " << playerName << " does not control a neighboring square" << endl;
		return;
	}
	
	
	if(!checkEnemyNeighbors(col, row, playerID))
	{
		cout << "Error: invalid sabotage - an enemy does not control a neighboring square" << endl;
		return;
	}

	//todo: evaluate success or fail DOUBLE CHECK THIS
	bool success = false;
	if(gamma<0 || gamma>1) {
		cout << "Error: invalid gamma for sabotage. Please enter a value between 0 and 1, inclusive" << endl;
	}
	gamma *=100; //if gamma is initially 0.2, 20% chance of success, compare with 20. If random number <= 20, succeed. If random number > 20, fail.
	srand(time(NULL));
	int delta = rand() % 100 + 1; //returns a number between 1 and 100
	if(delta<=gamma){
		success = true;
	}

	if(!success)
	{
		cout << "Unsuccessful sabotage by " << playerName << " (" << playerID << ") on [" << i_column << "," << row+1 << "]" << endl;
		
		int enemy_playerID = getEnemyNeighborID(col, row, playerID);
		
		//this should never happen
		if(enemy_playerID == -1)
		{
			cerr << "ERROR IN IMPLEMENTATION OF SABOTAGE (see getEnemyNeighborID call)" << endl;
		}

		//ONLY WORKS IF THERE IS ONE OTHER PLAYER
		int square_score = board[row*6+col].first;
		player_map[enemy_playerID].first += square_score;
		player_map[playerID].first -= square_score; //only affects unassigned player

		//update player's claim to square
		board[row*6+col].second = enemy_playerID;
	}
	else
	{
		cout << "successful sabotage by " << playerName << " (" << playerID << ") on [" << i_column << "," << row+1 << "]" << endl;
		//update score
		int square_score = board[row*6+col].first;
		player_map[orig_playerID].first -= square_score;
		player_map[playerID].first += square_score;

		//update player's claim to square
		board[row*6+col].second = playerID;

		conquerNeighbors(col, row, playerID);
		
		if(DEBUG) {
			printBoard();
			if(!FAST)
				usleep(500000);
		}
	}

}

void Board::makeMove(int index, int playerID, int move, double gamma)
{
	int column = index%6;
	char i_column = 'A'+column;
	int i_row = index/6 + 1;
	string playerName = "Unassigned";
	
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
			this->paraDrop(i_column, i_row, playerName);
			break;
		case 1:
			this->deathBlitz(i_column, i_row, playerName);
			break;
		case 2:
			this->sabotage(i_column, i_row, playerName, gamma);
			break;
		default:
			cerr << "Error: Invalid move for makeMove, please select a value between 0 and 2, inclusive" << endl;
			break;
	}
	return;
}

//returns index i.e. board[index] containing highest unclaimed square location
//returns -1 if there is no unclaimed square
int Board::findHighestUnclaimedSquare()
{
	int maxScore = INT_MIN;
	int maxScoreLocation = -1;
	int counter = 0;
	for(vector< pair<int, int> >::const_iterator it = board.begin(); it!=board.end(); ++it) {
		//if unclaimed and score greater than maxScore
		if( it->second==-1 && it->first > maxScore) {
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
	for(vector< pair<int, int> >::const_iterator it = board.begin(); it!=board.end(); ++it) {
		//if unclaimed and score greater than maxScore
		if( it->second==-1 && it->first < minScore) {
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
	for(vector<pair<int, int> >::const_iterator it = board.begin(); it!=board.end(); ++it) {
		if(it->second==-1)
			retval.push_back(counter);
		counter++;
	}
	return retval;
}

set<int> Board::getEmptyNeighboringSquares(int playerID)
{
	set<int> indices;
	int counter = 0;
	//search the board for matching playerID
	for(vector< pair<int, int> >::const_iterator it = board.begin(); it!=board.end(); ++it) {	
		//for every board location occupied by playerID
		if( (it)->first == playerID) {	
			if(board[counter-6].second==-1)//up
				indices.insert(counter-6);
			if(board[counter+1].second==-1)//right
				indices.insert(counter+1);
			if(board[counter+6].second==-1)//down
				indices.insert(counter+6);
			if(board[counter-1].second==-1)//left
				indices.insert(counter-1);
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
	if (DEBUG)
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
