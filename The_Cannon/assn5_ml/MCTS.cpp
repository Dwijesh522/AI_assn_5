#include <vector>
#include <utility>
#include <iostream>
#include <chrono>
#include "classes.h"
#include "functions.h"
#include "MCTS.h"
#include <math.h>
#include <pair>
//#include<fstream>
using namespace std;

random_device rd;
mt19937 mt(rd());

MCTS_State::MCTS_State(board &b, action_by a)
{
    board_state = b;
    visit_count = 0;
    expected_reward = 0;
    // Tree attributes
    parent = nullptr;
    current_act = a;
    children_count = b.get_possible_action().size();
}

action_by MCTS_State::change_player(action_by act)
{
    if (act == OUR_ACTION)
    {
        return OPPONENT_ACTION;
    }
    else
    {
        return OUR_ACTION;
    }
}

vector<MCTS_State> MCTS_State::getAllPossibleStates()
{
    vector<action> possible_actions = board_state.get_possible_action();
    vector<MCTS_State> children;
    for (auto x : possible_actions)
    {
        board next_board = result(board_state, x.get_soldier_r, x.get_soldier_c, x.get_target_r, x.get_target_c, x.get_action_type, current_act);
        children.push_back(MCTS_State(next_board, change_player(current_act)));
    }
    return children;
}

float MCTS_State::PUCT()
{
    return state.expected_reward + (CPUCT * sqrt(parent->visit_count) / (1 + visit_count));
}

vector<MCTS_State *> MCTS_State::get_children() { return children; }

MCTS_State MCTS_State::get_parent() { return parent; }

action_by MCTS_State::get_current_act() { return current_act; }

bool MCTS_State::is_terminal() { return board_state.terminal_test(1, 3); }

pair<float, float> MCTS_State::utility_fn()
{
    //return type - our score, adversary score.
    vector<MCTS_State> all_children = this.getAllPossibleStates();
    if (all_children.size() == 0)
        if (this.get_current_act() == OUR_ACTION)
            return this.board_state.utility_function(false);
        else
            return this.board_state.utility_function(true);
}

void MCTS_State::increment_visit_count()
{
    this.visit_count++;
}

void MCTS_State::update_reward(float value)
{
    this.expected_reward = (this.expected_reward * visit_count + value) / (visit_count + 1);
}

float MCTS_State::get_expected_reward()
{
    return expected_reward;
}

////////////////////////////////////MCTS Definitions///////////////////////////

MCTS::MCTS(board &b, action_by a)
{
    root = MCTS_State(b, a);
}

vector<int> vector_subtract(vector<int> a1, vector<int> a2)
{
    //a1,a2 have to be sorted.
    if (a2.size() > a1.size())
    {
        vector<int> temp = a1;
        a1 = a2;
        a2 = temp;
    }
    count = 0;
    vector<int> ans;
    for (int i = 0; i < a1.size(); i++)
    {
        if (count < a2.size() && a1.at(i) == a2.at(count))
        {
            count++;
        }
        else
        {
            ans.push_back(a1.at(i));
        }
    }
    return ans;
}

MCTS_State *MCTS::traverse()
{
    MCTS_State *current_node = &root;

    while (!current_node->children_picked.empty() && (current_node->children_picked.size() == current_node->children_count))
    { // The condition is that the node is fully expanded
        float max = -INFINITY;
        MCTS_State *possible_node;
        for (auto x : current_node->children)
        {
            if (max < x.PUCT())
            {
                max = x.PUCT();
                possible_node = x;
            }
        }
        current_node = possible_node;
    }
    return current_node;
}

void *MCTS::expand(MCTS_State *current_node)
{
    if (!current_node.is_terminal())
    {
        v = current_node->children_picked;
        if (!v.empty())
        {
            sort(v.begin(), v.end());
        }

        count = 0;
        vector<int> unpicked;
        for (int i = 0; i < current_node->children_count; i++)
        {
            unpicked.push_back(i);
        }
        unpicked = vector_subtract(unpicked, children_picked);

        uniform_int_distribution<> dist(0, unpicked.size() - 1); //range is 20 to 22
        int random = dist(mt);
        vector<MCTS_State> all_children = current_node->getAllPossibleStates();
        MCTS_State chosen_node = all_children.at(unpicked[random]);

        chosen_node.parent = current_node;
        current_node->children_picked.push_back(unpicked[random]);
        current_node->children.push_back(chosen_node);
        current_node->children_order.push_back(unpicked[random]);

        size_t s = current_node->children.size();
        simulate_node = &current_node->children.at(s - 1);
        float reward = simulate(simulate_node);
        backprop(current_node, reward);
    }
    else
    {
        pair<float, float> utility = current_node->utility_fn();
        backprop(current_node, utility);
    }
}

float MCTS::simulate(MCTS_State *node)
{
    current_node = *node;
    int count = 0;
    while (!current_node.is_terminal() && count < cut_off_depth)
    {
        count++;
        vector<MCTS_State> temp = current_node.getAllPossibleStates();
        uniform_int_distribution<> dist(0, temp.size() - 1); //range is 20 to 22
        int random = dist(mt);
        current_node = temp[random];
    }
    pair<float, float> reward = current_node.utility_fn();
    return reward;
}

void backprop(MCTS_State *node, pair<float, float> utility)
{
    MCTS_State *current_node = node;
    if (current_node->get_current_act() == OUR_ACTION)
    {
        current_node->update_reward(utility.first);
    }
    else
        (current_node->get_current_act() == OPPONENT_ACTION)
        {
            current_node->update_reward(-1 * utility.second);
        }
    current_node->visit_count++;

    while (current_node->parent != nullptr)
    {
        current_node = current_node->parent;

        if (current_node->get_current_act() == OUR_ACTION)
        {
            current_node->update_reward(utility.first);
        }
        else
            (current_node->get_current_act() == OPPONENT_ACTION)
            {
                current_node->update_reward(-1 * utility.second);
            }
        current_node->visit_count++;
    }
}

int MCTS::find_best_state()
{ //Finds the index of the best state from among the board's children actions
    //WARNING: this is not the best out of MCTS states.
    //This is the index of the action returned from board_state.get_possible_action();
    vector<MCTS_State> children = root.children;
    int max = -INFINITY;
    int max_idx = -1;
    for (int i = 0; i < children.size(); i++)
    {
        if (max < children[i].expected_reward)
        {
            max = children[i].expected_reward;
            max_idx = i;
        }
    }
    return root.children_order[max_idx];
}
