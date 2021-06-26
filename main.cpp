#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <stack>

#define MIN_ROM -3888
#define MAX_ROM 3888
#define ERROR 404

using namespace std;

typedef map<const char, const unsigned> Roman_symbols;
Roman_symbols r_s;
void is_corr_char_order(const string &roman_ex);
void init_tables();
void delete_spaces(string &roman_ex);
string to_arab_ex(string &roman_ex);
int to_shorten_arab_ex(string &roman_ex);
void to_roman_ex(double arab_res);

int     main(int argc, char *argv[]) {
    int arab_res = 0;
    string roman_ex;
    while (getline(cin, roman_ex)) {
        try {
            is_corr_char_order(roman_ex);
        } catch (const logic_error &l_err) {
            cout << l_err.what() << endl;
            continue;
        }
        init_tables();
        delete_spaces(roman_ex);
        to_arab_ex(roman_ex);
        //cout << roman_ex << endl;
        if (to_shorten_arab_ex(roman_ex) == ERROR) { continue;; }
        else {
            to_shorten_arab_ex(roman_ex);
            arab_res = to_shorten_arab_ex(roman_ex);
            //cout << arab_res << endl;
            to_roman_ex(arab_res);
        }
    }
}

void is_corr_char_order(const string &roman_ex) {
    unsigned l_br = 0;
    unsigned r_br = 0;
    for (int i = 0; i < roman_ex.length(); ++i) {
        if (roman_ex[i] == '(' ) { l_br++; }
        if (roman_ex[i] == ')' ) { r_br++; }
        if ((roman_ex[i] == roman_ex[i + 1]) &&
            (roman_ex[i] == '+' || roman_ex[i] == '-' || roman_ex[i] == '*' ||
             roman_ex[i] == '/')) { throw logic_error("error: Invalid sequence of arithmetic symbols"); }
        if ((roman_ex[i] == roman_ex[i + 1] && roman_ex[i] == roman_ex[i + 2] && roman_ex[i] == roman_ex[i + 3]) && (roman_ex[i] == 'I' || roman_ex[i] == 'V' || roman_ex[i] == 'X' || roman_ex[i] == 'L' || roman_ex[i] == 'C' || roman_ex[i] == 'D' || roman_ex[i] == 'M' || roman_ex[i] == 'Z')) {
            throw logic_error("error: Incorrect sequence of Roman characters");
        }
        if ((roman_ex[i] == roman_ex[i + 1]) && (roman_ex[i] == 'V' || roman_ex[i] == 'L' ||
                                                 roman_ex[i] == 'D' || roman_ex[i] == 'Z')) {
            throw logic_error("error: Incorrect sequence of Roman characters");
        }
        if (roman_ex[i] == 'Z' && (roman_ex[i + 1] == 'I' || roman_ex[i + 1] == 'V' || roman_ex[i + 1] == 'X' || roman_ex[i + 1] == 'L' || roman_ex[i + 1] == 'C' || roman_ex[i + 1] == 'D' || roman_ex[i + 1] == 'M' )) { throw logic_error("error: Incorrect sequence of parentheses"); }
    }
    if (l_br != r_br) { throw logic_error("error: Incorrect sequence of parentheses"); }
}

void init_tables() {
    r_s.emplace('I', 1);
    r_s.emplace('V', 5);
    r_s.emplace('X', 10);
    r_s.emplace('L', 50);
    r_s.emplace('C', 100);
    r_s.emplace('D', 500);
    r_s.emplace('M', 1000);
    r_s.emplace('Z', 0);
}

void delete_spaces(string &roman_ex) {
    for (int i = 0; i < roman_ex.length(); ++i) {
        while (roman_ex[i] == ' ') {
            roman_ex.erase(i, 1);
        }
    }
}

string to_arab_ex(string &roman_ex) {
    for (int unsigned long i = 0; i < roman_ex.length(); ++i) {
        for (auto t : r_s) {
            if (roman_ex[i] == t.first) {
                if (r_s.find(roman_ex[i + 1]) == r_s.end()) {
                    roman_ex.erase(i, 1);
                    roman_ex.insert(i, to_string(t.second));
                    i += to_string(t.second).length() - 1;
                }
                else {
                    unsigned temp = r_s.find(roman_ex[i])->second;
                    unsigned long curr = i;
                    while (r_s.find(roman_ex[i + 1]) != r_s.end()) {
                        if (temp < r_s.find(roman_ex[i + 1])->second) {
                            temp = r_s.find(roman_ex[i + 1])->second - temp;
                            roman_ex.erase(curr, ((i + 1 - curr) + 1));
                            roman_ex.insert(curr, to_string(temp));
                            i = curr + to_string(temp).length() - 1;
                        }
                        else if (temp == r_s.find(roman_ex[i + 1])->second) {
                            temp = temp + r_s.find(roman_ex[i + 1])->second;
                            roman_ex.erase(curr, ((i + 1 - curr) + 1));
                            roman_ex.insert(curr, to_string(temp));
                            i = curr + to_string(temp).length() - 1;
                        }
                        else if (temp > r_s.find(roman_ex[i + 1])->second) {
                            temp = temp + r_s.find(roman_ex[i + 1])->second;
                            roman_ex.erase(curr, ((i + 1 - curr) + 1));
                            roman_ex.insert(curr, to_string(temp));
                            i = curr + to_string(temp).length() - 1;
                        }
                    }
                }
            }
        }
    }
    return roman_ex;
}

struct  Significance{
    char type;
    long double value = 0;
};

