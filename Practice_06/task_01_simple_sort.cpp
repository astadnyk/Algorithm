#include <iostream>
#include <chrono>

struct Ship
{
    /* data */
    char bortNumber[9];
    int quantityOfSailors;
};


void generateData (Ship* data, int count); //for testing purpose
void printData (Ship* data, int qtyForPrint); //for testing purpose

void selectionSort (Ship* data, int count);
void insertionSort (Ship* data, int count);
void quickSort (Ship* data, int count);
void quickSortsupport (Ship* data, int left, int right) ;
void countingSort (Ship* data, int count);
void shellSort (Ship* data, int count);
void modifiedQuickSort (Ship* data, int count);
void modifiedQuickSortsupport(Ship* data, int left, int right);

int main() {
    const int INITIAL_NUMBER_OF_SHIPS = 11000;
    const int ADDITIONAL_SHIPS = 11000;

    Ship originalData[INITIAL_NUMBER_OF_SHIPS];

    std::srand(static_cast<unsigned int>(std::time(0)));
    generateData(originalData, INITIAL_NUMBER_OF_SHIPS);
    //std::cout << "--------ORIGINAL DATA-----------" << std::endl;
    //printData(originalData, INITIAL_NUMBER_OF_SHIPS);
    //std::cout << '\n' << std::endl;
    
    std::cout << "ROUND | Number of ships | Number of addition ships | Task # | Type of sort | Time of sorting" << std::endl;
    for (int sequens = 0; sequens < 5 ; sequens++) {
        //get 3 test data
        Ship testData1[INITIAL_NUMBER_OF_SHIPS];
        for (int i = 0; i < INITIAL_NUMBER_OF_SHIPS; i++) {
            testData1[i] = originalData[i];
        }

        Ship testData2[INITIAL_NUMBER_OF_SHIPS];
        for (int i = 0; i < INITIAL_NUMBER_OF_SHIPS; i++) {
            testData2[i] = originalData[i];
        }

        Ship testData3[INITIAL_NUMBER_OF_SHIPS];
        for (int i = 0; i < INITIAL_NUMBER_OF_SHIPS; i++) {
            testData3[i] = originalData[i];
        }

        //first task: sort data by 3 methods
        auto start = std::chrono::high_resolution_clock::now();

        quickSort(testData1, INITIAL_NUMBER_OF_SHIPS);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << sequens + 1 << " | " << INITIAL_NUMBER_OF_SHIPS << " | " << ADDITIONAL_SHIPS << " | 1 | quick | " << duration.count() << std::endl;
        //std::cout << '\n' << '\n' << "--------SORTED DATA-----------" << std::endl;
        //printData(testData1, INITIAL_NUMBER_OF_SHIPS);

        start = std::chrono::high_resolution_clock::now();

        selectionSort(testData2, INITIAL_NUMBER_OF_SHIPS);
        end = std::chrono::high_resolution_clock::now();

        duration = end - start;
        std::cout << sequens + 1 << " | " << INITIAL_NUMBER_OF_SHIPS << " | " << ADDITIONAL_SHIPS << " | 1 | selection | " << duration.count() << std::endl;

        start = std::chrono::high_resolution_clock::now();

        insertionSort(testData3, INITIAL_NUMBER_OF_SHIPS);
        end = std::chrono::high_resolution_clock::now();

        duration = end - start;
        std::cout << sequens + 1 << " | " << INITIAL_NUMBER_OF_SHIPS << " | " << ADDITIONAL_SHIPS << " | 1 | insertion | " << duration.count() << std::endl;

        //ADD ADDITION DATA
        Ship additionData[ADDITIONAL_SHIPS];
        generateData(additionData, ADDITIONAL_SHIPS);

        int newSize = INITIAL_NUMBER_OF_SHIPS + ADDITIONAL_SHIPS;
        
        Ship newTest1[newSize];
        for (int i = 0; i < newSize; i++) {
            if (i < INITIAL_NUMBER_OF_SHIPS) {
                newTest1[i] = testData1[i];
            } else {
                newTest1[i] = additionData[i - INITIAL_NUMBER_OF_SHIPS];
            }
        }

        Ship newTest2[newSize];
        for (int i = 0; i < newSize; i++) {
            if (i < INITIAL_NUMBER_OF_SHIPS) {
                newTest2[i] = testData2[i];
            } else {
                newTest2[i] = additionData[i - INITIAL_NUMBER_OF_SHIPS];
            }
        }
        
        Ship newTest3[newSize];
        for (int i = 0; i < newSize; i++) {
            if (i < INITIAL_NUMBER_OF_SHIPS) {
                newTest3[i] = testData2[i];
            } else {
                newTest3[i] = additionData[i - INITIAL_NUMBER_OF_SHIPS];
            }
        }


        start = std::chrono::high_resolution_clock::now();
        quickSort(newTest1, INITIAL_NUMBER_OF_SHIPS);
        end = std::chrono::high_resolution_clock::now();

        duration = end - start;
        std::cout << sequens + 1 << " | " << INITIAL_NUMBER_OF_SHIPS << " | " << ADDITIONAL_SHIPS << " | 2 | quick | " << duration.count() << std::endl;

        start = std::chrono::high_resolution_clock::now();
        selectionSort(newTest2, INITIAL_NUMBER_OF_SHIPS);
        end = std::chrono::high_resolution_clock::now();

        duration = end - start;
        std::cout << sequens + 1 << " | " << INITIAL_NUMBER_OF_SHIPS << " | " << ADDITIONAL_SHIPS << " | 2 | selection | " << duration.count() << std::endl;

        start = std::chrono::high_resolution_clock::now();
        insertionSort(newTest3, INITIAL_NUMBER_OF_SHIPS);
        end = std::chrono::high_resolution_clock::now();

        duration = end - start;
        std::cout << sequens + 1 << " | " << INITIAL_NUMBER_OF_SHIPS << " | " << ADDITIONAL_SHIPS << " | 2 | insertion | " << duration.count() << std::endl;

        //RETURN TEST DATA FOR INITITAL TEST
        for (int i = 0; i < newSize; i++) {
            if (i < INITIAL_NUMBER_OF_SHIPS) {
                newTest1[i] = originalData[i];
            } else {
                newTest1[i] = additionData[i - INITIAL_NUMBER_OF_SHIPS];
            }
        }
        for (int i = 0; i < newSize; i++) {
            newTest2[i] = newTest1[i];
            newTest3[i] = newTest1[i];
        }

        start = std::chrono::high_resolution_clock::now();
        quickSort(newTest1, INITIAL_NUMBER_OF_SHIPS);
        end = std::chrono::high_resolution_clock::now();

        duration = end - start;
        std::cout << sequens + 1 << " | " << INITIAL_NUMBER_OF_SHIPS << " | " << ADDITIONAL_SHIPS << " | 3 | quick | " << duration.count() << std::endl;

        start = std::chrono::high_resolution_clock::now();
        modifiedQuickSort(newTest2, INITIAL_NUMBER_OF_SHIPS);
        end = std::chrono::high_resolution_clock::now();

        duration = end - start;
        std::cout << sequens + 1 << " | " << INITIAL_NUMBER_OF_SHIPS << " | " << ADDITIONAL_SHIPS << " | 3 | modified quick | " << duration.count() << std::endl;

        start = std::chrono::high_resolution_clock::now();
        countingSort(newTest3, INITIAL_NUMBER_OF_SHIPS);
        end = std::chrono::high_resolution_clock::now();

        duration = end - start;
        std::cout << sequens + 1 << " | " << INITIAL_NUMBER_OF_SHIPS << " | " << ADDITIONAL_SHIPS << " | 3 | counting | " << duration.count() << std::endl;
    }
    

    return 0;
}

