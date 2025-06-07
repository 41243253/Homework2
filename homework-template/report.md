# 41243240 
# 41243253

# **Max/Min Heap**

## 解題說明

本題為替定義一個最小優先權佇列的ADT MinPQ寫一個類似於ADT 5.2的抽象類別，寫一個衍生自這個抽象類別的MinHeap並實作所有MinPQ的虛擬函式，最後計算每一個函式的時間複雜度並且和MaxHeap複雜度做是否相同的判斷。

### 解題策略

1. 
2.   
3. 

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



### 圖表分析
MinHeap和MaxHeap的函式IsEmpty比較圖

![函式IsEmpty比較圖](<https://github.com/41243253/example-1/blob/main/IsEmpty.png>)

MinHeap和MaxHeap的函式Top比較圖

![函式Top比較圖](<https://github.com/41243253/example-1/blob/main/Top.png>)

MinHeap和MaxHeap的函式Push比較圖

![函式Push比較圖](<https://github.com/41243253/example-1/blob/main/Push.png>)

MinHeap和MaxHeap的函式Pop比較圖

![函式Pop比較圖](<https://github.com/41243253/example-1/blob/main/Pop.png>)

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
### 測試結果
![二元搜尋樹示意圖](https://github.com/41243240/Example/blob/main/BST.png)
### 高度除以 $log2(n)$ 圖表
![樹高度除以log2n](https://github.com/41243240/Example/blob/main/Tree-height.png)
### 結論

1. 程式能正確計算 $n$ 到 $1$ 的連加總和。  
2. 在 $n < 0$ 的情況下，程式會成功拋出異常，符合設計預期。  
3. 測試案例涵蓋了多種邊界情況（$n = 0$、$n = 1$、$n > 1$、$n < 0$），驗證程式的正確性。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. **程式邏輯簡單直觀**  
   遞迴的寫法能夠清楚表達「將問題拆解為更小的子問題」的核心概念。  
   例如，計算 $\Sigma(n)$ 的過程可分解為：  

   $$
   \Sigma(n) = n + \Sigma(n-1)
   $$

   當 $n$ 等於 1 或 0 時，直接返回結果，結束遞迴。

2. **易於理解與實現**  
   遞迴的程式碼更接近數學公式的表示方式，特別適合新手學習遞迴的基本概念。  
   以本程式為例：  

   ```cpp
   int sigma(int n) {
       if (n < 0)
           throw "n < 0";
       else if (n <= 1)
           return n;
       return n + sigma(n - 1);
   }
   ```

3. **遞迴的語意清楚**  
   在程式中，每次遞迴呼叫都代表一個「子問題的解」，而最終遞迴的返回結果會逐層相加，完成整體問題的求解。  
   這種設計簡化了邏輯，不需要額外變數來維護中間狀態。

透過遞迴實作 Sigma 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $n$ 值過大時，應考慮使用迭代版本來避免 Stack Overflow 問題。
