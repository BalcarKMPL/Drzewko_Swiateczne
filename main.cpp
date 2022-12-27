//Jakub Mazur
#include <iostream>
using namespace std;

struct Formula {
    virtual bool val() = 0;
    explicit virtual operator bool() final {
        return val();
    }
};

struct Variable : Formula {
    bool* ptr;
    explicit Variable(bool* ptr): ptr(ptr) {}
    bool val() override {
        return *ptr;
    }
};

struct Implication : Formula {
    Formula* c1; Formula* c2;
    Implication(Formula* c1, Formula* c2): c1(c1), c2(c2) {}
    bool val() override {
        return !c1->val() || c2->val();
    }
};

struct Alternative : Formula {
    Formula* c1; Formula* c2;
    Alternative(Formula* c1, Formula* c2): c1(c1), c2(c2) {}
    bool val() override {
        return c1->val() || c2->val();
    }
};

struct Conjunction : Formula {
    Formula* c1; Formula* c2;
    Conjunction(Formula* c1, Formula* c2): c1(c1), c2(c2) {}
    bool val() override {
        return c1->val() && c2->val();
    }
};

struct Equivalence : Formula {
    Formula* c1; Formula* c2;
    Equivalence(Formula* c1, Formula* c2): c1(c1), c2(c2) {}
    bool val() override {
        return c1->val() == c2->val();
    }
};

struct Negation : Formula {
    Formula* c;
    Negation(Formula* c) : c(c) {}
    bool val() override {
        return !c->val();
    }
};

bool isletter(char c) {
    return 'a'<=c and c <= 'z';
}

Formula* Parse(char* b, char* e, Formula* vars) {
    if (isletter(*b) and b+1 == e)
        return vars + *b - 'a';
    if (*b=='~')
        return new Negation(Parse(b+1,e,vars));
//    if (*b=='('){
//
//    }
    return nullptr;
}

int main() {
    //       r || (p && q)
    bool _p = true, _q = true, _r = false;
    auto p = new Variable(&_p);
    auto q = new Variable(&_q);
    auto r = new Variable(&_r);
    auto F1 = new Conjunction(p,q);
    auto F2 = new Alternative(r,F1);
    cout << F2->val();
}
