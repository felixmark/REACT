#include "ReactionGameHandler.h"

using namespace std;
using namespace chrono;

Reaction_game_handler::Reaction_game_handler(
	shared_ptr<Digital_output> prepare_led, shared_ptr<Digital_output> warn_led, 
	shared_ptr<Digital_output> piezo_speaker, shared_ptr<Digital_output> p1_led, 
	shared_ptr<Digital_output> p2_led, shared_ptr<Digital_input> p1_btn, 
	shared_ptr<Digital_input> p2_btn) : prepare_led_{ prepare_led },
	warn_led_{ warn_led }, piezo_speaker_{ piezo_speaker },
	p1_led_{ p1_led }, p2_led_{ p2_led }, 
	p1_btn_{ p1_btn }, p2_btn_{ p2_btn },
	player1_{ make_shared<Player>(Player("Player1", 0)) },
	player2_{ make_shared<Player>(Player("Player2", 0)) },
	last_pressed_{ system_clock::now().time_since_epoch() },
	time_difference_{ 0 },
	rounds_{ 0 }, btn_p1_pressed_{ false }, btn_p2_pressed_{ false } {
}

void Reaction_game_handler::prepare_game() {
	print_title();
	player1_->reset_won_games();
	player2_->reset_won_games();
	player1_->set_name("Player1");
	player2_->set_name("Player2");

	// Get player names
	ask_player_name(player1_, false);
	ask_player_name(player2_, true);

	// Get rounds to play
	ask_rounds();

	// Start game in 3 Seconds
	print_title();
	cout << "\t\t" "Game will begin in 3 seconds." << endl;
	for (int i = 3; i > 0; --i) {
		cout << "\t\t" << i << ".." << endl;
		delay(1000);
	}
}

bool Reaction_game_handler::run_round(int round) {
	all_leds_off();

	// Print current round
	print_title();
	cout << "\t\t" "Round #" << round + 1 << endl;

	// Wait for 2 sec
	delay(2000);

	// Prepare (in 5 - 10 secs the WARN LED lights up)
	btn_p1_pressed_ = false;
	btn_p2_pressed_ = false;
	cout << "\t\t" "- Get ready!!" << endl;
	prepare_led_->write(Status::ON);
	int64_t randomtime = 5 * 1000 * 1000 + (rand() % (5 * 1000 * 1000));
	nanoseconds begin = system_clock::now().time_since_epoch();
	begin = system_clock::now().time_since_epoch();
	while (
		!btn_p1_pressed_ &&
		!btn_p2_pressed_ &&
		duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - duration_cast<microseconds>(begin).count() < randomtime) {
		delay(5); // do not stress CPU too much
	}

	// 5-10 seconds are up or somebody pushed a button
	prepare_led_->write(Status::OFF);
	if (btn_p1_pressed_ || btn_p2_pressed_) {
		// Somebody pushed his/her button too early
		string verlierer = "";
		if (btn_p1_pressed_) {
			// Player 1 has lost
			verlierer = player1_->get_name();
			player2_->add_won_game();
			p2_led_->write(Status::ON);
		} else {
			// Player 2 has lost
			verlierer = player2_->get_name();
			player1_->add_won_game();
			p1_led_->write(Status::ON);
		}
		cout << "\t\t" << verlierer << " has LOST round #" << round + 1 << "! (Pushed button in the preparation phase)" << endl;
	} else {
		// NOW the player should press their button
		btn_p1_pressed_ = false;
		btn_p2_pressed_ = false;
		warn_led_->write(Status::ON);
		piezo_speaker_->write(Status::ON);

		// Wait until button press or 3 secs
		begin = system_clock::now().time_since_epoch();
		while (
			!btn_p1_pressed_ && !btn_p2_pressed_ &&
			duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - duration_cast<microseconds>(begin).count() < 3 * 1000 * 1000
			) {
			delay(5);
		}

		// A button was pressed or 3 seconds are over
		piezo_speaker_->write(Status::OFF);
		warn_led_->write(Status::OFF);

		// Round is over
		string gewinner = "";
		if (btn_p1_pressed_ || btn_p2_pressed_) {
			// Somebody has pressed a button
			if (btn_p1_pressed_) {
				// Player 1 has won the round
				gewinner = player1_->get_name();
				player1_->add_won_game();
				p1_led_->write(Status::ON);
			} else {
				// Player 2 has won the round
				gewinner = player2_->get_name();
				player2_->add_won_game();
				p2_led_->write(Status::ON);
			}

			// Wait for second player (0,4 secs)
			delay(400);

			// Print winner of the current round
			cout << "\t\t" << gewinner << " has WON round #" << round + 1 << "!" << endl;
			if (btn_p1_pressed_ && btn_p2_pressed_) {
				cout << "\t\t" << "Time difference: " << (static_cast<double>(time_difference_) / 1000.0) << "ms" << endl;
			}
		} else {
			// For 3 seconds no button was pressed
			cout << "\t\t" << "NOBODY pushed a button! Are you still there?" << endl;
			cout << "\t\t" << "Terminating Game in 3 Seconds." << endl;
			
			// Wait for 3 seconds until game ends
			delay(3000);
			return false;
		}
	}

	// Wait for 3 seconds until next round
	delay(3000);

	// Check if more than 50% is won by one player
	if (player2_->get_won_games() > rounds_ / 2 || player1_->get_won_games() > rounds_ / 2) {
		return false;
	}
	return true;
}

