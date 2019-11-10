#include<vector>
#include<utility>
#include<iostream>
#include "classes.h"
#include "functions.h"
//#include<fstream>
using namespace std;

const float BETA = 1.0;

board::board(){}
board::board(color my_color, int r, int c, int soldiers_per_team)
{
	horizontal_cannons_diff=0;	verticle_cannons_diff=0;	right_diagonal_cannons_diff=0;	
	left_diagonal_cannons_diff=0;	valid_moves_diff=0;		soldier_kills_diff=0;
	townhall_kills_diff=0;

	this->best_action_found = action(make_pair(-1, -1), make_pair(-1, -1), SOLDIER_MOVE);
	this->my_color = my_color;
	this->r = r;
	this->c = c;
	this->soldiers_per_team = soldiers_per_team;
	this->freq = 1;
	int soldiers_in_coloumn = soldiers_per_team/(c/2);
	// filling up the first row
	vector<cell_content> row_of_grid;
	cell_content temp_cell = NONE;
	for(int j=0; j<c; j++)
	{
		if(j%2==0)	// white townhalls
		{
			townhall temp_townhall = townhall(0, j);
			white_townhalls.push_back(temp_townhall);
			row_of_grid.push_back(temp_cell);
		}
		else		// white soldiers
		{
			soldier temp_soldier = soldier(0, j);
			white_soldiers.push_back(temp_soldier);
			row_of_grid.push_back(temp_cell);
		}
	}
	grid.push_back(row_of_grid);
	// filling up rows from index 1 to index (soldiers_per_coloumn -1)
	for(int i=1; i<soldiers_in_coloumn; i++)
	{
		// filling up the ith row
		for(int j=0; j<c; j++)
			if(j%2==1)	// white soldiers
			{
				soldier temp_soldier = soldier(i, j);
				white_soldiers.push_back(temp_soldier);
			}
		grid.push_back(row_of_grid);
	}
	// filling up the empty rows
	// indices from (soldiers_in_coloumn)th to (r-soldiers_in_coloumn -1 )th
	int bound = r - soldiers_in_coloumn - 1;
	for(int i=soldiers_in_coloumn; i<= bound; i++)	grid.push_back(row_of_grid);
	// filling up the black soldiers.
	// indices: from (bound+1)th to (r-2)th
	for(int i=bound+1; i<r-1; i++)
	{
		// filling up the ith row
		for(int j=0; j<c; j++)
			if(j%2==0)	// black soldier
			{
				soldier temp_soldier = soldier(i, j);
				black_soldiers.push_back(temp_soldier);
			}
		grid.push_back(row_of_grid);
	}
	// filling up the last row
	for(int j=0; j<c; j++)
	{
		if(j%2==0)	// black soldier
		{
			soldier temp_soldier = soldier(r-1, j);
			black_soldiers.push_back(temp_soldier);
		}
		else		// black townhall
		{
			townhall temp_townhall = townhall(r-1, j);
			black_townhalls.push_back(temp_townhall);
		}
	}
	grid.push_back(row_of_grid);

	// filling up the grids
	for(int i=0; i<soldiers_per_team; i++)
	{
		// black soldiers
		int r = black_soldiers[i].get_r(), c = black_soldiers[i].get_c();
		grid[r][c] = BLACK_SOLDIER;
		// white soldiers
		r = white_soldiers[i].get_r(), c = white_soldiers[i].get_c();
		grid[r][c] = WHITE_SOLDIER;
	}
	int half_col = c/2;
	for(int i=0; i<half_col; i++)
	{
		// black townhalls
		int r = black_townhalls[i].get_r(), c = black_townhalls[i].get_c();
		grid[r][c] = BLACK_TOWNHALL;
		// white townhalls
		r = white_townhalls[i].get_r(), c = white_townhalls[i].get_c();
		grid[r][c] = WHITE_TOWNHALL;
	}
	enemy_soldiers_weight = -3.419;
	our_soldiers_weight = 2.769;
	enemy_townhalls_weight = -20.6;
	our_townhalls_weight = 52;
	our_townhall_covering_agents_weight = 0.272;
	our_soldier_covering_agents_weight = 0.0750797;
	our_soldiers_under_soldier_attack_weight = -0.6;
	our_soldiers_under_cannon_attack_weight = -2.99;

}
//void board::init_weights(float w1, float w2, float w3, float w4, float w5, float w6, float w7, float w8 )
//{
//	cerr << "initializing the weights: " << w1<<" "<< w2 <<" "<< w3 <<" "<< w4 <<" "<< w5 <<" "<< w6 <<" "<< w7 <<" "<< w8 <<" "<< endl; 
//	enemy_soldiers_weight = w1;
//	our_soldiers_weight = w2;
//	enemy_townhalls_weight = w3;
//	our_townhalls_weight = w4;
//	our_townhall_covering_agents_weight = w5;
//	our_soldier_covering_agents_weight = w6;
//	our_soldiers_under_soldier_attack_weight = w7;
//	our_soldiers_under_cannon_attack_weight = w8;
//}
vector<vector<cell_content> > board::get_grid()
{
	return grid;
}
bool board::board_equals(board &other)							////////////////////////////////////////////
{											//////	this fxn can be optimized  /////////
	vector<vector<cell_content> > other_grid = other.get_grid();			//////  by traversing only lists   /////////
	for(int i = 0; i<r;i++)								////////////////////////////////////////////
	{
		for(int j = 0; j<c; j++)
		{
			if(grid[i][j]!=other_grid[i][j])
				return false;
		}
	}
	return true;
}
void board::add_freq()
{
	freq++;
}
int board::get_freq()
{
	return freq;
}
void board::init_freq()
{
	freq = 1;
}
cell_content board::get_cell(int r, int c)
{
	return grid[r][c];
}
color board::get_my_color()
{
	return my_color;
}
action board::get_best_action_found()
{
	return best_action_found;
}
void board::set_best_action_found(action best_action)
{
	best_action_found = action(	make_pair(best_action.get_soldier_r(), best_action.get_soldier_c()), 
					make_pair(best_action.get_target_r(), best_action.get_target_c()), 
					best_action.get_action_type());
}
int board::get_number_of_rows()
{
	return r;
}
int board::get_number_of_cols()
{
	return c;
}
void board::remove_black_soldier(const int &r, const int &c)
{
	// modifing the grid
	grid[r][c] = {NONE};
	// modifing the soldier vector
	auto black_soldiers_end = black_soldiers.end();
	for(auto it=black_soldiers.begin(); it!=black_soldiers_end; it++)
		if(it->get_r() == r and it->get_c() == c) { black_soldiers.erase(it); break;}
}
void board::remove_white_soldier(const int &r,const int &c)
{
	// modifing the grid
	grid[r][c] = {NONE};
	// modifing the soldier vector
	auto white_soldiers_end = white_soldiers.end();
	for(auto it=white_soldiers.begin(); it!=white_soldiers_end; it++)
		if(it->get_r() == r and it->get_c() == c) { white_soldiers.erase(it); break;}
}
void board::remove_black_townhall(const int &r, const int &c)
{
	// modifing the grid
	grid[r][c] = {NONE};
	// modifing the soldier vector
	auto black_townhalls_end = black_townhalls.end();
	for(auto it=black_townhalls.begin(); it!=black_townhalls_end; it++)
		if(it->get_r() == r and it->get_c() == c) { black_townhalls.erase(it); break;}
}
void board::remove_white_townhall(const int &r, const int &c)
{
	// modifing the grid
	grid[r][c] = {NONE};
	// modifing the soldier vector
	auto white_townhalls_end = white_townhalls.end();
	for(auto it=white_townhalls.begin(); it!=white_townhalls_end; it++)
		if(it->get_r() == r and it->get_c() == c) { white_townhalls.erase(it); break;}
}
// updates the soldier at old coordinates to have new coordinates
// updates the old grid postion to NONE and new position to have corresponding old enitity
void board::update_white_soldier(const int &old_r, const int &old_c, const int &new_r, const int &new_c)
{
	// updating the grid
	cell_content old_cell = grid[old_r][old_c];
	grid[old_r][old_c] = NONE;
	grid[new_r][new_c] = old_cell;
	//updating white soldier vector
	int white_soldiers_length = white_soldiers.size();
	for(int i=0; i<white_soldiers_length; i++)
		if(white_soldiers[i].get_r() == old_r and white_soldiers[i].get_c() == old_c)
			{ white_soldiers[i].set_r_c(new_r, new_c); break;}
}
// updates the soldier at old coordinates to have new coordinates
// updates the old grid postion to NONE and new position to have corresponding old enitity
void board::update_black_soldier(const int &old_r, const int &old_c, const int &new_r, const int &new_c)
{
	// updating the grid
	cell_content old_cell = grid[old_r][old_c];
	grid[old_r][old_c] = NONE;
	grid[new_r][new_c] = old_cell;
	//updating black soldier vector
	int black_soldiers_length = black_soldiers.size();
	for(int i=0; i<black_soldiers_length; i++)
		if(black_soldiers[i].get_r() == old_r and black_soldiers[i].get_c() == old_c)
		{
			black_soldiers[i].set_r_c(new_r, new_c); 
			break;
		}	
}
void board::print_weights()
{
	fstream myfile;
	myfile.open("weights.txt");
	if(myfile.is_open())
	{
		myfile << enemy_soldiers_weight << "\n" << our_soldiers_weight << "\n" <<
		enemy_townhalls_weight << "\n" << our_townhalls_weight << "\n" <<
		our_townhall_covering_agents_weight << "\n" << our_soldier_covering_agents_weight << "\n" <<
		our_soldiers_under_soldier_attack_weight << "\n" << our_soldiers_under_cannon_attack_weight << endl;
		myfile.close();
	}
	else	cerr << "file not opened\n";	
	cerr << enemy_soldiers_weight << " " << our_soldiers_weight << " " <<
		enemy_townhalls_weight << " " << our_townhalls_weight << " " <<
		our_townhall_covering_agents_weight << " " << our_soldier_covering_agents_weight << " " <<
		our_soldiers_under_soldier_attack_weight << " " << our_soldiers_under_cannon_attack_weight << endl;
}
// this function modifies the weights such that they sum up to 1 using softmax function
void board::softmax_to_weights()
{
	// denomenator
	float sum = 	exp(enemy_soldiers_weight) + exp(our_soldiers_weight) + exp(enemy_townhalls_weight) + exp(our_townhalls_weight)+
			exp(our_townhall_covering_agents_weight) + exp(our_soldier_covering_agents_weight) +
			exp(our_soldiers_under_soldier_attack_weight) + exp(our_soldiers_under_cannon_attack_weight);
	// modifing each of the weights
	enemy_soldiers_weight = exp(enemy_soldiers_weight) / sum;
	our_soldiers_weight = exp(our_soldiers_weight) / sum;
	enemy_townhalls_weight = exp(enemy_townhalls_weight) / sum;
	our_townhalls_weight = exp(our_townhalls_weight) / sum;
	our_townhall_covering_agents_weight = exp(our_townhall_covering_agents_weight) / sum;
	our_soldier_covering_agents_weight = exp(our_soldier_covering_agents_weight) / sum;
	our_soldiers_under_soldier_attack_weight = exp(our_soldiers_under_soldier_attack_weight) / sum;
	our_soldiers_under_cannon_attack_weight = exp(our_soldiers_under_cannon_attack_weight) / sum;
}
// this function updates the weights according to the event that has occured
void board::update_event_feature_weights(event_type event)
{
	switch(event)
	{
		case I_KILLED_SOLDIER_POS:
			enemy_soldiers_weight += (-0.05);
//			softmax_to_weights();
			break;
		case OPP_KILLED_SOLDIER_POS:
			our_soldiers_weight += (0.01);
//			softmax_to_weights();
			break;
		case I_KILLED_SOLDIER_NEG:
			// future work
			break;
		case OPP_KILLED_SOLDIER_NEG:
			// future work
			break;
		case I_KILLED_TOWNHALL:
			enemy_townhalls_weight += (-3.2);
//			softmax_to_weights();
			break;
		case OPP_KILLED_TOWNHALL:
			our_townhalls_weight += (2.0);
//			softmax_to_weights();
			break;
		case ME_ADDING_TOWNHALL_COVER:
			our_townhall_covering_agents_weight += (0.001);
//			softmax_to_weights();
			break;
		case ME_ADDING_SOLDIER_COVER:
			our_soldier_covering_agents_weight += (0.00008);
//			softmax_to_weights();
			break;
		case ME_UNDER_CANNON_ATTACK:
			our_soldiers_under_cannon_attack_weight += (-0.1);
//			softmax_to_weights();
			break;
		case ME_UNDER_SOLDIER_ATTACK:
			our_soldiers_under_soldier_attack_weight += (-0.02);
//			softmax_to_weights();
			break;
		case NO_EVENT:
			break;
	}
}
// this function updates the following feature values
// our_soldiers_under_soldier_attack
// our_soldiers_under_cannon_attack
// our_soldier_covering_agents
// our_townhall_covering_agents
void board::update_event_feature_values()
{
	our_soldiers_under_soldier_attack=0; our_soldiers_under_cannon_attack=0; our_soldier_covering_agents=0; our_townhall_covering_agents=0;
	vector<soldier> our_soldiers;
	if(my_color == BLACK)	our_soldiers = black_soldiers;
	else			our_soldiers = white_soldiers;
	int our_soldiers_size = our_soldiers.size(), target_r, target_c, board_rows = r, board_cols = c;
	board old_board = *this;
	// traversing from our soldiers
	for(int i=0; i<our_soldiers_size; i++)
	{
		target_r = our_soldiers[i].get_r();
		target_c = our_soldiers[i].get_c();
		// checking in vicinity
		// right
		if(target_c+1 < board_cols)
		{
			cell_content cell_r = old_board.get_cell(target_r, target_c+1);
			switch(cell_r)
			{
				case NONE:
					break;
				case WHITE_SOLDIER:
					if(my_color == WHITE)	our_soldier_covering_agents++;
					else			our_soldiers_under_soldier_attack++;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	our_soldiers_under_soldier_attack++;
					else			our_soldier_covering_agents++;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	our_townhall_covering_agents++;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	our_townhall_covering_agents++;
					break;
			}
		}
		// upper_right
		if(target_c+1 < board_cols and target_r-1>=0 )
		{
			cell_content cell_r = old_board.get_cell(target_r-1, target_c+1);
			switch(cell_r)
			{
				case NONE:
					break;
				case WHITE_SOLDIER:
					if(my_color == WHITE)	our_soldier_covering_agents++;
					else			our_soldiers_under_soldier_attack++;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	our_soldiers_under_soldier_attack++;
					else			our_soldier_covering_agents++;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	our_townhall_covering_agents++;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	our_townhall_covering_agents++;
					break;
			}
		}
		// up
		if(target_r-1>=0 )
		{
			cell_content cell_r = old_board.get_cell(target_r-1, target_c);
			switch(cell_r)
			{
				case NONE:
					break;
				case WHITE_SOLDIER:
					if(my_color == WHITE)	our_soldier_covering_agents++;
					else			our_soldiers_under_soldier_attack++;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	our_soldiers_under_soldier_attack++;
					else			our_soldier_covering_agents++;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	our_townhall_covering_agents++;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	our_townhall_covering_agents++;
					break;
			}
		}
		// upper_left
		if(target_r-1>=0 and target_c-1>=0)
		{
			cell_content cell_r = old_board.get_cell(target_r-1, target_c-1);
			switch(cell_r)
			{
				case NONE:
					break;
				case WHITE_SOLDIER:
					if(my_color == WHITE)	our_soldier_covering_agents++;
					else			our_soldiers_under_soldier_attack++;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	our_soldiers_under_soldier_attack++;
					else			our_soldier_covering_agents++;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	our_townhall_covering_agents++;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	our_townhall_covering_agents++;
					break;
			}
		}
		// left
		if(target_c-1>=0)
		{
			cell_content cell_r = old_board.get_cell(target_r, target_c-1);
			switch(cell_r)
			{
				case NONE:
					break;
				case WHITE_SOLDIER:
					if(my_color == WHITE)	our_soldier_covering_agents++;
					else			our_soldiers_under_soldier_attack++;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	our_soldiers_under_soldier_attack++;
					else			our_soldier_covering_agents++;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	our_townhall_covering_agents++;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	our_townhall_covering_agents++;
					break;
			}
		}
		// bottom_left
		if(target_r+1 < board_rows and target_c-1>=0)
		{
			cell_content cell_r = old_board.get_cell(target_r+1, target_c-1);
			switch(cell_r)
			{
				case NONE:
					break;
				case WHITE_SOLDIER:
					if(my_color == WHITE)	our_soldier_covering_agents++;
					else			our_soldiers_under_soldier_attack++;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	our_soldiers_under_soldier_attack++;
					else			our_soldier_covering_agents++;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	our_townhall_covering_agents++;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	our_townhall_covering_agents++;
					break;
			}
		}
		// bottom
		if(target_r+1 < board_rows)
		{
			cell_content cell_r = old_board.get_cell(target_r+1, target_c);
			switch(cell_r)
			{
				case NONE:
					break;
				case WHITE_SOLDIER:
					if(my_color == WHITE)	our_soldier_covering_agents++;
					else			our_soldiers_under_soldier_attack++;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	our_soldiers_under_soldier_attack++;
					else			our_soldier_covering_agents++;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	our_townhall_covering_agents++;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	our_townhall_covering_agents++;
					break;
			}
		}
		// bottom_right
		if(target_r+1 < board_rows and target_c+1 < board_cols)
		{
			cell_content cell_r = old_board.get_cell(target_r+1, target_c+1);
			switch(cell_r)
			{
				case NONE:
					break;
				case WHITE_SOLDIER:
					if(my_color == WHITE)	our_soldier_covering_agents++;
					else			our_soldiers_under_soldier_attack++;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	our_soldiers_under_soldier_attack++;
					else			our_soldier_covering_agents++;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	our_townhall_covering_agents++;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	our_townhall_covering_agents++;
					break;
			}
		}
		//// me_under_cannon_attack
		// defining the two complete cannons
		// upper_right
		int r_offset2 = 1, r_offset3 = 2, c_offset2 = -1, c_offset3 = -2;
		int cannon1_r = target_r-4, cannon1_c = target_c+4;
		int cannon2_r = target_r-5, cannon2_c = target_c+5;
		// nearer cannon exists
		if(cannon1_r >= 0 and cannon1_c < board_cols)
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// further cannon exists
		if(cannon2_r >= 0 and cannon2_c < board_cols)
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// up
		r_offset2 = 1; r_offset3 = 2; c_offset2 = 0; c_offset3 = 0;
		cannon1_r = target_r-4; cannon1_c = target_c;
		cannon2_r = target_r-5; cannon2_c = target_c;
		// nearer cannon exists
		if(cannon1_r >= 0 and cannon1_c < board_cols)
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// further cannon exists
		if(cannon2_r >= 0 and cannon2_c < board_cols)
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// upper_left
		r_offset2 = 1; r_offset3 = 2; c_offset2 = 1; c_offset3 = 2;
		cannon1_r = target_r-4; cannon1_c = target_c-4;
		cannon2_r = target_r-5; cannon2_c = target_c-5;
		// nearer cannon exists
		if(cannon1_r >= 0 and cannon1_c >= 0)
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// further cannon exists
		if(cannon2_r >= 0 and cannon2_c >= 0 )
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// left
		r_offset2 = 0; r_offset3 = 0; c_offset2 = 1; c_offset3 = 2;
		cannon1_r = target_r; cannon1_c = target_c-4;
		cannon2_r = target_r; cannon2_c = target_c-5;
		// nearer cannon exists
		if(cannon1_r >= 0 and cannon1_c >= 0)
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// further cannon exists
		if(cannon2_r >= 0 and cannon2_c >= 0 )
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// bottom_left
		r_offset2 = -1; r_offset3 = -2; c_offset2 = 1; c_offset3 = 2;
		cannon1_r = target_r+4; cannon1_c = target_c-4;
		cannon2_r = target_r+5; cannon2_c = target_c-5;
		// nearer cannon exists
		if(cannon1_r < board_rows and cannon1_c >= 0)
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// further cannon exists
		if(cannon2_r < board_rows and cannon2_c >= 0 )
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// bottom
		r_offset2 = -1; r_offset3 = -2; c_offset2 = 0; c_offset3 = 0;
		cannon1_r = target_r+4; cannon1_c = target_c;
		cannon2_r = target_r+5; cannon2_c = target_c;
		// nearer cannon exists
		if(cannon1_r < board_rows and cannon1_c >= 0)
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// further cannon exists
		if(cannon2_r < board_rows and cannon2_c >= 0 )
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// bottom_right
		r_offset2 = -1; r_offset3 = -2; c_offset2 = -1; c_offset3 = -2;
		cannon1_r = target_r+4; cannon1_c = target_c+4;
		cannon2_r = target_r+5; cannon2_c = target_c+5;
		// nearer cannon exists
		if(cannon1_r < board_rows and cannon1_c < board_cols)
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// further cannon exists
		if(cannon2_r < board_rows and cannon2_c < board_cols)
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// right
		r_offset2 = 0; r_offset3 = 0; c_offset2 = -1; c_offset3 = -2;
		cannon1_r = target_r; cannon1_c = target_c+4;
		cannon2_r = target_r; cannon2_c = target_c+5;
		// nearer cannon exists
		if(cannon1_r < board_rows and cannon1_c < board_cols)
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
		// further cannon exists
		if(cannon2_r < board_rows and cannon2_c < board_cols)
		{
			if(my_color == WHITE)
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == BLACK_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == BLACK_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == BLACK_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					our_soldiers_under_cannon_attack++;
			}
		}
	}
	our_soldier_covering_agents /= 2;
}
//returns all possible actions for a given board configuration
////////////////////////////////////////
///////////// equivalent to ////////////
//////// get_black_actions  ////////////
////////////////////////////////////////
vector<action> board::get_possible_action()
{
	vector<action> possible_actions;
	// traverse through all black soldiers
	int black_soldiers_size = black_soldiers.size();
	for(int i=0; i<black_soldiers_size; i++)
	{
		bool is_backtracked = false;
		// soldier move
		const int &old_r = black_soldiers[i].get_r();
		const int &old_c = black_soldiers[i].get_c();
		// front moves
		if(old_r != 0)	// front move not possible
		{
			// orthogonal
			switch(grid[old_r-1][old_c])
			{
				case WHITE_SOLDIER:	// or
					if((not is_backtracked) and old_r+2 <= r-1)
					{
						is_backtracked = true;
						// backside left diagonal 
						if(old_c-2>=0)
						{
							switch(grid[old_r+2][old_c-2])
							{
								case NONE:
								case WHITE_SOLDIER:	// can do a capture
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
									break;
							}
						}
						// backside orthogonal
						switch(grid[old_r+2][old_c])
						{
							case NONE:
							case WHITE_SOLDIER:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
								break;
						}
						// backside right diagonal
						if(old_c+2 <= c-1)
						{
							switch(grid[old_r+2][old_c+2])
							{
								case NONE:
								case WHITE_SOLDIER:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
									break;
							}
						}
					}
				case NONE:		// or
				case WHITE_TOWNHALL:
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-1, old_c), SOLDIER_MOVE));
					break;
			}
			// left diagonal
			if(old_c != 0)
			{
				switch(grid[old_r-1][old_c-1])
				{
					case WHITE_SOLDIER:	// or
						if((not is_backtracked) and old_r+2 <= r-1)
						{
							is_backtracked = true;
								// backside left diagonal 
							if(old_c-2>=0)
							{
								switch(grid[old_r+2][old_c-2])
								{
									case NONE:
									case WHITE_SOLDIER:	// can do a capture
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
										break;
								}
							}
							// backside orthogonal
							switch(grid[old_r+2][old_c])
							{
								case NONE:
								case WHITE_SOLDIER:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
									break;
							}
							// backside right diagonal
							if(old_c+2 <= c-1)
							{
								switch(grid[old_r+2][old_c+2])
								{
									case NONE:
									case WHITE_SOLDIER:
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
										break;
								}
							}
						}
					case NONE:		// or
					case WHITE_TOWNHALL:
						possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-1, old_c-1), SOLDIER_MOVE));
						break;
				}
			}
			// right diagonal
			if(old_c != c-1)
			{
				switch(grid[old_r-1][old_c+1])
				{
					case WHITE_SOLDIER:	// or
						if((not is_backtracked) and old_r+2 <= r-1)
						{
							is_backtracked = true;
							// backside left diagonal 
							if(old_c-2>=0)
							{
								switch(grid[old_r+2][old_c-2])
								{
									case NONE:
									case WHITE_SOLDIER:	// can do a capture
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
										break;
								}
							}
							// backside orthogonal
							switch(grid[old_r+2][old_c])
							{
								case NONE:
								case WHITE_SOLDIER:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
									break;
							}
							// backside right diagonal
							if(old_c+2 <= c-1)
							{
								switch(grid[old_r+2][old_c+2])
								{
									case NONE:
									case WHITE_SOLDIER:
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
										break;
								}
							}
						}
					case NONE:		// or
					case WHITE_TOWNHALL:
						possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-1, old_c+1), SOLDIER_MOVE));
						break;
				}
			}
		}
		// side way move and capture
		if(old_c != 0)			// left side check is possible or not
		{
			switch(grid[old_r][old_c-1])
			{
				case WHITE_SOLDIER:
					if((not is_backtracked) and old_r+2 <= r-1)
					{
						is_backtracked = true;
						// backside left diagonal 
						if(old_c-2>=0)
						{
							switch(grid[old_r+2][old_c-2])
							{
								case NONE:
								case WHITE_SOLDIER:	// can do a capture
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
									break;
							}
						}
						// backside orthogonal
						switch(grid[old_r+2][old_c])
						{
							case NONE:
							case WHITE_SOLDIER:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
								break;
						}
						// backside right diagonal
						if(old_c+2 <= c-1)
						{
							switch(grid[old_r+2][old_c+2])
							{
								case NONE:
								case WHITE_SOLDIER:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
									break;
							}
						}
					}
				case WHITE_TOWNHALL:
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-1), SOLDIER_MOVE));
					break;
			}
		}
		if(old_c != c-1)		// right side check is possible or not
		{
			switch(grid[old_r][old_c+1])
			{
				case WHITE_SOLDIER:
					if((not is_backtracked) and old_r+2 <= r-1)
					{
						is_backtracked = true;
						// backside left diagonal 
						if(old_c-2>=0)
						{
							switch(grid[old_r+2][old_c-2])
							{
								case NONE:
								case WHITE_SOLDIER:	// can do a capture
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
									break;
							}
						}
						// backside orthogonal
						switch(grid[old_r+2][old_c])
						{
							case NONE:
							case WHITE_SOLDIER:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
								break;
						}
						// backside right diagonal
						if(old_c+2 <= c-1)
						{
							switch(grid[old_r+2][old_c+2])
							{
								case NONE:
								case WHITE_SOLDIER:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
									break;
							}
						}
					}
				case WHITE_TOWNHALL:
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+1), SOLDIER_MOVE));
					break;	
			}
		}

		/////////////////////////////////
		///////// cannon moves  /////////
		/////////////////////////////////
		/////// verticle direction //////
		/////////////////////////////////
		if(old_r+2 <= r-1)	// can be first element
		{
			// cnannon formation check	
			if(grid[old_r+1][old_c] == BLACK_SOLDIER and grid[old_r+2][old_c] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_r != 0 and grid[old_r-1][old_c] == NONE)
				{
					// qualified for cannon shot
					if(old_r-2>= 0)
					{
						switch(grid[old_r-2][old_c])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), CANNON_SHOT));
								break;
						}
					}
					if(old_r-3>= 0)
					{
						switch(grid[old_r-3][old_c])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c), CANNON_SHOT));
								break;
						}
					}
				}
				//// cannon move check
				if(old_r+3 <= r-1 and grid[old_r+3][old_c] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c), SOLDIER_MOVE));
			}
		}
		if(old_r-2 >= 0)	// can be last element
		{
			// cannon formation check
			if(grid[old_r-1][old_c] == BLACK_SOLDIER and grid[old_r-2][old_c] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_r != r-1 and grid[old_r+1][old_c] == NONE)
				{
					// qualified for cannon shot
					if(old_r+2 <= r-1)
						switch(grid[old_r+2][old_c])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), CANNON_SHOT));
								break;
						}
					if(old_r+3 <= r-1)
						switch(grid[old_r+3][old_c])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r-3 >= 0 and grid[old_r-3][old_c] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c), SOLDIER_MOVE));
			}
		}
		///////////////////////////////
		/////// right diagonal ////////
		///////////////////////////////
		if(old_r+2 <= r-1 and old_c-2 >= 0)	// can be first element
		{
			// cnannon formation check	
			if(grid[old_r+1][old_c-1] == BLACK_SOLDIER and grid[old_r+2][old_c-2] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_r != 0 and old_c != c-1 and grid[old_r-1][old_c+1] == NONE)
				{
					// qualified for cannon shot
					if(old_r-2>= 0 and old_c+2<= c-1)
						switch(grid[old_r-2][old_c+2])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), CANNON_SHOT));
								break;
						}
					if(old_r-3>= 0 and old_c+3<= c-1)
						switch(grid[old_r-3][old_c+3])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c+3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r+3 <= r-1 and old_c-3>= 0 and grid[old_r+3][old_c-3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c-3), SOLDIER_MOVE));
			}
		}
		if(old_r-2 >= 0 and old_c+2<= c-1)	// can be last element
		{
			// cannon formation check
			if(grid[old_r-1][old_c+1] == BLACK_SOLDIER and grid[old_r-2][old_c+2] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_r != r-1 and old_c != 0 and grid[old_r+1][old_c-1] == NONE)
				{
					// qualified for cannon shot
					if(old_r+2 <= r-1 and old_c-2>= 0)
						switch(grid[old_r+2][old_c-2])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), CANNON_SHOT));
								break;
						}
					if(old_r+3 <= r-1 and old_c-3>= 0)
						switch(grid[old_r+3][old_c-3])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c-3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r-3 >= 0 and old_c+3<=c-1 and grid[old_r-3][old_c+3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c+3), SOLDIER_MOVE));
			}
		}
		///////////////////////////////
		/////// left diagonal ////////
		///////////////////////////////
		if(old_r+2 <= r-1 and old_c+2 >= 0)	// can be first element
		{
			// cnannon formation check	
			if(grid[old_r+1][old_c+1] == BLACK_SOLDIER and grid[old_r+2][old_c+2] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_r != 0 and old_c != 0 and grid[old_r-1][old_c-1] == NONE)
				{
					// qualified for cannon shot
					if(old_r-2>= 0 and old_c-2>= 0)
						switch(grid[old_r-2][old_c-2])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), CANNON_SHOT));
								break;
						}
					if(old_r-3>= 0 and old_c-3>= 0)
						switch(grid[old_r-3][old_c-3])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c-3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r+3 <= r-1 and old_c+3<= c-1 and grid[old_r+3][old_c+3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c+3), SOLDIER_MOVE));
			}
		}
		if(old_r-2 >= 0 and old_c-2>= 0)	// can be last element
		{
			// cannon formation check
			if(grid[old_r-1][old_c-1] == BLACK_SOLDIER and grid[old_r-2][old_c-2] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_r != r-1 and old_c != c-1 and grid[old_r+1][old_c+1] == NONE)
				{
					// qualified for cannon shot
					if(old_r+2 <= r-1 and old_c+2<= c-1)
						switch(grid[old_r+2][old_c+2])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), CANNON_SHOT));
								break;
						}
					if(old_r+3 <= r-1 and old_c+3<= c-1)
						switch(grid[old_r+3][old_c+3])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c+3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r-3 >= 0 and old_c-3>= 0 and grid[old_r-3][old_c-3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c-3), SOLDIER_MOVE));
			}
		}
		/////////////////////////////////
		/////// horizontal direction ////
		/////////////////////////////////
		if(old_c-2 >= 0)	// can be first element
		{
			// cnannon formation check	
			if(grid[old_r][old_c-1] == BLACK_SOLDIER and grid[old_r][old_c-2] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_c != c-1 and grid[old_r][old_c+1] == NONE)
				{
					// qualified for cannon shot
					if(old_c+2 <= c-1)
						switch(grid[old_r][old_c+2])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+2), CANNON_SHOT));
								break;
						}
					if(old_c+3 <= c-1)
						switch(grid[old_r][old_c+3])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_c-3 >= 0 and grid[old_r][old_c-3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-3), SOLDIER_MOVE));
			}
		}
		if(old_c+2 <= c-1)	// can be last element
		{
			// cannon formation check
			if(grid[old_r][old_c+1] == BLACK_SOLDIER and grid[old_r][old_c+2] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_c != 0 and grid[old_r][old_c-1] == NONE)
				{
					// qualified for cannon shot
					if(old_c-2 >= 0)
						switch(grid[old_r][old_c-2])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-2), CANNON_SHOT));
								break;
						}
					if(old_c-3 >= 0)
						switch(grid[old_r][old_c-3])
						{
							case NONE:
							case WHITE_SOLDIER:
							case WHITE_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_c+3 <= c-1 and grid[old_r][old_c+3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+3), SOLDIER_MOVE));
			}
		}
	}
	return possible_actions;
}
//returns all possible opponent actions for a given board configuration
////////////////////////////////////////
///////////// equivalent to ////////////
//////// get_white_actions  ////////////
////////////////////////////////////////
vector<action> board::get_opponent_action()
{
	vector<action> possible_actions;
	// traverse through all white soldiers
	int white_soldiers_size = white_soldiers.size();
	for(int i=0; i<white_soldiers_size; i++)
	{
		bool is_backtracked = false;
		// soldier move
		const int &old_r = white_soldiers[i].get_r();
		const int &old_c = white_soldiers[i].get_c();
		// front moves
		if(old_r != c-1)	// front move not possible
		{
			// orthogonal
			switch(grid[old_r+1][old_c])
			{
				case BLACK_SOLDIER:	// or
					if((not is_backtracked) and old_r-2 >= 0)
					{
						is_backtracked = true;
						// backside left diagonal 
						if(old_c-2>=0)
						{
							switch(grid[old_r-2][old_c-2])
							{
								case NONE:
								case BLACK_SOLDIER:	// can do a capture
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
									break;
							}
						}
						// backside orthogonal
						switch(grid[old_r-2][old_c])
						{
							case NONE:
							case BLACK_SOLDIER:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
								break;
						}
						// backside right diagonal
						if(old_c+2 <= c-1)
						{
							switch(grid[old_r-2][old_c+2])
							{
								case NONE:
								case BLACK_SOLDIER:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
									break;
							}
						}
					}
				case NONE:		// or
				case BLACK_TOWNHALL:
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+1, old_c), SOLDIER_MOVE));
					break;
			}
			// left diagonal
			if(old_c != 0)
			{
				switch(grid[old_r+1][old_c-1])
				{
					case BLACK_SOLDIER:	// or
						if((not is_backtracked) and old_r-2 >= 0)
						{
							is_backtracked = true;
							// backside left diagonal 
							if(old_c-2>=0)
							{
								switch(grid[old_r-2][old_c-2])
								{
									case NONE:
									case BLACK_SOLDIER:	// can do a capture
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
										break;
								}
							}
							// backside orthogonal
							switch(grid[old_r-2][old_c])
							{
								case NONE:
								case BLACK_SOLDIER:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
									break;
							}
							// backside right diagonal
							if(old_c+2 <= c-1)
							{
								switch(grid[old_r-2][old_c+2])
								{
									case NONE:
									case BLACK_SOLDIER:
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
										break;
								}
							}
						}
					case NONE:		// or
					case BLACK_TOWNHALL:
						possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+1, old_c-1), SOLDIER_MOVE));
						break;
				}
			}
			// right diagonal
			if(old_c != c-1)
			{
				switch(grid[old_r+1][old_c+1])
				{
					case BLACK_SOLDIER:	// or
						if((not is_backtracked) and old_r-2 >= 0)
						{
							is_backtracked = true;
							// backside left diagonal 
							if(old_c-2>=0)
							{
								switch(grid[old_r-2][old_c-2])
								{
									case NONE:
									case BLACK_SOLDIER:	// can do a capture
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
										break;
								}
							}
							// backside orthogonal
							switch(grid[old_r-2][old_c])
							{
								case NONE:
								case BLACK_SOLDIER:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
									break;
							}
							// backside right diagonal
							if(old_c+2 <= c-1)
							{
								switch(grid[old_r-2][old_c+2])
								{
									case NONE:
									case BLACK_SOLDIER:
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
										break;
								}
							}
						}
					case NONE:		// or
					case BLACK_TOWNHALL:
						possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+1, old_c+1), SOLDIER_MOVE));
						break;
				}
			}
		}
		// side way move and capture
		if(old_c != 0)			// left side check is possible or not
		{
			switch(grid[old_r][old_c-1])
			{
				case BLACK_SOLDIER:
					if((not is_backtracked) and old_r-2 >= 0)
					{
						is_backtracked = true;
						// backside left diagonal 
						if(old_c-2>=0)
						{
							switch(grid[old_r-2][old_c-2])
							{
								case NONE:
								case BLACK_SOLDIER:	// can do a capture
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
									break;
							}
						}
						// backside orthogonal
						switch(grid[old_r-2][old_c])
						{
							case NONE:
							case BLACK_SOLDIER:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
								break;
						}
						// backside right diagonal
						if(old_c+2 <= c-1)
						{
							switch(grid[old_r-2][old_c+2])
							{
								case NONE:
								case BLACK_SOLDIER:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
									break;
							}
						}
					}
				case BLACK_TOWNHALL:
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-1), SOLDIER_MOVE));
					break;
			}
		}
		if(old_c != c-1)		// right side check is possible or not
		{
			switch(grid[old_r][old_c+1])
			{
				case BLACK_SOLDIER:
					if((not is_backtracked) and old_r-2 >= 0)
					{
						is_backtracked = true;
						// backside left diagonal 
						if(old_c-2>=0)
						{
							switch(grid[old_r-2][old_c-2])
							{
								case NONE:
								case BLACK_SOLDIER:	// can do a capture
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
									break;
							}
						}
						// backside orthogonal
						switch(grid[old_r-2][old_c])
						{
							case NONE:
							case BLACK_SOLDIER:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
								break;
						}
						// backside right diagonal
						if(old_c+2 <= c-1)
						{
							switch(grid[old_r-2][old_c+2])
							{
								case NONE:
								case BLACK_SOLDIER:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
									break;
							}
						}
					}
				case BLACK_TOWNHALL:
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+1), SOLDIER_MOVE));
					break;	
			}
		}

		/////////////////////////////////
		///////// cannon moves  /////////
		/////////////////////////////////
		/////// verticle direction //////
		/////////////////////////////////
		if(old_r+2 <= r-1)	// can be last element
		{
			// cnannon formation check	
			if(grid[old_r+1][old_c] == WHITE_SOLDIER and grid[old_r+2][old_c] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_r != 0 and grid[old_r-1][old_c] == NONE)
				{
					// qualified for cannon shot
					if(old_r-2>= 0)
						switch(grid[old_r-2][old_c])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), CANNON_SHOT));
								break;
						}
					if(old_r-3>= 0)
						switch(grid[old_r-3][old_c])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r+3 <= r-1 and grid[old_r+3][old_c] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c), SOLDIER_MOVE));
			}
		}
		if(old_r-2 >= 0)	// can be first element
		{
			// cannon formation check
			if(grid[old_r-1][old_c] == WHITE_SOLDIER and grid[old_r-2][old_c] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_r != r-1 and grid[old_r+1][old_c] == NONE)
				{
					// qualified for cannon shot
					if(old_r+2 <= r-1)
						switch(grid[old_r+2][old_c])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), CANNON_SHOT));
								break;
						}
					if(old_r+3 <= r-1)
						switch(grid[old_r+3][old_c])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r-3 >= 0 and grid[old_r-3][old_c] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c), SOLDIER_MOVE));
			}
		}
		///////////////////////////////
		/////// right diagonal ////////
		///////////////////////////////
		if(old_r+2 <= r-1 and old_c-2 >= 0)	// can be last element
		{
			// cnannon formation check	
			if(grid[old_r+1][old_c-1] == WHITE_SOLDIER and grid[old_r+2][old_c-2] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_r != 0 and old_c != c-1 and grid[old_r-1][old_c+1] == NONE)
				{
					// qualified for cannon shot
					if(old_r-2>= 0 and old_c+2<= c-1)
						switch(grid[old_r-2][old_c+2])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), CANNON_SHOT));
								break;
						}
					if(old_r-3>= 0 and old_c+3<= c-1)
						switch(grid[old_r-3][old_c+3])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c+3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r+3 <= r-1 and old_c-3>= 0 and grid[old_r+3][old_c-3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c-3), SOLDIER_MOVE));
			}
		}
		if(old_r-2 >= 0 and old_c+2<= c-1)	// can be first element
		{
			// cannon formation check
			if(grid[old_r-1][old_c+1] == WHITE_SOLDIER and grid[old_r-2][old_c+2] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_r != r-1 and old_c != 0 and grid[old_r+1][old_c-1] == NONE)
				{
					// qualified for cannon shot
					if(old_r+2 <= r-1 and old_c-2>= 0)
						switch(grid[old_r+2][old_c-2])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), CANNON_SHOT));
								break;
						}
					if(old_r+3 <= r-1 and old_c-3>= 0)
						switch(grid[old_r+3][old_c-3])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c-3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r-3 >= 0 and old_c+3<=c-1 and grid[old_r-3][old_c+3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c+3), SOLDIER_MOVE));
			}
		}
		///////////////////////////////
		/////// left diagonal ////////
		///////////////////////////////
		if(old_r+2 <= r-1 and old_c+2 >= 0)	// can be last element
		{
			// cnannon formation check	
			if(grid[old_r+1][old_c+1] == WHITE_SOLDIER and grid[old_r+2][old_c+2] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_r != 0 and old_c != 0 and grid[old_r-1][old_c-1] == NONE)
				{
					// qualified for cannon shot
					if(old_r-2>= 0 and old_c-2>= 0)
						switch(grid[old_r-2][old_c-2])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), CANNON_SHOT));
								break;
						}
					if(old_r-3>= 0 and old_c-3>= 0)
						switch(grid[old_r-3][old_c-3])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c-3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r+3 <= r-1 and old_c+3<= c-1 and grid[old_r+3][old_c+3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c+3), SOLDIER_MOVE));
			}
		}
		if(old_r-2 >= 0 and old_c-2>= 0)	// can be FIRST element
		{
			// cannon formation check
			if(grid[old_r-1][old_c-1] == WHITE_SOLDIER and grid[old_r-2][old_c-2] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_r != r-1 and old_c != c-1 and grid[old_r+1][old_c+1] == NONE)
				{
					// qualified for cannon shot
					if(old_r+2 <= r-1 and old_c+2<= c-1)
						switch(grid[old_r+2][old_c+2])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), CANNON_SHOT));
								break;
						}
					if(old_r+3 <= r-1 and old_c+3<= c-1)
						switch(grid[old_r+3][old_c+3])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c+3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r-3 >= 0 and old_c-3>= 0 and grid[old_r-3][old_c-3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c-3), SOLDIER_MOVE));
			}
		}
		/////////////////////////////////
		/////// horizontal direction ////
		/////////////////////////////////
		if(old_c-2 >= 0)	// can be first element
		{
			// cnannon formation check	
			if(grid[old_r][old_c-1] == WHITE_SOLDIER and grid[old_r][old_c-2] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_c != c-1 and grid[old_r][old_c+1] == NONE)
				{
					// qualified for cannon shot
					if(old_c+2 <= c-1)
						switch(grid[old_r][old_c+2])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+2), CANNON_SHOT));
								break;
						}
					if(old_c+3 <= c-1)
						switch(grid[old_r][old_c+3])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_c-3 >= 0 and grid[old_r][old_c-3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-3), SOLDIER_MOVE));
			}
		}
		if(old_c+2 <= c-1)	// can be last element
		{
			// cannon formation check
			if(grid[old_r][old_c+1] == WHITE_SOLDIER and grid[old_r][old_c+2] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				//// cannon shot check
				if(old_c != 0 and grid[old_r][old_c-1] == NONE)
				{
					// qualified for cannon shot
					if(old_c-2 >= 0)
						switch(grid[old_r][old_c-2])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-2), CANNON_SHOT));
								break;
						}
					if(old_c-3 >= 0)
						switch(grid[old_r][old_c-3])
						{
							case NONE:
							case BLACK_SOLDIER:
							case BLACK_TOWNHALL:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_c+3 <= c-1 and grid[old_r][old_c+3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+3), SOLDIER_MOVE));
			}
		}
	}
	return possible_actions;
}

