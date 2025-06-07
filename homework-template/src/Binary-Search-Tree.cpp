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

// 定義 BST 節點
struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int val) : key(val), left(nullptr), right(nullptr) {}
};

// 插入節點
Node* insert(Node* root, int key) {
    if (!root)
        return new Node(key);
    if (key < root->key)
        root->left = insert(root->left, key);
    else
        root->right = insert(root->right, key);
    return root;
}

// 中序遍歷 BST，並把節點值加入 vector
void inorder(Node* root, vector<int>& keys) {
    if (!root) return;
    inorder(root->left, keys);
    keys.push_back(root->key);
    inorder(root->right, keys);
}

// 計算樹的高度
int Height(Node* root) {
    if (!root)
        return 0;
    int leftHeight = Height(root->left);
    int rightHeight = Height(root->right);
    return 1 + max(leftHeight, rightHeight);
}

// 找到 BST 中最小值節點
Node* findMin(Node* root, int& steps) {
    while (root && root->left) {
        root = root->left;
        steps++;
    }
    return root;
}

// 刪除節點，並計算比較次數
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

// 查找節點屬性
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

// 釋放樹的記憶體
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// 顯示記憶體使用狀況
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------\n"
        << "Memory Usage Information:\n"
        << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB\n"
        << "----------------------------------------------------------\n";
}

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
