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
using namespace std;

// Reflects what each node represents...
// First value units of water in A, second units of water in B
typedef pair<int,int> state;

// Each string in edge_label must be one of the following:
const string actions[] = {"Fill A",
                          "Fill B",
                          "Empty A",
                          "Empty B",
                          "Pour A->B",
                          "Pour B->A"};

// GENERIC -- these shouldn't need to be changed...
map<state, bool> visited;         // have we queued up this state for visitation?
map<state, state> pred;           // predecessor state we came from
map<state, int> dist;             // distance (# of hops) from source node
map<state, vector<state>> nbrs;   // vector of neighboring states

map<pair<state,state>, string> edge_label;

// GENERIC (breadth-first search, outward from source_node)
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

// GENERIC
void print_path(state s, state t)
{
  if (s != t) {
    print_path(s, pred[t]);
    cout << edge_label[make_pair(pred[t], t)] << ": " << "[" << t.first << "," << t.second << "]\n";
  } else {
    cout << "Initial state: " << "[" << s.first << "," << s.second << "]\n";
  }
}

// function to perform action on the state
state performAction(int action, state currentState) {
  // capacity of jug A and B
  int aCapacity = 3; 
  int bCapacity = 4;

  // amount of water when pouring from A to B and B to A
  int pourAtoB = min(currentState.first, bCapacity - currentState.second);
  int pourBtoA = min(currentState.second, aCapacity - currentState.first);
  
  // switch cases from the action provided in the parameter
  switch(action) {
    // fill jug A
    case 0:
      currentState.first = aCapacity;
      break;
    // fill jug B
    case 1:
      currentState.second = bCapacity;
      break;
    // empty jug A
    case 2:
      currentState.first = 0;
      break;
    // empty jug B
    case 3:
      currentState.second = 0;
      break;
    // pour jug A to B
    case 4:
      currentState.second += pourAtoB;
      currentState.first -= pourAtoB;
      break;
    // pour jug B to A
    case 5:
      currentState.first += pourBtoA;
      currentState.second -= pourBtoA;
      break;
  }
  // return state
  return currentState;
}

// function to create a graph
void build_graph(void)
{
  // initialize state with 0,0 and push into the queue
  state stateNow({0,0});
  queue<state>stateQueue;
  stateQueue.push(stateNow);
  // while the queue of the state is not empty
  while (!stateQueue.empty()) {
    // assign current state to the front then pop front
    stateNow = stateQueue.front();
    stateQueue.pop();
    // return if both jugs added together is 5
    if (stateNow.first + stateNow.second == 5) {
      return;
    }
    else {
      // loop through actions
      for (int i = 0 ; i < 6; i++) {
        // perform action of current state and store in temp state
        state stateNext(performAction(i, stateNow));
        // if pair of current state and temporary state is not duplicates
        if (edge_label.find({stateNow, stateNext}) == edge_label.end()) {
          // push actioned state into the queue and insert into the maps of edges and neighbors
          stateQueue.push(stateNext);
          edge_label.insert({{stateNow, stateQueue.back()}, actions[i]});
          nbrs[stateNow].push_back(stateNext);
        }
      }
    }
  }
}