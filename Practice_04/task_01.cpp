//Скласти список дат народження хлопців потоку для можливості в подальшому
//вручення їм смаколиків на день Козацтва, а для того щоб зберегти це в таємниці
//згенерувати відповідну хеш-таблицю.
#include <iostream>
#include <cstring>

const int TABLE_SIZE = 17;
const int MAX_WORD_SIZE = 25;
const int MAX_RESULT_SIZE = 100;

struct HashBirthdayGift
{
    /* data */
    int date;
    char gift[MAX_WORD_SIZE];
    HashBirthdayGift* next;

    int hashFunctionDivision (int key) {
        //h(k) = k mod m
        return key % TABLE_SIZE;
    }

    int hashFunctionMult (int key) {
        //h (K) = [M * ((C * K) mod 1)]
        //[] -> auto when double -> int
        double c = 0.618033; //Golden Ratio Constant: 
        double ck = (double) key * c;
        double ckmod = ck - (int) ck;

        return (int) (TABLE_SIZE * ckmod);
    }

    //add new item
    void insert(HashBirthdayGift* table[], int date, char* gift) {
        int index = hashFunctionDivision(date);

        HashBirthdayGift* newItem = new HashBirthdayGift();
        newItem->date = date;
        strcpy(newItem->gift, gift);
        
        //new node point to head of the current list for this index
        newItem->next = table[index];

        //new node become a head
        table[index] = newItem;
    }

    char* findGift(HashBirthdayGift* table[], int date) {

        int index = hashFunctionDivision(date);
        HashBirthdayGift* current = table[index];
        char result[MAX_RESULT_SIZE] = "\0";

        while (current != NULL) 
        {
            //in case of collision -> get all result
            if (current->date == date) {
                strcat(result, current->gift);
                strcat(result, " ");
            } 
            current = current->next;
        }

        if (result == "\0") return "Gift not found";
        return result;
    }
};

struct ClosedHashBirthdayGift
{
    /* data */    
    int date;
    char gift[MAX_WORD_SIZE];
    bool isOccupied = false;
    bool isDeleted = false; // !!!!! Important for deletion

    int hashFunctionDivision (int key) {
        //h(k) = k mod m
        return key % TABLE_SIZE;
    }

    void insertLinear(ClosedHashBirthdayGift table[], int date, char* gift) {
        //idx = (hash(x) + ik) mod N    
        int h = hashFunctionDivision(date);
        int i = 0;
        
        // 
        while (table[(h + i) % TABLE_SIZE].isOccupied && i < TABLE_SIZE) {
            i++;
        }

        // Що ми маємо зробити після того, як знайшли індекс (h + i) % TABLE_SIZE?
        if (i < TABLE_SIZE) {
            int index = (h + i) % TABLE_SIZE;
            table[index].date = date;
            strcpy(table[index].gift, gift);
            table[index].isOccupied = true;
            table[index].isDeleted = false;

        } else {
            std::cout << "Table is full.\n";    
        }   
    }

    void insertQuad(ClosedHashBirthdayGift table[], int date, char* gift) {
        //idx = (hash(x) + ik) mod N    
        int h = hashFunctionDivision(date);
        int i = 0;
        
        // 
        while (table[(h + i * i) % TABLE_SIZE].isOccupied && i < TABLE_SIZE) {
            i++;
        }

        // Що ми маємо зробити після того, як знайшли індекс (h + i * i) % TABLE_SIZE?
        if (i < TABLE_SIZE) {
            int index = (h + i * i) % TABLE_SIZE;
            table[index].date = date;
            strcpy(table[index].gift, gift);
            table[index].isOccupied = true;
            table[index].isDeleted = false;

        } else {
            std::cout << "Table is full.\n";    
        }   
    }

    char* searchLinear(ClosedHashBirthdayGift table[], int date) {
        int i = 0;
        int h = hashFunctionDivision(date);
        int index = h;

        while ((table[index].isOccupied || table[index].isDeleted) && i < TABLE_SIZE) {
            if (table[index].isOccupied && table[index].date == date) {
                return table[index].gift;
            }
            i++;
            index = (h + i) % TABLE_SIZE; // наприклад, квадратичне пробування
        }

        return "Item not found";
    }