//  terminal test returns 	true if number of townhalls of either team is 2
//	  			false if number of townhalls of both teams are greater than 2
//  clearly this function does not considers the fact that if there are no moves possible
//  for either team, the state is a leaf node. Incorporating this fact will increase the
//  time complexity bcz then we will need to see the possible moves of the player.
//  BUT we can incorporate this later in the code when we will identify the possible 
//  moves, so if no move possible then stop and return it's utility function.
bool board::terminal_test(int depth, const int &cut_off_depth)
{
	if(depth >= cut_off_depth)	{ return true;}
	if(white_townhalls.size() <= c/4 or black_townhalls.size() <= c/4)	return true;
	return false;
}
float board::dummy_utility_function()
{
	return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}
// new utility function for assn5 just implements the table given in the assignment in proper order
// do_i_have_moves == True : I have moves, opponent does not have moves
// do_i_have_moves == False: I don't have moves, opponent has moves
float board::new_utility_function(bool do_i_have_moves)
{
	if(my_color == BLACK)
	{
		float score = (black_soldiers.size() * 1.0)/100;
		switch(black_townhalls.size())	// my townhalls
		{
			case 4:
				if(white_townhalls.size() == 2) // opponent townhalls
				{
					score += 10;
					return score*5000;
				}
				break;
			case 3:
				if(white_townhalls.size() == 2)
				{
					score += 8;
					return score*5000;
				}
				break;
			case 2:
				if(white_townhalls.size() == 3)
				{
					score += 2;
					return score*5000;
				}
				else if(white_townhalls.size() == 4)
				{
					score += 0;
					return score*5000;
				}
				break;
		}
		//////////////////// 	Stalemate  ///////////////////
		////////////////////  condition 1  ////////////////////
		// opponent has no soldiers left
		if(white_soldiers.size() == 0)
		{
			// I am A
			// opponent is B
			switch(black_townhalls.size())
			{
				case 4:
					switch(white_townhalls.size())
					{
						case 4:
							score += 8;
							return score;
						case 3:
							score += 10;
							return score;
					}
					break;
				case 3:
					switch(white_townhalls.size())
					{
						case 4:
							score += 6;
							return score;
						case 3:
							score += 8;
							return score;
					}
					break;
			}
		}
		// I have no soldiers left
		else if(black_soldiers.size() == 0)
		{
			// I am B
			// opponent is A
			switch(black_townhalls.size())
			{
				case 4:
					switch(white_townhalls.size())
					{
						case 4:
							score += 2;
							return score;
						case 3:
							score += 0;
							return score;
					}
					break;
				case 3:
					switch(white_townhalls.size())
					{
						case 4:
							score += 4;
							return score;
						case 3:
							score += 2;
							return score;
					}
					break;
			}
		}
		//////////////////  condtion 2  ///////////////////////
		// opponent does not have moves
		if(do_i_have_moves)
		{
			// I am A
			// opponent is B
			switch(black_townhalls.size())
			{
				case 4:
					switch(white_townhalls.size())
					{
						case 4:
							score += 6;
							return score;
						case 3:
							score += 8;
							return score;
					}
					break;
				case 3:
					switch(white_townhalls.size())
					{
						case 4:
							score += 4;
							return score;
						case 3:
							score += 6;
							return score;
					}
					break;
			}
		}
		// I don't have moves
		else
		{
			// I am B
			// opponent is A
			switch(black_townhalls.size())
			{
				case 4:
					switch(white_townhalls.size())
					{
						case 4:
							score += 4;
							return score;
						case 3:
							score += 2;
							return score;
					}
					break;
				case 3:
					switch(white_townhalls.size())
					{
						case 4:
							score += 6;
							return score;
						case 3:
							score += 4;
							return score;
					}
					break;
			}
		}
	}
	else
	{
		float score = (white_soldiers.size() * 1.0)/100;
		switch(white_townhalls.size())	// my townhalls
		{
			case 4:
				if(black_townhalls.size() == 2) // opponent townhalls
				{
						score += 10;
						return score*5000;
				}
				break;
			case 3:
				if(black_townhalls.size() == 2)
				{
						score += 8;
						return score*5000;
				}
				break;
			case 2:
				if(black_townhalls.size() == 3)
				{
						score += 2;
						return score*5000;
				}
				else if(black_townhalls.size() == 4)
				{
						score += 0;
						return score*5000;
				}
				break;
		}
		//////////////////   Stalemate 	 ////////////////
		/////////////////   condition 1	////////////////
		// opponent has no soldiers left
		if(black_soldiers.size() == 0)
		{
			// I am A
			// opponent is B
			switch(white_townhalls.size())
			{
				case 4:
					switch(black_townhalls.size())
					{
						case 4:
							score += 8;
							break;
						case 3:
							score += 10;
							break;
					}
					break;
				case 3:
					switch(black_townhalls.size())
					{
						case 4:
							score += 6;
							break;
						case 3:
							score += 8;
							break;
					}
					break;
			}
		}
		// I have no soldiers left
		else if(white_soldiers.size() == 0)
		{
			// I am B
			// opponent is A
			switch(white_townhalls.size())
			{
				case 4:
					switch(black_townhalls.size())
					{
						case 4:
							score += 2;
							break;
						case 3:
							score += 0;
							break;
					}
					break;
				case 3:
					switch(black_townhalls.size())
					{
						case 4:
							score += 4;
							break;
						case 3:
							score += 2;
							break;
					}
					break;
			}
		}
		//////////////////  condtion 2  ///////////////////////
		// opponent does not have moves
		if(do_i_have_moves)
		{
			// I am A
			// opponent is B
			switch(white_townhalls.size())
			{
				case 4:
					switch(black_townhalls.size())
					{
						case 4:
							score += 6;
							return score;
						case 3:
							score += 8;
							return score;
					}
					break;
				case 3:
					switch(black_townhalls.size())
					{
						case 4:
							score += 4;
							return score;
						case 3:
							score += 6;
							return score;
					}
					break;
			}
		}
		// I don't have moves
		else
		{
			// I am B
			// opponent is A
			switch(white_townhalls.size())
			{
				case 4:
					switch(black_townhalls.size())
					{
						case 4:
							score += 4;
							return score;
						case 3:
							score += 2;
							return score;
					}
					break;
				case 3:
					switch(black_townhalls.size())
					{
						case 4:
							score += 6;
							return score;
						case 3:
							score += 4;
							return score;
					}
					break;
			}
		}
	}
}
// utility function just implements the table given in the assignment in proper order
float board::utility_function(bool do_i_have_moves)
{
	if(my_color == BLACK)
	{
		float score = (black_soldiers.size() * 1.0)/100;
		switch(black_townhalls.size())	// my townhalls
		{
			case 4:
				switch(white_townhalls.size()) // opponent townhalls
				{
					case 2:
						score += 10;
						break;
					case 3:
						if(do_i_have_moves)	if(white_soldiers.size()==0)	score += 10;
									else				score += 8;
						else			if(black_soldiers.size()==0)	score += 0;
									else				score += 2;
						break;
					case 4:
						// I am A.
						if(do_i_have_moves)	if(white_soldiers.size()==0)	score += 8;
									else				score += 6;
						// I am B.
						else			if(black_soldiers.size()==0)	score += 2;
									else				score += 4;				
						break;
				}
				break;
			case 3:
				switch(white_townhalls.size())
				{
					case 2:
						score += 8;
						break;
					case 3:
						// I am A.
						if(do_i_have_moves)	if(white_soldiers.size()==0)	score += 8;
									else				score += 6;
						// I am B.
						else			if(black_soldiers.size()==0)	score += 2;
									else				score += 4;
						break;
					case 4:
						// I am A.
						if(do_i_have_moves)	if(white_soldiers.size()==0)	score += 6;
									else				score += 4;
						// I am B.
						else			if(black_soldiers.size()==0)	score += 4;
									else				score += 6;
						break;
				}
				break;
			case 2:
				switch(white_townhalls.size())
				{
					case 3:
						score += 2;
						break;
					case 4:
						score += 0;
						break;
				}
				break;
		}
		return score*5000;
	}
	else
	{
		float score = (white_soldiers.size() * 1.0)/100;
		switch(white_townhalls.size())	// my townhalls
		{
			case 4:
				switch(black_townhalls.size()) // opponent townhalls
				{
					case 2:
						score += 10;
						break;
					case 3:
						if(do_i_have_moves)	if(black_soldiers.size()==0)	score += 10;
									else				score += 8;
						else			if(white_soldiers.size()==0)	score += 0;
									else				score += 2;
						break;
					case 4:
						// I am A.
						if(do_i_have_moves)	if(black_soldiers.size()==0)	score += 8;
									else				score += 6;
						// I am B.
						else			if(white_soldiers.size()==0)	score += 2;
									else				score += 4;				
						break;
				}
				break;
			case 3:
				switch(black_townhalls.size())
				{
					case 2:
						score += 8;
						break;
					case 3:
						// I am A.
						if(do_i_have_moves)	if(black_soldiers.size()==0)	score += 8;
									else				score += 6;
						// I am B.
						else			if(white_soldiers.size()==0)	score += 2;
									else				score += 4;
						break;
					case 4:
						// I am A.
						if(do_i_have_moves)	if(black_soldiers.size()==0)	score += 6;
									else				score += 4;
						// I am B.
						else			if(white_soldiers.size()==0)	score += 4;
									else				score += 6;
						break;
				}
				break;
			case 2:
				switch(black_townhalls.size())
				{
					case 3:
						score += 2;
						break;
					case 4:
						score += 0;
						break;
				}
				break;
		}
		return score*5000;
	}
//	else
//	{
//		float score = (white_soldiers.size() * 1.0)/100;
//		switch(white_townhalls.size())	// my townhalls
//		{
//			case 4:
//				switch(black_townhalls.size()) // opponent townhalls
//				{
//					case 3:
//						score += 7;
//						break;
//					case 4:
//						score += 5;
//						break;
//				}
//				break;
//			case 3:
//				switch(black_townhalls.size())
//				{
//					case 3:
//						score += 5;
//						break;
//					case 4:
//						score += 3;
//						break;
//				}
//		}
//		return score;
//	}
	
}

