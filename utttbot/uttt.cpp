// uttt.cpp
// Aswin van Woudenberg

#include "uttt.h"

using namespace std;

unsigned const trials = 100;

ostream &operator<<(ostream& os, const Player &p) 
{
	if (p == Player::None) {
		os << ".";
	} else if (p == Player::X) {
		os << "X";
	} else if (p == Player::O) {
		os << "O";
	} else if (p == Player::Active) {
		os << "*";
	}
	return os;
}

ostream &operator<<(ostream& os, const State &s) {
	for (int r=0; r<9; r++) {
		for (int c=0; c<9; c++) {
			os << s.board[r][c];
		}
		os << endl;
	}
	os << endl;
	for (int r=0; r<3; r++) {
		for (int c=0; c<3; c++) {
			os << s.macroboard[r][c];
		}
		os << endl;
	}
	return os;
}

ostream &operator<<(ostream& os, const Move &m) {
	os << m.y << " " << m.x;
	return os;
}

Player getCurrentPlayer(const State &state)
{
	int countX = 0;
	int countO = 0;
	for (int r=0; r<9; r++) {
		for (int c=0; c<9; c++) {
			if (state.board[r][c] == Player::X) countX++;
			else if (state.board[r][c] == Player::O) countO++;
		}
	}
	return (countX > countO ? Player::O : Player::X); 
}

Player getWinner(const State &state, int row, int col)
{
	for (int r=0; r<3; r++)
		if (state.board[row*3+r][col*3] == state.board[row*3+r][col*3+1] && 
				state.board[row*3+r][col*3+1] == state.board[row*3+r][col*3+2] && 
				state.board[row*3+r][col*3+2] != Player::None) 
			return state.board[row*3+r][col*3];
	for (int c=0; c<3; c++)
		if (state.board[row*3][col*3+c] == state.board[row*3+1][col*3+c] && 
				state.board[row*3+1][col*3+c] == state.board[row*3+2][col*3+c] && 
				state.board[row*3][col*3+c] != Player::None) 
			return state.board[row*3][col*3+c];
	if (state.board[row*3][col*3] == state.board[row*3+1][col*3+1] && 
			state.board[row*3+1][col*3+1] == state.board[row*3+2][col*3+2] && 
			state.board[row*3][col*3] != Player::None) 
		return state.board[row*3][col*3];
	if (state.board[row*3][col*3+2] == state.board[row*3+1][col*3+1] && 
			state.board[row*3+1][col*3+1] == state.board[row*3+2][col*3] && 
			state.board[row*3][col*3+2] != Player::None) 
		return state.board[row*3][col*3+2];
	for (int r=0; r<3; r++)
		for (int c=0; c<3; c++)
			if (state.board[row*3+r][col*3+c] == Player::None)
				return Player::Active;
	return Player::None;
}

State doMove(const State &state, const Move &m)
{
	State result = state;

	if (state.macroboard[m.y/3][m.x/3] != Player::Active) 
	{
		return result; // Invalid move
	}

	result.board[m.y][m.x] = getCurrentPlayer(state);
	for (int r=0; r<3; r++) 
	{
		for (int c=0; c<3; c++) 
		{
			result.macroboard[r][c] = getWinner(result, r, c);
		}
	}

	int empty = 0;

	for (int r=0; r<9; r++)
		for (int c=0; c<9; c++)
			if (result.board[r][c] == Player::None)
			{
				empty++;
			}
				
	bool stillPlaying = empty > 0 && getWinner(result) == Player::None;
	
	if (result.macroboard[m.y%3][m.x%3] == Player::Active)
		for (int r=0; r<3; r++)
			for (int c=0; c<3; c++)
				if ((r != m.y % 3 || c != m.x % 3 || !stillPlaying) && result.macroboard[r][c] == Player::Active)
				{
					result.macroboard[r][c] = Player::None;
				}
	return result; 
}

