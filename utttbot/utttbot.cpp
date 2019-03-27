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
	int trialtime = timeout / 20; //calculate no. of trials

	Move move = Move{ -1,-1 };

	std::vector<Move> moves = getMoves(state);
	if (state.board[4][4] == Player::None && state.macroboard[1][1] == Player::Active) {
		move = Move{ 4,4 };
	}
	else {

		Player CurrentPlayer = getCurrentPlayer(state); //get current player

		if (getMoves(state).size() != 1) {
			move = mcMove(state, CurrentPlayer); //move = mcMove(state, CurrentPlayer);
		}
		else {
			move = getMoves(state)[0];
		}
	}

	std::cout << "place_disc " << move << std::endl; //move


}

void UTTTBot::update(std::string &key, std::string &value) {
	if (key == "round") {
		round = std::stoi(value);
	} else if (key == "field") {
		int row = 0;
		int col = 0;
		std::vector<std::string> fields = split(value, ',');
		for (std::string &field : fields) {
			if (field == "0") {
				state.board[row][col] = Player::X; 
			} else if (field == "1") {
				state.board[row][col] = Player::O;
			} else {
				state.board[row][col] = Player::None;
			}
			col++;
			if (col == 9) {
				row++; 
				col = 0;
			}
		}
	} else if (key == "macroboard") {
		int row = 0;
		int col = 0;
		std::vector<std::string> fields = split(value, ',');
		for (std::string &field : fields) {
			if (field == "-1") {
				state.macroboard[row][col] = Player::Active;
			} else if (field == "0") {
				state.macroboard[row][col] = Player::X;
			} else if (field == "1") {
				state.macroboard[row][col] = Player::O;
			} else {
				state.macroboard[row][col] = Player::None;
			}
			col++;
			if (col == 3) {
				row++;
				col = 0;
			}
		}
	}
}

void UTTTBot::setting(std::string &key, std::string &value) {
	if (key == "timebank") {
		timebank = std::stoi(value);
	} else if (key == "time_per_move") {
		time_per_move = std::stoi(value);
	} else if (key == "player_names") {
		std::vector<std::string> names = split(value, ',');
		player_names[0] = names[0];
		player_names[1] = names[1];
	} else if (key == "your_bot") {
		your_bot = value;
	} else if (key == "your_botid") {
		your_botid = std::stoi(value);
	}
}

std::vector<std::string> UTTTBot::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}