float board::dynamic_eval_function()
{
	// for now imposing the constraint that don't use utility function. Incorporate utility function when normalization is done.
	float score=0;
	int our_soldiers, enemy_soldiers, our_townhalls, enemy_townhalls;
	    // newly added variables

	int soldiers_i_can_kill, soldiers_enemy_can_kill, townhalls_i_can_kill, townhalls_enemy_can_kill;
	
	// modifing newely added variables
	if(my_color == BLACK)	update_black_feature_values();
	else			update_white_feature_values();
	// our things are ready
	soldiers_i_can_kill = (soldier_kills_diff);
	townhalls_i_can_kill = townhall_kills_diff;
	// modifing enemy things
	if(my_color == BLACK)	update_white_feature_values();
	else 			update_black_feature_values();
	soldiers_enemy_can_kill = soldiers_i_can_kill-soldier_kills_diff;
	townhalls_enemy_can_kill = townhalls_i_can_kill-townhall_kills_diff;


	if(my_color==BLACK)	
	{ 
		our_soldiers = black_soldiers.size(); 
		our_townhalls = black_townhalls.size();
		enemy_soldiers = white_soldiers.size();
		enemy_townhalls = white_townhalls.size();
	}
	else			
	{ 
		enemy_soldiers = black_soldiers.size(); 
		enemy_townhalls = black_townhalls.size();
		our_soldiers = white_soldiers.size();
		our_townhalls = white_townhalls.size();
	}
	// update the event feature values for the given state
	update_event_feature_values();
	score = our_soldiers_weight * our_soldiers +
		enemy_soldiers_weight * enemy_soldiers+
		our_townhalls_weight * our_townhalls+
		enemy_townhalls_weight * enemy_townhalls+
		our_soldiers_under_soldier_attack_weight * our_soldiers_under_soldier_attack+
		our_soldiers_under_cannon_attack_weight * our_soldiers_under_cannon_attack + 
		our_townhall_covering_agents_weight * our_townhall_covering_agents+
		our_soldier_covering_agents_weight * our_soldier_covering_agents+
	//	0.2 * soldiers_i_can_kill+
		-20 * (enemy_townhalls - our_townhalls);
		 //* soldiers_enemy_can_kill+
	//	100 * townhalls_i_can_kill+
	//	(-100) * townhalls_enemy_can_kill;

		// newly added weights
//		soldiers_i_can_kill_weight * soldiers_i_can_kill + 
//		soldiers_enemy_can_kill_weight * soldiers_enemy_can_kill + 
//		townhalls_i_can_kill_weight * townhalls_i_can_kill + 
//		townhalls_enemy_can_kill_weight * townhalls_enemy_can_kill;
	return score;
}

