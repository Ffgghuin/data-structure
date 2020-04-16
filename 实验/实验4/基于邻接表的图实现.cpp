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
#define overflow -3

/*----- ����Ԫ�����Ͷ��� -----*/
typedef int status;   //����ֵ����
typedef int ElemType; //����������
typedef char TagType; //�ؼ�������

/*----- ͼ�Ĵ洢��ʾ -----*/
typedef struct Arc
{                   //���Ķ���
    TagType adjvex; //�ڽӵ���
    Arc *nextarc;   //����ָ����һ�������ڸö���Ļ�
    int info;       //��־��
} Arc, *pArc;
typedef struct Vex
{                  //����Ķ���
    TagType tag;   //�ؼ���
    ElemType data; //������
    Arc *firstArc; //����ָ���һ�������ڸö���Ļ�
} Vex, *pVex;
typedef struct Graph
{               //ͼ�Ķ���
    Vex V[100]; //����ṹ����
    int Vexnum; //�������
    int Arcnum; //������
} Digraph;

/*----- ջ��˳��洢��ʾ -----*/
#define STACK_INIT_SIZE 100 //�洢�ռ��ʼ������
#define STACKINCREMENT 10   //��������

typedef pVex SElemType;
typedef struct
{                    //ջ�Ķ���
    SElemType *base; //ջ��ָ��
    SElemType *top;  //ջ��ָ��
    int stacksize;   //�ѷ���洢�ռ�
} SqStack;

/*------ �������� ------*/
status CreateGraph(Digraph &G);                              //����ͼ
status DestroyGraph(Digraph &G);                             //����ͼ
status LocateVex(Digraph G, TagType e);                      //���Ҷ���
status PutVex(Digraph &G, TagType e, ElemType value);        //���㸳ֵ
status FirstAdjVex(Digraph G, TagType v, pVex &u);           //��õ�һ�ڽӵ�
status NextAdjVex(Digraph G, TagType v, TagType w, pVex &u); //�����һ�ڽӵ�
status InsertVex(Digraph &G, pVex &u);                       //���붥��
status DeleteVex(Digraph &G, TagType v);                     //ɾ������
status InsertArc(Digraph &G, TagType v, TagType w);          //���뻡
status DeleteArc(Digraph &G, TagType v, TagType w);          //ɾ����
status DFSTraverse(Digraph G, TagType v);                    //���������������
status BFSTraverse(Digraph G, TagType v, int visit[]);       //���������������
status SaveGraph(Digraph G, char filename[]);                //����ͼ
status GetGraph(Digraph &G, char filename[]);                //����ͼ
status ChangeGraph(int &t);                                  //�ı������ͼ
status PrintGraph(Digraph G);                                //���ͼ���ڽӱ�

/*------ ������������ ------*/
status DFS(Digraph G, TagType v, int *p, int i); //��������������������ݹ麯��
status InitStack(SqStack &S);                    //������ջ
status Push(SqStack &S, SElemType p);            //����ջ��Ԫ��

