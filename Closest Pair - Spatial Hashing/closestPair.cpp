/*
 * Name: Alvin Yang
 * Date Submitted: 10/24/22
 * Lab Section: 001
 * Assignment Name: Finding the Closest Pair of Points
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct point
{
    double x;
    double y;
};

/*Implement the following function
  Reads in a file specified by the parameter
  Format of file: #of points on first line
                  remaining (#of points) lines: x-value and y-value of point
                  one point per line
                  x-value and y-value are double-precision values and
                  bounded by the unit square 0.0 <= x,y < 1.0
  Should use "spatial hashing" where the #of cells scales with the #of points
  and find the distance between the closest pair of points which will be
  returned as a double type value
*/

// function to find distance between two points, set shortest distance to newly calculated distance if latter is smaller
void findDistance(double & dis, double & shortestDis, const point & leftCoord, const point & rightCoord) {
  // set distance to a distance the 2 points
  dis = sqrt(pow((leftCoord.x - rightCoord.x), 2) + pow((leftCoord.y - rightCoord.y), 2));  
  // if distance is smaller than the current shortest distance, set shortest distance to value
  if (shortestDis > dis) { 
    shortestDis = dis;
  }
}

// function to compare distance between a point and points in vector
void comparePoints(double & dis, double & shortestDis, const point & currentPoint, const vector<point> & points) {
  // looop through vector
  for (int i = 0; i < points.size(); i++) {
    // check if current point of vector is same as the point
    if (points.at(i).x != currentPoint.x || points.at(i).y != currentPoint.y) {
      findDistance(dis, shortestDis, currentPoint, points.at(i)); // compare distance between two points
    }
  }
}

// function to get adjacent pairs from adjcent cells
void getAdjacentPoints(int startX, int startY, const int & edge, vector<point> & adj, const vector<vector<vector<point>>> & pointVector) {
  // loop through adjcent cells
  for (int i = startX - 1; i <= startX + 1; i++) { // loop left and right
    if (i >= 0 && i < edge) { // check if not out of bound
    // loop down and up
      for (int j = startY - 1; j <= startY + 1; j++) {
        if (j >= 0 && j < edge) { // check if not out of bound
          // loop through coordinates in the square
          for (int k = 0; k < pointVector.at(i).at(j).size(); k++) { 
            adj.push_back(pointVector.at(i).at(j).at(k)); // push in pairs in vector
          }
        }
      } 
    }
  }
}

// function to find the closest pair of coordinates in the file
double closestPair(string filename) {

  ifstream fileObj(filename); // create file obj

  int numCoordinates = 0; // number of coordiantes
  int b = 0; // table size if b x b 
  int xpos = 0; // x coordinate of block in table
  int ypos = 0; // y coordinate of block in table

  fileObj >> numCoordinates; // get first line of numbers, which is number of coordinates

  // use square root to find b with # of coordinates because the result of sqrt times itself it equal to the number of coordinates
  b = sqrt(numCoordinates); 

  // create 3d vector, initialize the first 2 vector size to b, because table size is b x b
  vector<vector<vector<point>>> pointVec(b, vector<vector<point>>(b)); 
   
  point thisPoint; // create a point struct obj

  // while fileObj is still getting x coordinate to point obj
  while (fileObj >> thisPoint.x) {
    fileObj >> thisPoint.y; // get y coordinate to point obj

    xpos = int(floor(thisPoint.x * b)); // hash table x coordinate by multipling x value by b and then flooring it
    ypos = int(floor(thisPoint.y * b)); // hash table y coordinate by multipling y value by b and then flooring it

    pointVec.at(xpos).at(ypos).push_back(thisPoint); // push back obj into hashed x and y position on table
  }

  fileObj.close(); // close the file

  vector<point> adj; // vector to store adjcent pairs
  double shortestDistance = 1; // value for shortest distance, initialize to 1
  double distance = 1; // distance to compare shortest distance with 

  // loop through table
  for (int i = 0; i < b; i++) { // loop x positions
    for (int j = 0; j < b; j++) { // loop y positions
      getAdjacentPoints(i, j, b, adj, pointVec); // get pairs of coordinates from adjacent squares
      for (int k = 0; k < pointVec.at(i).at(j).size(); k++) { // loop through coordinates in each table position
        // compare distance to between pairs in this square with pairs in adjacent squares
        comparePoints(distance, shortestDistance, pointVec.at(i).at(j).at(k), adj); 
      }
      adj.clear(); // clear adjacent pairs
    }
  }

  return shortestDistance; // return the shortest distance
}