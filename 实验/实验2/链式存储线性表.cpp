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
#define LIST_INIT_SIZE 100	//�洢�ռ������

typedef struct str{  	         //����Ķ���
	struct str * next;	 //ָ����
	ElemType data;       //������
}SqList;

/*------ �������� ------*/
status InitList(SqList& L);
status DestroyList(SqList& L);
status ClearList(SqList& L);
status ListEmpty(SqList L);
status ListLength(SqList L);
status GetElem(SqList L, int i, ElemType& e);
status LocateElem(SqList L, ElemType e); //�򻯹�
status PriorElem(SqList L, ElemType cur, ElemType& pre_e);
status NextElem(SqList L, ElemType cur, ElemType& next_e);
status ListInsert(SqList& L, int i, ElemType e);
status ListDelete(SqList& L, int i, ElemType& e);
status ListTrabverse(SqList L);  //�򻯹�
status SaveList(SqList L, char filename[]);
status GetList(SqList& L, char filename[]);
status ChangeList(int &t);

/*----main����----*/
int main(void) {
	SqList L[100];
	char filename[100];
	int i, j, t = 0;
	for(i = 0; i < 100; i++) L[i].next = NULL;  //ָ���ʼ��
	do{
		printf(" ������Ҫ�Եڼ������Ա���в��� [1~100]\n");
		scanf("%d", &t);
		getchar();
	}while(t < 1 || t > 100);
	system("cls");   //����
	ElemType e, cur, pre_e, next_e;
	int op = 1;
	while(op) {
		system("cls");	printf("\n\n");
		printf("      ���ڶԵ�%d�����Ա���в���    \n", t);
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
		printf("      ��ѡ����Ĳ���[0~15]:");
		scanf("%d", &op);
    	switch(op) {
	   		case 1:
		 		//InitList
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
 		  		i = ClearList(L[t-1]);
				if(i == OK) printf("���Ա����óɹ���\n");  //����OK
		     	else if(i == ERROR) printf("��Ϊ�ձ����Ա�����ʧ�ܣ�\n");
		     	else printf("���Ա����ڣ�����ʧ�ܣ�\n");
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
	 			i = ListLength(L[t-1]);
	 			if(i >= 0) printf("���Ա�������Ԫ�ظ���Ϊ%d����\n", i); //���Ԫ�ظ���
	 			else printf("���Ա����ڣ�������Ԫ�أ�\n");
	 			getchar();getchar();
	 			break;
  			case 6:
	 			//GetElem
	 			printf("������Ҫ����ڼ�λ����Ԫ�أ�\n");
	 			scanf("%d", &i);
	 			getchar();
	 			j = GetElem(L[t-1], i, e);
	 			if(j == OVERFLOW) printf("���Ա����ڣ�\n"); //����OVERFLOW,���Ա�����
	 			else if(j == OK) printf("���Ա��е�%d������Ԫ�ص�ֵΪ%d��\n", i, e); //����OK�����Ԫ��ֵ
	 			else printf("λ��������Χ��\n");//����ERROR��λ��������Χ
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
	 			//ListInsert
	 			printf("���������ڵڼ���Ԫ��ǰ����Ԫ�أ�\n");
	 			scanf("%d", &i);
	 			printf("������������Ԫ�أ�\n");
	 			scanf("%d", &e);
	 			j = ListInsert(L[t-1], i, e);
	 			if(j) printf("����Ԫ��Ϊ%d���ɹ����뵽��%d��Ԫ��ǰ��\n", e, i);
	 			else if(j == FALSE) printf("������Χ������Ԫ�ز���ʧ�ܣ�\n");
	 			else printf("���Ա����ڣ�/n");
	 			getchar();getchar();
	 			break;
			case 11:
 				//ListDelete
 				printf("��������ɾ���ڼ���Ԫ�أ�\n");
	 			scanf("%d", &i);
				j = ListDelete(L[t-1], i, e);
	 			if(j) printf("��%d������Ԫ��%dɾ���ɹ���\n", i, e);
	 			else if(j == FALSE) printf("������Χ������Ԫ��ɾ��ʧ�ܣ�\n");
	 			else printf("���Ա����ڣ�\n");
	 			getchar();getchar();
	 			break;
	   		case 12:
		 		//ListTrabverse
		 		j = ListTrabverse(L[t-1]);
	 			if(j == FALSE) printf("���Ա��ǿձ�\n");
				else if(j == OVERFLOW) printf("���Ա����ڣ�\n");
				getchar();getchar();
	 			break;
            case 13:
				//SaveList
         		char filename[50];
         		printf("�����뱣���ļ�����\n");
         		scanf("%s", filename);
         		if(SaveList(L[t-1], filename)) printf("�ļ�����ɹ���\n");
         		else printf("�ļ�����ʧ�ܣ�\n");
         		getchar();getchar();
         		break;
         	case 14:
         		//GetList
         		printf("����������ļ�����\n");
         		scanf("%s", filename);
         		if(GetList(L[t-1], filename)) printf("�ļ����سɹ���\n");
         		else printf("�ļ�����ʧ�ܣ�\n");
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

/*---------��������---------*/
//1.����յ����Ա�
status InitList(SqList& L) {
	if(L.next != NULL) return ERROR; //���Ա��Ѵ���
	else {
		L.next = (SqList *)malloc(sizeof (SqList));
		if(!L.next) exit(OVERFLOW);  //�洢�ռ����ʧ��
		L.data = 0;                  //�����������壬��ʼ��Ϊ0
		(L.next)->next = NULL;           //��ʼ��ͷ���
		(L.next)->data = 0;
		return OK;                   //�洢�ռ����ɹ�������OK
	}
}
//2.�������Ա�
status DestroyList(SqList& L) {
	if(L.next != NULL) {
		SqList *p, *q;
		for(p = q = L.next; p->next != NULL; p = q ) {
			q = p->next;
			free(p);
		}	//�ͷ�����洢�ռ�
		free(p);
		L.next = NULL;  //������ͷ���
		return OK;         //����OK
	}
	return ERROR;          //ָ��Ϊ�գ�����ERROR
}
//3.����Ϊ�ձ�
status ClearList(SqList& L) {
	if(L.next != NULL) {
		SqList *head = L.next;
		if(head->next != NULL) {
		SqList *p, *q;
		for(p = q = head->next; p->next != NULL; p = q ) {
			q = p->next;
			free(p);
		}	//�ͷ�����洢�ռ�
		free(p);
		head->next = NULL;
		return OK;          //����OK�����óɹ�
		}
		else if(head->next == NULL) return ERROR;  //��Ϊ�ձ�����ERROR
	}
	else return OVERFLOW;   //�������ڣ�����OVERFLOW
}
//4.����Ƿ�Ϊ�ձ�
status ListEmpty(SqList L) {
	SqList *head = L.next;
	if(head != NULL && head->next == NULL) return TRUE;   //�ձ��򷵻�TRUE
	else return FALSE;               //���򷵻�FALSE
}
//5.����������Ԫ�ظ���
status ListLength(SqList L) {
	SqList *p, *head = L.next;
	int i;
	if(head != NULL)
 		for(i = 0, p = head; p->next != NULL; p = p->next) i++;
	else i = -1;
 	return i;
}
//6.���ر��е�i��Ԫ�ص�ֵ
status GetElem(SqList L, int i, ElemType& e) {
	SqList *p, *q;
	int j;
	if(L.next == NULL) return OVERFLOW;  //���Ա�����
	if(i <= 0) return ERROR; //i������Χ
	for(p = L.next, j = 0; p->next != NULL && j < i; p = p->next, j++); //�����ҵ���i��Ԫ��
	if(j == i) {
		e = p->data;    //��ֵ
		return OK;
	}
	else return ERROR; //����i������Χ
}
//7.���ر���������Ԫ����ȵ�����Ԫ�ص�λ��
status LocateElem(SqList L, ElemType e) {
	SqList *p,*q = L.next;
	int i;
	if(q == NULL || q->next == NULL) return FALSE;
	for(p = q->next, i = 1; p->next != NULL; p = p->next, i++)  //����Ѱ��Ԫ��
		if(p->data == e) return i;     //�ҵ�������i���˳�ѭ��
	if(p->data == e) return i;     //�������һ������������
	return FALSE;
}
//8.�������������Ԫ�ص�ǰ��
status PriorElem(SqList L, ElemType cur, ElemType& pre_e) {
	SqList *p, *q = L.next;
	int i, j;
	i = LocateElem(L, cur);   //���ص�i��Ԫ����cur���
	if(!i) return OVERFLOW;   //��i����FALSE�����ڱ��У�����OVERFLOW
	j = GetElem(L, i-1, pre_e);
	if(j == OK) return TRUE;  //����TRUE,���ǰ��Ԫ��ֵ
	else if(j == ERROR) return FALSE;  //����FALSE,��Ԫ��
	else return OVERFLOW;
	return 0;
}
//9.�������������Ԫ�صĺ��
status NextElem(SqList L, ElemType cur, ElemType& next_e) {
	SqList *p, *q = L.next;
	int i, j;
	i = LocateElem(L, cur);   //���ص�i��Ԫ����cur���
	if(!i) return OVERFLOW;   //��i����FALSE�����ڱ��У�����OVERFLOW
	j = GetElem(L, i+1, next_e);
	if(j == OK) return TRUE;  //����TRUE,������Ԫ��ֵ
	else if(j == ERROR) return FALSE;  //����FALSE,βԪ��
	else return OVERFLOW;
	return 0;
}
//10.����е�i��Ԫ��ǰ����������Ԫ��
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
//11.ɾ�����е�i��Ԫ��
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
//12.�������Ա���Ԫ��
status ListTrabverse(SqList L){
	int i;
	SqList *p, *q = L.next;
	if(q == NULL) return OVERFLOW;
	if(q->next == NULL) return FALSE;
	printf("\n-----------all elements -----------------------\n");
 	for(p = q->next; p->next != NULL; p = p->next)  //����Ѱ��Ԫ��
 		printf("%d ", p->data);
 	printf("%d", p->data);
	printf("\n------------------ end ------------------------\n");
	return TRUE;
}
//13.�������Ա����ļ�
status SaveList(SqList L, char filename[]) {
	FILE* fp;     //�ļ�ָ��
	SqList *p, *q = L.next;
	if(q == NULL) return OVERFLOW;
	if((fp = fopen(filename, "wb")) == NULL) {  //��ֻд��ʽ���ļ�
		printf("�ļ��򿪴���\n");             // ָ��Ϊ�գ�����
		return FALSE;
	}
	for(p = q->next; p->next != NULL; p = p->next)
		if(fwrite(&(p->data), sizeof(ElemType), 1, fp) != 1)  //����ֵ��Ϊ1��д�����
			printf("�ļ�д�����\n");
	if(fwrite(&(p->data), sizeof(ElemType), 1, fp) != 1)  //����ֵ��Ϊ1��д�����
			printf("�ļ�д�����\n");
	fclose(fp);   //�ر��ļ�
	return TRUE;
}
//14.��ȡ�ļ�
status GetList(SqList& L, char filename[]) {
	FILE* fp;     //�ļ�ָ��
	int i;
	SqList *l = NULL, *p = NULL;
 	DestroyList(L);  //��������
	InitList(L);  //���´���������
	if((fp = fopen(filename, "rb")) == NULL) {  //��ֻ����ʽ���ļ�
		printf("�ļ���ȡ����\n");             //ָ��Ϊ�գ�����
		return FALSE;
	}
 	for(p = L.next; fread(&i, sizeof(ElemType), 1, fp); p = p->next) {  //��ȡ�ļ�
		l = (SqList *)malloc(sizeof (SqList));  //�����½�㴢������
		l->data = i;
		l->next = NULL;
		p->next = l;   //p->nextָ��l
	}
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
