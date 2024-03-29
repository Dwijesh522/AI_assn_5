#pragma once
#include<chrono>
#include "classes.h"
using namespace std;
using namespace chrono;
// soldier_r: 	r coordinate of a soldier under consideration
// soldier_c:	c coordinate of a soldier under consideration
// target_r: 	r coordinate of a target
// target_c:	c coordinate of a target

// returns a new board configuration
board result(board &old_board, const int &soldier_r, const int &soldier_c, const int &target_r, const int &target_c, action_type action_t, action_by whose_action);
float max_value(board state, int depth, const int &cut_off_depth, float alpha, float beta, time_point<system_clock> &start_time_ms, int time_per_move);
float min_value(board state, int depth, const int &cut_off_depth, float alpha, float beta, time_point<system_clock> &start_time_ms, int time_per_move);
action minimax_decision(board root_state, int time_per_move);

