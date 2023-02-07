/*
 * Name: Alvin Yang 
 * Date Submitted: 12/6/22
 * Lab Section: 001
 * Assignment Name: Using Breadth-First Search to Solve Puzzles
 */

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <bitset>
using namespace std;

// Reflects what each node represents...
// int with each bit == 0 left of river, bit == 1 right of river
typedef int state;

// Bit position representation for wolf/goat/cabbage/me
bool bit(int x, int i) { return (x>>i) & 1; }
const int wolf=0, goat=1, cabbage=2, me=3;

// GENERIC -- these shouldn't need to be changed...
map<state, bool> visited;         // have we queued up this state for visitation?
map<state, state> pred;           // predecessor state we came from
map<state, int> dist;             // distance (# of hops) from source node
map<state, vector<state>> nbrs;   // vector of neighboring states

map<pair<state,state>, string> edge_label;

// GENERIC (breadth-first search, outward from curnode)
void search(state source_node)
{
  queue<state> to_visit;
  to_visit.push(source_node);
  visited[source_node] = true;
  dist[source_node] = 0;
  
  while (!to_visit.empty()) {
    state curnode = to_visit.front();
    to_visit.pop();
    for (state n : nbrs[curnode])
      if (!visited[n]) {
	pred[n] = curnode;
	dist[n] = 1 + dist[curnode];
	visited[n] = true;
	to_visit.push(n);
      }
  }
}

string state_string(state s)
{
  string items[4] = { "wolf", "goat", "cabbage", "you" }, result = "";
  for (int i=0; i<4; i++)
    if (!bit(s, i)) result += items[i] + " ";
  result += " |river| ";
  for (int i=0; i<4; i++)
    if (bit(s, i)) result += items[i] + " ";
  return result;
}

// GENERIC
void print_path(state s, state t)
{
  if (s != t) {
    print_path(s, pred[t]);
    cout << edge_label[make_pair(pred[t], t)] << ": " << state_string(t) << "\n";
  } else {
    cout << "Initial state: " << state_string(s) << "\n";
  }
}

string neighbor_label(int s, int t)
{
  string items[3] = { "wolf", "goat", "cabbage" }, which_cross;
  if (bit(s,me) == bit(t,me)) return "";  // must cross river
  int cross_with = 0;
  for (int i=0; i<3; i++) {
    if (bit(s,i) != bit(t,i) && bit(s,i)==bit(s,me)) { cross_with++; which_cross = items[i]; }
    if (bit(s,i) != bit(t,i) && bit(s,i)!=bit(s,me)) return "";
  }
  if (cross_with > 1) return "";
  if (cross_with == 0) return "Cross alone";
  else return "Cross with " + which_cross;
}

// helper function to see if current state of bitset is safe
bool isSafe(bitset<4>currentState) {
    // if wolf is with goat but me and not around, return false
    if (currentState[wolf] == currentState[goat] && currentState[wolf] != currentState[me]) {
        return false;
    }
    // if goat with cabbage but me is not around, return false
    if (currentState[goat] == currentState[cabbage] && currentState[goat] != currentState[me]) {
        return false;
    }
    // else return true
    return true;
}

// function to build graph
void build_graph(void)
{

    // initialize bit set of 4, with 0's as default and push onto queue
  bitset<4> stateNow(0);
  queue<bitset<4>> toVisit;
  toVisit.push(stateNow);

    // while integer size of stateNow is less or equal to 15
  while ((int)stateNow.to_ulong() <= 15) {
    // assign current state to front of the queue, then pop the queue
    stateNow = toVisit.front();
    toVisit.pop();
    // integer size of current state, return if equal to 15
    int stateInt = (int)stateNow.to_ulong();
    if (stateInt == 15) {
        return;
    }
    else {
      // loop through wolf to me
      for (int bit = wolf; bit <= me; bit++) {
        // create temporary bitset with current state
        bitset<4> stateNext(stateInt);
        stateNext[me].flip(); // flip me in temporary bitset
        if (bit != me) {
            stateNext[bit].flip(); // flip anything that is not me in temporary bitset
        }
          // if the action of the flip is safe
        if (isSafe(stateNext)) {
        // store next state of integer size, push into visited array
          int nextStateInt = (int)stateNext.to_ulong();
          toVisit.push(stateNext);
          // push into neighbors map and insert into edge label map
          nbrs[stateInt].push_back(nextStateInt);
          edge_label.insert({{stateInt, nextStateInt}, neighbor_label(stateInt, nextStateInt)});
        }
      }
    }
  }
}