void Reaction_game_handler::check_total_winner() {
	// Somebody is the total winner(?)
	print_title();
	if (player1_->get_won_games() > player2_->get_won_games()) {
		cout << "\t\t" << player1_->get_name() << " has WON the game!" << endl;
	} else if (player2_->get_won_games() > player1_->get_won_games()) {
		cout << "\t\t" << player2_->get_name() << " has WON the game!" << endl;
	} else {
		cout << "\t\t" "It's a tie!" << endl;
	}
	cout << endl;

	// Statistic
	cout << "\t\t" << player1_->get_name() << " has won " << player1_->get_won_games() << " rounds." << endl;
	cout << "\t\t" << player2_->get_name() << " has won " << player2_->get_won_games() << " rounds." << endl;
	cout << endl;

	// Blink LED
	p1_led_->write(Status::OFF);
	p2_led_->write(Status::OFF);
	if (player1_->get_won_games() > player2_->get_won_games()) {
		blink_led(1);
	} else if (player2_->get_won_games() > player1_->get_won_games()) {
		blink_led(2);
	} else {
		blink_led(0);
	}
	p1_led_->write(Status::OFF);
	p2_led_->write(Status::OFF);
}


void Reaction_game_handler::run_game() {
	// Show title screen
	show_title_screen();

	// Prepare new game (reading names and rounds)
	prepare_game();

	// Run rounds
	for (int round = 0; round < rounds_; round++) {
		if (run_round(round) == false) break;
	}

	// Check if somebody has won and print winner
	check_total_winner();
	
	// Wait for Enter to exit game
	cout << "\t\t" "Press ENTER to exit game." << endl;
	getchar();
	getchar();
}

void Reaction_game_handler::show_title_screen() {
	print_title();
	cout << "\t\t" "Press ENTER to start game!" << endl;
	getchar();
}

void Reaction_game_handler::ask_player_name(std::shared_ptr<Player> player, bool is_second_player) {
	string who = player->get_name();
	string string_name = "";
	while (string_name == "" || (is_second_player && string_name == player1_->get_name())) {
		cout << "\t\t" << who << ": What's your name?" << endl << "\t\t";
		cin >> string_name;
		if (is_second_player && string_name == player1_->get_name()) {
			cout << "\t\t" "Player1 has the same Name.. please choose another one!" << endl;
		}
	}
	player->set_name(string_name);
}

void Reaction_game_handler::ask_rounds() {
	int rounds = 0;
	while (rounds <= 0 || rounds > 999) {
		try {
			string s_rounds = "";
			cout << "\t\t" "How many rounds do you want to play?" << endl << "\t\t";
			cin >> s_rounds;
			rounds = stoi(s_rounds);
			if (rounds <= 0 || rounds > 999) cout << "\t\t" "Value should be between 1 and 999." << endl;
		}
		catch (...) {
			cout << "\t\t" << "Oh nooo what happened here?!" << endl;
			rounds = 0;
		}
	}
	rounds_ = rounds;
}

void Reaction_game_handler::all_leds_off() {
	p1_led_->write(Status::OFF);
	p2_led_->write(Status::OFF);
	prepare_led_->write(Status::OFF);
	warn_led_->write(Status::OFF);
}

void Reaction_game_handler::print_title() {
	system("clear");
	cout <<
		"\n\n\n"
		"\t\t"	"██████╗ ███████╗ █████╗  ██████╗████████╗"	"\n"
		"\t\t"	"██╔══██╗██╔════╝██╔══██╗██╔════╝╚══██╔══╝"	"\n"
		"\t\t"	"██████╔╝█████╗  ███████║██║        ██║"	"\n"
		"\t\t"	"██╔══██╗██╔══╝  ██╔══██║██║        ██║"	"\n"
		"\t\t"	"██║  ██║███████╗██║  ██║╚██████╗   ██║"	"\n"
		"\t\t"	"╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝ ╚═════╝   ╚═╝"	"\n"
		"\t\t"	"______________________________________"	"\n"
		"\t\t"	"  v" << VERSION << " by Felix Mark and Nona Netta"			"\n"
		"\n" << endl;
}

void Reaction_game_handler::blink_led(int which_led) {
	auto begin = system_clock::now().time_since_epoch();
	auto begin2 = system_clock::now().time_since_epoch();
	while (duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - duration_cast<microseconds>(begin).count() < 5 * 1000 * 1000) {
		if (duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - duration_cast<microseconds>(begin2).count() > 400 * 1000) {
			if (which_led == 1) {
				p1_led_->write(p1_led_->read() == Status::ON ? Status::OFF : Status::ON); // Toggle LED Player 1
			} else if (which_led == 2) {
				p2_led_->write(p2_led_->read() == Status::ON ? Status::OFF : Status::ON); // Toggle LED Player 2
			} else {
				p1_led_->write(p1_led_->read() == Status::ON ? Status::OFF : Status::ON);
				p2_led_->write(p2_led_->read() == Status::ON ? Status::OFF : Status::ON);
			}
			begin2 = system_clock::now().time_since_epoch();
		}
		delay(5);
	}
}

bool Reaction_game_handler::get_btn_p1_pressed() {
	return btn_p1_pressed_;
}

bool Reaction_game_handler::get_btn_p2_pressed() {
	return btn_p2_pressed_;
}

void Reaction_game_handler::set_last_pressed(std::chrono::nanoseconds last_pressed) {
	last_pressed_ = last_pressed;
}

std::chrono::nanoseconds Reaction_game_handler::get_last_pressed() {
	return last_pressed_;
}

void Reaction_game_handler::set_btn_p1_pressed(bool pressed) {
	btn_p1_pressed_ = pressed;
}

void Reaction_game_handler::set_btn_p2_pressed(bool pressed) {
	btn_p2_pressed_ = pressed;
}

void Reaction_game_handler::set_time_difference(int64_t time_difference) {
	time_difference_ = time_difference;
}