Player getWinner(const State &state)
{
	for (int r=0; r<3; r++)
		if (state.macroboard[r][0] == state.macroboard[r][1] && 
				state.macroboard[r][1] == state.macroboard[r][2] && 
				state.macroboard[r][2] != Player::None && 
				state.macroboard[r][2] != Player::Active) 
			return state.macroboard[r][2];
	for (int c=0; c<3; c++)
		if (state.macroboard[0][c] == state.macroboard[1][c] && 
				state.macroboard[1][c] == state.macroboard[2][c] && 
				state.macroboard[2][c] != Player::None && 
				state.macroboard[2][c] != Player::Active) 
			return state.macroboard[2][c];
	if (state.macroboard[0][0] == state.macroboard[1][1] && state.macroboard[1][1] == state.macroboard[2][2] && 
			state.macroboard[0][0] != Player::None && state.macroboard[0][0] != Player::Active) 
		return state.macroboard[0][0];
	if (state.macroboard[0][2] == state.macroboard[1][1] && state.macroboard[1][1] == state.macroboard[2][0] && 
			state.macroboard[0][2] != Player::None && state.macroboard[0][2] != Player::Active) 
		return state.macroboard[0][2];
	return Player::None;
}

vector<Move> getMoves(const State &state)
{
	vector<Move> moves;
	if (getWinner(state) == Player::None) 
	{
		for (int r=0; r<9; r++) {
			for (int c=0; c<9; c++) {
				if (state.macroboard[r/3][c/3] == Player::Active && state.board[r][c] == Player::None) {
					moves.push_back(Move{c,r});
				}
			}
		}
	}
	return moves;
}

State mcTrial(const State &board)
{
	State trialboard = State(board);
	vector<Move> moves;
	vector<Move>::iterator move;	
	moves = getMoves(trialboard);
	int number = moves.size();
	
	while (true)
	{
		moves = getMoves(trialboard);

		if (moves.size() == 0) 
		{
			return trialboard;
		}

		move = select_randomly<vector<Move>::iterator>(moves.begin(), moves.end());

		trialboard = doMove(trialboard, *move);
	}

	return board;
}

void mcUpdateScores(array < array<int, 9>, 9> &scores, const State &board, const Player &player, Move &tryMove)
{
	int const mc_win = 5;
	int const mc_lose = 10;

	array < array<Player, 9>, 9>::const_iterator boardIte;

	if (getWinner(board) == player)
	{
		// add score if the player move would result in a win
		scores[tryMove.y][tryMove.x] += mc_win;
	}
	
	else if (getWinner(board) == Player::None)
	{}
	else
	{
		scores[tryMove.x][tryMove.y] -= mc_lose;
	}
}

Move getBestMove(const std::array < std::array<int, 9>, 9> &scores, const State &board)
{
	int highScore = -999;
	int p = 0;
	Move nonInverted;
	Move bestMove;
	vector<Move> moves = getMoves(board);

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (scores[i][j] > highScore)
			{
				highScore = scores[i][j];
				bestMove.x = i;
				bestMove.y = j;
				p++;
			}
		}
	}

	if (p == 0)
	{
		nonInverted = *select_randomly(moves.begin(), moves.end());
		bestMove.x = nonInverted.y;
		bestMove.y = nonInverted.x;
	}
	cerr << "p=" << p << endl;

	return bestMove;
}

Move mcMove(const State &board, const Player &player)
{
	vector<Move> moves = getMoves(board);
	array < array<int, 9>, 9> score;
	int possibleMoveLength = moves.size();

	//cerr << "scores voor aanpassen:" << endl;

	for (int r = 0; r < 9; r++) 
	{
		for (int c = 0; c < 9; c++) 
		{
			score[r][c] = -999;
		
			//cerr << score[r][c] << endl;
			// zet elke score op -999
		}
	}

	cerr << "available moves:" << endl;
	
	for (Move m : moves) 
	{
		//prints available moves
		cerr << m.y << ", " << m.x << endl;
	}
	
	for (int j = 0; j < moves.size(); j++)
	{
		State tryBoard = State(board);
		doMove(tryBoard, moves[j]);

		for (unsigned i = 0; i < trials / (possibleMoveLength); i++)
		{
			const State trialboard = mcTrial(board);
			mcUpdateScores(score, trialboard, player, moves[j]); //score updating
		}	
		
		cerr << "scores na aanpassen:" << endl;

		for (int r = 0; r < 9; r++)
		{
			for (int c = 0; c < 9; c++)
			{
				

				cerr << score[r][c] << endl;
			}
		}	
	}
	return getBestMove(score, board); //best move
}