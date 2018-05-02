#pragma once
#include <string>
#include <iostream>

class Player {
private:
	std::string name_;
	int won_games_;

public:
	Player(std::string name, int won_games);
	std::string get_name() const;
	void set_name(std::string name);
	int get_won_games() const;
	void add_won_game();
	void reset_won_games();
};