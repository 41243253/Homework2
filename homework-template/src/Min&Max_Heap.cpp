#include <iostream>
#include <vector>
//#include <queue>
#include <cstdlib>
#include <ctime>         //取得目前時間來做亂數種子
#include <chrono>        //時間的測量
#include <windows.h>     //取得記憶體的使用資訊
#include <psapi.h>       //配合上面<windows.h> 一起取得記憶體資訊
using namespace std;
using namespace chrono; //用來計時

//抽象類別MinPQ(最小優先權佇列)
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

//MinHeap類別實作MinPQ
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

//抽象類別MaxPQ(最大優先權佇列)
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

//MaxHeap類別實作MaxPQ
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

//顯示目前程式使用的記憶體狀態
void printMemoryUsage() 
{
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------\n"
        << "Memory Usage Information:\n"
        << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB\n"
        << "----------------------------------------------------------\n";
}

//測試堆積的效能與各個函式平均耗時
//傳入堆積類別、資料向量與標籤名稱(用來識別是最小堆積還是最大堆積)
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

//主程式
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