float board::eval_function()
{
	
	if(white_townhalls.size() <= c/4 or black_townhalls.size() <= c/4)	return utility_function(true);	// value does not matter
	
	// features
	// # of soldiers and townhalls
	// # of horizontal, vertical, left and right diagonal oriented cannons
	// # of possible moves
	// # of kills possible in the next step
	int 	soldier_diff=0, townhall_diff=0, 
	    	our_horizontal_cannons=0, enemy_horizontal_cannons=0,
		our_verticle_cannons=0, enemy_verticle_cannons=0,
		our_right_cannons=0, enemy_right_cannons=0,
		our_left_cannons=0, enemy_left_cannons=0,
		soldiers_i_can_kill=0, soldiers_enemy_can_kill=0,
		townhalls_i_can_kill=0, townhalls_enemy_can_kill=0,
		our_soldiers=0, enemy_soldiers=0;
	vector<action> our_moves, opp_moves;
	// modifing our things
	if(my_color == BLACK)	our_moves = update_black_feature_values();
	else			our_moves = update_white_feature_values();
	
	// our things are ready
	our_horizontal_cannons = (horizontal_cannons_diff)/2;
	our_verticle_cannons = (verticle_cannons_diff)/2;
	our_right_cannons = (right_diagonal_cannons_diff)/2;
	our_left_cannons = (left_diagonal_cannons_diff)/2;
	soldiers_i_can_kill = (soldier_kills_diff);
	townhalls_i_can_kill = townhall_kills_diff;

	if(my_color==BLACK)	
	{ 
		soldier_diff = black_soldiers.size() - white_soldiers.size(); 
		townhall_diff = black_townhalls.size()-white_townhalls.size();
		our_soldiers = black_soldiers.size();
		enemy_soldiers = white_soldiers.size();
	}
	else			
	{ 
		soldier_diff = white_soldiers.size() - black_soldiers.size(); 
		townhall_diff = white_townhalls.size()-black_townhalls.size();
		our_soldiers = white_soldiers.size();
		enemy_soldiers = black_soldiers.size();
	}

	// modifing enemy things
	if(my_color == BLACK)	opp_moves = update_white_feature_values();
	else 			opp_moves = update_black_feature_values();
	
	enemy_horizontal_cannons = our_horizontal_cannons - horizontal_cannons_diff/2;
	enemy_verticle_cannons = our_verticle_cannons - verticle_cannons_diff/2;
	enemy_right_cannons = our_right_cannons-right_diagonal_cannons_diff/2;
	enemy_left_cannons = our_left_cannons-left_diagonal_cannons_diff/2;
	soldiers_enemy_can_kill = soldiers_i_can_kill-soldier_kills_diff;
	townhalls_enemy_can_kill = townhalls_i_can_kill-townhall_kills_diff;
	
	float ans = 	((9 * our_soldiers) / 12) +
			((-10 * enemy_soldiers) / 12) +

			((0 * soldier_diff) / 12) +
			((100 * townhall_diff) / 2) +

			((1 * our_horizontal_cannons) / 4) +
			(((0) * enemy_horizontal_cannons) / 4) +

			((3 * our_verticle_cannons) / 4) +
			(((0) * enemy_verticle_cannons) / 4) +

			((3.5 * our_right_cannons) / 4) +
			(((0) * enemy_right_cannons) / 4) +

			((3.5 * our_left_cannons) / 4) +
			(((0) * enemy_left_cannons) / 4) +

			((1 * soldiers_i_can_kill) / 12) +
			(((-1) * soldiers_enemy_can_kill) / 12) +

			((50 * townhalls_i_can_kill) / 4) +
			(((-90) * townhalls_enemy_can_kill) / 4) +

			((6 * our_moves.size()) / 40) +
			(((0) * opp_moves.size()) / 40);
	return ans;
}

