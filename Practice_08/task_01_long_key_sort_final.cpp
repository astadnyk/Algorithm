#include <iostream>
#include <chrono>
#include <cstring>
#include <math.h>

const int ALLIANCE_QTY = 100000;

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
void generateSortedData (alliance* data, int count, int prc);
void generateRepeatedData (alliance* data, int count, int range);

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Scenario 1
    int testSizes[] = {20000, 40000, 60000, 80000, 100000}; 
    
    std::cout << "--- EXPERIMENT START ---" << std::endl;
    std::cout << "Round | Algorithm | Elements | Time (ms)" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    for (int size : testSizes) {

        alliance* originalData = new alliance[size];
        alliance* workingCopy = new alliance[size];

        generateData(originalData, size);
        
        for (int sec = 0; sec < 5; sec++) {
            std::memcpy(workingCopy, originalData, size * sizeof(alliance));
            auto start = std::chrono::high_resolution_clock::now();
            radixLSD(workingCopy, size);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << sec + 1 << " |  Radix LSD  | " << size << " | " << duration.count() << std::endl;

            std::memcpy(workingCopy, originalData, size * sizeof(alliance));
            start = std::chrono::high_resolution_clock::now();
            bucketSort(workingCopy, size);
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            std::cout << sec + 1 << " | Bucket Sort | " << size << " | " << duration.count() << std::endl;
        }
        
        //clear
        delete[] workingCopy;
        delete[] originalData;
    }
    
    // Scenario 2
    int testSize = 100000;
    
    std::cout << "--- EXPERIMENT START ---" << std::endl;
    std::cout << "Round | Algorithm | Elements | % | Time (ms)" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    int prcTest[] = {20, 40, 60, 80, 100};

    for (int test = 0; test < 5; test++) {
        
        alliance* originalData1 = new alliance[testSize];
        alliance* workingCopy1 = new alliance[testSize];

        generateSortedData(originalData1, testSize, prcTest[test]);
        
        for (int sec = 0; sec < 5; sec++) {
            std::memcpy(workingCopy1, originalData1, testSize * sizeof(alliance));
            auto start = std::chrono::high_resolution_clock::now();
            radixLSD(workingCopy1, testSize);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << sec + 1 << " |  LSD Sort   | " << prcTest[test] << " | " << duration.count() << std::endl;

            std::memcpy(workingCopy1, originalData1, testSize * sizeof(alliance));
            start = std::chrono::high_resolution_clock::now();
            bucketSort(workingCopy1, testSize);
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            std::cout << sec + 1 << " | Bucket Sort | " << prcTest[test] << " | " << duration.count() << std::endl;
        }

        //clear
        delete[] originalData1;
        delete[] workingCopy1;
    }
    
    // Scenario 3
    std::cout << "--- EXPERIMENT START ---" << std::endl;
    std::cout << "Round | Algorithm | Elements | Time (ms)" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    for (int size : testSizes) {

        alliance* originalData2 = new alliance[size];
        alliance* workingCopy2 = new alliance[size];

        generateRepeatedData(originalData2, size, 4);
        
        for (int sec = 0; sec < 5; sec++) {
            std::memcpy(workingCopy2, originalData2, size * sizeof(alliance));
            auto start = std::chrono::high_resolution_clock::now();
            radixLSD(workingCopy2, size);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << sec + 1 << " |  Radix LSD  | " << size << " | " << duration.count() << std::endl;

            std::memcpy(workingCopy2, originalData2, size * sizeof(alliance));
            start = std::chrono::high_resolution_clock::now();
            bucketSort(workingCopy2, size);
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            std::cout << sec + 1 << " | Bucket Sort | " << size << " | " << duration.count() << std::endl;
        }
        
        //clear
        delete[] originalData2;
        delete[] workingCopy2;
    }

    std::cout << "------------------------------------" << std::endl;
    
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
    
    int base = 1;
    // sort
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
    static alliance buckets[100][ALLIANCE_QTY];
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

void generateSortedData (alliance* data, int count, int prc) {
    // initialize random
    for (int i = 0; i < count; i++) {
        std::sprintf(data[i].country, "C%09d", i);
        if (i / count * 100 > prc) data[i].qtyOfSailors = (std::rand() % 100000000);
        else  data[i].qtyOfSailors = i * 200;
    }
}

void generateRepeatedData (alliance* data, int count, int range) {
    // initialize random
    for (int i = 0; i < count; i++) {
        std::sprintf(data[i].country, "C%09d", i);
        data[i].qtyOfSailors = std::rand() % range;
    }
}
