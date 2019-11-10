#include <iostream>
#include "classes.h"
#include "functions.h"
using namespace std;

// struct HASH
// {
//     size_t operator()(const board &x) const
//     {
//         return hash<long long>()(((long long)x.first) ^ (((long long)x.second) << 32));
//     }
// };	

// r = no. of board cells, c = no. of pieces = 4
transposition_table::transposition_table(int r, int c) 	{
	hash_fn = custom_hash(r, c); // WARNING : DWIJESH CHANGE
}
void transposition_table::insert(long long hn, board b)
{
	mp.insert(make_pair(hn, b));
}

pair<bool, action> transposition_table::find(long long hn, board *b1)
{
	if (mp.find(hn) != mp.end())
	{
		board b2 = mp[hn];
		if (b1->board_equals(b2))	return make_pair(true, b2.get_best_action_found());
		else				return make_pair(false, b2.get_best_action_found());
	}
	else					return make_pair(false, b1->get_best_action_found());
}
pair<bool, board> transposition_table::find_stagnant(long long hn, board *b1)
{
	auto it = mp.find(hn);
	if (it != mp.end())
	{
		board b2 = mp[hn];
		if (b1->board_equals(b2))	return make_pair(true, b2);
		else				return make_pair(false, b2);
	}
	else					return make_pair(false, *b1);
}
int transposition_table::get_freq(long long hn, board *b1)
{
	auto it = mp.find(hn);
	if (it != mp.end() and b1->board_equals(it->second))	return it->second.get_freq();
	else							return 0;
}
void transposition_table::add_freq(long long hn, board *b1)
{
	auto it = mp.find(hn);
	if (it != mp.end())			it->second.add_freq();
}
void transposition_table::print_explored_boards()
{
	for(auto i: mp)
	{
		cout << "hash value: " << i.first << endl;
		i.second.print_board();
		cout << endl;
		action temp_action = i.second.get_best_action_found();
		cout << "mapped action is: " << temp_action.get_soldier_r() << " " << temp_action.get_soldier_c() << " "
			<< temp_action.get_target_r() << " " << temp_action.get_target_c() << endl;
	}
}
