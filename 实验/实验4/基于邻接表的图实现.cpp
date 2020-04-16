#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

/*--------- 函数结果状态代码 ---------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2
#define overflow -3

/*----- 数据元素类型定义 -----*/
typedef int status;   //返回值类型
typedef int ElemType; //数据域类型
typedef char TagType; //关键字类型

/*----- 图的存储表示 -----*/
typedef struct Arc
{                   //弧的定义
    TagType adjvex; //邻接点域
    Arc *nextarc;   //链域，指向下一条依附于该顶点的弧
    int info;       //标志域
} Arc, *pArc;
typedef struct Vex
{                  //顶点的定义
    TagType tag;   //关键字
    ElemType data; //数据域
    Arc *firstArc; //链域，指向第一条依附于该顶点的弧
} Vex, *pVex;
typedef struct Graph
{               //图的定义
    Vex V[100]; //顶点结构数组
    int Vexnum; //顶点个数
    int Arcnum; //弧个数
} Digraph;

/*----- 栈的顺序存储表示 -----*/
#define STACK_INIT_SIZE 100 //存储空间初始分配量
#define STACKINCREMENT 10   //分配增量

typedef pVex SElemType;
typedef struct
{                    //栈的定义
    SElemType *base; //栈底指针
    SElemType *top;  //栈顶指针
    int stacksize;   //已分配存储空间
} SqStack;

/*------ 函数声明 ------*/
status CreateGraph(Digraph &G);                              //创建图
status DestroyGraph(Digraph &G);                             //销毁图
status LocateVex(Digraph G, TagType e);                      //查找顶点
status PutVex(Digraph &G, TagType e, ElemType value);        //顶点赋值
status FirstAdjVex(Digraph G, TagType v, pVex &u);           //获得第一邻接点
status NextAdjVex(Digraph G, TagType v, TagType w, pVex &u); //获得下一邻接点
status InsertVex(Digraph &G, pVex &u);                       //插入顶点
status DeleteVex(Digraph &G, TagType v);                     //删除顶点
status InsertArc(Digraph &G, TagType v, TagType w);          //插入弧
status DeleteArc(Digraph &G, TagType v, TagType w);          //删除弧
status DFSTraverse(Digraph G, TagType v);                    //深度优先搜索遍历
status BFSTraverse(Digraph G, TagType v, int visit[]);       //广度优先搜索遍历
status SaveGraph(Digraph G, char filename[]);                //保存图
status GetGraph(Digraph &G, char filename[]);                //加载图
status ChangeGraph(int &t);                                  //改变操作的图
status PrintGraph(Digraph G);                                //输出图及邻接表

/*------ 辅助函数声明 ------*/
status DFS(Digraph G, TagType v, int *p, int i); //深度优先搜索遍历辅助递归函数
status InitStack(SqStack &S);                    //创建空栈
status Push(SqStack &S, SElemType p);            //插入栈顶元素