void selectionSort (Ship* data, int count) {

    for (int i = 0; i < count - 1; i++) { //we do not check last element (will be checked in j-loop)
        //int minElement = data[i];
        int minIdx = i;

        for (int j = i + 1; j < count; j++) {
            if (data[j].quantityOfSailors < data[minIdx].quantityOfSailors) {
                //minElement = data[j];
                minIdx = j;
            }    
        }
        //swap elements
        if (minIdx != i) {
            std::swap(data[minIdx], data[i]);
        }
    }
}

void insertionSort (Ship* data, int count) {

    for (int i = 1; i < count; i++) {
        Ship current = data[i];
        int j = i - 1;

        while (j >= 0 && data[j].quantityOfSailors > current.quantityOfSailors) {
            data[j + 1] = data[j];
            j--;
        }

        data[j + 1] = current;
    }
}

void quickSort(Ship* data, int count) {
    if (count <= 1) return;
    
    quickSortsupport(data, 0, count - 1);
}

void quickSortsupport (Ship* data, int left, int right) {
    //stop recursion if q-ty 1
    if (left >= right) return;

    //select element from array
    int pivot = (left + (right - left) / 2); //find pivot value
    int i = left;
    int j = right;

    while (i <= j) {
        while(data[i].quantityOfSailors < data[pivot].quantityOfSailors) i++;
        while(data[j].quantityOfSailors > data[pivot].quantityOfSailors) j--;

        if (i <= j) {
            std::swap(data[i], data[j]);
            i++;
            j--;
        }
    }
    //recursion
    if (left < j) quickSortsupport(data, left, j);
    if (i < right) quickSortsupport(data, i, right);
}

