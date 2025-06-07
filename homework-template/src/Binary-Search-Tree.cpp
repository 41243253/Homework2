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

// �w�q BST �`�I
struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int val) : key(val), left(nullptr), right(nullptr) {}
};

// ���J�`�I
Node* insert(Node* root, int key) {
    if (!root)
        return new Node(key);
    if (key < root->key)
        root->left = insert(root->left, key);
    else
        root->right = insert(root->right, key);
    return root;
}

// ���ǹM�� BST�A�ç�`�I�ȥ[�J vector
void inorder(Node* root, vector<int>& keys) {
    if (!root) return;
    inorder(root->left, keys);
    keys.push_back(root->key);
    inorder(root->right, keys);
}

// �p��𪺰���
int Height(Node* root) {
    if (!root)
        return 0;
    int leftHeight = Height(root->left);
    int rightHeight = Height(root->right);
    return 1 + max(leftHeight, rightHeight);
}

// ��� BST ���̤p�ȸ`�I
Node* findMin(Node* root, int& steps) {
    while (root && root->left) {
        root = root->left;
        steps++;
    }
    return root;
}

// �R���`�I�A�íp��������
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

// �d��`�I�ݩ�
void nodeType(Node* root, int key) {
    if (!root) return;
    if (key < root->key)
        nodeType(root->left, key);
    else if (key > root->key)
        nodeType(root->right, key);
    else {
        cout << "�R���`�I " << key << "�G";
        if (!root->left && !root->right)
            cout << "���O���`�I�C\n";
        else if (!root->left || !root->right)
            cout << "���� 1 �Ӥl�`�I�C\n";
        else
            cout << "���� 2 �Ӥl�`�I�C\n";
    }
}

// ����𪺰O����
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// ��ܰO����ϥΪ��p
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------\n"
        << "Memory Usage Information:\n"
        << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB\n"
        << "----------------------------------------------------------\n";
}

int main() {
    srand(time(0)); // ��l�ƶüƺؤl

    // Part 1�Gn=100,500,...,10000 �إ� BST�A�H���R���ì����������
    int n_values[] = { 100, 500, 1000, 2000, 3000, 4000, 5000, 10000 };

    cout << "n\t��\tlog2(n)\t\t��/log2(n)" << endl;
    cout << "----------------------------------------------------------" << endl;

    vector<int> deleteComparisons;

    for (int i = 0; i < 8; ++i) {
        int n = n_values[i];
        Node* root = nullptr;

        // ���J n ���üƸ��
        for (int j = 0; j < n; ++j) {
            int val = rand() % (n * 10) + 1;
            root = insert(root, val);
        }

        // �p�Ⱚ�׻P���
        int height = Height(root);
        double log2n = log2(n);
        double ratio = height / log2n;

        cout << n << "\t" << height << "\t" << log2n << "\t\t" << ratio << endl;

        // �H���q���ǹM�����G����@�ӭȰ��R��
        vector<int> keys;
        inorder(root, keys);
        int randomIndex = rand() % keys.size();
        int deleteKey = keys[randomIndex];

        int steps = 0;
        root = deleteNode(root, deleteKey, steps);
        deleteComparisons.push_back(steps);

        deleteTree(root);
    }

    // �έp�����M���a�������
    int total = 0, worst = 0;
    for (int c : deleteComparisons) {
        total += c;
        worst = max(worst, c);
    }
    double average = (double)total / deleteComparisons.size();

    cout << "\n�R���ާ@�������������: " << average << endl;
    cout << "�R���ާ@�����a�������: " << worst << endl;

    // Part 2�G�إ߹w�]��ơA���T�B�J�R��
    cout << "\n----------------------------------------------------------" << endl;
    cout << "�إ߹w�]��� [10, 20, 30, 40, 50] �ȩ̀ǧR�� 10, 30, 40�C\n";

    Node* root = nullptr;
    int preset[] = { 10, 20, 30, 40, 50 };
    for (int val : preset) {
        root = insert(root, val);
    }

    cout << "��l BST (���ǹM��)�G";
    vector<int> tmp;
    inorder(root, tmp);
    for (int x : tmp) cout << x << " ";
    cout << endl;

    int keysToDelete[] = { 10, 30, 40 };
    for (int key : keysToDelete) {
        nodeType(root, key);
        int steps = 0;
        root = deleteNode(root, key, steps);
        cout << "�R�� " << key << " �ᤤ�ǹM���G";
        vector<int> tmp2;
        inorder(root, tmp2);
        for (int x : tmp2) cout << x << " ";
        cout << "\n�𪺰���: " << Height(root) << endl;
        cout << "�R�� " << key << " �L�{�������: " << steps << "\n" << endl;
    }

    printMemoryUsage();
    deleteTree(root);

    return 0;
}
