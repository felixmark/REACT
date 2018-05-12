/*
	██████╗ ███████╗ █████╗  ██████╗████████╗
	██╔══██╗██╔════╝██╔══██╗██╔════╝╚══██╔══╝
	██████╔╝█████╗  ███████║██║        ██║
	██╔══██╗██╔══╝  ██╔══██║██║        ██║
	██║  ██║███████╗██║  ██║╚██████╗   ██║
	╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝ ╚═════╝   ╚═╝
	(REACT)

	Written by:			Felix Mark and Nona Netta

	Studies:			Electronics and Computer Engineering
	Subject:			Object-oriented Software Design
	Last modified:		28.04.2018
*/

// Includes
#include <wiringPi.h>
#include <iostream>
#include <string>
#include <chrono>
#include <memory>
#include <fstream>
#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "Player.h"
#include "ReactionGameHandler.h"
#include "json.hpp"
#include "PinManager.h"


// Namespaces
using namespace std;
using namespace chrono;

using json = nlohmann::json;


// Reactiongamehandler
shared_ptr<Reaction_game_handler> reaction_game_handler = NULL;


// Make shared pointer functions
std::shared_ptr<Digital_input> make_digital_input(std::string name, int pin, int pull_updown) {
	return std::make_shared<Digital_input>(Digital_input(name, pin, pull_updown));
}
std::shared_ptr<Digital_output> make_digital_output(std::string name, int pin, Status status) {
	return std::make_shared<Digital_output>(Digital_output(name, pin, status));
}

// ISRs of the Pushbuttons
void btn_player1_isr() {
	if (DEBUG) cout << "Button 1 pressed." << endl;
	if (!reaction_game_handler->get_btn_p1_pressed() && !reaction_game_handler->get_btn_p2_pressed()) {
		reaction_game_handler->set_btn_p1_pressed(true);
		reaction_game_handler->set_last_pressed(system_clock::now().time_since_epoch());
	} else if (!reaction_game_handler->get_btn_p1_pressed() && reaction_game_handler->get_btn_p2_pressed()) {
		reaction_game_handler->set_btn_p1_pressed(true);
		nanoseconds end = system_clock::now().time_since_epoch();
		int64_t elapsed_time_us = duration_cast<microseconds>(end).count() - duration_cast<microseconds>(reaction_game_handler->get_last_pressed()).count();
		reaction_game_handler->set_time_difference(elapsed_time_us);
	}
}
void btn_player2_isr() {
	if (DEBUG) cout << "Button 2 pressed." << endl;
	if (!reaction_game_handler->get_btn_p1_pressed() && !reaction_game_handler->get_btn_p2_pressed()) {
		reaction_game_handler->set_btn_p2_pressed(true);
		reaction_game_handler->set_last_pressed(system_clock::now().time_since_epoch());
	} else if (reaction_game_handler->get_btn_p1_pressed() && !reaction_game_handler->get_btn_p2_pressed()) {
		reaction_game_handler->set_btn_p2_pressed(true);
		nanoseconds end = system_clock::now().time_since_epoch();
		int64_t elapsed_time_us = duration_cast<microseconds>(end).count() - duration_cast<microseconds>(reaction_game_handler->get_last_pressed()).count();
		reaction_game_handler->set_time_difference(elapsed_time_us);
	}
}


//================================= MAIN =================================
int main(void) {
	wiringPiSetup();

	// Load pin configuration
	ifstream file("pins.json");
	json json_object;
	file >> json_object;

	// Pin manager for handling pins
	Pin_manager pm;

	// Create Variables and Inputs/Outputs
	shared_ptr<Digital_input> btn_p1 = make_digital_input(
		"btn_p1", pm.reserve(json_object["p1_button"].get<int>()), PUD_DOWN);
	shared_ptr<Digital_input> btn_p2 =  make_digital_input(
		"btn_p2", pm.reserve(json_object["p2_button"].get<int>()), PUD_DOWN);
	shared_ptr<Digital_output> p1_won_led = make_digital_output(
		"p1_won_led", pm.reserve(json_object["p1_led"].get<int>()), Status::OFF);
	shared_ptr<Digital_output> p2_won_led = make_digital_output(
		"p2_won_led", pm.reserve(json_object["p2_led"].get<int>()), Status::OFF);
	shared_ptr<Digital_output> prepare_led = make_digital_output(
		"prepare_led", pm.reserve(json_object["prepare_led"].get<int>()), Status::OFF);
	shared_ptr<Digital_output> warn_led = make_digital_output(
		"warn_led", pm.reserve(json_object["state"].get<int>()), Status::OFF);
	shared_ptr<Digital_output> piezo = make_digital_output(
		"piezo_speaker", pm.reserve(json_object["piezo_speaker"].get<int>()), Status::OFF);
	
	// Create Reactiongamehandler
	reaction_game_handler = make_shared<Reaction_game_handler>(
		Reaction_game_handler(prepare_led, warn_led, piezo, p1_won_led, p2_won_led, btn_p1, btn_p2)
	);

	// Set ISRs for the Pushbuttons
	wiringPiISR(btn_p1->get_pin(), INT_EDGE_RISING, &btn_player1_isr);
	wiringPiISR(btn_p2->get_pin(), INT_EDGE_RISING, &btn_player2_isr);

	// Start the Reactiongame
	reaction_game_handler->run_game();

	return 0;
}


