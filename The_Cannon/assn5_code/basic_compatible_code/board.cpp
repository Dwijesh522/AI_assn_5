#include<vector>
#include<utility>
#include<iostream>
#include "classes.h"
#include "functions.h"
//#include<fstream>
using namespace std;

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
}
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
		
//		cerr << "ans is: " << ans << endl;
//		cerr << "///////////////////////////////////////////////////////////////\n";
//		cerr <<	((1 * our_soldiers) / 12) << " ";
	//	cerr <<	((1 * enemy_soldiers) / 12) << " ";

	//	cerr <<	((1 * soldier_diff) / 12) << " ";
	//	cerr <<	((1 * townhall_diff) / 2) << " ";

	//	cerr <<	((1 * our_horizontal_cannons) / 4) << " ";
	//	cerr <<	(((1) * enemy_horizontal_cannons) / 4) <<  " ";

	//	cerr <<	((1 * our_verticle_cannons) / 4) << " ";
	//	cerr <<	(((1) * enemy_verticle_cannons) / 4) << " ";

	//	cerr <<	((1 * our_right_cannons) / 4) << " ";
	//	cerr <<	(((1) * enemy_right_cannons) / 4) << " ";

	//	cerr <<	((1 * our_left_cannons) / 4) << " ";
	//	cerr <<	(((1) * enemy_left_cannons) / 4) << " ";

	//	cerr <<	((1 * soldiers_i_can_kill) / 12) <<" ";
	//	cerr <<	(((1) * soldiers_enemy_can_kill) / 12) << " ";

	//	cerr <<	((1 * townhalls_i_can_kill) / 4) << " ";
	//	cerr <<	(((1) * townhalls_enemy_can_kill) / 4) << " ";

	//	cerr <<	((1 * our_moves.size()) / 40) << " ";
	//	cerr <<	(((1) * opp_moves.size()) / 40) << endl;
	//	cerr << "///////////////////////////////////////////////////////////////\n";
//	print_board();

	return ans;
}

// updates all feature variables and returns all possible moves for black team
////////////////////////////////////////
///////////// equivalent to ////////////
//////// get_black_actions  ////////////
////////////////////////////////////////
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