/*----main函数----*/
int main(void)
{
    Digraph G[100];     //结构数组
    char filename[100]; //存储文件名字符串
    ElemType value;     //存储数据域值
    TagType e, v, w;    //存储关键字值
    status j;           //存储函数返回值
    pVex u;
    Vex c;
    pArc p;
    int i, t = 0;
    for (i = 0; i < 100; i++)
    { //初始化
        G[i].Arcnum = 0;
        G[i].Vexnum = 0;
    }
    do
    {
        printf(" 请输入要对第几个图进行操作 [1~100]\n");
        scanf("%d", &t);
        getchar();
    } while (t < 1 || t > 100);
    system("cls"); //清屏
    int op = 1;
    while (op)
    {
        system("cls");
        printf("\n\n");
        printf("      正在对第%d个图进行操作    \n", t);
        printf("      Menu for Digraph On Sequence Structure \n");
        printf("---------------------------------------------------------\n");
        printf("    	  1. CreateGraph       9. InsertArc      \n");
        printf("    	  2. DestroyGraph     10. DeleteArc      \n");
        printf("    	  3. LocateVex        11. DFSTraverse    \n");
        printf("    	  4. PutVex           12. BFSTraverse    \n");
        printf("    	  5. FirstAdjVex      13. SaveGraph      \n");
        printf("    	  6. NextAdjVex       14. GetGraph       \n");
        printf("    	  7. InsertVex        15. ChangeGraph    \n");
        printf("    	  8. DeleteVex        16. PrintGraph     \n");
        printf("    	                       0. Exit           \n");
        printf("---------------------------------------------------------\n");
        printf("      请选择你的操作[0~16]:");
        scanf("%d", &op);
        getchar();
        switch (op)
        {
        case 1:
            //CreateGraph
            if (G[t - 1].Vexnum != 0)
                printf("图已存在，创建失败！\n");
            else
            {
                Digraph g = G[t - 1];
                j = CreateGraph(g);
                if (j == OK)
                {
                    G[t - 1] = g;
                    printf("图创建成功！\n");
                }
                else if (j == ERROR)
                    printf("顶点关键字重复，图创建失败！\n");
                else if (j == INFEASTABLE)
                    printf("弧连接的某顶点不在图中，图创建失败！\n");
                else if (j == OVERFLOW)
                    printf("关系集存在自环，图创建失败！\n");
                else if (j == overflow)
                    printf("关系集重复，图创建失败！\n");
            }
            getchar();
            getchar();
            break;
        case 2:
            //DestroyGraph
            if (G[t - 1].Vexnum == 0)
                printf("图不存在，销毁失败！\n");
            else
            {
                j = DestroyGraph(G[t - 1]);
                if (j == OK)
                    printf("图销毁成功！\n");
                else
                    printf("图销毁失败！\n");
            }
            getchar();
            getchar();
            break;
        case 3:
            //LocateVex
            if (G[t - 1].Vexnum == 0)
                printf("图不存在！\n");
            else
            {
                printf("请输入想要查找顶点的关键字：\n");
                scanf("%c", &e);
                getchar();
                j = LocateVex(G[t - 1], e);
                if (j)
                    printf("所查找的顶点为图中第%d个顶点，即%c(%d)！\n", j, G[t - 1].V[j - 1].tag, G[t - 1].V[j - 1].data);
                else
                    printf("所查找的顶点不在图中！\n");
            }
            getchar();
            getchar();
            break;
        case 4:
            //PutVex
            if (G[t - 1].Vexnum == 0)
                printf("图不存在！\n");
            else
            {
                printf("请输入想要赋值顶点的关键字：\n");
                scanf("%c", &e);
                getchar();
                printf("请输入想要赋的值：\n");
                scanf("%d", &value);
                getchar();
                j = PutVex(G[t - 1], e, value);
                if (j)
                    printf("顶点%c赋值成功！\n", e);
                else
                    printf("所赋值的顶点不在图中！\n");
            }
            getchar();
            getchar();
            break;
        case 5:
            //FirstAdjVex
            if (G[t - 1].Vexnum == 0)
                printf("图不存在！\n");
            else
            {
                printf("请输入想要查找第一邻接点的顶点的关键字：\n");
                scanf("%c", &v);
                getchar();
                j = FirstAdjVex(G[t - 1], v, u);
                if (j)
                {
                    if (u)
                        printf("所查找顶点%c的第一邻接点为%c！\n", v, u->tag);
                    else
                        printf("所查找顶点无邻接点！\n");
                }
                else
                    printf("所查找的顶点不在图中！\n");
            }
            getchar();
            getchar();
            break;
        case 6:
            //NextAdjVex
            if (G[t - 1].Vexnum == 0)
                printf("图不存在！\n");
            else
            {
                printf("请输入想要查找邻接点的顶点的关键字：\n");
                scanf("%c", &v);
                getchar();
                printf("请输入想要查找下一邻接点的邻接点的关键字：\n");
                scanf("%c", &w);
                getchar();
                j = NextAdjVex(G[t - 1], v, w, u);
                if (j == OK)
                {
                    if (u)
                        printf("顶点%c相对于顶点%c的下一邻接点为%c！\n", v, w, u->tag);
                    else
                        printf("顶点%c的最后一个邻接点是%c！\n", v, w);
                }
                else if (j == ERROR)
                    printf("顶点%c不是顶点%c的邻接点！\n", w, v);
                else if (j == INFEASTABLE)
                    printf("顶点%c不在图中！\n", w);
                else
                    printf("顶点%c不在图中！\n", v);
            }
            getchar();
            getchar();
            break;
        case 7:
            //InsertVex
            if (G[t - 1].Vexnum == 0)
                printf("图不存在！\n");
            else
            {
                printf("请输入插入顶点的关键字及数据域：\n");
                scanf("%c", &c.tag);
                getchar();
                scanf("%d", &c.data);
                getchar();
                c.firstArc = NULL;
                u = &c;
                j = InsertVex(G[t - 1], u);
                if (j == OK)
                    printf("顶点%c插入成功！\n", c.tag);
                else if (j == ERROR)
                    printf("邻接点错误，图中不存在该顶点！\n");
                else
                    printf("顶点错误，关键字在图中已存在！\n");
            }
            getchar();
            getchar();
            break;
        case 8:
            //DeleteVex
            if (G[t - 1].Vexnum == 0)
                printf("图不存在！\n");
            else
            {
                printf("请输入删除顶点的关键字：\n");
                scanf("%c", &v);
                getchar();
                j = DeleteVex(G[t - 1], v);
                if (j == OK)
                    printf("顶点%c删除成功！\n", v);
                else if (j == ERROR)
                    printf("顶点删除失败，图中不存在该顶点！\n");
            }
            getchar();
            getchar();
            break;
        case 9:
            //InsertArc
            if (G[t - 1].Vexnum == 0)
                printf("图不存在！\n");
            else
            {
                printf("请输入插入弧：\n");
                scanf("%c", &v);
                getchar();
                scanf("%c", &w);
                getchar();
                j = InsertArc(G[t - 1], v, w);
                if (j == OK)
                    printf("弧插入成功！\n", v);
                else if (j == ERROR)
                    printf("弧插入失败，图中已存在该弧！\n");
                else
                    printf("弧插入失败，图中不存在弧的某顶点！\n");
            }
            getchar();
            getchar();
            break;
        case 10:
            //DeleteArc
            if (G[t - 1].Vexnum == 0)
                printf("图不存在！\n");
            else
            {
                printf("请输入删除弧：\n");
                scanf("%c", &v);
                getchar();
                scanf("%c", &w);
                getchar();
                j = DeleteArc(G[t - 1], v, w);
                if (j == OK)
                    printf("弧删除成功！\n", v);
                else if (j == ERROR)
                    printf("弧删除失败，图中不存在该弧！\n");
                else
                    printf("弧删除失败，图中不存在弧的某顶点！\n");
            }
            getchar();
            getchar();
            break;
        case 11:
            //DFSTraverse
            if (G[t - 1].Vexnum == 0)
                printf("图不存在！\n");
            else
            {
                printf("请输入深度搜索遍历的开始顶点关键字：\n");
                scanf("%c", &v);
                getchar();
                if (DFSTraverse(G[t - 1], v) == ERROR)
                    printf("图中不存在该顶点！\n");
            }
            getchar();
            getchar();
            break;
        case 12:
            //BFSTraverse
            if (G[t - 1].Vexnum == 0)
                printf("图不存在！\n");
            else
            {
                int visited[G[t-1].Vexnum]; //查找标志数组
                for (i = 0; i < G[t - 1].Vexnum; i++)
                    visited[i] = 0;
                printf("请输入广度搜索遍历的开始顶点关键字：\n");
                scanf("%c", &v);
                getchar();
                if (BFSTraverse(G[t - 1], v, visited) == ERROR)
                    printf("图中不存在该顶点！\n");
            }
            getchar();
            getchar();
            break;
        case 13:
            //SaveGraph
            printf("请输入想要保存的文件名：\n");
            scanf("%s", filename);
            j = SaveGraph(G[t - 1], filename);
            if (j == OK)
                printf("文件保存成功！\n");
            else if (j == OVERFLOW)
                printf("图不存在，文件保存失败！\n");
            else
                printf("文件保存失败！\n");
            getchar();
            getchar();
            break;
        case 14:
            //GetGraph
            printf("请输入想要加载的文件名：\n");
            scanf("%s", filename);
            j = GetGraph(G[t - 1], filename);
            if (j == OK)
                printf("文件加载成功！\n");
            else
                printf("文件加载失败！\n");
            getchar();
            getchar();
            break;
        case 15:
            //ChangeGraph
            i = t;
            j = ChangeGraph(i);
            if (j == OK)
            {
                t = i;
                printf("图已成功改变至第%d个！\n", t);
            }
            else
                printf("图改变失败！\n");
            getchar();
            getchar();
            break;
        case 16:
            //PrintGraph
            if (G[t - 1].Vexnum == 0)
                printf("图不存在！\n");
            else
                PrintGraph(G[t - 1]);
            getchar();
            getchar();
            break;
        case 0:
            //退出
            break;
        default:
            //输入错误
            printf("输入错误，请重新输入！\n");
            getchar();
            getchar();
            break;
        } //end of switch
    }     //end of while
    printf("欢迎下次再使用本系统！\n");
} //end of main()

