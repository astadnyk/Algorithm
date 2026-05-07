#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <fstream> // for file handle
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

//function declarations
void defineMatrixSize(int& n, int& m);
vector<vector<int>> autoFill(int n, int m);
int countZeros(const vector<vector<int>>& inputedNumbers);
void printMatrix(const vector<vector<int>>& numbers);
vector<vector<int>> manualFill(int n, int m);
vector<vector<int>> fileFill();

int main() 
{
    clock_t start, end;
    
    int n, m; //matrix rows and columns
    int counterZero;
    int selectedInput;
    vector<vector<int>> numbers;

    while (true) {
        cout << "Please, select:\n\t1 - for random input of a matrix,\n\t2 - for manual input of a matrix,\n\t3 - for read matrix from file.\n";
        cin >> selectedInput;
        
        if (!(cin) || selectedInput < 1 || selectedInput > 3) {
            cout << "The entered value is not a whole number or not in a range. Please input a correct interface.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        break;
    }
    switch (selectedInput) {
        case 1: {
            defineMatrixSize(n, m);
            start = clock();
            numbers = autoFill(n, m);
            printMatrix(numbers);
            break;
        }
        case 2: {
            defineMatrixSize(n, m);
            numbers = manualFill(n, m);
            start = clock();
            break;
        }
        case 3: {
            start = clock();
            numbers = fileFill();
            break;
        }
    }
    
    counterZero = countZeros(numbers);

    cout << "There is/are " << counterZero << " zero(s) in the matrix\n";

    end = clock();
    
    cout << "Execution time is " << ((double)end - start) / ((double)CLOCKS_PER_SEC) << " seconds.\n";
    //pause console
    cout << "Press ENTER to exit...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}

//function that create dynamic matrix with zero
void defineMatrixSize(int& n, int& m) 
{
    
    while (true) {
        //user's input of matrix's size
        cout << "Input number of rows in matrix (n):\n";
        cin >> n;

        //check if n is int
        if (!(cin) || n <= 0 || n > 100000) {
            cout << "The entered value is not appropriate. Please input the night number of rows.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        break;         
    }
    while (true) {
        //user's input of matrix's size
        cout << "Input number of columns in matrix (m):\n";
        cin >> m;

        //check m is int
        if (!(cin) || m <= 0 || m > 100000) {
            cout << "The entered value is not appropriate. Please input the night number of columns.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        break;
    }

    
}

//count all zeros in matrix
int countZeros(const vector<vector<int>>& inputedNumbers) 
{
    int counter = 0;

    for(const auto& row : inputedNumbers) {
        for(int item : row) {
            if(item == 0) {
                counter++;
            }
        }
    }

    return counter;
}

//fill matrix with random
vector<vector<int>> autoFill(int n, int m) 
{
    const int MIN_VALUE = -10; // for more chances to get 0
    const int MAX_VALUE = 10;

    const int range = MAX_VALUE - MIN_VALUE + 1;

    //Create vector
    //Initialize it with 'n' rows.
    //Each row is initialized as a vector of 'm' integers, all set to 0.
    vector<vector<int>> numbersNew(n, vector<int>(m, 0));
    
    cout << "Successfully created a " << n << "x" << m << " matrix.\n";

    //Seed the random number generator once at the start of the program
    //The cast to unsigned int is good practice
    srand(static_cast<unsigned int>(time(0)));

    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < m; j++) {
            numbersNew[i][j] = rand() % range + MIN_VALUE;
        }
    }

    return numbersNew;
}

//print genarated matrix
void printMatrix (const vector<vector<int>>& numbers)
{
    string filename = "random_generated_matrix.txt";

    //create file
    ofstream outputFile(filename);

    if (outputFile.is_open()) {
        for(const auto& row : numbers) {
            for(int item : row) {
                outputFile << item << " ";
            }
            outputFile << '\n';
        }
        //close file
        outputFile.close();
        cout << "Matrix successfully created and saved to file.\n";
    }
    else {
        cout << "Could not open the file " << filename << '\n';
    }

}

//manual input for matrix
vector<vector<int>> manualFill(int n, int m)
{
    //Create vector
    //Initialize it with 'n' rows.
    //Each row is initialized as a vector of 'm' integers, all set to 0.
    vector<vector<int>> numbersNew(n, vector<int>(m, 0));
    
    cout << "Successfully created a " << n << "x" << m << " matrix.\n";

    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < m; j++) {
            while (true) {
                cout << "Input matrix element:\n";
                cin >> numbersNew[i][j];

                if(!cin) {
                    cout << "The entered value is not a whole number. Please input a whole number.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue; 
                }
                break;
            }
        }
    }
    return numbersNew;
}

//fill a matrix from the file
vector<vector<int>> fileFill()
{
    string filename = "random_generated_matrix.txt";
    string line;

    ifstream inFile(filename);

    vector<vector<int>> numbersNew;

    //file is not open then return empty vector
    if (!inFile.is_open()) {
        cout << "The file" << filename << " is not open. Return empty matrix\n";
        return {};
    }

    //read the file
    while (getline(inFile, line)) {
        stringstream strFromFile(line);
        int item;

        vector<int> currentRow;

        while(strFromFile >> item) {
            currentRow.push_back(item);
        }
        
        if (!currentRow.empty()) {
            numbersNew.push_back(currentRow);
        }
    }
    inFile.close();

    return numbersNew;
}