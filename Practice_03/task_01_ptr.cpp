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

struct OneWayList
{
    int data;
    OneWayList* next;
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
        OneWayList* newList = new OneWayList{val, last};
        last = newList;
    }

    int pop() {
        if(!last) return -1;
        int val = last -> data;
        OneWayList* temp = last;
        last = last -> next;
        delete temp;
        return val;
    }
};

struct QueuePtr
{
    OneWayList* head = nullptr;
    OneWayList* tail = nullptr;
    int quantityOfElements = 0;

    ~QueuePtr() {
        while (head != nullptr) {
            dequeue();
        }
    }

    void enqueue(int val) {
        // create new list
        OneWayList* newList = new OneWayList{val, nullptr};

        if (tail == nullptr) {
            // if empty - new list is head and tail
            head = newList;
            tail = newList;
        } else {
            tail->next = newList;
            tail = newList;
        }
        quantityOfElements++;
    }

    int dequeue() {
        if (head == nullptr) return -1;

        OneWayList* temp = head;
        int val = temp->data;
        head = head->next;

        if (head == nullptr) {
            tail = nullptr;
        }

        delete temp;
        quantityOfElements--;
        return val;
    }
};

void copyDataToTable(StackPtr& result, StackPtr& table);
void generateSixYearData (QueuePtr** &history);
void analyseHistory(QueuePtr** history, int year);
void updateHistory(QueuePtr** &history, StackPtr& report);
void freeMemory (QueuePtr** &history);

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
    QueuePtr** sixYearData;
    generateSixYearData(sixYearData);
    auto start = std::chrono::high_resolution_clock::now();
    
    // change data - add current year and remove first
    updateHistory(sixYearData, reportTable);
    auto int1 = std::chrono::high_resolution_clock::now();
    
    // analyse 3 years
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

    freeMemory(sixYearData);

    return 0;
}

// transfer data after control to reference table
void copyDataToTable(StackPtr& result, StackPtr& table) {
    while (result.last != nullptr) {
        table.push(result.pop());
    }

    std::cout << "Transfer to report table is complete" << std::endl;
}

// simulate 6-year data
void generateSixYearData (QueuePtr** &history) {
    // initialize random
    std::srand(static_cast<unsigned int>(std::time(0)));
    int students;
    int year = 1;

    history = new QueuePtr*[YEAR_OF_HISTORY];

    for (int i = 0; i < YEAR_OF_HISTORY; i++) {
        history[i] = new QueuePtr();
        
        // ask number of student
        while (true) {
            std::cout << '\n' << "Input a number of student in year " << year << ":\n";
            std::cout << "Data for year " << i + 1 << ":" << std::endl;
            std::cin >> students;
            
            if ((std::cin) && (students >= 1 && students <= 100)) {
                for (int j = 0; j < students; j++) {
                    int randomMark = std::rand() % 100 + 1; 
                    history[i]->enqueue(randomMark);
                    
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
void updateHistory(QueuePtr** &history, StackPtr& report) {
    // move all year to previous year
    delete history[0];

    for (int i = 0; i < YEAR_OF_HISTORY - 1; i++) {
        history[i] = history[i + 1];
    }

    history[YEAR_OF_HISTORY - 1] = new QueuePtr();
    
    while (report.last != nullptr) {
        history[YEAR_OF_HISTORY - 1]->enqueue(report.pop());
    }
}

// get analysis
void analyseHistory(QueuePtr** history, int year) {
    // find average mark for each year
    for (int i = 0; i < ANALYSED_YEARS; i++) {
        OneWayList* currentYear = history[year + i]->head;
        
        if (currentYear == nullptr) {
            std::cout << "Year " << year + i + 1 << " is empty." << std::endl;
            continue;
        }

        int sum = 0;
        int count = 0;

        while (currentYear != nullptr) {
            sum += currentYear->data;
            count++;
            currentYear = currentYear -> next;
        }

        double avr = static_cast<double> (sum) / count;
        std::cout << "The average mark for year " << year + i + 1 << " is " << avr << std::endl;
    }
}

// free memory
void freeMemory (QueuePtr** &history) {
    if (history == nullptr) return;

    for (int i = 0; i < YEAR_OF_HISTORY; i++) {
        delete history[i]; 
        history[i] = nullptr;
    }

    delete[] history;    
    history = nullptr;

    std::cout << std::endl;
    std::cout << "All memory has been successfully freed." << std::endl;
}