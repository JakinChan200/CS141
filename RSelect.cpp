#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

int RSELECT(vector<int> A, int n, int k){

    srand(time(NULL));
    //Get a random number to be the index
    int index = (rand() % n);
    int pivot = A[index];

    //Move the pivot to the end of the vector
    int temp = A[n-1];
    A[n-1] = A[index];
    A[index] = temp;
    index = n-1;

    //Partition vector A into two vectors
    vector<int> l;
    vector<int> r;
    for(int i = 0; i < n-1; i++){
        if(A[i] < pivot){
            l.push_back(A[i]);
        }else{
            r.push_back(A[i]);
        }
    }

    if((k-1) == l.size()){
        return pivot;
    }

    if(k <= l.size()){
        return RSELECT(l,l.size(),k);
    }

    return RSELECT(r,r.size(),k-l.size()-1);
}

int DSELECT(vector<int> A, int n, int k){

    //Return the correct answer if only 5 elements left
    if(n <= 5){
        sort(A.begin(), A.end());
        return A[k-1];
    }

    //Split the vector A into groups of 5 and push the median into vector M
    vector<int> M;
    for(int i = 0; i+5 < A.size(); i+=5){
        sort(A.begin()+i, A.begin()+5+i);
        M.push_back(A[i+2]);
    }

    //get the median of the medians
    int pivot = DSELECT(M, M.size(), (M.size()+1)/2);

    //Partition vector A around the pivot.
    vector<int> l;
    vector<int> r;
    for(int i = 0; i < n; i++){
        if(A[i] < pivot){
            l.push_back(A[i]);
        }else {
            r.push_back(A[i]);
        }
    }

    if((k-1) == l.size()){
        return pivot;
    }

    if(k <= l.size()){
        return DSELECT(l,l.size(),k);
    }

    return DSELECT(r,r.size(),k-l.size());
}

int main (int argv, char** argc){
    vector<int> A;
    int numInts = 100000;

    //Get random numbers and push them into vector A
    srand(time(NULL));
    for(int i = 0; i < numInts; i++){
        A.push_back(rand());
    }

    //Run RSelect and output the runtime
    cout << fixed << setprecision(9);
    auto start = chrono::high_resolution_clock::now();
    int answer = RSELECT(A, A.size(), (A.size())/2);
    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    time_taken *= 1e-9;
    cout << "RSELECT Answer: " << answer << " in "; 
    cout << time_taken;
    cout << " sec" << endl;

    //Run RSelect and output the runtime
    auto start1 = chrono::high_resolution_clock::now();
    int answer1 = DSELECT(A, A.size(), (A.size())/2);
    auto end1 = chrono::high_resolution_clock::now();
    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();

    time_taken1 *= 1e-9;
    cout << "DSELECT Answer: " << answer1 << " in "; 
    cout << time_taken1;
    cout << " sec" << endl;

    return 0;
}