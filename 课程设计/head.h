#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define ERROR 0
#define OVERFLOW -1

#define TOKEN_TEXT 100

typedef struct lookUp{  //���ұ�Ľṹ����
	char lookup[100];
	struct lookUp *next;
}LUT, *pLUT;
typedef struct tree{   //�﷨���ڵ�Ľṹ����
	char info[TOKEN_TEXT];
	int type;
	struct tree *right;
	struct tree *left;
	struct tree *middle;
}Tree, *pTree;

/*----- ջ��˳��洢��ʾ -----*/
#define STACK_INIT_SIZE 100   //�洢�ռ��ʼ������
#define STACKINCREMENT 10     //��������
typedef pTree SElemType;
typedef struct {     //ջ�Ķ���
	SElemType *base; //ջ��ָ��
	SElemType *top;  //ջ��ָ��
	int stacksize;   //�ѷ���洢�ռ�
}SqStack;

/*-----------------ö�ٳ���-----------------*/
//ERROR_TOKEN--����  IDENT--��ʶ��  INT_CONST--���ͳ���  ASSIGN--��ֵ
//EQ--���  UEQ--����  MORE--����  LESS--С��  MOEQ--���ڵ���  LEEQ--С�ڵ���
//LP,RP--Բ����  LSB,RSB--������  LB,RB--������  SEMI--�ֺ�  COMMA--����
//PLUS,SUBTRACT,MULTIPLY,DIVIDE--�Ӽ��˳� ENTER--����  HASH--#  
//SQM--������  DQM--˫����  PERC--�ٷֺ�  AND--&&  OR--||  DOT--��
//LCOMMENT,RCOMMENT--��ע��  COMMENT--��ע��  OP--�����  OPN--������
enum token_kind {
	ERROR_TOKEN,IDENT,INT_CONST,FLOAT_CONST,CHAR_CONST,INT,FLOAT,CHAR,IF,
	ELSE,FOR,DO,WHILE,BREAK,CONTINUE,RETURN,INCLUDE,DEFINE,EQ,UEQ,MORE,
	LESS,MOEQ,LEEQ,ASSIGN,LP,RP,LSB,RSB,LB,RB,SEMI,COMMA,PLUS,SUBTRACT,
	MULTIPLY,DIVIDE,ENTER,HASH,SQM,DQM,PERC,AND,OR,LCOMMENT,RCOMMENT,
	COMMENT,PROGRAM,EXTDEFLIST,EXTDEF,EXTVARDEF,FUNCDEF,FORMPAR,COMPSEN,
	LOCVARDEF,SENTLIST,SENTENCE,EXP,OP,OPN,PRETREATMENT,LOCDEFLIST,DOT,
};

extern char filename[100];
extern char token_text[TOKEN_TEXT]; //������ʶ��ֵ
extern pLUT Lookup[26];  //���ұ�ṹ����
extern FILE *fp; //�ļ�ָ��
extern int rownum, errnum;
int numtab = 0, numtab1 = 0; //��������
int sym = 0;	//�����ⲿ������ֲ�����
int sign = 0, flag = 0;
extern enum token_kind w;    //�洢����ֵ
enum token_kind ret, prew = -1;    //�洢����ֵ
char tokenText0[TOKEN_TEXT]; //�洢������������


/*-----------------��������-----------------*/
int gettoken();   	//���ʷ���
int got();			//���ʷ����ӳ���
int search();  		//�жϹؼ���
int LookupTable();  //�������ұ�
pTree Program(); 	//����
pTree PreTreatment();//Ԥ����
pTree ExtDefList(); //�ⲿ��������
pTree ExtDef(); 	//�ⲿ����
pTree ExtVarDef(); 	//�ⲿ��������
pTree FuncDef(); 	//��������
pTree FormPar();    //�����β�
pTree formpar();    //�����βεݹ��ӳ���
pTree CompSen();    //�������
pTree LocDefList();  //�ֲ�������������
pTree SentList();   //�������
pTree Sentence();   //���
pTree Exp(int endsym);//���ʽ
int check(int w);		//�жϲ������������
int compare(int p, int q);	//�Ƚ����ȼ���ǰ�߸��ں��߷���OK
int Destroy(pTree T);		//����
int InitStack(SqStack *S);  //������ջ
int GetTop(SqStack S, SElemType *p); //����ջ��Ԫ��
int Push(SqStack *S, SElemType p);   //����ջ��Ԫ��
int Pop(SqStack *S, SElemType *p);   //ɾ��ջ��Ԫ�ز�������ֵ
int Lexan();	//�ʷ�����
int ErrHandle();		//������
int OutProgram(pTree root); //�������
int OutExtVarDef(pTree root); //����ⲿ��������
int OutFuncDef(pTree root); //�������
int OutCompSen(pTree root); //����������
int OutSentence(pTree root);//������
int OutExp(pTree T); 	//������ʽ
int outexp(pTree T); 	//������ʽ�ݹ��ӳ���
/*-----------------��������-----------------*/
/*******************************************
  	�������ƣ�gettoken
	����������ȫ�ֱ������ַ�����token_text���ļ�ָ��fp��
	�������ܣ�����ʶ��
	����ֵ�����������
*******************************************/
int gettoken(){
	w = got();
	if(w == DEFINE) sign = 1;
	else if(w == ENTER) sign = 0;
	if(w == ERROR_TOKEN) {
		ErrHandle();
		w = gettoken(); 
		return w;
	}
	else if(prew == SEMI && w == SEMI){
		ErrHandle();
		w = gettoken(); 
		return w;			
	}
	else if(check(w) == check(prew) && check(w) != ERROR && sign == 0){
		ErrHandle();
		w = gettoken(); 
		return w;		
	} 
	if(prew == ENTER){
		while(w == ENTER) w = got();
	}
	prew = w;
	return w;
}
int got(){
	char c;
	int i = 0;
	token_text[i] = '\0';  //��ʼ���ַ�����
	while( (c=fgetc(fp)) == ' ' || c == '\n')
		if(c == '\n') {
			rownum++; 
			return ENTER;
		}
	if((c>='a' && c<='z') || (c>='A' && c<='Z')){ //��ĸ
		do{
			token_text[i] = c;
			i++;
		}while(((c=fgetc(fp))>='a'&&c<='z') || (c>='A'&&c<='Z') || (c>='0'&&c<='9'));
		ungetc(c, fp); //�˻ض����ַ����ļ�������
		token_text[i] = '\0';  //�ַ����������
		if(search() == ERROR) return IDENT;  //���ǹؼ��֣����ر�ʶ������IDENT
		else return w;                       //���عؼ��ֱ���
	}
	else if(c>='1' && c<='9'){ //����
		do{
			token_text[i] = c;
			i++;
		}while((c=fgetc(fp))>='0' && c<='9');
		if(c != '.'){
			ungetc(c, fp); //�˻ض����ַ����ļ�������
			token_text[i] = '\0';  //���ִ��������
	    	return INT_CONST;  //�����������ֱ���INT_CONST
		}
  		else{
			do{
			   token_text[i] = c;
			   i++;
			}while((c=fgetc(fp))>='0' && c<='9');
			ungetc(c, fp); //�˻ض����ַ����ļ�������
			token_text[i] = '\0';  //���ִ��������
	    	return FLOAT_CONST;  //���ظ��������ֱ���FLOAT_CONST
		}
	}
	else if(c == '0'){
		token_text[i] = c;
		i++;
		c = fgetc(fp);
		if(c>='1' && c<='9') {//�˽���
			do{
				token_text[i] = c;
				i++;
			}while((c=fgetc(fp))>='0' && c<='9');
			ungetc(c, fp); //�˻ض����ַ����ļ�������
			token_text[i] = '\0';  //���ִ��������
	    	return INT_CONST;  //�����������ֱ���INT_CONST
		}
		else if(c == 'x' || c == 'X'){//ʮ������
			do{
				token_text[i] = c;
				i++;
				c = fgetc(fp);
			}while((c>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F'));
			ungetc(c, fp); //�˻ض����ַ����ļ�������
			token_text[i] = '\0';  //���ִ��������
	    	return INT_CONST;  //�����������ֱ���INT_CONST
		}
		else if(c == '.'){//������
			do{
			   token_text[i] = c;
			   i++;
			}while((c=fgetc(fp))>='0' && c<='9');
			ungetc(c, fp); //�˻ض����ַ����ļ�������
			token_text[i] = '\0';  //���ִ��������
	    	return FLOAT_CONST;  //���ظ��������ֱ���FLOAT_CONST			
		}
		else{//0
			ungetc(c, fp); //�˻ض����ַ����ļ�������
			token_text[i] = '\0';  //���ִ��������
	    	return INT_CONST;  //�����������ֱ���INT_CONST			
		}
	}
	switch(c) {
		case '=':
			c = fgetc(fp);
			if(c == '=') return EQ;  //������ȱ���EQ
			ungetc(c, fp); //�˻ض����ַ����ļ�������
			return ASSIGN;  //���ظ�ֵ����ASSIGN
		case '>':
			c = fgetc(fp);
			if(c == '=') return MOEQ;  //���ش��ڵ��ڱ���MOEQ
			ungetc(c, fp); //�˻ض����ַ����ļ�������
			return MORE;   //���ش��ڱ���MORE
		case '<':
			c = fgetc(fp);
			if(c == '=') return LEEQ;  //����С�ڵ��ڱ���LEEQ
			ungetc(c, fp); //�˻ض����ַ����ļ�������
			return LESS;   //����С�ڱ���LESS
		case '+': return PLUS;   //���ؼӷ�����PLUS
		case '-': return SUBTRACT; //���ؼ�������SUBTRACT
		case '*':
			c = fgetc(fp);
			if(c == '/') return RCOMMENT;  //���ؿ�ע�ͱ���RCOMMENT
			ungetc(c, fp); //�˻ض����ַ����ļ�������
			return MULTIPLY; //���س˷�����MULTIPLY
		case '/':
			c = fgetc(fp);
			if(c == '/') return COMMENT;  //������ע�ͱ���COMMENT
			else if(c == '*') return LCOMMENT;  //���ؿ�ע�ͱ���LCOMMENT
			else {
				ungetc(c, fp); //�˻ض����ַ����ļ�������
				return DIVIDE; //���س�������DIVIDE
			}
		case '.':
			return DOT;  //���ص����DOT
        case '(':
        	return LP;   //������Բ����LP
        case ')':
        	return RP;   //������Բ���ű���RP
        case '[':
        	return LSB;   //���������ű���LSB
        case ']':
        	return RSB;   //�����ҷ����ű���RSB
        case '{':
        	return LB;   //���������ű���LB
        case '}':
        	return RB;   //�����һ����ű���RB
		case ';':
			return SEMI;  //���طֺű���SEMI
		case ',':
			return COMMA; //���ض��ű���COMMA
		case '#':
			return HASH; //���ؾ��ű���HASH
		case '\'':
			return SQM; //���ص����ű���SQM
		case '"':
			return DQM; //����˫���ű���DQM
		case '%':
			return PERC; //���ذٷֺű���PERC
		case '|':
			if((c=fgetc(fp)) == '|') return OR;  //���ػ����OR
			else return ERROR_TOKEN;  //���ش������ERROR_TOKEN
		case '&':
			if((c=fgetc(fp)) == '&') return AND;  //���������AND
			else return ERROR_TOKEN;  //���ش������ERROR_TOKEN
		case '!':
			if((c=fgetc(fp)) == '=') return UEQ;  //���ز��ȱ���UEQ
			else return ERROR_TOKEN;  //���ش������ERROR_TOKEN
		default:
			if(feof(fp)) return EOF;  //�ļ��Ѷ���
			else return ERROR_TOKEN;  //���ش������ERROR_TOKEN
	}
}

/*******************************************
  	�������ƣ�search
	����������ȫ�ֱ������ַ�����token_text���ṹ����Lookup��
	�������ܣ��жϹؼ���
	����ֵ��OK or ERROR
*******************************************/
int search(){
	pLUT p;
	int i;
	char ch = token_text[0];
	if(ch < 'a' || ch > 'z') return ERROR;
	i = ch - 'a';
	if(Lookup[i] == NULL) return ERROR;
	for(p = Lookup[i]; p != NULL; p = p->next)  //�ж��Ƿ�Ϊ�ؼ���
		if(strcmp(p->lookup, token_text) == 0) { //�жϹؼ�������
			switch(token_text[0]){
				case 'i': 
					if(token_text[1] == 'n'){
						if(token_text[2] == 't') w = INT;
						else w = INCLUDE;
					}
					else w = IF;
					break;
				case 'd':
					if(token_text[1] == 'o') w = DO;
					else w = DEFINE;
					break;
				case 'f':
					if(token_text[1] == 'o') w = FOR;
					else w = FLOAT;
					break;
				case 'c':
					if(token_text[1] == 'h') w = CHAR;
					else w = CONTINUE;
					break;
				case 'w': w = WHILE; break;
				case 'b': w = BREAK; break;
				case 'e': w = ELSE; break;
				case 'r': w = RETURN; break;
			} 
			return OK;
		}
	return ERROR;
}

/*******************************************
  	�������ƣ�LookupTable
	����������ȫ�ֱ������ļ�ָ��fp���ַ�����filename��
	�������ܣ��������ұ�
	����ֵ��OK or ERROR
*******************************************/
int LookupTable(){
	pLUT p, q;
	char ch, s[100];
	int i, j;
	fp = NULL;  //��ʼ��
	if((fp = fopen(filename, "rt")) == NULL) {  //��ֻ����ʽ���ļ�
		printf("�ļ��򿪴���\n");             //ָ��Ϊ�գ�����
		return ERROR;
	}
	for(i = 0; i < 26; i++) Lookup[i] = NULL;     //��ʼ��
	do {
		s[0] = '\0';
		for(j = 0; fread(&ch, sizeof(char), 1, fp); j++) { //�����ַ���
			if(ch < 'a' || ch > 'z') break;
			else s[j] = ch;
		}
		if(s[0] == '\0') continue;  //δ�����ַ���
		s[j] = '\0';
		i = s[0] - 'a';  //�ҵ��ؼ��ֶ�Ӧλ��
		if(Lookup[i] == NULL) {   //ָ��Ϊ��
			p = (pLUT)malloc(sizeof(LUT));
			strcpy(p->lookup, s);
			p->next = NULL;
			Lookup[i] = p;
		}
		else{     //ָ�벻Ϊ��
			p = (pLUT)malloc(sizeof(LUT));
			strcpy(p->lookup, s);
			q = Lookup[i];
			Lookup[i] = p;
			p->next = q;
		}
	}while(!feof(fp));  //�ļ��Ѷ�ȡ��
	fclose(fp);
	return OK;
}

/*******************************************
  	�������ƣ�Program
	����������
	�������ܣ�����
	����ֵ���﷨�������ָ��
*******************************************/
pTree Program(){
	pTree root = (pTree)malloc(sizeof(Tree));
	root->right = root->left = NULL;
	root->type = PROGRAM;

    w = gettoken();
	root->left = PreTreatment();
    root->right = ExtDefList();
//    if (root->left)
	if(errnum == 0){
//    	�����﷨��ȷ�����ص��﷨�������ָ�룬�ɱ�����ʾ
		return root;
	}
    else{
//    	���﷨����
		return NULL;
	}
}
/*******************************************
  	�������ƣ�PreTreatment
	����������
	�������ܣ�Ԥ����
	����ֵ�����������ָ��
*******************************************/
pTree PreTreatment(){
	if(w != HASH) return NULL;
	pTree root = (pTree)malloc(sizeof(Tree));
	root->right = root->left = NULL;
	root->type = PRETREATMENT;
	pTree p = (pTree)malloc(sizeof(Tree));
	p->right = p->left = NULL;
	char c;
	int i, j;
	w =  gettoken();
	if(w == INCLUDE){
		p->type = INCLUDE;
		root->left = p;
		w = gettoken();
		if(w == LESS) {
			w = gettoken();
			if(w != IDENT) {ErrHandle(); return NULL;}
			p->info[0] = '<';
			for(i = 1; token_text[i-1] != '\0'; i++) p->info[i] = token_text[i-1];
			w = gettoken();
			if(w != DOT) {ErrHandle(); return NULL;}
			p->info[i] = '.';//��Ϊ�ļ���
			i++;
			prew = -1;
			w = gettoken();
			if(w != IDENT) {ErrHandle(); return NULL;}
			for(j = 0; token_text[j] != '\0'; i++, j++) p->info[i] = token_text[j];
			p->info[i] = '>';
			p->info[i+1] = '\0';	
			w = gettoken();
			if(w != MORE) {ErrHandle(); return NULL;}					
		}
		else if(w == DQM){
			w = gettoken();
			if(w != IDENT) {ErrHandle(); return NULL;}
			p->info[0] = '\"';
			for(i = 1; token_text[i-1] != '\0'; i++) p->info[i] = token_text[i-1];
			w = gettoken();
			if(w != DOT) {ErrHandle(); return NULL;}
			p->info[i] = '.';//��Ϊ�ļ���
			i++;
			prew = -1;
			w = gettoken();
			if(w != IDENT) {ErrHandle(); return NULL;}
			for(j = 0; token_text[j] != '\0'; i++, j++) p->info[i] = token_text[j];
			p->info[i] = '\"';
			p->info[i+1] = '\0';	
			w = gettoken();
			if(w != DQM) {ErrHandle(); return NULL;}	
		}
		else {ErrHandle(); return NULL;}
		w = gettoken();
		if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}	
	}
	else if(w == DEFINE){
		sign = 1;
		p->type = DEFINE;
		root->left = p;
		w = gettoken();
		if(w != IDENT) {ErrHandle(); return NULL;}
		for(i = 0; token_text[i] != '\0'; i++) p->info[i] = token_text[i];
		p->info[i] = ' ';//��Ϊ�ָ���
		i++;
		prew = -1;
		w = gettoken();
		if(w!=INT_CONST && w!=FLOAT_CONST && w!=CHAR_CONST) {ErrHandle(); return NULL;}
		for(j = 0; token_text[j] != '\0'; i++, j++) p->info[i] = token_text[j];
		p->info[i] = '\0';	
		w = gettoken();
		if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}	
		sign = 0;
	}
	else {ErrHandle(); return NULL;}
	w = gettoken();
	root->right = PreTreatment();
	if(errnum == 0) return root;
	else return NULL;
}
/*******************************************
  	�������ƣ�skip
	������������
	�������ܣ�������һ��
	����ֵ����
*******************************************/
void skip(){
	while(1){
		w = got();
		if(w == ENTER) break;
	}
}

