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

void print_event(event_type event)
{
	switch(event)
	{
		case I_KILLED_SOLDIER_POS:
			cerr << "I killed soldier positively\n";
			break;
		case OPP_KILLED_SOLDIER_POS:
			cerr << "Opp killed soldier positively\n";
			break;
		case I_KILLED_TOWNHALL:
			cerr << "I killed townhall\n";
			break;
		case OPP_KILLED_TOWNHALL:
			cerr << "Opp killed townhall\n";
			break;
		case I_KILLED_SOLDIER_NEG:
			cerr << "I killed soldier negatively\n";
			break;
		case OPP_KILLED_SOLDIER_NEG:
			cerr << "Opp killed soldier negatively\n";
			break;
		case ME_ADDING_TOWNHALL_COVER:
			cerr << "I added townhall cover\n";
			break;
		case ME_ADDING_SOLDIER_COVER:
			cerr << "I added soldier cover\n";
			break;
		case ME_UNDER_CANNON_ATTACK:
			cerr << "I am under cannon attack\n";
			break;
		case ME_UNDER_SOLDIER_ATTACK:
			cerr << "I am under soldier attack\n";
			break;
		case NO_EVENT:
			cerr << "No event\n";
			break;
	}
}

// this function is called after our ply to figure out one of the following events:
// me_adding_townhall_cover
// me_adding_soldier_cover
// me_under_cannon_attack
// me_under_soldier_attack
// no_event
event_type vicinity_check(board &old_board, action_by whose_action, const int &target_r, const int &target_c, color my_color)
{
	event_type event = NO_EVENT;
	int board_rows = old_board.get_number_of_rows(), board_cols = old_board.get_number_of_cols();
	if(whose_action == OUR_ACTION)
	{	
		//// me_adding_townhall_cover
		//// me_adding_soldier_cover
		//// me_under_soldier_attack
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
					if(my_color == WHITE)	event = ME_ADDING_SOLDIER_COVER;
					else			event = ME_UNDER_SOLDIER_ATTACK;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	event = ME_UNDER_SOLDIER_ATTACK;
					else			event = ME_ADDING_SOLDIER_COVER;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	event = ME_ADDING_TOWNHALL_COVER;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	event = ME_ADDING_TOWNHALL_COVER;
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
					if(my_color == WHITE)	event = ME_ADDING_SOLDIER_COVER;
					else			event = ME_UNDER_SOLDIER_ATTACK;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	event = ME_UNDER_SOLDIER_ATTACK;
					else			event = ME_ADDING_SOLDIER_COVER;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	event = ME_ADDING_TOWNHALL_COVER;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	event = ME_ADDING_TOWNHALL_COVER;
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
					if(my_color == WHITE)	event = ME_ADDING_SOLDIER_COVER;
					else			event = ME_UNDER_SOLDIER_ATTACK;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	event = ME_UNDER_SOLDIER_ATTACK;
					else			event = ME_ADDING_SOLDIER_COVER;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	event = ME_ADDING_TOWNHALL_COVER;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	event = ME_ADDING_TOWNHALL_COVER;
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
					if(my_color == WHITE)	event = ME_ADDING_SOLDIER_COVER;
					else			event = ME_UNDER_SOLDIER_ATTACK;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	event = ME_UNDER_SOLDIER_ATTACK;
					else			event = ME_ADDING_SOLDIER_COVER;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	event = ME_ADDING_TOWNHALL_COVER;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	event = ME_ADDING_TOWNHALL_COVER;
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
					if(my_color == WHITE)	event = ME_ADDING_SOLDIER_COVER;
					else			event = ME_UNDER_SOLDIER_ATTACK;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	event = ME_UNDER_SOLDIER_ATTACK;
					else			event = ME_ADDING_SOLDIER_COVER;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	event = ME_ADDING_TOWNHALL_COVER;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	event = ME_ADDING_TOWNHALL_COVER;
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
					if(my_color == WHITE)	event = ME_ADDING_SOLDIER_COVER;
					else			event = ME_UNDER_SOLDIER_ATTACK;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	event = ME_UNDER_SOLDIER_ATTACK;
					else			event = ME_ADDING_SOLDIER_COVER;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	event = ME_ADDING_TOWNHALL_COVER;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	event = ME_ADDING_TOWNHALL_COVER;
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
					if(my_color == WHITE)	event = ME_ADDING_SOLDIER_COVER;
					else			event = ME_UNDER_SOLDIER_ATTACK;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	event = ME_UNDER_SOLDIER_ATTACK;
					else			event = ME_ADDING_SOLDIER_COVER;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	event = ME_ADDING_TOWNHALL_COVER;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	event = ME_ADDING_TOWNHALL_COVER;
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
					if(my_color == WHITE)	event = ME_ADDING_SOLDIER_COVER;
					else			event = ME_UNDER_SOLDIER_ATTACK;
					break;
				case BLACK_SOLDIER:
					if(my_color == WHITE)	event = ME_UNDER_SOLDIER_ATTACK;
					else			event = ME_ADDING_SOLDIER_COVER;
					break;
				case WHITE_TOWNHALL:
					if(my_color == WHITE)	event = ME_ADDING_TOWNHALL_COVER;
					break;
				case BLACK_TOWNHALL:
					if(my_color == BLACK)	event = ME_ADDING_TOWNHALL_COVER;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon1_r, cannon1_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon1_r + r_offset2, cannon1_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon1_r + r_offset3, cannon1_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
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
					event = ME_UNDER_CANNON_ATTACK;
			}
			else
			{
				if(	old_board.get_cell(cannon2_r, cannon2_c) == WHITE_SOLDIER and 
					old_board.get_cell(cannon2_r + r_offset2, cannon2_c + c_offset2) == WHITE_SOLDIER and
					old_board.get_cell(cannon2_r + r_offset3, cannon2_c + c_offset3) == WHITE_SOLDIER
				  )
					event = ME_UNDER_CANNON_ATTACK;
			}
		}	
		return event;
	}
	else
	{
		cerr << "Not implemented yet. Learning should be done when a move affected me in some way.\n";
		return NO_EVENT;
	}
}

