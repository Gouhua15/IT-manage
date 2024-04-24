#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

//第一部分：词法分析
string keywords[8] = {"main", "if", "else", "for", "while", "int", "void", "printf"};
vector<string> inputString;
int digitalf(char ch)
{
    return (ch >= '0' && ch <= '9');
}

int letterf(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

int boundaryf(char ch)
{
    return (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ',' || ch == ';');
}

int keyf(string g)
{
    // keywords包含了关键字列表
    for (int i = 0; i < 8; i++)
    {
        if (g == keywords[i])
            return 1;
    }
    return 0;
}

int operatorf(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '!' || ch == '<' || ch == '>');
}

void LexicalAnalysis(string s)
{
    for (int i = 0; i < s.length();)
    {
        if (boundaryf(s[i]))
        {
            cout << "(boundary," << s[i] << ")" << endl;
            inputString.push_back(string(1, s[i])); // 使用 string(1, s[i]) 来构造单个字符的字符串
            i++;
        }
        else if (digitalf(s[i]))
        {
            string g = "";
            while (digitalf(s[i]))
            {
                g += s[i++];
            }
            cout << "(integer," << g << ")" << endl;
            inputString.push_back(g);
        }
        else if (letterf(s[i]) || s[i] == '_')
        {
            string g = "";
            while (letterf(s[i]) || s[i] == '_' || digitalf(s[i]))
            {
                g += s[i++];
            }
            if (keyf(g))
            {
                cout << "(keyword," << g << ")" << endl;
                inputString.push_back(g);
            }
            else
            {
                cout << "(identifier," << g << ")" << endl;
                inputString.push_back(g);
            }
        }
        else if (operatorf(s[i]))
        {
            if (i + 1 < s.length() && s[i + 1] == '=')
            {
                cout << "(operator," << s[i] << s[i + 1] << ")" << endl;
                string t = string(1, s[i]) + s[i + 1]; // 使用 string(1, s[i]) 来构造单个字符的字符串
                inputString.push_back(t);
                i++;
            }
            else
            {
                string t = string(1, s[i]); // 使用 string(1, s[i]) 来构造单个字符的字符串
                cout << "(operator," << s[i] << ")" << endl;
                inputString.push_back(t);
            }
            i++;
        }
    }
}


//第二部分：语法分析

int currentIndex = 1;
void Program();
void DS(); // 声明序列
void DV(); // 声明变量
void SS(); // 语句序列
void AssignValue();
void AE();     // 算术表达式
void item();   // 项
void factor(); // 因子
void Identifier();
void letter(); // 字母
void Number();
void Keywordmain();
void Keywordint();
void Keywordprint();
void output1();
void Program()
{
    if (inputString[currentIndex] == "main")
    {
        cout << "<程序> --> <main关键字> () { <声明序列> <语句序列>} " << endl;
        currentIndex++;
        Keywordmain();
        if (inputString[currentIndex] == "(" && inputString[++currentIndex] == ")" && inputString[++currentIndex] == "{")
        {
            currentIndex++;
            if (inputString[currentIndex] == "int")
            {
                DS();
                SS();
                if (inputString[currentIndex] == "}")
                    currentIndex++;
                else
                    return;
            }
        }
    }
}

void DS()
{
    cout << "<声明序列> --> <声明语句> <声明序列>" << endl;
    DV();
    if (inputString[currentIndex] == "int")
        DS();
    else
        cout << "<声明序列> --> <空>" << endl;
}

void SS()
{
    if (inputString[currentIndex] >= "a" && inputString[currentIndex] <= "z" && inputString[currentIndex] != "print")
    {
        cout << "<语句序列> --> <赋值语句> <语句序列>" << endl;
        AssignValue();
        SS();
    }
    else if (inputString[currentIndex] == "print")
    {
        cout << "<语句序列> --> <输出语句> <语句序列>" << endl;
        output1();
        SS();
    }
    else
        cout << "<语句序列> --> <空>" << endl;
}

void DV()
{
    cout << "<声明语句> --> <int关键字> <标识符>" << endl;
    Keywordint();
    Identifier();
}

void Identifier()
{
    cout << "<标识符> --> " << inputString[currentIndex++] << endl;
    if (inputString[currentIndex] == ";")
        currentIndex++;
    else
        return;
}

