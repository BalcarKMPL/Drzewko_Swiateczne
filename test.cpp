//Jakub Mazur
#include <iostream>
using namespace std;

struct Node {
    virtual bool value() = 0;
};

struct True : Node {
    True() = default;
    bool value() override { return true; }
};

struct False : Node {
    False() = default;
    bool value() override { return false; }
};

struct Variable : Node {
    bool* ptr;
    Variable(bool* ptr): ptr(ptr) {}
    bool value() override { return *ptr; }
};

struct Negation : Node {
    Node* o1;
    Negation(Node* o1): o1(o1) {}
    bool value() override { return not o1->value(); }
};

struct Alternative : Node {
    Node* left;
    Node* right;
    Alternative(Node* left, Node* right) {
        this->left = left;
        this->right = right;
    }
    bool value() override { return left->value() or right->value(); }
};

struct Conjunction : Node {
    Node* left;
    Node* right;
    Conjunction(Node* left, Node* right) {
        this->left = left;
        this->right = right;
    }
    bool value() override { return left->value() and right->value(); }
};

struct Implication : Node {
    Node* left;
    Node* right;
    Implication(Node* left, Node* right) {
        this->left = left;
        this->right = right;
    }
    bool value() override { return not left->value() and right->value(); }
};

struct Equivalence : Node {
    Node* left;
    Node* right;
    Equivalence(Node* left, Node* right) {
        this->left = left;
        this->right = right;
    }
    bool value() override { return (left->value() and right->value()) or (not left->value() and not right->value()); }
};

struct Formula : Node {
    Node* o1;
    Formula(Node* o1): o1(o1) {}
    bool value() { return o1->value(); }
};

//Formula* FormulaFromString(char* formula) {
//    Formula* f = new Formula();
//}

int main() {
//    char formula[] = "(p>q)";
    char c;
    while (true){
        cin >> c;
        if (c=='W') return 0;
        if (c=='=') cout << 1/0;
    }
}