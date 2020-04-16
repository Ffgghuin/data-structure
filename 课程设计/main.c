#include "head.h"

char filename[100] = "keywords.txt";
char token_text[TOKEN_TEXT]; //������ʶ��ֵ
pLUT Lookup[26];  //����1��ṹ����
FILE *fp; //�ļ�ָ��
int rownum = 1;
int errnum = 0;
enum token_kind w;    //�洢����ֵ
int main(int argc, char *argv[]) {
	int i;
	pLUT p;
	pTree root = (pTree)malloc(sizeof(Tree));
    root = NULL;
	int op = 1;
	while(op) {
		system("cls");	printf("\n\n");
		printf("--------------------------Menu---------------------------\n");
		printf("---------------------------------------------------------\n");
		printf("    	  1. CreateTable       4. GrammarAnalysis        \n");
		printf("    	  2. CheckTable        5. PrintAST               \n");
		printf("    	  3. LexicalAnalysis   6. PrintProgram           \n");
		printf("    	                       0. Exit                   \n");
		printf("---------------------------------------------------------\n");
		printf("      ��ѡ����Ĳ���[0~6]:");
		scanf("%d", &op);
		switch(op){
			case 1: //	�����ؼ��ֲ��ұ�
				if(LookupTable() == ERROR) {
					printf("�������ұ�ʧ�ܣ�\n");
				}			
				else printf("�������ұ�ɹ���\n");
				system("pause");
				break;
			case 2: //  ���ؼ��ֲ��ұ�
				for(i = 0; i < 26; i++){
					printf("%c:", i+'a');
					if(Lookup[i] == NULL) printf("\n");
					else {
						for(p = Lookup[i]; p != NULL; p = p->next)
							printf("%s  ", p->lookup);
						printf("\n");
					}
				}
				system("pause");
				break;
			case 3: //  �ʷ�����
				rownum = 1;
				Lexan();
				break;
			case 4: //  �﷨����
				rownum = 1;
				if((fp = fopen("test.txt", "rt")) == NULL) {  //��ֻ����ʽ���ļ�
					printf("�����ļ��򿪴���\n");    //ָ��Ϊ�գ�����
				}
				else {
					errnum = 0;
					root = Program();
					if(errnum) printf("����\n");
					else if(root != NULL) printf("�ɹ������﷨����\n");
				}
				fclose(fp);
				system("pause");
				break;
			case 5: //  ����﷨��
				if(root == NULL) printf("ERROR!\n");
				else {
					flag = 1;
					OutProgram(root);
				}
				system("pause");
				break;
			case 6: //  ���Դ����
				if(root == NULL) printf("ERROR!\n");
				else {
					flag = 0;
					OutProgram(root);
				}
				system("pause");	
				break;
			case 0: //  �˳�
				break;
			default ://�������
				printf("�������������룡\n");
				system("pause");
				break;			
		}
	}
	return 0;
}
