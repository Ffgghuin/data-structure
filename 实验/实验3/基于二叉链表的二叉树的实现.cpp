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

/*----- 数据元素类型定义 -----*/
typedef int status;   //返回值类型
typedef int ElemType; //数据域类型
typedef char TagType; //关键字类型

/*----- 二叉树的存储表示 -----*/
typedef struct tree
{                        //二叉树的定义
    struct tree *lchild; //指针域
    struct tree *rchild;
    ElemType data;  //数据域
    TagType tag;    //关键字
} BiTree, *pBiTree; //结构及结构指针

/*----- 栈的顺序存储表示 -----*/
#define STACK_INIT_SIZE 100 //存储空间初始分配量
#define STACKINCREMENT 10   //分配增量

typedef pBiTree SElemType;
typedef struct
{                    //栈的定义
    SElemType *base; //栈底指针
    SElemType *top;  //栈顶指针
    int stacksize;   //已分配存储空间
} SqStack;

/*------ 函数声明 ------*/
status CreateBiTree(pBiTree &T);                              //创建树
status DestroyBiTree(pBiTree &T);                             //销毁树
status ClearBiTree(pBiTree &T);                               //清空树
status BiTreeEmpty(pBiTree T);                                //判断空树
status BiTreeDepth(pBiTree T);                                //求树深度
pBiTree LocateNode(pBiTree T, TagType e);                     //查找结点
status Assign(pBiTree &T, TagType e, ElemType value);         //结点赋值
pBiTree GetSibling(pBiTree T, TagType e, int &LR);            //获得兄弟结点
status InsertNode(pBiTree &T, TagType e, int LR, pBiTree &c); //插入结点
status DeleteNode(pBiTree &T, TagType e);                     //删除结点
status PreOrderTraverse(pBiTree T);                           //前序遍历
status InOrderTraverse(pBiTree T);                            //中序遍历
status PostOrderTraverse(pBiTree T);                          //后序遍历
status LevelOrderTraverse(pBiTree T);                         //按层遍历
status PrintBiTree(pBiTree T);                                //打印二叉树
status SaveBiTree(pBiTree T, char filename[]);                //保存二叉树
status GetBiTree(pBiTree &T, char filename[]);                //加载二叉树
status ChangeBiTree(int &t);                                  //改变操作的二叉树

/*------ 辅助函数声明 ------*/
status MAX(status a, status b);         //返回两数较大者的值
status InitStack(SqStack &S);           //创建空栈
status GetTop(SqStack S, SElemType &p); //返回栈顶元素
status Push(SqStack &S, SElemType p);   //插入栈顶元素
status Pop(SqStack &S, SElemType &p);   //删除栈顶元素并返回其值
pBiTree Parent(pBiTree T, pBiTree c);   //查找双亲结点
pBiTree RightNode(pBiTree T);           //返回指向最右结点的指针
status pow(status a, status b);         //返回a的b次方的值

