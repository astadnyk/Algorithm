#include <iostream>
#include <string>
#include <limits>
#include <chrono>

using namespace std;

// Вузол однозв'язного списку для зберігання доріг
struct EdgeNode {
    int to;
    int distance;
    int priority;
    EdgeNode* next; // Вказівник на наступну дорогу
};

class Graph {
private:
    int V; // Кількість вершин
    EdgeNode** adj; // Масив вказівників (список суміжності)
    string* indexToCity; // Звичайний динамічний масив для назв міст

public:
    // Конструктор: виділяємо пам'ять
    Graph(int vertices) {
        V = vertices;
        adj = new EdgeNode*[V];
        indexToCity = new string[V];
        
        for (int i = 0; i < V; ++i) {
            adj[i] = nullptr; // Спочатку доріг немає
        }
    }

    // Деструктор: очищуємо пам'ять, щоб не було витоків
    ~Graph() {
        for (int i = 0; i < V; ++i) {
            EdgeNode* curr = adj[i];
            while (curr != nullptr) {
                EdgeNode* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
        delete[] adj;
        delete[] indexToCity;
    }

    // Додавання міста у масив
    void addCity(int index, string name) {
        indexToCity[index] = name;
    }

    // Вивід списку міст
    void showCities() {
        cout << "СПИСОК ДОСТУПНИХ МІСТ:" << endl;
        cout << "\n======================================================" << endl;
        for (int i = 0; i < V; i++) {
            if (indexToCity[i] != "") {
                cout << i << ". " << indexToCity[i] << endl;
            }
        }
        cout << "\n======================================================" << endl;
    }

    // Рекурсивний метод для Пошуку в глибину (DFS)
    void DFSUtil(int v, int target, bool* visited, bool& found) {
        // Якщо кінцеве місто вже знайдено в іншій гілці рекурсії - припиняємо роботу
        if (found) return;

        visited[v] = true; // Позначаємо поточне місто як відвідане
        cout << indexToCity[v];

        // Якщо ми дійшли до цільового міста, зупиняємо обхід
        if (v == target) {
            found = true;
            return;
        }
        cout << " -> ";

        // Перевіряємо всіх сусідів поточного міста
        EdgeNode* curr = adj[v];
        while (curr != nullptr) {
            int nextCity = curr->to;
            if (!visited[nextCity]) {
                DFSUtil(nextCity, target, visited, found); // Рекурсивний крок
            }
            curr = curr->next;
        }
    }

    // Обхід графа методом DFS
    void task1_DFS_Traversal(int start, int end) {
        bool* visited = new bool[V];
        for (int i = 0; i < V; i++) {
            visited[i] = false;
        }
        bool found = false;

        cout << "\n=== ЗАВДАННЯ 1: ОБХІД ГРАФА (Пошук у глибину / DFS) ===" << endl;
        cout << "Порядок відвідування міст: ";
        
        DFSUtil(start, end, visited, found);
        
        cout << "\n======================================================" << endl;

        delete[] visited; // Очищуємо пам'ять
    }

    // Пошук шляху за допомогою BFS
    void task2_BFS_Path(int start, int end) {
        if (start < 0 || start >= V || end < 0 || end >= V) return;

        bool* visited = new bool[V];
        int* parent = new int[V];
        int* queue = new int[V]; // Наша власна черга
        int front = 0, rear = 0;

        for (int i = 0; i < V; i++) {
            visited[i] = false;
            parent[i] = -1;
        }

        // Починаємо з початкового міста
        visited[start] = true;
        queue[rear++] = start;

        bool found = false;

        while (front < rear) {
            int u = queue[front++]; // Беремо перше місто з черги

            if (u == end) {
                found = true;
                break;
            }

            // Перевіряємо всіх сусідів
            EdgeNode* curr = adj[u];
            while (curr != nullptr) {
                int v = curr->to;
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u; // Запам'ятовуємо, звідки прийшли
                    queue[rear++] = v; // Додаємо в чергу
                }
                curr = curr->next;
            }
        }

        cout << "\n=== ЗАВДАННЯ 2: ПОШУК ШЛЯХУ (Пошук у ширину / BFS) ===" << endl;
        if (!found) {
            cout << "Шлях від " << indexToCity[start] << " до " << indexToCity[end] << " не знайдено." << endl;
        } else {
            // Відтворюємо шлях з кінця в початок
            int* path = new int[V];
            int pathLen = 0;
            for (int at = end; at != -1; at = parent[at]) {
                path[pathLen++] = at;
            }

            cout << "Знайдений шлях (найменша кількість міст): ";
            for (int i = pathLen - 1; i >= 0; --i) {
                cout << indexToCity[path[i]] << (i == 0 ? "" : " -> ");
            }
            cout << endl;
            delete[] path;
        }
        cout << "\n======================================================" << endl;

        delete[] visited;
        delete[] parent;
        delete[] queue;
    }

    void addEdge(int u, int v, int dist, int prio) {
        adj[u] = new EdgeNode{v, dist, prio, adj[u]};
        adj[v] = new EdgeNode{u, dist, prio, adj[v]};
    }

    // Алгоритм Дейкстри
    void dijkstra(int start, int end, bool useDistance) {
        if (start < 0 || start >= V || end < 0 || end >= V) {
            cout << "Помилка: Невірний індекс міста!" << endl;
            return;
        }

        // Динамічні масиви
        int* minWeight = new int[V];
        int* parent = new int[V];
        bool* visited = new bool[V];

        // Ініціалізація
        for (int i = 0; i < V; ++i) {
            minWeight[i] = numeric_limits<int>::max();
            parent[i] = -1;
            visited[i] = false;
        }

        minWeight[start] = 0;

        // Основний цикл Дейкстри
        for (int i = 0; i < V - 1; ++i) {
            // Шукаємо невідвідану вершину з мінімальною вагою
            int u = -1;
            int currentMin = numeric_limits<int>::max();
            for (int j = 0; j < V; ++j) {
                if (!visited[j] && minWeight[j] < currentMin) {
                    currentMin = minWeight[j];
                    u = j;
                }
            }

            // Якщо шлях перервався або дійшли до кінця
            if (u == -1 || u == end) break;

            visited[u] = true; // Відмічаємо як відвідану

            // Перевіряємо всіх сусідів через зв'язний список
            EdgeNode* curr = adj[u];
            while (curr != nullptr) {
                int v = curr->to;
                int weight = useDistance ? curr->distance : curr->priority;

                if (!visited[v] && minWeight[u] + weight < minWeight[v]) {
                    minWeight[v] = minWeight[u] + weight;
                    parent[v] = u;
                }
                curr = curr->next;
            }
        }

        // Відновлення та вивід шляху
        if (minWeight[end] == numeric_limits<int>::max()) {
            cout << "Шлях не знайдено!" << endl;
        } else {
            // Масив для збереження шляху
            int* path = new int[V];
            int pathLen = 0;
            
            for (int at = end; at != -1; at = parent[at]) {
                path[pathLen++] = at; // Записуємо шлях з кінця
            }
            cout << "\n=== Критерій: " << (useDistance ? "Відстань (км)" : "Пріоритет дороги") << " ===" << endl;
            cout << "Шлях: ";
            for (int i = pathLen - 1; i >= 0; --i) {
                cout << indexToCity[path[i]] << (i == 0 ? "" : " -> ");
            }
            cout << "\nРезультат: " << minWeight[end] << (useDistance ? " км" : " одиниць пріоритету") << endl;
            delete[] path; // Очищуємо пам'ять
        }

        // Очищуємо пам'ять
        delete[] minWeight;
        delete[] parent;
        delete[] visited;
    }
};

// Структура для зберігання даних про місто для сортування
struct CityInfo {
    string name;
    double longitude; // Довгота (для заходу-сходу)
    int population;   // Населення
};

// 1. Швидке сортування (Quick Sort) за довготою (зростання)
void quickSort(CityInfo arr[], int low, int high) {
    if (low < high) {
        double pivot = arr[high].longitude;
        int i = (low - 1);
        
        for (int j = low; j <= high - 1; j++) {
            if (arr[j].longitude <= pivot) {
                i++;
                // Міняємо місцями
                CityInfo temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        CityInfo temp2 = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp2;
        
        int pi = i + 1;
        
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 2. Сортування Шелла (Shell Sort) за населенням (спадання)
void shellSort(CityInfo arr[], int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i += 1) {
            CityInfo temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap].population < temp.population; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

int main() {
    // 1. Ініціалізація графа (21 вершина)
    Graph g(21);

    // 2. Реєстрація міст (індекс, назва міста)
    g.addCity(0, "Київ");
    g.addCity(1, "Калинівка");
    g.addCity(2, "Коростишів");
    g.addCity(3, "Житомир");
    g.addCity(4, "Звягель");
    g.addCity(5, "Корець");
    g.addCity(6, "Рівне");
    g.addCity(7, "Дубно");
    g.addCity(8, "Радивилів");
    g.addCity(9, "Буськ");
    g.addCity(10, "Львів");
    g.addCity(11, "Золочів");
    g.addCity(12, "Тернопіль");
    g.addCity(13, "Волочиськ");
    g.addCity(14, "Хмельницький");
    g.addCity(15, "Вінниця");
    g.addCity(16, "Умань");
    g.addCity(17, "Одеса");
    g.addCity(18, "Миколаїв");
    g.addCity(19, "Кропивницький");
    g.addCity(20, "Черкаси");

    // 3. Додавання ребер (індекс початку, індекс кінця, відстань, пріоритет)
    g.addEdge(0, 1, 40, 3);
    g.addEdge(0, 20, 200, 2);
    g.addEdge(0, 3, 140, 1);
    g.addEdge(0, 17, 475, 1);
    g.addEdge(1, 2, 105, 1);
    g.addEdge(1, 16, 178, 1);
    g.addEdge(2, 3, 34, 1);
    g.addEdge(3, 15, 128, 1);
    g.addEdge(3, 14, 202, 2);
    g.addEdge(3, 4, 87, 1);
    g.addEdge(4, 5, 35, 1);
    g.addEdge(5, 6, 66, 1);
    g.addEdge(6, 7, 47, 1);
    g.addEdge(6, 14, 194, 2);
    g.addEdge(7, 12, 106, 1);
    g.addEdge(7, 8, 52, 1);
    g.addEdge(8, 11, 49, 1);
    g.addEdge(8, 9, 61, 1);
    g.addEdge(9, 10, 53, 1);
    g.addEdge(10, 11, 68, 2);
    g.addEdge(11, 12, 60, 2);
    g.addEdge(12, 13, 49, 1);
    g.addEdge(13, 14, 66, 1);
    g.addEdge(14, 15, 121, 1);
    g.addEdge(15, 16, 160, 1);
    g.addEdge(16, 20, 184, 2);
    g.addEdge(16, 19, 165, 1);
    g.addEdge(16, 17, 272, 1);
    g.addEdge(17, 18, 133, 2);
    g.addEdge(18, 19, 183, 2);
    g.addEdge(19, 20, 129, 2);

    // Відображення списку
    g.showCities();

    int startIdx, endIdx;
    cout << "\nВведіть номер міста ВІДПРАВЛЕННЯ: ";
    cin >> startIdx;
    cout << "Введіть номер міста ПРИБУТТЯ: ";
    cin >> endIdx;

    // Запуск секундоміра
    auto start_time = chrono::high_resolution_clock::now();   

    cout << "\n=== РЕЗУЛЬТАТИ ПОШУКУ ===" << endl;
    g.dijkstra(startIdx, endIdx, true);  // за відстанню
    g.dijkstra(startIdx, endIdx, false); // за типом дороги

    // Виклик Завдання 1 (0 - Київ, 20 - Черкаси)
    g.task1_DFS_Traversal(0, 20);

    // Виклик Завдання 2
    g.task2_BFS_Path(startIdx, endIdx);

    // Виклик Завдання 4
    cout << "\n=== ЗАВДАННЯ 4: СОРТУВАННЯ МІСТ ===" << endl;

    // Створюємо масив з містами, їх довготою та населенням
    CityInfo cities[21] = {
        {"Київ", 30.5234, 2952000}, 
        {"Калинівка", 30.2360, 5000},
        {"Коростишів", 29.0660, 24000}, 
        {"Житомир", 28.6580, 261000},
        {"Звягель", 27.6330, 55000}, 
        {"Корець", 27.1660, 6900},
        {"Рівне", 26.2510, 243000}, 
        {"Дубно", 25.7330, 37000},
        {"Радивилів", 25.2660, 10000}, 
        {"Буськ", 24.6160, 8600},
        {"Львів", 24.0290, 717000}, 
        {"Золочів", 24.9000, 23000},
        {"Тернопіль", 25.5940, 225000}, 
        {"Волочиськ", 26.1660, 18000},
        {"Хмельницький", 26.9870, 274000}, 
        {"Вінниця", 28.4680, 369000},
        {"Умань", 30.2210, 81000}, 
        {"Одеса", 30.7230, 1010000},
        {"Миколаїв", 31.9940, 470000}, 
        {"Кропивницький", 32.2590, 222000},
        {"Черкаси", 32.0590, 269000}
    };

    // 1. Сортуємо із заходу на схід (за довготою)
    quickSort(cities, 0, 20);
    cout << "\nМіста із заходу на схід (Швидке сортування):" << endl;
    for (int i = 0; i < 21; i++) {
        cout << cities[i].name << " (Довгота: " << cities[i].longitude << ")" << endl;
    }

    // 2. Сортуємо за населенням (за спаданням)
    shellSort(cities, 21);
    cout << "\nМіста за кількістю населення (Сортування Шелла):" << endl;
    for (int i = 0; i < 21; i++) {
        cout << cities[i].name << " (" << cities[i].population << " осіб)" << endl;
    }
    cout << "\n======================================================" << endl;

    // Зупинка секундоміра
    auto end_time = chrono::high_resolution_clock::now();

    // Обчислення тривалості у мікросекундах та мілісекундах
    auto duration_micro = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
    auto duration_milli = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    cout << "=== ЗАВДАННЯ 8: ЧАС РОБОТИ ПРОГРАМИ ===" << endl;
    cout << "Час виконання всіх алгоритмів (DFS, BFS, Dijkstra x2, QuickSort, ShellSort):" << endl;
    cout << duration_micro << " мікросекунд (" << duration_milli << " мілісекунд)." << endl;
    cout << "======================================================" << endl;

    return 0;
}
