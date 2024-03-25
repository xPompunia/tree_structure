#include<iostream>
#include<vector>
#define N 11

struct node {
    int value;
    node *left;
    node *right;

    node (int val):
        value(val),
        left(nullptr),
        right(nullptr) {}
};

void addNode(node *root, int n) {
    if (root->value > n) {
        if (root-> left != nullptr)
            addNode(root->left, n);
        else
            root->left = new node(n);
    }
    else {
        if (root->right != nullptr)
            addNode(root->right, n);
        else
            root->right = new node(n);
    }
}

void inorderPrint(node *root) {
    if (root != nullptr) {
        inorderPrint(root->left);
        std::cout << root->value << "\n";
        inorderPrint(root->right);
    }
}

int findMax(node *root) {
    if (root == nullptr)
        return -1;
    while (root->right != nullptr) {
        root = root->right;
    }
    return root->value;
}

node* buildBST(std::vector<int>& sortedArray, int start, int end) {
    if (start > end)
        return nullptr;

    int mid = (start + end) / 2;
    node* root = new node(sortedArray[mid]);
    root->left = buildBST(sortedArray, start, mid - 1);
    root->right = buildBST(sortedArray, mid + 1, end);

    return root;
}

int main() {
    int *n;
    n = new int[N];
    for(int i=0; i < N; ++i)
        n[i] = i;

    node *root = new node(n[0]);
    for (int i = 1; i < N; ++i) {
        addNode(root, n[i]);
    }
    inorderPrint(root);

    int maxElement = findMax(root);
    std::cout << "Maksymalny element w drzewie: " << maxElement << std::endl;

    std::vector<int> sortedArray = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    node* root2 = buildBST(sortedArray, 0, sortedArray.size() - 1);
    inorderPrint(root2);
    delete[] n;
    return 0;
}
