#include <iostream>
#include "classes.h"
#include<cmath>
#include "functions.h"
#include <algorithm>
#include <exception>
using namespace std;

const float ALPHA = 0.001;

// assumption - board result(board& s, const int soldier_r, const int soldier_c, const int target_r, const int target_c, action_type action_t, action_by whose_action)
//            - vector<actions> get_possible_action(board s)
//            - boolean terminal_test(board s)
//            - int utility_function(board s)

//  terminal test returns 	true if number of townhalls of either team is 2
//  				false if number of townhalls of both teams are greater than 2
//  1) clearly this function does not considers the fact that if there are no moves possible
//  for either team, the state is a leaf node. Incorporating this fact will increase the
//  time complexity bcz then we will need to see the possible moves of the player.
//  BUT we can incorporate this later in the code when we will identify the possible
//  moves, so if no move possible then stop and return it's utility function.
//
// utility function just implements the table given in the assignment. It will work as expected.

//float max_value(board state, int depth);
//float min_value(board state, int depth)
float max_value(board state, int depth, const int &cut_off_depth, float alpha, float beta, time_point<system_clock> &start_time_point, int time_per_move, color my_color); 
// min_value and max_value are interdependent

float min_value(board state, int depth, const int &cut_off_depth, float alpha, float beta, time_point<system_clock> &start_time_point, int time_per_move, color my_color)
{
	try
	{
		// half a second per move
		if( ((duration<double>)(system_clock::now() - start_time_point)).count()*1000 >= time_per_move)		throw runtime_error("Time out.");

		if (state.terminal_test(depth, cut_off_depth))
		{
//			state.update_white_feature_values();
//			state.update_black_feature_values();
//			return state.eval_function();
			return state.dynamic_eval_function();
		}
		else
		{
			action_by action_by_me, action_by_opponent;
		   	vector<action> moves_possible;
       		    	if(state.get_my_color() == BLACK)	{ moves_possible = state.get_opponent_action(); action_by_me = OUR_ACTION; action_by_opponent = OPPONENT_ACTION;}	
       	    		else					{ moves_possible = state.get_possible_action(); action_by_me = OPPONENT_ACTION; action_by_opponent = OUR_ACTION;}
			float v = INFINITY;
			// check if no move possible
			if (moves_possible.size() == 0)
			{
	    			return state.utility_function(true);	// opponent does not have moves, so i have moves
			}
			for (action move : moves_possible)
			{
	    			v = min(v, max_value(result(state, move.get_soldier_r(), move.get_soldier_c(), 
						move.get_target_r(), move.get_target_c(), 
						move.get_action_type(), action_by_opponent, my_color).first,
						depth+1, cut_off_depth, alpha, beta, start_time_point, time_per_move, my_color));
				if (v <= alpha)	return v - ALPHA;
				beta = min(beta, v);
			}
			return v - ALPHA;
		}
	}
	catch(exception e)
	{
		throw e;
	}
}

//float max_value(board state, int depth)
float max_value(board state, int depth, const int &cut_off_depth, float alpha, float beta, time_point<system_clock> &start_time_point, int time_per_move, color my_color)
{
    try
    {
	    // half a second for each move
	    if( ((duration<double>)(system_clock::now() - start_time_point)).count()*1000 >= time_per_move)	throw runtime_error("Time out."); 

	    if (state.terminal_test(depth, cut_off_depth))
	    {
//		state.update_white_feature_values();
//		state.update_black_feature_values();
//		return state.eval_function();
		return state.dynamic_eval_function();
	    }
	    else
	    {
		action_by action_by_me, action_by_opponent;
	   	vector<action> moves_possible;
       	    	if(state.get_my_color() == BLACK)		{ moves_possible = state.get_possible_action(); action_by_me = OUR_ACTION; action_by_opponent = OPPONENT_ACTION;}	
       	    	else						{ moves_possible = state.get_opponent_action(); action_by_me = OPPONENT_ACTION; action_by_opponent = OUR_ACTION;}
		
		float v = -INFINITY;
		// check if no moves possible
		if (moves_possible.size() == 0)
		{
		    return state.utility_function(false);	// i dont have moves
		}

		for (action move : moves_possible)
		{
		    v = max(v, min_value(result	(state,move.get_soldier_r(), move.get_soldier_c(), 
						move.get_target_r(), move.get_target_c(), 
						move.get_action_type(), action_by_me, my_color).first, 
						depth+1, cut_off_depth, alpha, beta, start_time_point, time_per_move, my_color));
		    if (v >= beta)	return v - ALPHA;
		    alpha = max(alpha, v);
		}
		return v - ALPHA;
	    }
    }
    catch(exception e)
    {
	    throw e;
    }
}
bool compare_decr_cost(action &a1, action &a2)
{
	return a1.get_action_cost() > a2.get_action_cost();
}
action minimax_decision(board root_state, int time_per_move, color my_color)
{
	// getting the start time
	time_point<system_clock> start;
	start = system_clock::now();
	duration<double> elapsed_seconds = start-start;

	vector<action> moves_possible;
	action_by action_by_me, action_by_opponent;
	if(root_state.get_my_color() == BLACK)		{ moves_possible = root_state.get_possible_action(); action_by_me = OUR_ACTION; action_by_opponent = OPPONENT_ACTION;}	
	else						{ moves_possible = root_state.get_opponent_action(); action_by_me = OPPONENT_ACTION; action_by_opponent = OUR_ACTION;}


	action best = moves_possible[0], move;

	// if time_per_move is negative then return random move
	if(time_per_move < 0)
	{
		while(true)
		{
			int random_index = rand() % (moves_possible.size());
			if(moves_possible[random_index].get_action_type() != CANNON_SHOT)	return moves_possible[random_index];
		}
	}

	int moves_possible_size = moves_possible.size(), cut_off_depth = 2;
	float v = -1, i=0;
	// if time has not elapsed
	// half a second for one move
	while(true)
	{
		try
		{
//			cout << " \n\ncut_off_depth: " << cut_off_depth << endl;
//			root_state.print_all_actions(moves_possible);
			for(i=0; i<moves_possible_size; i++)
			{
				move = moves_possible[i];
				float bound = min_value(result(root_state, move.get_soldier_r(), move.get_soldier_c(), 
							move.get_target_r(), move.get_target_c(), move.get_action_type(), action_by_me, my_color).first,
							1, cut_off_depth, -INFINITY, INFINITY, start, time_per_move, my_color);
				moves_possible[i].set_action_cost(bound);

				if (v <= bound) //WARNING: NO ORDERING OF MOVES WITH SAME VALUE
				{
//					cerr << "backed up value is: " << bound << endl;
					v = bound;
					best = move;
				}
			}
			// running out of time
			sort(moves_possible.begin(), moves_possible.end(), compare_decr_cost);
			cut_off_depth++;
		}
		catch(exception e)
		{
			break;
		}
	}
	return best;
}
