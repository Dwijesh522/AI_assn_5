#pragma once
#include <fstream>
#include <cmath>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <assert.h>
using namespace std;

typedef enum {OUR_ACTION, OPPONENT_ACTION} action_by;
typedef enum {CANNON_SHOT, SOLDIER_MOVE}action_type;
typedef	enum {BLACK_SOLDIER, WHITE_SOLDIER, BLACK_TOWNHALL, WHITE_TOWNHALL, NONE} cell_content;
typedef enum {WHITE, BLACK} color;		// my_color represents, am I black or white.
typedef enum {	I_KILLED_SOLDIER_POS, OPP_KILLED_SOLDIER_POS, I_KILLED_TOWNHALL, OPP_KILLED_TOWNHALL, I_KILLED_SOLDIER_NEG, OPP_KILLED_SOLDIER_NEG,
		ME_ADDING_TOWNHALL_COVER, ME_ADDING_SOLDIER_COVER, 
		ME_UNDER_CANNON_ATTACK, ME_UNDER_SOLDIER_ATTACK,
		NO_EVENT} event_type;
class soldier
{
	private:
		int r, c;
	public:
		soldier(int r, int c);
		int get_r();
		int get_c();
		void set_r_c(int r, int c);
};
class townhall
{
	private:
		int r, c;
	public:
		townhall(int r, int c);
		int get_r();
		int get_c();
};
class action
{
	private:
		pair<int, int> current_pos;
		pair<int, int> future_pos;
		action_type action_t;
		float action_cost;
	public:
		action(pair<int, int> cp, pair<int, int> fp, action_type action_t);
		action();
		int get_soldier_r();
		int get_soldier_c();
		int get_target_r();
		int get_target_c();
		action_type get_action_type();
		pair<int, int> get_current_pos();
		pair<int, int> get_future_pos();
		float get_action_cost();
		void set_action_cost(float cost);
};
class board
{
	private:
		color my_color;
		vector<soldier> black_soldiers, white_soldiers;
		vector<townhall> black_townhalls, white_townhalls;
		// each cell in the grid will either be a pointer to soldier or
		// pointer to townhall, atleast one of both must be a null pointer.
		vector<vector<cell_content > > grid;
		int r, c, soldiers_per_team;
		action best_action_found;
		int freq;
		// following variables are introduced for eval function
		int horizontal_cannons_diff, verticle_cannons_diff, right_diagonal_cannons_diff, left_diagonal_cannons_diff,
		    valid_moves_diff, soldier_kills_diff, townhall_kills_diff,
		    our_soldiers_under_soldier_attack=0, our_soldiers_under_cannon_attack=0, our_townhall_covering_agents=0, our_soldier_covering_agents=0;
		// weights used in the eval function
		float our_soldiers_weight = 0,	
		      enemy_soldiers_weight = 0,
		      our_townhalls_weight = 0,
		      enemy_townhalls_weight = 0,
		      our_soldiers_under_soldier_attack_weight=0,
		      our_soldiers_under_cannon_attack_weight=0,
		      our_townhall_covering_agents_weight=0,
		      our_soldier_covering_agents_weight=0;
//		      soldiers_i_can_kill_weight=0,
//		      soldiers_enemy_can_kill_weight=0,
//		      townhalls_i_can_kill_weight=0,
//		      townhalls_enemy_can_kill_weight=0;
//		float our_soldiers_weight = 9,	
//		      enemy_soldiers_weight = -10,
//		      soldier_diff_weight = 0,
//		      townhall_diff_weight = 100,
//		      our_horizontal_cannons_weight = 1,
//		      enemy_horizontal_cannons_weight = 0,
//		      our_verticle_cannons_weight = 3,
//		      enemy_verticle_cannons_weight = 0,
//		      our_right_cannons_weight = 3.5,
//		      enemy_right_cannons_weight = 0,
//		      our_left_cannons_weight = 3.5,
//		      enemy_left_cannons_weight = 0,
//		      soldiers_i_can_kill_weight = 1,
//		      soldiers_enemy_can_kill_weight = -1,
//		      townhalls_i_can_kill = 50,
//		      townhalls_enemy_can_kill = -90,
//		      our_moves_weight = 6,
//		      opp_moves_weight = 0;
	public:
		// complete initializatin of the board will be done here
		board();
		board(color turn, int r, int c, int soldiers_per_team);
		cell_content get_cell(int r, int c);
		vector<vector<cell_content> > get_grid();
		color get_my_color();
		void remove_black_soldier(const int &r, const int &c);
		void remove_white_soldier(const int &r, const int &c);
		void remove_black_townhall(const int &r, const int &c);
		void remove_white_townhall(const int &r, const int &c);
		void update_white_soldier(const int &old_r, const int &old_c, const int &new_r, const int &new_c);
		void update_black_soldier(const int &old_r, const int &old_c, const int &new_r, const int &new_c);
		// following is actually get_black_actions
		vector<action> get_possible_action();
		// following is actually get_white_actions
		vector<action> get_opponent_action();
		bool board_equals(board& other);
		action get_best_action_found();
		void set_best_action_found(action best_action);
		bool terminal_test(int depth, const int &cut_off_depth);
		float dummy_utility_function();
		float eval_function();
		float dynamic_eval_function();
		float utility_function(bool);
		float new_utility_function(bool);
		vector<action> update_white_feature_values();
		vector<action> update_black_feature_values();
		void update_event_feature_values();
		void update_event_feature_weights(event_type event);
		void softmax_to_weights();
		int get_number_of_rows();
		int get_number_of_cols();
		void add_freq();
		int get_freq();
		void init_freq();
		void init_weights(float, float, float, float, float, float, float, float);
		void print_all_actions(vector<action>);
		void print_board();
		void print_weights();
		void print_feature_values();
		void print_black_soldiers();
		void print_white_soldiers();
		void print_black_townhalls();
		void print_white_townhalls();
};
class custom_hash
{
	private:
		vector<vector<long long>> rand_grid;
		map<cell_content, int> lt;
	public:
		custom_hash();
		custom_hash(int r, int c);
		long long findhash(board &a);
		long long next_hash(board &bo, long long prev_hash, action &a);
		void print_rand_grid();
};
class transposition_table
{
	private:
		unordered_map<long long, board> mp;
		custom_hash hash_fn;
	public:
		transposition_table(int r, int c);
		void insert(long long hn, board b);
		pair<bool, action> find(long long hn, board *b1);
		pair<bool, board> find_stagnant(long long hn, board *b1);
		void add_freq(long long hn, board *b1);
		int get_freq(long long hn, board *b1);
		void print_explored_boards();
};