void AssignValue()
{
    cout << "<赋值语句> --> <标识符> = <算数表达式>" << endl;
    Identifier();
    if (inputString[currentIndex] == "=")
        currentIndex++;
    else
        return;

    AE();
    if (inputString[currentIndex] == ";")
        currentIndex++;
    else
        return;
}

void AE()
{
    if (inputString[currentIndex + 1] == "+")
        cout << "<算数表达式> --> <项> + <算数表达式>" << endl;
    else if (inputString[currentIndex + 1] == "*")
        cout << "<算数表达式> --> <项> * <算术表达式>" << endl;
    else if (inputString[currentIndex + 1] == "-")
        cout << "<算数表达式> --> <项> - <算术表达式>" << endl;
    else if (inputString[currentIndex + 1] == "/")
        cout << "<算数表达式> --> <项> / <算术表达式>" << endl;
    else
        cout << "<算数表达式> --> <项>" << endl;

    item();
    if (inputString[currentIndex] == "+" || inputString[currentIndex] == "*" || inputString[currentIndex] == "-" || inputString[currentIndex] == "/")
    {
        currentIndex++;
        AE();
    }
}

void output1()
{
    cout << "<输出语句> --> <print关键字>(<标识符>)" << endl;
    Keywordprint();
    if (inputString[currentIndex] == "(")
    {
        currentIndex++;
        Identifier();
        if (inputString[currentIndex] == ")")
        {
            currentIndex++;
            if (inputString[currentIndex] == ";")
                currentIndex++;
        }
    }
}

void Keywordprint()
{
    cout << "<print关键字> -->" << inputString[currentIndex++] << endl;
}

void item()
{
    cout << "<项> --> <因子>" << endl;
    factor();
}

void factor()
{
    if (inputString[currentIndex] >= "0" && inputString[currentIndex] <= "9")
    {
        cout << "<因子> --> <数字>" << endl;
        Number();
    }
    else
    {
        cout << "<因子> --> <标识符>" << endl;
        Identifier();
    }
}

void letter()
{
    cout << "<字母> --> " << inputString[currentIndex] << endl;
    currentIndex++;
}

void Number()
{
    cout << "<数字> --> " << inputString[currentIndex] << endl;
    currentIndex++;
}

void Keywordmain()
{
    cout << "<main关键字> --> main" << endl;
}

void Keywordint()
{
    cout << "<int关键字> --> int" << endl;
    currentIndex++;
}

//第三部分：语义中间代码生成

// 查找运算符在 inputString 中的位置
int findoperator(int ind)
{
    static const std::unordered_set<std::string> operators = {"+", "=", "*", "-", "/"};

    for (int i = ind; i < inputString.size(); i++)
    {
        if (operators.count(inputString[i]))
            return i;
    }
    return -1;
}

// 生成中间代码
void MiddleCode()
{
    std::vector<std::string> q; 
    static const std::unordered_set<std::string> operators = {"+", "=", "*", "-", "/"};
    int num = 0;         
    int statementId = 1; 

    for (int index = 0; index < inputString.size();)
    {
        index = findoperator(index + 1); // 查找下一个运算符的位置
        if (index == -1)
            break;

        std::string prev = inputString[index - 1]; // 当前运算符前一个元素
        std::string next = inputString[index + 1]; // 当前运算符后一个元素

        if (inputString[index] == "=" && operators.count(inputString[index + 2]) == 0)
        {
            q.push_back(prev);
            q.push_back(next);
            printf("%d(=,%s,_,%s)\n", statementId, q[num++].c_str(), q[num++].c_str());
            num -= 1;
            q.pop_back();
            q.pop_back();
            q.push_back(prev);
            statementId++;
        }
        else if (operators.count(inputString[index]))
        {
            printf("%d(%s,%s,%s,T%d)\n", statementId++, inputString[index].c_str(), prev.c_str(), next.c_str(), num);
            printf("%d(=,T%d,_,%s)\n", statementId++, num++, inputString[index - 3].c_str());
        }
    }
}

int find2(int ind)
{
    for (int i = ind; i < inputString.size(); i++)
    {
        if (inputString[i] == "=" && (inputString[i + 2] == "+" || inputString[i + 2] == "*" || inputString[i + 2] == "-" || inputString[i + 2] == "/"))
            return i;
    }
    return -1;
}