void countingSort (Ship* data, int count) {
    if (count <= 1) return;
    
    //find range k
    int maxValue = data[0].quantityOfSailors;
    for (int i = 1; i < count; i++) {
        if (data[i].quantityOfSailors > maxValue) maxValue = data[i].quantityOfSailors;
    }

    //create a count array
    int* counts = new int[maxValue + 1] {};

    //iterate through the input array to store the count of each element
    for (int i = 0; i < count; i++) {
        counts[data[i].quantityOfSailors]++;
    }

    //calculate cumulative count (prefix sum)
    for (int i = 1; i <= maxValue; i++) {
        counts[i] = counts[i] + counts[i - 1];
    }

    //get result
    Ship* ans = new Ship[count];

    for (int i = count - 1; i >=0; i--) {
        ans[counts[data[i].quantityOfSailors] - 1] = data[i];
        counts[data[i].quantityOfSailors]--;
    }

    for (int i = 0; i < count; i++) {
        data[i] = ans[i];
    }

    delete[] ans;
    delete[] counts;
}

void generateData (Ship* data, int count) {
    
    for (int i = 0; i < count; i++) {  
        data[i].quantityOfSailors = (std::rand() % 50 + 10) * 5; // from 50 to 300 sailors
        std::sprintf(data[i].bortNumber, "SH%06d", std::rand() % 1000000);
    }
}

void printData (Ship* data, int qtyForPrint) {
    for (int i = 0; i < qtyForPrint; i++) {
        std::cout << "Ship#: " << data[i].bortNumber << " Q-ty of sailors: " << data[i].quantityOfSailors << std::endl;
        
    }
}

void modifiedQuickSort (Ship* data, int count) {
    //stop recursion if q-ty 1
    if (count > 1) {
        std::srand(static_cast<unsigned int>(std::time(0)));
        modifiedQuickSortsupport(data, 0, count - 1);
    }
}

void modifiedQuickSortsupport(Ship* data, int left, int right) {
    if (left >= right) return;

    int pivotIdx = left + std::rand() % (right - left + 1);
    int pivotValue = data[pivotIdx].quantityOfSailors;

    int lt = left;   //start of equal
    int gt = right;  //end of equal
    int i = left;

    while (i <= gt) {
        if (data[i].quantityOfSailors < pivotValue) {
            std::swap(data[lt], data[i]);
            lt++;
            i++;
        } else if (data[i].quantityOfSailors > pivotValue) {
            std::swap(data[i], data[gt]);
            gt--;
        } else {
            //if equal - go without changes
            i++;
        }
    }

    modifiedQuickSortsupport(data, left, lt - 1);
    modifiedQuickSortsupport(data, gt + 1, right);
}