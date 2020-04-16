#include "head.h"

char filename[100] = "keywords.txt";
char token_text[TOKEN_TEXT]; //保留标识符值
pLUT Lookup[26];  //查找1表结构数组
FILE *fp; //文件指针
int rownum = 1;
int errnum = 0;
enum token_kind w;    //存储编码值
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
		printf("      请选择你的操作[0~6]:");
		scanf("%d", &op);
		switch(op){
			case 1: //	创建关键字查找表
				if(LookupTable() == ERROR) {
					printf("创建查找表失败！\n");
				}			
				else printf("创建查找表成功！\n");
				system("pause");
				break;
			case 2: //  检查关键字查找表
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
			case 3: //  词法分析
				rownum = 1;
				Lexan();
				break;
			case 4: //  语法分析
				rownum = 1;
				if((fp = fopen("test.txt", "rt")) == NULL) {  //以只读方式打开文件
					printf("测试文件打开错误！\n");    //指针为空，出错
				}
				else {
					errnum = 0;
					root = Program();
					if(errnum) printf("出错！\n");
					else if(root != NULL) printf("成功创建语法树！\n");
				}
				fclose(fp);
				system("pause");
				break;
			case 5: //  输出语法树
				if(root == NULL) printf("ERROR!\n");
				else {
					flag = 1;
					OutProgram(root);
				}
				system("pause");
				break;
			case 6: //  输出源程序
				if(root == NULL) printf("ERROR!\n");
				else {
					flag = 0;
					OutProgram(root);
				}
				system("pause");	
				break;
			case 0: //  退出
				break;
			default ://输入错误
				printf("错误，请重新输入！\n");
				system("pause");
				break;			
		}
	}
	return 0;
}
