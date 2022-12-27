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
    Formula& c1; Formula& c2;
    Implication(Formula& c1, Formula& c2): c1(c1), c2(c2) {}
    bool val() override {
        return !c1 || c2;
    }
};

struct Alternative : Formula {
    Formula& c1; Formula& c2;
    Alternative(Formula& c1, Formula& c2): c1(c1), c2(c2) {}
    bool val() override {
        return c1 || c2;
    }
};

struct Conjunction : Formula {
    Formula& c1; Formula& c2;
    Conjunction(Formula& c1, Formula& c2): c1(c1), c2(c2) {}
    bool val() override {
        return c1 && c2;
    }
};

struct Equivalence : Formula {
    Formula& c1; Formula& c2;
    Equivalence(Formula& c1, Formula& c2): c1(c1), c2(c2) {}
    bool val() override {
        return (bool)c1 == (bool)c2;
    }
};

struct Negation : Formula {
    Formula& c;
    Negation(Formula& c) : c(c) {}
    bool val() override {
        return !c;
    }
};

bool isletter(char c) {
    return 'a'<=c and c <= 'z';
}

Formula* Parse(char* b, char* e, Formula* vars) {
    if (isletter(*b) and b+1 == e)
        return vars + *b - 'a';
    if (*b=='~')
        auto* a = new Negation(*);
    return nullptr;
}

int main() {
    //       r || (p && q)
    bool _p = true, _q = true, _r = false;
    auto p = Variable(&_p);
    auto q = Variable(&_q);
    auto r = Variable(&_r);
    auto F1 = Conjunction(p,q);
    auto F2 = Alternative(r,F1);
    cout << F2.val();
}
