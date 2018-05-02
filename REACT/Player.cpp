#include "Player.h"

Player::Player(std::string name, int won_games) {
	name_ = name;
	won_games_ = won_games;
}

std::string Player::get_name() const {
	return name_;
}

void Player::set_name(std::string name) {
	name_ = name;
}

int Player::get_won_games() const {
	return won_games_;
}

void Player::add_won_game() {
	won_games_ += 1;
}

void Player::reset_won_games() {
	won_games_ = 0;
}
