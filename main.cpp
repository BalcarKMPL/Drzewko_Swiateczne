//Jakub Mazur
#include <iostream>
//#include <typeinfo>
#define ifprint false
using namespace std;

void inline GlobalPrint(const char* str){
    if (ifprint) cout << str << endl;
}

struct FormulaOperator {
private:
    inline bool isletter(char c) {
        return 'a' <= c and c <= 'z';
    }
    inline bool is1op(char c){
        return c == '~';
    }
    inline bool is2op(char c){
        return c == '|' or c == '&' or c == '>' or c == '=';
    }

    struct Formula {
        virtual bool val() = 0;
        explicit virtual operator bool() final {
            return val();
        }
        virtual void print() = 0;
        virtual ~Formula() {};
    };
    struct Variable : Formula {
        bool* ptr;
        explicit Variable(bool* ptr): ptr(ptr) {}
        bool val() override {
            return *ptr;
        }
        ~Variable() override { GlobalPrint("Variable destructed"); }
        friend bool operator==(const Variable& a, const Formula& b) {
            if (typeid(a) == typeid(b))
                return a.ptr == dynamic_cast<const Variable&>(b).ptr;
            return false;
        }
        void print() {
            cout << this << " \tVAR\t" << ptr << endl;
        }
    };
    struct Implication : Formula {
        Formula* c1; Formula* c2;
        Implication(Formula* c1, Formula* c2): c1(c1), c2(c2) {}
        bool val() override {
            return !c1->val() || c2->val();
        }
        void print(){
            cout << this << "\tIMP\t" << c1 << "\t" << c2 << endl;
            c1->print();
            c2->print();
        }
        ~Implication() override { GlobalPrint("Implication destructed"); delete c1; delete c2; }
    };
    struct Alternative : Formula {
        Formula* c1; Formula* c2;
        Alternative(Formula* c1, Formula* c2): c1(c1), c2(c2) {}
        bool val() override {
            bool v1 = c1->val(), v2 = c2 ->val();
            return v1 || v2;
        }
        void print(){
            cout << this << "\tALT\t" << c1 << "\t" << c2 << endl;
            c1->print();
            c2->print();
        }
        ~Alternative() override { GlobalPrint("Alternative destructed"); delete c1; delete c2;}
    };
    struct Conjunction : Formula {
        Formula* c1; Formula* c2;
        Conjunction(Formula* c1, Formula* c2): c1(c1), c2(c2) {}
        bool val() override {
            bool v1 = c1->val(), v2 = c2 ->val();
            return v1 && v2;
        }
        void print(){
            cout << this << "\tCON\t" << c1 << "\t" << c2 << endl;
            c1->print();
            c2->print();
        }
        ~Conjunction() override { GlobalPrint("Conjunction destructed"); delete c1; delete c2;}
    };
    struct Equivalence : Formula {
        Formula* c1; Formula* c2;
        Equivalence(Formula* c1, Formula* c2): c1(c1), c2(c2) {}
        bool val() override {
            bool v1 = c1->val(), v2 = c2 ->val();
            return v1 == v2;
        }
        void print(){
            cout << this << "\tEQN\t" << c1 << "\t" << c2 << endl;
            c1->print();
            c2->print();
        }
        ~Equivalence() override { GlobalPrint("Equivalence destructed"); delete c1; delete c2;}
    };
    struct Negation : Formula {
        Formula* c;
        explicit Negation(Formula* c) : c(c) {}
        bool val() override {
            bool v = c->val();
            return !v;
        }
        void print(){
            cout << this << "\tNEG\t" << c << endl;
            c->print();
        }
        ~Negation() override { GlobalPrint("Negation destructed"); delete c; }
    };
    struct Identity : Formula {
        Formula* c;
        Identity(Formula* c) : c(c) {}
        bool val() override {
            return c->val();
        }
        void print(){
            cout << this << "\tIDN\t" << c << endl;
            c->print();
        }
        ~Identity() override { GlobalPrint("Identity destructed"); }
    };
    struct True : Formula {
        True() {}
        bool val() override { return true; }
        void print(){
            cout << this << "\tTRUE" << endl;
        }
        ~True() override {}
    };
    struct False : Formula {
        False() {}
        bool val() override { return false; }
        void print(){
            cout << this << "\tFALSE" << endl;
        }
        ~False() override {}
    };

    Formula* Parse(const char* const b, const char* const e, Variable** vars) {
        if (isletter(*b) and b+1 == e)
            return new Identity(vars[*b - 'a']);
        if (*b=='~')
            return new Negation(Parse(b+1,e,vars));
        if (*b=='('){
            int nawiasow_count = -1; char c;
            for (int i = 0; b[i] != 0; ++i) {
                c = b[i];
                if (c == '(') nawiasow_count++;
                if (c == ')') nawiasow_count--;
                if (nawiasow_count == 0 and is2op(c)) {
                    const char* b1 = b + 1;
                    const char* e1 = b + i;
                    const char* b2 = b + i + 1;
                    const char* e2 = e - 1;
                    switch (c) {
                        case '|':
                            return new Alternative(Parse(b1, e1, vars), Parse(b2,e2,vars));
                        case '&':
                            return new Conjunction(Parse(b1, e1, vars), Parse(b2,e2,vars));
                        case '>':
                            return new Implication(Parse(b1, e1, vars), Parse(b2,e2,vars));
                        case '=':
                            return new Equivalence(Parse(b1, e1, vars), Parse(b2,e2,vars));
                        default:
                            cout << "Coś się zjebało 1...";
                            break;
                    }
                }
            }
        }
        cout << "Coś się zjebało 2...";
        return nullptr;
    }

