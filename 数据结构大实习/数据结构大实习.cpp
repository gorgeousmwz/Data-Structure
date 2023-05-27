// 数据结构大实习之最短路径.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<iostream>
#include<string>
#define MAXN 10001
#include<sapi.h>
#pragma comment(lib,"sapi.lib")
#include<windows.h>

using namespace std;

LARGE_INTEGER BeginTime;
LARGE_INTEGER EndTime;
LARGE_INTEGER Frequency;


void MSSSpeak(LPCTSTR speakContent) {
    ISpVoice* pVoice = NULL;//初始化COM接口
    if (FAILED(::CoInitialize(NULL))) MessageBox(NULL, (LPCWSTR)L"COM接口初始化失败", (LPCWSTR)L"提示", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);//获取SpVoice接口
    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
    if (SUCCEEDED(hr)) {
        pVoice->SetVolume((USHORT)100);//设置音量在0-100
        pVoice->Speak(speakContent, 0, NULL);
        pVoice->Release();
        pVoice = NULL;
    }
    ::CoUninitialize();

}
std::wstring StringToWString(const std::string& s) {
    std::wstring wszStr;
    int nLength = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, NULL);
    wszStr.resize(nLength);
    LPWSTR lpwszStr = new wchar_t[nLength];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, lpwszStr, nLength);
    wszStr = lpwszStr;
    delete[] lpwszStr;
    return wszStr;
}
void read(string temp) {
    wstring a = StringToWString(temp);
    LPCWSTR str = a.c_str();
    MSSSpeak(str);
}

typedef struct City {//定义顶点City
    string country;//国家
    string city;//城市
    double latitude = 0;//维度
    double longitude = 0;//经度
};

typedef struct Route {//定义弧Route
    string origin_city;//起点城市
    string destination_city;//终点城市
    string transport;//交通方式
    double time = 0;//通行时间
    double cost = 0;//成本
    string other_information;//其他信息
};

typedef struct Graph {//定义有向图
    City* vertex;//顶点数组
    Route** arc;//邻接矩阵
    int vexnum = 0, arcnum = 0;//顶点数，弧数
};

int Locate(string s, City* ct, int num1) {//定位函数，在city[]数组中寻找s城市 返回其在city中的下标 如果没有这个城市 返回-1
    
    for (int i = 0;i < num1;i++) {
       // cout << i << endl;
        if (s == ct[i].city) return i;

    }

    return -1;
}

bool visited[MAXN];//访问标志数组

void DFS(Graph g, int v) {//深度遍历子函数
    visited[v] = true;//访问这个顶点并且标志已经遍历
    cout << g.vertex[v].city << "->";
    //cout << "国家：" << g.vertex[v].country << "  城市：" << g.vertex[v].city << "  维度：" << g.vertex[v].latitude << "  经度：" << g.vertex[v].longitude << endl;
    for (int j = 0;j < g.vexnum;j++) {//遍历每一个顶点的邻接点
        if (g.arc[v][j].cost < MAXN && g.arc[v][j].time < MAXN && !visited[j]) {//如果有i顶点通向j顶点的路径并且没有被访问过 就再次进行深度遍历
            DFS(g, j);
        }
    }
}

void DFSTraverse(Graph g) {//深度优先遍历函数
    for (int i = 0;i < g.vexnum;i++) {//初始化访问标志数组
        visited[i] = false;
    }
    for (int i = 0;i < g.vexnum;i++) {//对于没有访问的顶点 进行深度优先遍历
        
        if (!visited[i]) {
            cout << endl;
            DFS(g, i);
        }
    }
}

