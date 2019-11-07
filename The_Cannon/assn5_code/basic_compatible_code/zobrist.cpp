#include <iostream>
#include "classes.h"
#include "functions.h"
using namespace std;


// r = no of board cells, c = no of pieces = 4
custom_hash::custom_hash(int r, int c)
{
	rand_grid.resize(r, vector<long long>(c, 0));
	for (auto &i : rand_grid)
		for (auto &j : i)
			j = rand();

	lt.insert(make_pair(BLACK_SOLDIER, 0));
	lt.insert(make_pair(BLACK_TOWNHALL, 1));
	lt.insert(make_pair(WHITE_SOLDIER, 2));
	lt.insert(make_pair(WHITE_TOWNHALL, 3));

}
custom_hash::custom_hash() {}

long long custom_hash::findhash(board &a)
{
	long long hash = 0;
	vector<vector<cell_content>> grid = a.get_grid();
	size_t r = grid.size();
	size_t c = grid[0].size();
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			if (grid[i][j] != NONE)
				hash ^= rand_grid[i * c + j][lt[grid[i][j]]];
	return hash;
}

long long custom_hash::next_hash(board &bo, long long prev_hash, action &a)
{
	vector<vector<cell_content> > grid = bo.get_grid();
	size_t r = grid.size();
	size_t c = grid[0].size();
	int i1 = a.get_soldier_r();
	int j1 = a.get_soldier_c();

	int i2 = a.get_target_r();
	int j2 = a.get_target_c();

	cell_content source = grid[i1][j1];
	cell_content dest = grid[i2][j2];

	if (source != NONE and a.get_action_type() == SOLDIER_MOVE)
		// remove the source from i1, j1 and put it into i2, j2 cell
		prev_hash ^= rand_grid[i1 * c + j1][lt[source]] ^ rand_grid[i2 * c + j2][lt[source]]; // WARNING: Cannon shot not compatible // WARNING : DWIJESH CHANGE
	else if (source == NONE)
		throw runtime_error("Invalid Action - Source is Null ");
	if (dest != NONE)
		// removeing enemy thing if there exists
		prev_hash ^= rand_grid[i2 * c + j2][lt[dest]];
	return prev_hash;
}
void custom_hash::print_rand_grid()
{
	for(int i=0; i<rand_grid.size(); i++)
	{
		for(int j=0; j<rand_grid[0].size(); j++)
			cout << rand_grid[i][j] << " ";
		cout << endl;
	}
}
