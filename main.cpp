//Jakub Mazur
#include <iostream>
#define ifprint true
using namespace std;


void inline print(const char* str){
    if (ifprint) cout << str << endl;
}

struct Formula {
    virtual bool val() = 0;
    explicit virtual operator bool() final {
        return val();
    }
    virtual ~Formula() {};
};

struct Variable : Formula {
    bool* ptr;
    explicit Variable(bool* ptr): ptr(ptr) {}
    bool val() override {
        return *ptr;
    }
    ~Variable() override { print("Variable destructed"); }
};

struct Implication : Formula {
    Formula* c1; Formula* c2;
    Implication(Formula* c1, Formula* c2): c1(c1), c2(c2) {}
    bool val() override {
        return !c1->val() || c2->val();
    }
    ~Implication() override { print("Implication destructed"); delete c1; delete c2; }
};

struct Alternative : Formula {
    Formula* c1; Formula* c2;
    Alternative(Formula* c1, Formula* c2): c1(c1), c2(c2) {}
    bool val() override {
        return c1->val() || c2->val();
    }
    ~Alternative() override { print("Alternative destructed"); delete c1; delete c2;}
};

struct Conjunction : Formula {
    Formula* c1; Formula* c2;
    Conjunction(Formula* c1, Formula* c2): c1(c1), c2(c2) {}
    bool val() override {
        return c1->val() && c2->val();
    }
    ~Conjunction() override { print("Conjunction destructed"); delete c1; delete c2;}
};

struct Equivalence : Formula {
    Formula* c1; Formula* c2;
    Equivalence(Formula* c1, Formula* c2): c1(c1), c2(c2) {}
    bool val() override {
        return c1->val() == c2->val();
    }
    ~Equivalence() override { print("Equivalence destructed"); delete c1; delete c2;}
};

struct Negation : Formula {
    Formula* c;
    explicit Negation(Formula* c) : c(c) {}
    bool val() override {
        return !c->val();
    }
    ~Negation() override { print("Negation destructed"); delete c; }
};

inline bool isletter(char c) {
    return 'a' <= c and c <= 'z';
}

inline bool is1op(char c){
    return c == '~';
}
inline bool is2op(char c){
    return c == '|' or c == '&' or c == '>' or c == '=';
}

Formula* Parse(char* b, char* e, Variable** vars) {
    if (isletter(*b) and b+1 == e)
        return vars[*b - 'a'];
    if (*b=='~')
        return new Negation(Parse(b+1,e,vars));
    if (*b=='('){
        int nawiasow_count = -1; char c;
        for (int i = 0; b[i] != 0; ++i) {
            c = b[i];
            if (c == '(') nawiasow_count++;
            if (c == ')') nawiasow_count--;
            if (nawiasow_count == 0 and is2op(c)) {
                char* b1 = b + 1;
                char* e1 = b + i;
                char* b2 = b + i + 1;
                char* e2 = e - 1;
                switch (c) {
                    case '|':
                        return new Alternative(Parse(b1, e1, vars), Parse(b2,e2,vars))
                        break;
                    case '&':
                        return new Conjunction(Parse(b1, e1, vars), Parse(b2,e2,vars))
                        break;
                    case '>':
                        return new Implication(Parse(b1, e1, vars), Parse(b2,e2,vars))
                        break;
                    case '=':
                        return new Equivalence(Parse(b1, e1, vars), Parse(b2,e2,vars))
                        break;
                    default:
                        cout << "Coś się zjebało..."
                        break;

                }
            }
        }
    }
    return nullptr;
}

int main() {
    constexpr int len = 'z' - 'a' + 1;
    bool* vars = new bool[len];
    Variable** Vars = new Variable*[len];
    for (int i = 0; i < len; ++i) {
        Vars[i] = new Variable(vars + i);
    }

    char b[] = "(hakuna|matata)";
    char* e = b + sizeof(b) - 1;

    Formula* f = Parse(b, e, Vars); return 0;
    vars['p'-'a'] = true;
    cout << f->val() << endl;
    vars['p'-'a'] = false;
    cout << f->val() << endl;

//    bool* b = new bool;
//    *b = true;
//    auto* v = new Variable(b);
//    auto* nv = new Negation(v);
//    auto* nnv = new Negation(nv);
//    cout << v->val() << endl;
//    cout << nv->val() << endl;
//    delete nnv;
}
