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

typedef int status;
typedef int ElemType;	//����Ԫ�����Ͷ���

/*----- ���Ա�̬�������Դ洢�ṹ -----*/
#define LIST_INIT_SIZE 100	//�洢�ռ��ʼ������
#define LISTINCREMENT  10	//��������
typedef struct {  	    //˳���˳��ṹ���Ķ���
	ElemType * elem;	//�洢�ռ��ַ
	int length;     	//��ǰ����
	int listsize;   	//����洢����
}SqList;

/*------ �������� ------*/
status InitList(SqList& L);
status DestroyList(SqList& L);
status ClearList(SqList& L);
status ListEmpty(SqList L);
status ListLength(SqList L);
status GetElem(SqList L, int i, ElemType& e);
status LocateElem(SqList L,ElemType e); //�򻯹�
status PriorElem(SqList L, ElemType cur, ElemType& pre_e);
status NextElem(SqList L, ElemType cur, ElemType &next_e);
status ListInsert(SqList& L, int i, ElemType e);
status ListDelete(SqList& L, int i, ElemType& e);
status ListTrabverse(SqList L);  //�򻯹�
status SaveList(SqList L, char filename[]);
status GetList(SqList& L, char filename[]);
status ChangeList(int &t);

/*----main����----*/
int main(void) {
	SqList L[100];  //�ṹ���飬�Զ�����Ա����
	char filename[100];  //�ַ����飬�洢�ַ���
	int i, t = 0;
	for(i = 0; i < 100; i++) L[i].elem = NULL;  //ָ���ʼ��
	do{
		printf(" ������Ҫ�Եڼ������Ա���в��� [1~100]\n");
		scanf("%d", &t);
		getchar();
	}while(t < 1 || t > 100);
	system("cls");   //����
	ElemType e, cur, pre_e, next_e;
	int op = 1;
	while(op) {
		i = 0;
		system("cls");	printf("\n\n");
		printf("      ���ڶԵ�%d�����Ա���в���    \n", t);
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
		printf("    ��ѡ����Ĳ���[0~15]:");
		scanf("%d", &op);
    	switch(op) {
	   		case 1:
		 		//IntiList
		 		i = InitList(L[t-1]);
		 		if(i == OK) printf("���Ա����ɹ���\n");   //����ֵΪOK
		     	else if(i == ERROR) printf("���Ա��Ѵ��ڣ�\n");
				else printf("���Ա���ʧ�ܣ�\n");
		 		getchar();getchar();
		 		break;
	   		case 2:
		 		//DestroyList
		 		if(DestroyList(L[t-1]) == OK) printf("���Ա����ٳɹ���\n");//����OK
		     	else printf("���Ա�����ʧ�ܣ�\n");
	 			getchar();getchar();
	 			break;
	   		case 3:
 		  		//ClearList
				if(ClearList(L[t-1]) == OK) printf("���Ա����óɹ���\n");  //����OK
		     	else printf("���Ա�����ʧ�ܣ�\n");
				getchar();getchar();
	 			break;
  			case 4:
 				//ListEmpty
 				if(ListEmpty(L[t-1]) == TRUE) printf("���Ա�Ϊ�ձ�\n");  //����TRUE
 				else printf("���Ա�Ϊ�ձ�\n");
	 			getchar();getchar();
	 			break;
  			case 5:
	 			//ListLength
	 			printf("���Ա�������Ԫ�ظ���Ϊ%d����\n", ListLength(L[t-1])); //���Ԫ�ظ���
	 			getchar();getchar();
	 			break;
  			case 6:
	 			//GetElem
	 			printf("������Ҫ����ڼ�λ����Ԫ�أ�\n");
	 			scanf("%d", &i);
	 			getchar();
	 			if(GetElem(L[t-1], i, e) == OVERFLOW)   //����OVERFLOW
					printf("λ��������Χ��\n");    //������Χ
	 			else                               //�������Ԫ��ֵ
			 		printf("���Ա��е�%d������Ԫ�ص�ֵΪ%d��\n", i, e);
	 			getchar();getchar();
	 			break;
  			case 7:
	 			//LocateElem
	 			printf("���������ѯ������Ԫ�أ�\n");
	 			scanf("%d", &e);
	 			i = LocateElem(L[t-1], e);   //����Ԫ��λ��
				if(i != FALSE)
					printf("������Ԫ��Ϊ��%d��Ԫ�أ�\n", i);
				else printf("������Ԫ�ز��ڱ��У�\n");
	 			getchar();getchar();
	 			break;
			case 8:
	 			//PriorElem
	 			printf("���������ѯǰ��������Ԫ�أ�\n");
	 			scanf("%d", &cur);                               //��������Ԫ��
	 			i = PriorElem(L[t-1], cur, pre_e);
	 			if(i == TRUE) printf("��ѯ������Ԫ��Ϊ%d����ǰ��Ϊ%d��\n", cur, pre_e);  //����TRUE,���ǰ��Ԫ��ֵ
	 			else if(i == FALSE) printf("��ѯ������Ԫ��Ϊ��Ԫ�أ���ǰ����\n");  //����FALSE,��Ԫ��
				else printf("��ѯ������Ԫ�ز��ڱ��У�\n");
	 			getchar();getchar();
	 			break;
  			case 9:
	 			//NextElem
	 			printf("���������ѯ��̵�����Ԫ�أ�\n");
	 			scanf("%d", &cur);                                //��������Ԫ��
	 			i = NextElem(L[t-1], cur, next_e);
	 			if(i == TRUE) printf("��ѯ������Ԫ��Ϊ%d������Ϊ%d��\n", cur, next_e);  //����TRUE,������Ԫ��ֵ
	 			else if(i == FALSE) printf("��ѯ������Ԫ��ΪβԪ�أ��޺�̣�\n");  //����FALSE,βԪ��
				else printf("��ѯ������Ԫ�ز��ڱ��У�\n");
	 			getchar();getchar();
	 			break;
  			case 10:
	 			//printf("\n----ListInsert���ܴ�ʵ�֣�\n");
	 			printf("���������ڵڼ���Ԫ��ǰ����Ԫ�أ�\n");
	 			scanf("%d", &i);
	 			printf("������������Ԫ�أ�\n");
	 			scanf("%d", &e);
	 			if(ListInsert(L[t-1], i, e)) printf("����Ԫ�ز���ɹ���\n");
	 			else printf("����Ԫ�ز���ʧ�ܣ�\n");
	 			getchar();getchar();
	 			break;
			case 11:
 				//ListDelete
 				printf("��������ɾ���ڼ���Ԫ�أ�\n");
	 			scanf("%d", &i);
	 			if(ListDelete(L[t-1], i, e)) printf("��%d������Ԫ��%dɾ���ɹ���\n", i, e);
	 			else printf("����Ԫ��ɾ��ʧ�ܣ�\n");
	 			getchar();getchar();
	 			break;
	   		case 12:
		 		//ListTrabverse
		 		if(L[t-1].elem == NULL) {
		 			printf("�����Ա����ڣ�\n");
		 			getchar();getchar();
		 			break;
				 }
	 			if(!ListTrabverse(L[t-1])) printf("���Ա��ǿձ�\n");
				getchar();getchar();
	 			break;
            case 13:
            	//SaveList
         		printf("�����뱣���ļ�����\n");
         		scanf("%s", filename);
         		if(SaveList(L[t-1], filename)) printf("�ļ�����ɹ���\n");
         		else printf("�ļ�����ʧ�ܣ�\n");
         		getchar();getchar();
         		break;
         	case 14:
         		//GetList
				printf("��������Ҫ���ص��ļ�����\n");
				scanf("%s", filename);
         		if(GetList(L[t-1], filename)) printf("�ļ����سɹ���\n");
         		else ("�ļ�����ʧ�ܣ�\n");
				getchar();getchar();
         		break;
         	case 15:
         		//ChangeList
         		i = t;
         		if(ChangeList(i) == OK) {
         			t = i;
         			printf("���Ա��ѳɹ��ı�����%d����\n", t);
				}
         		else printf("���Ա�ı�ʧ�ܣ�\n");
         		getchar();getchar();
         		break;
			case 0:
				//�˳�
         		break;
			default :
				//�������
				printf("����������������룡\n");
				getchar();getchar();
				break;
			}//end of switch
		}//end of while
	printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
}//end of main()