// updates all feature variables and returns all possible moves for black team
////////////////////////////////////////
///////////// equivalent to ////////////
//////// get_black_actions  ////////////
////////////////////////////////////////
// additionally targeting the following variables:
// our_soldier_under_soldier_attack	: inferred from opp moves	related variable: soldier_kills_diff
// our_soldier_under_cannon_attack	: inferred from opp moves	related variable: soldier_kills_diff
// our_townhall_covering_agents		: inferred from our moves
// our_soldier_covering_agents		: inferred from our moves
vector<action> board::update_black_feature_values()
{
	vector<action> possible_actions;
	// traverse through all black soldiers
	int black_soldiers_size = black_soldiers.size();
	for(int i=0; i<black_soldiers_size; i++)
	{
		bool is_backtracked = false;
		// soldier move
		const int &old_r = black_soldiers[i].get_r();
		const int &old_c = black_soldiers[i].get_c();
		// front moves
		if(old_r != 0)	// front move not possible
		{
			// orthogonal
			switch(grid[old_r-1][old_c])
			{
				case WHITE_SOLDIER:	// or
					
					if(my_color == BLACK)	soldier_kills_diff++;	// my move
					else			soldier_kills_diff--;	// opponent move

					if((not is_backtracked) and old_r+2 <= r-1)
					{
						is_backtracked = true;
						// backside left diagonal 
						if(old_c-2>=0)
						{
							switch(grid[old_r+2][old_c-2])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
									break;
								case WHITE_SOLDIER:	// can do a capture
									if(my_color == BLACK)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
									break;
							}
						}
						// backside orthogonal
						switch(grid[old_r+2][old_c])
						{
							case NONE:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
								break;
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
								break;
						}
						// backside right diagonal
						if(old_c+2 <= c-1)
						{
							switch(grid[old_r+2][old_c+2])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
									break;
								case WHITE_SOLDIER:
									if(my_color == BLACK)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
									break;
							}
						}
					}
				case NONE:		// or
				case WHITE_TOWNHALL:
					if(grid[old_r-1][old_c] == WHITE_TOWNHALL)
					if(my_color == BLACK )	townhall_kills_diff++;	// my move
					else			townhall_kills_diff--;	// opponent move
					
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-1, old_c), SOLDIER_MOVE));
					break;
			}
			// left diagonal
			if(old_c != 0)
			{
				switch(grid[old_r-1][old_c-1])
				{
					case WHITE_SOLDIER:	// or
						if(my_color == BLACK)	soldier_kills_diff++;	// my move
						else			soldier_kills_diff--;	// opponent move

						if((not is_backtracked) and old_r+2 <= r-1)
						{
							is_backtracked = true;
								// backside left diagonal 
							if(old_c-2>=0)
							{
								switch(grid[old_r+2][old_c-2])
								{
									case NONE:
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
										break;
									case WHITE_SOLDIER:	// can do a capture
										if(my_color == BLACK)	soldier_kills_diff++;	// my move
										else			soldier_kills_diff--;	// opponent move
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
										break;
								}
							}
							// backside orthogonal
							switch(grid[old_r+2][old_c])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
									break;
								case WHITE_SOLDIER:
									if(my_color == BLACK)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
									break;
							}
							// backside right diagonal
							if(old_c+2 <= c-1)
							{
								switch(grid[old_r+2][old_c+2])
								{
									case NONE:
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
										break;
									case WHITE_SOLDIER:
										if(my_color == BLACK)	soldier_kills_diff++;	// my move
										else			soldier_kills_diff--;	// opponent move
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
										break;
								}
							}
						}
					case NONE:		// or
					case WHITE_TOWNHALL:
						if(grid[old_r-1][old_c-1] == WHITE_TOWNHALL)
						if(my_color == BLACK)	townhall_kills_diff++;	// my move
						else			townhall_kills_diff--;	// opponent move
		
						possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-1, old_c-1), SOLDIER_MOVE));
						break;
				}
			}
			// right diagonal
			if(old_c != c-1)
			{
				switch(grid[old_r-1][old_c+1])
				{
					case WHITE_SOLDIER:	// or
						if(my_color == BLACK)	soldier_kills_diff++;	// my move
						else			soldier_kills_diff--;	// opponent move

						if((not is_backtracked) and old_r+2 <= r-1)
						{
							is_backtracked = true;
							// backside left diagonal 
							if(old_c-2>=0)
							{
								switch(grid[old_r+2][old_c-2])
								{
									case NONE:
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
										break;
									case WHITE_SOLDIER:	// can do a capture
										if(my_color == BLACK)	soldier_kills_diff++;	// my move
										else			soldier_kills_diff--;	// opponent move
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
										break;
								}
							}
							// backside orthogonal
							switch(grid[old_r+2][old_c])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
									break;
								case WHITE_SOLDIER:
									if(my_color == BLACK)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
									break;
							}
							// backside right diagonal
							if(old_c+2 <= c-1)
							{
								switch(grid[old_r+2][old_c+2])
								{
									case NONE:
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
										break;
									case WHITE_SOLDIER:
										if(my_color == BLACK)	soldier_kills_diff++;	// my move
										else			soldier_kills_diff--;	// opponent move
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
										break;
								}
							}
						}
					case NONE:		// or
					case WHITE_TOWNHALL:
						if(grid[old_r-1][old_c+1] == WHITE_TOWNHALL)
						if(my_color == BLACK)	townhall_kills_diff++;	// my move
						else			townhall_kills_diff--;	// opponent move

						possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-1, old_c+1), SOLDIER_MOVE));
						break;
				}
			}
		}
		// side way move and capture
		if(old_c != 0)			// left side check is possible or not
		{
			switch(grid[old_r][old_c-1])
			{
				case WHITE_SOLDIER:
					if(my_color == BLACK)	soldier_kills_diff++;	// my move
					else			soldier_kills_diff--;	// opponent move
					
					if((not is_backtracked) and old_r+2 <= r-1)
					{
						is_backtracked = true;
						// backside left diagonal 
						if(old_c-2>=0)
						{
							switch(grid[old_r+2][old_c-2])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
									break;
								case WHITE_SOLDIER:	// can do a capture
									if(my_color == BLACK)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
									break;
							}
						}
						// backside orthogonal
						switch(grid[old_r+2][old_c])
						{
							case NONE:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
								break;
							case WHITE_SOLDIER:
									if(my_color == BLACK)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
								break;
						}
						// backside right diagonal
						if(old_c+2 <= c-1)
						{
							switch(grid[old_r+2][old_c+2])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
									break;
								case WHITE_SOLDIER:
									if(my_color == BLACK)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
									break;
							}
						}
					}
				case WHITE_TOWNHALL:
					if(grid[old_r][old_c-1] == WHITE_TOWNHALL)
					if(my_color == BLACK)	townhall_kills_diff++;	// my move
					else			townhall_kills_diff--;	// opponent move

					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-1), SOLDIER_MOVE));
					break;
			}
		}
		if(old_c != c-1)		// right side check is possible or not
		{
			switch(grid[old_r][old_c+1])
			{
				case WHITE_SOLDIER:
					if(my_color == BLACK)	soldier_kills_diff++;	// my move
					else			soldier_kills_diff--;	// opponent move

					if((not is_backtracked) and old_r+2 <= r-1)
					{
						is_backtracked = true;
						// backside left diagonal 
						if(old_c-2>=0)
						{
							switch(grid[old_r+2][old_c-2])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
									break;
								case WHITE_SOLDIER:	// can do a capture
									if(my_color == BLACK)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), SOLDIER_MOVE));
									break;
							}
						}
						// backside orthogonal
						switch(grid[old_r+2][old_c])
						{
							case NONE:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
								break;
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), SOLDIER_MOVE));
								break;
						}
						// backside right diagonal
						if(old_c+2 <= c-1)
						{
							switch(grid[old_r+2][old_c+2])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
									break;
								case WHITE_SOLDIER:
									if(my_color == BLACK)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), SOLDIER_MOVE));
									break;
							}
						}
					}
				case WHITE_TOWNHALL:
					if(grid[old_r][old_c+1] == WHITE_TOWNHALL)
					if(my_color == BLACK)	townhall_kills_diff++;	// my move
					else			townhall_kills_diff--;	// opponent move

					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+1), SOLDIER_MOVE));
					break;	
			}
		}

		/////////////////////////////////
		///////// cannon moves  /////////
		/////////////////////////////////
		/////// verticle direction //////
		/////////////////////////////////
		if(old_r+2 <= r-1)	// can be first element
		{
			// cnannon formation check	
			if(grid[old_r+1][old_c] == BLACK_SOLDIER and grid[old_r+2][old_c] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == BLACK)	verticle_cannons_diff++;
				else			verticle_cannons_diff--;
				//// cannon shot check
				if(old_r != 0 and grid[old_r-1][old_c] == NONE)
				{
					// qualified for cannon shot
					if(old_r-2>= 0)
					{
						switch(grid[old_r-2][old_c])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r-2][old_c] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), CANNON_SHOT));
								break;
						}
					}
					if(old_r-3>= 0)
					{
						switch(grid[old_r-3][old_c])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r-3][old_c] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c), CANNON_SHOT));
								break;
						}
					}
				}
				//// cannon move check
				if(old_r+3 <= r-1 and grid[old_r+3][old_c] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c), SOLDIER_MOVE));
			}
		}
		if(old_r-2 >= 0)	// can be last element
		{
			// cannon formation check
			if(grid[old_r-1][old_c] == BLACK_SOLDIER and grid[old_r-2][old_c] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == BLACK)	verticle_cannons_diff++;
				else			verticle_cannons_diff--;
				//// cannon shot check
				if(old_r != r-1 and grid[old_r+1][old_c] == NONE)
				{
					// qualified for cannon shot
					if(old_r+2 <= r-1)
						switch(grid[old_r+2][old_c])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r+2][old_c] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), CANNON_SHOT));
								break;
						}
					if(old_r+3 <= r-1)
						switch(grid[old_r+3][old_c])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r+2][old_c] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r-3 >= 0 and grid[old_r-3][old_c] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c), SOLDIER_MOVE));
			}
		}
		///////////////////////////////
		/////// right diagonal ////////
		///////////////////////////////
		if(old_r+2 <= r-1 and old_c-2 >= 0)	// can be first element
		{
			// cnannon formation check	
			if(grid[old_r+1][old_c-1] == BLACK_SOLDIER and grid[old_r+2][old_c-2] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == BLACK)	right_diagonal_cannons_diff++;
				else			right_diagonal_cannons_diff--;
				//// cannon shot check
				if(old_r != 0 and old_c != c-1 and grid[old_r-1][old_c+1] == NONE)
				{
					// qualified for cannon shot
					if(old_r-2>= 0 and old_c+2<= c-1)
						switch(grid[old_r-2][old_c+2])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r-2][old_c+2] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), CANNON_SHOT));
								break;
						}
					if(old_r-3>= 0 and old_c+3<= c-1)
						switch(grid[old_r-3][old_c+3])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r-3][old_c+3] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c+3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r+3 <= r-1 and old_c-3>= 0 and grid[old_r+3][old_c-3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c-3), SOLDIER_MOVE));
			}
		}
		if(old_r-2 >= 0 and old_c+2<= c-1)	// can be last element
		{
			// cannon formation check
			if(grid[old_r-1][old_c+1] == BLACK_SOLDIER and grid[old_r-2][old_c+2] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == BLACK)	right_diagonal_cannons_diff++;
				else			right_diagonal_cannons_diff--;
				//// cannon shot check
				if(old_r != r-1 and old_c != 0 and grid[old_r+1][old_c-1] == NONE)
				{
					// qualified for cannon shot
					if(old_r+2 <= r-1 and old_c-2>= 0)
						switch(grid[old_r+2][old_c-2])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r+2][old_c-2] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), CANNON_SHOT));
								break;
						}
					if(old_r+3 <= r-1 and old_c-3>= 0)
						switch(grid[old_r+3][old_c-3])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r+3][old_c-3] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c-3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r-3 >= 0 and old_c+3<=c-1 and grid[old_r-3][old_c+3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c+3), SOLDIER_MOVE));
			}
		}
		///////////////////////////////
		/////// left diagonal ////////
		///////////////////////////////
		if(old_r+2 <= r-1 and old_c+2 >= 0)	// can be first element
		{
			// cnannon formation check	
			if(grid[old_r+1][old_c+1] == BLACK_SOLDIER and grid[old_r+2][old_c+2] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == BLACK)	left_diagonal_cannons_diff++;
				else			left_diagonal_cannons_diff--;
				//// cannon shot check
				if(old_r != 0 and old_c != 0 and grid[old_r-1][old_c-1] == NONE)
				{
					// qualified for cannon shot
					if(old_r-2>= 0 and old_c-2>= 0)
						switch(grid[old_r-2][old_c-2])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r-2][old_c-2] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), CANNON_SHOT));
								break;
						}
					if(old_r-3>= 0 and old_c-3>= 0)
						switch(grid[old_r-3][old_c-3])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r-3][old_c-3] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c-3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r+3 <= r-1 and old_c+3<= c-1 and grid[old_r+3][old_c+3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c+3), SOLDIER_MOVE));
			}
		}
		if(old_r-2 >= 0 and old_c-2>= 0)	// can be last element
		{
			// cannon formation check
			if(grid[old_r-1][old_c-1] == BLACK_SOLDIER and grid[old_r-2][old_c-2] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == BLACK)	left_diagonal_cannons_diff++;
				else			left_diagonal_cannons_diff--;
				//// cannon shot check
				if(old_r != r-1 and old_c != c-1 and grid[old_r+1][old_c+1] == NONE)
				{
					// qualified for cannon shot
					if(old_r+2 <= r-1 and old_c+2<= c-1)
						switch(grid[old_r+2][old_c+2])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r+2][old_c+2] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), CANNON_SHOT));
								break;
						}
					if(old_r+3 <= r-1 and old_c+3<= c-1)
						switch(grid[old_r+3][old_c+3])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r+3][old_c+3] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c+3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r-3 >= 0 and old_c-3>= 0 and grid[old_r-3][old_c-3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c-3), SOLDIER_MOVE));
			}
		}
		/////////////////////////////////
		/////// horizontal direction ////
		/////////////////////////////////
		if(old_c-2 >= 0)	// can be first element
		{
			// cnannon formation check	
			if(grid[old_r][old_c-1] == BLACK_SOLDIER and grid[old_r][old_c-2] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == BLACK)	horizontal_cannons_diff++;
				else			horizontal_cannons_diff--;
				//// cannon shot check
				if(old_c != c-1 and grid[old_r][old_c+1] == NONE)
				{
					// qualified for cannon shot
					if(old_c+2 <= c-1)
						switch(grid[old_r][old_c+2])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r][old_c+2] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+2), CANNON_SHOT));
								break;
						}
					if(old_c+3 <= c-1)
						switch(grid[old_r][old_c+3])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r][old_c+3] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_c-3 >= 0 and grid[old_r][old_c-3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-3), SOLDIER_MOVE));
			}
		}
		if(old_c+2 <= c-1)	// can be last element
		{
			// cannon formation check
			if(grid[old_r][old_c+1] == BLACK_SOLDIER and grid[old_r][old_c+2] == BLACK_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == BLACK)	horizontal_cannons_diff++;
				else			horizontal_cannons_diff--;
				//// cannon shot check
				if(old_c != 0 and grid[old_r][old_c-1] == NONE)
				{
					// qualified for cannon shot
					if(old_c-2 >= 0)
						switch(grid[old_r][old_c-2])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r][old_c-2] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-2), CANNON_SHOT));
								break;
						}
					if(old_c-3 >= 0)
						switch(grid[old_r][old_c-3])
						{
							case WHITE_SOLDIER:
								if(my_color == BLACK)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case WHITE_TOWNHALL:
								if(grid[old_r][old_c-3] == WHITE_TOWNHALL)
									if(my_color == BLACK)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_c+3 <= c-1 and grid[old_r][old_c+3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+3), SOLDIER_MOVE));
			}
		}
	}
	return possible_actions;
}
// updates all feature variables and returns all possible moves for white team
////////////////////////////////////////
///////////// equivalent to ////////////
//////// get_white_actions  ////////////
////////////////////////////////////////
vector<action> board::update_white_feature_values()
{
	vector<action> possible_actions;
	// traverse through all white soldiers
	int white_soldiers_size = white_soldiers.size();
	for(int i=0; i<white_soldiers_size; i++)
	{
		bool is_backtracked = false;
		// soldier move
		const int &old_r = white_soldiers[i].get_r();
		const int &old_c = white_soldiers[i].get_c();
		// front moves
		if(old_r != c-1)	// front move not possible
		{
			// orthogonal
			switch(grid[old_r+1][old_c])
			{
				case BLACK_SOLDIER:	// or
					if(my_color == WHITE)	soldier_kills_diff++;	// my move
					else			soldier_kills_diff--;	// opponent move

					if((not is_backtracked) and old_r-2 >= 0)
					{
						is_backtracked = true;
						// backside left diagonal 
						if(old_c-2>=0)
						{
							switch(grid[old_r-2][old_c-2])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
									break;
								case BLACK_SOLDIER:	// can do a capture
									if(my_color == WHITE)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
									break;
							}
						}
						// backside orthogonal
						switch(grid[old_r-2][old_c])
						{
							case NONE:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
								break;
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
								break;
						}
						// backside right diagonal
						if(old_c+2 <= c-1)
						{
							switch(grid[old_r-2][old_c+2])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
									break;
								case BLACK_SOLDIER:
									if(my_color == WHITE)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
									break;
							}
						}
					}
				case NONE:		// or
				case BLACK_TOWNHALL:
					if(grid[old_r+1][old_c] == BLACK_TOWNHALL)
					if(my_color == WHITE)	townhall_kills_diff++;	// my move
					else			townhall_kills_diff--;	// opponent move

					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+1, old_c), SOLDIER_MOVE));
					break;
			}
			// left diagonal
			if(old_c != 0)
			{
				switch(grid[old_r+1][old_c-1])
				{
					case BLACK_SOLDIER:	// or
						if(my_color == WHITE)	soldier_kills_diff++;	// my move
						else			soldier_kills_diff--;	// opponent move

						if((not is_backtracked) and old_r-2 >= 0)
						{
							is_backtracked = true;
							// backside left diagonal 
							if(old_c-2>=0)
							{
								switch(grid[old_r-2][old_c-2])
								{
									case NONE:
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
										break;
									case BLACK_SOLDIER:	// can do a capture
										if(my_color == WHITE)	soldier_kills_diff++;	// my move
										else			soldier_kills_diff--;	// opponent move
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
										break;
								}
							}
							// backside orthogonal
							switch(grid[old_r-2][old_c])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
									break;
								case BLACK_SOLDIER:
									if(my_color == WHITE)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
									break;
							}
							// backside right diagonal
							if(old_c+2 <= c-1)
							{
								switch(grid[old_r-2][old_c+2])
								{
									case NONE:
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
										break;
									case BLACK_SOLDIER:
										if(my_color == WHITE)	soldier_kills_diff++;	// my move
										else			soldier_kills_diff--;	// opponent move
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
										break;
								}
							}
						}
					case NONE:		// or
					case BLACK_TOWNHALL:
						if(grid[old_r+1][old_c-1] == BLACK_TOWNHALL)
						if(my_color == WHITE)	townhall_kills_diff++;	// my move
						else			townhall_kills_diff--;	// opponent move

						possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+1, old_c-1), SOLDIER_MOVE));
						break;
				}
			}
			// right diagonal
			if(old_c != c-1)
			{
				switch(grid[old_r+1][old_c+1])
				{
					case BLACK_SOLDIER:	// or
						if(my_color == WHITE)	soldier_kills_diff++;	// my move
						else			soldier_kills_diff--;	// opponent move

						if((not is_backtracked) and old_r-2 >= 0)
						{
							is_backtracked = true;
							// backside left diagonal 
							if(old_c-2>=0)
							{
								switch(grid[old_r-2][old_c-2])
								{
									case NONE:
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
										break;
									case BLACK_SOLDIER:	// can do a capture
										if(my_color == WHITE)	soldier_kills_diff++;	// my move
										else			soldier_kills_diff--;	// opponent move
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
										break;
								}
							}
							// backside orthogonal
							switch(grid[old_r-2][old_c])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
									break;
								case BLACK_SOLDIER:
									if(my_color == WHITE)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
									break;
							}
							// backside right diagonal
							if(old_c+2 <= c-1)
							{
								switch(grid[old_r-2][old_c+2])
								{
									case NONE:
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
										break;
									case BLACK_SOLDIER:
										if(my_color == WHITE)	soldier_kills_diff++;	// my move
										else			soldier_kills_diff--;	// opponent move
										possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
										break;
								}
							}
						}
					case NONE:		// or
					case BLACK_TOWNHALL:
						if(grid[old_r+1][old_c+1] == BLACK_TOWNHALL)
						if(my_color == WHITE)	townhall_kills_diff++;	// my move
						else			townhall_kills_diff--;	// opponent move

						possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+1, old_c+1), SOLDIER_MOVE));
						break;
				}
			}
		}
		// side way move and capture
		if(old_c != 0)			// left side check is possible or not
		{
			switch(grid[old_r][old_c-1])
			{
				case BLACK_SOLDIER:
					if(my_color == WHITE)	soldier_kills_diff++;	// my move
					else			soldier_kills_diff--;	// opponent move

					if((not is_backtracked) and old_r-2 >= 0)
					{
						is_backtracked = true;
						// backside left diagonal 
						if(old_c-2>=0)
						{
							switch(grid[old_r-2][old_c-2])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
									break;
								case BLACK_SOLDIER:	// can do a capture
									if(my_color == WHITE)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
									break;
							}
						}
						// backside orthogonal
						switch(grid[old_r-2][old_c])
						{
							case NONE:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
								break;
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
								break;
						}
						// backside right diagonal
						if(old_c+2 <= c-1)
						{
							switch(grid[old_r-2][old_c+2])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
									break;
								case BLACK_SOLDIER:
									if(my_color == WHITE)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
									break;
							}
						}
					}
				case BLACK_TOWNHALL:
					if(grid[old_r][old_c-1] == BLACK_TOWNHALL)
					if(my_color == WHITE)	townhall_kills_diff++;	// my move
					else			townhall_kills_diff--;	// opponent move
				
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-1), SOLDIER_MOVE));
					break;
			}
		}
		if(old_c != c-1)		// right side check is possible or not
		{
			switch(grid[old_r][old_c+1])
			{
				case BLACK_SOLDIER:
					if(my_color == WHITE)	soldier_kills_diff++;	// my move
					else			soldier_kills_diff--;	// opponent move
				
					if((not is_backtracked) and old_r-2 >= 0)
					{
						is_backtracked = true;
						// backside left diagonal 
						if(old_c-2>=0)
						{
							switch(grid[old_r-2][old_c-2])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
									break;
								case BLACK_SOLDIER:	// can do a capture
									if(my_color == WHITE)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), SOLDIER_MOVE));
									break;
							}
						}
						// backside orthogonal
						switch(grid[old_r-2][old_c])
						{
							case NONE:
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
								break;
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), SOLDIER_MOVE));
								break;
						}
						// backside right diagonal
						if(old_c+2 <= c-1)
						{
							switch(grid[old_r-2][old_c+2])
							{
								case NONE:
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
									break;
								case BLACK_SOLDIER:
									if(my_color == WHITE)	soldier_kills_diff++;	// my move
									else			soldier_kills_diff--;	// opponent move
									possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), SOLDIER_MOVE));
									break;
							}
						}
					}
				case BLACK_TOWNHALL:
					if(grid[old_r][old_c+1] == BLACK_TOWNHALL)
					if(my_color == WHITE)	townhall_kills_diff++;	// my move
					else			townhall_kills_diff--;	// opponent move

					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+1), SOLDIER_MOVE));
					break;	
			}
		}

		/////////////////////////////////
		///////// cannon moves  /////////
		/////////////////////////////////
		/////// verticle direction //////
		/////////////////////////////////
		if(old_r+2 <= r-1)	// can be last element
		{
			// cnannon formation check	
			if(grid[old_r+1][old_c] == WHITE_SOLDIER and grid[old_r+2][old_c] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == WHITE)	verticle_cannons_diff++;
				else			verticle_cannons_diff--;
				//// cannon shot check
				if(old_r != 0 and grid[old_r-1][old_c] == NONE)
				{
					// qualified for cannon shot
					if(old_r-2>= 0)
						switch(grid[old_r-2][old_c])
						{	
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r-2][old_c] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c), CANNON_SHOT));
								break;
						}
					if(old_r-3>= 0)
						switch(grid[old_r-3][old_c])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r-3][old_c] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r+3 <= r-1 and grid[old_r+3][old_c] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c), SOLDIER_MOVE));
			}
		}
		if(old_r-2 >= 0)	// can be first element
		{
			// cannon formation check
			if(grid[old_r-1][old_c] == WHITE_SOLDIER and grid[old_r-2][old_c] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == WHITE)	verticle_cannons_diff++;
				else			verticle_cannons_diff--;
				//// cannon shot check
				if(old_r != r-1 and grid[old_r+1][old_c] == NONE)
				{
					// qualified for cannon shot
					if(old_r+2 <= r-1)
						switch(grid[old_r+2][old_c])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r+2][old_c] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c), CANNON_SHOT));
								break;
						}
					if(old_r+3 <= r-1)
						switch(grid[old_r+3][old_c])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r+3][old_c] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r-3 >= 0 and grid[old_r-3][old_c] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c), SOLDIER_MOVE));
			}
		}
		///////////////////////////////
		/////// right diagonal ////////
		///////////////////////////////
		if(old_r+2 <= r-1 and old_c-2 >= 0)	// can be last element
		{
			// cnannon formation check	
			if(grid[old_r+1][old_c-1] == WHITE_SOLDIER and grid[old_r+2][old_c-2] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == WHITE)	right_diagonal_cannons_diff++;
				else			right_diagonal_cannons_diff--;
				//// cannon shot check
				if(old_r != 0 and old_c != c-1 and grid[old_r-1][old_c+1] == NONE)
				{
					// qualified for cannon shot
					if(old_r-2>= 0 and old_c+2<= c-1)
						switch(grid[old_r-2][old_c+2])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r-2][old_c+2] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c+2), CANNON_SHOT));
								break;
						}
					if(old_r-3>= 0 and old_c+3<= c-1)
						switch(grid[old_r-3][old_c+3])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r-3][old_c+3] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c+3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r+3 <= r-1 and old_c-3>= 0 and grid[old_r+3][old_c-3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c-3), SOLDIER_MOVE));
			}
		}
		if(old_r-2 >= 0 and old_c+2<= c-1)	// can be first element
		{
			// cannon formation check
			if(grid[old_r-1][old_c+1] == WHITE_SOLDIER and grid[old_r-2][old_c+2] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == WHITE)	right_diagonal_cannons_diff++;
				else			right_diagonal_cannons_diff--;
				//// cannon shot check
				if(old_r != r-1 and old_c != 0 and grid[old_r+1][old_c-1] == NONE)
				{
					// qualified for cannon shot
					if(old_r+2 <= r-1 and old_c-2>= 0)
						switch(grid[old_r+2][old_c-2])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r+2][old_c-2] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c-2), CANNON_SHOT));
								break;
						}
					if(old_r+3 <= r-1 and old_c-3>= 0)
						switch(grid[old_r+3][old_c-3])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r+3][old_c-3] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c-3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r-3 >= 0 and old_c+3<=c-1 and grid[old_r-3][old_c+3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c+3), SOLDIER_MOVE));
			}
		}
		///////////////////////////////
		/////// left diagonal ////////
		///////////////////////////////
		if(old_r+2 <= r-1 and old_c+2 >= 0)	// can be last element
		{
			// cnannon formation check	
			if(grid[old_r+1][old_c+1] == WHITE_SOLDIER and grid[old_r+2][old_c+2] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == WHITE)	left_diagonal_cannons_diff++;
				else			left_diagonal_cannons_diff--;
				//// cannon shot check
				if(old_r != 0 and old_c != 0 and grid[old_r-1][old_c-1] == NONE)
				{
					// qualified for cannon shot
					if(old_r-2>= 0 and old_c-2>= 0)
						switch(grid[old_r-2][old_c-2])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r-2][old_c-2] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-2, old_c-2), CANNON_SHOT));
								break;
						}
					if(old_r-3>= 0 and old_c-3>= 0)
						switch(grid[old_r-3][old_c-3])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r-3][old_c-3] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c-3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r+3 <= r-1 and old_c+3<= c-1 and grid[old_r+3][old_c+3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c+3), SOLDIER_MOVE));
			}
		}
		if(old_r-2 >= 0 and old_c-2>= 0)	// can be FIRST element
		{
			// cannon formation check
			if(grid[old_r-1][old_c-1] == WHITE_SOLDIER and grid[old_r-2][old_c-2] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == WHITE)	left_diagonal_cannons_diff++;
				else			left_diagonal_cannons_diff--;
				//// cannon shot check
				if(old_r != r-1 and old_c != c-1 and grid[old_r+1][old_c+1] == NONE)
				{
					// qualified for cannon shot
					if(old_r+2 <= r-1 and old_c+2<= c-1)
						switch(grid[old_r+2][old_c+2])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r+2][old_c+2] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+2, old_c+2), CANNON_SHOT));
								break;
						}
					if(old_r+3 <= r-1 and old_c+3<= c-1)
						switch(grid[old_r+3][old_c+3])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r+3][old_c+3] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r+3, old_c+3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_r-3 >= 0 and old_c-3>= 0 and grid[old_r-3][old_c-3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r-3, old_c-3), SOLDIER_MOVE));
			}
		}
		/////////////////////////////////
		/////// horizontal direction ////
		/////////////////////////////////
		if(old_c-2 >= 0)	// can be first element
		{
			// cnannon formation check	
			if(grid[old_r][old_c-1] == WHITE_SOLDIER and grid[old_r][old_c-2] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == WHITE)	horizontal_cannons_diff++;
				else			horizontal_cannons_diff--;
				//// cannon shot check
				if(old_c != c-1 and grid[old_r][old_c+1] == NONE)
				{
					// qualified for cannon shot
					if(old_c+2 <= c-1)
						switch(grid[old_r][old_c+2])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r][old_c+2] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+2), CANNON_SHOT));
								break;
						}
					if(old_c+3 <= c-1)
						switch(grid[old_r][old_c+3])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r][old_c+3] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_c-3 >= 0 and grid[old_r][old_c-3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-3), SOLDIER_MOVE));
			}
		}
		if(old_c+2 <= c-1)	// can be last element
		{
			// cannon formation check
			if(grid[old_r][old_c+1] == WHITE_SOLDIER and grid[old_r][old_c+2] == WHITE_SOLDIER)
			{
				////// qualified for cannon moves
				if(my_color == WHITE)	horizontal_cannons_diff++;
				else			horizontal_cannons_diff--;
				//// cannon shot check
				if(old_c != 0 and grid[old_r][old_c-1] == NONE)
				{
					// qualified for cannon shot
					if(old_c-2 >= 0)
						switch(grid[old_r][old_c-2])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r][old_c-2] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-2), CANNON_SHOT));
								break;
						}
					if(old_c-3 >= 0)
						switch(grid[old_r][old_c-3])
						{
							case BLACK_SOLDIER:
								if(my_color == WHITE)	soldier_kills_diff++;	// my move
								else			soldier_kills_diff--;	// opponent move
							case NONE:
							case BLACK_TOWNHALL:
								if(grid[old_r][old_c-3] == BLACK_TOWNHALL)
									if(my_color == WHITE)	townhall_kills_diff++;
									else			townhall_kills_diff--;
								possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c-3), CANNON_SHOT));
								break;
						}
				}
				//// cannon move check
				if(old_c+3 <= c-1 and grid[old_r][old_c+3] == NONE)	// qualified for cannon move	
					possible_actions.push_back(action(make_pair(old_r, old_c), make_pair(old_r, old_c+3), SOLDIER_MOVE));
			}
		}
	}
	return possible_actions;
}