/*---------函数定义---------*/
//1.创建图
status CreateGraph(Digraph &G)
{
    Arc *p, *q;
    int i, j, l;
    TagType ch1, ch2;
    //创建图顶点集的过程
    printf("请输入所要创建图的顶点数(1-100)：\n");
    scanf("%d", &G.Vexnum);
    getchar();
    printf("请输入顶点集：\n");
    for (i = 0; i < G.Vexnum; i++)
    {                           //读入顶点信息
        G.V[i].firstArc = NULL; //指针域初始化
        scanf("%c", &G.V[i].tag);
        getchar();
        scanf("%d", &G.V[i].data);
        getchar();
    }
    //检查图顶点集是否符合要求
    for (i = 0; i < G.Vexnum; i++)
    {
        ch1 = G.V[i].tag;
        for (j = i + 1; j < G.Vexnum; j++)
        {
            ch2 = G.V[j].tag;
            if (ch1 == ch2)
                return ERROR; //顶点关键字重复
        }
    }
    //创建图关系集的过程
    printf("请输入所要创建图的弧数：\n");
    scanf("%d", &G.Arcnum);
    getchar();
    printf("请输入关系集：\n");
    for (i = 0; i < G.Arcnum; i++)
    { //读入弧信息
        scanf("%c", &ch1);
        getchar();
        scanf("%c", &ch2);
        getchar();
        for (j = 0; j < G.Vexnum; j++) //查找关键字为ch1的顶点
            if (G.V[j].tag == ch1)
                break;
        if (j == G.Vexnum)
            return INFEASTABLE;        //顶点ch1不在图中
        for (l = 0; l < G.Vexnum; l++) //查找关键字为ch1的顶点
            if (G.V[l].tag == ch2)
                break;
        if (l == G.Vexnum)
            return INFEASTABLE;        //顶点ch2不在图中
        p = (pArc)malloc(sizeof(Arc)); //创建新结点
        p->adjvex = ch2;
        p->nextarc = NULL;
        if (G.V[j].firstArc == NULL)
            G.V[j].firstArc = p;
        else
        {
            for (q = G.V[j].firstArc; q->nextarc != NULL; q = q->nextarc)
                ;
            q->nextarc = p; //新结点连接链表尾端
        }
    }
    //检查关系集是否符合要求
    for (i = 0; i < G.Vexnum; i++)
    {
        p = G.V[i].firstArc;
        if (p == NULL)
            continue;
        if (p->adjvex == G.V[i].tag)
            return OVERFLOW; //有自环
        if (p->nextarc == NULL)
            continue;
        q = p;
        p = p->nextarc;
        while (p != NULL)
        {
            if (q->adjvex == G.V[i].tag)
                return OVERFLOW; //有自环
            for (; p->nextarc != NULL; p = p->nextarc)
            {
                if (q->adjvex == p->adjvex)
                    return overflow; //弧重复
            }
            if (q->adjvex == p->adjvex)
                return overflow; //弧重复
            q = q->nextarc;
            p = q->nextarc;
        }
        if (q->adjvex == G.V[i].tag)
            return OVERFLOW; //有自环
    }
    return OK;
}
//2.销毁图
status DestroyGraph(Digraph &G)
{
    Arc *p, *q;
    int i;
    for (i = 0; i < G.Vexnum; i++)
    {
        G.V[i].data = G.V[i].tag = 0; //重置关键字及数据域
        if (G.V[i].firstArc != NULL)
        {
            p = G.V[i].firstArc;
            while (p->nextarc != NULL)
            { //销毁链表
                q = p;
                p = p->nextarc;
                free(q);
            }
            free(p);
        }
    }
    G.Arcnum = G.Vexnum = 0; //重置图顶点数和弧数
    return OK;
}
//3.查找顶点
status LocateVex(Digraph G, TagType e)
{
    int i;
    for (i = 0; i < G.Vexnum; i++)
    {
        if (G.V[i].tag == e)
            return i + 1; //返回所查找顶点是第几个
    }
    return 0;
}
//4.顶点赋值
status PutVex(Digraph &G, TagType e, ElemType value)
{
    status i = LocateVex(G, e); //查找顶点e
    if (i)
    {
        G.V[i - 1].data = value; //数据域赋值
        return OK;
    }
    else
        return ERROR; //顶点e不在图中
}
//5.获得第一邻接点
status FirstAdjVex(Digraph G, TagType v, pVex &u)
{
    Arc *p;
    status i, j;
    i = LocateVex(G, v); //查找顶点v
    if (i)
    {
        p = G.V[i - 1].firstArc;
        if (!p)
            u = NULL; //无邻接点
        else
        {
            j = LocateVex(G, p->adjvex);
            u = &G.V[j - 1]; //返回邻接点指针
        }
        return OK;
    }
    else
        return ERROR; //顶点v不在图中
}
//6.获得下一邻接点
status NextAdjVex(Digraph G, TagType v, TagType w, pVex &u)
{
    pArc p, q;
    status i, k, j = ERROR;
    i = LocateVex(G, v);
    if (!i)
        return OVERFLOW; //顶点v不在图中
    k = LocateVex(G, w);
    if (!k)
        return INFEASTABLE; //顶点w不在图中
    p = G.V[i - 1].firstArc;
    if (!p)
        return ERROR; //无邻接点
    else
    {
        while (p->nextarc != NULL)
        {
            if (p->adjvex == w)
            { //找到w在链表中的位置
                q = p->nextarc;
                j = OK;
                break;
            }
            q = p;
            p = p->nextarc;
        }
        if (p->adjvex == w)
        {
            q = p->nextarc;
            j = OK;
        }
        if (j)
        { //找到w
            if (!q)
                u = NULL; //w在链表尾端
            else
            {
                j = LocateVex(G, q->adjvex);
                u = &G.V[j - 1]; //返回下一邻接点的指针
            }
            return OK;
        }
        else
            return ERROR; //w不是v的邻接点
    }
}
//7.插入顶点
status InsertVex(Digraph &G, pVex &u)
{
    pArc p, q;
    TagType ch;
    int i, j, num;
    j = LocateVex(G, u->tag);
    if (j)
        return OVERFLOW; //顶点关键字重复
    printf("请输入插入结点的邻接点个数：\n");
    scanf("%d", &num);
    getchar();
    for (i = 0; i < num; i++)
    {
        printf("请依次输入插入结点的邻接点关键字：\n");
        scanf("%c", &ch);
        getchar();
        j = LocateVex(G, ch);
        if (j == 0)
            return ERROR;              //邻接点不在图中
        p = (pArc)malloc(sizeof(Arc)); //创建新结点
        p->adjvex = ch;
        p->nextarc = NULL;
        if (u->firstArc == NULL)
            u->firstArc = p; //新结点作为第一个结点
        else
        {
            for (q = u->firstArc; q->nextarc != NULL; q = q->nextarc)
                ;
            q->nextarc = p; //非第一个结点
        }
    }
    G.V[G.Vexnum] = *u; //将顶点插入顶点集中
    G.Vexnum++;         //更新图顶点数
    G.Arcnum += num;    //更新图弧数
    return OK;
}
//8.删除顶点
status DeleteVex(Digraph &G, TagType v)
{
    pArc p, q;
    status ret;
    int i, j, num = 0;
    j = LocateVex(G, v);
    if (j == 0)
        return ERROR; //顶点v不在图中
    p = G.V[j - 1].firstArc;
    if (p)
    { //删除以顶点v为弧尾的弧
        while (p->nextarc != NULL)
        {
            q = p;
            p = p->nextarc;
            num++; //对删除的弧计数
            free(q);
        }
        num++; //对删除的弧计数
        free(p);
        G.V[j - 1].firstArc = NULL; //初始化
    }
    G.V[j - 1].data = G.V[-1].tag = 0; //初始化
    for (i = 0; i < G.Vexnum; i++)
    { //删除以顶点v为弧头的弧
        q = p = G.V[i].firstArc;
        if (!p)
            continue;                              //无邻接点，跳过本次循环
        for (; p->nextarc != NULL; p = p->nextarc) //遍历链表寻找邻接点v
            if (p->adjvex == v)
                break;
        if (p->nextarc == NULL && p->adjvex != v)
            continue;
        //没有找到，跳过本次循环
        if (p == G.V[i].firstArc)
        { //若为第一个结点，直接删除
            G.V[i].firstArc = p->nextarc;
            free(p);
            num++;    //对删除的弧计数
            continue; //跳过本次循环
        }
        else
        {
            for (; q->nextarc != NULL; q = q->nextarc)
                //遍历链表寻找顶点v前一个结点
                if (q->nextarc == p)
                    break;
            q->nextarc = p->nextarc;
            free(p);
            num++;    //对删除的弧计数
            continue; //跳过本次循环
        }
    }
    for (; j < G.Vexnum; j++)
        G.V[j - 1] = G.V[j]; //顶点集中后续顶点依次前移
    G.Vexnum--;              //更新顶点数
    G.Arcnum -= num;         //更新弧数
    return OK;
}
//9.插入弧
status InsertArc(Digraph &G, TagType v, TagType w)
{
    pArc p, q;
    int i, j;
    i = LocateVex(G, v);
    if (i == 0)
        return OVERFLOW; //不存在顶点v
    j = LocateVex(G, w);
    if (j == 0)
        return OVERFLOW; //不存在顶点w
    p = G.V[i - 1].firstArc;
    if (!p)
    {                                  //无邻接点
        q = (pArc)malloc(sizeof(Arc)); //插入弧v,w
        q->adjvex = w;
        q->nextarc = NULL;
        G.V[i - 1].firstArc = q;
        G.Arcnum++; //更新弧数
        return OK;
    }
    else
    {
        for (; p->nextarc != NULL; p = p->nextarc) //查找弧v,w
            if (p->adjvex == w)
                break;
        if (p->nextarc == NULL && p->adjvex != w)
        {                                  //v,w间没有弧
            q = (pArc)malloc(sizeof(Arc)); //插入弧v,w
            q->adjvex = w;
            q->nextarc = NULL;
            p->nextarc = q;
            G.Arcnum++; //更新弧数
            return OK;
        }
        else
            return ERROR; //v,w间已存在弧
    }
}
//10.删除弧
status DeleteArc(Digraph &G, TagType v, TagType w)
{
    pArc p, q;
    int i, j, num = 0;
    i = LocateVex(G, v);
    if (i == 0)
        return OVERFLOW; //不存在顶点v
    j = LocateVex(G, w);
    if (j == 0)
        return OVERFLOW; //不存在顶点w
    q = p = G.V[i - 1].firstArc;
    if (!p)
        return ERROR;                          //无邻接点
    for (; p->nextarc != NULL; p = p->nextarc) //查找弧v,w
        if (p->adjvex == w)
            break;
    if (p->nextarc == NULL && p->adjvex != w)
        return ERROR; //v,w间没有弧
    else
    { //删除v，w间的弧
        if (q == p)
            G.V[i - 1].firstArc = p->nextarc; //删除第一个结点
        else
        { //删除非第一个结点
            for (; q->nextarc != NULL; q = q->nextarc)
                if (q->nextarc == p)
                    break;
            q->nextarc = p->nextarc;
        }
        free(p);
        G.Arcnum--; //更新弧数
        return OK;
    }
}
//11.深度优先搜索遍历
status DFSTraverse(Digraph G, TagType v)
{
    int visited[G.Vexnum]; //查找标志数组
    TagType w;
    int *p = visited;
    status ret; //存储函数返回值
    pVex u;     //存储函数返回指针
    int i, j;
    for (i = 0; i < G.Vexnum; i++)
        visited[i] = 0; //初始化
    j = LocateVex(G, v);
    if (j == 0)
        return ERROR; //顶点v不在图中
    printf("图的深度搜索遍历序列为：\n");
    if (!visited[j - 1])
        DFS(G, G.V[j - 1].tag, visited, j - 1);
    for (i = 0; i < G.Vexnum; i++) //查找是否还有顶点未被查找
        if (!visited[i])           //如果有则继续深度搜索遍历
            DFS(G, G.V[i].tag, visited, i);
    return OK;
}
//12.广度优先搜索遍历
status BFSTraverse(Digraph G, TagType v, int *visited)
{
    TagType w, x;
    status ret; //存储函数返回值
    pVex u;     //存储函数返回指针
    int i, j;
    j = LocateVex(G, v);
    if (j == 0)
        return ERROR; //顶点v不在图中
    SElemType *q;
    SqStack S;
    InitStack(S);         //创建空栈
    Push(S, &G.V[j - 1]); //指向顶点的指针进栈
    printf("图的广度搜索遍历序列为：\n");
    for (q = S.base; q != S.top; q++)
    {
        w = (*q)->tag;
        j = LocateVex(G, w);
        if (!visited[j - 1])
        {                       //未被查找
            visited[j - 1] = 1; //修改查找标志
            printf("%c(%d) ", G.V[j - 1].tag, G.V[j - 1].data);
            ret = FirstAdjVex(G, w, u);
            for (; ret > 0; ret = NextAdjVex(G, w, x, u))
            { //依次查找邻接点
                if (u)
                {                             //找到邻接点
                    j = LocateVex(G, u->tag); //定位顶点
                    if (!visited[j - 1])
                        Push(S, &G.V[j - 1]); //新顶点进栈
                    x = u->tag;
                }
                else
                    break; //邻接点已查找完，退出循环
            }
        }
    }
    for (i = 0; i < G.Vexnum; i++) //查找是否还有顶点未被查找
        if (!visited[i])           //如果有则继续广度搜索遍历
            BFSTraverse(G, G.V[i].tag, visited);
    return OK;
}
//13.保存图
status SaveGraph(Digraph G, char filename[])
{
    FILE *fp; //文件指针
    pArc p = NULL;
    char ch = '#';
    int i;
    if (G.Vexnum == 0)
        return OVERFLOW;
    if ((fp = fopen(filename, "wb")) == NULL)
    {                               //以只写方式打开文件
        printf("文件打开错误！\n"); // 指针为空，出错
        return ERROR;
    }
    if (fwrite(&G.Vexnum, sizeof(int), 1, fp) != 1)
    { //保存顶点数
        printf("文件写入错误！\n");
        return ERROR;
    }
    if (fwrite(&G.Arcnum, sizeof(int), 1, fp) != 1)
    { //保存弧数
        printf("文件写入错误！\n");
        return ERROR;
    }
    for (i = 0; i < G.Vexnum; i++)
    { //依次保存每个顶点关键字，数据域及邻接点
        if (fwrite(&G.V[i].tag, sizeof(TagType), 1, fp) != 1)
        { //保存顶点关键字
            printf("文件写入错误！\n");
            return ERROR;
        }
        if (fwrite(&G.V[i].data, sizeof(ElemType), 1, fp) != 1)
        { //保存顶点数据域
            printf("文件写入错误！\n");
            return ERROR;
        }
        if (G.V[i].firstArc == NULL)
        { //无邻接表
            if (fwrite(&ch, sizeof(TagType), 1, fp) != 1)
            {
                //保存字符'#'表示邻接表为空
                printf("文件写入错误！\n");
                return ERROR;
            }
        }
        else
        { //有邻接表
            p = G.V[i].firstArc;
            for (; p->nextarc != NULL; p = p->nextarc)
            {
                if (fwrite(&p->adjvex, sizeof(TagType), 1, fp) != 1)
                {
                    //保存每个邻接点
                    printf("文件写入错误！\n");
                    return ERROR;
                }
            }
            if (fwrite(&p->adjvex, sizeof(TagType), 1, fp) != 1)
            {
                printf("文件写入错误！\n");
                return ERROR;
            }
            if (fwrite(&ch, sizeof(TagType), 1, fp) != 1)
            {
                //保存字符'#'表示邻接表结束
                printf("文件写入错误！\n");
                return ERROR;
            }
        }
    }
    fclose(fp); //关闭文件
    return OK;
}
//14.加载图
status GetGraph(Digraph &G, char filename[])
{
    FILE *fp; //文件指针
    pArc p, q;
    char ch;
    int i;
    if (G.Vexnum != 0)
        DestroyGraph(G); //如果图已存在，销毁图
    if ((fp = fopen(filename, "rb")) == NULL)
    {                               //以只读方式打开文件
        printf("文件打开错误！\n"); //指针为空，出错
        return ERROR;
    }
    if (fread(&G.Vexnum, sizeof(int), 1, fp) != 1)
    { //加载顶点数
        printf("文件读取错误！\n");
        return ERROR;
    }
    if (fread(&G.Arcnum, sizeof(int), 1, fp) != 1)
    { //加载弧数
        printf("文件读取错误！\n");
        return ERROR;
    }
    for (i = 0; i < G.Vexnum; i++)
    {
        //依次加载每个顶点的关键字，数据域及邻接点
        if (fread(&G.V[i].tag, sizeof(TagType), 1, fp) != 1)
        { //加载顶点关键字
            printf("文件读取错误！\n");
            return ERROR;
        }
        if (fread(&G.V[i].data, sizeof(ElemType), 1, fp) != 1)
        { //加载顶点数据域
            printf("文件读取错误！\n");
            return ERROR;
        }
        if (fread(&ch, sizeof(TagType), 1, fp) != 1)
        { //加载邻接点
            printf("文件读取错误！\n");
            return ERROR;
        }
        if (ch == '#')
            G.V[i].firstArc = NULL; //'#'表示邻接表为空
        else
        { //不为空，加载第一个邻接点
            p = (pArc)malloc(sizeof(Arc));
            p->adjvex = ch;
            p->nextarc = NULL;
            G.V[i].firstArc = p;
        }
        while (1)
        { //继续加载邻接点
            if (fread(&ch, sizeof(TagType), 1, fp) != 1)
            {
                printf("文件读取错误！\n");
                return ERROR;
            }
            if (ch == '#')
                break; //'#'表示邻接点以加载完，退出循环
            else
            {
                q = (pArc)malloc(sizeof(Arc));
                q->adjvex = ch;
                q->nextarc = NULL;
                p->nextarc = q;
                p = p->nextarc;
            }
        }
    }
    fclose(fp); //关闭文件
    return OK;
}
//15.改变操作的图
status ChangeGraph(int &t)
{
    printf(" 请输入要对第几个图进行操作 [1~100]\n");
    scanf("%d", &t);
    if (t > 0 && t < 101)
        return OK; //符合范围要求
    else
        return ERROR; //超出范围
}
//16.输出图及邻接表
status PrintGraph(Digraph G)
{
    pArc p, q;
    int i;
    printf("图G有%d个顶点，%d条弧！\n", G.Vexnum, G.Arcnum);
    //输出顶点数，弧数
    for (i = 0; i < G.Vexnum; i++)
    {                                                //依次输出顶点信息
        printf("%c(%d): ", G.V[i].tag, G.V[i].data); //输出顶点关键字及数据域
        p = G.V[i].firstArc;
        if (p)
        {
            while (p->nextarc != NULL)
            {
                printf("%c ", p->adjvex); //依次输出邻接点
                p = p->nextarc;
            }
            printf("%c ", p->adjvex); //输出最后一个邻接点
        }
        printf("\n");
    }
}

