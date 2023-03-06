#include<iostream>
#include<vector>
#include<queue>
#include<utility>
#include<cmath>
#include<algorithm>
#include<stdlib.h>
#include<time.h>
#include <iomanip>

#define BOARD_SIZE 5

using namespace std;

vector<int> a;

int heuristicValue(const int (&v)[BOARD_SIZE][BOARD_SIZE]){
    int sum = 0,c = 0;
    /*for(int y = 0; y < BOARD_SIZE; y ++){
        for(int x = 0; x < BOARD_SIZE; x ++){
            if(v[y][x]!=0){
                int goalX = (v[y][x]-1)%BOARD_SIZE;
                int goalY = (v[y][x]-1)/BOARD_SIZE;
                sum += (abs(x-goalX) + abs(y-goalY));
            }
        }
    }
    return sum;*/
    for(int y = 0; y < BOARD_SIZE; y ++){
        for(int x = 0; x < BOARD_SIZE; x ++){
            if(v[y][x]!=0){
                int goalX = (v[y][x]-1)%BOARD_SIZE;
                int goalY = (v[y][x]-1)/BOARD_SIZE;
                if(goalX!= x || goalY!=y){
                    c++;
                }
            }
        }
    }
        return c;
}

struct State{
    int elements[BOARD_SIZE][BOARD_SIZE];
    vector <int> path;
    pair<int,int> posOfZero; ///first is X, second is Y

    State(const int (&_elements)[BOARD_SIZE][BOARD_SIZE],vector<int> _path, pair<int,int> _posOfZero){
        for(int y = 0; y < BOARD_SIZE; y ++){
            for(int x = 0; x < BOARD_SIZE; x ++){
                this->elements[y][x] = _elements[y][x];
            }
        }
        this->path = _path;
        this->posOfZero = _posOfZero;
    }

    void operator= (State s) {
        for(int y = 0; y < BOARD_SIZE; y ++){
            for(int x = 0; x < BOARD_SIZE; x ++){
                this->elements[y][x] = s.elements[y][x];
            }
        }
        this->posOfZero = s.posOfZero;
        this->path = s.path;
    }
    void printState(){
        cout <<"*******************" << endl;
        for(int y = 0; y < BOARD_SIZE; y ++){
            for(int x = 0; x < BOARD_SIZE; x ++){
                if(this->elements[y][x] == 0){
                    cout << left << setw(3) << "_";
                }else{
                    cout << left << setw(3) << this->elements[y][x];
                }
            }
            cout << endl;
        }
        cout <<"*******************" << endl;
    }

    bool operator<(const State &s1)const{
        return false;
    }
};

bool areDifferent(State currentState, vector< vector<int> >goalState){
    for(int y = 0; y < BOARD_SIZE; y ++){
        for(int x = 0; x < BOARD_SIZE; x ++){
            if(currentState.elements[y][x] != goalState[y][x]){
                return true;
            }
        }
    }
    return false;
}

int main(){
    srand(time(NULL));
    int posOfZeroX = BOARD_SIZE-1, posOfZeroY = BOARD_SIZE-1, goalNum = 1;
    int currentState[BOARD_SIZE][BOARD_SIZE], goalState[BOARD_SIZE][BOARD_SIZE];
    char dir[4] = {'D','U','R','L'};
    int dY[4] = {-1, 1, 0, 0};
    int dX[4] = { 0, 0,-1, 1};
    ///GENERATING THE GOAL STATE
    for(int y = 0; y < BOARD_SIZE; y ++){
        for(int x = 0; x < BOARD_SIZE; x ++){
            if(goalNum > BOARD_SIZE*BOARD_SIZE-1){
                goalNum = 0;
            }
            goalState[y][x] = goalNum;
            goalNum++;
        }
    }

    int howToInput;
    cout << "Input 1 for manual input and 2 for auto generated: " << endl;
    cin >> howToInput;
    if(howToInput == 1){
        ///MANUAL INPUT
        for(int y = 0; y < BOARD_SIZE; y ++){
            for(int x = 0; x < BOARD_SIZE; x ++){
                cin >> currentState[y][x];
            }
        }
        cin >> posOfZeroX >> posOfZeroY;
    }else if(howToInput == 2){
        ///RANDOM GENERATING START STATE
        int steps = rand()%40 + 20;
        cout << "This puzzle will be randomized " <<  steps << " times!" << endl;
        for(int y = 0; y < BOARD_SIZE; y ++){
            for(int x = 0; x < BOARD_SIZE; x ++){
                currentState[y][x] = goalState[y][x];
            }
        }
        for(int i = 0; i < steps;){
            int idx = rand()%4;
            int newPosOfZeroX = posOfZeroX + dX[idx];
            int newPosOfZeroY = posOfZeroY + dY[idx];
            if(newPosOfZeroX >= 0 && newPosOfZeroX < BOARD_SIZE && newPosOfZeroY >= 0 && newPosOfZeroY < BOARD_SIZE){
                currentState[posOfZeroY][posOfZeroX] = currentState[newPosOfZeroY][newPosOfZeroX];
                currentState[newPosOfZeroY][newPosOfZeroX] = 0;
                posOfZeroX = newPosOfZeroX;
                posOfZeroY = newPosOfZeroY;
                i++;
            }
        }
    }else{
        cout << "Invalid choice!" << endl;
        return 1;
    }

    ///PRINTING THE GOAL STATE
    cout << "GOAL: " << endl;
    cout << "*******************" << endl;
    for(int y = 0; y < BOARD_SIZE; y ++){
        for(int x = 0; x < BOARD_SIZE; x ++){
            if(goalState[y][x] == 0){
                cout << left << setw(3) << "_";
                continue;
            }
            cout << left << setw(3) << goalState[y][x];
        }
        cout << endl;
    }
    cout << "*******************" << endl;

    State initial(currentState, {}, {posOfZeroX, posOfZeroY});

    priority_queue<pair<int, State>> open;
    cout << "INITIAL: " << endl;
    initial.printState();
    open.push({-(heuristicValue(currentState)+initial.path.size()),initial});
    State curr = open.top().second;
    while(heuristicValue(curr.elements)){
        for(int i = 0; i < 4; i ++){
            int newX = curr.posOfZero.first + dX[i];
            int newY = curr.posOfZero.second + dY[i];
            if(newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE){
                State newState = curr;
                newState.elements[curr.posOfZero.second][curr.posOfZero.first] = newState.elements[newY][newX];
                newState.elements[newY][newX] = 0;
                newState.posOfZero = {newX, newY};
                newState.path.push_back(dir[i]);
                open.push({-(heuristicValue(newState.elements)+newState.path.size()),newState});
            }
        }
        open.pop();
        curr = open.top().second;
    }
    ///PRINT THE SOLUTION
    string fullDirectionName[4] = {"down", "up", "right", "left"};
    for(int i = 0; i < curr.path.size(); i ++){
        auto itr = find(dir, dir + 4, curr.path[i]);
        int idx = distance(dir, itr);
        initial.elements[initial.posOfZero.second][initial.posOfZero.first] =
        initial.elements[initial.posOfZero.second+dY[idx]][initial.posOfZero.first+dX[idx]];
        initial.elements[initial.posOfZero.second+dY[idx]][initial.posOfZero.first+dX[idx]] = 0;
        initial.posOfZero = {initial.posOfZero.first+dX[idx],initial.posOfZero.second+dY[idx]};
        cout << "Step: " << i+1 << " - Move " << fullDirectionName[idx] << "!" << endl;
        initial.printState();
        cout << endl;
    }

return 0;
}