/*----main函数----*/
int main(void)
{
    pBiTree c, T[100];  //结构指针及结构指针数组
    char filename[100]; //存储文件名字符串
    ElemType value;     //存储数据域值
    TagType e;          //存储关键字值
    status j;           //存储函数返回值
    pBiTree p = NULL;   //存储函数返回指针值
    int LR;             //0表示左，1表示右
    int i, t = 0;
    for (i = 0; i < 100; i++)
        T[i] = NULL; //初始化指针
    do
    {
        printf(" 请输入要对第几个二叉树进行操作 [1~100]\n");
        scanf("%d", &t);
        getchar();
    } while (t < 1 || t > 100);
    system("cls"); //清屏
    int op = 1;
    while (op)
    {
        system("cls");
        printf("\n\n");
        printf("      正在对第%d个二叉树进行操作    \n", t);
        printf("      Menu for Binary Tree On Sequence Structure \n");
        printf("---------------------------------------------------------\n");
        printf("    	  1. CreateBiTree      10. DeleteNode         \n");
        printf("    	  2. DestroyBiTree     11. PreOrderTraverse   \n");
        printf("    	  3. ClearBiTree       12. InOrderTraverse    \n");
        printf("    	  4. BiTreeEmpty       13. PostOrderTraverse  \n");
        printf("    	  5. BiTreeDepth       14. LevelOrderTraverse \n");
        printf("    	  6. LocateNode        15. PrintBiTree        \n");
        printf("    	  7. Assign            16. SaveBiTree         \n");
        printf("    	  8. GetSibling        17. GetBiTree          \n");
        printf("    	  9. InsertNode        18. ChangeBiTree       \n");
        printf("    	                        0. Exit               \n");
        printf("---------------------------------------------------------\n");
        printf("      请选择你的操作[0~18]:");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            //CreateBiTree
            if (T[t - 1] != NULL)
            {
                printf("二叉树已存在，创建失败！\n");
                getchar();
                getchar();
                break;
            }
            T[t - 1] = (pBiTree)malloc(sizeof(BiTree)); //创建头结点
            T[t - 1]->lchild = T[t - 1]->rchild = NULL; //初始化头结点指针域
            printf("请输入数据：（带空子树的二叉树前序遍历序列，'#'表示结点为空）\n");
            getchar();
            j = CreateBiTree(T[t - 1]->lchild);
            if (j == OK)
                printf("二叉树创建成功！\n");
            else
                printf("二叉树创建失败！\n");
            getchar();
            getchar();
            break;
        case 2:
            //DestroyBiTree
            j = DestroyBiTree(T[t - 1]);
            if (j == OK)
                printf("二叉树销毁成功！\n");
            else if (j == ERROR)
                printf("二叉树销毁失败！\n");
            else
                printf("二叉树不存在，销毁失败！\n");
            getchar();
            getchar();
            break;
        case 3:
            //ClearBiTree
            j = ClearBiTree(T[t - 1]);
            if (j == OK)
                printf("二叉树清空成功！\n");
            else if (j == ERROR)
                printf("二叉树是空树，清空失败！\n");
            else
                printf("二叉树不存在，清空失败！\n");
            getchar();
            getchar();
            break;
        case 4:
            //BiTreeEmpty
            j = BiTreeEmpty(T[t - 1]);
            if (j == OK)
                printf("二叉树是空树！\n");
            else if (j == ERROR)
                printf("二叉树不是空树！\n");
            else
                printf("二叉树不存在！\n");
            getchar();
            getchar();
            break;
        case 5:
            //BiTreeDepth
            if (T[t - 1] == NULL)
                printf("二叉树不存在！\n");
            else
            {
                i = BiTreeDepth(T[t - 1]->lchild);
                printf("二叉树的深度为%d！\n", i);
            }
            getchar();
            getchar();
            break;
        case 6:
            //LocateNode
            if (T[t - 1] == NULL)
                printf("二叉树不存在！\n");
            else
            {
                printf("请输入所要查找结点的关键字：\n");
                getchar();
                scanf("%c", &e);
                p = LocateNode(T[t - 1]->lchild, e);
                if (!p)
                    printf("所查找的结点不在二叉树中！\n");
                else
                    printf("所查找的结点为%c(%d)！\n", e, p->data);
            }
            getchar();
            getchar();
            break;
        case 7:
            //Assign
            if (T[t - 1] == NULL)
                printf("二叉树不存在！\n");
            else
            {
                printf("请输入所要赋值结点的关键字：\n");
                getchar();
                scanf("%c", &e);
                printf("请输入想要赋给结点的值：\n");
                scanf("%d", &value);
                j = Assign(T[t - 1]->lchild, e, value);
                if (j == OK)
                    printf("结点赋值成功！\n");
                else if (j == ERROR)
                    printf("所查找的结点不在二叉树中！\n");
                else
                    printf("结点赋值失败！\n");
            }
            getchar();
            getchar();
            break;
        case 8:
            //GetSibling
            if (T[t - 1] == NULL)
                printf("二叉树不存在！\n");
            else
            {
                printf("请输入所要查找结点的关键字：\n");
                getchar();
                scanf("%c", &e);
                p = GetSibling(T[t - 1]->lchild, e, LR);
                if (!p)
                {
                    if (LR == -2)
                        printf("所查找的结点不在二叉树中！\n");
                    else if (LR == -1)
                        printf("所查找的结点为根结点，无兄弟结点！\n");
                    else if (LR == 0)
                        printf("所查找的结点无左兄弟结点！\n");
                    else
                        printf("所查找的结点无右兄弟结点！\n");
                }
                else
                {
                    if (LR)
                        printf("所查找结点的右兄弟结点为%c(%d)！\n", p->tag, p->data);
                    else
                        printf("所查找结点的左兄弟结点为%c(%d)！\n", p->tag, p->data);
                }
            }
            getchar();
            getchar();
            break;
        case 9:
            //InsertNode
            if (T[t - 1] == NULL)
                printf("二叉树不存在！\n");
            else if (T[t - 1]->lchild == NULL)
                printf("二叉树为空树！\n");
            else
            {
                c = (pBiTree)malloc(sizeof(BiTree));
                c->lchild = c->rchild = NULL;
                printf("请输入所插入结点的关键字：\n");
                getchar();
                scanf("%c", &c->tag);
                printf("请输入所插入结点的数据域：\n");
                scanf("%d", &c->data);
                printf("请输入插入结点位置的关键字：\n");
                getchar();
                scanf("%c", &e);
                printf("请输入想插入结点为左孩子还是右孩子：（0表示左孩子，1表示右孩子）\n");
                scanf("%d", &LR);
                j = InsertNode(T[t - 1], e, LR, c);
                if (j)
                    printf("结点插入成功！\n");
                else
                    printf("所插入结点的位置不在二叉树中！\n");
            }
            getchar();
            getchar();
            break;
        case 10:
            //DeleteNode
            if (T[t - 1] == NULL)
                printf("二叉树不存在！\n");
            else if (T[t - 1]->lchild == NULL)
                printf("二叉树为空树！\n");
            else
            {
                printf("请输入所删除结点的关键字：\n");
                getchar();
                scanf("%c", &e);
                j = DeleteNode(T[t - 1], e);
                if (j)
                    printf("结点删除成功！\n");
                else
                    printf("所删除结点不在二叉树中!\n");
            }
            getchar();
            getchar();
            break;
        case 11: //PreOrderTraverse
        case 12: //InOrderTraverse
        case 13: //PostOrderTraverse
        case 14: //LevelOrderTraverse
            j = BiTreeEmpty(T[t - 1]);
            if (j == OK)
                printf("二叉树是空树！\n");
            else if (j == ERROR)
            {
                if (op == 11)
                    PreOrderTraverse(T[t - 1]->lchild);
                else if (op == 12)
                    InOrderTraverse(T[t - 1]->lchild);
                else if (op == 13)
                    PostOrderTraverse(T[t - 1]->lchild);
                else
                    LevelOrderTraverse(T[t - 1]->lchild);
            }
            else
                printf("二叉树不存在！\n");
            getchar();
            getchar();
            break;
        case 15:
            PrintBiTree(T[t - 1]->lchild);
            getchar();
            getchar();
            break;
        case 16:
            //SaveBiTree
            printf("请输入想要保存的文件名：\n");
            scanf("%s", filename);
            j = SaveBiTree(T[t - 1], filename);
            if (j == OK)
                printf("文件保存成功！\n");
            else if (j == OVERFLOW)
                printf("二叉树不存在，文件保存失败！\n");
            else
                printf("文件保存失败！\n");
            getchar();
            getchar();
            break;
        case 17:
            //GetBiTree
            printf("请输入想要加载的文件名：\n");
            scanf("%s", filename);
            j = GetBiTree(T[t - 1], filename);
            if (j == OK)
                printf("文件加载成功！\n");
            else
                printf("文件加载失败!\n");
            getchar();
            getchar();
            break;
        case 18:
            //ChangeBiTree
            i = t;
            if (ChangeBiTree(i) == OK)
            {
                t = i;
                printf("二叉树已成功改变至第%d个！\n", t);
            }
            else
                printf("二叉树改变失败！\n");
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
//1.创建树
status CreateBiTree(pBiTree &T)
{
    ElemType dAta;
    TagType tAg;
    //printf("请输入关键字：\n");
    scanf("%c", &tAg);
    getchar();
    if (tAg == '#')
        T = NULL;
    else
    {
        if (!(T = (BiTree *)malloc(sizeof(BiTree))))
            exit(OVERFLOW);
        //申请新的结点空间
        T->lchild = T->rchild = NULL; //指针初始化
        T->tag = tAg;                 //关键字赋值
        //printf("请输入结点值：\n");
        scanf("%d", &T->data); //数据域赋值
        getchar();
        CreateBiTree(T->lchild); //递归
        CreateBiTree(T->rchild);
    }
    return OK;
}
//2.销毁树
status DestroyBiTree(pBiTree &T)
{
    if (T)
    {
        if (T->lchild)
            DestroyBiTree(T->lchild); //递归
        if (T->rchild)
            DestroyBiTree(T->rchild);
        free(T);  //释放T指向的结点存储空间
        T = NULL; //T为空指针
        return OK;
    }
    else
        return OVERFLOW;
}
//3.清空树
status ClearBiTree(pBiTree &T)
{
    if (T == NULL)
        return OVERFLOW; //树不存在
    else if (T->lchild == NULL)
        return ERROR;         //空树
    DestroyBiTree(T->lchild); //不为空树
    return OK;
}
//4.判断空树
status BiTreeEmpty(pBiTree T)
{
    if (T == NULL)
        return OVERFLOW; //树不存在
    else if (T->lchild == NULL)
        return OK; //空树
    else
        return ERROR; //不为空树
}
//5.求深度
status BiTreeDepth(pBiTree T)
{
    if (!T)
        return 0; //T为NULL则返回0
    else
    {
        if (T->lchild == NULL && T->rchild == NULL)
            return 1; //无孩子节点返回1
        else
            return 1 + MAX(BiTreeDepth(T->lchild), BiTreeDepth(T->rchild));
    }
}
//6.查找结点
pBiTree LocateNode(pBiTree T, TagType e)
{
    SElemType p;
    SqStack S;
    InitStack(S); //创建空栈
    Push(S, T);   //指向根结点的指针进栈
    while (S.base != S.top)
    { //不为空栈时循环
        while (GetTop(S, p) && p)
        {                       //向左走到尽头
            Push(S, p->lchild); //路径上指向各结点的指针依次进栈
            if (p->tag == e)
                return p; //找到结点返回指针值
        }
        Pop(S, p); //空指针退栈
        if (S.base != S.top)
        {                       //不为空栈时
            Pop(S, p);          //p指向上一个结点，且该指针退栈
            Push(S, p->rchild); //向右走一步
        }
    }
    return NULL;
}
//7.结点赋值
status Assign(pBiTree &T, TagType e, ElemType value)
{
    pBiTree p;
    p = LocateNode(T, e); //查找结点
    if (p)
    {                    //p不为空，即找到结点
        p->data = value; //赋值
        return OK;
    }
    else
        return ERROR; //p为空，返回ERROR
}
//8.获得兄弟结点
pBiTree GetSibling(pBiTree T, TagType e, int &LR)
{
    pBiTree p = NULL, q = NULL;
    p = LocateNode(T, e); //查找结点
    if (!p)
    { //未找到结点，结点不在树中,LR=-2
        LR = -2;
        return NULL;
    }
    if (p == T)
    { //根结点无兄弟节点,LR=-1
        LR = -1;
        return NULL;
    }
    q = Parent(T, p); //查找双亲结点
    if (q->rchild == p)
    {
        LR = 0; //找到左兄弟，LR=0
        return q->lchild;
    }
    if (q->lchild == p)
    {
        LR = 1; //找到右兄弟，LR=1
        return q->rchild;
    }
}
//9.插入结点
status InsertNode(pBiTree &T, TagType e, int LR, pBiTree &c)
{
    pBiTree p, q;
    p = LocateNode(T, e); //查找结点
    if (!p)
        return ERROR; //所查找的结点不在树中
    if (LR)
    {
        q = p->rchild; //q指向右孩子
        p->rchild = c; //插入结点为新的右孩子
    }
    else
    {
        q = p->lchild; //q指向左孩子
        p->lchild = c; //插入结点为新的左孩子
    }
    c->rchild = q; //原有的子树为插入结点的右子树
    return OK;
}
//10.删除结点
status DeleteNode(pBiTree &T, TagType e)
{
    int l = 1, r = 1; //l左，r右，0表示无孩子，1表示有孩子
    int m = 1;        //0表示删除左孩子，1表示删除右孩子
    pBiTree p, q, t;
    p = LocateNode(T->lchild, e); //查找结点，T->lchild指向根结点
    if (!p)
        return ERROR; //所查找的结点不在树中
    if (p == T->lchild)
        q = T; //所查找的结点为根结点
    else
        q = Parent(T, p); //查找双亲结点
    if (q->lchild == p)
        m = 0; //所删除结点为左孩子
    if (p->lchild == NULL)
        l = 0; //无左孩子
    if (p->rchild == NULL)
        r = 0; //无右孩子
    if (!l && !r)
    { //结点度为0，直接删除结点
        if (m)
            q->rchild = NULL; //删除右孩子
        else
            q->lchild = NULL; //删除左孩子
    }
    else if (l && r)
    { //结点度为2
        if (m)
            q->rchild = p->lchild; //p的左孩子代替p的位置
        else
            q->lchild = p->lchild; //p的左孩子代替p的位置
        t = RightNode(p->lchild);  //查找p左子树的最右结点
        t->rchild = p->rchild;     //p的右孩子代替t的右子树
    }
    else
    { //结点度为1
        if (l)
        { //p只有左孩子
            if (m)
                q->rchild = p->lchild; //删除q的右孩子
            else
                q->lchild = p->lchild; //删除q的左孩子
        }
        else
        { //p只有右孩子
            if (m)
                q->rchild = p->rchild; //删除q的右孩子
            else
                q->lchild = p->rchild; //删除q的左孩子
        }
    }
    free(p); //释放结点空间
    return OK;
}
//11.前序遍历
status PreOrderTraverse(pBiTree T)
{
    /*------------------递归前序遍历-----------------*/
    //	if(T == NULL) return OK;
    //	printf("%c(%d) ", T->tag, T->data); //输出结点关键字及数据域
    //	PreOrderTraverse(T->lchild);  //先序遍历左子树
    //	PreOrderTraverse(T->rchild);  //先序遍历右子树
    //	return OK;

    /*-----------------非递归前序遍历----------------*/
    SElemType p;
    SqStack S;
    InitStack(S); //创建空栈
    Push(S, T);   //指向根结点的指针进栈
    while (S.base != S.top)
    { //不为空栈时循环
        while (GetTop(S, p) && p)
        {                                       //向左走到尽头
            Push(S, p->lchild);                 //路径上指向各结点的指针依次进栈
            printf("%c(%d) ", p->tag, p->data); //输出结点关键字及数据域
        }
        Pop(S, p); //空指针退栈
        if (S.base != S.top)
        {                       //不为空栈时
            Pop(S, p);          //p指向上一个结点，且该指针退栈
            Push(S, p->rchild); //向右走一步
        }
    }
    return OK;
}
//12.中序遍历
status InOrderTraverse(pBiTree T)
{
    if (T == NULL)
        return OK;
    InOrderTraverse(T->lchild);         //中序遍历左子树
    printf("%c(%d) ", T->tag, T->data); //输出结点的关键字及数据域
    InOrderTraverse(T->rchild);         //中序遍历右子树
    return OK;
}
//13.后序遍历
status PostOrderTraverse(pBiTree T)
{
    if (T == NULL)
        return OK;
    PostOrderTraverse(T->lchild);       //后序遍历左子树
    PostOrderTraverse(T->rchild);       //后序遍历右子树
    printf("%c(%d) ", T->tag, T->data); //输出结点的关键字及数据域
    return OK;
}
//14.按层遍历
status LevelOrderTraverse(pBiTree T)
{
    SElemType *p;
    SqStack S;
    InitStack(S); //创建空栈
    Push(S, T);   //指向根结点的指针进栈
    for (p = S.base; p != S.top; p++)
    {
        printf("%c(%d) ", (*p)->tag, (*p)->data); //输出结点关键字及数据域
        if ((*p)->lchild)
            Push(S, (*p)->lchild); //指向左孩子的指针入栈
        if ((*p)->rchild)
            Push(S, (*p)->rchild); //指向右孩子的指针入栈
    }
    return OK;
}
//15.打印二叉树
status PrintBiTree(pBiTree T)
{
    int i, j, m, n;
    SElemType *p;
    SqStack S;
    InitStack(S); //创建空栈
    Push(S, T);   //指向根结点的指针进栈
    for (p = S.base, j = BiTreeDepth(T), n = 3; j > 0; p++)
    {
        m = pow(2, j);
        for (i = m - 1; i > 0; i--)
            printf(" ");
        if (*p == NULL)
        {
            printf(" ");
            Push(S, NULL);
            Push(S, NULL);
        }
        else
        {
            printf("%c", (*p)->tag); //输出结点关键字
            Push(S, (*p)->lchild);   //指向左孩子的指针入栈
            Push(S, (*p)->rchild);   //指向右孩子的指针入栈
        }
        for (i = m; i > 0; i--)
            printf(" ");
        if ((S.top - S.base) == n)
        {
            j--;
            n = 2 * n + 1;
            printf("\n");
        }
    }
    return OK;
}
//16.保存二叉树
status SaveBiTree(pBiTree T, char filename[])
{
    FILE *fp; //文件指针
    if (T == NULL)
        return OVERFLOW;
    if ((fp = fopen(filename, "wb")) == NULL)
    {                               //以只写方式打开文件
        printf("文件打开错误！\n"); // 指针为空，出错
        return ERROR;
    }
    SElemType p;
    TagType ch = '#'; //表示空结点
    SqStack S;
    InitStack(S);       //创建空栈
    Push(S, T->lchild); //指向根结点的指针进栈
    while (S.base != S.top)
    { //不为空栈时循环
        while (GetTop(S, p) && p)
        {                       //向左走到尽头
            Push(S, p->lchild); //路径上指向各结点的指针依次进栈
            if (fwrite(&(p->tag), sizeof(TagType), 1, fp) != 1)
            {                               //写入关键字
                printf("文件写入错误！\n"); //返回值不为1则写入出错
                return ERROR;
            }
            if (fwrite(&(p->data), sizeof(ElemType), 1, fp) != 1)
            {                               //写入数据域
                printf("文件写入错误！\n"); //返回值不为1则写入出错
                return ERROR;
            }
        }
        //指针为空，存入字符'#'代表空结点
        if (fwrite(&ch, sizeof(TagType), 1, fp) != 1)
        {
            printf("文件写入错误！\n"); //返回值不为1则写入出错
            return ERROR;
        }
        Pop(S, p); //空指针退栈
        if (S.base != S.top)
        {                       //不为空栈时
            Pop(S, p);          //p指向上一个结点，且该指针退栈
            Push(S, p->rchild); //向右走一步
        }
    }
    if (fwrite(&ch, sizeof(TagType), 1, fp) != 1)
    {
        //多存入一个字符'#'方便加载时操作
        printf("文件写入错误！\n"); //该'#'标示头结点的右孩子为空
        return ERROR;
    }
    fclose(fp); //关闭文件
    return OK;
}
//17.加载二叉树
status GetBiTree(pBiTree &T, char filename[])
{
    FILE *fp;   //文件指针
    TagType ch; //存储关键字
    if (T)
        DestroyBiTree(T);                //如果树已存在，销毁二叉树
    T = (pBiTree)malloc(sizeof(BiTree)); //创建头结点
    T->lchild = T->rchild = NULL;
    if ((fp = fopen(filename, "rb")) == NULL)
    {                               //以只读方式打开文件
        printf("文件打开错误！\n"); //指针为空，出错
        return ERROR;
    }
    SElemType p, q;
    SqStack S;
    InitStack(S); //创建空栈
    Push(S, T);   //指向头结点的指针进栈
    while (S.base != S.top)
    { //不为空栈时循环
        //向左走到尽头
        while (1)
        {
            if (fread(&ch, sizeof(TagType), 1, fp) != 1)
            {
                printf("文件读取错误！\n"); //返回值不为1则写入出错
                return ERROR;
            }
            GetTop(S, p);
            if (ch == '#')
            { //读入#则无左孩子，退出循环
                p->lchild = NULL;
                break;
            }
            else
            {
                q = (pBiTree)malloc(sizeof(BiTree)); //申请新结点储存数据
                q->tag = ch;                         //关键字赋值
                if (fread(&(q->data), sizeof(ElemType), 1, fp) != 1)
                { //数据域赋值
                    printf("文件读取错误！\n");
                    return ERROR;
                }
                p->lchild = q; //将指向左孩子的指针指向新结点
                Push(S, q);    //指向左孩子的指针进栈
            }
        }
        //右走一步
        while (1)
        {
            if (fread(&ch, sizeof(TagType), 1, fp) != 1)
            {
                printf("文件读取错误！\n"); //返回值不为1则写入出错
                return ERROR;
            }
            GetTop(S, p);
            if (ch == '#')
            { //读入#则无右孩子
                p->rchild = NULL;
                Pop(S, p); //栈顶元素出栈，即指向双亲结点的指针出栈
                if (S.base == S.top)
                    break; //栈空时退出循环
            }
            else
            {
                q = (pBiTree)malloc(sizeof(BiTree)); //申请新结点储存数据
                q->tag = ch;                         //关键字赋值
                if (fread(&(q->data), sizeof(ElemType), 1, fp) != 1)
                { //数据域赋值
                    printf("文件读取错误！\n");
                    return ERROR;
                }
                p->rchild = q; //将指向右孩子的指针指向新结点
                Pop(S, p);     //栈顶元素出栈，即指向双亲结点的指针出栈
                Push(S, q);    //指向右孩子的指针进栈
                break;
            }
        }
    }
    fclose(fp); //关闭文件
    return OK;
}
//18.更改二叉树
status ChangeBiTree(int &t)
{
    printf(" 请输入要对第几个二叉树进行操作 [1~100]\n");
    scanf("%d", &t);
    if (t > 0 && t < 101)
        return OK; //符合范围要求
    else
        return ERROR; //超出范围
}

/*--------------辅助函数定义---------------*/
//1.返回两数中较大值
status MAX(status a, status b)
{
    if (a > b)
        return a; //a较大则返回a
    else
        return b; //否则返回b
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
//3.返回栈顶元素
status GetTop(SqStack S, SElemType &p)
{
    if (S.base == S.top)
        return ERROR; //栈为空，无元素
    p = *(S.top - 1); //传出栈顶元素值
    return OK;
}
//4.插入栈顶元素
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
//5.删除栈顶元素
status Pop(SqStack &S, SElemType &p)
{
    if (S.base == S.top)
        return ERROR; //栈为空，无栈顶元素
    S.top--;          //栈顶指针自减
    p = *S.top;       //传出栈顶元素的值
    return OK;
}
//6.查找双亲结点
pBiTree Parent(pBiTree T, pBiTree c)
{
    SElemType p = NULL;
    SqStack S;
    InitStack(S); //创建空栈
    Push(S, T);   //指向根结点的指针进栈
    while (S.base != S.top)
    { //不为空栈时循环
        while (GetTop(S, p) && p)
        {                       //向左走到尽头
            Push(S, p->lchild); //路径上指向各结点的指针依次进栈
            if (p->lchild == c || p->rchild == c)
                return p; //找到双亲结点
        }
        Pop(S, p); //空指针退栈
        if (S.base != S.top)
        {                       //不为空栈时
            Pop(S, p);          //p指向上一个结点，且该指针退栈
            Push(S, p->rchild); //向右走一步
        }
    }
    return NULL;
}
//7.返回最右结点的指针
pBiTree RightNode(pBiTree T)
{
    if (T->rchild == NULL)
        return T;
    SElemType p, q;
    SqStack S;
    InitStack(S); //创建空栈
    Push(S, T);   //指向根结点的指针进栈
    while (S.base != S.top)
    { //不为空栈时循环
        while (GetTop(S, p) && p)
        {                       //向左走到尽头
            Push(S, p->lchild); //路径上指向各结点的指针依次进栈
        }
        Pop(S, p); //空指针退栈
        if (S.base != S.top)
        {                       //不为空栈时
            Pop(S, p);          //p指向上一个结点，且该指针退栈
            Push(S, p->rchild); //向右走一步
            if (p->rchild == NULL)
                q = p;
        }
    }
    return q;
}
//8.返回a的b次方的值
status pow(status a, status b)
{
    status i, num;
    for (i = 0, num = 1; i < b; i++)
    {
        num = num * a;
    }
    return num;
}