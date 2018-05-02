#pragma once
#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "Player.h"
#include <chrono>
#include <memory>

#define VERSION		"2.1"
#define DEBUG		false

class Reaction_game_handler {
private:
	std::shared_ptr<Digital_output> prepare_led_;
	std::shared_ptr<Digital_output> warn_led_;
	std::shared_ptr<Digital_output> piezo_speaker_;
	std::shared_ptr<Digital_output> p1_led_;
	std::shared_ptr<Digital_output> p2_led_;
	std::shared_ptr<Digital_input> p1_btn_;
	std::shared_ptr<Digital_input> p2_btn_;
	std::shared_ptr<Player> player1_;
	std::shared_ptr<Player> player2_;
	int rounds_;
	bool btn_p1_pressed_;
	bool btn_p2_pressed_;
	std::chrono::nanoseconds last_pressed_;
	int64_t time_difference_;
	
	// private methods
	void show_title_screen();
	void ask_player_name(std::shared_ptr<Player>, bool);
	void ask_rounds();
	void all_leds_off();
	void prepare_game();
	bool run_round(int round);
	void check_total_winner();
	void print_title();
	void blink_led(int which_led);

public:
	Reaction_game_handler(
		std::shared_ptr<Digital_output> prepare_led, std::shared_ptr<Digital_output> warn_led,
		std::shared_ptr<Digital_output> piezo_speaker, std::shared_ptr<Digital_output> p1_led,
		std::shared_ptr<Digital_output> p2_led, std::shared_ptr<Digital_input> p1_btn,
		std::shared_ptr<Digital_input> p2_btn);
	void run_game();
	void set_last_pressed(std::chrono::nanoseconds last_pressed);
	std::chrono::nanoseconds get_last_pressed();
	void set_btn_p1_pressed(bool pressed);
	void set_btn_p2_pressed(bool pressed);
	void set_time_difference(int64_t time_difference);
	bool get_btn_p1_pressed();
	bool get_btn_p2_pressed();
};