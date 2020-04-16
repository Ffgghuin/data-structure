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

typedef int status;
typedef int ElemType;	//数据元素类型定义

/*----- 线性表动态分配线性存储结构 -----*/
#define LIST_INIT_SIZE 100	//存储空间分配量

typedef struct str{  	         //链表的定义
	struct str * next;	 //指针域
	ElemType data;       //数据域
}SqList;

/*------ 函数声明 ------*/
status InitList(SqList& L);
status DestroyList(SqList& L);
status ClearList(SqList& L);
status ListEmpty(SqList L);
status ListLength(SqList L);
status GetElem(SqList L, int i, ElemType& e);
status LocateElem(SqList L, ElemType e); //简化过
status PriorElem(SqList L, ElemType cur, ElemType& pre_e);
status NextElem(SqList L, ElemType cur, ElemType& next_e);
status ListInsert(SqList& L, int i, ElemType e);
status ListDelete(SqList& L, int i, ElemType& e);
status ListTrabverse(SqList L);  //简化过
status SaveList(SqList L, char filename[]);
status GetList(SqList& L, char filename[]);
status ChangeList(int &t);

/*----main函数----*/
int main(void) {
	SqList L[100];
	char filename[100];
	int i, j, t = 0;
	for(i = 0; i < 100; i++) L[i].next = NULL;  //指针初始化
	do{
		printf(" 请输入要对第几个线性表进行操作 [1~100]\n");
		scanf("%d", &t);
		getchar();
	}while(t < 1 || t > 100);
	system("cls");   //清屏
	ElemType e, cur, pre_e, next_e;
	int op = 1;
	while(op) {
		system("cls");	printf("\n\n");
		printf("      正在对第%d个线性表进行操作    \n", t);
		printf("      Menu for Linked Table On Sequence Structure \n");
		printf("---------------------------------------------------------\n");
		printf("    	  1. InitList         9. NextElem      \n");
		printf("    	  2. DestroyList     10. ListInsert    \n");
		printf("    	  3. ClearList       11. ListDelete    \n");
		printf("    	  4. ListEmpty       12. ListTrabverse \n");
		printf("    	  5. ListLength      13. SavaList      \n");
		printf("    	  6. GetElem         14. GetList       \n");
		printf("    	  7. LocateElem      15. ChangeList    \n");
		printf("    	  8. PriorElem        0. Exit          \n");
		printf("---------------------------------------------------------\n");
		printf("      请选择你的操作[0~15]:");
		scanf("%d", &op);
    	switch(op) {
	   		case 1:
		 		//InitList
		 		i = InitList(L[t-1]);
		 		if(i == OK) printf("线性表创建成功！\n");   //返回值为OK
		     	else if(i == ERROR) printf("线性表已存在！\n");
				else printf("线性表创建失败！\n");
		 		getchar();getchar();
		 		break;
	   		case 2:
		 		//DestroyList
		 		if(DestroyList(L[t-1]) == OK) printf("线性表销毁成功！\n");//返回OK
		     	else printf("线性表销毁失败！\n");
	 			getchar();getchar();
	 			break;
	   		case 3:
 		  		//ClearList
 		  		i = ClearList(L[t-1]);
				if(i == OK) printf("线性表重置成功！\n");  //返回OK
		     	else if(i == ERROR) printf("已为空表，线性表重置失败！\n");
		     	else printf("线性表不存在，重置失败！\n");
				getchar();getchar();
	 			break;
  			case 4:
 				//ListEmpty
 				if(ListEmpty(L[t-1]) == TRUE) printf("线性表为空表！\n");  //返回TRUE
 				else printf("线性表不为空表！\n");
	 			getchar();getchar();
	 			break;
  			case 5:
	 			//ListLength
	 			i = ListLength(L[t-1]);
	 			if(i >= 0) printf("线性表中数据元素个数为%d个！\n", i); //输出元素个数
	 			else printf("线性表不存在，无数据元素！\n");
	 			getchar();getchar();
	 			break;
  			case 6:
	 			//GetElem
	 			printf("请输入要输出第几位数据元素！\n");
	 			scanf("%d", &i);
	 			getchar();
	 			j = GetElem(L[t-1], i, e);
	 			if(j == OVERFLOW) printf("线性表不存在！\n"); //返回OVERFLOW,线性表不存在
	 			else if(j == OK) printf("线性表中第%d个数据元素的值为%d！\n", i, e); //返回OK，输出元素值
	 			else printf("位数超出范围！\n");//返回ERROR，位数超出范围
	 			getchar();getchar();
	 			break;
  			case 7:
	 			//LocateElem
	 			printf("请输入想查询的数据元素！\n");
	 			scanf("%d", &e);
	 			i = LocateElem(L[t-1], e);   //返回元素位数
				if(i != FALSE)
					printf("该数据元素为第%d个元素！\n", i);
				else printf("该数据元素不在表中！\n");
	 			getchar();getchar();
	 			break;
			case 8:
	 			//PriorElem
	 			printf("请输入想查询前驱的数据元素！\n");
	 			scanf("%d", &cur);                               //读入输入元素
	 			i = PriorElem(L[t-1], cur, pre_e);
	 			if(i == TRUE) printf("查询的数据元素为%d，其前驱为%d！\n", cur, pre_e);  //返回TRUE,输出前驱元素值
	 			else if(i == FALSE) printf("查询的数据元素为首元素，无前驱！\n");  //返回FALSE,首元素
				else printf("查询的数据元素不在表中！\n");
	 			getchar();getchar();
	 			break;
  			case 9:
	 			//NextElem
	 			printf("请输入想查询后继的数据元素！\n");
	 			scanf("%d", &cur);                                //读入输入元素
	 			i = NextElem(L[t-1], cur, next_e);
	 			if(i == TRUE) printf("查询的数据元素为%d，其后继为%d！\n", cur, next_e);  //返回TRUE,输出后继元素值
	 			else if(i == FALSE) printf("查询的数据元素为尾元素，无后继！\n");  //返回FALSE,尾元素
				else printf("查询的数据元素不在表中！\n");
	 			getchar();getchar();
	 			break;
  			case 10:
	 			//ListInsert
	 			printf("请输入想在第几个元素前插入元素！\n");
	 			scanf("%d", &i);
	 			printf("请输入想插入的元素！\n");
	 			scanf("%d", &e);
	 			j = ListInsert(L[t-1], i, e);
	 			if(j) printf("数据元素为%d，成功插入到第%d个元素前！\n", e, i);
	 			else if(j == FALSE) printf("超出范围，数据元素插入失败！\n");
	 			else printf("线性表不存在！/n");
	 			getchar();getchar();
	 			break;
			case 11:
 				//ListDelete
 				printf("请输入想删除第几个元素！\n");
	 			scanf("%d", &i);
				j = ListDelete(L[t-1], i, e);
	 			if(j) printf("第%d个数据元素%d删除成功！\n", i, e);
	 			else if(j == FALSE) printf("超出范围，数据元素删除失败！\n");
	 			else printf("线性表不存在！\n");
	 			getchar();getchar();
	 			break;
	   		case 12:
		 		//ListTrabverse
		 		j = ListTrabverse(L[t-1]);
	 			if(j == FALSE) printf("线性表是空表！\n");
				else if(j == OVERFLOW) printf("线性表不存在！\n");
				getchar();getchar();
	 			break;
            case 13:
				//SaveList
         		char filename[50];
         		printf("请输入保存文件名！\n");
         		scanf("%s", filename);
         		if(SaveList(L[t-1], filename)) printf("文件保存成功！\n");
         		else printf("文件保存失败！\n");
         		getchar();getchar();
         		break;
         	case 14:
         		//GetList
         		printf("请输入加载文件名！\n");
         		scanf("%s", filename);
         		if(GetList(L[t-1], filename)) printf("文件加载成功！\n");
         		else printf("文件加载失败！\n");
				getchar();getchar();
         		break;
         	case 15:
         		//ChangeList
         		i = t;
         		if(ChangeList(i) == OK) {
         			t = i;
         			printf("线性表已成功改变至第%d个！\n", t);
				}
         		else printf("线性表改变失败！\n");
         		getchar();getchar();
         		break;
			case 0:
				//退出
         		break;
			default :
				//输入错误
				printf("输入错误，请重新输入！\n");
				getchar();getchar();
				break;
			}//end of switch
		}//end of while
	printf("欢迎下次再使用本系统！\n");
}//end of main()

