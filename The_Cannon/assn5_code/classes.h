#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
using namespace std;

typedef enum{OUR_ACTION, OPPONENT_ACTION} action_by;
typedef enum{CANNON_SHOT, SOLDIER_MOVE}action_type;
typedef	enum{BLACK_SOLDIER, WHITE_SOLDIER, BLACK_TOWNHALL, WHITE_TOWNHALL, NONE} cell_content;
typedef enum {WHITE, BLACK} color;		// my_color represents, am I black or white.
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
		// following variables are introduced for eval function
		int horizontal_cannons_diff, verticle_cannons_diff, right_diagonal_cannons_diff, left_diagonal_cannons_diff,
		    valid_moves_diff, soldier_kills_diff, townhall_kills_diff;
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
		float utility_function(bool);
		vector<action> update_white_feature_values();
		vector<action> update_black_feature_values();
		void print_all_actions(vector<action>);
		void print_board();
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
		void print_explored_boards();
};
