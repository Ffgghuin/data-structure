#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

/*--------- �������״̬���� ---------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2

/*----- ����Ԫ�����Ͷ��� -----*/
typedef int status;   //����ֵ����
typedef int ElemType; //����������
typedef char TagType; //�ؼ�������

/*----- �������Ĵ洢��ʾ -----*/
typedef struct tree
{                        //�������Ķ���
    struct tree *lchild; //ָ����
    struct tree *rchild;
    ElemType data;  //������
    TagType tag;    //�ؼ���
} BiTree, *pBiTree; //�ṹ���ṹָ��

/*----- ջ��˳��洢��ʾ -----*/
#define STACK_INIT_SIZE 100 //�洢�ռ��ʼ������
#define STACKINCREMENT 10   //��������

typedef pBiTree SElemType;
typedef struct
{                    //ջ�Ķ���
    SElemType *base; //ջ��ָ��
    SElemType *top;  //ջ��ָ��
    int stacksize;   //�ѷ���洢�ռ�
} SqStack;

/*------ �������� ------*/
status CreateBiTree(pBiTree &T);                              //������
status DestroyBiTree(pBiTree &T);                             //������
status ClearBiTree(pBiTree &T);                               //�����
status BiTreeEmpty(pBiTree T);                                //�жϿ���
status BiTreeDepth(pBiTree T);                                //�������
pBiTree LocateNode(pBiTree T, TagType e);                     //���ҽ��
status Assign(pBiTree &T, TagType e, ElemType value);         //��㸳ֵ
pBiTree GetSibling(pBiTree T, TagType e, int &LR);            //����ֵܽ��
status InsertNode(pBiTree &T, TagType e, int LR, pBiTree &c); //������
status DeleteNode(pBiTree &T, TagType e);                     //ɾ�����
status PreOrderTraverse(pBiTree T);                           //ǰ�����
status InOrderTraverse(pBiTree T);                            //�������
status PostOrderTraverse(pBiTree T);                          //�������
status LevelOrderTraverse(pBiTree T);                         //�������
status PrintBiTree(pBiTree T);                                //��ӡ������
status SaveBiTree(pBiTree T, char filename[]);                //���������
status GetBiTree(pBiTree &T, char filename[]);                //���ض�����
status ChangeBiTree(int &t);                                  //�ı�����Ķ�����

/*------ ������������ ------*/
status MAX(status a, status b);         //���������ϴ��ߵ�ֵ
status InitStack(SqStack &S);           //������ջ
status GetTop(SqStack S, SElemType &p); //����ջ��Ԫ��
status Push(SqStack &S, SElemType p);   //����ջ��Ԫ��
status Pop(SqStack &S, SElemType &p);   //ɾ��ջ��Ԫ�ز�������ֵ
pBiTree Parent(pBiTree T, pBiTree c);   //����˫�׽��
pBiTree RightNode(pBiTree T);           //����ָ�����ҽ���ָ��
status pow(status a, status b);         //����a��b�η���ֵ

