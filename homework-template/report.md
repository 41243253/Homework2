# 41243240 
# 41243253

# **Max/Min Heap**

## 解題說明

本題為替定義一個最小優先權佇列的ADT MinPQ寫一個類似於ADT 5.2的抽象類別，寫一個衍生自這個抽象類別的MinHeap並實作所有MinPQ的虛擬函式，最後計算每一個函式的時間複雜度並且和MaxHeap複雜度做是否相同的判斷。

## 程式實作

有使用到的標頭檔以及命名空間使用宣告
```cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>         //取得目前時間來做亂數種子
#include <chrono>        //時間的測量
#include <windows.h>     //取得記憶體的使用資訊
#include <psapi.h>       //配合上面<windows.h> 一起取得記憶體資訊
using namespace std;
using namespace chrono; //用來計時
```

以下是MinHeap程式碼:

抽象類別MinPQ(最小優先權佇列)
```cpp
template <class T>
class MinPQ 
{
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;         //是否為空
    virtual const T& Top() const = 0;         //取得堆積頂部的元素
    virtual void Push(const T&) = 0;          //插入元素
    virtual void Pop() = 0;                   //移除堆積頂部的元素
};
```
MinHeap類別實作MinPQ
```cpp
template <class T>
class MinHeap : public MinPQ<T> 
{
private:
    vector<T> heap;

    //將新插入的元素往上調整
    void HeapifyUp(int index) 
    {
        while (index > 0 && heap[index] < heap[(index - 1) / 2]) 
        {
            swap(heap[index], heap[(index - 1) / 2]);
            index = (index - 1) / 2;
        }
    }

    //將根節點往下調整
    void HeapifyDown(int index) 
    {
        int size = heap.size();
        while (index * 2 + 1 < size) 
        {
            int left = index * 2 + 1;
            int right = index * 2 + 2;
            int smallest = index;

            if (left < size && heap[left] < heap[smallest]) smallest = left;
            if (right < size && heap[right] < heap[smallest]) smallest = right;

            if (smallest != index) 
            {
                swap(heap[index], heap[smallest]);
                index = smallest;
            }
            else break;
        }
    }

public:
    bool IsEmpty() const override 
    {
        return heap.empty(); //檢查vector是否為空
    }

    const T& Top() const override 
    {
        if (heap.empty()) throw runtime_error("Heap is empty");
        return heap[0]; //回傳堆積頂部元素
    }

    void Push(const T& val) override 
    {
        heap.push_back(val);           //加到堆積尾端
        HeapifyUp(heap.size() - 1);    //調整位置維持最小堆積性質
    }

    void Pop() override 
    {
        if (heap.empty()) throw runtime_error("Heap is empty");
        heap[0] = heap.back();         //將最後元素移到堆積頂部
        heap.pop_back();               //移除最尾端的元素
        HeapifyDown(0);                //調整位置維持堆積性質
    }
};
```

以下是MaxHeap程式碼:

抽象類別MaxPQ(最大優先權佇列)
```cpp
template <class T>
class MaxPQ
{
public:
    virtual ~MaxPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};
```
MaxHeap類別實作MaxPQ
```cpp
template <class T>
class MaxHeap : public MaxPQ<T> 
{
private:
    vector<T> heap;

    //將新插入的元素往上調整
    void HeapifyUp(int index)
    {
        while (index > 0 && heap[index] > heap[(index - 1) / 2])
        {
            swap(heap[index], heap[(index - 1) / 2]);
            index = (index - 1) / 2;
        }
    }

    //將根節點往下調整
    void HeapifyDown(int index)
    {
        int size = heap.size();
        while (index * 2 + 1 < size) 
        {
            int left = index * 2 + 1;
            int right = index * 2 + 2;
            int largest = index;

            if (left < size && heap[left] > heap[largest]) largest = left;
            if (right < size && heap[right] > heap[largest]) largest = right;

            if (largest != index) 
            {
                swap(heap[index], heap[largest]);
                index = largest;
            }
            else break;
        }
    }

public:
    bool IsEmpty() const override 
    {
        return heap.empty(); //檢查vector是否為空
    }

    const T& Top() const override 
    {
        if (heap.empty()) throw runtime_error("Heap is empty");
        return heap[0]; //回傳堆積頂部元素
    }

    void Push(const T& val) override 
    {
        heap.push_back(val);           //加到堆積尾端
        HeapifyUp(heap.size() - 1);    //調整位置維持最大堆積性質
    }

    void Pop() override 
    {
        if (heap.empty()) throw runtime_error("Heap is empty");
        heap[0] = heap.back();         //將最後元素移到堆積頂部
        heap.pop_back();               //移除最尾端的元素
        HeapifyDown(0);                //調整位置維持堆積性質
    }
};
```
顯示目前程式使用的記憶體狀態
```cpp
void printMemoryUsage() 
{
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------\n"
        << "Memory Usage Information:\n"
        << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB\n"
        << "----------------------------------------------------------\n";
}
```
測試堆積的效能與各個函式平均耗時，傳入堆積類別、資料向量與標籤名稱(用來識別是最小堆積還是最大堆積)
```cpp
template <typename HeapType>
void TestHeapPerformance(HeapType& heap, const vector<int>& data, const string& label) 
{
    int n = data.size();

    //Push測試與計時
    auto start = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) heap.Push(data[i]);
    auto end = high_resolution_clock::now();
    double pushTime = duration<double, micro>(end - start).count() / n;

    //Top測試與計時
    start = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) heap.Top();
    end = high_resolution_clock::now();
    double topTime = duration<double, micro>(end - start).count() / n;

    //IsEmpty測試與計時
    start = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) heap.IsEmpty();
    end = high_resolution_clock::now();
    double isEmptyTime = duration<double, micro>(end - start).count() / n;

    //Pop測試與計時
    start = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) heap.Pop();
    end = high_resolution_clock::now();
    double popTime = duration<double, micro>(end - start).count() / n;

    //輸出每個函式的平均耗時與記憶體使用資訊
    cout << "(" << label << ") Data size = " << n << endl;
    cout << "Average Push time: \t" << pushTime << " us\n";
    cout << "Average Pop time: \t" << popTime << " us\n";
    cout << "Average Top time: \t" << topTime << " us\n";
    cout << "Average IsEmpty time: " << isEmptyTime << " us\n";
    printMemoryUsage();
    cout << endl;
}
```
主程式，使用者輸入測資筆數後會生成 n 筆亂數資料，再建立最小堆積和最大堆積，用相同測資測試兩種堆積的四個函式（Push、Top、Pop、IsEmpty）效能，最後輸出平均耗時與記憶體使用狀況
```cpp
int main() 
{
    srand(time(nullptr)); //設定亂數種子

    int n;
    cout << "請輸入測試資料筆數 n: ";
    cin >> n;

    vector<int> data(n); //建立長度為 n 的vector來儲存亂數
    for (int i = 0; i < n; ++i) data[i] = rand(); //生成亂數資料

    MinHeap<int> minHeap; //建立最小堆積
    MaxHeap<int> maxHeap; //建立最大堆積

    TestHeapPerformance(minHeap, data, "MinHeap"); //測試最小堆積的效能
    TestHeapPerformance(maxHeap, data, "MaxHeap"); //測試最大堆積的效能

    return 0;
}
```
## 效能分析

1. 時間複雜度：
   - 函式IsEmpty()和Top()在MaxHeap和MinHeap的時間複雜度皆為 $O(1)$。
   - 函式Push()和Pop()在MaxHeap和MinHeap的時間複雜度皆為 $O(log n)$。
3. 空間複雜度：使用vector<T>，因此空間複雜度為 $O(n)$。

## 測試與驗證

### 測試結果

最大堆積與最小堆積的Push函式在不同測資筆數下的耗時

|  測資數量   |    MinHeap(microseconds)    |    MaxHeap(microseconds) |  
|------------|------------------------------|--------------------------|
| $n=100$    |                 1.311        |             1.587        |
| $n=500$    |                0.9038        |            0.6982        |
| $n=1000$   |                0.8295        |            0.8774        |
| $n=2000$   |                0.8287        |            0.7024        |
| $n=3000$   |                0.7365        |            0.5082        | 
| $n=4000$   |                0.8573        |            0.4448        |
| $n=5000$   |               0.47584        |           0.51888        |

最大堆積與最小堆積的Pop函式在不同測資筆數下的耗時

