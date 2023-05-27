// 数据结构实习三.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#define MAXN 100001
using namespace std;

typedef struct{
    int weight;//权值
    int parent, lchild, rchild;//双亲、左孩子、右孩子
}HTNode,*HuffmanTree;//动态分配数组存储哈夫曼树

typedef char** HuffmanCode;//动态分配数组存储哈夫曼编码

void select(HuffmanTree HT, int n, int& s1, int& s2) {//搜索未进入哈夫曼树的权最小的两个节点
    int min = MAXN;
    for (int i = 1;i <= n;i++) {
        if (HT[i].parent == 0) {//只搜索双亲为0的结点 即未进入树的
            if (HT[i].weight < min) {
                s1 = i;
                min = HT[i].weight;
            }
        }
    }
    
    HT[s1].parent = -1;//将已经搜索出来的结点标志一下 不然第二次搜索会再次遍历
    min = MAXN;
    for (int i = 1;i <= n;i++) {
        if (HT[i].parent == 0) {
            if (HT[i].weight < min) {
                s2 = i;
                min = HT[i].weight;
            }
        }
    }
    
}

void HuffumanCoding(HuffmanTree& HT, HuffmanCode& HC, int* w, int n) {//w存放n个字符的权值，构造哈夫曼树HT，并求出n个字符的哈夫曼编码HC
    if (n <= 1) {
        cout << "字符个数过少，无法构造" << endl;
        return;
    }

    int m = 2 * n - 1;//哈弗曼树节点数为2*n-1
    HT = new HTNode[m+1];//分配m+1个节点 0号位置不用
    //哈弗曼树的初始化
    HTNode* p = HT;//设置一个遍历指针 以免改变原来的哈弗曼树
    p++;//0号位不用
    int i = 1;//从一号位开始遍历
    for (;i <= n;i++, p++) { *p = { *(w+i-1),0,0,0 }; }//1到n个节点为叶子节点 有权值
    for (;i <= m;i++, p++) *p = { 0,0,0,0 };//后面所有节点无权值

    //构造哈弗曼树
    for (i = n + 1;i <= m;i++) {
     
        int s1 = 0;
        int s2 = 0;
        select(HT, i - 1, s1, s2);
        HT[s1].parent = i;//两个节点的双亲设为i
        HT[s2].parent = i;
        HT[i].lchild = s1;//双亲的孩子设为这两个结点
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;//双亲的权值为孩子结点权值之和
        
    }

    //从叶子到根逆向求每个字符的哈夫曼编码
    HC = new char* [n + 1];//分配n个字符的哈夫曼编码的头指针
    char* cd = new char[n];//分配编码空间 每个字符的哈夫曼编码最多只有n-1个
    cd[n - 1] = '\0';//编码结束符
    for (i = 1;i <= n;i++) {//遍历每一个叶子节点 求他们的哈夫曼编码
        int start = n - 1;//逆序求编码
        
        for (int c = i, f = HT[i].parent;f != 0;c = f, f = HT[f].parent) {//不断地沿双亲向上爬直到双亲为0 即到达根节点
           
            if (HT[f].lchild == c) { cd[--start] = '0'; }//如果为左孩子则编码为0
            else { cd[--start] = '1'; }//如果为右孩子则编码为1 
           
        }
        HC[i] = new char[n - start];
        //为第i个叶子节点分配编码字符数组 长度为n-start
        strcpy(HC[i], &cd[start]);//储存编码
        } 
    delete []cd;//释放临时空间
    }
   


void HuffmanTranslation(HuffmanTree HT,char code[],int n,char cd[]) {
    int root = 2 * n-1;
   
        for (int i = 0;code[i] != '\0';i++) {
            int j = 0;
            while (HT[root].lchild&&HT[root].rchild) {
                
                if (code[i+j] == '\0') break;
                if (code[i+j] == '0') root = HT[root].lchild;
                if (code[i+j] == '1') root = HT[root].rchild;
                
                j++;
            }
            i += j-1;
            cout << cd[root - 1];
            root = 2 * n-1;
    }
}


int main()
{
    HuffmanTree HT;
    HuffmanCode HC;
    int n=0;
    cin >> n;//输入字符个数
    int w[MAXN];//存放权值
    FILE* fp;
    fopen_s(&fp, "d:\\haffuman_test.txt", "r");
    if (fp == NULL) {
        cout << "can't open the txt";
        exit(1);
    }
    char cd[MAXN];
    for (int i = 0;i < n;i++) {
        fscanf(fp, "%c %d ", &cd[i], &w[i]);//从文件中读取数据
        cout << cd[i] << "  " << w[i] << endl;
    }
    fclose(fp);
    HuffumanCoding(HT, HC, w, n);

    for (int i = 1;i <= n;i++) {//输出编码
        cout <<"字符 "<<cd[i-1]<<" 的哈夫曼编码为 "<< HC[i] << endl;
    }
    cout << "--------------------------------------------------------------------" << endl;
    char test1[MAXN];
    cout << "请输入测试字符串：字符集（A B C D G H F）";
    cin >> test1;
    cout << "哈夫曼编码为：";
    for(char *t=test1;*t!='\0';t++)
    for (int i = 0;i < n;i++) {
        if (*t == cd[i]) cout << HC[i + 1];
    }
    cout << endl;
    cout << "------------------------------------------------------------------------" << endl;
    char test2[MAXN];
    cout << "请输入测试哈夫曼编码：";
    cin >> test2;
    cout << "哈夫曼码对应的字符串为：";
    HuffmanTranslation(HT, test2, n, cd);
    cout << endl;
    cout << "------------------------------------------------------------------------" << endl;
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