pair<board, event_type> result(board &old_board, const int &soldier_r, const int &soldier_c, const int &target_r, const int &target_c, action_type action_t, action_by whose_action, color my_color)
{
	// assuming new copy of old_board will be formed
	board new_board = old_board;
	event_type event = NO_EVENT, temp_event = NO_EVENT;
	cell_content cell = old_board.get_cell(target_r, target_c);
	cell_content old_cell = old_board.get_cell(soldier_r, soldier_c);
	bool is_my_move = false;
	if((old_cell == BLACK_SOLDIER and my_color == BLACK) or (old_cell == WHITE_SOLDIER and my_color == WHITE)) is_my_move = true;

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
							if(my_color == BLACK)	event = I_KILLED_SOLDIER_POS;
							else			event = OPP_KILLED_SOLDIER_POS;
							new_board.remove_white_soldier(target_r, target_c);
							break;
						case BLACK_SOLDIER:
							cerr << "we have killed our soldier. Wrong move\n";
							break;
						case WHITE_TOWNHALL:
							if(my_color == BLACK) 	event = I_KILLED_TOWNHALL;
							else			event = OPP_KILLED_TOWNHALL;
							new_board.remove_white_townhall(target_r, target_c);
							break;
						case BLACK_TOWNHALL:
							cerr << "we have killed our townhall. Wrong move.\n";
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
							cerr << "opponent killed its soldier. Wrong move.\n";
							break;
						case BLACK_SOLDIER:
							if(my_color == BLACK)	event = OPP_KILLED_SOLDIER_POS;
							else			event = I_KILLED_SOLDIER_POS;
							new_board.remove_black_soldier(target_r, target_c);
							break;
						case WHITE_TOWNHALL:
							cerr << "opponent killed its own townhall. Wrong move.\n";
							break;
						case BLACK_TOWNHALL:
							if(my_color == BLACK) 	event = OPP_KILLED_TOWNHALL;
							else			event = I_KILLED_TOWNHALL;
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
						 	if(is_my_move)	event = vicinity_check(old_board, OUR_ACTION, target_r, target_c, my_color);
							new_board.update_black_soldier(soldier_r, soldier_c, target_r, target_c);
							break;
						case WHITE_SOLDIER:	// it must be my opponent's soldier
							if(is_my_move)	// I am black
							{
								event = I_KILLED_SOLDIER_POS;
								temp_event = vicinity_check(old_board, OUR_ACTION, target_r, target_c, my_color);
								switch(temp_event)
								{
									case ME_UNDER_CANNON_ATTACK:
										event = I_KILLED_SOLDIER_NEG;
										break;
									case ME_UNDER_SOLDIER_ATTACK:
										event = I_KILLED_SOLDIER_NEG;
										break;
								}
							}
							else	event = OPP_KILLED_SOLDIER_POS;

							new_board.remove_white_soldier(target_r, target_c);
							new_board.update_black_soldier(soldier_r, soldier_c, target_r, target_c);
							break;
						case BLACK_SOLDIER:
							cerr << "My soldier killed my another soldier. Wrong move.\n";
							break;
						case WHITE_TOWNHALL:
							if(is_my_move)	event = I_KILLED_TOWNHALL;
							else		event = OPP_KILLED_TOWNHALL;

							new_board.remove_white_townhall(target_r, target_c);
							new_board.update_black_soldier(soldier_r, soldier_c, target_r, target_c);
							break;
						case BLACK_TOWNHALL:
							cerr << "my soldier can not kill my own townhall. Wrong move.\n";
							break;
					}	
					break;
				case OPPONENT_ACTION:// white
					// remove opponent's entity located at target_r and target_c
					// from board and list
					switch(cell)
					{
						case NONE:
						 	if(is_my_move)	event = vicinity_check(old_board, OUR_ACTION, target_r, target_c, my_color);

							new_board.update_white_soldier(soldier_r, soldier_c, target_r, target_c);
							break;
						case WHITE_SOLDIER:	// it must be my opponent's soldier
							cerr << "opponent killed its own soldier. Wrong move\n";
							break;
						case BLACK_SOLDIER:
							if(is_my_move)	// I am black
							{
								event = I_KILLED_SOLDIER_POS;
								temp_event = vicinity_check(old_board, OUR_ACTION, target_r, target_c, my_color);
								switch(temp_event)
								{
									case ME_UNDER_CANNON_ATTACK:
										event = I_KILLED_SOLDIER_NEG;
										break;
									case ME_UNDER_SOLDIER_ATTACK:
										event = I_KILLED_SOLDIER_NEG;
										break;
								}
							}
							else	event = OPP_KILLED_SOLDIER_POS;

							new_board.remove_black_soldier(target_r, target_c);
							new_board.update_white_soldier(soldier_r, soldier_c, target_r, target_c);
							break;
						case WHITE_TOWNHALL:
							cerr << "opponene killed its own townhall. Wrong move.\n";
							break;
						case BLACK_TOWNHALL:
							if(is_my_move)	event = I_KILLED_TOWNHALL;
							else		event = OPP_KILLED_TOWNHALL;

							new_board.remove_black_townhall(target_r, target_c);
							new_board.update_white_soldier(soldier_r, soldier_c, target_r, target_c);
							break;
					}
					break;
			}
			break;
	}
	new_board.init_freq();	
	return make_pair(new_board, event);
}