|  測資數量   |    MinHeap(microseconds)    |    MaxHeap(microseconds) |  
|------------|------------------------------|--------------------------|
| $n=100$    |                 1.807        |             1.653        |
| $n=500$    |                1.8684        |             1.544        |
| $n=1000$   |                2.1883        |            2.1458        |
| $n=2000$   |                2.9085        |           2.34775        |
| $n=3000$   |               1.71573        |           1.39873        | 
| $n=4000$   |                1.7223        |           1.45822        |
| $n=5000$   |               1.48032        |           1.62012        |

最大堆積與最小堆積的Top函式在不同測資筆數下的耗時

|  測資數量   |    MinHeap(microseconds)    |    MaxHeap(microseconds) |  
|------------|------------------------------|--------------------------|
| $n=100$    |                 0.122        |             0.087        |
| $n=500$    |                0.0646        |            0.0454        |
| $n=1000$   |                0.0635        |            0.0574        |
| $n=2000$   |               0.05785        |            0.05655       |
| $n=3000$   |             0.0563667        |            0.0369        | 
| $n=4000$   |              0.053425        |            0.0316        |
| $n=5000$   |                0.0324        |           0.03236        |

最大堆積與最小堆積的IsEmpty函式在不同測資筆數下的耗時

|  測資數量   |    MinHeap(microseconds)    |    MaxHeap(microseconds) |  
|------------|------------------------------|--------------------------|
| $n=100$    |                 0.075        |             0.058        |
| $n=500$    |                0.0394        |            0.0298        |
| $n=1000$   |                0.0373        |             0.038        |
| $n=2000$   |                0.0365        |            0.0366        |
| $n=3000$   |             0.0361333        |         0.0261667        | 
| $n=4000$   |               0.03745        |            0.0203        |
| $n=5000$   |               0.02144        |           0.02408        |