struct Node
{
    int left = -1;
    int right = -1;
    char id;
    vector<char> var;
} node[111];

int n;

int find_char(int i, char c) // 查找c是否存在
{
    return std::find(node[i].var.begin(), node[i].var.end(), c) != node[i].var.end();
}

int add_node(char c) // 添加节点
{
    for (int i = currentIndex - 1; i >= 0; i--)
    {
        if (node[i].id == c || find_char(i, c)) 
            return i;
    }
    node[currentIndex].id = c; 
    return currentIndex++;
}

void add_operator(char op, char c, int l, int r) // 添加运算符
{
    for (int i = currentIndex - 1; i >= 0; i--)
    {
        if (node[i].left == l && node[i].right == r && node[i].id == op)
        {
            node[i].var.push_back(c);
            return;
        }
    }
    node[currentIndex].id = op; 
    node[currentIndex].left = l;
    node[currentIndex].right = r;
    node[currentIndex].var.push_back(c);
    currentIndex++;
}

int flag[150];

void dfs(int i) // 深度优先搜索，遍历算术表达式
{
    if (node[i].left != -1) 

    {
        flag[i] = 1;
        dfs(node[i].left);
        dfs(node[i].right);
    }
}

char a[150][15];

char ans[150][15];

string getMess(int ind)
{
    string Mess = "";
    if (ind != -1)
    {
        Mess += inputString[ind - 1];
        Mess += inputString[ind];
        Mess += inputString[ind + 1];
        Mess += inputString[ind + 2];
        Mess += inputString[ind + 3];
    }
    return Mess;
}
int Shizisum = 0;

vector<string> ResultDag;

void DAG()
{
    int ind = 0;
    cout << "未优化前:" << endl;
    vector<char> temp;
    while (ind != -1)
    {
        ind = find2(ind + 1);
        if (ind == -1)
            break;
        string Temp = getMess(ind);
        cout << Temp << endl;
        int l = add_node(Temp[2]);
        int r = add_node(Temp[4]);
        add_operator(Temp[3], Temp[0], l, r);
        temp.push_back(Temp[0]);
    }
    for (int i = 0; i < currentIndex; i++)
    {
        if (node[i].left != -1) 
        {
            ans[i][0] = node[i].var[0];
            ans[i][1] = '=';
            Node ll = node[node[i].left];
            Node rr = node[node[i].right];
            ans[i][2] = ll.var.size() > 0 ? ll.var[0] : ll.id; // 仅对二元运算进行优化
            ans[i][3] = node[i].id;
            ans[i][4] = rr.var.size() > 0 ? rr.var[0] : rr.id;
        }
    }
    for (char c : temp)
    {
        for (int i = currentIndex - 1; i >= 0; i--)
        {
            if (ans[i][0] == c)
            {
                dfs(i);
                break;
            }
        }
    }
    cout << endl;
    cout << "优化后:" << endl;
    for (int i = 0; i < currentIndex; i++)
    {
        if (flag[i])
        {
            cout << ans[i] << endl;
            ResultDag.push_back(ans[i]);
        }
    }
}

void setWindowStyle()
{
    system("简单编译软件");
    system("color 09");
}

void show()
{

    cout << "简单编译软件,功能如下：" << endl;
    cout << "1：词法分析" << endl;
    cout << "2：语法分析" << endl;
    cout << "3：语义分析" << endl;
    cout << "4：中间代码优化" << endl;

    printf("请输入所需操作，并回车\n");
    getchar();
    fflush(stdin);
}

int main()
{
    setWindowStyle();
    show(); // 初始化
    ifstream myfile("code.txt");
    string s;
    while (1)
    {
        cout << "请输入所需操作，并回车";
        int x;
        cin >> x;
        switch (x)
        {
        case 1:
            while (myfile >> s)
                LexicalAnalysis(s);
            show();
            break;
        case 2:
            Program();
            if (currentIndex == inputString.size())
            {
                cout << "文法无误" << endl;
                show();
            }
            else
            {
                cout << "文法出错" << endl;
            }
            break;

        case 3:
            MiddleCode();
            show();
            break;
        case 4:
            DAG();
            break;
        case 5:
            return 0;
        }
    }
}
