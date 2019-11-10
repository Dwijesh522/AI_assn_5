#pragma once
#include "classes.h"
using namespace std;

class MCTS_State
{
private:
    // Monte Carlo Attributes
    board board_state;
    double visit_count;
    double expected_reward;
    action_by current_act;
    const static int C_PUCT = 1;
    vector<int> children_order;

public:
    // Tree attributes
    MCTS_State *parent;
    vector<MCTS_State> children;
    int children_count;
    vector<int> children_picked;

    //Functions
    MCTS_State(board &b, int p_no);
    action_by change_player(action_by act);
    vector<MCTS_State> getAllPossibleStates();
    float PUCT();
    vector<MCTS_State> get_children();
    MCTS_State get_parent();
    action_by get_current_act();
    bool is_terminal();
    float utility_fn();
    void increment_visit_count();
    void update_reward(float value);
    float get_expected_reward();
};

class MCTS
{
private:
    const static int WIN_SCORE = 10;
    MCTS_State root;
    const static int cut_off_depth = 150; // just temporarily set. try different values.

public:
    MCTS(MCTS_State &root);
    MCTS_State *traverse();
    void expand(MCTS *current_node);
    float simulate(MCTS_State *node);
    void backprop(MCTS_State *current_node, float utility);
    int find_best_state();
    // MCTS_State findNextMove(board game_state, int playerNo, float time);
};