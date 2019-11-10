#include "classes.h"
#include "functions.h"
using namespace std;

action::action(pair<int, int> cp, pair<int, int> fp, action_type action_t)
{
	current_pos = cp;
	future_pos = fp;
	this->action_t = action_t;
	action_cost = -1;
}
action::action()
{
	current_pos = make_pair(-1, -1);
	future_pos = make_pair(-1, -1);
	action_t = SOLDIER_MOVE;	// just for the initialization purpose
	action_cost = -1;
}

pair<int, int> action::get_current_pos()
{
	return current_pos;
}
action_type action::get_action_type()
{
	return action_t;
}
int action::get_soldier_r()
{
	return current_pos.first;
}
int action::get_soldier_c()
{
	return current_pos.second;
}
int action::get_target_r()
{
	return future_pos.first;
}
int action::get_target_c()
{
	return future_pos.second;
}
float action::get_action_cost()
{
	return action_cost;
}
void action::set_action_cost(float cost)
{
	action_cost = cost;
}
pair<int, int> action::get_future_pos() { return future_pos; }
