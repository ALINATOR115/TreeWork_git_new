#include <iostream>
#include <queue>
#include <string>
#include <cctype>
using namespace std;

class TreeNode {
public:
    int val;
    int height;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    TreeNode* root;
    int height(TreeNode* node);
    int balanceFactor(TreeNode* node);
    void fixHeight(TreeNode* node);
    TreeNode* rotateRight(TreeNode* p);
    TreeNode* rotateLeft(TreeNode* q);
    TreeNode* balance(TreeNode* p);
    TreeNode* removeNode(TreeNode* node, int key);
    TreeNode* findMin(TreeNode* node);

public:
    AVLTree() : root(nullptr) {}
    void insert(int key);
    void remove(int key);
    TreeNode* search(int key);
    void printTree();
    TreeNode* getRoot() { return root; }
private:
    void printTree(TreeNode* node, int indent);
};

//���� ����� ���������� ������ ���� node.
//���� ���� node �� ���������� (����� nullptr), ������ ��������� ������ 0.
int AVLTree::height(TreeNode* node) {
    return node ? node->height : 0;
}

//����� balanceFactor ���������� ������� ����� ������� � ������ ����������� ���� node, 
//��� ������������ ��� ����������� ������� ����.
int AVLTree::balanceFactor(TreeNode* node) {
    return height(node->right) - height(node->left);
}

//����� fixHeight ������������� ������ ���� node �� ������ ����� ��� ������ � ������� �����������.
void AVLTree::fixHeight(TreeNode* node) {
    int hl = height(node->left);
    int hr = height(node->right);
    node->height = max(hl, hr) + 1;
}

//������ rotateRight � rotateLeft ��������� ������ � ����� �������� ��������������.
//��� �������� ������������ ��� ������������ ������.
TreeNode* AVLTree::rotateRight(TreeNode* p) {
    TreeNode* q = p->left;
    p->left = q->right;
    q->right = p;
    fixHeight(p);
    fixHeight(q);
    return q;
}

TreeNode* AVLTree::rotateLeft(TreeNode* q) {
    TreeNode* p = q->right;
    q->right = p->left;
    p->left = q;
    fixHeight(q);
    fixHeight(p);
    return p;
}

//����� balance ��������� ������ ���� p �, ���� �� �������
//��������� ��������������� �������� ��� �������������� �������.
TreeNode* AVLTree::balance(TreeNode* p) {
    fixHeight(p);
    if (balanceFactor(p) == 2) {
        if (balanceFactor(p->right) < 0)
            p->right = rotateRight(p->right);
        return rotateLeft(p);
    }
    if (balanceFactor(p) == -2) {
        if (balanceFactor(p->left) > 0)
            p->left = rotateLeft(p->left);
        return rotateRight(p);
    }
    return p;
}

//����� insert ��������� ����� ���� � ������ key � ������. ���� ������ ������, ��������� �������� ����. 
//����� ���� ����������� � ��������������� ����� � ������
//� ����� ���������� ����� balance ��� �������������� �������.
void AVLTree::insert(int key) {
    if (!root) {
        root = new TreeNode(key);
        return;
    }
    TreeNode* curr = root;
    TreeNode* prev = nullptr;
    while (curr) {
        prev = curr;
        if (key < curr->val)
            curr = curr->left;
        else
            curr = curr->right;
    }
    if (key < prev->val)
        prev->left = new TreeNode(key);
    else
        prev->right = new TreeNode(key);
    root = balance(root);
}

//����� remove ������� ���� � ������ key �� ������.
void AVLTree::remove(int key) {
    root = removeNode(root, key);
}

//����� removeNode ���������� ������� ���� � ������ key �� ��������� � ������ � node.
// ����� �������� ���� ����������� ������������ ������.
TreeNode* AVLTree::removeNode(TreeNode* node, int key) {
    if (!node) return nullptr;

    if (key < node->val) {
        node->left = removeNode(node->left, key);
    }
    else if (key > node->val) {
        node->right = removeNode(node->right, key);
    }
    else {
        if (!node->left || !node->right) {
            TreeNode* temp = (node->left) ? node->left : node->right;
            if (!temp) {
                temp = node;
                node = nullptr;
            }
            else {
                *node = *temp;
            }
            delete temp;
        }
        else {
            TreeNode* temp = findMin(node->right);
            node->val = temp->val;
            node->right = removeNode(node->right, temp->val);
        }
    }

    if (!node) return nullptr;

    node = balance(node);

    return node;
}


//������ findMin � findMax ������� ���� � ����������� � ������������ ������� � ������, ��������������.
TreeNode* AVLTree::findMin(TreeNode* node) {
    while (node->left) {
        node = node->left;
    }
    return node;
}

//����� search ��������� ����� ���� � ������ key � ������ � ���������� ���, ���� �� ������.
TreeNode* AVLTree::search(int key) {
    TreeNode* curr = root;
    while (curr) {
        if (curr->val == key)
            return curr;
        else if (key < curr->val)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return nullptr;
}

//����� printTree ������� ������ � ���� ������ �� �����, � ��������� ��� ������� ������.
void AVLTree::printTree() {
    printTree(root, 0);
}

void AVLTree::printTree(TreeNode* node, int indent) {
    if (node == nullptr)
        return;

    printTree(node->right, indent + 4);
    cout << string(indent, ' ') << node->val << endl;
    printTree(node->left, indent + 4);
}


int main() {
    setlocale(LC_ALL, "");
    AVLTree avl;
    int choice;
    do {
        cout << "\n1. Insert\n2. Remove\n3. Print AVL_Tree\n4. Exit\nEnter your choice: ";
        cin >> choice;
        switch (choice) {
        case 1: {
            int num;
            cout << "Enter element to insert: \n";
            cin >> num;
            avl.insert(num);
            break;
        }
        case 2: {
            int num;
            cout << "Enter element to remove: ";
            cin >> num;
            avl.remove(num);
            break;
        }
        case 3: {
            cout << "***************|AVL Tree:|***************" << endl;
            avl.printTree();
            break;
        }
        case 4: {
            cout << "Exiting...";
            break;
        }
        default:
            cout << "Invalid choice.";
        }
    } while (choice != 4);

    return 0;
}