void Operators(stack<Significance> &Stack_num, stack<Significance> &Stack_op, Significance &Sign) {
    long double a, b, c;
    a = Stack_num.top().value;
    Stack_num.pop();
   
    switch (Stack_op.top().type) {
        case '+':
            b = Stack_num.top().value;
            Stack_num.pop();
            c = b + a;
            Sign.type = '0';
            Sign.value = c;
            Stack_num.push(Sign);
            break;
        case '*':
            b = Stack_num.top().value;
            Stack_num.pop();
            c = b * a;
            Sign.type = '0';
            Sign.value = c;
            Stack_num.push(Sign);
            break;
        case '/':
            b = Stack_num.top().value;
            Stack_num.pop();
            if (a == 0) {
                throw logic_error("error: Can't be divided by zero");
            }
            else {
                c = b / a;
                Sign.type = '0';
                Sign.value = c;
                Stack_num.push(Sign);
                break;
            }
        case '-':
            b = Stack_num.top().value;
            Stack_num.pop();
            c = b - a;
            Sign.type = '0';
            Sign.value = c;
            Stack_num.push(Sign);
            break;
            
        default:
            break;
    }
    Stack_op.pop();
}

int get_Priority(char &c) {
    if (c == '+' || c == '-') { return 1; }
    else if (c == '*' || c == '/') { return 2; }
    else { return 0; }
}

int to_shorten_arab_ex(string &roman_ex) {
    stack<Significance> Stack_num;
    stack<Significance> Stack_op;
    Significance Sign;
    bool sign = 1;
    int i;
    for (i = 0; i < roman_ex.length(); ++i) {
        if ((roman_ex[i] >= '0' && roman_ex[i] <= '9') || (roman_ex[i] == '-' && sign == 1)) {
            while (roman_ex[i] >= '0' && roman_ex[i] <= '9') {
                Sign.type = '0';
                Sign.value = Sign.value * 10 + roman_ex[i] - '0';
                ++i;
            }
            Stack_num.push(Sign);
            sign = 0;
        }
        if (roman_ex[i] == '+' || (roman_ex[i] == '-' && sign == 0) || roman_ex[i] == '*' || roman_ex[i] == '/') {
            if ((Stack_op.size() != 0 && get_Priority(roman_ex[i]) > get_Priority(Stack_op.top().type)) || Stack_op.size() == 0) {
                Sign.type = roman_ex[i];
                Sign.value = 0;
                Stack_op.push(Sign);
            }
            else if (Stack_op.size() != 0 && get_Priority(roman_ex[i]) <= get_Priority(Stack_op.top().type)) {
                try {
                    Operators(Stack_num, Stack_op, Sign);
                    --i;
                } catch (const logic_error &l_err) {
                    cout << l_err.what() << endl;
                    return ERROR;
                }
            }
        }
        if (roman_ex[i] == '(') {
            Sign.type = roman_ex[i];
            Sign.value = 0;
            Stack_op.push(Sign);
        }
        if (roman_ex[i] == ')') {
            while (Stack_op.top().type != '(') {
                try {
                    Operators(Stack_num, Stack_op, Sign);
                } catch (const logic_error &l_err) {
                    cout << l_err.what() << endl;
                    return ERROR;
                }
            }
            Stack_op.pop();
        }
    }
    while (Stack_op.size() != 0) {
        try {
            Operators(Stack_num, Stack_op, Sign);
            --i;
        } catch (const logic_error &l_err) {
            cout << l_err.what() << endl;
            return ERROR;
        }
    }
    try {
        if (Stack_num.top().value < MIN_ROM || Stack_num.top().value > MAX_ROM) {
            throw overflow_error("error: Вefining an integer overflow");
        }
    } catch (const overflow_error &of_err) {
        cout << of_err.what() << endl;
        return 0;
    }
    
    /*
    try {
        if (Stack_num.top().value - int(Stack_num.top().value) != 0) {
            throw underflow_error("error: Вefining an integer underflow");
        }
    } catch (const underflow_error &uf_err) {
        cout << uf_err.what() << endl;
        return 0;
    }*/
    { return int(Stack_num.top().value); }
}

string convert(int digit, string low, string mid, string high)
{
    if (digit == 1) { return low; }
    if (digit == 2) { return low + low; }
    if (digit == 3) { return low + low + low; }
    if (digit == 4) { return low + mid; }
    if (digit == 5) { return mid; }
    if (digit == 6) { return mid + low; }
    if (digit == 7) { return mid + low + low; }
    if (digit == 8) { return mid + low + low + low; }
    if (digit == 9) { return low + high; }
    if (digit == 0) { return ""; }
    return  0;;
}

void to_roman_ex(double arab_res) {
    if (arab_res == 0) { cout << 'Z' << endl; }
    else {
        bool sign = 1;
        string roman_res[4];
        if (arab_res < 0) {
            arab_res = arab_res * -1;
        }
        else {sign = 0; }
        int thous = arab_res / 1000;
        int hund = int(arab_res) % 1000 / 100;
        int tens = int(arab_res) % 100 / 10;
        int ones = int(arab_res) % 10 / 1;
        roman_res[0] = convert(thous, "M", "M", "M");
        roman_res[1] = convert(hund, "C", "D", "M");
        roman_res[2] = convert(tens, "X", "L", "C");
        roman_res[3] = convert(ones, "I", "V", "X");
        if (sign == 1) { cout << '-' ;}
        cout << roman_res[0] << roman_res[1] << roman_res[2] << roman_res[3] << endl;
    }
}
