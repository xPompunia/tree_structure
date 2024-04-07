#include<iostream>
#include<vector>
#define N 100

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
        std::cout << root->value << " -> ";
        inorderPrint(root->right);
    }
}

void preorderPrint(node *root) {
    if (root != nullptr) {
        std::cout << root->value << " -> ";
        preorderPrint(root->left);
        preorderPrint(root->right);
    }
}
void postorderPrint(node *root) {
    if (root != nullptr) {
        preorderPrint(root->left);
        preorderPrint(root->right);
        std::cout << root->value << " -> ";
    }
}

node* deleteNode(node* root, int val) {
    if (root == nullptr)
        return root;

    if (val < root->value)
        root->left = deleteNode(root->left, val);
    else if (val > root->value)
        root->right = deleteNode(root->right, val);
    else {
        if (root->left == nullptr) {
            node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            node* temp = root->left;
            delete root;
            return temp;
        }
        node* temp = root->right;
        while (temp->left != nullptr)
            temp = temp->left;
        root->value = temp->value;
        root->right = deleteNode(root->right, temp->value);
    }
    return root;
}

void postOrderDelete(node *root) {
    if (root != nullptr) {
        postOrderDelete(root->left);
        postOrderDelete(root->right);
        std::cout << "Deleting node with key: " << root->value << "\n";
        delete root;
    }
}

int findMax(node *root) {
    if (root == nullptr){
        std::cout << "Drzewo jest puste.\n";
        return -1;
    }
    std::cout << "Sciezka poszukiwania (od korzenia do maksymalnego elementu): ";
    while (root->right != nullptr) {
        std::cout << root->value << " -> ";
        root = root->right;
    }
    return root->value;
}

int findMin(node *root) {
    if (root == nullptr) {
        std::cout << "Drzewo jest puste.\n";
        return -1;
    }
    std::cout << "Sciezka poszukiwania (od korzenia do minimalnego elementu): ";
    while (root->left != nullptr) {
        std::cout << root->value << " -> ";
        root = root->left;
    }
    return root->value;
}

node* buildAVL(std::vector<int>& sortedArray, int start, int end) {
    if (start > end)
        return nullptr;

    int mid = (start + end) / 2;
    node* root = new node(sortedArray[mid]);
    root->left = buildAVL(sortedArray, start, mid - 1);
    root->right = buildAVL(sortedArray, mid + 1, end);

    return root;
}

int main() {
//    TWORZENIE ZDEGENEROWANEGO DRZEWA BST
    int *n;
    n = new int[N];
    for(int i=0; i < N; ++i)
        n[i] = i+1;

    node *root = new node(n[0]);
    for (int i = 1; i < N; ++i) {
        addNode(root, n[i]);
    }
    std::cout << "Przeszukiwanie Preorder: ";
    preorderPrint(root);
    std::cout << std::endl;

    std::cout << "Maksymalny element w drzewie: " << findMax(root) << std::endl;
    std::cout << "Minimalny element w drzewie: " << findMin(root) << std::endl;

// TWORZENIE DRZEWA AVL METODA POLOWIENIA BINARNEGO
    std::vector<int> sortedArray;
    for (int i = 1; i <= 100; ++i) {
        sortedArray.push_back(i);
    }

    node* root2 = buildAVL(sortedArray, 0, sortedArray.size() - 1);
    std::cout << "Przeszukiwanie Preorder: ";
    preorderPrint(root2);
    std::cout << std::endl;

    std::cout << "Maksymalny element w drzewie: " << findMax(root2) << std::endl;
    std::cout << "Minimalny element w drzewie: " << findMin(root2) << std::endl;
    delete[] n;
    return 0;
}