void Shortest_Path_DIJ_Time(Graph g,int origin,int destination,int *minPath,double *minAdj) {//迪杰斯特拉算法求时间最短路径
    //minPath数组储存最短路径上v的双亲 minAdj数组储存最短路径的权值
    bool final[MAXN];//标志是否已经求出最短路径
    for (int i = 0;i < g.vexnum;i++) {//初始化
        final[i] = false;//所有点都没有求出最短路径
        minAdj[i]=g.arc[origin][i].time;//最短路径为直线
        minPath[i] = -1;//设空路径
        if (minAdj[i] < MAXN) {
            minPath[i] = origin;//如果起点到其他顶点有路径 则亥为最短路径
        }
    }
    
    minAdj[origin] = 0;
    minPath[origin] = -1;
    final[origin] = true;//起点先进入最短路径
    int v;
    for (int i = 1;i < g.vexnum;i++) {//主循环 对剩下n-1个点操作 求最短路径
        double min = MAXN;
        for (int w = 0;w < g.vexnum;w++) {//求当前到起点最近的点
            if (!final[w] && minAdj[w] < min) {
                v = w;
                min = minAdj[w];
            }
        }
        final[v] = true;//将最近的点加入最短路径
     
        if (v == destination) break;//到达终点就停止
        for (int w = 0;w < g.vexnum;w++) {//更新各点到起点的距离
            if (!final[w] && (min + g.arc[v][w].time) < minAdj[w]) {//如果前面求得的最短路径加上直线距离小于原来的距离，则更新
                minAdj[w] = min + g.arc[v][w].time;
                minPath[w] = v;//储存路径
            }
        }   
    }
}

void Shortest_Path_DIJ_Cost(Graph g, int origin, int destination, int* minPath, double* minAdj) {//迪杰斯特拉算法求花费最短路径
    //minPath数组储存最短路径上v的双亲 minAdj数组储存最短路径的权值
    bool final[MAXN];//标志是否已经求出最短路径
    for (int i = 0;i < g.vexnum;i++) {//初始化
        final[i] = false;//所有点都没有求出最短路径
        minAdj[i] = g.arc[origin][i].cost;//最短路径为直线
        minPath[i] = -1;//设空路径
        if (minAdj[i] < MAXN) {
            minPath[i] = origin;//如果起点到其他顶点有路径 则亥为最短路径
        }
    }

    minAdj[origin] = 0;
    minPath[origin] = -1;
    final[origin] = true;//起点先进入最短路径
    int v;
    for (int i = 1;i < g.vexnum;i++) {//主循环 对剩下n-1个点操作 求最短路径
        double min = MAXN;
        for (int w = 0;w < g.vexnum;w++) {//求当前到起点最近的点
            if (!final[w] && minAdj[w] < min) {
                v = w;
                min = minAdj[w];
            }
        }
        final[v] = true;//将最近的点加入最短路径

        if (v == destination) break;//到达终点就停止
        for (int w = 0;w < g.vexnum;w++) {//更新各点到起点的距离
            if (!final[w] && (min + g.arc[v][w].cost) < minAdj[w]) {//如果前面求得的最短路径加上直线距离小于原来的距离，则更新
                minAdj[w] = min + g.arc[v][w].cost;
                minPath[w] = v;//储存路径
            }
        }
    }
}