// printing the board, just for the sake of debugging
void board::print_all_actions(vector<action> possible_actions)
{
	cerr << "\npossible actions:\n";
	int action_size = possible_actions.size();
	for(int i=0; i<action_size; i++)
	{
		action temp_action = possible_actions[i];
		switch(temp_action.get_action_type())
		{
			case CANNON_SHOT:
				cerr << "CANNON_SHOT : " << temp_action.get_soldier_r() << ", " << temp_action.get_soldier_c()
		     	     	     << " -> " << temp_action.get_target_r() << ", " << temp_action.get_target_c() 
				     << " cost: " << temp_action.get_action_cost() <<endl;
				break;
			case SOLDIER_MOVE:
				cerr << "SOLDIER_MOVE: " << temp_action.get_soldier_r() << ", " << temp_action.get_soldier_c()
		     	     	     << " -> " << temp_action.get_target_r() << ", " << temp_action.get_target_c() 
				     << " cost: " << temp_action.get_action_cost() << endl;
				break;
		}
//		result(*this, temp_action.get_soldier_r(), temp_action.get_soldier_c(), 
  //				temp_action.get_target_r(), temp_action.get_target_c(), 
//				temp_action.get_action_type(), OUR_ACTION).print_board();
	}
}
void board::print_board()
{
	/////////////// remove code for outputing in file /////////////////
//	fstream myfile;
//	myfile.open("print_states.txt", ios::app);
	cerr << "current grid:\n";
	for(int i=0; i<r; i++)
	{
		for(int j=0; j<c; j++)
		{
			cell_content cell = grid[i][j];
			switch(cell)
			{
				case NONE:		cerr << "   "; break;
				case BLACK_TOWNHALL:	cerr << "B# "; break;
				case WHITE_TOWNHALL:	cerr << "W# "; break;
				case WHITE_SOLDIER:	cerr << "WO "; break;
				case BLACK_SOLDIER:	cerr << "BO "; break;
			}
		}
		cerr << endl;
	}
	cerr << endl;
//	myfile.close();
}
void board::print_feature_values()
{
	cout << "\nFeature values: \n";
	cout << "horizontal cannons diff: " << horizontal_cannons_diff << endl;
	cout << "verticle cannons diff: "  << verticle_cannons_diff << endl;
	cout << "right diagonal diff: " << right_diagonal_cannons_diff << endl;
	cout << "left diagonal diff: " << left_diagonal_cannons_diff << endl;
	cout << "soldier kill spots diff: " << soldier_kills_diff << endl;
	cout << "townhall kills spot diff: "<< townhall_kills_diff << endl;
	cout << endl;
}
void board::print_black_soldiers()
{
	cout << "black soldiers:		";
	for(int i=0; i<black_soldiers.size(); i++)	cout << black_soldiers[i].get_r() << "," << black_soldiers[i].get_c() << "  ";
	cout << endl;
}
void board::print_white_soldiers()
{
	cout << "white soldiers:		";
	for(int i=0; i<white_soldiers.size(); i++)	cout << white_soldiers[i].get_r() << "," << white_soldiers[i].get_c() << "  ";
	cout << endl;
}
void board::print_black_townhalls()
{
	cout << "black townhalls:	";
	for(int i=0; i<black_townhalls.size(); i++)	cout << black_townhalls[i].get_r() << "," << black_townhalls[i].get_c() << "  ";
	cout << endl;
}
void board::print_white_townhalls()
{
	cout << "white townhalls:	" ;
	for(int i=0; i<white_townhalls.size(); i++)	cout << white_townhalls[i].get_r() << "," << white_townhalls[i].get_c() << "  ";
	cout << endl;
}
