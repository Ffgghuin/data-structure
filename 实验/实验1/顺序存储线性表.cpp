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
#define LIST_INIT_SIZE 100	//存储空间初始分配量
#define LISTINCREMENT  10	//分配增量
typedef struct {  	    //顺序表（顺序结构）的定义
	ElemType * elem;	//存储空间基址
	int length;     	//当前长度
	int listsize;   	//分配存储容量
}SqList;

/*------ 函数声明 ------*/
status InitList(SqList& L);
status DestroyList(SqList& L);
status ClearList(SqList& L);
status ListEmpty(SqList L);
status ListLength(SqList L);
status GetElem(SqList L, int i, ElemType& e);
status LocateElem(SqList L,ElemType e); //简化过
status PriorElem(SqList L, ElemType cur, ElemType& pre_e);
status NextElem(SqList L, ElemType cur, ElemType &next_e);
status ListInsert(SqList& L, int i, ElemType e);
status ListDelete(SqList& L, int i, ElemType& e);
status ListTrabverse(SqList L);  //简化过
status SaveList(SqList L, char filename[]);
status GetList(SqList& L, char filename[]);
status ChangeList(int &t);

/*----main函数----*/
int main(void) {
	SqList L[100];  //结构数组，对多个线性表操作
	char filename[100];  //字符数组，存储字符串
	int i, t = 0;
	for(i = 0; i < 100; i++) L[i].elem = NULL;  //指针初始化
	do{
		printf(" 请输入要对第几个线性表进行操作 [1~100]\n");
		scanf("%d", &t);
		getchar();
	}while(t < 1 || t > 100);
	system("cls");   //清屏
	ElemType e, cur, pre_e, next_e;
	int op = 1;
	while(op) {
		i = 0;
		system("cls");	printf("\n\n");
		printf("      正在对第%d个线性表进行操作    \n", t);
		printf("      Menu for Linear Table On Sequence Structure \n");
		printf("---------------------------------------------------------\n");
		printf("    	  1. InitList         9. NextElem      \n");
		printf("    	  2. DestroyList     10. ListInsert    \n");
		printf("    	  3. ClearList       11. ListDelete    \n");
		printf("    	  4. ListEmpty       12. ListTrabverse \n");
		printf("    	  5. ListLength      13. SaveList      \n");
		printf("    	  6. GetElem         14. GetList       \n");
		printf("    	  7. LocateElem      15. ChangeList    \n");
		printf("    	  8. PriorElem        0. Exit          \n");
		printf("---------------------------------------------------------\n");
		printf("    请选择你的操作[0~15]:");
		scanf("%d", &op);
    	switch(op) {
	   		case 1:
		 		//IntiList
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
				if(ClearList(L[t-1]) == OK) printf("线性表重置成功！\n");  //返回OK
		     	else printf("线性表重置失败！\n");
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
	 			printf("线性表中数据元素个数为%d个！\n", ListLength(L[t-1])); //输出元素个数
	 			getchar();getchar();
	 			break;
  			case 6:
	 			//GetElem
	 			printf("请输入要输出第几位数据元素！\n");
	 			scanf("%d", &i);
	 			getchar();
	 			if(GetElem(L[t-1], i, e) == OVERFLOW)   //返回OVERFLOW
					printf("位数超出范围！\n");    //超出范围
	 			else                               //否则输出元素值
			 		printf("线性表中第%d个数据元素的值为%d！\n", i, e);
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
	 			//printf("\n----ListInsert功能待实现！\n");
	 			printf("请输入想在第几个元素前插入元素！\n");
	 			scanf("%d", &i);
	 			printf("请输入想插入的元素！\n");
	 			scanf("%d", &e);
	 			if(ListInsert(L[t-1], i, e)) printf("数据元素插入成功！\n");
	 			else printf("数据元素插入失败！\n");
	 			getchar();getchar();
	 			break;
			case 11:
 				//ListDelete
 				printf("请输入想删除第几个元素！\n");
	 			scanf("%d", &i);
	 			if(ListDelete(L[t-1], i, e)) printf("第%d个数据元素%d删除成功！\n", i, e);
	 			else printf("数据元素删除失败！\n");
	 			getchar();getchar();
	 			break;
	   		case 12:
		 		//ListTrabverse
		 		if(L[t-1].elem == NULL) {
		 			printf("该线性表不存在！\n");
		 			getchar();getchar();
		 			break;
				 }
	 			if(!ListTrabverse(L[t-1])) printf("线性表是空表！\n");
				getchar();getchar();
	 			break;
            case 13:
            	//SaveList
         		printf("请输入保存文件名！\n");
         		scanf("%s", filename);
         		if(SaveList(L[t-1], filename)) printf("文件保存成功！\n");
         		else printf("文件保存失败！\n");
         		getchar();getchar();
         		break;
         	case 14:
         		//GetList
				printf("请输入需要加载的文件名：\n");
				scanf("%s", filename);
         		if(GetList(L[t-1], filename)) printf("文件加载成功！\n");
         		else ("文件加载失败！\n");
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

/*------------------函数定义-------------------*/
//1.构造空的线性表
status InitList(SqList& L) {
	if(L.elem != NULL) return ERROR; //线性表已存在
	else {
		L.elem = (ElemType *)malloc( LIST_INIT_SIZE * sizeof (ElemType));
		if(!L.elem) exit(OVERFLOW);  //存储空间分配失败
		L.length = 0;                //空表长度为0
		L.listsize = LIST_INIT_SIZE; //初始存储容量为LIST_INIT_SIZE
		return OK;                   //存储空间分配成功，返回OK
	}

}
//2.销毁线性表
status DestroyList(SqList& L) {
	if(L.elem != NULL) {
		free(L.elem);      //释放存储空间
		L.elem = NULL;
		L.length = 0;
		L.listsize = 0;    //均重置为0
		return OK;         //返回OK
	}
	return ERROR;          //指针为空，返回ERROR
}
//3.重置为空表
status ClearList(SqList& L) {
	if(L.elem != NULL) {
		L.length = 0;       //表长度重置为0
		return OK;          //返回OK
	}
	return ERROR;           //指针为空，返回ERROR
}
//4.检测是否为空表
status ListEmpty(SqList L) {
	if(L.length == 0) return TRUE;   //空表则返回TRUE
	else return FALSE;               //否则返回FALSE
}
//5.检测表中数据元素个数
status ListLength(SqList L) {
	return L.length;
}
//6.返回表中第i个元素的值
status GetElem(SqList L, int i, ElemType& e) {
	if(i < 1 || i > L.length)  //i超出范围
		return OVERFLOW;
	else {
		e = L.elem[i-1];       //赋值
		return OK;
	}
}
//7.返回表中与输入元素相等的数据元素所在的位数
status LocateElem(SqList L,ElemType e) {
	int i;
	for(i = 0; i < L.length; i++)      //遍历
		if(L.elem[i] == e) return i + 1;  //找到即返回i+1
	return FALSE;
}
//8.返回输入的数据元素的前驱
status PriorElem(SqList L, ElemType cur, ElemType& pre_e) {
	int i;
	i = LocateElem(L, cur);   //返回第i个元素与cur相等
	if(i > 1 && i < L.length+1) {
		pre_e = L.elem[i-2];  //pre_e为第i-1个元素，下标为i-2
		return TRUE;
	}
	else if(i == 1) return FALSE;
	else return OVERFLOW;
}
//9.返回输入的数据元素的后继
status NextElem(SqList L, ElemType cur, ElemType &next_e) {
	int i;
	i = LocateElem(L, cur);  //返回第i个元素与cur相等
	if(i > 0 && i < L.length) {
		next_e = L.elem[i];  //next_e为第i+1个元素，下标为i
		return TRUE;
	}
	else if(i == L.length) return FALSE;
	else return OVERFLOW;
}
//10.向表中第i个元素前插入新数据元素
status ListInsert(SqList& L, int i, ElemType e) {
	ElemType *newsize;
	int j;
	if(i < 1 || i > L.length + 1) return FALSE;  //i超出范围
	if(L.length >= L.listsize) {  //存储空间已满
		newsize = (ElemType*)realloc(L.elem,(L.listsize+LISTINCREMENT)*sizeof(ElemType));
		if(!newsize) exit(OVERFLOW);   //存储空间分配失败
		L.elem = newsize;
		L.listsize += LISTINCREMENT;   //存储空间刷新
	}
	for(j = L.length; j >= i; j-- ) L.elem[j] = L.elem[j-1];  //后续元素后移一位
	L.elem[i-1] = e;   //插入元素
	L.length++;        //长度刷新
	return TRUE;
}
//11.删除表中第i个元素
status ListDelete(SqList& L, int i, ElemType& e) {
	if(i < 1 || i > L.length) return FALSE;  //i超出范围
	e = L.elem[i-1];   //赋值
	int j;
	for(j = i; j < L.length; j++ ) L.elem[j-1] = L.elem[j]; //后续元素前移一位
	L.length--;     //长度刷新
	return TRUE;
}
//12.遍历线性表中元素
status ListTrabverse(SqList L){
	int i;
	printf("\n-----------all elements -----------------------\n");
	for(i = 0; i < L.length; i++) printf("%d ", L.elem[i]);  //遍历输出各元素值
	printf("\n------------------ end ------------------------\n");
	return L.length;
}
//13.保存线性表至文件
status SaveList(SqList L, char filename[]) {
	FILE* fp;     //文件指针
	int i;
	if((fp = fopen(filename, "wb")) == NULL) {  //以只写方式打开文件
		printf("文件打开错误！\n");             // 指针为空，出错
		return FALSE;
	}
	for(i = 0; i < L.length; i++) {
		if(fwrite(&L.elem[i], sizeof(ElemType), 1, fp) != 1)  //返回值不为1则写入出错
			printf("文件写入错误！\n");
	}
	fclose(fp);   //关闭文件
	return TRUE;
}
//14.读取文件
status GetList(SqList& L, char filename[]) {
	FILE* fp;     //文件指针
	int i = 0;
	if((fp = fopen(filename, "rb")) == NULL) {  //以只读方式打开文件
		printf("文件读取错误！\n");                 //指针为空，出错
		return FALSE;
	}
	while(fread(&L.elem[i], sizeof(ElemType), 1, fp)) i++;  //读取文件
	L.length =  i;
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