/*----main����----*/
int main(void)
{
    pBiTree c, T[100];  //�ṹָ�뼰�ṹָ������
    char filename[100]; //�洢�ļ����ַ���
    ElemType value;     //�洢������ֵ
    TagType e;          //�洢�ؼ���ֵ
    status j;           //�洢��������ֵ
    pBiTree p = NULL;   //�洢��������ָ��ֵ
    int LR;             //0��ʾ��1��ʾ��
    int i, t = 0;
    for (i = 0; i < 100; i++)
        T[i] = NULL; //��ʼ��ָ��
    do
    {
        printf(" ������Ҫ�Եڼ������������в��� [1~100]\n");
        scanf("%d", &t);
        getchar();
    } while (t < 1 || t > 100);
    system("cls"); //����
    int op = 1;
    while (op)
    {
        system("cls");
        printf("\n\n");
        printf("      ���ڶԵ�%d�����������в���    \n", t);
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
        printf("      ��ѡ����Ĳ���[0~18]:");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            //CreateBiTree
            if (T[t - 1] != NULL)
            {
                printf("�������Ѵ��ڣ�����ʧ�ܣ�\n");
                getchar();
                getchar();
                break;
            }
            T[t - 1] = (pBiTree)malloc(sizeof(BiTree)); //����ͷ���
            T[t - 1]->lchild = T[t - 1]->rchild = NULL; //��ʼ��ͷ���ָ����
            printf("���������ݣ������������Ķ�����ǰ��������У�'#'��ʾ���Ϊ�գ�\n");
            getchar();
            j = CreateBiTree(T[t - 1]->lchild);
            if (j == OK)
                printf("�����������ɹ���\n");
            else
                printf("����������ʧ�ܣ�\n");
            getchar();
            getchar();
            break;
        case 2:
            //DestroyBiTree
            j = DestroyBiTree(T[t - 1]);
            if (j == OK)
                printf("���������ٳɹ���\n");
            else if (j == ERROR)
                printf("����������ʧ�ܣ�\n");
            else
                printf("�����������ڣ�����ʧ�ܣ�\n");
            getchar();
            getchar();
            break;
        case 3:
            //ClearBiTree
            j = ClearBiTree(T[t - 1]);
            if (j == OK)
                printf("��������ճɹ���\n");
            else if (j == ERROR)
                printf("�������ǿ��������ʧ�ܣ�\n");
            else
                printf("�����������ڣ����ʧ�ܣ�\n");
            getchar();
            getchar();
            break;
        case 4:
            //BiTreeEmpty
            j = BiTreeEmpty(T[t - 1]);
            if (j == OK)
                printf("�������ǿ�����\n");
            else if (j == ERROR)
                printf("���������ǿ�����\n");
            else
                printf("�����������ڣ�\n");
            getchar();
            getchar();
            break;
        case 5:
            //BiTreeDepth
            if (T[t - 1] == NULL)
                printf("�����������ڣ�\n");
            else
            {
                i = BiTreeDepth(T[t - 1]->lchild);
                printf("�����������Ϊ%d��\n", i);
            }
            getchar();
            getchar();
            break;
        case 6:
            //LocateNode
            if (T[t - 1] == NULL)
                printf("�����������ڣ�\n");
            else
            {
                printf("��������Ҫ���ҽ��Ĺؼ��֣�\n");
                getchar();
                scanf("%c", &e);
                p = LocateNode(T[t - 1]->lchild, e);
                if (!p)
                    printf("�����ҵĽ�㲻�ڶ������У�\n");
                else
                    printf("�����ҵĽ��Ϊ%c(%d)��\n", e, p->data);
            }
            getchar();
            getchar();
            break;
        case 7:
            //Assign
            if (T[t - 1] == NULL)
                printf("�����������ڣ�\n");
            else
            {
                printf("��������Ҫ��ֵ���Ĺؼ��֣�\n");
                getchar();
                scanf("%c", &e);
                printf("��������Ҫ��������ֵ��\n");
                scanf("%d", &value);
                j = Assign(T[t - 1]->lchild, e, value);
                if (j == OK)
                    printf("��㸳ֵ�ɹ���\n");
                else if (j == ERROR)
                    printf("�����ҵĽ�㲻�ڶ������У�\n");
                else
                    printf("��㸳ֵʧ�ܣ�\n");
            }
            getchar();
            getchar();
            break;
        case 8:
            //GetSibling
            if (T[t - 1] == NULL)
                printf("�����������ڣ�\n");
            else
            {
                printf("��������Ҫ���ҽ��Ĺؼ��֣�\n");
                getchar();
                scanf("%c", &e);
                p = GetSibling(T[t - 1]->lchild, e, LR);
                if (!p)
                {
                    if (LR == -2)
                        printf("�����ҵĽ�㲻�ڶ������У�\n");
                    else if (LR == -1)
                        printf("�����ҵĽ��Ϊ����㣬���ֵܽ�㣡\n");
                    else if (LR == 0)
                        printf("�����ҵĽ�������ֵܽ�㣡\n");
                    else
                        printf("�����ҵĽ�������ֵܽ�㣡\n");
                }
                else
                {
                    if (LR)
                        printf("�����ҽ������ֵܽ��Ϊ%c(%d)��\n", p->tag, p->data);
                    else
                        printf("�����ҽ������ֵܽ��Ϊ%c(%d)��\n", p->tag, p->data);
                }
            }
            getchar();
            getchar();
            break;
        case 9:
            //InsertNode
            if (T[t - 1] == NULL)
                printf("�����������ڣ�\n");
            else if (T[t - 1]->lchild == NULL)
                printf("������Ϊ������\n");
            else
            {
                c = (pBiTree)malloc(sizeof(BiTree));
                c->lchild = c->rchild = NULL;
                printf("��������������Ĺؼ��֣�\n");
                getchar();
                scanf("%c", &c->tag);
                printf("���������������������\n");
                scanf("%d", &c->data);
                printf("�����������λ�õĹؼ��֣�\n");
                getchar();
                scanf("%c", &e);
                printf("�������������Ϊ���ӻ����Һ��ӣ���0��ʾ���ӣ�1��ʾ�Һ��ӣ�\n");
                scanf("%d", &LR);
                j = InsertNode(T[t - 1], e, LR, c);
                if (j)
                    printf("������ɹ���\n");
                else
                    printf("���������λ�ò��ڶ������У�\n");
            }
            getchar();
            getchar();
            break;
        case 10:
            //DeleteNode
            if (T[t - 1] == NULL)
                printf("�����������ڣ�\n");
            else if (T[t - 1]->lchild == NULL)
                printf("������Ϊ������\n");
            else
            {
                printf("��������ɾ�����Ĺؼ��֣�\n");
                getchar();
                scanf("%c", &e);
                j = DeleteNode(T[t - 1], e);
                if (j)
                    printf("���ɾ���ɹ���\n");
                else
                    printf("��ɾ����㲻�ڶ�������!\n");
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
                printf("�������ǿ�����\n");
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
                printf("�����������ڣ�\n");
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
            printf("��������Ҫ������ļ�����\n");
            scanf("%s", filename);
            j = SaveBiTree(T[t - 1], filename);
            if (j == OK)
                printf("�ļ�����ɹ���\n");
            else if (j == OVERFLOW)
                printf("�����������ڣ��ļ�����ʧ�ܣ�\n");
            else
                printf("�ļ�����ʧ�ܣ�\n");
            getchar();
            getchar();
            break;
        case 17:
            //GetBiTree
            printf("��������Ҫ���ص��ļ�����\n");
            scanf("%s", filename);
            j = GetBiTree(T[t - 1], filename);
            if (j == OK)
                printf("�ļ����سɹ���\n");
            else
                printf("�ļ�����ʧ��!\n");
            getchar();
            getchar();
            break;
        case 18:
            //ChangeBiTree
            i = t;
            if (ChangeBiTree(i) == OK)
            {
                t = i;
                printf("�������ѳɹ��ı�����%d����\n", t);
            }
            else
                printf("�������ı�ʧ�ܣ�\n");
            getchar();
            getchar();
            break;
        case 0:
            //�˳�
            break;
        default:
            //�������
            printf("����������������룡\n");
            getchar();
            getchar();
            break;
        } //end of switch
    }     //end of while
    printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
} //end of main()