/*---------函数定义---------*/
//1.构造空的线性表
status InitList(SqList& L) {
	if(L.next != NULL) return ERROR; //线性表已存在
	else {
		L.next = (SqList *)malloc(sizeof (SqList));
		if(!L.next) exit(OVERFLOW);  //存储空间分配失败
		L.data = 0;                  //数据域无意义，初始化为0
		(L.next)->next = NULL;           //初始化头结点
		(L.next)->data = 0;
		return OK;                   //存储空间分配成功，返回OK
	}
}
//2.销毁线性表
status DestroyList(SqList& L) {
	if(L.next != NULL) {
		SqList *p, *q;
		for(p = q = L.next; p->next != NULL; p = q ) {
			q = p->next;
			free(p);
		}	//释放链表存储空间
		free(p);
		L.next = NULL;  //不存在头结点
		return OK;         //返回OK
	}
	return ERROR;          //指针为空，返回ERROR
}
//3.重置为空表
status ClearList(SqList& L) {
	if(L.next != NULL) {
		SqList *head = L.next;
		if(head->next != NULL) {
		SqList *p, *q;
		for(p = q = head->next; p->next != NULL; p = q ) {
			q = p->next;
			free(p);
		}	//释放链表存储空间
		free(p);
		head->next = NULL;
		return OK;          //返回OK，重置成功
		}
		else if(head->next == NULL) return ERROR;  //已为空表，返回ERROR
	}
	else return OVERFLOW;   //链表不存在，返回OVERFLOW
}
//4.检测是否为空表
status ListEmpty(SqList L) {
	SqList *head = L.next;
	if(head != NULL && head->next == NULL) return TRUE;   //空表则返回TRUE
	else return FALSE;               //否则返回FALSE
}
//5.检测表中数据元素个数
status ListLength(SqList L) {
	SqList *p, *head = L.next;
	int i;
	if(head != NULL)
 		for(i = 0, p = head; p->next != NULL; p = p->next) i++;
	else i = -1;
 	return i;
}
//6.返回表中第i个元素的值
status GetElem(SqList L, int i, ElemType& e) {
	SqList *p, *q;
	int j;
	if(L.next == NULL) return OVERFLOW;  //线性表不存在
	if(i <= 0) return ERROR; //i超出范围
	for(p = L.next, j = 0; p->next != NULL && j < i; p = p->next, j++); //遍历找到第i个元素
	if(j == i) {
		e = p->data;    //赋值
		return OK;
	}
	else return ERROR; //否则i超出范围
}
//7.返回表中与输入元素相等的数据元素的位数
status LocateElem(SqList L, ElemType e) {
	SqList *p,*q = L.next;
	int i;
	if(q == NULL || q->next == NULL) return FALSE;
	for(p = q->next, i = 1; p->next != NULL; p = p->next, i++)  //遍历寻找元素
		if(p->data == e) return i;     //找到即返回i，退出循环
	if(p->data == e) return i;     //检验最后一个结点的数据域
	return FALSE;
}
//8.返回输入的数据元素的前驱
status PriorElem(SqList L, ElemType cur, ElemType& pre_e) {
	SqList *p, *q = L.next;
	int i, j;
	i = LocateElem(L, cur);   //返回第i个元素与cur相等
	if(!i) return OVERFLOW;   //若i返回FALSE，不在表中，返回OVERFLOW
	j = GetElem(L, i-1, pre_e);
	if(j == OK) return TRUE;  //返回TRUE,输出前驱元素值
	else if(j == ERROR) return FALSE;  //返回FALSE,首元素
	else return OVERFLOW;
	return 0;
}
//9.返回输入的数据元素的后继
status NextElem(SqList L, ElemType cur, ElemType& next_e) {
	SqList *p, *q = L.next;
	int i, j;
	i = LocateElem(L, cur);   //返回第i个元素与cur相等
	if(!i) return OVERFLOW;   //若i返回FALSE，不在表中，返回OVERFLOW
	j = GetElem(L, i+1, next_e);
	if(j == OK) return TRUE;  //返回TRUE,输出后继元素值
	else if(j == ERROR) return FALSE;  //返回FALSE,尾元素
	else return OVERFLOW;
	return 0;
}
//10.向表中第i个元素前插入新数据元素
status ListInsert(SqList& L, int i, ElemType e) {
	SqList *p, *q, *l;
	int j;
	if(L.next == NULL) return OVERFLOW;
	for(p = L.next, j = 1; p->next != NULL && j < i; p = p->next, j++);
	if(j < i) return FALSE;
	q = p->next;
	l = (SqList *)malloc(sizeof(SqList));
	l->data = e;
	p->next = l;
	l->next = q;
	return TRUE;

}
//11.删除表中第i个元素
status ListDelete(SqList& L, int i, ElemType& e) {
 	SqList *p, *q;
	int j;
	if(L.next == NULL) return OVERFLOW;
	for(p = L.next, j = 1; p->next != NULL && j < i; p = p->next, j++);
	if(j < i) return FALSE;
	q = p->next;
	p->next = q->next;
	e = q->data;
	free(q);
	return TRUE;
}
//12.遍历线性表中元素
status ListTrabverse(SqList L){
	int i;
	SqList *p, *q = L.next;
	if(q == NULL) return OVERFLOW;
	if(q->next == NULL) return FALSE;
	printf("\n-----------all elements -----------------------\n");
 	for(p = q->next; p->next != NULL; p = p->next)  //遍历寻找元素
 		printf("%d ", p->data);
 	printf("%d", p->data);
	printf("\n------------------ end ------------------------\n");
	return TRUE;
}
//13.保存线性表至文件
status SaveList(SqList L, char filename[]) {
	FILE* fp;     //文件指针
	SqList *p, *q = L.next;
	if(q == NULL) return OVERFLOW;
	if((fp = fopen(filename, "wb")) == NULL) {  //以只写方式打开文件
		printf("文件打开错误！\n");             // 指针为空，出错
		return FALSE;
	}
	for(p = q->next; p->next != NULL; p = p->next)
		if(fwrite(&(p->data), sizeof(ElemType), 1, fp) != 1)  //返回值不为1则写入出错
			printf("文件写入错误！\n");
	if(fwrite(&(p->data), sizeof(ElemType), 1, fp) != 1)  //返回值不为1则写入出错
			printf("文件写入错误！\n");
	fclose(fp);   //关闭文件
	return TRUE;
}
//14.读取文件
status GetList(SqList& L, char filename[]) {
	FILE* fp;     //文件指针
	int i;
	SqList *l = NULL, *p = NULL;
 	DestroyList(L);  //销毁链表
	InitList(L);  //重新创建新链表
	if((fp = fopen(filename, "rb")) == NULL) {  //以只读方式打开文件
		printf("文件读取错误！\n");             //指针为空，出错
		return FALSE;
	}
 	for(p = L.next; fread(&i, sizeof(ElemType), 1, fp); p = p->next) {  //读取文件
		l = (SqList *)malloc(sizeof (SqList));  //申请新结点储存数据
		l->data = i;
		l->next = NULL;
		p->next = l;   //p->next指向l
	}
	fclose(fp);    //关闭文件
	return TRUE;
}
//15.更改操作的线性表
status ChangeList(int &t) {
	printf(" 请输入要对第几个线性表进行操作 [1~100]\n");
	scanf("%d", &t);
	if(t > 0 && t < 101) return OK;
	else return ERROR;
}