/*******************************************
  	�������ƣ�ExtDefList
	����������
	�������ܣ��ⲿ��������
	����ֵ�����������ָ��
*******************************************/
pTree ExtDefList(){
	//�����ⲿ�������У���ȷʱ���������������ָ�룬���������
    if (w == EOF) return NULL; //�ļ�����
	if(w != INT && w != FLOAT && w != CHAR) return NULL;
    //����һ���ⲿ�������н��root
    pTree root = (pTree)malloc(sizeof(Tree));
	root->right = root->left = NULL;
	if(sym == 0)root->type = EXTDEFLIST;
	else root->type = LOCDEFLIST;
    //����һ���ⲿ���壬�õ�һ����������Ϊroot�ĵ�һ������
    root->left = ExtDef();
	//����һ���ⲿ�������У��õ�����������Ϊroot�ĵڶ�������
    root->right = ExtDefList();
	if(errnum == 0) return root;
	else return NULL;
}

/*******************************************
  	�������ƣ�ExtDef
	����������
	�������ܣ��ⲿ����
	����ֵ�����������ָ��
*******************************************/
pTree ExtDef(){
	//�����ⲿ���壬��ȷʱ���������������ָ�룬���򷵻�NULL
	//����һ���ⲿ������root
    pTree root = (pTree)malloc(sizeof(Tree));
	root = NULL;
	//���ǹؼ��֣���������NULL
    if (w != INT && w != FLOAT && w != CHAR && w != IDENT){
		ErrHandle();
    	return NULL;
	}
	ret = w; //��������˵����
    w = gettoken();
    //���Ǳ�ʶ������������NULL
    if (w != IDENT ){
		ErrHandle();
    	return NULL;
	}
	strcpy(tokenText0, token_text); //�����һ����������������tokenText0

    w = gettoken();
    if (w != LP) root = ExtVarDef();  //�����ⲿ���������ӳ���
    else root = FuncDef();    //���ú��������ӳ���
    //�������ֵroot�ǿգ���ʾ�ɹ����һ���ⲿ����Ĵ���
	if(errnum == 0) return root;
	else return NULL;
}

/*******************************************
  	�������ƣ�ExtVarDef
	����������
	�������ܣ��ⲿ��������
	����ֵ�����������ָ��
*******************************************/
pTree ExtVarDef(){
	int i, j;
	//�����ⲿ��������ڵ�root
    pTree root = (pTree)malloc(sizeof(Tree));
	root->right = root->left = NULL;
	if(sym == 0) root->type = EXTVARDEF;
	else root->type = LOCVARDEF;
	//�����ⲿ�������ͽ��m����Ϊroot�ĵ�һ������
	pTree m = (pTree)malloc(sizeof(Tree));
	m->left = m->right = NULL;
	m->type = ret;
	root->left = m;
	//�����ⲿ�������н��p����Ϊroot�ĵڶ�������
	//ÿ���ⲿ�������н����Ӧһ��������
	pTree p = (pTree)malloc(sizeof(Tree));
	p->left = p->right = NULL;
	root->right = p;
	//�ɱ�����tokenText0�ĵ�һ������������һ�����������n����Ϊp�ĵ�һ������
	pTree n = (pTree)malloc(sizeof(Tree));
	n->left = n->right = NULL;
	n->type = IDENT;
	for(i = 0; tokenText0[i] != '\0'; i++) n->info[i] = tokenText0[i];
	if(w == LSB){	//��������
		w = gettoken();
		if(w != INT_CONST) {ErrHandle(); return NULL;}
		n->info[i] = '['; i++;
		for(j = 0; token_text[j] != '\0'; i++, j++) n->info[i] = token_text[j];
		w = gettoken();
		if(w != RSB) {ErrHandle(); return NULL;}
		n->info[i] = ']'; i++;
		w = gettoken();
	}
	n->info[i] = '\0';
	p->left = n;
	//ÿ���ⲿ�������н��ĵ�һ�����Ӷ�Ӧһ������
	while(1) {
		//����Ƕ��Ż�ֺţ������ͷ�rootΪ����ȫ����㣬���ؿ�ָ��
		if (w != COMMA && w != SEMI){
			ErrHandle();
			Destroy(root);
			return NULL;
		}
		else if (w == SEMI){
			w = gettoken();
			if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
			w = gettoken();
			if(errnum == 0) return root;//���ظ����root��
			else return NULL;  
		}
		w = gettoken(); //�������������
		//w���Ǳ�ʶ���������ͷ�rootΪ����ȫ����㣬���ؿ�ָ��
		if(w != IDENT) {
			ErrHandle();
			Destroy(root);
			return NULL;
		}
		//�����ⲿ�������н��q����Ϊp�ĵڶ������ӣ����뵽����
  		pTree q = (pTree)malloc(sizeof(Tree));
		q->right = q->left = NULL;
		q->type = EXTDEFLIST;
		p->right = q;
		//����token_text�ı���������һ��������㣬��Ϊq�ĵ�һ������
		pTree n = (pTree)malloc(sizeof(Tree));
		n->left = n->right = NULL;
		n->type = IDENT;
		for(i = 0; token_text[i] != '\0'; i++) n->info[i] = token_text[i];
		w = gettoken(); //�������������
		if(w == LSB){ //��������
			w = gettoken();
			if(w != INT_CONST) {ErrHandle(); return NULL;}
			n->info[i] = '['; i++;
			for(j = 0; token_text[j] != '\0'; i++, j++) n->info[i] = token_text[j];
			w = gettoken();
			if(w != RSB) {ErrHandle(); return NULL;}
			n->info[i] = ']'; i++;
			w = gettoken();
		}
		n->info[i] = '\0';
		q->left = n;
		p = q;
	    
	}
	if(errnum == 0) return root;
	else return NULL;
}