int main()
{
    //*******************************************读取csv文件数据*********************************************
    FILE* fp1;
    fp1 = fopen("cities.csv", "r");

    if (!fp1) {//保证文件正常打开
        cout << "无法打开文件" << endl;
        exit(0);
    }
    City ct[MAXN];
    int num1;
    int i = 0;
    char ch;
    num1 = 0;
    while (!feof(fp1)) {//判断文件记录个数
        char em[1000];
        fgets(em,1000,fp1);
        ch = fgetc(fp1);
        num1++;
    }
    rewind(fp1);
    while (!feof(fp1)) {

        if (i == num1) break;//到达指定个数停止读取
        ch = fgetc(fp1);
        for (;ch != ',';ch = fgetc(fp1)) {//读取国家
            ct[i].country += ch;
        }
        ch = fgetc(fp1);
        for (;ch != ',';ch = fgetc(fp1)) {//读取城市
            ct[i].city += ch;
        }
        fscanf(fp1, "%lf,%lf\n", &ct[i].latitude, &ct[i].longitude);//读取经纬度 记得读取换行符
        i++;
    }
    fclose(fp1);//关闭文件
    //for (int j = 0;j < num1;j++) {
    //    cout << "国家："<<ct[j].country <<"  城市："<< ct[j].city <<"  维度："<< ct[j].latitude <<"  经度："<< ct[j].longitude << endl;
    //}//测试代码

    int num2 = 0;
    Route route[MAXN];
    FILE* fp2;
    fp2 = fopen("routes.csv", "r");
    if (!fp2) {
        cout << "无法打开文件" << endl;
        exit(0);
    }
    i = 0;
    while (!feof(fp2)) {//判断文件记录个数
        char em[1000];
        fgets(em, 1000, fp2);
        ch = fgetc(fp2);
        num2++;
    }
    rewind(fp2);
    while (!feof(fp2)) {
        if (i == num2) break;
        ch = fgetc(fp2);
        for (;ch != ',';ch = fgetc(fp2)) {//读取起点
            route[i].origin_city += ch;
        }
        ch = fgetc(fp2);
        for (;ch != ',';ch = fgetc(fp2)) {//读取终点
            route[i].destination_city += ch;
        }
        ch = fgetc(fp2);
        for (;ch != ',';ch = fgetc(fp2)) {//读取交通方式
            route[i].transport += ch;
        }
        fscanf(fp2, "%lf,%lf,", &route[i].time, &route[i].cost);//读取时间和成本
        ch = fgetc(fp2);
        while (ch != '\n') {//读取其他信息
            
            route[i].other_information += ch;
            ch = fgetc(fp2);
        }
        i++;
    }
    fclose(fp2);
    //for (int j = 0;j < num2;j++) {
    //    cout << "起点：" << route[j].origin_city << "  终点：" << route[j].destination_city <<"  交通工具："<<route[j].transport<< "  时间："<<route[j ].time<< "  成本：" << route[j].cost << "  其他信息：" << route[j].other_information << endl;
    //}//测试代码
    /***********************************************创建图的邻接表***********************************************************/
    Graph g;//构建一个有向图g
    //初始化
    g.vexnum = num1;//存入顶点数
    g.arc = new Route * [num1];
    for (int i = 0;i < num1;i++) g.arc[i] = new Route[num1];//动态分配一个矩阵
    g.arcnum = num2;//存入边数
    g.vertex = ct;//导入顶点数据
    for (int i = 0;i < num1;i++) {
        for (int j = 0;j < num1;j++) {
            if (i == j) {//对于对角线上的表示从本地出发到本地 时间和成本都为0
                g.arc[i][j].time = 0;
                g.arc[i][j].cost = 0;
            }
            else {//否则 全部设为最大MAXN
                g.arc[i][j].time = MAXN;
                g.arc[i][j].cost = MAXN;
            }
            
        }
        //cout << endl;
    }
loop:
    read("您希望的判断标准是什么  是时间还是花费");
    cout << "您所希望的路径最短评判标准是什么？ 1.时间  2.花费 3.退出" << endl;
    cout << "请做出选择：";
    int pan = 0;
    cin >> pan;
    if (pan != 1 && pan != 2&&pan!=3) { cout << "请输入正确的选择" << endl;goto loop; }
    //时间最短
    if (pan == 1) {
        for (int i = 0;i < g.arcnum;i++) {
            int origin = -1, destination = -1;//保存起点和终点
            origin = Locate(route[i].origin_city, ct, num1);//定位坐标
            destination = Locate(route[i].destination_city, ct, num1);//假定 矩阵的行坐标为origin 列坐标为destinaton
            if (g.arc[origin][destination].time > route[i].time)//时间最优
                g.arc[origin][destination].time = route[i].time;//将时间赋值到里面
            g.arc[origin][destination].cost = route[i].cost;
        }
        /***********************************************************深度优先遍历***************************************************/
        read("下面进行深度优先遍历我们将其依次输出");
         cout << "下面进行深度优先遍历 我们将其依次输出：" << endl;
         DFSTraverse(g);
         cout << endl;
         system("pause");
         /************************************************************最短路径******************************************************/
        int origin = 0, destination = 0;
        string origin_, destination_;//输入起点终点
        read("请输入起点");
        cout << "请输入起点：";
        cin.get();
        getline(cin, origin_);
        read("请输入终点");
        cout << "请输入终点：";
        getline(cin, destination_);
        QueryPerformanceFrequency(&Frequency);
        QueryPerformanceCounter(&BeginTime);//开始计时
        for (int i = 0;i < g.vexnum;i++) {//确定起点和终点的位置
            if (g.vertex[i].city == origin_) origin = i;
            if (g.vertex[i].city == destination_) destination = i;
        }
        double minAdj[MAXN];
        int minPath[MAXN];
        Shortest_Path_DIJ_Time(g, origin, destination, minPath, minAdj);//求时间最短路径
        QueryPerformanceCounter(&EndTime);
        read("您的最短时间花费和具体路线如下");
        cout << "您最短的时间花费为：" << minAdj[destination] << "h" << endl;
        cout << "具体路线为: ";
        int path[MAXN];
        int* p = path;//存储最短路径
        int k = destination;
        *p = k;
        while (1) {//沿着前驱往上走 知道找到起点为止
            if (k == origin) break;
            p++;
            *p = minPath[k];
            k = minPath[k];
        }
        int zheng[MAXN];//正向存储的路径
        int ji = 0;
        for (;p >= path;p--) { //输出路径
            zheng[ji++] = *p;
            cout << g.vertex[*p].city;
            if (p != path) cout << "->";
        }
        cout << endl;
        cout << "算法运行时间：" << double(EndTime.QuadPart-BeginTime.QuadPart)/Frequency.QuadPart << "s" << endl;
        system("pause");
        //*****************************************************最短路径可视化***********************************
    //创建一个html文件，将百度API可视化所需要的代码和信息写到里面
        FILE* fp3;
        fp3 = fopen("graph.htm", "w");
        if (!fp3) {
            cout << "无法打开文件" << endl;
            exit(1);
        }
        //HTML文件头部说明
        char a[1024], b[1024];//储存起点和终点
        strcpy(a, origin_.data());
        strcpy(b, destination_.data());

        std::string s = "<!DOCTYPE html><html><head>\
<style type='text/css'>body, html{width: 100%;height: 100%;margin:0;font-family:'微软雅黑';}#allmap{height:100%;width:100%;}#r-result{width:100%;}</style>\
<script type='text/javascript' src='http://api.map.baidu.com/api?v=2.0&ak=nSxiPohfziUaCuONe4ViUP2N'></script>";

        fprintf(fp3, "%s<title>Shortest path from %s to %s</title></head><body>\
<div id='allmap'></div></div></body></html><script type='text/javascript'>\
var map = new BMap.Map('allmap');\
var point = new BMap.Point(0, 0);\
map.centerAndZoom(point, 2);\
map.enableScrollWheelZoom(true);", s.c_str(), a, b);

        i = 0;
        int j = 0;
        int bycity = origin, bycity2;

        while (bycity != destination) {
            //路径中一条弧弧头及其信息
            fprintf(fp3, "var marker%d = new BMap.Marker(new BMap.Point(%.4f, %.4f));\
map.addOverlay(marker%d);\n\
var infoWindow%d = new BMap.InfoWindow(\"<p style = 'font-size:14px;'>country: %s<br/>city: %s</p>\");\
marker%d.addEventListener(\"click\", function(){\
this.openInfoWindow(infoWindow%d);}); ", j, g.vertex[bycity].longitude, g.vertex[bycity].latitude, j, j, g.vertex[bycity].country.c_str(), g.vertex[bycity].city.c_str(), j, j);
            j++;
            bycity2 = bycity;
            bycity = zheng[i++];
            //路径中一条弧弧尾及其信息
            fprintf(fp3, "var marker%d = new BMap.Marker(new BMap.Point(%.4f, %.4f));\
map.addOverlay(marker%d);\n\
var infoWindow%d = new BMap.InfoWindow(\"<p style = 'font-size:14px;'>country: %s<br/>city: %s</p>\");\
marker%d.addEventListener(\"click\", function(){\
this.openInfoWindow(infoWindow%d);}); ", j, g.vertex[bycity].longitude, g.vertex[bycity].latitude, j, j, g.vertex[bycity].country.c_str(), g.vertex[bycity].city.c_str(), j, j);

            //路径中一条弧的信息
            fprintf(fp3, "var contentString0%d = '%s, %s --> %s, %s (%s - %.0f hours - $%.0f - %s)';\
var path%d = new BMap.Polyline([new BMap.Point(%.4f, %.4f),new BMap.Point(%.4f, %.4f)], {strokeColor:'#18a45b', strokeWeight:8, strokeOpacity:0.8});\
map.addOverlay(path%d);\
path%d.addEventListener(\"click\", function(){\
alert(contentString0%d);});", j, g.vertex[bycity2].city.c_str(), g.vertex[bycity2].country.c_str(), g.vertex[bycity].city.c_str(), g.vertex[bycity].country.c_str(), g.arc[bycity2][bycity].transport.c_str(), g.arc[bycity2][bycity].time, g.arc[bycity2][bycity].cost, g.arc[bycity2][bycity].other_information.c_str(), j, g.vertex[bycity2].longitude, g.vertex[bycity2].latitude, g.vertex[bycity].longitude, g.vertex[bycity].latitude, j, j, j);
        }
        fprintf(fp3, "</script>\n");//html文件结束
        ShellExecute(0, L"open", L"graph.htm", 0, 0, 1);//自动打开hml文件
        fclose(fp3);
        goto loop;
    }

    //花费最少
    if (pan == 2) {
        for (int i = 0;i < g.arcnum;i++) {
            int origin = -1, destination = -1;//保存起点和终点
            origin = Locate(route[i].origin_city, ct, num1);//定位坐标
            destination = Locate(route[i].destination_city, ct, num1);//假定 矩阵的行坐标为origin 列坐标为destinaton
            if (g.arc[origin][destination].cost > route[i].cost)//花费最优
                g.arc[origin][destination].cost = route[i].cost;//将花费赋值到里面
            g.arc[origin][destination].time = route[i].time;
        }
        /***********************************************************深度优先遍历***************************************************/
        read("下面进行深度优先遍历 我们将其依次输出");
        cout << "下面进行深度优先遍历 我们将其依次输出：" << endl;
        DFSTraverse(g);
        cout << endl;
        system("pause");
        /************************************************************最短路径******************************************************/
        int origin = 0, destination = 0;
        string origin_, destination_;//输入起点终点
        read("请输入起点");
        cout << "请输入起点：";
        cin.get();
        getline(cin, origin_);
        read("请输入终点");
        cout << "请输入终点：";
        getline(cin, destination_);
        QueryPerformanceFrequency(&Frequency);
        QueryPerformanceCounter(&BeginTime);//开始计时
        for (int i = 0;i < g.vexnum;i++) {//确定起点和终点的位置
            if (g.vertex[i].city == origin_) origin = i;
            if (g.vertex[i].city == destination_) destination = i;
        }
        double minAdj[MAXN];
        int minPath[MAXN];
        Shortest_Path_DIJ_Cost(g, origin, destination, minPath, minAdj);//求花费最短路径
        QueryPerformanceCounter(&EndTime);
        read("您的最少花费和具体路线如下");
        cout << "您最少的花费为：" << minAdj[destination] << "yuan" << endl;
        cout << "具体路线为: ";
        int path[MAXN];
        int* p = path;//存储最短路径
        int k = destination;
        *p = k;
        while (1) {//沿着前驱往上走 知道找到起点为止
            if (k == origin) break;
            p++;
            *p = minPath[k];
            k = minPath[k];
        }
        int zheng[MAXN];//正向存储的路径
        int ji = 0;
        for (;p >= path;p--) { //输出路径
            zheng[ji++] = *p;
            cout << g.vertex[*p].city;
            if (p != path) cout << "->";
        }
        cout << endl;
        cout << "算法运行时间：" << double(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart << "s" << endl;
        system("pause");
        //*****************************************************最短路径可视化***********************************
    //创建一个html文件，将百度API可视化所需要的代码和信息写到里面
        FILE* fp3;
        fp3 = fopen("graph.htm", "w");
        if (!fp3) {
            cout << "无法打开文件" << endl;
            exit(1);
        }
        //HTML文件头部说明
        char a[1024], b[1024];//储存起点和终点
        strcpy(a, origin_.data());
        strcpy(b, destination_.data());

        std::string s = "<!DOCTYPE html><html><head>\
<style type='text/css'>body, html{width: 100%;height: 100%;margin:0;font-family:'微软雅黑';}#allmap{height:100%;width:100%;}#r-result{width:100%;}</style>\
<script type='text/javascript' src='http://api.map.baidu.com/api?v=2.0&ak=nSxiPohfziUaCuONe4ViUP2N'></script>";

        fprintf(fp3, "%s<title>Shortest path from %s to %s</title></head><body>\
<div id='allmap'></div></div></body></html><script type='text/javascript'>\
var map = new BMap.Map('allmap');\
var point = new BMap.Point(0, 0);\
map.centerAndZoom(point, 2);\
map.enableScrollWheelZoom(true);", s.c_str(), a, b);

        i = 0;
        int j = 0;
        int bycity = origin, bycity2;
        
        while (bycity != destination) {
            //路径中一条弧弧头及其信息
            fprintf(fp3, "var marker%d = new BMap.Marker(new BMap.Point(%.4f, %.4f));\
map.addOverlay(marker%d);\n\
var infoWindow%d = new BMap.InfoWindow(\"<p style = 'font-size:14px;'>country: %s<br/>city: %s</p>\");\
marker%d.addEventListener(\"click\", function(){\
this.openInfoWindow(infoWindow%d);}); ", j, g.vertex[bycity].longitude, g.vertex[bycity].latitude, j, j, g.vertex[bycity].country.c_str(), g.vertex[bycity].city.c_str(), j, j);
            j++;
            bycity2 = bycity;
            bycity = zheng[i++];
            //路径中一条弧弧尾及其信息
            fprintf(fp3, "var marker%d = new BMap.Marker(new BMap.Point(%.4f, %.4f));\
map.addOverlay(marker%d);\n\
var infoWindow%d = new BMap.InfoWindow(\"<p style = 'font-size:14px;'>country: %s<br/>city: %s</p>\");\
marker%d.addEventListener(\"click\", function(){\
this.openInfoWindow(infoWindow%d);}); ", j, g.vertex[bycity].longitude, g.vertex[bycity].latitude, j, j, g.vertex[bycity].country.c_str(), g.vertex[bycity].city.c_str(), j, j);

            //路径中一条弧的信息
            fprintf(fp3, "var contentString0%d = '%s, %s --> %s, %s (%s - %.0f hours - $%.0f - %s)';\
var path%d = new BMap.Polyline([new BMap.Point(%.4f, %.4f),new BMap.Point(%.4f, %.4f)], {strokeColor:'#18a45b', strokeWeight:8, strokeOpacity:0.8});\
map.addOverlay(path%d);\
path%d.addEventListener(\"click\", function(){\
alert(contentString0%d);});", j, g.vertex[bycity2].city.c_str(), g.vertex[bycity2].country.c_str(), g.vertex[bycity].city.c_str(), g.vertex[bycity].country.c_str(), g.arc[bycity2][bycity].transport.c_str(), g.arc[bycity2][bycity].time, g.arc[bycity2][bycity].cost, g.arc[bycity2][bycity].other_information.c_str(), j, g.vertex[bycity2].longitude, g.vertex[bycity2].latitude, g.vertex[bycity].longitude, g.vertex[bycity].latitude, j, j, j);

        }
        fprintf(fp3, "</script>\n");//html文件结束
        ShellExecute(0,L"open",L"graph.htm", 0,0,1);//自动打开hml文件
        fclose(fp3);
        goto loop;
    }
    for (int i = 0;i < num1;i++) {//释放空间
         delete[]g.arc[i];
    }
    delete[]g.arc;
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
