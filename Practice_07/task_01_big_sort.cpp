#include <iostream>
#include <chrono>
#include <cstdlib>
#include <limits>
#include <cstdio>

const int SIZE_TO_PRINT = 20;
const int TEST_QUANTITY = 5;
const int SIZE_LEO = 44; //limit for int

struct Ship
{
    /* data */
    char bortNumber[9];
    int quantityOfSailors;
};

void fillArmada (Ship* armada, int qtyOfShips);
void printShips(Ship* armada, int qtyOfShips);
void quickSort (Ship* armada, int count);
void quickSortsupport (Ship* data, int left, int right);
void mergeSort (Ship* armada, int count);
void merge (Ship* left, Ship* right, int leftSize, int rightSize, Ship* armada);
void getLeonardo (int* data);
void smoothSort (Ship* armada, int count);
void trinkle (Ship* armada, int root, int forestSize, int* orders, int* lNumbers);
void sift (Ship* armada, int idx, int orderCurrent, int* lNumbers);

int main () {
    std::srand(static_cast<unsigned int>(std::time(0)));
    int counter = 0;
    int totalSize = 0;

    while (true) {
        std::cout << "Input q-ty of armadas" << std::endl;
        std::cin >> counter;

        if ((!std::cin) || counter < 1) {
            std::cout << "Q-ty of armadas should be more than 0" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        break;        
    }

    Ship** fleet = new Ship*[counter];
    int* sizesFleet = new int[counter];

    //fill quantity of ships
    for (int i = 0; i < counter; i++) {
        int shipsInArmada = std::rand() % 200 + 100;
        
        
        sizesFleet[i] = shipsInArmada;
        fleet[i] = new Ship[shipsInArmada]; // Виділяємо пам'ять під кораблі армади
        totalSize += shipsInArmada;
        fillArmada(fleet[i], shipsInArmada);
    }
    std::cout << "Fleet created with " << counter << " armadas." << std::endl;
    
    //header of the table
    std::cout << "Round | Sort | By | Items | ms " << std::endl;

    for (int sequence = 0; sequence < TEST_QUANTITY; sequence++) {
        
        //get copy of data for the test
        Ship** testData1 = new Ship*[counter];
        for (int i = 0; i < counter; i++) {
            testData1[i] = new Ship[sizesFleet[i]];
            for (int j = 0; j < sizesFleet[i]; j++) {
                testData1[i][j] = fleet[i][j];
            }
        }
        
        //------------------------------SORT OF FLEETS------------------------------
        //-------------------Merge Sort-----------------------------------------
        //Test by armada
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < counter; i++) {
            mergeSort(testData1[i], sizesFleet[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << sequence + 1 <<" | merge | armada |" << totalSize << " | " << duration.count() << std::endl;
        
        //printShips(testData1[1], sizesFleet[1]);
        Ship* testDataFullFleet1 = new Ship[totalSize];
        int c = 0;
        for (int i = 0; i < counter; i++) {
            for (int j = 0; j < sizesFleet[i]; j++) {
                testDataFullFleet1[c] = testData1[i][j];
                c++;
            }
        }

        for (int i = 0; i < counter; i++) {
            delete[] testData1[i];
        }
        delete[] testData1;

        start = std::chrono::high_resolution_clock::now();
        
        mergeSort(testDataFullFleet1, totalSize);
        
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << sequence + 1 <<" | merge | full |" << totalSize << " | " << duration.count() << std::endl;

        delete[] testDataFullFleet1;

        //get copy of data for the next test
        Ship** testData2 = new Ship*[counter];
        for (int i = 0; i < counter; i++) {
            testData2[i] = new Ship[sizesFleet[i]];
            for (int j = 0; j < sizesFleet[i]; j++) {
                testData2[i][j] = fleet[i][j];
            }
        }


        //-------------------Quick Sort-----------------------------------------
        start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < counter; i++) {
            quickSort(testData2[i], sizesFleet[i]);
        }

        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << sequence + 1 <<" | quick | armada |" << totalSize << " | " << duration.count() << std::endl;
        
        //printShips(testData2[1], sizesFleet[1]);
        
        Ship* testDataFullFleet2 = new Ship[totalSize];
        c = 0;
        for (int i = 0; i < counter; i++) {
            for (int j = 0; j < sizesFleet[i]; j++) {
                testDataFullFleet2[c] = testData2[i][j];
                c++;
            }
        }

        for (int i = 0; i < counter; i++) {
            delete[] testData2[i];
        }
        delete[] testData2;

        start = std::chrono::high_resolution_clock::now();
        
        quickSort(testDataFullFleet2, totalSize);
        
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << sequence + 1 <<" | quick | full |" << totalSize << " | " << duration.count() << std::endl;

        delete[] testDataFullFleet2;

        //get copy of data for the next test
        Ship** testData3 = new Ship*[counter];
        for (int i = 0; i < counter; i++) {
            testData3[i] = new Ship[sizesFleet[i]];
            for (int j = 0; j < sizesFleet[i]; j++) {
                testData3[i][j] = fleet[i][j];
            }
        }

        //-------------------Smooth Sort-----------------------------------------
        start = std::chrono::high_resolution_clock::now();

        //TO COMPLETE
        for (int i = 0; i < counter; i++) {
            smoothSort(testData3[i], sizesFleet[i]);
        }
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << sequence + 1 <<" | smooth | armada |" << totalSize << " | " << duration.count() << std::endl;

        //printShips(testData3[1], sizesFleet[1]);

        Ship* testDataFullFleet3 = new Ship[totalSize];
        c = 0;
        for (int i = 0; i < counter; i++) {
            for (int j = 0; j < sizesFleet[i]; j++) {
                testDataFullFleet3[c] = testData3[i][j];
                c++;
            }
        }

        for (int i = 0; i < counter; i++) {
            delete[] testData3[i];
        }
        delete[] testData3;

        start = std::chrono::high_resolution_clock::now();
        
        smoothSort(testDataFullFleet3, totalSize);
        
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << sequence + 1 <<" | smooth | full |" << totalSize << " | " << duration.count() << std::endl;

        delete[] testDataFullFleet3;
    }
    

    //free Memory
    for (int i = 0; i < counter; i++) {
        delete[] fleet[i];
    }

    delete[] fleet;
    delete[] sizesFleet;

    return 0;
}

void fillArmada (Ship* armada, int qtyOfShips) {

    for (int i = 0; i < qtyOfShips; i++) {  
        armada[i].quantityOfSailors = (std::rand() % 50 + 10) * 5; // from 50 to 300 sailors
        std::sprintf(armada[i].bortNumber, "SH%06d", std::rand() % 1000000);
    }
}

void printShips (Ship* armada, int qtyOfShips) {
    for (int i = 0; i < SIZE_TO_PRINT; i ++) {
        std::cout << "Ship#: " << armada[i].bortNumber << " Q-ty of sailors: " << armada[i].quantityOfSailors << std::endl;
    }
}

//function from practice 06
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

//merge sort
void mergeSort (Ship* armada, int count) {
    //if count = 1 than end
    if (count <= 1) return;

    //create left and right algorithm
    int leftSize = count / 2;
    int rightSize = count - leftSize;

    Ship* left = new Ship[leftSize];
    Ship* right = new Ship[rightSize];

    //fill left and right
    for (int i = 0; i < leftSize; i++)
        left[i] = armada[i];
    for (int i = 0; i < rightSize; i++)
        right[i] = armada[leftSize + i];

    //Recursive sort
    mergeSort(left, leftSize);
    mergeSort(right, rightSize);

    //merge both side
    merge(left, right, leftSize, rightSize, armada);

    //free memory
    delete[] left;
    delete[] right;

}

void merge (Ship* left, Ship* right, int leftSize, int rightSize, Ship* armada) {
    int n = 0;
    int m = 0;
    int i = 0;

    //sort
    while (n < leftSize && m < rightSize) {
        if (left[n].quantityOfSailors <= right[m].quantityOfSailors) {
            armada[i] = left[n];
            n++;
        } else {
            armada[i] = right[m];
            m++;
        }
        i++;
    }

    //merge addition element is available
    while (n < leftSize) {
        armada[i] = left[n];
        i++;
        n++;
    }

    while (m < rightSize) {
        armada[i] = right[m];
        i++;
        m++;
    }
}

//smooth sort
//get Leonardo numbers
void getLeonardo (int* data) {

    for (int i = 0; i < SIZE_LEO; i++) {
        if (i == 0 || i == 1) data[i] = 1;
        else data[i] = data[i - 1] + data[i - 2] + 1;
    }
}
//main function
void smoothSort (Ship* armada, int count) {
    if (count <= 1) return;

    int leoNumber[SIZE_LEO];
    getLeonardo(leoNumber);

    int order[SIZE_LEO]; //save size of forest
    int forest = 0;
    

    //build the forest
    for (int i = 0; i < count; i++) {
        //add a tree to a forest
        //if previous we have at least 2 trees and L(k) is neighbour L(k-1)
        if (forest >= 2 && (order[forest -2] == order[forest - 1] + 1)) {
            order[forest - 2]++;
            forest--;
        } else {
            //if last tree get order 1 than new tree has order 0
            if (forest >= 1 && (order[forest - 1] == 1)) {
                order[forest] = 0;
            } else {
                order[forest] = 1;
            }
            forest++;
        }

        trinkle (armada, i, forest, order, leoNumber);
    }

    //remove tree and sort from the end
    for (int i = count - 1; i > 0; i--) {
        if (order[forest - 1] <= 1) {
            forest--;
        } else {
            //ger a tree
            int k = order[forest - 1];
            forest--;

            //get left and right tree
            int lTree = i - 1 - leoNumber[k - 2];
            int rTree = i - 1;

            order[forest++] = k - 1;
            trinkle (armada, lTree, forest, order, leoNumber);

            order[forest++] = k - 2;
            trinkle (armada, rTree, forest, order, leoNumber);

        }
    }

}

void trinkle (Ship* armada, int root, int forestSize, int* orders, int* lNumbers) {
    int currentTree = forestSize - 1;

    //check all roots
    while (currentTree > 0) {
        int previousRoot = root - lNumbers[orders[currentTree]];
        //if current number > root of previous tree -> change root
        if (armada[previousRoot].quantityOfSailors > armada[root].quantityOfSailors){
            std::swap(armada[root], armada[previousRoot]);
            root = previousRoot;
            currentTree--;
        } else break;        
    }

    //check root down
    sift(armada, root, orders[currentTree], lNumbers);
}

//check if root greater than child
void sift (Ship* armada, int idx, int orderCurrent, int* lNumbers) {
    while (orderCurrent >= 2) {
        //get right and left child
        //right - next to the root
        int rChild = idx - 1;
        //left child - next to the left (left lNumber[k-1], right lNumber[k-2])
        int lChild = idx - 1 - lNumbers[orderCurrent - 2];

        //find max child
        int maxChild;
        if (armada[lChild].quantityOfSailors > armada[rChild].quantityOfSailors) {
            maxChild = lChild;
        } else maxChild = rChild;

        //check root
        if (armada[idx].quantityOfSailors < armada[maxChild].quantityOfSailors) {
            std::swap(armada[idx], armada[maxChild]);
            idx = maxChild;

            if (maxChild == lChild) {
                orderCurrent = orderCurrent - 1;
            } else {
                orderCurrent = orderCurrent - 2;
            }
        } else break;
    }
}