/*******************************************
  	�������ƣ�FuncDef
	����������
	�������ܣ���������
	����ֵ�����������ָ��
*******************************************/
pTree FuncDef(){
	if(errnum) return NULL;
	//���ɺ���������root
	pTree root = (pTree)malloc(sizeof(Tree));
	root->right = root->left = root->middle = NULL;
	root->type = FUNCDEF;
	//���ɷ���ֵ���ͽ�㣬��Ϊroot�ĵ�һ������
	pTree m = (pTree)malloc(sizeof(Tree));
	m->left = m->right = m->middle = NULL;
	m->type = ret;
	root->left = m;
	//����������ֵ���С���Ž����������β��ӳ��򣬵õ��������ֵ�������ָ��
	//�޲κ����õ�NULL����������Ϊroot�ĵڶ�������
	//�ɱ�����tokenText0�ĵ�һ������������һ�����������n
	pTree n = (pTree)malloc(sizeof(Tree));
	n->left = n->right = n->middle = NULL;
	n->type = IDENT;
	strcpy(n->info, tokenText0);
	root->middle = n;
	n->middle = FormPar();
	//������ţ�����ֺţ����Ǻ���ԭ������������������Ϊ�գ�
	if(w == SEMI) {
		root->right = NULL;
		w = gettoken();
		if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
	}
	//�������ţ�����ú����壨������䣩�ӳ��򣬵õ�������������ָ�룬
	else if(w == LB){
		w = gettoken();
		if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
		root->right = CompSen();

	}
	//�������ű�ʾ�д��õ��ĺ�����������Ϊroot�ĵ���������
	else {ErrHandle(); return NULL;}
	w = gettoken();
	if(errnum == 0) return root;
	else return NULL;  //����root��
}

/*******************************************
  	�������ƣ�FormPar
	����������
	�������ܣ������β�
	����ֵ�����������ָ��
*******************************************/
pTree FormPar(){
	if(errnum) return NULL;
	w = gettoken();
	if(w == RP) return NULL; //���β�
    pTree root = (pTree)malloc(sizeof(Tree));
    pTree p = (pTree)malloc(sizeof(Tree));
	root = formpar();
	p = root;

	w = gettoken(); //��������β�
	//ÿ���β����н��ĵ�һ�����Ӷ�Ӧһ���β�
	while(1) {
		//����Ƕ��Ż����ţ������ͷ�rootΪ����ȫ����㣬���ؿ�ָ��
		if (w == COMMA){
			w = gettoken(); //��������β�
			p->right = formpar();
			p = p->right;
			w = gettoken(); //�������������

		}
		else if (w == RP){
			w = gettoken();
			if(errnum == 0) return root;//���ظ����root��
			else return NULL;   
		}
		else{
			ErrHandle();
			Destroy(root);
			return NULL;			
		}

	}
}
/*******************************************
  	�������ƣ�formpar
	����������
	�������ܣ������βθ����ӳ���
	����ֵ�����������ָ��
*******************************************/
pTree formpar(){
	//���ǹؼ��֣���������NULL
    if (w != INT && w != FLOAT && w != CHAR){
		ErrHandle();
    	return NULL;
	}
	ret = w; //��������˵����
    w = gettoken();
    //���Ǳ�ʶ������������NULL
    if (w != IDENT ){
		ErrHandle();
    	return NULL;
	}

	//�����β����нڵ�root
    pTree root = (pTree)malloc(sizeof(Tree));
	root->right = root->left = NULL;
	root->type = FORMPAR;
	//�����βν��m����Ϊroot�ĵ�һ������
	pTree m = (pTree)malloc(sizeof(Tree));
	m->left = m->right = NULL;
	m->type = FORMPAR;
	root->left = m;
	//�����β����ͽ��p����Ϊm�ĵ�һ������
	pTree p = (pTree)malloc(sizeof(Tree));
	p->left = p->right = NULL;
	p->type = ret;
	m->left = p;
	//�ɱ�����token_text�ı���������һ�����������q����Ϊm�ĵڶ�������
	pTree q = (pTree)malloc(sizeof(Tree));
	q->left = q->right = NULL;
	q->type = IDENT;
	strcpy(q->info, token_text);
	m->right = q;

	if(errnum == 0) return root;
	else return NULL;
}

/*******************************************
  	�������ƣ�CompSen
	����������
	�������ܣ��������
	����ֵ�����������ָ��
*******************************************/
pTree CompSen(){
	if(errnum) return NULL;
	//���ɸ�������㣬ע�����оֲ�����˵����������ж�����Ϊ��
    pTree root = (pTree)malloc(sizeof(Tree));
	root->right = root->left = NULL;
	root->type = COMPSEN;

	w = gettoken();
	if(w == INT || w == FLOAT || w == CHAR){ //w�����͹ؼ���
		ret = w; //��������˵����
		//���ô���ֲ�����˵�������ӳ��򣬵õ����ص������������Ϊroot�ĵ�һ������
		root->left = LocDefList();
	}
	else {
		//�޾ֲ�����˵����root�ĵ�һ����������Ϊ��ָ��
		root->left = NULL;
	}
	//���ô�����������ӳ��򣬷������������ָ�룬��Ϊroot�ĵ�2������
	root->right = SentList();

    if(w != RB){ //w���Ƿ�������}���ؿ�ָ�룬�����ͷŽ��
		ErrHandle();
		Destroy(root);
		return NULL;
	}
    w = gettoken();
	//���ظ�������������ָ��
	if(errnum == 0) return root;
	else return NULL;
}

/*******************************************
  	�������ƣ�LocDefList
	����������
	�������ܣ��ֲ�������������
	����ֵ�����������ָ��
*******************************************/
pTree LocDefList(){
	if(errnum) return NULL;
    pTree root = (pTree)malloc(sizeof(Tree));	
	root = NULL;
	sym = 1;
	root = ExtDefList();
	if(errnum == 0) return root;
	else return NULL;
}

/*******************************************
  	�������ƣ�SentList
	����������
	�������ܣ��������
	����ֵ�����������ָ��
*******************************************/
pTree SentList(){
	if(errnum) return NULL;
	//��ʼ����������ָ��root=NULL��
	//����������н��
    pTree root = (pTree)malloc(sizeof(Tree));
    root->left = root->middle = root->right = NULL;
    pTree p = (pTree)malloc(sizeof(Tree));
    p = NULL;
	//���ô���һ�������ӳ��򣻷�����������ָ��r1��
	p = Sentence();
	if(p == NULL) {
	//û�з�����1����䣬errors>0ʱ������󣬷����ʾ��������ѽ���,����NULL
		// if(errors > 0){    //�������

		// }
		// else return NULL;
		return NULL;
	}
	else {
		//����������еĽ��root
	    root->left = p;
		root->right = SentList();  //�ݹ���ô�����������ӳ����ķ���ֵ��
		if(errnum == 0) return root;//����root��
		else return NULL; 
	}
}