/*----main����----*/
int main(void)
{
    Digraph G[100];     //�ṹ����
    char filename[100]; //�洢�ļ����ַ���
    ElemType value;     //�洢������ֵ
    TagType e, v, w;    //�洢�ؼ���ֵ
    status j;           //�洢��������ֵ
    pVex u;
    Vex c;
    pArc p;
    int i, t = 0;
    for (i = 0; i < 100; i++)
    { //��ʼ��
        G[i].Arcnum = 0;
        G[i].Vexnum = 0;
    }
    do
    {
        printf(" ������Ҫ�Եڼ���ͼ���в��� [1~100]\n");
        scanf("%d", &t);
        getchar();
    } while (t < 1 || t > 100);
    system("cls"); //����
    int op = 1;
    while (op)
    {
        system("cls");
        printf("\n\n");
        printf("      ���ڶԵ�%d��ͼ���в���    \n", t);
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
        printf("      ��ѡ����Ĳ���[0~16]:");
        scanf("%d", &op);
        getchar();
        switch (op)
        {
        case 1:
            //CreateGraph
            if (G[t - 1].Vexnum != 0)
                printf("ͼ�Ѵ��ڣ�����ʧ�ܣ�\n");
            else
            {
                Digraph g = G[t - 1];
                j = CreateGraph(g);
                if (j == OK)
                {
                    G[t - 1] = g;
                    printf("ͼ�����ɹ���\n");
                }
                else if (j == ERROR)
                    printf("����ؼ����ظ���ͼ����ʧ�ܣ�\n");
                else if (j == INFEASTABLE)
                    printf("�����ӵ�ĳ���㲻��ͼ�У�ͼ����ʧ�ܣ�\n");
                else if (j == OVERFLOW)
                    printf("��ϵ�������Ի���ͼ����ʧ�ܣ�\n");
                else if (j == overflow)
                    printf("��ϵ���ظ���ͼ����ʧ�ܣ�\n");
            }
            getchar();
            getchar();
            break;
        case 2:
            //DestroyGraph
            if (G[t - 1].Vexnum == 0)
                printf("ͼ�����ڣ�����ʧ�ܣ�\n");
            else
            {
                j = DestroyGraph(G[t - 1]);
                if (j == OK)
                    printf("ͼ���ٳɹ���\n");
                else
                    printf("ͼ����ʧ�ܣ�\n");
            }
            getchar();
            getchar();
            break;
        case 3:
            //LocateVex
            if (G[t - 1].Vexnum == 0)
                printf("ͼ�����ڣ�\n");
            else
            {
                printf("��������Ҫ���Ҷ���Ĺؼ��֣�\n");
                scanf("%c", &e);
                getchar();
                j = LocateVex(G[t - 1], e);
                if (j)
                    printf("�����ҵĶ���Ϊͼ�е�%d�����㣬��%c(%d)��\n", j, G[t - 1].V[j - 1].tag, G[t - 1].V[j - 1].data);
                else
                    printf("�����ҵĶ��㲻��ͼ�У�\n");
            }
            getchar();
            getchar();
            break;
        case 4:
            //PutVex
            if (G[t - 1].Vexnum == 0)
                printf("ͼ�����ڣ�\n");
            else
            {
                printf("��������Ҫ��ֵ����Ĺؼ��֣�\n");
                scanf("%c", &e);
                getchar();
                printf("��������Ҫ����ֵ��\n");
                scanf("%d", &value);
                getchar();
                j = PutVex(G[t - 1], e, value);
                if (j)
                    printf("����%c��ֵ�ɹ���\n", e);
                else
                    printf("����ֵ�Ķ��㲻��ͼ�У�\n");
            }
            getchar();
            getchar();
            break;
        case 5:
            //FirstAdjVex
            if (G[t - 1].Vexnum == 0)
                printf("ͼ�����ڣ�\n");
            else
            {
                printf("��������Ҫ���ҵ�һ�ڽӵ�Ķ���Ĺؼ��֣�\n");
                scanf("%c", &v);
                getchar();
                j = FirstAdjVex(G[t - 1], v, u);
                if (j)
                {
                    if (u)
                        printf("�����Ҷ���%c�ĵ�һ�ڽӵ�Ϊ%c��\n", v, u->tag);
                    else
                        printf("�����Ҷ������ڽӵ㣡\n");
                }
                else
                    printf("�����ҵĶ��㲻��ͼ�У�\n");
            }
            getchar();
            getchar();
            break;
        case 6:
            //NextAdjVex
            if (G[t - 1].Vexnum == 0)
                printf("ͼ�����ڣ�\n");
            else
            {
                printf("��������Ҫ�����ڽӵ�Ķ���Ĺؼ��֣�\n");
                scanf("%c", &v);
                getchar();
                printf("��������Ҫ������һ�ڽӵ���ڽӵ�Ĺؼ��֣�\n");
                scanf("%c", &w);
                getchar();
                j = NextAdjVex(G[t - 1], v, w, u);
                if (j == OK)
                {
                    if (u)
                        printf("����%c����ڶ���%c����һ�ڽӵ�Ϊ%c��\n", v, w, u->tag);
                    else
                        printf("����%c�����һ���ڽӵ���%c��\n", v, w);
                }
                else if (j == ERROR)
                    printf("����%c���Ƕ���%c���ڽӵ㣡\n", w, v);
                else if (j == INFEASTABLE)
                    printf("����%c����ͼ�У�\n", w);
                else
                    printf("����%c����ͼ�У�\n", v);
            }
            getchar();
            getchar();
            break;
        case 7:
            //InsertVex
            if (G[t - 1].Vexnum == 0)
                printf("ͼ�����ڣ�\n");
            else
            {
                printf("��������붥��Ĺؼ��ּ�������\n");
                scanf("%c", &c.tag);
                getchar();
                scanf("%d", &c.data);
                getchar();
                c.firstArc = NULL;
                u = &c;
                j = InsertVex(G[t - 1], u);
                if (j == OK)
                    printf("����%c����ɹ���\n", c.tag);
                else if (j == ERROR)
                    printf("�ڽӵ����ͼ�в����ڸö��㣡\n");
                else
                    printf("������󣬹ؼ�����ͼ���Ѵ��ڣ�\n");
            }
            getchar();
            getchar();
            break;
        case 8:
            //DeleteVex
            if (G[t - 1].Vexnum == 0)
                printf("ͼ�����ڣ�\n");
            else
            {
                printf("������ɾ������Ĺؼ��֣�\n");
                scanf("%c", &v);
                getchar();
                j = DeleteVex(G[t - 1], v);
                if (j == OK)
                    printf("����%cɾ���ɹ���\n", v);
                else if (j == ERROR)
                    printf("����ɾ��ʧ�ܣ�ͼ�в����ڸö��㣡\n");
            }
            getchar();
            getchar();
            break;
        case 9:
            //InsertArc
            if (G[t - 1].Vexnum == 0)
                printf("ͼ�����ڣ�\n");
            else
            {
                printf("��������뻡��\n");
                scanf("%c", &v);
                getchar();
                scanf("%c", &w);
                getchar();
                j = InsertArc(G[t - 1], v, w);
                if (j == OK)
                    printf("������ɹ���\n", v);
                else if (j == ERROR)
                    printf("������ʧ�ܣ�ͼ���Ѵ��ڸû���\n");
                else
                    printf("������ʧ�ܣ�ͼ�в����ڻ���ĳ���㣡\n");
            }
            getchar();
            getchar();
            break;
        case 10:
            //DeleteArc
            if (G[t - 1].Vexnum == 0)
                printf("ͼ�����ڣ�\n");
            else
            {
                printf("������ɾ������\n");
                scanf("%c", &v);
                getchar();
                scanf("%c", &w);
                getchar();
                j = DeleteArc(G[t - 1], v, w);
                if (j == OK)
                    printf("��ɾ���ɹ���\n", v);
                else if (j == ERROR)
                    printf("��ɾ��ʧ�ܣ�ͼ�в����ڸû���\n");
                else
                    printf("��ɾ��ʧ�ܣ�ͼ�в����ڻ���ĳ���㣡\n");
            }
            getchar();
            getchar();
            break;
        case 11:
            //DFSTraverse
            if (G[t - 1].Vexnum == 0)
                printf("ͼ�����ڣ�\n");
            else
            {
                printf("������������������Ŀ�ʼ����ؼ��֣�\n");
                scanf("%c", &v);
                getchar();
                if (DFSTraverse(G[t - 1], v) == ERROR)
                    printf("ͼ�в����ڸö��㣡\n");
            }
            getchar();
            getchar();
            break;
        case 12:
            //BFSTraverse
            if (G[t - 1].Vexnum == 0)
                printf("ͼ�����ڣ�\n");
            else
            {
                int visited[G[t-1].Vexnum]; //���ұ�־����
                for (i = 0; i < G[t - 1].Vexnum; i++)
                    visited[i] = 0;
                printf("�����������������Ŀ�ʼ����ؼ��֣�\n");
                scanf("%c", &v);
                getchar();
                if (BFSTraverse(G[t - 1], v, visited) == ERROR)
                    printf("ͼ�в����ڸö��㣡\n");
            }
            getchar();
            getchar();
            break;
        case 13:
            //SaveGraph
            printf("��������Ҫ������ļ�����\n");
            scanf("%s", filename);
            j = SaveGraph(G[t - 1], filename);
            if (j == OK)
                printf("�ļ�����ɹ���\n");
            else if (j == OVERFLOW)
                printf("ͼ�����ڣ��ļ�����ʧ�ܣ�\n");
            else
                printf("�ļ�����ʧ�ܣ�\n");
            getchar();
            getchar();
            break;
        case 14:
            //GetGraph
            printf("��������Ҫ���ص��ļ�����\n");
            scanf("%s", filename);
            j = GetGraph(G[t - 1], filename);
            if (j == OK)
                printf("�ļ����سɹ���\n");
            else
                printf("�ļ�����ʧ�ܣ�\n");
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
                printf("ͼ�ѳɹ��ı�����%d����\n", t);
            }
            else
                printf("ͼ�ı�ʧ�ܣ�\n");
            getchar();
            getchar();
            break;
        case 16:
            //PrintGraph
            if (G[t - 1].Vexnum == 0)
                printf("ͼ�����ڣ�\n");
            else
                PrintGraph(G[t - 1]);
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
//1.����ͼ
status CreateGraph(Digraph &G)
{
    Arc *p, *q;
    int i, j, l;
    TagType ch1, ch2;
    //����ͼ���㼯�Ĺ���
    printf("��������Ҫ����ͼ�Ķ�����(1-100)��\n");
    scanf("%d", &G.Vexnum);
    getchar();
    printf("�����붥�㼯��\n");
    for (i = 0; i < G.Vexnum; i++)
    {                           //���붥����Ϣ
        G.V[i].firstArc = NULL; //ָ�����ʼ��
        scanf("%c", &G.V[i].tag);
        getchar();
        scanf("%d", &G.V[i].data);
        getchar();
    }
    //���ͼ���㼯�Ƿ����Ҫ��
    for (i = 0; i < G.Vexnum; i++)
    {
        ch1 = G.V[i].tag;
        for (j = i + 1; j < G.Vexnum; j++)
        {
            ch2 = G.V[j].tag;
            if (ch1 == ch2)
                return ERROR; //����ؼ����ظ�
        }
    }
    //����ͼ��ϵ���Ĺ���
    printf("��������Ҫ����ͼ�Ļ�����\n");
    scanf("%d", &G.Arcnum);
    getchar();
    printf("�������ϵ����\n");
    for (i = 0; i < G.Arcnum; i++)
    { //���뻡��Ϣ
        scanf("%c", &ch1);
        getchar();
        scanf("%c", &ch2);
        getchar();
        for (j = 0; j < G.Vexnum; j++) //���ҹؼ���Ϊch1�Ķ���
            if (G.V[j].tag == ch1)
                break;
        if (j == G.Vexnum)
            return INFEASTABLE;        //����ch1����ͼ��
        for (l = 0; l < G.Vexnum; l++) //���ҹؼ���Ϊch1�Ķ���
            if (G.V[l].tag == ch2)
                break;
        if (l == G.Vexnum)
            return INFEASTABLE;        //����ch2����ͼ��
        p = (pArc)malloc(sizeof(Arc)); //�����½��
        p->adjvex = ch2;
        p->nextarc = NULL;
        if (G.V[j].firstArc == NULL)
            G.V[j].firstArc = p;
        else
        {
            for (q = G.V[j].firstArc; q->nextarc != NULL; q = q->nextarc)
                ;
            q->nextarc = p; //�½����������β��
        }
    }
    //����ϵ���Ƿ����Ҫ��
    for (i = 0; i < G.Vexnum; i++)
    {
        p = G.V[i].firstArc;
        if (p == NULL)
            continue;
        if (p->adjvex == G.V[i].tag)
            return OVERFLOW; //���Ի�
        if (p->nextarc == NULL)
            continue;
        q = p;
        p = p->nextarc;
        while (p != NULL)
        {
            if (q->adjvex == G.V[i].tag)
                return OVERFLOW; //���Ի�
            for (; p->nextarc != NULL; p = p->nextarc)
            {
                if (q->adjvex == p->adjvex)
                    return overflow; //���ظ�
            }
            if (q->adjvex == p->adjvex)
                return overflow; //���ظ�
            q = q->nextarc;
            p = q->nextarc;
        }
        if (q->adjvex == G.V[i].tag)
            return OVERFLOW; //���Ի�
    }
    return OK;
}
//2.����ͼ
status DestroyGraph(Digraph &G)
{
    Arc *p, *q;
    int i;
    for (i = 0; i < G.Vexnum; i++)
    {
        G.V[i].data = G.V[i].tag = 0; //���ùؼ��ּ�������
        if (G.V[i].firstArc != NULL)
        {
            p = G.V[i].firstArc;
            while (p->nextarc != NULL)
            { //��������
                q = p;
                p = p->nextarc;
                free(q);
            }
            free(p);
        }
    }
    G.Arcnum = G.Vexnum = 0; //����ͼ�������ͻ���
    return OK;
}
//3.���Ҷ���
status LocateVex(Digraph G, TagType e)
{
    int i;
    for (i = 0; i < G.Vexnum; i++)
    {
        if (G.V[i].tag == e)
            return i + 1; //���������Ҷ����ǵڼ���
    }
    return 0;
}
//4.���㸳ֵ
status PutVex(Digraph &G, TagType e, ElemType value)
{
    status i = LocateVex(G, e); //���Ҷ���e
    if (i)
    {
        G.V[i - 1].data = value; //������ֵ
        return OK;
    }
    else
        return ERROR; //����e����ͼ��
}
//5.��õ�һ�ڽӵ�
status FirstAdjVex(Digraph G, TagType v, pVex &u)
{
    Arc *p;
    status i, j;
    i = LocateVex(G, v); //���Ҷ���v
    if (i)
    {
        p = G.V[i - 1].firstArc;
        if (!p)
            u = NULL; //���ڽӵ�
        else
        {
            j = LocateVex(G, p->adjvex);
            u = &G.V[j - 1]; //�����ڽӵ�ָ��
        }
        return OK;
    }
    else
        return ERROR; //����v����ͼ��
}
//6.�����һ�ڽӵ�
status NextAdjVex(Digraph G, TagType v, TagType w, pVex &u)
{
    pArc p, q;
    status i, k, j = ERROR;
    i = LocateVex(G, v);
    if (!i)
        return OVERFLOW; //����v����ͼ��
    k = LocateVex(G, w);
    if (!k)
        return INFEASTABLE; //����w����ͼ��
    p = G.V[i - 1].firstArc;
    if (!p)
        return ERROR; //���ڽӵ�
    else
    {
        while (p->nextarc != NULL)
        {
            if (p->adjvex == w)
            { //�ҵ�w�������е�λ��
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
        { //�ҵ�w
            if (!q)
                u = NULL; //w������β��
            else
            {
                j = LocateVex(G, q->adjvex);
                u = &G.V[j - 1]; //������һ�ڽӵ��ָ��
            }
            return OK;
        }
        else
            return ERROR; //w����v���ڽӵ�
    }
}
//7.���붥��
status InsertVex(Digraph &G, pVex &u)
{
    pArc p, q;
    TagType ch;
    int i, j, num;
    j = LocateVex(G, u->tag);
    if (j)
        return OVERFLOW; //����ؼ����ظ�
    printf("�������������ڽӵ������\n");
    scanf("%d", &num);
    getchar();
    for (i = 0; i < num; i++)
    {
        printf("�����������������ڽӵ�ؼ��֣�\n");
        scanf("%c", &ch);
        getchar();
        j = LocateVex(G, ch);
        if (j == 0)
            return ERROR;              //�ڽӵ㲻��ͼ��
        p = (pArc)malloc(sizeof(Arc)); //�����½��
        p->adjvex = ch;
        p->nextarc = NULL;
        if (u->firstArc == NULL)
            u->firstArc = p; //�½����Ϊ��һ�����
        else
        {
            for (q = u->firstArc; q->nextarc != NULL; q = q->nextarc)
                ;
            q->nextarc = p; //�ǵ�һ�����
        }
    }
    G.V[G.Vexnum] = *u; //��������붥�㼯��
    G.Vexnum++;         //����ͼ������
    G.Arcnum += num;    //����ͼ����
    return OK;
}
//8.ɾ������
status DeleteVex(Digraph &G, TagType v)
{
    pArc p, q;
    status ret;
    int i, j, num = 0;
    j = LocateVex(G, v);
    if (j == 0)
        return ERROR; //����v����ͼ��
    p = G.V[j - 1].firstArc;
    if (p)
    { //ɾ���Զ���vΪ��β�Ļ�
        while (p->nextarc != NULL)
        {
            q = p;
            p = p->nextarc;
            num++; //��ɾ���Ļ�����
            free(q);
        }
        num++; //��ɾ���Ļ�����
        free(p);
        G.V[j - 1].firstArc = NULL; //��ʼ��
    }
    G.V[j - 1].data = G.V[-1].tag = 0; //��ʼ��
    for (i = 0; i < G.Vexnum; i++)
    { //ɾ���Զ���vΪ��ͷ�Ļ�
        q = p = G.V[i].firstArc;
        if (!p)
            continue;                              //���ڽӵ㣬��������ѭ��
        for (; p->nextarc != NULL; p = p->nextarc) //��������Ѱ���ڽӵ�v
            if (p->adjvex == v)
                break;
        if (p->nextarc == NULL && p->adjvex != v)
            continue;
        //û���ҵ�����������ѭ��
        if (p == G.V[i].firstArc)
        { //��Ϊ��һ����㣬ֱ��ɾ��
            G.V[i].firstArc = p->nextarc;
            free(p);
            num++;    //��ɾ���Ļ�����
            continue; //��������ѭ��
        }
        else
        {
            for (; q->nextarc != NULL; q = q->nextarc)
                //��������Ѱ�Ҷ���vǰһ�����
                if (q->nextarc == p)
                    break;
            q->nextarc = p->nextarc;
            free(p);
            num++;    //��ɾ���Ļ�����
            continue; //��������ѭ��
        }
    }
    for (; j < G.Vexnum; j++)
        G.V[j - 1] = G.V[j]; //���㼯�к�����������ǰ��
    G.Vexnum--;              //���¶�����
    G.Arcnum -= num;         //���»���
    return OK;
}
//9.���뻡
status InsertArc(Digraph &G, TagType v, TagType w)
{
    pArc p, q;
    int i, j;
    i = LocateVex(G, v);
    if (i == 0)
        return OVERFLOW; //�����ڶ���v
    j = LocateVex(G, w);
    if (j == 0)
        return OVERFLOW; //�����ڶ���w
    p = G.V[i - 1].firstArc;
    if (!p)
    {                                  //���ڽӵ�
        q = (pArc)malloc(sizeof(Arc)); //���뻡v,w
        q->adjvex = w;
        q->nextarc = NULL;
        G.V[i - 1].firstArc = q;
        G.Arcnum++; //���»���
        return OK;
    }
    else
    {
        for (; p->nextarc != NULL; p = p->nextarc) //���һ�v,w
            if (p->adjvex == w)
                break;
        if (p->nextarc == NULL && p->adjvex != w)
        {                                  //v,w��û�л�
            q = (pArc)malloc(sizeof(Arc)); //���뻡v,w
            q->adjvex = w;
            q->nextarc = NULL;
            p->nextarc = q;
            G.Arcnum++; //���»���
            return OK;
        }
        else
            return ERROR; //v,w���Ѵ��ڻ�
    }
}
//10.ɾ����
status DeleteArc(Digraph &G, TagType v, TagType w)
{
    pArc p, q;
    int i, j, num = 0;
    i = LocateVex(G, v);
    if (i == 0)
        return OVERFLOW; //�����ڶ���v
    j = LocateVex(G, w);
    if (j == 0)
        return OVERFLOW; //�����ڶ���w
    q = p = G.V[i - 1].firstArc;
    if (!p)
        return ERROR;                          //���ڽӵ�
    for (; p->nextarc != NULL; p = p->nextarc) //���һ�v,w
        if (p->adjvex == w)
            break;
    if (p->nextarc == NULL && p->adjvex != w)
        return ERROR; //v,w��û�л�
    else
    { //ɾ��v��w��Ļ�
        if (q == p)
            G.V[i - 1].firstArc = p->nextarc; //ɾ����һ�����
        else
        { //ɾ���ǵ�һ�����
            for (; q->nextarc != NULL; q = q->nextarc)
                if (q->nextarc == p)
                    break;
            q->nextarc = p->nextarc;
        }
        free(p);
        G.Arcnum--; //���»���
        return OK;
    }
}
//11.���������������
status DFSTraverse(Digraph G, TagType v)
{
    int visited[G.Vexnum]; //���ұ�־����
    TagType w;
    int *p = visited;
    status ret; //�洢��������ֵ
    pVex u;     //�洢��������ָ��
    int i, j;
    for (i = 0; i < G.Vexnum; i++)
        visited[i] = 0; //��ʼ��
    j = LocateVex(G, v);
    if (j == 0)
        return ERROR; //����v����ͼ��
    printf("ͼ�����������������Ϊ��\n");
    if (!visited[j - 1])
        DFS(G, G.V[j - 1].tag, visited, j - 1);
    for (i = 0; i < G.Vexnum; i++) //�����Ƿ��ж���δ������
        if (!visited[i])           //���������������������
            DFS(G, G.V[i].tag, visited, i);
    return OK;
}
//12.���������������
status BFSTraverse(Digraph G, TagType v, int *visited)
{
    TagType w, x;
    status ret; //�洢��������ֵ
    pVex u;     //�洢��������ָ��
    int i, j;
    j = LocateVex(G, v);
    if (j == 0)
        return ERROR; //����v����ͼ��
    SElemType *q;
    SqStack S;
    InitStack(S);         //������ջ
    Push(S, &G.V[j - 1]); //ָ�򶥵��ָ���ջ
    printf("ͼ�Ĺ��������������Ϊ��\n");
    for (q = S.base; q != S.top; q++)
    {
        w = (*q)->tag;
        j = LocateVex(G, w);
        if (!visited[j - 1])
        {                       //δ������
            visited[j - 1] = 1; //�޸Ĳ��ұ�־
            printf("%c(%d) ", G.V[j - 1].tag, G.V[j - 1].data);
            ret = FirstAdjVex(G, w, u);
            for (; ret > 0; ret = NextAdjVex(G, w, x, u))
            { //���β����ڽӵ�
                if (u)
                {                             //�ҵ��ڽӵ�
                    j = LocateVex(G, u->tag); //��λ����
                    if (!visited[j - 1])
                        Push(S, &G.V[j - 1]); //�¶����ջ
                    x = u->tag;
                }
                else
                    break; //�ڽӵ��Ѳ����꣬�˳�ѭ��
            }
        }
    }
    for (i = 0; i < G.Vexnum; i++) //�����Ƿ��ж���δ������
        if (!visited[i])           //���������������������
            BFSTraverse(G, G.V[i].tag, visited);
    return OK;
}
//13.����ͼ
status SaveGraph(Digraph G, char filename[])
{
    FILE *fp; //�ļ�ָ��
    pArc p = NULL;
    char ch = '#';
    int i;
    if (G.Vexnum == 0)
        return OVERFLOW;
    if ((fp = fopen(filename, "wb")) == NULL)
    {                               //��ֻд��ʽ���ļ�
        printf("�ļ��򿪴���\n"); // ָ��Ϊ�գ�����
        return ERROR;
    }
    if (fwrite(&G.Vexnum, sizeof(int), 1, fp) != 1)
    { //���涥����
        printf("�ļ�д�����\n");
        return ERROR;
    }
    if (fwrite(&G.Arcnum, sizeof(int), 1, fp) != 1)
    { //���满��
        printf("�ļ�д�����\n");
        return ERROR;
    }
    for (i = 0; i < G.Vexnum; i++)
    { //���α���ÿ������ؼ��֣��������ڽӵ�
        if (fwrite(&G.V[i].tag, sizeof(TagType), 1, fp) != 1)
        { //���涥��ؼ���
            printf("�ļ�д�����\n");
            return ERROR;
        }
        if (fwrite(&G.V[i].data, sizeof(ElemType), 1, fp) != 1)
        { //���涥��������
            printf("�ļ�д�����\n");
            return ERROR;
        }
        if (G.V[i].firstArc == NULL)
        { //���ڽӱ�
            if (fwrite(&ch, sizeof(TagType), 1, fp) != 1)
            {
                //�����ַ�'#'��ʾ�ڽӱ�Ϊ��
                printf("�ļ�д�����\n");
                return ERROR;
            }
        }
        else
        { //���ڽӱ�
            p = G.V[i].firstArc;
            for (; p->nextarc != NULL; p = p->nextarc)
            {
                if (fwrite(&p->adjvex, sizeof(TagType), 1, fp) != 1)
                {
                    //����ÿ���ڽӵ�
                    printf("�ļ�д�����\n");
                    return ERROR;
                }
            }
            if (fwrite(&p->adjvex, sizeof(TagType), 1, fp) != 1)
            {
                printf("�ļ�д�����\n");
                return ERROR;
            }
            if (fwrite(&ch, sizeof(TagType), 1, fp) != 1)
            {
                //�����ַ�'#'��ʾ�ڽӱ����
                printf("�ļ�д�����\n");
                return ERROR;
            }
        }
    }
    fclose(fp); //�ر��ļ�
    return OK;
}
//14.����ͼ
status GetGraph(Digraph &G, char filename[])
{
    FILE *fp; //�ļ�ָ��
    pArc p, q;
    char ch;
    int i;
    if (G.Vexnum != 0)
        DestroyGraph(G); //���ͼ�Ѵ��ڣ�����ͼ
    if ((fp = fopen(filename, "rb")) == NULL)
    {                               //��ֻ����ʽ���ļ�
        printf("�ļ��򿪴���\n"); //ָ��Ϊ�գ�����
        return ERROR;
    }
    if (fread(&G.Vexnum, sizeof(int), 1, fp) != 1)
    { //���ض�����
        printf("�ļ���ȡ����\n");
        return ERROR;
    }
    if (fread(&G.Arcnum, sizeof(int), 1, fp) != 1)
    { //���ػ���
        printf("�ļ���ȡ����\n");
        return ERROR;
    }
    for (i = 0; i < G.Vexnum; i++)
    {
        //���μ���ÿ������Ĺؼ��֣��������ڽӵ�
        if (fread(&G.V[i].tag, sizeof(TagType), 1, fp) != 1)
        { //���ض���ؼ���
            printf("�ļ���ȡ����\n");
            return ERROR;
        }
        if (fread(&G.V[i].data, sizeof(ElemType), 1, fp) != 1)
        { //���ض���������
            printf("�ļ���ȡ����\n");
            return ERROR;
        }
        if (fread(&ch, sizeof(TagType), 1, fp) != 1)
        { //�����ڽӵ�
            printf("�ļ���ȡ����\n");
            return ERROR;
        }
        if (ch == '#')
            G.V[i].firstArc = NULL; //'#'��ʾ�ڽӱ�Ϊ��
        else
        { //��Ϊ�գ����ص�һ���ڽӵ�
            p = (pArc)malloc(sizeof(Arc));
            p->adjvex = ch;
            p->nextarc = NULL;
            G.V[i].firstArc = p;
        }
        while (1)
        { //���������ڽӵ�
            if (fread(&ch, sizeof(TagType), 1, fp) != 1)
            {
                printf("�ļ���ȡ����\n");
                return ERROR;
            }
            if (ch == '#')
                break; //'#'��ʾ�ڽӵ��Լ����꣬�˳�ѭ��
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
    fclose(fp); //�ر��ļ�
    return OK;
}
//15.�ı������ͼ
status ChangeGraph(int &t)
{
    printf(" ������Ҫ�Եڼ���ͼ���в��� [1~100]\n");
    scanf("%d", &t);
    if (t > 0 && t < 101)
        return OK; //���Ϸ�ΧҪ��
    else
        return ERROR; //������Χ
}
//16.���ͼ���ڽӱ�
status PrintGraph(Digraph G)
{
    pArc p, q;
    int i;
    printf("ͼG��%d�����㣬%d������\n", G.Vexnum, G.Arcnum);
    //���������������
    for (i = 0; i < G.Vexnum; i++)
    {                                                //�������������Ϣ
        printf("%c(%d): ", G.V[i].tag, G.V[i].data); //�������ؼ��ּ�������
        p = G.V[i].firstArc;
        if (p)
        {
            while (p->nextarc != NULL)
            {
                printf("%c ", p->adjvex); //��������ڽӵ�
                p = p->nextarc;
            }
            printf("%c ", p->adjvex); //������һ���ڽӵ�
        }
        printf("\n");
    }
}

/*----------��������----------*/
//1.����������������ݹ麯��
status DFS(Digraph G, TagType v, int *visited, int i)
{
    pVex u;     //�洢����ָ��
    TagType w;  //�洢����ؼ���
    status ret; //�洢��������ֵ
    int j;
    if (!visited[i])
    {                       //�ö���δ�����ҹ�
        *(visited + i) = 1; //�޸Ĳ��ұ�־
        printf("%c(%d) ", G.V[i].tag, G.V[i].data);
    }
    else
        return OK;
    ret = FirstAdjVex(G, v, u);
    for (; ret > 0; ret = NextAdjVex(G, v, w, u))
    { //���β����ڽӵ�
        if (u)
        { //�ҵ��ڽӵ�
            for (j = 0; j < G.Vexnum; j++)
                if (u->tag == G.V[j].tag)
                    break;              //��λ����
            DFS(G, u->tag, visited, j); //���¶��㿪ʼ�����������
            w = u->tag;
        }
        else
            break; //�ڽӵ��Ѳ����꣬�˳�ѭ��
    }
    return OK;
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