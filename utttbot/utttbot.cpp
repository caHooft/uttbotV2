// utttbot.cpp
// Aswin van Woudenberg

#include "utttbot.h"
#include <iostream>
#include <sstream>

void UTTTBot::run() {
	std::string line;
	while (std::getline(std::cin, line)) {
		std::vector<std::string> command = split(line, ' ');
		if (command[0] == "settings") {
			setting(command[1], command[2]);
		} else if (command[0] == "update" && command[1] == "game") {
			update(command[2], command[3]);
		} else if (command[0] == "action" && command[1] == "move") {
			move(std::stoi(command[2]));
		} else {
			std::cerr << "Unknown command: " << line << std::endl;
		}
	}
}

void UTTTBot::move(int timeout) 
{
	std::cerr << state << std::endl;
	Player CurrentPlayer = getCurrentPlayer(state); //get current player

	Move move = Move{ -1,-1 };
	Move invertMove = Move{ -1,-1 };

	std::vector<Move> moves = getMoves(state);


	/*
	if (state.board[3][5] == Player::None && state.macroboard[1][1] == Player::Active) 
	{
		move = Move{ 3,5 };
		std::cerr << "standard move" << std::endl;
	}
	*/

	

	if (getMoves(state).size() == 1) //als er maar 1 move available is, doe die move
	{
			
		//move = getMoves(state)[0];
		invertMove = getMoves(state)[0];
		move.x = invertMove.y;
		move.y = invertMove.x;

		std::cerr << "er is 1 move" << std::endl;
	}

	else 
	{
		std::cerr << "er is meer dan 1 move" << std::endl;

		move = mcMove(state, CurrentPlayer); //move = mcMove(state, CurrentPlayer);
	}
	

	if (move.x == -1)//catch voor als de voorgaande functies niet lukken
	{
		std::cerr << "random move" << std::endl;
		move = *select_randomly(moves.begin(), moves.end());
	}

	std::cerr << "attempting to place move: " << move.y << move.x << std::endl;
	std::cout << "place_disc " << move << std::endl; //move
	std::cerr << "placed move: " << move.y << move.x << std::endl;
}

void UTTTBot::update(std::string &key, std::string &value) {
	if (key == "round") {
		round = std::stoi(value);
	} else if (key == "field") 
	{
		int row = 0;
		int col = 0;
		std::vector<std::string> fields = split(value, ',');
		for (std::string &field : fields) 
		{
			if (field == "0") 
			{
				state.board[row][col] = Player::X; 
			} 
			
			else if (field == "1") 
			{
				state.board[row][col] = Player::O;
			} 
			else
			{
				state.board[row][col] = Player::None;
			}
			col++;
			if (col == 9)
			{
				row++; 
				col = 0;
			}
		}
	} 
	else if (key == "macroboard") {
		int row = 0;
		int col = 0;
		std::vector<std::string> fields = split(value, ',');
		
		for (std::string &field : fields)
		{
			if (field == "-1") 
			{
				state.macroboard[row][col] = Player::Active;
			} else if (field == "0") 
			{
				state.macroboard[row][col] = Player::X;
			} else if (field == "1") 
			{
				state.macroboard[row][col] = Player::O;
			} else 
			{
				state.macroboard[row][col] = Player::None;
			}
			col++;

			if (col == 3) 
			{
				row++;
				col = 0;
			}
		}
	}
}

void UTTTBot::setting(std::string &key, std::string &value) 
{
	if (key == "timebank")
	{
		timebank = std::stoi(value);
	} 
	
	else if (key == "time_per_move") 
	{
		time_per_move = std::stoi(value);
	}
	
	else if (key == "player_names") 
	{
		std::vector<std::string> names = split(value, ',');
		player_names[0] = names[0];
		player_names[1] = names[1];
	} 
	
	else if (key == "your_bot")
	{
		your_bot = value;
	}
	
	else if (key == "your_botid") 
	{
		your_botid = std::stoi(value);
	}
}

std::vector<std::string> UTTTBot::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim)) 
	{
		elems.push_back(item);
	}
	return elems;
}