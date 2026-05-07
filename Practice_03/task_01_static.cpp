#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <chrono>

const int MAX_ELEMENTS = 100;
const int YEAR_OF_HISTORY = 6;
const int ANALYSED_YEARS = 3;

struct StackArray
{
    int data[MAX_ELEMENTS];
    int topElement = -1;

    void push(int val) {
        if (topElement < MAX_ELEMENTS - 1) {
            topElement++;
            data[topElement] = val;
        }
    }

    int pop() {
        if (topElement > -1) {
            int returnValue = data[topElement];
            topElement--;
            return returnValue;
        } else {
            return -1;
        }
    }
};

struct Queue
{
    int data[MAX_ELEMENTS];
    int head = 0;
    int tail = 0;
    int quantityOfElements = 0;

    void enqueue(int val) {
        if (quantityOfElements < MAX_ELEMENTS) {
            data[tail] = val;
            tail = (tail + 1) % MAX_ELEMENTS;
            quantityOfElements++;
        }
    }

    int dequeue() {
        if (quantityOfElements == 0) return -1;

        int returnValue = data[head];
        head = (head + 1) % MAX_ELEMENTS;
        quantityOfElements--;
        return returnValue;
    }
};

void copyDataToTable(StackArray& result, StackArray& table);
void generateSixYearData (Queue history[YEAR_OF_HISTORY]);
void analyseHistory(Queue history[YEAR_OF_HISTORY], int year);
void updateHistory(Queue history[YEAR_OF_HISTORY], StackArray report);

int main() {
    int mark = 0;
    int count = 0;
    std::string input;
    StackArray papers;
    std::string errorMessage = "Please input a number from 1 to 100";
    std::cout << "Please input mark for the current paper or input Y for exit" << std::endl;

    while(true) {
        std::cin >> input;
        if (input == "y" || input == "Y") break;

        try {
            mark = std::stoi(input);
            if (mark > 0 && mark <= 100) {
                papers.push(mark);
            } else std::cout << errorMessage << std::endl;
            
        } catch (...) {
            std::cout << errorMessage << std::endl;
        }
    }

    // copy data to report table
    StackArray reportTable;
    copyDataToTable(papers, reportTable);

    // simulate data for 6 year
    Queue sixYearData[YEAR_OF_HISTORY];
    generateSixYearData(sixYearData); //to change 
    auto start = std::chrono::high_resolution_clock::now();
    
    // change data - add current year and remove first
    updateHistory(sixYearData, reportTable);
    auto int1 = std::chrono::high_resolution_clock::now();
    
    // anylyse 3 years
    while (true) {
        int yearToAnalyse;
        std::cout << "Select 3 years to analyse:\n"
                    << " - Select 1 to analyse 1-3 years\n"
                    << " - Select 2 to analyse 2-4 years\n"
                    << " - Select 3 to analyse 3-5 years\n"
                    << " - Select 4 to analyse 4-6 years\n";
        
        std::cin >> yearToAnalyse;
        if (yearToAnalyse == 1 || yearToAnalyse == 2 || yearToAnalyse == 3 || yearToAnalyse == 4) {
            analyseHistory(sixYearData, yearToAnalyse - 1);
            break;
        } else {
            std::cout << "Selected period should be from 1 to 4\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    auto int2 = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::endl;
    std::chrono::duration<double, std::milli> elapsed = end - start - (int2 - int1);
    std::cout << std::fixed << std::setprecision(4) << "Time of program's execution is: " << elapsed.count() << " msec\n";

    return 0;
}

// transfer data after control to reference table
void copyDataToTable(StackArray& result, StackArray& table) {
    int i = 0;

    while (result.topElement > -1) {
        table.push(result.pop());
        i++;
    }

    std::cout << "Transfer to report table is complete" << std::endl;
}

// simulate 6-year data
void generateSixYearData (Queue history[YEAR_OF_HISTORY]) {
    // initialize random
    std::srand(static_cast<unsigned int>(std::time(0)));
    int students;
    int year = 1;

    for (int i = 0; i < YEAR_OF_HISTORY; i++) {
        // ask number of student
        while (true) {
            
            std::cout << '\n' << "Input a number of student in year " << year << ":\n";
            std::cout << "Data for year " << i + 1 << ":" << std::endl;
            std::cin >> students;
            if ((std::cin) && (students >= 1 && students <= 100)) {
                for (int j = 0; j < students; j++) {
                    int randomMark = std::rand() % 100 + 1; 
                    history[i].enqueue(randomMark);
                    
                    std::cout << "Mark of student " << j << " is " << randomMark << '\n';
                }
                year++;
                std::cout << std::endl;
            break;
            } else {
                std::cout << "Number of students should be from 1 to 100\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
}

// add new data to history
void updateHistory(Queue history[YEAR_OF_HISTORY], StackArray report) {
    // move all year to previous year
    for (int i = 0; i < YEAR_OF_HISTORY - 1; i++) {
        history[i] = history[i + 1];
    }

    history[YEAR_OF_HISTORY - 1].head = 0;
    history[YEAR_OF_HISTORY - 1].tail = 0;
    history[YEAR_OF_HISTORY - 1].quantityOfElements = 0;

    while (report.topElement > -1) {
        history[YEAR_OF_HISTORY - 1].enqueue(report.pop());
    }
}

// get analysis
void analyseHistory(Queue history[YEAR_OF_HISTORY], int year) {
    // find average mark for each year
    for (int i = 0; i < ANALYSED_YEARS; i++) {
        Queue currentYear = history[year + i];
        
        if (currentYear.quantityOfElements == 0) {
            std::cout << "Year " << year + i + 1 << " is empty." << std::endl;
            continue;
        }

        int sum = 0;

        for (int j = 0; j < currentYear.quantityOfElements; j++) {
            int idx = (currentYear.head + j) % MAX_ELEMENTS;
            sum = sum + currentYear.data[idx];
        }

        double avr = static_cast<double> (sum) / currentYear.quantityOfElements;

        std::cout << "The average mark for year " << year + i + 1 << " is " << avr << std::endl;
    }
}