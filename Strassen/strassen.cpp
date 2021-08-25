#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;

vector<vector<int>> matrixSubtraction(vector<vector<int>> A, vector<vector<int>> B, int n){
    vector<vector<int>> C(n, vector<int> (n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

vector<vector<int>> matrixAddition(vector<vector<int>> A, vector<vector<int>> B, int n){
    vector<vector<int>> C(n, vector<int> (n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

vector<vector<int>> Strassen(vector<vector<int>> A, vector<vector<int>> B, int n){
    if(n == 1){
        vector<vector<int>> C{{A[0][0] * B[0][0]}};
        return C;
    }else if(n == 2){
        int S1 = B[0][1] - B[1][1];
        int S2 = A[0][0] + A[0][1];
        int S3 = A[1][0] + A[1][1];
        int S4 = B[1][0] - B[0][0];
        int S5 = A[0][0] + A[1][1];
        int S6 = B[0][0] + B[1][1];
        int S7 = A[0][1] - A[1][1];
        int S8 = B[1][0] + B[1][1];
        int S9 = A[0][0] - A[1][0];
        int S10 = B[0][0] + B[0][1];

        int P1 = A[0][0] * S1;
        int P2 = S2 * B[1][1];
        int P3 = S3 * B[0][0];
        int P4 = A[1][1] * S4;
        int P5 = S5 * S6;
        int P6 = S7 * S8;
        int P7 = S9 * S10;

        vector<vector<int>> C {{P5+P4-P2+P6, P1+P2},
                                {P3+P4, P5+P1-P3-P7}};
        return C;
    }

    int quadrantSize = n/2;
    vector<int> quadVect(quadrantSize, 0);
    vector<vector<int>> A11(quadrantSize, quadVect);
    vector<vector<int>> A12(quadrantSize, quadVect);
    vector<vector<int>> A21(quadrantSize, quadVect);
    vector<vector<int>> A22(quadrantSize, quadVect);
    vector<vector<int>> B11(quadrantSize, quadVect);
    vector<vector<int>> B12(quadrantSize, quadVect);
    vector<vector<int>> B21(quadrantSize, quadVect);
    vector<vector<int>> B22(quadrantSize, quadVect);

    for(int i = 0; i < quadrantSize; i++){
        for(int j = 0; j < quadrantSize; j++){
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j+quadrantSize];
            A21[i][j] = A[i+quadrantSize][j];
            A22[i][j] = A[i+quadrantSize][j+quadrantSize];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j+quadrantSize];
            B21[i][j] = B[i+quadrantSize][j];
            B22[i][j] = B[i+quadrantSize][j+quadrantSize];
        }
    }

    vector<vector<int>> S1 = matrixSubtraction(B12, B22, quadrantSize);
    vector<vector<int>> S2 = matrixAddition(A11, A12, quadrantSize);
    vector<vector<int>> S3 = matrixAddition(A21, A22, quadrantSize);
    vector<vector<int>> S4 = matrixSubtraction(B21, B11, quadrantSize);
    vector<vector<int>> S5 = matrixAddition(A11, A22, quadrantSize);
    vector<vector<int>> S6 = matrixAddition(B11, B22, quadrantSize);
    vector<vector<int>> S7 = matrixSubtraction(A12, A22, quadrantSize);
    vector<vector<int>> S8 = matrixAddition(B21, B22, quadrantSize);
    vector<vector<int>> S9 = matrixSubtraction(A11, A21, quadrantSize);
    vector<vector<int>> S10 = matrixAddition(B11, B12, quadrantSize);

    vector<vector<int>> P1 = Strassen(A11, S1, quadrantSize);
    vector<vector<int>> P2 = Strassen(S2, B22, quadrantSize);
    vector<vector<int>> P3 = Strassen(S3, B11, quadrantSize);
    vector<vector<int>> P4 = Strassen(A22, S4, quadrantSize);
    vector<vector<int>> P5 = Strassen(S5, S6, quadrantSize);
    vector<vector<int>> P6 = Strassen(S7, S8, quadrantSize);
    vector<vector<int>> P7 = Strassen(S9, S10, quadrantSize);

    vector<vector<int>> C11 = matrixAddition(matrixSubtraction(matrixAddition(P5, P4, quadrantSize), P2, quadrantSize), P6, quadrantSize);
    vector<vector<int>> C12 = matrixAddition(P1, P2, quadrantSize);
    vector<vector<int>> C21 = matrixAddition(P3, P4, quadrantSize);
    vector<vector<int>> C22 = matrixSubtraction(matrixSubtraction(matrixAddition(P5, P1, quadrantSize), P3, quadrantSize), P7, quadrantSize);

    vector<vector<int>> C(n, vector<int> (n, 0));
    for(int i = 0; i < quadrantSize; i++){
        for(int j = 0; j < quadrantSize; j++){
            C[i][j] = C11[i][j];
            C[i][j+quadrantSize] = C12[i][j];
            C[i+quadrantSize][j] = C21[i][j];
            C[i+quadrantSize][j+quadrantSize] = C22[i][j];
        }
    }
    return C;
}

int main (int argv, char** argc){
    srand(time(NULL));
    //base^exponent = the size of the matrices
    //Feel free to change the exponent to any value greater than 0
    int exponent = 2;
    int base = 2;
    int vectorSize = base;

    if(vectorSize != 2 || exponent <= 0){
        cout << "The base needs to be a 2, and the exponent must be greater than 0" << endl;
        return 0;
    }
    for(int i = 1; i < exponent; i++){
        vectorSize *= base;
    }

    //Creating + printing vector A
    cout << "Matrix A: " << endl;
    vector<vector<int>> A(vectorSize, vector<int> (vectorSize, 0));
    for(int i = 0; i < vectorSize; i++){
        for(int j = 0; j < vectorSize; j++){
            int temp = rand() % 10;
            A[i][j] = temp;
            cout << temp << " ";
        }
        cout << endl;
    }
    cout << endl;

    //Creating + printing vector B
    cout << "Matrix B: " << endl;
    vector<vector<int>> B(vectorSize, vector<int> (vectorSize, 0));
    for(int i = 0; i < vectorSize; i++){
        for(int j = 0; j < vectorSize; j++){
            int temp = rand() % 10;
            B[i][j] = temp;
            cout << temp << " ";
        }
        cout << endl;
    }
    cout << endl;

    //Calling Strassen
    vector<vector<int>> C = Strassen(A, B, vectorSize);
    //Printing the Final Matrix
    cout << "Final Matrix: " << endl;
    for(int i = 0; i < vectorSize; i++){
        for(int j = 0; j < vectorSize; j++){
            cout << C[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}

/*
My testing Strategy wasn't exactly very rigorous, but it was as follows:
My main function generates random integers to fill Matrix A and Matrix B.
Then it runs Strassen to find the answer matrix and prints it.
Then I cross referenced the answers with an online calculator.
    (In hindsight, i should have cross referenced it with the naive method, but then I would have to test 2 algorithms)
Then I repeated this several times with different sized matrices (all a size of a power of 2 as the main function throws an error if it isn't)
You can change the size on line 126.

Below are some examples of my testing (I've cross referenced these to make sure they were correct)
I've stopped at 16x16 as most online calculators do not support bigger sizes, and I figured that if the recursion works at that stage, it would work with larger inputs.

What this testing means:
    It means that the code works at its core for what its supposed to do (Aka it multiplies matrices of size power of 2 filled with elements less than 10 in value)
What this testing doesn't account for
    Don't know the upper limit (limit test)
    I only tested for random numbers less than 10 (Although it should work for larger numbers to an extent if it works with smaller numbers)
    Main is the function that throws out errors, although it probably should be Strassen.
////////////////     2x2 Matrices   //////////////
Matrix A:     
2 8 
0 7 

Matrix B:     
3 4 
5 4 

Final Matrix: 
46 40         
35 28

////////////////     4x4 Matrices   //////////////
Vector A: 
6 1 0 3   
8 5 5 1   
9 0 9 9   
0 4 4 2

Vector B:
4 1 0 1
4 1 5 3
7 7 5 7
2 5 9 7

Final Matrix:
34 22 32 30
89 53 59 65
117 117 126 135
48 42 58 54

////////////////     8x8 Matrices   //////////////
Vector A: 
3 2 5 6 6 2 5 9
0 2 6 5 8 0 4 9
3 4 0 8 1 8 3 4
4 3 5 4 7 2 4 8 
1 8 1 5 4 4 5 4 
9 2 3 2 0 7 8 9 
6 6 8 5 0 6 2 8
6 9 8 2 7 9 9 7 

Vector B:
5 1 6 6 8 3 2 0 
4 3 6 7 1 7 0 0
4 3 5 4 4 8 7 6
1 5 3 8 9 7 2 0
8 7 5 8 0 7 2 0
5 3 8 9 3 3 9 1
3 7 1 2 9 2 2 3
2 4 5 3 9 8 2 2

Final Matrix:
140 173 169 203 232 235 111 65
131 169 146 177 188 233 94 66
104 123 158 208 187 162 110 25
150 163 174 203 205 228 107 60
121 144 151 196 158 184 81 33
144 147 196 202 278 188 140 67
143 137 217 232 239 245 152 76
242 234 287 323 278 309 199 98

////////////////     16x16 Matrices   //////////////
Vector A: 
2 6 5 5 6 8 0 4 5 3 4 4 2 6 6 8 
1 5 0 5 6 0 0 9 7 8 0 5 9 6 5 7 
3 0 1 7 8 2 3 5 2 8 1 2 3 4 6 4 
4 1 6 5 6 7 3 5 5 4 1 8 6 7 9 2 
7 8 9 1 2 9 1 4 3 5 2 0 5 9 2 0 
0 1 2 7 6 6 2 7 4 1 5 3 4 1 9 4 
6 1 9 2 4 4 7 0 9 5 0 0 0 0 0 5 
4 4 4 2 3 0 9 6 8 4 5 4 1 4 3 6 
2 6 5 6 1 0 1 2 3 3 8 0 4 8 7 4 
1 5 8 6 2 0 3 8 3 0 0 4 4 5 8 0 
3 3 4 2 8 9 8 4 0 8 5 4 4 7 2 1 
1 1 8 4 0 7 8 9 5 0 9 9 3 3 7 5 
6 1 2 5 5 0 9 2 2 5 5 6 0 6 5 4 
6 9 0 3 7 3 9 7 1 8 4 6 6 1 9 3 
3 5 3 5 6 6 6 2 5 9 6 1 1 2 3 9 
6 7 4 7 8 7 9 0 6 5 5 3 2 6 5 9 

Vector B:
0 2 0 6 5 0 8 0 2 5 1 5 7 8 3 6 
4 2 8 0 2 5 6 2 0 6 0 1 1 2 1 7 
7 9 1 3 2 7 8 5 8 6 7 7 4 6 7 4 
4 9 2 7 9 8 7 2 9 6 7 0 1 4 9 2
8 5 7 5 2 0 4 0 0 4 6 8 5 4 6 9 
6 3 0 4 5 2 0 6 0 0 8 6 3 2 6 4 
8 7 9 8 0 6 5 9 2 0 3 9 0 2 9 2 
5 5 1 4 9 0 8 9 3 9 5 3 7 9 7 4 
2 9 5 5 8 9 9 1 8 4 1 3 9 0 2 9 
2 2 2 9 3 9 8 0 4 8 8 7 4 7 4 2
7 8 2 6 2 8 7 2 8 6 2 6 1 9 7 9 
3 3 8 8 3 3 2 7 4 7 3 5 6 5 0 0 
9 5 1 7 8 8 2 2 2 5 7 1 9 9 1 0
2 9 1 5 2 3 7 8 1 7 8 8 1 0 0 4 
4 3 7 0 7 0 8 1 5 6 1 9 5 7 9 1 
1 9 1 0 7 1 6 8 2 8 3 8 2 2 4 0 

Final Matrix: 
313 429 238 292 360 279 430 294 257 414 333 407 282 307 342 303
298 403 249 347 416 312 448 270 245 470 340 345 361 354 280 249
256 323 200 307 301 229 367 197 210 343 302 339 245 297 317 207
360 429 275 387 388 303 445 312 286 424 383 443 364 377 368 272
298 349 159 298 278 290 398 253 198 351 340 348 272 306 267 301
313 359 219 269 353 232 357 235 254 335 276 327 266 319 365 235
220 330 172 259 218 265 342 187 218 234 224 315 224 195 269 240
284 414 270 326 291 304 444 298 263 367 243 385 264 291 322 285
261 383 185 248 280 297 410 206 265 368 252 315 199 292 287 249
276 337 221 238 295 233 373 248 243 342 251 278 248 288 295 196
372 375 248 397 257 303 378 304 199 348 384 440 253 337 353 290
393 486 280 373 374 337 457 408 347 419 327 447 311 397 432 278
263 366 261 345 243 256 397 260 236 340 257 398 213 291 323 234
388 367 365 400 359 314 474 290 238 442 314 447 334 438 405 300
310 415 247 339 314 334 438 260 258 379 316 420 242 308 372 301
387 522 337 410 374 378 523 337 298 439 373 517 297 344 437 371
*/