/*----------辅助函数----------*/
//1.深度搜索遍历辅助递归函数
status DFS(Digraph G, TagType v, int *visited, int i)
{
    pVex u;     //存储顶点指针
    TagType w;  //存储顶点关键字
    status ret; //存储函数返回值
    int j;
    if (!visited[i])
    {                       //该顶点未被查找过
        *(visited + i) = 1; //修改查找标志
        printf("%c(%d) ", G.V[i].tag, G.V[i].data);
    }
    else
        return OK;
    ret = FirstAdjVex(G, v, u);
    for (; ret > 0; ret = NextAdjVex(G, v, w, u))
    { //依次查找邻接点
        if (u)
        { //找到邻接点
            for (j = 0; j < G.Vexnum; j++)
                if (u->tag == G.V[j].tag)
                    break;              //定位顶点
            DFS(G, u->tag, visited, j); //从新顶点开始深度搜索遍历
            w = u->tag;
        }
        else
            break; //邻接点已查找完，退出循环
    }
    return OK;
}
//2.创建空栈
status InitStack(SqStack &S)
{
    S.base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if (!S.base)
        exit(OVERFLOW);            //存储空间分配失败
    S.top = S.base;                //栈顶指针等于栈底指针，即空栈
    S.stacksize = STACK_INIT_SIZE; //更新存储空间
    return OK;
}
//3.插入栈顶元素
status Push(SqStack &S, SElemType p)
{
    if (S.top - S.base >= S.stacksize)
    { //栈存储空间已满
        S.base = (SElemType *)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!S.base)
            exit(OVERFLOW);            //存储空间分配失败
        S.top = S.base + S.stacksize;  //更新栈顶指针
        S.stacksize += STACKINCREMENT; //更新存储空间
    }
    *S.top = p; //将元素插入栈顶
    S.top++;    //栈顶指针自增
    return OK;
}