/*******************************************
  	�������ƣ�Sentence
	����������
	�������ܣ����
	����ֵ�����������ָ��
*******************************************/
pTree Sentence(){
	if(errnum) return NULL;
    pTree root = (pTree)malloc(sizeof(Tree));
    root->left = root->right = root->middle = NULL;
	pTree p = (pTree)malloc(sizeof(Tree));
	p->left = p->middle = p->right = NULL;
	switch (w){
  		case IF://�����������
		 	root->type = w;
			w = gettoken();
			if (w != LP){ //w������С���ű������ؿ�
				ErrHandle();
				return NULL;
			}
			//	���ô�����ʽ���ӳ��򣨽�������Ϊ��С���ţ�
			//	��ȷʱ�õ��������ʽ�������ָ��
			w = gettoken();
			root->left = p;
			p->type = EXP;
			p->middle = Exp(RP);
			//	���ô���һ�������ӳ��򣬵õ�IF�Ӿ��������ָ�롣
			if(w != LB) {ErrHandle(); return NULL;}
			w = gettoken();
			if(w != ENTER) {ErrHandle(); return NULL;}
			w = gettoken();
			root->middle = SentList();
			if(w != RB) {ErrHandle(); return NULL;}
			w = gettoken();	
			if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
			w = gettoken();			
			if(w == ELSE){
			//���ô���һ�������ӳ��򣬵õ�IF�Ӿ��������ָ�롣
			//����IF-ELSE��㣬�¹�������IF�Ӿ䡢ELSE�Ӿ�3������
				w = gettoken();
				if(w != LB) {ErrHandle(); return NULL;}
				w = gettoken();
				if(w != ENTER) {ErrHandle(); return NULL;}
				w = gettoken();
				root->right = SentList();
				if(w != RB) {ErrHandle(); return NULL;}
				w = gettoken();	
				if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
				w = gettoken();	
			}
			else {
			//����IF��㣬�¹�������IF�Ӿ�2������
				root->right = NULL;
			}
			if(errnum == 0) return root;
			else return NULL;
   		case WHILE:  //����while���
		   	root->type = w;
			w = gettoken();
			if (w != LP){ //w������С���ű������ؿ�
				ErrHandle();
				return NULL;
			}
			//	���ô�����ʽ���ӳ��򣨽�������Ϊ��С���ţ�
			//	��ȷʱ�õ��������ʽ�������ָ��
			w = gettoken();
			root->left = p;
			p->type = EXP;
			p->middle = Exp(RP);
			if(w != LB) {ErrHandle(); return NULL;}
			w = gettoken();
			if(w != ENTER) {ErrHandle(); return NULL;}
			w = gettoken();
			//	���ô���һ�������ӳ��򣬵õ�WHILE�Ӿ��������ָ�롣
			root->middle = SentList();
			if(w != RB) {ErrHandle(); return NULL;}
			w = gettoken();
			if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
			break;
		case FOR:
			root->type = w;
			w = gettoken();
			if (w != LP){ //w������С���ű������ؿ�
				ErrHandle();
				return NULL;
			}
			//	���ô�����ʽ���ӳ���
			//	��ȷʱ�õ������������ʽ�������ָ��
			root->left = p;
			p->type = EXP;
			w = gettoken();
			p->left = Exp(SEMI);
			p->middle = Exp(SEMI);
			p->right = Exp(RP);
			//	���ô���һ�������ӳ��򣬵õ�WHILE�Ӿ��������ָ�롣
			if(w != LB) {ErrHandle(); return NULL;}
			w = gettoken();
			if(w != ENTER) {ErrHandle(); return NULL;}
			w = gettoken();
			root->middle = SentList();
			if(w != RB) {ErrHandle(); return NULL;}
			w = gettoken();	
			if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
			break;
		case LP:      //���ֱ��ʽ��䣬����ֵ�ȣ���ʽΪ���ʽ�ԷֺŽ���
			root->type = EXP; 
			root->middle = Exp(SEMI);
			break;
		case IDENT:
		case INT_CONST:
  		case FLOAT_CONST:
		case CHAR_CONST:
  			//���ñ��ʽ�����ӳ��򣨽�������Ϊ�ֺţ�
			//��ȷʱ��w=gettoken();  ���ر��ʽ��������ĸ����ָ��
			root->type = EXP; 
			root->middle = Exp(SEMI);
			if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
			break;
  		//���ô���������ӳ���,���صõ���������ָ��
		case LB: root = CompSen(); break;
		//������н������ţ��������֧��switch��䣬�������Ż�����case��default
		case RB: case SEMI: 
			return NULL;
		case CONTINUE:
			root->type = CONTINUE; 
			w = gettoken();
			if(w != SEMI) {ErrHandle(); return NULL;}
			w = gettoken();
			if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
			break;
		case BREAK:
			root->type = BREAK; 
			w = gettoken();
			if(w != SEMI) {ErrHandle(); return NULL;}
			w = gettoken();
			if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
			break;
		case RETURN:
			root->type = RETURN;
			w = gettoken();
			root->middle = Exp(SEMI);
			if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
			break;
		case ENTER:
			root = Sentence();
			break;
		default:
		    ErrHandle();
    	   	return NULL;	//��������NULL
	}
	w = gettoken();
	if(errnum == 0) return root;
	else return NULL;
}

