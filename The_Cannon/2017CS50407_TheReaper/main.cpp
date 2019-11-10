#include <iostream>
#include <time.h>
#include "classes.h"
#include "functions.h"
using namespace std;

int main()
{
	srand(time(0));
	//values fixed for assn 2.
	int n = 8, m = 8, soldiers_per_team = 12, time_ms=0, time_spent_so_far_ms=0;
	// read what is our player id
	// 1 will make the first move
	int player_id = 1, index=0, calibration_string_length=0;
	string calibration="", temp_string;
	getline(cin, calibration);
	calibration_string_length = calibration.length();
	while(calibration[index] != ' ')		{ temp_string += calibration[index]; index++;}
	player_id = stoi(temp_string);
	index++;	// space
	temp_string = "";
	while(calibration[index] != ' ')		{ temp_string += calibration[index]; index++;}
	n = stoi(temp_string);
	index++;
	temp_string = "";
	while(calibration[index] != ' ')		{ temp_string += calibration[index]; index++;}
	m = stoi(temp_string);
	index++;
	temp_string = "";
	while(index < calibration_string_length)	{ temp_string += calibration[index]; index++;}
	time_ms = stoi(temp_string)*1000;		// milli seconds
	if(n==10 and m == 10)	soldiers_per_team = 15;	


	// we can be BLACK or WHITE. position of black and white are fix.
	action_by action_by_me, action_by_opponent;
	bool is_my_first_turn;
	color my_color;
	if(player_id == 1)	{ my_color = BLACK; action_by_me = OUR_ACTION; action_by_opponent = OPPONENT_ACTION; is_my_first_turn = true;}
	else			{ my_color = WHITE; action_by_me = OPPONENT_ACTION; action_by_opponent = OUR_ACTION; is_my_first_turn = false;}

	transposition_table explored_boards = transposition_table(n*m, 4);
	custom_hash zobrist_hashing = custom_hash(n*m, 4);
	// states will be stored after i make a move
	transposition_table visited_states = transposition_table(n*m, 4);
	long long hash_val;

	// initializing the board
	srand(time(0));
	board game = board(my_color, n, m, soldiers_per_team);

	///////////////////////// deleteThis part while submitting	///////////////////////
//	fstream myfile;
//	myfile.open("weights.txt");
//	if (myfile.is_open())
//	{
//		string line = "";
//		float w1, w2, w3, w4, w5, w6, w7, w8;
//		getline (myfile,line);
//		w1 = stof(line);
//		getline (myfile,line);
//		w2 = stof(line);
//		getline (myfile,line);
//		w3 = stof(line);
//		getline (myfile,line);
//		w4 = stof(line);
//		getline (myfile,line);
//		w5 = stof(line);
//		getline (myfile,line);
//		w6 = stof(line);
//		getline (myfile,line);
//		w7 = stof(line);
//		getline (myfile,line);
//		w8 = stof(line);
//		game.init_weights(w1, w2, w3, w4, w5, w6, w7, w8);
//	    	myfile.close();
//	}
//	else	cerr << "unable to open the file.\n";
	//////////////////////////////////////////////////////////////////////////////////////

	pair<board, event_type> game_event;
	event_type event = NO_EVENT;
	hash_val = zobrist_hashing.findhash(game);

	// if stagnant case occures then don't use history to anser
	bool make_different_move = false;

	while(true)
	{
		time_point<system_clock> start = system_clock::now(), end;
		// opponent will make a move.
		// we are white.
		if(not is_my_first_turn)
		{
			action_type opp_action_t = CANNON_SHOT;
			
			// take input from server
			int opp_old_r=-1, opp_old_c=-1, opp_new_r=-1, opp_new_c=-1;
			string opponent_move_string;
			getline(cin, opponent_move_string);

			// starting our timer
			start = system_clock::now();
				

			opp_old_c = opponent_move_string[2] - '0';
			opp_old_r = opponent_move_string[4] - '0';
			if(opponent_move_string[6] == 'B')		opp_action_t = CANNON_SHOT;
			else						opp_action_t = SOLDIER_MOVE;
			opp_new_c = opponent_move_string[8] - '0';
			opp_new_r = opponent_move_string[10] - '0';
			
			action opponent_action = action(make_pair(opp_old_r, opp_old_c), make_pair(opp_new_r, opp_new_c), opp_action_t);
			hash_val = zobrist_hashing.next_hash(game, hash_val, opponent_action);
			game_event = result(game, opp_old_r, opp_old_c, opp_new_r, opp_new_c, opp_action_t, action_by_opponent, my_color);	// our_action := black_action
			game = game_event.first;
			event = game_event.second;
			 // leaf test
			int cut_off_depth=3;
			if(game.terminal_test(1, cut_off_depth))	break;
			/////////////////////////////	updating eval function after a move	///////////////////////////////////
			game.update_event_feature_weights(event);
//			game.print_weights();
//			print_event(event);
			event = NO_EVENT;	
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		is_my_first_turn = false;
		action best_action;
		
		// take a decision whether it is a stagnant game or not
		// if the game is seen for 2 times
		if(visited_states.get_freq(hash_val, &game) == 2)
		{
			cerr << " !!! stagnant move !!!\n";
			make_different_move = true;
		}


		//////// Searching current state in transposition table
		//// hash value is ready for current state
		pair<bool, action> find_in_tt = explored_boards.find(hash_val, &game);
		if(find_in_tt.first and (not make_different_move))
		{
			// take a decision whether it is a stagnant game or not
			// if the game is seen for 2 times
			best_action = find_in_tt.second;
			hash_val = zobrist_hashing.next_hash(game, hash_val, best_action);
			// no need to search
			cout << "S " << best_action.get_soldier_c() << " " << best_action.get_soldier_r() << " ";
			if(best_action.get_action_type() == SOLDIER_MOVE)	cout << "M ";
			else							cout << "B ";
			cout << best_action.get_target_c() << " " << best_action.get_target_r() << endl;

			// stopping our timer
			end = system_clock::now();
			duration<double> elapsed_seconds = end-start;
			time_spent_so_far_ms += elapsed_seconds.count()*1000;
			
			game_event = result(	game, best_action.get_soldier_r(), best_action.get_soldier_c(), 
						best_action.get_target_r(), best_action.get_target_c(), best_action.get_action_type(), action_by_me, my_color);
			game = game_event.first;
			if(visited_states.find(hash_val, &game).first)	{ visited_states.add_freq(hash_val, &game);}
			else						visited_states.insert(hash_val, game);
			continue;
		}	

		//  	Game duration: 
		//    			|------- 30% ------|----------------- 40% -------------|--- 15% ---|-- 14% --|- 1% -|
		//    	Time/move:	1500 ms			3000 ms				1000 ms	    500 ms    100 ms
		//    	Just to make sure that time out does not occure due to us, introduced a very quick step in the last 1% of the game
		if(time_ms - time_spent_so_far_ms >= (0.7)*time_ms)
		{
			if(make_different_move)	
			{
			       	best_action = minimax_decision(game, 4000, my_color);
				if(best_action.get_action_type() == CANNON_SHOT)	best_action = minimax_decision(game, -1, my_color);
				make_different_move = false;
			}
			else	best_action = minimax_decision(game, 1500, my_color);
		}
		else if(time_ms - time_spent_so_far_ms >= 0.3*time_ms)
		{
			if(make_different_move)	
			{
			       	best_action = minimax_decision(game, 5000, my_color);
				if(best_action.get_action_type() == CANNON_SHOT)	best_action = minimax_decision(game, -1, my_color);
				make_different_move = false;
			}
			else	best_action = minimax_decision(game, 3000, my_color);
		}
		else if(time_ms- time_spent_so_far_ms >= 0.15*time_ms)
		{
			if(make_different_move)	
			{
			       	best_action = minimax_decision(game, 5000, my_color);
				if(best_action.get_action_type() == CANNON_SHOT)	best_action = minimax_decision(game, -1, my_color);
				make_different_move = false;
			}
			else	best_action = minimax_decision(game, 1000, my_color);
		}
		else if(time_ms - time_spent_so_far_ms >= 0.01*time_ms)
		{
			if(make_different_move)	
			{
			       	best_action = minimax_decision(game, -1, my_color);
				make_different_move = false;
			}
			else	best_action = minimax_decision(game, 500, my_color);
		}
		else	
		{
			if(make_different_move)	
			{
			       	best_action = minimax_decision(game, -1, my_color);
				make_different_move = false;
			}
			else	best_action = minimax_decision(game, 100, my_color);
		}
		// printing the results
		cout << "S " << best_action.get_soldier_c() << " " << best_action.get_soldier_r() << " ";
	        if(best_action.get_action_type() == SOLDIER_MOVE)	cout << "M ";
		else							cout << "B ";
		cout << best_action.get_target_c() << " " << best_action.get_target_r() << endl;

		// stopping our timer
		end = system_clock::now();
		duration<double> elapsed_seconds = end-start;

		time_spent_so_far_ms += elapsed_seconds.count()*1000;
		
		// assigning best action found for this perticular board config
		game.set_best_action_found(best_action);
		// adding into the table
		explored_boards.insert(hash_val, game);
		// updating the hash value
		hash_val = zobrist_hashing.next_hash(game, hash_val, best_action);
		// modifing the game board
		game_event = result(game, best_action.get_soldier_r(), best_action.get_soldier_c(), 
				best_action.get_target_r(), best_action.get_target_c(), 
				best_action.get_action_type(), action_by_me, my_color);
		game = game_event.first;
		event = game_event.second;
		// leaf test
		int cut_off_depth = 3;
		if(game.terminal_test(1, cut_off_depth))	break;
		////////////////////////////	updating eval funciton after my ply	///////////////////////////
		game.update_event_feature_weights(event);
//		game.print_weights();
//		print_event(event);
		event = NO_EVENT;
		///////////////////////////////////////////////////////////////////////////////////////////////////
		if(visited_states.find(hash_val, &game).first)	{ visited_states.add_freq(hash_val, &game);}
		else						visited_states.insert(hash_val, game);
	}
	return 0;
}







//	if(game.terminal_test())
//		cout << "terminal state with utility function: " << game.utility_function() << endl;
//	else
//		cout << "not a terminal state.\n";
	//game.print_black_townhalls();
	//game.print_black_soldiers();
	//game.print_white_townhalls();
//	//game.print_white_soldiers();
//
//	game.print_all_actions(game.get_opponent_action());

//	int soldier_r = 2, soldier_c = 1, target_r = 4, target_c = 3;
//	board new_game = result(game, soldier_r, soldier_c, target_r, target_c, SOLDIER_MOVE, OPPONENT_ACTION);
//	soldier_r = 5;
//	soldier_c = 2;
//	target_r = 0;
//	target_c = 2;
//	new_game = result(new_game, soldier_r, soldier_c, target_r, target_c, SOLDIER_MOVE, OPPONENT_ACTION);
	
//	if(new_game.terminal_test())
//		cout << "terminal state with utility function: " << new_game.utility_function() << endl;
//	new_game.print_black_townhalls();
//	new_game.print_black_soldiers();
//	new_game.print_white_townhalls();
//	new_game.print_white_soldiers();