    Formula* Parse(const char* b, Variable** vars) {
        int i;
        for (i = 0; b[i] != 0; i++);
        return Parse(b, b+i, vars);
    }
    Formula* formula;
    Variable** Vars;
    bool* vars;
    bool* usedVars;
    char* varsNames;
    bool** varsRef;
    char* blocks;
    int usedVarsCount;
    static constexpr int len = 'z' - 'a' + 1;
    int pow2(int x) {
        if (x == 0)
            return 1;
        return 2*pow2(x-1);
    }
    void InitializeVars(){
        for (int i = 0; i < usedVarsCount; ++i) {
            if (blocks[varsNames[i] - 'a'] == '1') {
                varsRef[i][0] = true;
            }
            else {
                varsRef[i][0] = false;
            }
        }
    }
    bool IncrementVars() {
        for (int j = 0; j < usedVarsCount; ++j) {
            if (blocks[varsNames[j] - 'a'] == '0' or blocks[varsNames[j] - 'a'] == '1')
                continue;
            if (varsRef[j][0]){
                varsRef[j][0] = false;
            }
            else {
                varsRef[j][0] = true;
                return true;
                break;
            }
        }
        return false;
    }
public:
    FormulaOperator(const char* const f) {
        usedVarsCount = 0;
        vars = new bool[len] {0};
        Vars = new Variable*[len] {0};
        usedVars = new bool[len] {0};
        blocks = new char[len] {0};
        for (int i = 0; i < len; ++i) {
            Vars[i] = new Variable(vars + i);
            usedVars[i] = false;
            blocks[i] = ' ';
        }
        for (int i = 0; f[i] != 0; ++i) {
            if (isletter(f[i]))
                usedVars[f[i]-'a'] = true;
        }
        for (int i = 0; i < len; ++i) {
            if (usedVars[i]) usedVarsCount++;
        }
        varsNames = new char[usedVarsCount] {0};
        varsRef = new bool * [usedVarsCount] {0};
        int k = 0;
        for (int i = 0; i < len; ++i) {
            if(usedVars[i]) {
                varsNames[k] = 'a' + i;
                varsRef[k] = vars + i;
                k++;
            }
        }
        formula = Parse(f, Vars);
        InitializeVars();
    }
    bool CheckTautology(){
        InitializeVars();
        do {
            if (not formula->val()) return false;
        } while (IncrementVars());
        return true;
    }
    int CountTrues(){
        InitializeVars();
        int v = 0;
        do {
            if (formula->val()) v++;
        } while (IncrementVars());
        return v;
    }
    int CountFalses(){
        InitializeVars();
        int v = 0;
        do {
            if (not formula->val()) v++;
        } while (IncrementVars());
        return v;
    }
//    void PrintTruthTable() {
//        InitializeVars();
//        for (int j = 0; j < usedVarsCount; ++j) {
//            cout << varsNames[j] << "\t";
//        }
//        cout << endl;
//        do {
//            for (int j = 0; j < usedVarsCount; ++j) {
//                cout << varsRef[j][0] << "\t";
//            }
//            cout << formula->val() << endl;
//        } while (IncrementVars());
//    }
    void Block(char var, bool val) {
        blocks[var - 'a'] = (val ? '1' : '0');
    }
//    void Block(char var, char val) {
//        blocks[var - 'a'] = val;
//    }
//    void Print(){
//        formula->print();
//    }
    ~FormulaOperator(){
        delete [] vars;
        delete [] usedVars;
        delete [] varsNames;
        delete [] varsRef;
        delete [] blocks;
        for (int i = 0; i < len; ++i) {
            delete Vars[i];
        }
        delete [] Vars;
        // tu mogą być problemy, bo Variable używne przez formula są usunięte dwa razy
        delete formula;
    }
};

int main() {
    char command, var;
    char formula[1000];
    while(true){
        cin >> command;
        if (command == 'W' or command == 'K')
            return 0;
        if (command == 'T') {
            cin >> formula;
            auto* fo = new FormulaOperator(formula);
            cout << (fo->CheckTautology() ? "TAK" : "NIE") << endl;
            delete fo;
        }
        if (command == 'P') {
            cin >> formula;
            auto* fo = new FormulaOperator(formula);
            cout << fo->CountTrues() << endl;
            delete fo;
        }
        if (command == 'F') {
            cin >> formula;
            auto* fo = new FormulaOperator(formula);
            cout << fo->CountFalses() << endl;
            delete fo;
        }
        if (command == '1') {
            cin >> var;
            cin >> formula;
            auto* fo = new FormulaOperator(formula);
            fo->Block(var, true);
            cout << (fo->CheckTautology() ? "TAK" : "NIE") << endl;
            delete fo;
        }
        if (command == '0') {
            cin >> var;
            cin >> formula;
            auto* fo = new FormulaOperator(formula);
            fo->Block(var, false);
            cout << (fo->CheckTautology() ? "TAK" : "NIE") << endl;
            delete fo;
        }
    }

//    char b[] = "(r|q)";
//    auto* fo = new FormulaOperator(b);
//    fo->PrintTruthTable();
//    fo->Block('p',false);
//    cout << endl;
//    fo->PrintTruthTable();
}
