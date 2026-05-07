#include <iostream>
#include <chrono>
#include <cstring>
#include <math.h>

const int ALLIANCE_QTY = 20;

struct alliance
{
    char country[15];
    int qtyOfSailors;
};

void radixLSD (alliance* data, int count);
void bucketSort (alliance* data, int count);
void quickSort(alliance* data, int count);
void quickSortsupport (alliance* data, int left, int right);

void generateData (alliance* data, int count);
void printalliance (alliance* data, int count);

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    std::cout << "-------------- GENERATING DATA -------------" << std::endl;
    alliance originalData[ALLIANCE_QTY];
    generateData(originalData, ALLIANCE_QTY);
    printalliance(originalData, ALLIANCE_QTY);
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "------------ BUCKET SORTING DATA -----------" << std::endl;
    bucketSort(originalData, ALLIANCE_QTY);
    printalliance(originalData, ALLIANCE_QTY);
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "------------ RADIX SORTING DATA ------------" << std::endl;
    radixLSD(originalData, ALLIANCE_QTY);
    printalliance(originalData, ALLIANCE_QTY);
    std::cout << "--------------------------------------------" << std::endl;

    return 0;
}

void radixLSD (alliance* data, int count) {
    // find size of the max number
    int maxValue = 0;
    for (int i = 0; i < count; i++) {
        if (data[i].qtyOfSailors > maxValue) maxValue = data[i].qtyOfSailors; 
    }

    // get bins
    static alliance bins[10][ALLIANCE_QTY] = {};
    
    // sort
    int base = 1;
    while (maxValue / base > 0) {
        // append to digits counter
        int countSize[10] = {0};
        for (int j = 0; j < count; j++) {
            int digit = (data[j].qtyOfSailors / base) % 10;
            bins[digit][countSize[digit]] = data[j];
            countSize[digit]++;
        }

        int m = 0;
        for (int i = 0; i < 10; i++) { 
            for (int j = 0; j < countSize[i]; j++) {
                data[m] = bins[i][j];
                m++;
            }
        }
        base *= 10;
    }
}

void bucketSort (alliance* data, int count) {
    alliance buckets[100][ALLIANCE_QTY];
    int quantityInBucket[100] = {0};
    
    // find max value
    int maxValue = data[0].qtyOfSailors;
    for (int i = 1; i < count; i++) {
        if (data[i].qtyOfSailors > maxValue) maxValue = data[i].qtyOfSailors;
    }

    // define bucket size
    int bucketSize = maxValue / 100;
    int nBucket;

    // sort by bucket
    for (int i = 0; i < count; i++) {
        nBucket = nBucket = (int)((double)data[i].qtyOfSailors / (maxValue + 1) * 100);
        buckets[nBucket][quantityInBucket[nBucket]] = data[i];
        quantityInBucket[nBucket]++;
    }

    int m = 0;
    // sort every bucket
    for (int i = 0; i < 100; i++) {
        quickSort(buckets[i], quantityInBucket[i]);
        for (int j = 0; j < quantityInBucket[i]; j++) {
            data[m] = buckets[i][j];
            m++;
        }
    }
}

void quickSort(alliance* data, int count) {
    if (count <= 1) return;
    
    quickSortsupport(data, 0, count - 1);
}

void quickSortsupport (alliance* data, int left, int right) {
    // stop recursion if q-ty 1
    if (left >= right) return;

    // select element from array
    int pivot = (left + (right - left) / 2); //find pivot value
    int i = left;
    int j = right;

    while (i <= j) {
        while(data[i].qtyOfSailors < data[pivot].qtyOfSailors) i++;
        while(data[j].qtyOfSailors > data[pivot].qtyOfSailors) j--;

        if (i <= j) {
            std::swap(data[i], data[j]);
            i++;
            j--;
        }
    }
    // recursion
    if (left < j) quickSortsupport(data, left, j);
    if (i < right) quickSortsupport(data, i, right);
}

void generateData (alliance* data, int count) {
    // initialize random
    for (int i = 0; i < count; i++) {
        std::sprintf(data[i].country, "C%09d", i);
        data[i].qtyOfSailors = (std::rand() % 100000000); 
    }
}

void printalliance (alliance* data, int count) {
    for (int i = 0; i < count; i ++) {
        std::cout << "Country: " << data[i].country << " Q-ty of sailors: " << data[i].qtyOfSailors << std::endl;
    }
}