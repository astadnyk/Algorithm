#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <chrono>

const int YEAR_OF_HISTORY = 6;
const int ANALYSED_YEARS = 3;
const int STUDENTS_COUNT = 100000;

struct OneWayList
{
    int data;
    OneWayList* next;
    OneWayList(int d, OneWayList* n) : data(d), next(n) {}
};

struct StackPtr
{
    OneWayList* last = nullptr;

    ~StackPtr() {
        while (last != nullptr) {
            pop(); 
        }
    }

    void push(int val) {
        last = new OneWayList(val, last);
    }

    int pop() {
        if(!last) return -1;
        int val = last -> data;
        OneWayList* temp = last;
        last = last -> next;
        delete temp;
        return val;
    }

    bool isEmpty() const {
        return last == nullptr;
    }
};

void copyDataToTable(StackPtr& result, StackPtr& table);
void generateSixYearData (StackPtr** &history, int students);
void analyseHistory(StackPtr** history, int year);
void updateHistory(StackPtr** &history, StackPtr& report);
void freeMemory (StackPtr** &history);

int main() {
    int mark = 0;
    int count = 0;
    std::string input;
    StackPtr papers;
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
    StackPtr reportTable;
    copyDataToTable(papers, reportTable);

    // simulate data for 6 year
    StackPtr** sixYearData;
    generateSixYearData(sixYearData, STUDENTS_COUNT);
    auto start = std::chrono::high_resolution_clock::now();
    
    // change data - add current year and remove first
    updateHistory(sixYearData, reportTable);
    auto int1 = std::chrono::high_resolution_clock::now();

    // anylyse 3 years
    while (true) {
        int yearToAnalyse;
        std::cout << "Select start year (1-4) to analyse 3-year period:\n"
                    << " - Select 1 to analyse 1-3 years\n"
                    << " - Select 2 to analyse 2-4 years\n"
                    << " - Select 3 to analyse 3-5 years\n"
                    << " - Select 4 to analyse 4-6 years\n";
        
        std::cin >> yearToAnalyse;
        if (yearToAnalyse >= 1 && yearToAnalyse <= 4) {
            analyseHistory(sixYearData, yearToAnalyse - 1);
            break;
        } else {
            std::cout << "Selected period should be from 1 to 4 !\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    auto int2 = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    
    std::cout << std::endl;
    std::chrono::duration<double, std::milli> elapsed = end - start - (int2 - int1);
    std::cout << std::fixed << std::setprecision(4) << "Time of program's execution is: " << elapsed.count() << " msec\n";

    freeMemory(sixYearData);

    return 0;
}

//transfer data after control to reference table
void copyDataToTable(StackPtr& result, StackPtr& table) {
    while (result.last != nullptr) {
        table.push(result.pop());
    }
    std::cout << "Transfer to report table is complete" << std::endl;
}

void generateSixYearData (StackPtr** &history, int students) {
    //initialize random
    std::srand(static_cast<unsigned int>(std::time(0)));
    history = new StackPtr*[YEAR_OF_HISTORY];
    
    for (int i = 0; i < YEAR_OF_HISTORY; i++) {
        history[i] = new StackPtr();

        for (int j = 0; j < students; j++) {
            int randomMark = std::rand() % 100 + 1; 
            history[i] -> push(randomMark);
        }
    }
}

void updateHistory(StackPtr** &history, StackPtr& report) {
    //move all year to previous year
    delete history[0];

    for (int i = 0; i < YEAR_OF_HISTORY - 1; i++) {
        history[i] = history[i + 1];
    }

    //add new year
    history[YEAR_OF_HISTORY - 1] = new StackPtr();

    while(report.last != nullptr) {
        history[YEAR_OF_HISTORY - 1]->push(report.pop());
    }
}

void analyseHistory(StackPtr** history, int year) {
  
    //find average mark for each year
    for (int i = 0; i < ANALYSED_YEARS; i++) {
        OneWayList* currentYear = history[year + i]->last;

        int sum = 0;
        int count = 0;

        while (currentYear != nullptr) {
            sum += currentYear->data;
            count++;
            currentYear = currentYear->next;
        }

        if (count > 0) {
            double avr = static_cast<double> (sum) / count;
            std::cout << "The average mark for year " << year + i + 1 << " is " << avr << std::endl;
        } else {
            std::cout << "Year " << year + i +1 << " is empty." << std::endl;
        }
    }
}

void freeMemory (StackPtr** &history) {
    if (history == nullptr) return;

    for (int i = 0; i < YEAR_OF_HISTORY; i++) {
        delete history[i]; 
    }

    delete[] history;    
    history = nullptr;
    
    std::cout << std::endl;
    std::cout << "All memory has been successfully freed." << std::endl;
}