/*******************************************
  	�������ƣ�Exp
	����������ö�ٳ���
	�������ܣ����ʽ
	����ֵ�����������ָ��
*******************************************/
//���ָ������  exp(int endsym)
//���ʽ��������endsym�����Ƿֺţ�����ʽ��䣬�����Ƿ�С���ţ���Ϊ����ʱʹ��
pTree Exp(int endsym){  //���ø��㷨ʱ���ڵ������Ѿ������˵�һ��������w��
	int numlp = 0; //δ��Ե������ż���
	int i, j;
	char c;
   	SqStack op, opn;
    InitStack(&op); //�����������ջ
    InitStack(&opn); //�����ղ�����ջ
	pTree q = (pTree)malloc(sizeof(Tree));
    q->left = q->right = q->middle = NULL;
	pTree m = (pTree)malloc(sizeof(Tree));
    m->left = m->right = m->middle = NULL;
	pTree n = (pTree)malloc(sizeof(Tree));
    n->left = n->right = n->middle = NULL;
    while(1){  //����׺���ʽת���ɺ�׺���ʽ���洢�ڲ�����ջ��
		pTree p = (pTree)malloc(sizeof(Tree)); //�������
    	p->left = p->right = p->middle = NULL;
    	p->type = w;
		for(i = 0; token_text[i] != '\0'; i++) p->info[i] = token_text[i];
		if(w == IDENT){
			c = fgetc(fp); 
			if(c == '[') {
				p->info[i] = '['; i++;
				sign = 1;
				w = gettoken();
				if(w != INT_CONST) {ErrHandle(); return NULL;}
				for(j = 0; token_text[j] != '\0'; i++, j++)
					p->info[i] = token_text[j];
				c = fgetc(fp);
				if(c != ']')  {ErrHandle(); return NULL;}
				p->info[i] = ']'; i++;
				sign = 0;
			}
			else{
				ungetc(c, fp);
			}
		}
		p->info[i] = '\0';
		if(check(w) == OPN) {  //������������������ջ
			p->type = OPN;
	  		Push(&opn, p);
   		}
   		else if(check(w) == OP){  //�����
   			//������������������ջ�����ȼ������ڸ�������Ľ��������ջ
			while(1){
				if(GetTop(op, &q) ==ERROR) break; //ջ����Ԫ��
				if(compare(p->type, q->type) == OK) break;//p���ȼ�����q
				else { //p���ȼ����ڻ����q
					Pop(&op, &q);
				 	Push(&opn, q);
				}
			}
			Push(&op, p);
		}
   		else if(w == LP){ //�����ţ�����������ջ
			 Push(&op, p);
			 numlp++;
		}
   		else if(w == RP){ //�����ţ��������ջ���������ŵ���������������ջ
        	if(endsym == RP && numlp == 0){ //��������Ϊ��������û��δ��Ե�������
     			while(op.base != op.top) { //�����ջ����Ԫ�ؽ��������ջ
   					Pop(&op, &q);
   					Push(&opn, q);
				}
				break;
			}
			else if(endsym == SEMI && numlp == 0){//��������Ϊ�ֺ���û��δ��Ե�������
		    	 ErrHandle();
    	   		 return NULL;
			}
			while(1){     //��δ��Ե�������
   				Pop(&op, &q);
   				if(q->type == LP) break;
   				else Push(&opn, q);
			}
            numlp--;
		}
    	else if(w == endsym) { //�ֺŽ����ַ�
    		while(op.base != op.top) { //�����ջ����Ԫ�ؽ��������ջ
   				Pop(&op, &q);
   				Push(&opn, q);
			}
			break;
		}
		else { //��������NULL
	 		ErrHandle();
    	   	return NULL;
		}
		w = gettoken();
	}
	while (opn.base != opn.top)
	{//��������ջ�еĺ�׺���ʽ���������������ջ��
		Pop(&opn, &m);
		Push(&op, m);
	}
	while(op.base != op.top){//������׺���ʽ�Ķ�����
		Pop(&op, &m);
		if(m->type == OPN) Push(&opn, m);//��������ֱ�ӽ�ջ
		else {
			Pop(&opn, &q);
			Pop(&opn, &n);
			m->right = q;
			m->left = n;
			Push(&opn, m);
		}
	}
	w = gettoken(); 
	if(errnum == 0) return m;
	else return NULL;
}
int check(int w) {  //�жϲ������������
	switch(w){
		case EQ:	case UEQ: 	case MORE: 	case LESS: 		case MOEQ:
		case LEEQ:	case ASSIGN:case PLUS: 	case SUBTRACT: 	case MULTIPLY:
		case DIVIDE:case PERC: 	case OR: 	case AND:
        	return OP;   //�����
		case IDENT: case INT_CONST: case FLOAT_CONST: case CHAR_CONST:
			return OPN;  //������
        default :
			return ERROR;//�Ƿ��ַ�
	}
}
int compare(int p, int q){ //�Ƚ����ȼ���ǰ�߸��ں��߷���OK
	if(p == ASSIGN) {//��ֵ���ȼ����
		if(q == ASSIGN) return OK; //ͬ�� ���ҵ���
		else return ERROR;
	} 
	if(q == ASSIGN || q == LP) return OK;//��ֵ�����������ȼ����
	switch(p){
		case EQ: case UEQ:
			 if(q == AND || q == OR) return OK;
			 else return ERROR;
		case MORE: case LESS: case MOEQ: case LEEQ:
			if(q == EQ || q == UEQ || q == OR || q == AND) return OK;
			else return ERROR;
		case PLUS:
		case SUBTRACT:
			if(q == MULTIPLY || q == DIVIDE || q == PERC || q == PLUS || q == SUBTRACT) return ERROR;
			else return OK;

		case MULTIPLY: case DIVIDE: case PERC:
			if(q == MULTIPLY || q == DIVIDE || q == PERC) return ERROR;
			else return OK;
		case OR: return ERROR;
		case AND:
			 if(q == OR) return OK;
			 else return ERROR;
	}
}
/*******************************************
  	�������ƣ�Destroy
	���������������ָ��
	�������ܣ�����
	����ֵ�����������ָ��
*******************************************/
int Destroy(pTree T) {
	if(!T) return ERROR;
	if(T->left) Destroy(T->left); //�ݹ�
	if(T->right) Destroy(T->right);
	free(T);       //�ͷ�Tָ��Ľ��洢�ռ�
	T = NULL;      //TΪ��ָ��
	return OK;
}
/*******************************************
  	�������ƣ�InitStack
	����������ջSָ��
	�������ܣ�������ջ
	����ֵ��OK or ERROR
*******************************************/
int InitStack(SqStack *S) {
	S->base = (SElemType*)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if(!S->base) exit(OVERFLOW); //�洢�ռ����ʧ��
	S->top = S->base;  //ջ��ָ�����ջ��ָ�룬����ջ
	S->stacksize = STACK_INIT_SIZE; //���´洢�ռ�
	return OK;
}
/*******************************************
  	�������ƣ�GetTop
	����������ջS��ջԪ��ָ��
	�������ܣ�����ջ��Ԫ��
	����ֵ��OK or ERROR
*******************************************/
int GetTop(SqStack S, SElemType *p) {
	if(S.base == S.top) return ERROR; //ջΪ�գ���Ԫ��
	*p = *(S.top-1);  //����ջ��Ԫ��ֵ
	return OK;
}
/*******************************************
  	�������ƣ�Push
	����������ջSָ�룬ջԪ��
	�������ܣ�����ջ��Ԫ��
	����ֵ��OK or ERROR
*******************************************/
int Push(SqStack *S, SElemType p) {
	if(S->top - S->base >= S->stacksize) { //ջ�洢�ռ�����
		S->base = (SElemType*)realloc(S->base, (S->stacksize+STACKINCREMENT)*sizeof(SElemType));
		if(!S->base) exit(OVERFLOW); //�洢�ռ����ʧ��
		S->top = S->base + S->stacksize;  //����ջ��ָ��
		S->stacksize += STACKINCREMENT; //���´洢�ռ�
	}
	*S->top = p;  //��Ԫ�ز���ջ��
	S->top++;     //ջ��ָ������
	return OK;
}
/*******************************************
  	�������ƣ�Pop
	����������ջSָ�룬ջԪ��ָ��
	�������ܣ�ɾ��ջ��Ԫ��
	����ֵ��OK or ERROR
*******************************************/
int Pop(SqStack *S, SElemType *p) {
	if(S->base == S->top) return ERROR; //ջΪ�գ���ջ��Ԫ��
	S->top--;  //ջ��ָ���Լ�
	*p = *(S->top); //����ջ��Ԫ�ص�ֵ
	return OK;
}
/*******************************************
  	�������ƣ�Lexan
	����������
	�������ܣ��ʷ�����
	����ֵ��OK or ERROR
*******************************************/
int Lexan(){
	int ret;
 	if((fp = fopen("test.txt", "rt")) == NULL) {  //��ֻ����ʽ���ļ�
		printf("�����ļ��򿪴���\n");    //ָ��Ϊ�գ�����
		return ERROR;
	}
	ret = rownum;  //��ʼ��ret
	printf("��%d��\n", rownum);
	do{
		w = gettoken();
		if(rownum > ret){
			printf("��%d��\n", rownum);
			ret = rownum;
		}
		switch(w) {
			case INT: printf("�ؼ���: int\n"); break;
			case FLOAT: printf("�ؼ���: float\n"); break;
			case CHAR: printf("�ؼ���: char\n"); break;
			case IF: printf("�ؼ���: if\n"); break;
			case ELSE: printf("�ؼ���: else\n"); break;
			case DO: printf("�ؼ���: do\n"); break;
			case WHILE: printf("�ؼ���: while\n"); break;
			case FOR: printf("�ؼ��֣�for\n"); break;
			case BREAK: printf("�ؼ���: break\n"); break;
			case CONTINUE: printf("�ؼ���: continue\n"); break;
			case RETURN: printf("�ؼ���: return\n"); break;
			case INCLUDE: printf("�ؼ���: include\n"); break;
			case DEFINE: printf("�ؼ���: define\n"); break;

			case IDENT: printf("��ʶ��: %s\n", token_text); break;
			case INT_CONST: printf("��������: %s\n", token_text); break;
			case FLOAT_CONST: printf("����������: %s\n", token_text); break;
			case CHAR_CONST: printf("�ַ�������: %s\n", token_text); break;

			case EQ: printf("���:   ==\n"); break;
			case UEQ: printf("����:   !=\n"); break;
			case MORE: printf("����:   >\n"); break;
			case LESS: printf("С��:   <\n"); break;
			case MOEQ: printf("���ڵ���: >=\n"); break;
			case LEEQ: printf("С�ڵ���: <=\n"); break;
			case ASSIGN: printf("��ֵ:   =\n"); break;

			case PLUS: printf("�Ӻ�:   +\n"); break;
			case SUBTRACT: printf("����:   -\n"); break;
			case MULTIPLY: printf("�˺�:   *\n"); break;
			case DIVIDE: printf("����:   /\n"); break;

			case LP: printf("��Բ��: (\n"); break;
			case RP: printf("��Բ��: )\n"); break;
			case LSB: printf("����: [\n"); break;
			case RSB: printf("�ҷ���: ]\n"); break;
			case LB: printf("����: {\n"); break;
			case RB: printf("�һ���: }\n"); break;
			case SEMI: printf("�ֺ�:   ;\n"); break;
			case COMMA: printf("����:   ,\n"); break;
			case HASH: printf("����:   #\n"); break;
			case SQM: printf("������: '\n"); break;
			case DQM: printf("˫����: \"\n"); break;
			case PERC: printf("�ٷֺ�: %%\n"); break;
			case OR: printf("��:     ||\n"); break;
			case AND: printf("��:     &&\n"); break;
			case DOT: printf("�㣺    .\n"); break;
			case ENTER: break;
			default :
				if(feof(fp)) break;  //�ļ��Ѷ���
				else printf("�������");  //�������
				break;
		}
	}while(!feof(fp));  //�ļ���ȡ��
	fclose(fp);
	system("pause");
	return OK;
}
/*******************************************
  	�������ƣ�ErrHandle
	����������
	�������ܣ�������
	����ֵ��OK or ERROR
*******************************************/
int ErrHandle(){
	errnum++;
	if(errnum != 1) return ERROR;
	printf("��%d�� ��", rownum);
	switch(w) {
		case INT: case FLOAT: case CHAR: case IF: case ELSE: case DO: case WHILE:
		case FOR: case BREAK: case CONTINUE: case RETURN: case INCLUDE: case DEFINE:
		case IDENT: case INT_CONST: case FLOAT_CONST: case CHAR_CONST:
			printf("%s\n", token_text); break;
		case EQ: printf("==\n"); break;
		case UEQ: printf("!=\n"); break;
		case MORE: printf(">\n"); break;
		case LESS: printf("<\n"); break;
		case MOEQ: printf(">=\n"); break;
		case LEEQ: printf("<=\n"); break;
		case ASSIGN: printf("=\n"); break;

		case PLUS: printf("+\n"); break;
		case SUBTRACT: printf("-\n"); break;
		case MULTIPLY: printf("*\n"); break;
		case DIVIDE: printf("/\n"); break;

		case LP: printf("(\n"); break;
		case RP: printf(")\n"); break;
		case LSB: printf("[\n"); break;
		case RSB: printf("]\n"); break;
		case LB: printf("{\n"); break;
		case RB: printf("}\n"); break;
		case SEMI: printf(";\n"); break;
		case COMMA: printf(",\n"); break;
		case HASH: printf("#\n"); break;
		case SQM: printf("'\n"); break;
		case DQM: printf("\"\n"); break;
		case PERC: printf("%%\n"); break;
		case OR: printf("||\n"); break;
		case AND: printf("&&\n"); break;
		case DOT: printf(".\n"); break;
		case ENTER: printf("��βȱ��\n"); break;
		case EOF: printf("�ļ��Ѷ�ȡ��\n"); break;
		default :
			if(feof(fp)) break;  //�ļ��Ѷ���
			else printf("�������\n");  //�������
			break;
	}
	skip();
	return OK;
}
/*******************************************
  	�������ƣ�OutProgram
	�����������������
	�������ܣ������������﷨��
	����ֵ��OK or ERROR
*******************************************/
int OutProgram(pTree root){
	if(root == NULL) return ERROR;
	int i, j; char c;
	switch(root->type){
		case PROGRAM:
		case PRETREATMENT:
		case EXTDEFLIST:
		case LOCDEFLIST:
			OutProgram(root->left);
			OutProgram(root->right);
			break;
		case EXTVARDEF: //����ⲿ��������
		case LOCVARDEF: //����ֲ���������
			OutExtVarDef(root); break;
		case FUNCDEF: //�������
			OutFuncDef(root); break; 
		case INCLUDE: //����ļ�����ָ��
			if(flag){
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("�ļ�����ָ�\n"); numtab1++;
				for(i = 0; i < numtab; i++) printf("    ");//����
				printf("�ļ�����%s\n", root->info);
				numtab1--;
			}
			else printf("#include %s\n", root->info); break;
		case DEFINE: ///����궨��ָ��
			if(flag){
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("�궨��ָ�\n"); numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("��ʶ����");
				for(j = 0; root->info[j] != ' '; j++){
					printf("%c", root->info[j]);
				}
				printf("\n");
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("��ֵ��");
				for(j++; root->info[j] != '\0'; j++){
					printf("%c", root->info[j]);
				}
				printf("\n");
				numtab1--;
			}
			else printf("#define %s\n", root->info);
			break;
		case EXP: //������ʽ
			if(flag){ //����﷨��
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("���ʽ��");
				OutExp(root->middle);
				printf("\n");
			}
			else OutExp(root->middle); //�������
			break;
	}
	return OK;
}
/*******************************************
  	�������ƣ�OutExtVarDef
	�����������������
	�������ܣ�����ⲿ���������ֲ������������ߵ��﷨��
	����ֵ��OK or ERROR
*******************************************/
int OutExtVarDef(pTree root){
	int i;
	pTree p = (pTree)malloc(sizeof(Tree));
	pTree q = (pTree)malloc(sizeof(Tree));
	if(flag){ //����﷨��
		for(i = 0; i < numtab1; i++) printf("    ");//����
		if(root->type = EXTVARDEF) printf("�ⲿ�������壺\n");
		else if(root->type = LOCVARDEF) printf("�ֲ��������壺\n");
		numtab1++;
		for(i = 0; i < numtab1; i++) printf("    ");//����
		printf("���ͣ�");
		p = root->left;	
		switch(p->type){ //�����������
			case INT: printf("int \n"); break;
			case FLOAT: printf("float \n"); break;
			case CHAR: printf("char \n"); break;
		}
		for(i = 0; i < numtab1; i++) printf("    ");//����		
		printf("��������\n"); numtab1++;
		for(p = root->right; p != NULL; p = p->right){
			q = p->left;	//���������
			for(i = 0; i < numtab1; i++) printf("    ");//����
			printf("ID: ");
			printf("%s \n", q->info);
		}	
		numtab1--; numtab1--;
	}
	else{	//�������
		for(i = 0; i < numtab; i++) printf("    ");//����
		p = root->left;	
		switch(p->type){ //�����������
			case INT: printf("int "); break;
			case FLOAT: printf("float "); break;
			case CHAR: printf("char "); break;
		}
		for(p = root->right; p != NULL; p = p->right){
			q = p->left;	//���������
			printf("%s", q->info);
			if(p->right != NULL) printf(", ");
		}
		printf(";\n");
	}
	return OK;
}
/*******************************************
  	�������ƣ�OutFuncDef
	�����������������
	�������ܣ�������������﷨��
	����ֵ��OK or ERROR
*******************************************/
int OutFuncDef(pTree root){
	int i = 0;
	pTree p = (pTree)malloc(sizeof(Tree));
	pTree q = (pTree)malloc(sizeof(Tree));
	pTree m = (pTree)malloc(sizeof(Tree));
	if(flag){
		for(i = 0; i < numtab1; i++) printf("    ");//����
		p = root->right;
		if(p == NULL) printf("����������\n");//��������
		else printf("�������壺\n");
		numtab1++;
		for(i = 0; i < numtab1; i++) printf("    ");//����
		printf("���ͣ�");
		p = root->left;
		switch(p->type){	//�����������
			case INT: printf("int \n"); break;
			case FLOAT: printf("float \n"); break;
			case CHAR: printf("char \n"); break;
		}
		for(i = 0; i < numtab1; i++) printf("    ");//����
		printf("��������");
		p = root->middle;
		printf("%s \n", p->info); //���������
		for(i = 0; i < numtab1; i++) printf("    ");//����
		printf("�����βΣ�\n");
		p = p->middle;
		if(p == NULL) printf("\n"); //���β�
		else{
			numtab1++;
			for(; p != NULL; p = p->right){
				q = p->left;
				m = q->left;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("���ͣ�");//����β�
				switch(m->type){ //����β�����
					case INT: printf("int    "); break;
					case FLOAT: printf("float    "); break;
					case CHAR: printf("char    "); break;				
				}
				printf("��������");
				m = q->right; //����β���
				printf("%s\n", m->info); 
			}
			numtab1--;
		}
		p = root->right;
		if(p != NULL) OutCompSen(p);
		numtab1--;
	}
	else{	
		for(i = 0; i < numtab; i++) printf("    ");//����
		p = root->left;
		switch(p->type){	//�����������
			case INT: printf("int "); break;
			case FLOAT: printf("float "); break;
			case CHAR: printf("char "); break;
		}
		p = root->middle;
		printf("%s", p->info); //���������
		p = p->middle;
		if(p == NULL) printf("()"); //���β�
		else {
			printf("(");//����β�
			for(; p != NULL; p = p->right){
				q = p->left;
				m = q->left;
				switch(m->type){ //����β�����
					case INT: printf("int "); break;
					case FLOAT: printf("float "); break;
					case CHAR: printf("char "); break;				
				}
				m = q->right; //����β���
				printf("%s", m->info); 
				if(p->right != NULL) printf(", ");
			}
			printf(")");
		}
		p = root->right;
		if(p == NULL) printf(";\n");//��������
		else{		//�������壬���������
			printf("{\n");
			numtab++;
			OutCompSen(p);
			numtab--;
			for(i = 0; i < numtab; i++) printf("    ");//����
			printf("}\n");
		}
	}
	return OK;
}
/*******************************************
  	�������ƣ�OutCompSen
	�����������������
	�������ܣ���������������﷨��
	����ֵ��OK or ERROR
*******************************************/
int OutCompSen(pTree root){
	int i;
	pTree p = (pTree)malloc(sizeof(Tree));
	pTree q = (pTree)malloc(sizeof(Tree));
	if(flag){
		for(i = 0; i < numtab1; i++) printf("    ");//����
		printf("������䣺\n");
		if(root == NULL){
			printf("\n");
		}
		else{
			numtab1++;
			for(i = 0; i < numtab1; i++) printf("    ");//����
			printf("�������ı������壺\n"); numtab1++;
			p = root->left;
			if(p == NULL) printf("\n");
			else{ //����ֲ�����
				for(; p != NULL; p = p->right){
					q = p->left;
					q->type = LOCVARDEF;
					OutExtVarDef(q);
				}				
			}
			numtab1--;
			for(i = 0; i < numtab1; i++) printf("    ");//����
			printf("����������䲿�֣�\n"); numtab1++;
			p = root->right;
			if(p == NULL) printf("\n");
			else{
				for(; p != NULL; p = p->right){ //����������
					q = p->left;
					OutSentence(q); //������
				}
			}
			numtab1--;
		}
	}
	else{	
		p = root->left;
		if(p != NULL) { //����ڲ�����
			for(; p != NULL; p = p->right){
				q = p->left;
				q->type = LOCVARDEF;
				OutExtVarDef(q);
			}
				
		}
		p = root->right;
		for(; p != NULL; p = p->right){ //����������
			q = p->left;
			OutSentence(q); //������
		}
	}	
	return OK;
}
/*******************************************
  	�������ƣ�OutSentence
	�����������������
	�������ܣ�����������﷨��
	����ֵ��OK or ERROR
*******************************************/
int OutSentence(pTree root){
	if(root == NULL) return ERROR;
	int i;
	pTree p = (pTree)malloc(sizeof(Tree));
	pTree q = (pTree)malloc(sizeof(Tree));
	switch(root->type){
		case IF: //���if�������﷨��
			if(flag){//flagΪ1�����if�����﷨��
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("�������");
				if(root->right == NULL) printf("(IF_THEN):\n");
				else printf("(IF_THEN_ELSE):\n");
				numtab1++;
				p = root->left;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("������\n");
				numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				if(p != NULL) OutExp(p->middle); //����������ʽ
				numtab1--;
				printf("\n");
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("IF�Ӿ䣺\n");
				p = root->middle;
				if(p != NULL) {
					numtab1++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab1--;
				}
				else printf("\n");
				p = root->right;
				if(p != NULL){
					for(i = 0; i < numtab1; i++) printf("    ");//����
					printf("ELSE�Ӿ䣺\n");
					numtab1++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab1--;
				}
				numtab1--;
			}
			else{	//flagΪ0�����if���Ĵ���
				for(i = 0; i < numtab; i++) printf("    ");//����
				printf("if(");
				p = root->left; //����������ʽ
				if(p != NULL) OutExp(p->middle);
				printf(")");
				p = root->middle;//���IF�Ӿ�
				if(p != NULL) {
					printf("{\n");
					numtab++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab--;
					for(i = 0; i < numtab; i++) printf("    ");//����
					printf("}\n");
				}
				else printf("\n");
				p = root->right;
				if(p != NULL){
					for(i = 0; i < numtab; i++) printf("    ");//����
					printf("else{\n");
					numtab++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab--;
					for(i = 0; i < numtab; i++) printf("    ");//����
					printf("}\n");
				}
			}
			break;
		case WHILE://���while�������﷨��
			if(flag){//flagΪ1�����while�����﷨��
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("whileѭ����䣺\n"); numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("������\n"); numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				p = root->left;
				if(p != NULL) OutExp(p->middle); //����������ʽ
				numtab1--;
				printf("\n");
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("while�Ӿ䣺\n");
				p = root->middle;//���WHILE�Ӿ�
				if(p != NULL) {
					numtab1++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab1--;
				}
				else printf("\n");
				numtab1--;
			}
			else{	//flagΪ0�����while���
				for(i = 0; i < numtab; i++) printf("    ");//����
				printf("while(");
				p = root->left; //����������ʽ
				if(p != NULL) OutExp(p->middle);
				printf(")");
				p = root->middle;//���WHILE�Ӿ�
				if(p != NULL) {
					printf("{\n");
					numtab++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab--;
					for(i = 0; i < numtab; i++) printf("    ");//����
					printf("}\n");
				}
				else printf(";\n");
			}
			break;
		case FOR: //���for�������﷨��
			if(flag){//flagΪ1�����for�����﷨��
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("forѭ����䣺\n"); numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("������\n"); numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				p = root->left; //��������������ʽ
				if(p->left != NULL) OutExp(p->left); 
				printf("\n");
				for(i = 0; i < numtab1; i++) printf("    ");//����
				if(p->middle != NULL) OutExp(p->middle);
				printf("\n");
				for(i = 0; i < numtab1; i++) printf("    ");//����
				if(p->right != NULL) OutExp(p->right);
				printf("\n");
				numtab1--;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("for�Ӿ䣺\n"); numtab1++;
				
				p = root->middle;//���FOR�Ӿ�
				if(p != NULL) {
					numtab1++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab1--;
				}
				else printf("\n");
				numtab1--;
				numtab1--;
			}
			else{	 //flagΪ0�����for���
				for(i = 0; i < numtab; i++) printf("    ");//����
				printf("for(");
				p = root->left; //��������������ʽ
				if(p->left != NULL) OutExp(p->left); 
				printf("; ");
				if(p->middle != NULL) OutExp(p->middle);
				printf(";");
				if(p->right != NULL) OutExp(p->right);
				printf(")");
				p = root->middle;//���FOR�Ӿ�
				if(p != NULL) {
					printf("{\n");
					numtab++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab--;
					for(i = 0; i < numtab; i++) printf("    ");//����
					printf("}\n");
				}
				else printf(";\n");
			}
			break;
		case EXP: //������ʽ�����﷨��
			if(flag){ //flagΪ1������ʽ���﷨��
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("���ʽ��䣺\n"); numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				OutExp(root->middle);
				printf("\n");
				numtab1--;
			}
			else{	//flagΪ0��������ʽ
				for(i = 0; i < numtab; i++) printf("    ");//����
				OutExp(root->middle);
				printf(";\n");
			}
			break;
		case CONTINUE: //���continue�������﷨��
			if(flag){ //flagΪ1�����continue�����﷨��
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("continue��䣺\n");
				numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("continue;\n");
				numtab1--;
			}
			else{	//flagΪ0�����continue���
				for(i = 0; i < numtab; i++) printf("    ");//����
				printf("continue;\n");}
			break;
		case BREAK: //���break�������﷨��
			if(flag){ //flagΪ1�����break�����﷨��
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("break��䣺\n");
				numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("break;\n");
				numtab1--;
			}
			else{ //flagΪ0�����break���
				for(i = 0; i < numtab; i++) printf("    ");//����
				printf("break;\n");
			}
			break;
		case RETURN://���return�������﷨��
			if(flag){ //flagΪ1�����return�����﷨��
				for(i = 0; i < numtab1; i++) printf("    ");//����
				printf("return��䣺\n");
				numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//����
				OutExp(root->middle);
				numtab1--;
				printf("\n");
			}
			else{	//flagΪ0�����return���
				for(i = 0; i < numtab; i++) printf("    ");//����
				printf("return ");
				OutExp(root->middle);
				printf(";\n");
			}
			break;
	}	
	return OK;
}
/*******************************************
  	�������ƣ�OutExp
	�����������������
	�������ܣ�������ʽ
	����ֵ��OK or ERROR
*******************************************/
int OutExp(pTree T) { //������ʽ
	if(T == NULL) return OK;
	outexp(T->left); //�������������
	if(T->type == OPN) printf("%s ", T->info); //��������Ϣ
	else{
		switch (T->type){ //��������
			case EQ: printf("== "); break;
			case UEQ: printf("!= "); break;
			case MORE: printf("> "); break;
			case LESS: printf("< "); break;
			case MOEQ: printf(">= "); break;
			case LEEQ: printf("<= "); break;
			case ASSIGN: printf("= "); break;
			case PLUS: printf("+ "); break;
			case SUBTRACT: printf("- "); break;
			case MULTIPLY: printf("* "); break;
			case DIVIDE: printf("/ "); break;
			case PERC: printf("%% "); break;
			case OR: printf("|| "); break;
			case AND: printf("&& "); break;
			default: break;
		}
	}
	outexp(T->right); //�������������
	return OK;
}
/*******************************************
  	�������ƣ�outexp
	�����������������
	�������ܣ�������ʽ�ݹ��ӳ��򣬸���������ʽ
	����ֵ��OK or ERROR
*******************************************/
int outexp(pTree T) { //������ʽ�ݹ��ӳ���
	if(T == NULL) return OK;
	if(T->left != NULL && T->right != NULL) printf("( ");
	outexp(T->left); //�������������
	if(T->type == OPN) printf("%s ", T->info); //��������Ϣ
	else{
		switch (T->type){ //��������
			case EQ: printf("== "); break;
			case UEQ: printf("!= "); break;
			case MORE: printf("> "); break;
			case LESS: printf("< "); break;
			case MOEQ: printf(">= "); break;
			case LEEQ: printf("<= "); break;
			case ASSIGN: printf("= "); break;
			case PLUS: printf("+ "); break;
			case SUBTRACT: printf("- "); break;
			case MULTIPLY: printf("* "); break;
			case DIVIDE: printf("/ "); break;
			case PERC: printf("%% "); break;
			case OR: printf("|| "); break;
			case AND: printf("&& "); break;
			default: break;
		}
	}
	outexp(T->right); //�������������
	if(T->left != NULL && T->right != NULL) printf(") ");
	return OK;
}
