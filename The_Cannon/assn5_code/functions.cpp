#include<iostream>
#include "classes.h"
#include "functions.h"
using namespace std;

// ##########################################################################################################################################
//		while using update method and remove method simultaneously, remove method must be called first as shown below
//				new_board.remove_black_townhall(target_r, target_c);
//				new_board.update_white_soldier(soldier_r, soldier_c, target_r, target_c);
//		I am assuming we will always be the one at the 5, 6, 7 th row
//		we are always black though we are not // but valid assumption
// ##########################################################################################################################################


// soldier_r: 	r coordinate of a soldier under consideration
// soldier_c:	c coordinate of a soldier under consideration
// target_r: 	r coordinate of a target 
// target_c:	c coordinate of a target 
// action:	whether a cannon shot or a soldier move
// returns a new board configuration
// *******************************************
// ******* OUR_ACTION: BLACK_ACTION **********
// ******* OPPONENT_ACTION: WHITE_ACTION *****
// *******************************************
board result(board &old_board, const int &soldier_r, const int &soldier_c, const int &target_r, const int &target_c, action_type action_t, action_by whose_action)
{
	// assuming new copy of old_board will be formed
	board new_board = old_board;
	cell_content cell = old_board.get_cell(target_r, target_c);
	switch(action_t)
	{
		case CANNON_SHOT:
			switch(whose_action)
			{
				case OUR_ACTION://black
					// remove opponent's entity located at target_r and target_c
					// from board and list
					switch(cell)
					{
						case NONE:
							break;
						case WHITE_SOLDIER:	// assuming we are black
							new_board.remove_white_soldier(target_r, target_c);
							break;
						case BLACK_SOLDIER:
							cout << "we have killed our soldier. Wrong move\n";
							break;
						case WHITE_TOWNHALL:
							new_board.remove_white_townhall(target_r, target_c);
							break;
						case BLACK_TOWNHALL:
							cout << "we have killed our townhall. Wrong move.\n";
							break;
					}	
					break;
				case OPPONENT_ACTION: //white
					// remove our entity located at target_r and target_c
					// from board and list
					switch(cell)
					{
						case NONE:
							break;
						case WHITE_SOLDIER:
							cout << "opponent killed its soldier. Wrong move.\n";
							break;
						case BLACK_SOLDIER:
							new_board.remove_black_soldier(target_r, target_c);
							break;
						case WHITE_TOWNHALL:
							cout << "opponent killed its own townhall. Wrong move.\n";
							break;
						case BLACK_TOWNHALL:
							new_board.remove_black_townhall(target_r, target_c);
							break;
					}
					break;
			}
			break;
		case SOLDIER_MOVE:
			switch(whose_action)
			{
				case OUR_ACTION: // black
					// remove opponent's entity located at target_r and target_c
					// from board and list
					switch(cell)
					{
						case NONE:
//							switch(old_board.get_my_color())
//							{
//								case WHITE:	// modify white soldier list and grid
//									new_board.update_white_soldier(soldier_r, soldier_c, target_r, target_c);
//									break;
//								case BLACK:	// modify black soldier list and grid
//									new_board.update_black_soldier(soldier_r, soldier_c, target_r, target_c);
//									break;
//					 		}
							new_board.update_black_soldier(soldier_r, soldier_c, target_r, target_c);
							break;
						case WHITE_SOLDIER:	// it must be my opponent's soldier
							new_board.remove_white_soldier(target_r, target_c);
							new_board.update_black_soldier(soldier_r, soldier_c, target_r, target_c);
							break;
						case BLACK_SOLDIER:
							cout << "My soldier killed my another soldier. Wrong move.\n";
							break;
						case WHITE_TOWNHALL:
							new_board.remove_white_townhall(target_r, target_c);
							new_board.update_black_soldier(soldier_r, soldier_c, target_r, target_c);
							break;
						case BLACK_TOWNHALL:
							cout << "my soldier can not kill my own townhall. Wrong move.\n";
							break;
					}	
					break;
				case OPPONENT_ACTION:// white
					// remove opponent's entity located at target_r and target_c
					// from board and list
					switch(cell)
					{
						case NONE:
//							switch(old_board.get_my_color())
//							{
//								case WHITE:	// modify black soldier list and grid
//									new_board.update_black_soldier(soldier_r, soldier_c, target_r, target_c);
//									break;
//								case BLACK:	// modify white soldier list and grid
//									new_board.update_white_soldier(soldier_r, soldier_c, target_r, target_c);
//									break;
//							}
							new_board.update_white_soldier(soldier_r, soldier_c, target_r, target_c);
							break;
						case WHITE_SOLDIER:	// it must be my opponent's soldier
							cout << "opponent killed its own soldier. Wrong move\n";
							break;
						case BLACK_SOLDIER:
							new_board.remove_black_soldier(target_r, target_c);
							new_board.update_white_soldier(soldier_r, soldier_c, target_r, target_c);
							break;
						case WHITE_TOWNHALL:
							cout << "opponene killed its own townhall. Wrong move.\n";
							break;
						case BLACK_TOWNHALL:
							new_board.remove_black_townhall(target_r, target_c);
							new_board.update_white_soldier(soldier_r, soldier_c, target_r, target_c);
							break;
					}
					break;
			}
			break;
	}	
	return new_board;
}