    char* searchQuad(ClosedHashBirthdayGift table[], int date) {
        int i = 0;
        int h = hashFunctionDivision(date);
        int index = h;

        while ((table[index].isOccupied || table[index].isDeleted) && i < TABLE_SIZE) {
            if (table[index].isOccupied && table[index].date == date) {
                return table[index].gift;
            }
            i++;
            index = (h + i * i) % TABLE_SIZE; // наприклад, квадратичне пробування
        }

        return "Item not found";
    }

    void deleteLinear(ClosedHashBirthdayGift table[], int date) {
        int i = 0;
        int h = hashFunctionDivision(date);
        int index = h;

        while ((table[index].isOccupied || table[index].isDeleted) && i < TABLE_SIZE) {
            if (table[index].isOccupied && table[index].date == date) {
                table[index].isDeleted = true;
                table[index].isOccupied = false;
            }
            i++;
            index = (h + i) % TABLE_SIZE; 
        }
    }

    void deleteQuad(ClosedHashBirthdayGift table[], int date) {
        int i = 0;
        int h = hashFunctionDivision(date);
        int index = h;

        while ((table[index].isOccupied || table[index].isDeleted) && i < TABLE_SIZE) {
            if (table[index].isOccupied && table[index].date == date) {
                table[index].isDeleted = true;
                table[index].isOccupied = false;
            }
            i++;
            index = (h + i * i) % TABLE_SIZE; 
        }
    }
};

int main() {

    //define hash table
    ClosedHashBirthdayGift hashTable[TABLE_SIZE];

    int dayOfBirth[10] = {14102005, 20102005, 05102004, 14102005, 31102005,
                            12102006, 10102005, 27102005, 01102005, 18102005}; //only for test purpose

    hashTable->insertLinear(hashTable, 14102005, (char*)"Chokolade");
    hashTable->insertLinear(hashTable, 20102005, (char*)"Stickers");
    hashTable->insertLinear(hashTable, 05102004, (char*)"Coffee");
    hashTable->insertLinear(hashTable, 14102005, (char*)"Cookies");
    hashTable->insertLinear(hashTable, 31102005, (char*)"Notebook");
    hashTable->insertLinear(hashTable, 12102006, (char*)"Honey");
    hashTable->insertLinear(hashTable, 10102005, (char*)"Cup");
    hashTable->insertLinear(hashTable, 27102005, (char*)"Tea");
    hashTable->insertLinear(hashTable, 01102005, (char*)"Candies");
    hashTable->insertLinear(hashTable, 18102005, (char*)"Keychain");

    std::cout << "Hash table is completed" << '\n';

    std::cout << '\n' << "Find gift for each day of birth" << '\n';
    for (int i = 0; i < 10; i++) {
        std::cout << '\t' << "for " << dayOfBirth[i] << " the gift is " << hashTable->searchLinear(hashTable, dayOfBirth[i]) << '\n';
    }

    std::cout << '\n' << "Delete all elements" << '\n';
    for (int i = 0; i < 10; i++) {
        hashTable->deleteLinear(hashTable, dayOfBirth[i]);
    }

    //define hash table
    ClosedHashBirthdayGift hashQuadTable[TABLE_SIZE];
    
    hashQuadTable->insertQuad(hashQuadTable, 14102005, (char*)"Chokolade");
    hashQuadTable->insertQuad(hashQuadTable, 20102005, (char*)"Stickers");
    hashQuadTable->insertQuad(hashQuadTable, 05102004, (char*)"Coffee");
    hashQuadTable->insertQuad(hashQuadTable, 14102005, (char*)"Cookies");
    hashQuadTable->insertQuad(hashQuadTable, 31102005, (char*)"Notebook");
    hashQuadTable->insertQuad(hashQuadTable, 12102006, (char*)"Honey");
    hashQuadTable->insertQuad(hashQuadTable, 10102005, (char*)"Cup");
    hashQuadTable->insertQuad(hashQuadTable, 27102005, (char*)"Tea");
    hashQuadTable->insertQuad(hashQuadTable, 01102005, (char*)"Candies");
    hashQuadTable->insertQuad(hashQuadTable, 18102005, (char*)"Keychain");

    std::cout << '\n' << "Find gift for each day of birth" << '\n';
    for (int i = 0; i < 10; i++) {
        std::cout << '\t' << "for " << dayOfBirth[i] << " the gift is " << hashQuadTable->searchQuad(hashQuadTable, dayOfBirth[i]) << '\n';
    }

    return 1;
}