/*---------��������---------*/
//1.������
status CreateBiTree(pBiTree &T)
{
    ElemType dAta;
    TagType tAg;
    //printf("������ؼ��֣�\n");
    scanf("%c", &tAg);
    getchar();
    if (tAg == '#')
        T = NULL;
    else
    {
        if (!(T = (BiTree *)malloc(sizeof(BiTree))))
            exit(OVERFLOW);
        //�����µĽ��ռ�
        T->lchild = T->rchild = NULL; //ָ���ʼ��
        T->tag = tAg;                 //�ؼ��ָ�ֵ
        //printf("��������ֵ��\n");
        scanf("%d", &T->data); //������ֵ
        getchar();
        CreateBiTree(T->lchild); //�ݹ�
        CreateBiTree(T->rchild);
    }
    return OK;
}
//2.������
status DestroyBiTree(pBiTree &T)
{
    if (T)
    {
        if (T->lchild)
            DestroyBiTree(T->lchild); //�ݹ�
        if (T->rchild)
            DestroyBiTree(T->rchild);
        free(T);  //�ͷ�Tָ��Ľ��洢�ռ�
        T = NULL; //TΪ��ָ��
        return OK;
    }
    else
        return OVERFLOW;
}
//3.�����
status ClearBiTree(pBiTree &T)
{
    if (T == NULL)
        return OVERFLOW; //��������
    else if (T->lchild == NULL)
        return ERROR;         //����
    DestroyBiTree(T->lchild); //��Ϊ����
    return OK;
}
//4.�жϿ���
status BiTreeEmpty(pBiTree T)
{
    if (T == NULL)
        return OVERFLOW; //��������
    else if (T->lchild == NULL)
        return OK; //����
    else
        return ERROR; //��Ϊ����
}
//5.�����
status BiTreeDepth(pBiTree T)
{
    if (!T)
        return 0; //TΪNULL�򷵻�0
    else
    {
        if (T->lchild == NULL && T->rchild == NULL)
            return 1; //�޺��ӽڵ㷵��1
        else
            return 1 + MAX(BiTreeDepth(T->lchild), BiTreeDepth(T->rchild));
    }
}
//6.���ҽ��
pBiTree LocateNode(pBiTree T, TagType e)
{
    SElemType p;
    SqStack S;
    InitStack(S); //������ջ
    Push(S, T);   //ָ�������ָ���ջ
    while (S.base != S.top)
    { //��Ϊ��ջʱѭ��
        while (GetTop(S, p) && p)
        {                       //�����ߵ���ͷ
            Push(S, p->lchild); //·����ָ�������ָ�����ν�ջ
            if (p->tag == e)
                return p; //�ҵ���㷵��ָ��ֵ
        }
        Pop(S, p); //��ָ����ջ
        if (S.base != S.top)
        {                       //��Ϊ��ջʱ
            Pop(S, p);          //pָ����һ����㣬�Ҹ�ָ����ջ
            Push(S, p->rchild); //������һ��
        }
    }
    return NULL;
}
//7.��㸳ֵ
status Assign(pBiTree &T, TagType e, ElemType value)
{
    pBiTree p;
    p = LocateNode(T, e); //���ҽ��
    if (p)
    {                    //p��Ϊ�գ����ҵ����
        p->data = value; //��ֵ
        return OK;
    }
    else
        return ERROR; //pΪ�գ�����ERROR
}
//8.����ֵܽ��
pBiTree GetSibling(pBiTree T, TagType e, int &LR)
{
    pBiTree p = NULL, q = NULL;
    p = LocateNode(T, e); //���ҽ��
    if (!p)
    { //δ�ҵ���㣬��㲻������,LR=-2
        LR = -2;
        return NULL;
    }
    if (p == T)
    { //��������ֵܽڵ�,LR=-1
        LR = -1;
        return NULL;
    }
    q = Parent(T, p); //����˫�׽��
    if (q->rchild == p)
    {
        LR = 0; //�ҵ����ֵܣ�LR=0
        return q->lchild;
    }
    if (q->lchild == p)
    {
        LR = 1; //�ҵ����ֵܣ�LR=1
        return q->rchild;
    }
}
//9.������
status InsertNode(pBiTree &T, TagType e, int LR, pBiTree &c)
{
    pBiTree p, q;
    p = LocateNode(T, e); //���ҽ��
    if (!p)
        return ERROR; //�����ҵĽ�㲻������
    if (LR)
    {
        q = p->rchild; //qָ���Һ���
        p->rchild = c; //������Ϊ�µ��Һ���
    }
    else
    {
        q = p->lchild; //qָ������
        p->lchild = c; //������Ϊ�µ�����
    }
    c->rchild = q; //ԭ�е�����Ϊ�������������
    return OK;
}
//10.ɾ�����
status DeleteNode(pBiTree &T, TagType e)
{
    int l = 1, r = 1; //l��r�ң�0��ʾ�޺��ӣ�1��ʾ�к���
    int m = 1;        //0��ʾɾ�����ӣ�1��ʾɾ���Һ���
    pBiTree p, q, t;
    p = LocateNode(T->lchild, e); //���ҽ�㣬T->lchildָ������
    if (!p)
        return ERROR; //�����ҵĽ�㲻������
    if (p == T->lchild)
        q = T; //�����ҵĽ��Ϊ�����
    else
        q = Parent(T, p); //����˫�׽��
    if (q->lchild == p)
        m = 0; //��ɾ�����Ϊ����
    if (p->lchild == NULL)
        l = 0; //������
    if (p->rchild == NULL)
        r = 0; //���Һ���
    if (!l && !r)
    { //����Ϊ0��ֱ��ɾ�����
        if (m)
            q->rchild = NULL; //ɾ���Һ���
        else
            q->lchild = NULL; //ɾ������
    }
    else if (l && r)
    { //����Ϊ2
        if (m)
            q->rchild = p->lchild; //p�����Ӵ���p��λ��
        else
            q->lchild = p->lchild; //p�����Ӵ���p��λ��
        t = RightNode(p->lchild);  //����p�����������ҽ��
        t->rchild = p->rchild;     //p���Һ��Ӵ���t��������
    }
    else
    { //����Ϊ1
        if (l)
        { //pֻ������
            if (m)
                q->rchild = p->lchild; //ɾ��q���Һ���
            else
                q->lchild = p->lchild; //ɾ��q������
        }
        else
        { //pֻ���Һ���
            if (m)
                q->rchild = p->rchild; //ɾ��q���Һ���
            else
                q->lchild = p->rchild; //ɾ��q������
        }
    }
    free(p); //�ͷŽ��ռ�
    return OK;
}
//11.ǰ�����
status PreOrderTraverse(pBiTree T)
{
    /*------------------�ݹ�ǰ�����-----------------*/
    //	if(T == NULL) return OK;
    //	printf("%c(%d) ", T->tag, T->data); //������ؼ��ּ�������
    //	PreOrderTraverse(T->lchild);  //�������������
    //	PreOrderTraverse(T->rchild);  //�������������
    //	return OK;

    /*-----------------�ǵݹ�ǰ�����----------------*/
    SElemType p;
    SqStack S;
    InitStack(S); //������ջ
    Push(S, T);   //ָ�������ָ���ջ
    while (S.base != S.top)
    { //��Ϊ��ջʱѭ��
        while (GetTop(S, p) && p)
        {                                       //�����ߵ���ͷ
            Push(S, p->lchild);                 //·����ָ�������ָ�����ν�ջ
            printf("%c(%d) ", p->tag, p->data); //������ؼ��ּ�������
        }
        Pop(S, p); //��ָ����ջ
        if (S.base != S.top)
        {                       //��Ϊ��ջʱ
            Pop(S, p);          //pָ����һ����㣬�Ҹ�ָ����ջ
            Push(S, p->rchild); //������һ��
        }
    }
    return OK;
}
//12.�������
status InOrderTraverse(pBiTree T)
{
    if (T == NULL)
        return OK;
    InOrderTraverse(T->lchild);         //�������������
    printf("%c(%d) ", T->tag, T->data); //������Ĺؼ��ּ�������
    InOrderTraverse(T->rchild);         //�������������
    return OK;
}
//13.�������
status PostOrderTraverse(pBiTree T)
{
    if (T == NULL)
        return OK;
    PostOrderTraverse(T->lchild);       //�������������
    PostOrderTraverse(T->rchild);       //�������������
    printf("%c(%d) ", T->tag, T->data); //������Ĺؼ��ּ�������
    return OK;
}
//14.�������
status LevelOrderTraverse(pBiTree T)
{
    SElemType *p;
    SqStack S;
    InitStack(S); //������ջ
    Push(S, T);   //ָ�������ָ���ջ
    for (p = S.base; p != S.top; p++)
    {
        printf("%c(%d) ", (*p)->tag, (*p)->data); //������ؼ��ּ�������
        if ((*p)->lchild)
            Push(S, (*p)->lchild); //ָ�����ӵ�ָ����ջ
        if ((*p)->rchild)
            Push(S, (*p)->rchild); //ָ���Һ��ӵ�ָ����ջ
    }
    return OK;
}
//15.��ӡ������
status PrintBiTree(pBiTree T)
{
    int i, j, m, n;
    SElemType *p;
    SqStack S;
    InitStack(S); //������ջ
    Push(S, T);   //ָ�������ָ���ջ
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
            printf("%c", (*p)->tag); //������ؼ���
            Push(S, (*p)->lchild);   //ָ�����ӵ�ָ����ջ
            Push(S, (*p)->rchild);   //ָ���Һ��ӵ�ָ����ջ
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
//16.���������
status SaveBiTree(pBiTree T, char filename[])
{
    FILE *fp; //�ļ�ָ��
    if (T == NULL)
        return OVERFLOW;
    if ((fp = fopen(filename, "wb")) == NULL)
    {                               //��ֻд��ʽ���ļ�
        printf("�ļ��򿪴���\n"); // ָ��Ϊ�գ�����
        return ERROR;
    }
    SElemType p;
    TagType ch = '#'; //��ʾ�ս��
    SqStack S;
    InitStack(S);       //������ջ
    Push(S, T->lchild); //ָ�������ָ���ջ
    while (S.base != S.top)
    { //��Ϊ��ջʱѭ��
        while (GetTop(S, p) && p)
        {                       //�����ߵ���ͷ
            Push(S, p->lchild); //·����ָ�������ָ�����ν�ջ
            if (fwrite(&(p->tag), sizeof(TagType), 1, fp) != 1)
            {                               //д��ؼ���
                printf("�ļ�д�����\n"); //����ֵ��Ϊ1��д�����
                return ERROR;
            }
            if (fwrite(&(p->data), sizeof(ElemType), 1, fp) != 1)
            {                               //д��������
                printf("�ļ�д�����\n"); //����ֵ��Ϊ1��д�����
                return ERROR;
            }
        }
        //ָ��Ϊ�գ������ַ�'#'����ս��
        if (fwrite(&ch, sizeof(TagType), 1, fp) != 1)
        {
            printf("�ļ�д�����\n"); //����ֵ��Ϊ1��д�����
            return ERROR;
        }
        Pop(S, p); //��ָ����ջ
        if (S.base != S.top)
        {                       //��Ϊ��ջʱ
            Pop(S, p);          //pָ����һ����㣬�Ҹ�ָ����ջ
            Push(S, p->rchild); //������һ��
        }
    }
    if (fwrite(&ch, sizeof(TagType), 1, fp) != 1)
    {
        //�����һ���ַ�'#'�������ʱ����
        printf("�ļ�д�����\n"); //��'#'��ʾͷ�����Һ���Ϊ��
        return ERROR;
    }
    fclose(fp); //�ر��ļ�
    return OK;
}
//17.���ض�����
status GetBiTree(pBiTree &T, char filename[])
{
    FILE *fp;   //�ļ�ָ��
    TagType ch; //�洢�ؼ���
    if (T)
        DestroyBiTree(T);                //������Ѵ��ڣ����ٶ�����
    T = (pBiTree)malloc(sizeof(BiTree)); //����ͷ���
    T->lchild = T->rchild = NULL;
    if ((fp = fopen(filename, "rb")) == NULL)
    {                               //��ֻ����ʽ���ļ�
        printf("�ļ��򿪴���\n"); //ָ��Ϊ�գ�����
        return ERROR;
    }
    SElemType p, q;
    SqStack S;
    InitStack(S); //������ջ
    Push(S, T);   //ָ��ͷ����ָ���ջ
    while (S.base != S.top)
    { //��Ϊ��ջʱѭ��
        //�����ߵ���ͷ
        while (1)
        {
            if (fread(&ch, sizeof(TagType), 1, fp) != 1)
            {
                printf("�ļ���ȡ����\n"); //����ֵ��Ϊ1��д�����
                return ERROR;
            }
            GetTop(S, p);
            if (ch == '#')
            { //����#�������ӣ��˳�ѭ��
                p->lchild = NULL;
                break;
            }
            else
            {
                q = (pBiTree)malloc(sizeof(BiTree)); //�����½�㴢������
                q->tag = ch;                         //�ؼ��ָ�ֵ
                if (fread(&(q->data), sizeof(ElemType), 1, fp) != 1)
                { //������ֵ
                    printf("�ļ���ȡ����\n");
                    return ERROR;
                }
                p->lchild = q; //��ָ�����ӵ�ָ��ָ���½��
                Push(S, q);    //ָ�����ӵ�ָ���ջ
            }
        }
        //����һ��
        while (1)
        {
            if (fread(&ch, sizeof(TagType), 1, fp) != 1)
            {
                printf("�ļ���ȡ����\n"); //����ֵ��Ϊ1��д�����
                return ERROR;
            }
            GetTop(S, p);
            if (ch == '#')
            { //����#�����Һ���
                p->rchild = NULL;
                Pop(S, p); //ջ��Ԫ�س�ջ����ָ��˫�׽���ָ���ջ
                if (S.base == S.top)
                    break; //ջ��ʱ�˳�ѭ��
            }
            else
            {
                q = (pBiTree)malloc(sizeof(BiTree)); //�����½�㴢������
                q->tag = ch;                         //�ؼ��ָ�ֵ
                if (fread(&(q->data), sizeof(ElemType), 1, fp) != 1)
                { //������ֵ
                    printf("�ļ���ȡ����\n");
                    return ERROR;
                }
                p->rchild = q; //��ָ���Һ��ӵ�ָ��ָ���½��
                Pop(S, p);     //ջ��Ԫ�س�ջ����ָ��˫�׽���ָ���ջ
                Push(S, q);    //ָ���Һ��ӵ�ָ���ջ
                break;
            }
        }
    }
    fclose(fp); //�ر��ļ�
    return OK;
}
//18.���Ķ�����
status ChangeBiTree(int &t)
{
    printf(" ������Ҫ�Եڼ������������в��� [1~100]\n");
    scanf("%d", &t);
    if (t > 0 && t < 101)
        return OK; //���Ϸ�ΧҪ��
    else
        return ERROR; //������Χ
}

/*--------------������������---------------*/
//1.���������нϴ�ֵ
status MAX(status a, status b)
{
    if (a > b)
        return a; //a�ϴ��򷵻�a
    else
        return b; //���򷵻�b
}
//2.������ջ
status InitStack(SqStack &S)
{
    S.base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if (!S.base)
        exit(OVERFLOW);            //�洢�ռ����ʧ��
    S.top = S.base;                //ջ��ָ�����ջ��ָ�룬����ջ
    S.stacksize = STACK_INIT_SIZE; //���´洢�ռ�
    return OK;
}
//3.����ջ��Ԫ��
status GetTop(SqStack S, SElemType &p)
{
    if (S.base == S.top)
        return ERROR; //ջΪ�գ���Ԫ��
    p = *(S.top - 1); //����ջ��Ԫ��ֵ
    return OK;
}
//4.����ջ��Ԫ��
status Push(SqStack &S, SElemType p)
{
    if (S.top - S.base >= S.stacksize)
    { //ջ�洢�ռ�����
        S.base = (SElemType *)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!S.base)
            exit(OVERFLOW);            //�洢�ռ����ʧ��
        S.top = S.base + S.stacksize;  //����ջ��ָ��
        S.stacksize += STACKINCREMENT; //���´洢�ռ�
    }
    *S.top = p; //��Ԫ�ز���ջ��
    S.top++;    //ջ��ָ������
    return OK;
}
//5.ɾ��ջ��Ԫ��
status Pop(SqStack &S, SElemType &p)
{
    if (S.base == S.top)
        return ERROR; //ջΪ�գ���ջ��Ԫ��
    S.top--;          //ջ��ָ���Լ�
    p = *S.top;       //����ջ��Ԫ�ص�ֵ
    return OK;
}
//6.����˫�׽��
pBiTree Parent(pBiTree T, pBiTree c)
{
    SElemType p = NULL;
    SqStack S;
    InitStack(S); //������ջ
    Push(S, T);   //ָ�������ָ���ջ
    while (S.base != S.top)
    { //��Ϊ��ջʱѭ��
        while (GetTop(S, p) && p)
        {                       //�����ߵ���ͷ
            Push(S, p->lchild); //·����ָ�������ָ�����ν�ջ
            if (p->lchild == c || p->rchild == c)
                return p; //�ҵ�˫�׽��
        }
        Pop(S, p); //��ָ����ջ
        if (S.base != S.top)
        {                       //��Ϊ��ջʱ
            Pop(S, p);          //pָ����һ����㣬�Ҹ�ָ����ջ
            Push(S, p->rchild); //������һ��
        }
    }
    return NULL;
}
//7.�������ҽ���ָ��
pBiTree RightNode(pBiTree T)
{
    if (T->rchild == NULL)
        return T;
    SElemType p, q;
    SqStack S;
    InitStack(S); //������ջ
    Push(S, T);   //ָ�������ָ���ջ
    while (S.base != S.top)
    { //��Ϊ��ջʱѭ��
        while (GetTop(S, p) && p)
        {                       //�����ߵ���ͷ
            Push(S, p->lchild); //·����ָ�������ָ�����ν�ջ
        }
        Pop(S, p); //��ָ����ջ
        if (S.base != S.top)
        {                       //��Ϊ��ջʱ
            Pop(S, p);          //pָ����һ����㣬�Ҹ�ָ����ջ
            Push(S, p->rchild); //������һ��
            if (p->rchild == NULL)
                q = p;
        }
    }
    return q;
}
//8.����a��b�η���ֵ
status pow(status a, status b)
{
    status i, num;
    for (i = 0, num = 1; i < b; i++)
    {
        num = num * a;
    }
    return num;
}