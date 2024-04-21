#include <iostream>
#include <vector>
#include <chrono> // Do pomiaru czasu
#include <cmath>

#define N 1000

struct Node {
    int value;
    Node *left;
    Node *right;

    Node(int val) :
            value(val),
            left(nullptr),
            right(nullptr) {}
};

int height(Node* node) {
    if (node == nullptr)
        return 0;
    return std::max(height(node->left), height(node->right)) + 1;
}
int countNodes(Node* root) {
    if (root == nullptr) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void createBackbone(Node* root) {
    Node* prev = nullptr;
    Node* curr = root;

    while (curr != nullptr) {
        if (curr->left != nullptr) {
            // Rotate right to eliminate left child
            Node* temp = curr->left;
            curr->left = temp->right;
            temp->right = curr;
            curr = temp;
            if (prev != nullptr) prev->right = temp;
            else root = temp;
        }
        else {
            prev = curr;
            curr = curr->right;
        }
    }
}

int balanceFactor(Node* node) {
    if (node == nullptr)
        return 0;
    return std::abs(height(node->left) - height(node->right));
}

Node* leftRotate(Node* node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    return newRoot;
}

Node* rightRotate(Node* node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    return newRoot;
}

void compress(Node* grand, int m)
{
    Node* tmp = grand->right;

    for (int i = 0; i < m; i++) {
        Node* oldTmp = tmp;
        tmp = tmp->right;
        grand->right = tmp;
        oldTmp->right = tmp->left;
        tmp->left = oldTmp;
        grand = tmp;
        tmp = tmp->right;
    }
}

Node* balanceTree(Node* root) {
    if (root == nullptr)
        return nullptr;

    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    if (leftHeight - rightHeight > 1) {
        if (height(root->left->left) >= height(root->left->right))
            root = rightRotate(root);
        else {
            root->left = leftRotate(root->left);
            root = rightRotate(root);
        }
    }
    else if (rightHeight - leftHeight > 1) {
        if (height(root->right->right) >= height(root->right->left))
            root = leftRotate(root);
        else {
            root->right = rightRotate(root->right);
            root = leftRotate(root);
        }
    }

    return root;
}

Node* dsw_balance(Node* root) {
    if (root == nullptr) return nullptr;
    Node* grand = new Node(0);
    grand->right = root;
    createBackbone(root); //gdyby drzewo nie bylo zdegenerowane, nalezy na poczatku sprowadzic je do winorosli
    int n = countNodes(root);
    int h = floor(log2(n + 1));
    int m = pow(2, h) - 1;

    compress(grand, n - m);
    for (m = m / 2; m > 0; m /= 2) {
        compress(grand, m);
    }

    return grand->right;
}


void addNode(Node *root, int n) {
    if (root->value > n) {
        if (root-> left != nullptr)
            addNode(root->left, n);
        else
            root->left = new Node(n);
    }
    else {
        if (root->right != nullptr)
            addNode(root->right, n);
        else
            root->right = new Node(n);
    }
}

void inorderPrint(Node *root) {
    if (root != nullptr) {
        inorderPrint(root->left);
        std::cout << root->value << " -> ";
        inorderPrint(root->right);
    }
}

void preorderPrint(Node *root) {
    if (root != nullptr) {
        std::cout << root->value << " -> ";
        preorderPrint(root->left);
        preorderPrint(root->right);
    }
}
void postorderPrint(Node *root) {
    if (root != nullptr) {
        postorderPrint(root->left);
        postorderPrint(root->right);
        std::cout << root->value << " -> ";
    }
}

Node* deleteNode(Node* root, int val) {
    if (root == nullptr)
        return root;

    if (val < root->value)
        root->left = deleteNode(root->left, val);
    else if (val > root->value)
        root->right = deleteNode(root->right, val);
    else {
        if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        Node* temp = root->right;
        while (temp->left != nullptr)
            temp = temp->left;
        root->value = temp->value;
        root->right = deleteNode(root->right, temp->value);
    }
    return root;
}

void postOrderDelete(Node *root) {
    if (root != nullptr) {
        postOrderDelete(root->left);
        postOrderDelete(root->right);
        std::cout << "Usuwanie elementu: " << root->value << "\n";
        delete root;
    }
}

int findMax(Node *root) {
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

int findMin(Node *root) {
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

Node* buildAVL(std::vector<int>& sortedArray, int start, int end) {
    if (start > end)
        return nullptr;

    int mid = (start + end) / 2; //zakladamy ze lista jest posortowana (jesli nie, nalezy ja wczesniej posortowac)
    Node* root = new Node(sortedArray[mid]);
    root->left = buildAVL(sortedArray, start, mid - 1);
    root->right = buildAVL(sortedArray, mid + 1, end);

    return root;
}

int main() {

    int option_generate;
    std::cout << "Wybierz opcję:\n";
    std::cout << "1. Generuj drzewa automatycznie\n";
    std::cout << "2. Wprowadź listę liczb\n";
    std::cin >> option_generate;
    int n;
    std::vector<int> numbers;
    if (option_generate == 2) {
        std::cout << "Podaj liczbę elementów: ";
        std::cin >> n;
        std::cout << "Podaj " << n << " liczb:\n";
        numbers.resize(n);
        for (int i = 0; i < n; ++i) {
            std::cin >> numbers[i];
        }
        std::sort(numbers.begin(), numbers.end());
    } else {
        std::cout << "Podaj liczbę elementów: ";
        std::cin >> n;
        numbers.resize(n);
        for (int i = 0; i < n; ++i) {
            numbers[i] = i + 1;
        }
    }
    // TWORZENIE ZDEGENEROWANEGO DRZEWA BST
//    int *n;
//    n = new int[N];
//    for(int i=0; i < N; ++i)
//        n[i] = i+1;

    std::cout<<"Tworzenie BST (zdegenerowanego): ";
    Node *bstRoot = new Node(numbers[0]);
    auto start = std::chrono::steady_clock::now();
    for (int i = 1; i < n; ++i) {
        addNode(bstRoot, numbers[i]);
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "Utworzono drzewo BST. Czas operacji: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " mikrosekund" << std::endl;



//    // TWORZENIE DRZEWA AVL METODA POLOWIENIA BINARNEGO
//    std::vector<int> sortedArray;
//    for (int i = 1; i <= n; ++i) {
//        sortedArray.push_back(i);
//    }
    std::cout<<"Tworzenie AVL metoda polowienia binarnego: ";
    start = std::chrono::steady_clock::now();
    Node* avlRoot = buildAVL(numbers, 0, numbers.size() - 1);
    end = std::chrono::steady_clock::now();
    std::cout << "Utworzono drzewo AVL. Czas operacji: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " mikrosekund" << std::endl;


    // WYWOLANIE MENU
    int option;
    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. Wyszukanie elementu o najmniejszej i najwiekszej wartosci w drzewie\n";
        std::cout << "2. Usuniecie elementu drzewa o podanej wartosci klucza\n";
        std::cout << "3. Wypisanie wszystkich elementow drzewa w porzadku in-order i pre-order\n";
        std::cout << "4. Usuniecie calego drzewa element po elemencie metoda post-order\n";
        std::cout << "5. Rownowazenie drzewa\n";
        std::cout << "0. Wyjscie\n";
        std::cout << "Wybierz opcje: ";
        std::cin >> option;

        switch (option) {
            case 1:
            {
                auto start = std::chrono::steady_clock::now();
                std::cout << "Drzewo BST:\n";
                std::cout << "Maksymalny element: " << findMax(bstRoot) << std::endl;
//                std::cout << "Minimalny element: " << findMin(bstRoot) << std::endl;
                auto end = std::chrono::steady_clock::now();
                std::cout << "Czas operacji dla drzewa BST: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " mikrosekund" << std::endl;

                start = std::chrono::steady_clock::now();
                std::cout << "Drzewo AVL:\n";
                std::cout << "Maksymalny element: " << findMax(avlRoot) << std::endl;
//                std::cout << "Minimalny element: " << findMin(avlRoot) << std::endl;
                end = std::chrono::steady_clock::now();
                std::cout << "Czas operacji dla drzewa AVL: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " mikrosekund" << std::endl;
            }
                break;
            case 2:
            {
                int numToDelete;
                std::cout << "Podaj liczbe wezlow do usuniecia: ";
                std::cin >> numToDelete;
                std::cout << "Podaj wartosci kluczy do usuniecia:\n";

                auto startBST = std::chrono::steady_clock::now();
                for (int i = 0; i < numToDelete; ++i) {
                    int key;
                    std::cout << "Wartosc klucza " << i + 1 << ": ";
                    std::cin >> key;
                    bstRoot = deleteNode(bstRoot, key);
                }
                auto endBST = std::chrono::steady_clock::now();
                std::cout << "Czas operacji usuwania dla drzewa BST: " << std::chrono::duration_cast<std::chrono::microseconds>(endBST - startBST).count() << " mikrosekund" << std::endl;

                auto startAVL = std::chrono::steady_clock::now();
                for (int i = 0; i < numToDelete; ++i) {
                    int key;
                    std::cout << "Wartosc klucza " << i + 1 << ": ";
                    std::cin >> key;
                    avlRoot = deleteNode(avlRoot, key);
                }
                auto endAVL = std::chrono::steady_clock::now();
                std::cout << "Czas operacji usuwania dla drzewa AVL: " << std::chrono::duration_cast<std::chrono::microseconds>(endAVL - startAVL).count() << " mikrosekund" << std::endl;
            }
                break;
            case 3:
            {
                auto startBST = std::chrono::steady_clock::now();
                std::cout << "Drzewo BST (In-order): ";
                inorderPrint(bstRoot);
                std::cout << std::endl;
//                std::cout << "Drzewo BST (Pre-order): ";
//                preorderPrint(bstRoot);
//                std::cout << std::endl;
                auto endBST = std::chrono::steady_clock::now();
                std::cout << "Czas operacji dla wypisywania elementow dla drzewa BST: " << std::chrono::duration_cast<std::chrono::microseconds>(endBST - startBST).count() << " mikrosekund" << std::endl;

                auto startAVL = std::chrono::steady_clock::now();
                std::cout << "Drzewo AVL (In-order): ";
                inorderPrint(avlRoot);
                std::cout << std::endl;
//                std::cout << "Drzewo AVL (Pre-order): ";
//                preorderPrint(avlRoot);
//                std::cout << std::endl;
                auto endAVL = std::chrono::steady_clock::now();
                std::cout << "Czas operacji dla wypisywania elementow dla drzewa AVL: " << std::chrono::duration_cast<std::chrono::microseconds>(endAVL - startAVL).count() << " mikrosekund" << std::endl;
            }
                break;
            case 4:
            {
                auto startBST = std::chrono::steady_clock::now();
                postOrderDelete(bstRoot);
                std::cout << "Drzewo BST zostalo usuniete.\n";
                auto endBST = std::chrono::steady_clock::now();
                std::cout << "Czas operacji usuwania drzewa BST: " << std::chrono::duration_cast<std::chrono::microseconds>(endBST - startBST).count() << " mikrosekund" << std::endl;

                auto startAVL = std::chrono::steady_clock::now();
                postOrderDelete(avlRoot);
                std::cout << "Drzewo AVL zostalo usuniete.\n";
                auto endAVL = std::chrono::steady_clock::now();
                std::cout << "Czas operacji usuwania drzewa AVL: " << std::chrono::duration_cast<std::chrono::microseconds>(endAVL - startAVL).count() << " mikrosekund" << std::endl;
            }
                break;
            case 5:
            {
                auto startBST = std::chrono::steady_clock::now();
                std::cout << "Wywoluje rownowazenie drzewa BST...\n";
                bstRoot = dsw_balance(bstRoot);
                std::cout << "Drzewo BST zostalo zrownowazone.\n";
                auto endBST = std::chrono::steady_clock::now();
                std::cout << "Czas operacji rownowazenia drzewa BST: " << std::chrono::duration_cast<std::chrono::microseconds>(endBST - startBST).count() << " mikrosekund" << std::endl;

//                auto startAVL = std::chrono::steady_clock::now();
//                std::cout << "Wywoluje rownowazenie drzewa AVL...\n";
//                avlRoot = balanceTree(avlRoot);
//                std::cout << "Drzewo AVL zostalo zrownowazone.\n";
//                auto endAVL = std::chrono::steady_clock::now();
//                std::cout << "Czas operacji rownowazenia drzewa AVL: " << std::chrono::duration_cast<std::chrono::microseconds>(endAVL - startAVL).count() << " mikrosekund" << std::endl;
            }
                break;
            case 0:
                std::cout << "Koniec programu.\n";
                break;
            default:
                std::cout << "Niepoprawna opcja. Sprobuj ponownie.\n";
                break;
        }
    } while (option != 0);

//    delete[] n;
    return 0;
}