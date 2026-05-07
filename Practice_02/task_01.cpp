#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;

//const for Deyskter
const double INF = 1e9;
const int NUM_OF_PLACES = 12;

string getName(int index);
void findPathDijkstra(double listOfPoints[NUM_OF_PLACES][NUM_OF_PLACES], int startPoint);
void printResult(double result[NUM_OF_PLACES], int initialPoint);
void findPathWave(double listOfPoints[NUM_OF_PLACES][NUM_OF_PLACES], int startPoint, int endPoint);

int main() {

    double mapPoints[NUM_OF_PLACES][NUM_OF_PLACES] = {
                                                        {0.0, INF, INF, INF, INF, INF, INF, 0.4, INF, INF, INF, INF},
                                                        {INF, 0.0, INF, INF, 0.9, INF, INF, 4.6, 1.0, INF, INF, INF},
                                                        {INF, INF, 0.0, 0.6, INF, INF, 0.5, INF, INF, 0.7, INF, 0.6},
                                                        {INF, INF, 0.6, 0.0, INF, INF, 0.4, INF, INF, 0.5, INF, INF},
                                                        {INF, 0.9, INF, INF, 0.0, INF, INF, INF, INF, 0.6, INF, INF},
                                                        {INF, INF, INF, INF, INF, 0.0, INF, INF, 0.8, 0.6, 0.7, INF},
                                                        {INF, INF, 0.5, 0.4, INF, INF, 0.0, INF, INF, INF, 0.8, INF},
                                                        {0.4, 4.6, INF, INF, INF, INF, INF, 0.0, INF, INF, INF, INF},
                                                        {INF, 1.0, INF, INF, INF, 0.8, INF, INF, 0.0, INF, INF, INF},
                                                        {INF, INF, 0.7, 0.5, 0.6, 0.6, INF, INF, INF, 0.0, INF, INF},
                                                        {INF, INF, INF, INF, INF, 0.7, 0.8, INF, INF, INF, 0.0, INF},
                                                        {INF, INF, 0.6, INF, INF, INF, INF, INF, INF, INF, INF, 0.0}, 
                                                    };

    cout << "Shortest path to locations from Dormitory KPI (Гртожиток КПІ) find by Dijkstra: " << endl;
    
    auto start = chrono::high_resolution_clock::now();
    findPathDijkstra(mapPoints, 0);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> elapsed = end - start;
    cout << "Execution time is " << elapsed.count() << endl;
    cout << endl;

    cout << "Path to locations from Dormitory KPI (Гртожиток КПІ) find by Wave Method: " << endl;
    start = chrono::high_resolution_clock::now();
    for (int i = 1; i < NUM_OF_PLACES; i++) {
        findPathWave(mapPoints, 0, i);
    }
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "Execution time is " << elapsed.count() << endl;

    return 0;
}

//name of points on map based on index
string getName(int index) {
    string pointName;

    switch (index) {
        case 0: 
            pointName = "Dormitory KPI";
            break;
        case 1:
            pointName = "Red Building KNU";
            break;
        case 2: 
            pointName = "St Andrew Church";
            break;
        case 3: 
            pointName = "St. Michael's Golden-Domed Monastery";
            break;
        case 4: 
            pointName = "Golden Gate";
            break;
        case 5: 
            pointName = "Lach Gates";
            break;
        case 6: 
            pointName = "Funicular";
            break;
        case 7: 
            pointName = "Kyivska Politechnika";
            break;
        case 8: 
            pointName = "Fountain on Khreshchatyk";
            break;
        case 9: 
            pointName = "St Sofia Cathedral";
            break;
        case 10: 
            pointName = "National Phylarmony";
            break;
        case 11: 
            pointName = "One Street's Museum";
            break;
        default: pointName = "No such place";
    }

    return pointName;
}

void findPathDijkstra(double listOfPoints[NUM_OF_PLACES][NUM_OF_PLACES], int startPoint) {
    double minPath[NUM_OF_PLACES]; //array of distances
    bool visitedPlaces[NUM_OF_PLACES];

    //Step 1: Initialisation
    //All distances from 0 are infinity
    //For starting point (0) disctance is 0
    for (int i = 0; i < NUM_OF_PLACES; i++) {
        minPath[i] = INF;
        visitedPlaces[i] = false;
    }
    minPath[startPoint] = 0;

    while (true) {
        double minEdge = INF;
        int currentPoint = -1;

        //select next start point (not visited (not check all columns) and with min value)
        for (int item = 0; item < NUM_OF_PLACES; item++) {
            if (!visitedPlaces[item] && minPath[item] <= minEdge) {
                minEdge = minPath[item];
                currentPoint = item;
            }
        }

        //check if all edges is checked (all places is marked as visited)
        if (currentPoint == -1) break;

        //exclude current edge
        visitedPlaces[currentPoint] = true;

        //Step 2: find points with shortest way to neighbours
        //after - go to the closest neighbour
        for (int col = 0; col < NUM_OF_PLACES; col++) {
            if (minPath[col] > listOfPoints[currentPoint][col] + minPath[currentPoint] && 
                    listOfPoints[currentPoint][col] != INF) {
                minPath[col] = listOfPoints[currentPoint][col] + minPath[currentPoint];
            }  
        }
    }

    printResult(minPath, startPoint);

}

void findPathWave(double listOfPoints[NUM_OF_PLACES][NUM_OF_PLACES], int startPoint, int endPoint) {
    //start fro start point and get it number 0
    int waveNumbers[NUM_OF_PLACES];
    int currentPoint = startPoint;

    for (int i = 0; i < NUM_OF_PLACES; i++) {
        waveNumbers[i] = -1;
    }
    int wave = 0;
    waveNumbers[startPoint] = 0;
    bool findNew = true;
    //all neighbours get number 1, its neighbours - 2 etc.
    while(waveNumbers[endPoint] == -1 && findNew) {
        findNew = false;
        
        for(int n = 0; n < NUM_OF_PLACES; n++) {   
            if (waveNumbers[n] == wave) {
            
                for (int col = 0; col < NUM_OF_PLACES; col++) {
                    if ((listOfPoints[n][col] < INF && listOfPoints[n][col] > 0) 
                            && waveNumbers[col] == -1) {
                                waveNumbers[col] = wave + 1;
                                findNew = true;
                    }
                }
        
            }
        }
        wave++;
    }

    //way back
    int curr = endPoint;
    double distance = 0;
    while (curr != startPoint) {
        bool found = false;
        for (int i = 0; i < NUM_OF_PLACES; i++) {
            if (listOfPoints[i][curr] < INF && waveNumbers[i] == waveNumbers[curr] - 1) {
                distance += listOfPoints[i][curr];
                curr = i;
                found = true;
                break;
            }
        }
        if (!found) break;
    }

    if (waveNumbers[endPoint] != -1) cout << "Total distance to " << getName(endPoint) << " is " << distance << endl;
    else cout << "Way to " << getName(endPoint) << "did not find" << endl;
}


void printResult(double result[NUM_OF_PLACES], int initialPoint) {
    for (int i = 0; i < NUM_OF_PLACES; i++) {
        if (result[i] != INF && i != initialPoint) {
            cout << fixed << setprecision(1) << "Shortest distance to " << getName(i) << " is " 
                    << result[i] << " km." << endl;
        }
    }
}