/*------------------��������-------------------*/
//1.����յ����Ա�
status InitList(SqList& L) {
	if(L.elem != NULL) return ERROR; //���Ա��Ѵ���
	else {
		L.elem = (ElemType *)malloc( LIST_INIT_SIZE * sizeof (ElemType));
		if(!L.elem) exit(OVERFLOW);  //�洢�ռ����ʧ��
		L.length = 0;                //�ձ���Ϊ0
		L.listsize = LIST_INIT_SIZE; //��ʼ�洢����ΪLIST_INIT_SIZE
		return OK;                   //�洢�ռ����ɹ�������OK
	}

}
//2.�������Ա�
status DestroyList(SqList& L) {
	if(L.elem != NULL) {
		free(L.elem);      //�ͷŴ洢�ռ�
		L.elem = NULL;
		L.length = 0;
		L.listsize = 0;    //������Ϊ0
		return OK;         //����OK
	}
	return ERROR;          //ָ��Ϊ�գ�����ERROR
}
//3.����Ϊ�ձ�
status ClearList(SqList& L) {
	if(L.elem != NULL) {
		L.length = 0;       //��������Ϊ0
		return OK;          //����OK
	}
	return ERROR;           //ָ��Ϊ�գ�����ERROR
}
//4.����Ƿ�Ϊ�ձ�
status ListEmpty(SqList L) {
	if(L.length == 0) return TRUE;   //�ձ��򷵻�TRUE
	else return FALSE;               //���򷵻�FALSE
}
//5.����������Ԫ�ظ���
status ListLength(SqList L) {
	return L.length;
}
//6.���ر��е�i��Ԫ�ص�ֵ
status GetElem(SqList L, int i, ElemType& e) {
	if(i < 1 || i > L.length)  //i������Χ
		return OVERFLOW;
	else {
		e = L.elem[i-1];       //��ֵ
		return OK;
	}
}
//7.���ر���������Ԫ����ȵ�����Ԫ�����ڵ�λ��
status LocateElem(SqList L,ElemType e) {
	int i;
	for(i = 0; i < L.length; i++)      //����
		if(L.elem[i] == e) return i + 1;  //�ҵ�������i+1
	return FALSE;
}
//8.�������������Ԫ�ص�ǰ��
status PriorElem(SqList L, ElemType cur, ElemType& pre_e) {
	int i;
	i = LocateElem(L, cur);   //���ص�i��Ԫ����cur���
	if(i > 1 && i < L.length+1) {
		pre_e = L.elem[i-2];  //pre_eΪ��i-1��Ԫ�أ��±�Ϊi-2
		return TRUE;
	}
	else if(i == 1) return FALSE;
	else return OVERFLOW;
}
//9.�������������Ԫ�صĺ��
status NextElem(SqList L, ElemType cur, ElemType &next_e) {
	int i;
	i = LocateElem(L, cur);  //���ص�i��Ԫ����cur���
	if(i > 0 && i < L.length) {
		next_e = L.elem[i];  //next_eΪ��i+1��Ԫ�أ��±�Ϊi
		return TRUE;
	}
	else if(i == L.length) return FALSE;
	else return OVERFLOW;
}
//10.����е�i��Ԫ��ǰ����������Ԫ��
status ListInsert(SqList& L, int i, ElemType e) {
	ElemType *newsize;
	int j;
	if(i < 1 || i > L.length + 1) return FALSE;  //i������Χ
	if(L.length >= L.listsize) {  //�洢�ռ�����
		newsize = (ElemType*)realloc(L.elem,(L.listsize+LISTINCREMENT)*sizeof(ElemType));
		if(!newsize) exit(OVERFLOW);   //�洢�ռ����ʧ��
		L.elem = newsize;
		L.listsize += LISTINCREMENT;   //�洢�ռ�ˢ��
	}
	for(j = L.length; j >= i; j-- ) L.elem[j] = L.elem[j-1];  //����Ԫ�غ���һλ
	L.elem[i-1] = e;   //����Ԫ��
	L.length++;        //����ˢ��
	return TRUE;
}
//11.ɾ�����е�i��Ԫ��
status ListDelete(SqList& L, int i, ElemType& e) {
	if(i < 1 || i > L.length) return FALSE;  //i������Χ
	e = L.elem[i-1];   //��ֵ
	int j;
	for(j = i; j < L.length; j++ ) L.elem[j-1] = L.elem[j]; //����Ԫ��ǰ��һλ
	L.length--;     //����ˢ��
	return TRUE;
}
//12.�������Ա���Ԫ��
status ListTrabverse(SqList L){
	int i;
	printf("\n-----------all elements -----------------------\n");
	for(i = 0; i < L.length; i++) printf("%d ", L.elem[i]);  //���������Ԫ��ֵ
	printf("\n------------------ end ------------------------\n");
	return L.length;
}
//13.�������Ա����ļ�
status SaveList(SqList L, char filename[]) {
	FILE* fp;     //�ļ�ָ��
	int i;
	if((fp = fopen(filename, "wb")) == NULL) {  //��ֻд��ʽ���ļ�
		printf("�ļ��򿪴���\n");             // ָ��Ϊ�գ�����
		return FALSE;
	}
	for(i = 0; i < L.length; i++) {
		if(fwrite(&L.elem[i], sizeof(ElemType), 1, fp) != 1)  //����ֵ��Ϊ1��д�����
			printf("�ļ�д�����\n");
	}
	fclose(fp);   //�ر��ļ�
	return TRUE;
}
//14.��ȡ�ļ�
status GetList(SqList& L, char filename[]) {
	FILE* fp;     //�ļ�ָ��
	int i = 0;
	if((fp = fopen(filename, "rb")) == NULL) {  //��ֻ����ʽ���ļ�
		printf("�ļ���ȡ����\n");                 //ָ��Ϊ�գ�����
		return FALSE;
	}
	while(fread(&L.elem[i], sizeof(ElemType), 1, fp)) i++;  //��ȡ�ļ�
	L.length =  i;
	fclose(fp);    //�ر��ļ�
	return TRUE;
}
//15.���Ĳ��������Ա�
status ChangeList(int &t) {
	printf(" ������Ҫ�Եڼ������Ա���в��� [1~100]\n");
	scanf("%d", &t);
	if(t > 0 && t < 101) return OK;
	else return ERROR;
}