### 測試結果示意
![輸出結果](<https://github.com/41243253/example-1/blob/main/%E8%BC%B8%E5%87%BA%E7%B5%90%E6%9E%9C.png>)

### 圖表分析
MinHeap和MaxHeap的函式IsEmpty比較圖

![函式IsEmpty比較圖](<https://github.com/41243253/example-1/blob/main/IsEmpty.png>)

MinHeap和MaxHeap的函式Top比較圖

![函式Top比較圖](<https://github.com/41243253/example-1/blob/main/Top.png>)

MinHeap和MaxHeap的函式Push比較圖

![函式Push比較圖](<https://github.com/41243253/example-1/blob/main/Push.png>)

MinHeap和MaxHeap的函式Pop比較圖

![函式Pop比較圖](<https://github.com/41243253/example-1/blob/main/Pop.png>)

### 結論

1. Push與Pop函式的執行時間與資料筆數呈對數成長，符合 $O(log n)$時間複雜度。
2. Top與IsEmpty函式的時間複雜度 $O(1)$，即使在大量資料下也維持穩定效能。
3. MaxHeap與MinHeap在同樣資料下的效能表現相近，差異只有比較方向(大於、小於)。

# **Binary Search Tree**

## 題目需求
- (a)寫一個程式，從一棵最初為空的二元搜尋樹（BST）開始，並進行 n 筆隨機插入。
使用均勻隨機數產生器來產生要插入的值。
接著，量測最終得到的二元搜尋樹的高度，並將此高度除以log2𝑛對於以下 n 值進行測試：
n=100,500,1000,2000,3000,…,10,000。
- (b)
寫一個 C++ 函式，從二元搜尋樹中刪除鍵值為 k 的節點，並說明這個函式的時間複雜度。

## 解題說明
以下為程式使用的標頭檔  
```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <windows.h>
#include <psapi.h>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;
```
定義節點的結構  
```cpp
struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int val) : key(val), left(nullptr), right(nullptr) {}
};
```
判斷是否為空後根據條件插入節點內容  
```cpp
Node* insert(Node* root, int key) {
    if (!root)
        return new Node(key);
    if (key < root->key)
        root->left = insert(root->left, key);
    else
        root->right = insert(root->right, key);
    return root;
}
```
中序遍歷  
```cpp
void inorder(Node* root, vector<int>& keys) {
    if (!root) return;
    inorder(root->left, keys);
    keys.push_back(root->key);
    inorder(root->right, keys);
}
```
計算樹的高度  
```cpp
int Height(Node* root) {
    if (!root)
        return 0;
    int leftHeight = Height(root->left);
    int rightHeight = Height(root->right);
    return 1 + max(leftHeight, rightHeight);
}
```
找尋最小節點並計算找尋的步驟數  
```cpp
Node* findMin(Node* root, int& steps) {
    while (root && root->left) {
        root = root->left;
        steps++;
    }
    return root;
}
```
判定節點是否為葉節點或是擁有子節點  
```cpp
void nodeType(Node* root, int key) {
    if (!root) return;
    if (key < root->key)
        nodeType(root->left, key);
    else if (key > root->key)
        nodeType(root->right, key);
    else {
        cout << "刪除節點 " << key << "：";
        if (!root->left && !root->right)
            cout << "它是葉節點。\n";
        else if (!root->left || !root->right)
            cout << "它有 1 個子節點。\n";
        else
            cout << "它有 2 個子節點。\n";
    }
}
```
刪除鍵值為k2的節點，並計算比較次數  
```cpp
Node* deleteNode(Node* root, int k, int& steps) {
    if (!root) return nullptr;
    steps++;
    if (k < root->key) {
        root->left = deleteNode(root->left, k, steps);
    }
    else if (k > root->key) {
        root->right = deleteNode(root->right, k, steps);
    }
    else {
        if (!root->left) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (!root->right) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        else {
            int findSteps = 1;
            Node* successor = findMin(root->right, findSteps);
            steps += findSteps;
            root->key = successor->key;
            root->right = deleteNode(root->right, successor->key, steps);
        }
    }
    return root;
}

```
顯示目前程式使用的記憶體狀態
```cpp
void printMemoryUsage() 
{
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------\n"
        << "Memory Usage Information:\n"
        << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB\n"
        << "----------------------------------------------------------\n";
}
```
釋放建立樹的記憶體  
```cpp
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
```
### 以下為主程式的內容  
第一部分為建立n=100, 500, 1000, 2000, 3000, 4000, 5000, 10000 筆資料時樹的高度，及將高度除以log2n後是否趨近於常數值2的結果，並計算在建立這些資料後隨機挑選一個樹裡面的樹值，並計算在刪除後所需要的平均步驟和最壞的步驟數。
```cpp
int main() {
    srand(time(0)); // 初始化亂數種子

    // Part 1：n=100,500,...,10000 建立 BST，隨機刪除並紀錄比較次數
    int n_values[] = { 100, 500, 1000, 2000, 3000, 4000, 5000, 10000 };

    cout << "n\t樹高\tlog2(n)\t\t樹高/log2(n)" << endl;
    cout << "----------------------------------------------------------" << endl;

    vector<int> deleteComparisons;

    for (int i = 0; i < 8; ++i) {
        int n = n_values[i];
        Node* root = nullptr;

        // 插入 n 筆亂數資料
        for (int j = 0; j < n; ++j) {
            int val = rand() % (n * 10) + 1;
            root = insert(root, val);
        }

        // 計算高度與比值
        int height = Height(root);
        double log2n = log2(n);
        double ratio = height / log2n;

        cout << n << "\t" << height << "\t" << log2n << "\t\t" << ratio << endl;

        // 隨機從中序遍歷結果中選一個值做刪除
        vector<int> keys;
        inorder(root, keys);
        int randomIndex = rand() % keys.size();
        int deleteKey = keys[randomIndex];

        int steps = 0;
        root = deleteNode(root, deleteKey, steps);
        deleteComparisons.push_back(steps);

        deleteTree(root);
    }

    // 統計平均和最壞比較次數
    int total = 0, worst = 0;
    for (int c : deleteComparisons) {
        total += c;
        worst = max(worst, c);
    }
    double average = (double)total / deleteComparisons.size();

    cout << "\n刪除操作的平均比較次數: " << average << endl;
    cout << "刪除操作的最壞比較次數: " << worst << endl;
```
第二部分為查看刪除的步驟，並顯示刪除的節點的狀態為何，之後顯示記憶體使用量並刪除樹占用的記憶體。  
```cpp
    // Part 2：建立預設資料，分三步驟刪除
    cout << "\n----------------------------------------------------------" << endl;
    cout << "建立預設資料 [10, 20, 30, 40, 50] 並依序刪除 10, 30, 40。\n";

    Node* root = nullptr;
    int preset[] = { 10, 20, 30, 40, 50 };
    for (int val : preset) {
        root = insert(root, val);
    }

    cout << "初始 BST (中序遍歷)：";
    vector<int> tmp;
    inorder(root, tmp);
    for (int x : tmp) cout << x << " ";
    cout << endl;

    int keysToDelete[] = { 10, 30, 40 };
    for (int key : keysToDelete) {
        nodeType(root, key);
        int steps = 0;
        root = deleteNode(root, key, steps);
        cout << "刪除 " << key << " 後中序遍歷：";
        vector<int> tmp2;
        inorder(root, tmp2);
        for (int x : tmp2) cout << x << " ";
        cout << "\n樹的高度: " << Height(root) << endl;
        cout << "刪除 " << key << " 過程比較次數: " << steps << "\n" << endl;
    }

    printMemoryUsage();
    deleteTree(root);

    return 0;
}
```
## 效能分析
### 二元樹高度分析  
| $n$     | $log₂(n)$  (平均樹高度) | 最壞情況高度 (鏈狀) |
| ----- | ---------------- | ----------- |
| $100$   | ≈ 6.64           | 100         |
| $500$   | ≈ 8.97           | 500         |
| $1000$  | ≈ 9.97           | 1000        |
| $2000$  | ≈ 10.97           | 2000        |
| $3000$  | ≈ 11.55           | 3000        |
| $4000$  | ≈ 11.97           | 4000        |
| $5000$  | ≈ 12.29           | 5000        |
| $10000$ | ≈ 13.29           | 10000       |
### 刪除鍵值k的時間複雜度分析
### Average case
- 刪除節點的時間複雜度，與節點的「高度」有關（因為 BST 是按照高度來搜尋）。  
- 平均情況下（平衡或近似平衡）：節點高度 ≈ $log₂(n)$ ，所以刪除的時間複雜度： $O(log n)$。
### Worst case
- 最差情況下（例如 BST 退化成鏈狀）：節點高度 = n，時間複雜度： $O(n)$ 。  
### 測試結果
![二元搜尋樹示意圖](https://github.com/41243240/Example/blob/main/BST.png)
### 高度除以 $log2(n)$ 圖表
![樹高度除以log2n](https://github.com/41243240/Example/blob/main/Height.png)

### 結論

1. 程式能從空的二元樹，接收n筆亂數產生的資料後建立二元樹。  
2. 樹的高度除以 $log2(n)$ 確實趨近於2。  
3. 刪除k鍵值的時間1.複雜度在平均情況下為 $O$ $log(n)$而最壞情況則為 $O(n)$。   

### 申論及開發報告
1️. 先使用隨機產生大量 BST 節點，模擬各種規模的實際應用場景。  
2. 透過中序遍歷來隨機挑選刪除的節點，確保刪除目標確實存在於 BST 中。  
3️. 記錄刪除操作的比較次數，並統計平均值與最壞值，以掌握實際操作下的效能表現。  
4️. 最後再以小規模固定資料集做逐步刪除，幫助理解 BST 刪除時結構調整的具體過程。
綜合這些設計，能全面展示 BST 的建立、查找、刪除與其效率變化。
# **exercise 1**
## 題目需求
- (a)n筆記錄要在一台記憶體容量為S的電腦上做排序，其中S<<n。假設這整個S-紀錄的容量都可以用來做為輸入/輸出的緩衝區。輸入的紀錄儲存在磁碟上，包含m個行程。假設每次存取磁碟機時的搜尋時間是ts而延遲時間則是t1。傳送每筆紀錄的傳輸時間是tt。如果我們採用k-路合併並且將內部記憶體切割以使得我們可以像演算法Buffering那樣地平行處理輸入、輸出、以及CPU運算，那麼外部排序的第二個回合的總輸入時間為何?
- (b)
假設CPU合併所有的行程需要tCPU的時間(我們可以假設它跟k無關，因此是一個常數)。令ts=80ms，t1=20ms，n=200000，m=64，tt=10^-3次秒/紀錄，而且S=2000。粗略畫出一個總輸入時間tinput對k的函數圖形。是否一定有一個k值使得tcpu≈tinput

## 程式實作

有使用到的標頭檔以及命名空間使用宣告
```cpp
#include <iostream>
#include <cmath>   // for ceil and log
#include <iomanip> // for setprecision
using namespace std;
```
主程式，計算k=2~m的總輸入時間並且印出來
```cpp
int main() {
    // 已知參數
    double ts = 0.08;      // seek time (s)
    double tl = 0.02;      // latency time (s)
    double tt = 0.001;     // transmission time (s)
    int n = 200000;        // 總記錄數
    int m = 64;            // 初始 runs 數量

    // 計算每筆記錄的 I/O 時間
    double t_record = ts + tl + 2 * tt;

    cout << fixed << setprecision(6);
    cout << "k\tmerge_passes\ttotal_input_time (seconds)\n";

    // 計算 k=2 ~ m 時，總輸入時間
    for (int k = 2; k <= m; ++k) {
        // 計算需要幾次合併
        int merge_passes = static_cast<int>(ceil(log(m) / log(k)));

        // 總輸入時間
        double t_input = merge_passes * n * t_record;

        cout << k << "\t" << merge_passes << "\t\t" << t_input << endl;
    }

    return 0;
}
```
## 執行結果示意
![執行結果示意圖](https://github.com/41243253/example-1/blob/main/result.png)
![執行結果示意圖](https://github.com/41243253/example-1/blob/main/result2.png)

# 函數圖形
![執行結果示意圖](https://github.com/41243253/example-1/blob/main/result3.png)

### 結論
1. 假設每一輪合併都需重新讀寫所有資料，總輸入時間公式如下：
   - t $input$=merge_passesxnx( $ts$+ $tl $+ $2tt$)
2. 程式會計算每個k值(2~64)所需的合併次數merge_passes，對應的總輸入時間t_input。
4. k越大，合併次數越少，總輸入時間下降，k太小則會導致多輪merge，增加I/O時間。
5. 當k=8時，合併次數為2，為一個合理選擇。
## 申論及開發報告

- **實作重點**  
  - **Max/Min Heap**：
    - 使用C++ vector搭配template設計。
    - Push/Pop實作HeapifyUp、HeapifyDown維持heap的特性。
    - 抽象化共同介面(如IsEmpty()、Top())，便於Max與Min共用邏輯。
    - 使用chrono量測每個操作時間，搭配記憶體追蹤工具量測用量。
  - **Binary Search Tree**：  
    - 節點包含值與左右子樹指標，建構空樹後進行隨機插入。
    - 比較TreeHeight/log2(n)，觀察是否趨近平衡。
    - 節點刪除涵蓋三種情況：葉節點、單子樹、雙子樹(找inorder successor)。
    - 實作中紀錄比較次數，觀察實際時間複雜度變化。
  - **exercise 1**：  
    - 改變 $k$值模擬多路合併影響，觀察輸入成本隨 $k$的變化。
    - 將模擬數據繪圖，分析存在最佳 $k$值使CPU與I/O負載平衡。
  

- **測試策略**  
  - **Max/Min Heap**：各函式(Push/Pop/Top/IsEmpty)個別多次執行並記錄耗時。  
  - **Binary Search Tree**：  
    - BST重複插入 $n$筆隨機資料，平均化高度與比較次數結果。  
    - BST刪除測試三種情境(葉節點/單子樹/雙子樹)，記錄比較數與時間。
  - **exercise 1**：External Merge測試多組 $k$值，觀察合併次數與總輸入時間趨勢。

- **結論**  
  - Heap結構效能穩定，Push/Pop維持 $O(log n)$，Top/IsEmpty為 $O(1)$，Max/Min Heap差異極小。
  - BST高度驗證理論：隨機插入下高度趨近 $O(log n)$，刪除操作則可能達 $O(n)$。
  - External Merge：合併次數隨 $k$ 增加而減少，輸入時間下降。k過小會導致I/O次數過多，過大則CPU合併成本增加，需權衡取最適值。
  - 整體實作驗證了資料結構與演算法理論在實務中之應用與限制，提升對時間與空間複雜度的理解。
