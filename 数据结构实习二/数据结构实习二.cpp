// 数据结构实习二.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#define OVERFLOWER -2
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 100
#define OK 1
#define ERROR 0
#define MAXN 100001
typedef char ElemType;
typedef int Status;

using namespace std;

//定义栈的顺序结构
typedef struct SqStack {
    Status* base;//栈底指针
    Status* top;//栈顶指针
    Status stacksize;//当前可用最大储存空间
};

//初始化一个空栈
Status InitStack(SqStack& S) {
    S.base = (Status *)malloc(sizeof(Status)*STACK_INIT_SIZE);//为栈分配一块空间
    if (!S.base) {//如果分配失败则出现提示
        cout << "创建空栈失败" << endl;
        exit(OVERFLOWER);
    }
    S.top = S.base;//使栈顶等于栈底 表示栈为空
    S.stacksize = STACK_INIT_SIZE;//栈初始大小为STACK_INIT_SIZE
    return OK;
}

//判断栈是否为空
Status StackEmpty(SqStack S) {
    if (S.base == S.top) {//若栈为空则返回true
        return OK;
    }
    else return ERROR;
}

//入栈
Status Push(SqStack &S,Status e) {//一定要有引用 因为这个函数里面涉及到对S的修改
    if ((S.top - S.base) >= S.stacksize) {//如果栈满则增加空间
        S.base = (Status*)realloc(S.base, S.stacksize + sizeof(Status) * STACKINCREMENT);//增加空间
        if (!S.base) exit(OVERFLOWER);//如果栈底指针为空则重新分配失败
        S.top = S.base + S.stacksize;//由于base指针的地址改变,现在要重新将栈顶指针定位到栈顶元素的下一位
        S.stacksize += STACKINCREMENT;//修改栈的储存空间
    }
    *S.top++ = e;//入栈，先赋值，再把栈顶指针后移
    return OK;
}

//获取栈顶元素
Status GetTop(SqStack S, Status& e) {
    if (StackEmpty(S)) {//若栈为空则出现提示
        cout << "栈为空，无栈顶元素" << endl;
        return ERROR;
    }
    else e = *(S.top-1);//提取栈顶元素
    return OK;
}

//出栈
Status Pop(SqStack &S, Status &e) {
    if (StackEmpty(S)) {
        cout << "栈为空，无栈顶元素" << endl;
        return ERROR;
    }
    S.top--;
    e = *S.top;//先减后弹
    return OK;
}

//求栈的长度
Status StackLength(SqStack S) {
    return S.top - S.base;
}

//清空栈
Status ClearStack(SqStack& S) {
    S.top = S.base;
    return OK;
}

//栈的销毁
Status DestroyStack(SqStack& S) {
    free(S.base);
    S.base = S.top = NULL;
    S.stacksize = 0;
    return OK;
}

//主要功能函数
Status longestValidParentheses(char* s) {
    Status max = 0, length = 0;
    SqStack S;
    InitStack(S);
    Push(S, -1);//先把-1放在栈底作为起算点
    //cout << "hh" << endl;
    for (int i = 0;i < strlen(s);i++) {//对字符串进行遍历
        //cout << i << endl;
        if (s[i] == '(') Push(S, i);//如果为（则入栈
        if (s[i] == ')') {//如果为）则讨论
            Status h = 0;
            GetTop(S, h);
            if (h==-1||s[h]==')') {//栈顶元素为-1或者）时，表明栈中已经没有与）配对的（了，所以将）直接入栈，作为下一次计算的起算点
                Push(S, i);
              // cout << "e" << endl;
            }
            else {
                Status a = 0;
                Pop(S, a);//弹出与之配对的（
               // cout << "a  " << a << endl;
                Status t = 0;
                GetTop(S, t);//获取起算点的数据
                //cout << "t  " << t << endl;
                length = i - t;//计算长度
               // cout <<"length  "<< length << endl;
                if (max < length) max = length;//取最大值
            }
        }
    }
    ClearStack(S);
    DestroyStack(S);
    return max;
}


int main()
{
    cout << "下面开始对栈功能的测试:" << endl;

    //创建空栈
    cout << "创建一个空栈" << endl;
    SqStack S;
    if(InitStack(S)) cout << "创建空栈成功" << endl;
    system("pause");

    //判断栈是否为空
    cout << "判断栈是否为空：" << endl;
    if (StackEmpty(S)) cout << "栈为空" << endl;
    else cout << "栈不为空" << endl;
    system("pause");

    //入栈
    cout << "入栈，我们输入十个数据测试：" << endl;
    for (int i = 0;i < 10;i++) {
        Status e;
        cin >> e;
        Push(S, e);
    }
    system("pause");

    //测试GetTop Pop StackLength等函数
    cout << "测试GetTop Pop StackLength等函数" << "   让我们不断出栈 并且获取新的栈顶元素 和新的栈长度 重复五次" << endl;
    if (!StackEmpty(S)) cout << "栈不为空" << endl;
    for (int i = 0;i < 5;i++) {
        Status e=0;
        Pop(S, e);
        cout << "弹出元素为：" << e << " " << endl;
        Status f=0;
        GetTop(S, f);
        cout << "新的栈的栈顶元素为：" << f << endl;
        cout << "   剩余栈长度为: " << StackLength(S) << endl;
    }
    if (!StackEmpty(S)) cout << "栈不为空" << endl;
    system("pause");

    //清空栈
    cout << "清空栈" << endl;
    ClearStack(S);
    if (StackEmpty(S)) cout << "清空成功" << endl;
    system("pause");

    //销毁栈
    cout << "销毁栈" << endl;
    DestroyStack(S);
    if (S.base == NULL) cout << "销毁成功" << endl;
    system("pause");

    cout << endl;
    cout << endl;
    cout << endl;
//开始解决 求有效括号子串的最大长度
    cout << "下面我们开始解决求有效括号子串的最大长度：" << endl;
    cout << "请输入只含有(和)的字符串: ";
    char s[MAXN];
    cin >> s;
    //cout << "size::" << strlen(s)<< endl;
    cout << "最大长度为：" << longestValidParentheses(s) << endl;
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
