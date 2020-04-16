#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define ERROR 0
#define OVERFLOW -1

#define TOKEN_TEXT 100

typedef struct lookUp{  //查找表的结构定义
	char lookup[100];
	struct lookUp *next;
}LUT, *pLUT;
typedef struct tree{   //语法树节点的结构定义
	char info[TOKEN_TEXT];
	int type;
	struct tree *right;
	struct tree *left;
	struct tree *middle;
}Tree, *pTree;

/*----- 栈的顺序存储表示 -----*/
#define STACK_INIT_SIZE 100   //存储空间初始分配量
#define STACKINCREMENT 10     //分配增量
typedef pTree SElemType;
typedef struct {     //栈的定义
	SElemType *base; //栈底指针
	SElemType *top;  //栈顶指针
	int stacksize;   //已分配存储空间
}SqStack;

/*-----------------枚举常量-----------------*/
//ERROR_TOKEN--错误  IDENT--标识符  INT_CONST--整型常数  ASSIGN--赋值
//EQ--相等  UEQ--不等  MORE--大于  LESS--小于  MOEQ--大于等于  LEEQ--小于等于
//LP,RP--圆括号  LSB,RSB--方括号  LB,RB--花括号  SEMI--分号  COMMA--逗号
//PLUS,SUBTRACT,MULTIPLY,DIVIDE--加减乘除 ENTER--换行  HASH--#  
//SQM--单引号  DQM--双引号  PERC--百分号  AND--&&  OR--||  DOT--点
//LCOMMENT,RCOMMENT--块注释  COMMENT--行注释  OP--运算符  OPN--操作数
enum token_kind {
	ERROR_TOKEN,IDENT,INT_CONST,FLOAT_CONST,CHAR_CONST,INT,FLOAT,CHAR,IF,
	ELSE,FOR,DO,WHILE,BREAK,CONTINUE,RETURN,INCLUDE,DEFINE,EQ,UEQ,MORE,
	LESS,MOEQ,LEEQ,ASSIGN,LP,RP,LSB,RSB,LB,RB,SEMI,COMMA,PLUS,SUBTRACT,
	MULTIPLY,DIVIDE,ENTER,HASH,SQM,DQM,PERC,AND,OR,LCOMMENT,RCOMMENT,
	COMMENT,PROGRAM,EXTDEFLIST,EXTDEF,EXTVARDEF,FUNCDEF,FORMPAR,COMPSEN,
	LOCVARDEF,SENTLIST,SENTENCE,EXP,OP,OPN,PRETREATMENT,LOCDEFLIST,DOT,
};

extern char filename[100];
extern char token_text[TOKEN_TEXT]; //保留标识符值
extern pLUT Lookup[26];  //查找表结构数组
extern FILE *fp; //文件指针
extern int rownum, errnum;
int numtab = 0, numtab1 = 0; //控制缩进
int sym = 0;	//控制外部变量或局部变量
int sign = 0, flag = 0;
extern enum token_kind w;    //存储编码值
enum token_kind ret, prew = -1;    //存储编码值
char tokenText0[TOKEN_TEXT]; //存储变量名或函数名


/*-----------------函数声明-----------------*/
int gettoken();   	//单词分析
int got();			//单词分析子程序
int search();  		//判断关键字
int LookupTable();  //创建查找表
pTree Program(); 	//程序
pTree PreTreatment();//预处理
pTree ExtDefList(); //外部定义序列
pTree ExtDef(); 	//外部定义
pTree ExtVarDef(); 	//外部变量定义
pTree FuncDef(); 	//函数定义
pTree FormPar();    //函数形参
pTree formpar();    //函数形参递归子程序
pTree CompSen();    //复合语句
pTree LocDefList();  //局部变量定义序列
pTree SentList();   //语句序列
pTree Sentence();   //语句
pTree Exp(int endsym);//表达式
int check(int w);		//判断操作数和运算符
int compare(int p, int q);	//比较优先级，前者高于后者返回OK
int Destroy(pTree T);		//销毁
int InitStack(SqStack *S);  //创建空栈
int GetTop(SqStack S, SElemType *p); //返回栈顶元素
int Push(SqStack *S, SElemType p);   //插入栈顶元素
int Pop(SqStack *S, SElemType *p);   //删除栈顶元素并返回其值
int Lexan();	//词法分析
int ErrHandle();		//错误处理
int OutProgram(pTree root); //输出程序
int OutExtVarDef(pTree root); //输出外部变量定义
int OutFuncDef(pTree root); //输出函数
int OutCompSen(pTree root); //输出复合语句
int OutSentence(pTree root);//输出语句
int OutExp(pTree T); 	//输出表达式
int outexp(pTree T); 	//输出表达式递归子程序
/*-----------------函数定义-----------------*/
/*******************************************
  	函数名称：gettoken
	函数参数：全局变量（字符数组token_text，文件指针fp）
	函数功能：单词识别
	返回值：各种类编码
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
	token_text[i] = '\0';  //初始化字符数组
	while( (c=fgetc(fp)) == ' ' || c == '\n')
		if(c == '\n') {
			rownum++; 
			return ENTER;
		}
	if((c>='a' && c<='z') || (c>='A' && c<='Z')){ //字母
		do{
			token_text[i] = c;
			i++;
		}while(((c=fgetc(fp))>='a'&&c<='z') || (c>='A'&&c<='Z') || (c>='0'&&c<='9'));
		ungetc(c, fp); //退回多余字符至文件缓冲区
		token_text[i] = '\0';  //字符串读入结束
		if(search() == ERROR) return IDENT;  //不是关键字，返回标识符编码IDENT
		else return w;                       //返回关键字编码
	}
	else if(c>='1' && c<='9'){ //数字
		do{
			token_text[i] = c;
			i++;
		}while((c=fgetc(fp))>='0' && c<='9');
		if(c != '.'){
			ungetc(c, fp); //退回多余字符至文件缓冲区
			token_text[i] = '\0';  //数字串读入结束
	    	return INT_CONST;  //返回整型数字编码INT_CONST
		}
  		else{
			do{
			   token_text[i] = c;
			   i++;
			}while((c=fgetc(fp))>='0' && c<='9');
			ungetc(c, fp); //退回多余字符至文件缓冲区
			token_text[i] = '\0';  //数字串读入结束
	    	return FLOAT_CONST;  //返回浮点型数字编码FLOAT_CONST
		}
	}
	else if(c == '0'){
		token_text[i] = c;
		i++;
		c = fgetc(fp);
		if(c>='1' && c<='9') {//八进制
			do{
				token_text[i] = c;
				i++;
			}while((c=fgetc(fp))>='0' && c<='9');
			ungetc(c, fp); //退回多余字符至文件缓冲区
			token_text[i] = '\0';  //数字串读入结束
	    	return INT_CONST;  //返回整型数字编码INT_CONST
		}
		else if(c == 'x' || c == 'X'){//十六进制
			do{
				token_text[i] = c;
				i++;
				c = fgetc(fp);
			}while((c>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F'));
			ungetc(c, fp); //退回多余字符至文件缓冲区
			token_text[i] = '\0';  //数字串读入结束
	    	return INT_CONST;  //返回整型数字编码INT_CONST
		}
		else if(c == '.'){//浮点数
			do{
			   token_text[i] = c;
			   i++;
			}while((c=fgetc(fp))>='0' && c<='9');
			ungetc(c, fp); //退回多余字符至文件缓冲区
			token_text[i] = '\0';  //数字串读入结束
	    	return FLOAT_CONST;  //返回浮点型数字编码FLOAT_CONST			
		}
		else{//0
			ungetc(c, fp); //退回多余字符至文件缓冲区
			token_text[i] = '\0';  //数字串读入结束
	    	return INT_CONST;  //返回整型数字编码INT_CONST			
		}
	}
	switch(c) {
		case '=':
			c = fgetc(fp);
			if(c == '=') return EQ;  //返回相等编码EQ
			ungetc(c, fp); //退回多余字符至文件缓冲区
			return ASSIGN;  //返回赋值编码ASSIGN
		case '>':
			c = fgetc(fp);
			if(c == '=') return MOEQ;  //返回大于等于编码MOEQ
			ungetc(c, fp); //退回多余字符至文件缓冲区
			return MORE;   //返回大于编码MORE
		case '<':
			c = fgetc(fp);
			if(c == '=') return LEEQ;  //返回小于等于编码LEEQ
			ungetc(c, fp); //退回多余字符至文件缓冲区
			return LESS;   //返回小于编码LESS
		case '+': return PLUS;   //返回加法编码PLUS
		case '-': return SUBTRACT; //返回减法编码SUBTRACT
		case '*':
			c = fgetc(fp);
			if(c == '/') return RCOMMENT;  //返回块注释编码RCOMMENT
			ungetc(c, fp); //退回多余字符至文件缓冲区
			return MULTIPLY; //返回乘法编码MULTIPLY
		case '/':
			c = fgetc(fp);
			if(c == '/') return COMMENT;  //返回行注释编码COMMENT
			else if(c == '*') return LCOMMENT;  //返回块注释编码LCOMMENT
			else {
				ungetc(c, fp); //退回多余字符至文件缓冲区
				return DIVIDE; //返回除法编码DIVIDE
			}
		case '.':
			return DOT;  //返回点编码DOT
        case '(':
        	return LP;   //返回左圆括号LP
        case ')':
        	return RP;   //返回右圆括号编码RP
        case '[':
        	return LSB;   //返回左方括号编码LSB
        case ']':
        	return RSB;   //返回右方括号编码RSB
        case '{':
        	return LB;   //返回左花括号编码LB
        case '}':
        	return RB;   //返回右花括号编码RB
		case ';':
			return SEMI;  //返回分号编码SEMI
		case ',':
			return COMMA; //返回逗号编码COMMA
		case '#':
			return HASH; //返回井号编码HASH
		case '\'':
			return SQM; //返回单引号编码SQM
		case '"':
			return DQM; //返回双引号编码DQM
		case '%':
			return PERC; //返回百分号编码PERC
		case '|':
			if((c=fgetc(fp)) == '|') return OR;  //返回或编码OR
			else return ERROR_TOKEN;  //返回错误编码ERROR_TOKEN
		case '&':
			if((c=fgetc(fp)) == '&') return AND;  //返回与编码AND
			else return ERROR_TOKEN;  //返回错误编码ERROR_TOKEN
		case '!':
			if((c=fgetc(fp)) == '=') return UEQ;  //返回不等编码UEQ
			else return ERROR_TOKEN;  //返回错误编码ERROR_TOKEN
		default:
			if(feof(fp)) return EOF;  //文件已读完
			else return ERROR_TOKEN;  //返回错误编码ERROR_TOKEN
	}
}

/*******************************************
  	函数名称：search
	函数参数：全局变量（字符数组token_text，结构数组Lookup）
	函数功能：判断关键字
	返回值：OK or ERROR
*******************************************/
int search(){
	pLUT p;
	int i;
	char ch = token_text[0];
	if(ch < 'a' || ch > 'z') return ERROR;
	i = ch - 'a';
	if(Lookup[i] == NULL) return ERROR;
	for(p = Lookup[i]; p != NULL; p = p->next)  //判断是否为关键字
		if(strcmp(p->lookup, token_text) == 0) { //判断关键字类型
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
  	函数名称：LookupTable
	函数参数：全局变量（文件指针fp，字符数组filename）
	函数功能：创建查找表
	返回值：OK or ERROR
*******************************************/
int LookupTable(){
	pLUT p, q;
	char ch, s[100];
	int i, j;
	fp = NULL;  //初始化
	if((fp = fopen(filename, "rt")) == NULL) {  //以只读方式打开文件
		printf("文件打开错误！\n");             //指针为空，出错
		return ERROR;
	}
	for(i = 0; i < 26; i++) Lookup[i] = NULL;     //初始化
	do {
		s[0] = '\0';
		for(j = 0; fread(&ch, sizeof(char), 1, fp); j++) { //读入字符串
			if(ch < 'a' || ch > 'z') break;
			else s[j] = ch;
		}
		if(s[0] == '\0') continue;  //未读入字符串
		s[j] = '\0';
		i = s[0] - 'a';  //找到关键字对应位置
		if(Lookup[i] == NULL) {   //指针为空
			p = (pLUT)malloc(sizeof(LUT));
			strcpy(p->lookup, s);
			p->next = NULL;
			Lookup[i] = p;
		}
		else{     //指针不为空
			p = (pLUT)malloc(sizeof(LUT));
			strcpy(p->lookup, s);
			q = Lookup[i];
			Lookup[i] = p;
			p->next = q;
		}
	}while(!feof(fp));  //文件已读取完
	fclose(fp);
	return OK;
}

/*******************************************
  	函数名称：Program
	函数参数：
	函数功能：程序
	返回值：语法树根结点指针
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
//    	程序语法正确，返回的语法树根结点指针，可遍历显示
		return root;
	}
    else{
//    	有语法错误
		return NULL;
	}
}
/*******************************************
  	函数名称：PreTreatment
	函数参数：
	函数功能：预处理
	返回值：子树根结点指针
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
			p->info[i] = '.';//作为文件名
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
			p->info[i] = '.';//作为文件名
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
		p->info[i] = ' ';//作为分隔符
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
  	函数名称：skip
	函数参数：无
	函数功能：跳至下一行
	返回值：无
*******************************************/
void skip(){
	while(1){
		w = got();
		if(w == ENTER) break;
	}
}

/*******************************************
  	函数名称：ExtDefList
	函数参数：
	函数功能：外部定义序列
	返回值：子树根结点指针
*******************************************/
pTree ExtDefList(){
	//处理外部定义序列，正确时，返回子树根结点指针，否则错误处理
    if (w == EOF) return NULL; //文件结束
	if(w != INT && w != FLOAT && w != CHAR) return NULL;
    //生成一个外部定义序列结点root
    pTree root = (pTree)malloc(sizeof(Tree));
	root->right = root->left = NULL;
	if(sym == 0)root->type = EXTDEFLIST;
	else root->type = LOCDEFLIST;
    //处理一个外部定义，得到一棵子树，作为root的第一棵子树
    root->left = ExtDef();
	//处理一个外部定义序列，得到的子树，作为root的第二棵子树
    root->right = ExtDefList();
	if(errnum == 0) return root;
	else return NULL;
}

/*******************************************
  	函数名称：ExtDef
	函数参数：
	函数功能：外部定义
	返回值：子树根结点指针
*******************************************/
pTree ExtDef(){
	//处理外部定义，正确时，返回子树根结点指针，否则返回NULL
	//生成一个外部定义结点root
    pTree root = (pTree)malloc(sizeof(Tree));
	root = NULL;
	//不是关键字，报错并返回NULL
    if (w != INT && w != FLOAT && w != CHAR && w != IDENT){
		ErrHandle();
    	return NULL;
	}
	ret = w; //保存类型说明符
    w = gettoken();
    //不是标识符，报错并返回NULL
    if (w != IDENT ){
		ErrHandle();
    	return NULL;
	}
	strcpy(tokenText0, token_text); //保存第一个变量名或函数名到tokenText0

    w = gettoken();
    if (w != LP) root = ExtVarDef();  //调用外部变量定义子程序
    else root = FuncDef();    //调用函数定义子程序
    //如果返回值root非空，表示成功完成一个外部定义的处理
	if(errnum == 0) return root;
	else return NULL;
}

/*******************************************
  	函数名称：ExtVarDef
	函数参数：
	函数功能：外部变量定义
	返回值：子树根结点指针
*******************************************/
pTree ExtVarDef(){
	int i, j;
	//生成外部变量定义节点root
    pTree root = (pTree)malloc(sizeof(Tree));
	root->right = root->left = NULL;
	if(sym == 0) root->type = EXTVARDEF;
	else root->type = LOCVARDEF;
	//生成外部变量类型结点m，作为root的第一个孩子
	pTree m = (pTree)malloc(sizeof(Tree));
	m->left = m->right = NULL;
	m->type = ret;
	root->left = m;
	//生成外部变量序列结点p，作为root的第二个孩子
	//每个外部变量序列结点会对应一个变量名
	pTree p = (pTree)malloc(sizeof(Tree));
	p->left = p->right = NULL;
	root->right = p;
	//由保存在tokenText0的第一个变量名生成一个变量名结点n，作为p的第一个孩子
	pTree n = (pTree)malloc(sizeof(Tree));
	n->left = n->right = NULL;
	n->type = IDENT;
	for(i = 0; tokenText0[i] != '\0'; i++) n->info[i] = tokenText0[i];
	if(w == LSB){	//输入数组
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
	//每个外部变量序列结点的第一个孩子对应一个变量
	while(1) {
		//读入非逗号或分号，报错，释放root为根的全部结点，返回空指针
		if (w != COMMA && w != SEMI){
			ErrHandle();
			Destroy(root);
			return NULL;
		}
		else if (w == SEMI){
			w = gettoken();
			if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
			w = gettoken();
			if(errnum == 0) return root;//返回根结点root；
			else return NULL;  
		}
		w = gettoken(); //读入后续变量名
		//w不是标识符，报错，释放root为根的全部结点，返回空指针
		if(w != IDENT) {
			ErrHandle();
			Destroy(root);
			return NULL;
		}
		//生成外部变量序列结点q，作为p的第二个孩子，插入到树中
  		pTree q = (pTree)malloc(sizeof(Tree));
		q->right = q->left = NULL;
		q->type = EXTDEFLIST;
		p->right = q;
		//根据token_text的变量名生成一个变量结点，作为q的第一个孩子
		pTree n = (pTree)malloc(sizeof(Tree));
		n->left = n->right = NULL;
		n->type = IDENT;
		for(i = 0; token_text[i] != '\0'; i++) n->info[i] = token_text[i];
		w = gettoken(); //读入后续变量名
		if(w == LSB){ //读入数组
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
  	函数名称：FuncDef
	函数参数：
	函数功能：函数定义
	返回值：子树根结点指针
*******************************************/
pTree FuncDef(){
	if(errnum) return NULL;
	//生成函数定义结点root
	pTree root = (pTree)malloc(sizeof(Tree));
	root->right = root->left = root->middle = NULL;
	root->type = FUNCDEF;
	//生成返回值类型结点，作为root的第一个孩子
	pTree m = (pTree)malloc(sizeof(Tree));
	m->left = m->right = m->middle = NULL;
	m->type = ret;
	root->left = m;
	//处理参数部分到反小括号结束，调用形参子程序，得到参数部分的子树根指针
	//无参函数得到NULL，该子树作为root的第二棵子树
	//由保存在tokenText0的第一个变量名生成一个变量名结点n
	pTree n = (pTree)malloc(sizeof(Tree));
	n->left = n->right = n->middle = NULL;
	n->type = IDENT;
	strcpy(n->info, tokenText0);
	root->middle = n;
	n->middle = FormPar();
	//读入符号，如果分号，就是函数原型声明，函数体子树为空；
	if(w == SEMI) {
		root->right = NULL;
		w = gettoken();
		if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
	}
	//正大括号，则调用函数体（复合语句）子程序，得到函数体子树根指针，
	else if(w == LB){
		w = gettoken();
		if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
		root->right = CompSen();

	}
	//其它符号表示有错。得到的函数体子树作为root的第三棵子树
	else {ErrHandle(); return NULL;}
	w = gettoken();
	if(errnum == 0) return root;
	else return NULL;  //返回root；
}

/*******************************************
  	函数名称：FormPar
	函数参数：
	函数功能：函数形参
	返回值：子树根结点指针
*******************************************/
pTree FormPar(){
	if(errnum) return NULL;
	w = gettoken();
	if(w == RP) return NULL; //无形参
    pTree root = (pTree)malloc(sizeof(Tree));
    pTree p = (pTree)malloc(sizeof(Tree));
	root = formpar();
	p = root;

	w = gettoken(); //读入后续形参
	//每个形参序列结点的第一个孩子对应一个形参
	while(1) {
		//读入非逗号或反括号，报错，释放root为根的全部结点，返回空指针
		if (w == COMMA){
			w = gettoken(); //读入后续形参
			p->right = formpar();
			p = p->right;
			w = gettoken(); //读入后续变量名

		}
		else if (w == RP){
			w = gettoken();
			if(errnum == 0) return root;//返回根结点root；
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
  	函数名称：formpar
	函数参数：
	函数功能：函数形参辅助子程序
	返回值：子树根结点指针
*******************************************/
pTree formpar(){
	//不是关键字，报错并返回NULL
    if (w != INT && w != FLOAT && w != CHAR){
		ErrHandle();
    	return NULL;
	}
	ret = w; //保存类型说明符
    w = gettoken();
    //不是标识符，报错并返回NULL
    if (w != IDENT ){
		ErrHandle();
    	return NULL;
	}

	//生成形参序列节点root
    pTree root = (pTree)malloc(sizeof(Tree));
	root->right = root->left = NULL;
	root->type = FORMPAR;
	//生成形参结点m，作为root的第一个孩子
	pTree m = (pTree)malloc(sizeof(Tree));
	m->left = m->right = NULL;
	m->type = FORMPAR;
	root->left = m;
	//生成形参类型结点p，作为m的第一个孩子
	pTree p = (pTree)malloc(sizeof(Tree));
	p->left = p->right = NULL;
	p->type = ret;
	m->left = p;
	//由保存在token_text的变量名生成一个变量名结点q，作为m的第二个孩子
	pTree q = (pTree)malloc(sizeof(Tree));
	q->left = q->right = NULL;
	q->type = IDENT;
	strcpy(q->info, token_text);
	m->right = q;

	if(errnum == 0) return root;
	else return NULL;
}

/*******************************************
  	函数名称：CompSen
	函数参数：
	函数功能：复合语句
	返回值：子树根结点指针
*******************************************/
pTree CompSen(){
	if(errnum) return NULL;
	//生成复合语句结点，注意其中局部变量说明和语句序列都可以为空
    pTree root = (pTree)malloc(sizeof(Tree));
	root->right = root->left = NULL;
	root->type = COMPSEN;

	w = gettoken();
	if(w == INT || w == FLOAT || w == CHAR){ //w是类型关键字
		ret = w; //保存类型说明符
		//调用处理局部变量说明序列子程序，得到返回的子树根结点作为root的第一个孩子
		root->left = LocDefList();
	}
	else {
		//无局部变量说明，root的第一个孩子设置为空指针
		root->left = NULL;
	}
	//调用处理语句序列子程序，返回子树根结点指针，作为root的第2个孩子
	root->right = SentList();

    if(w != RB){ //w不是反大括号}返回空指针，报错并释放结点
		ErrHandle();
		Destroy(root);
		return NULL;
	}
    w = gettoken();
	//返回复合语句的子树根指针
	if(errnum == 0) return root;
	else return NULL;
}

/*******************************************
  	函数名称：LocDefList
	函数参数：
	函数功能：局部变量定义序列
	返回值：子树根结点指针
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
  	函数名称：SentList
	函数参数：
	函数功能：语句序列
	返回值：子树根结点指针
*******************************************/
pTree SentList(){
	if(errnum) return NULL;
	//初始化子树，根指针root=NULL；
	//生成语句序列结点
    pTree root = (pTree)malloc(sizeof(Tree));
    root->left = root->middle = root->right = NULL;
    pTree p = (pTree)malloc(sizeof(Tree));
    p = NULL;
	//调用处理一条语句的子程序；返回其子树根指针r1；
	p = Sentence();
	if(p == NULL) {
	//没有分析到1条语句，errors>0时处理错误，否则表示语句序列已结束,返回NULL
		// if(errors > 0){    //处理错误

		// }
		// else return NULL;
		return NULL;
	}
	else {
		//生成语句序列的结点root
	    root->left = p;
		root->right = SentList();  //递归调用处理语句序列子程序后的返回值；
		if(errnum == 0) return root;//返回root；
		else return NULL; 
	}
}

/*******************************************
  	函数名称：Sentence
	函数参数：
	函数功能：语句
	返回值：子树根结点指针
*******************************************/
pTree Sentence(){
	if(errnum) return NULL;
    pTree root = (pTree)malloc(sizeof(Tree));
    root->left = root->right = root->middle = NULL;
	pTree p = (pTree)malloc(sizeof(Tree));
	p->left = p->middle = p->right = NULL;
	switch (w){
  		case IF://分析条件语句
		 	root->type = w;
			w = gettoken();
			if (w != LP){ //w不是左小括号报错并返回空
				ErrHandle();
				return NULL;
			}
			//	调用处理表达式的子程序（结束符号为反小括号）
			//	正确时得到条件表达式子树结点指针
			w = gettoken();
			root->left = p;
			p->type = EXP;
			p->middle = Exp(RP);
			//	调用处理一条语句的子程序，得到IF子句的子树根指针。
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
			//调用处理一条语句的子程序，得到IF子句的子树根指针。
			//生成IF-ELSE结点，下挂条件、IF子句、ELSE子句3棵子树
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
			//生成IF结点，下挂条件、IF子句2棵子树
				root->right = NULL;
			}
			if(errnum == 0) return root;
			else return NULL;
   		case WHILE:  //分析while语句
		   	root->type = w;
			w = gettoken();
			if (w != LP){ //w不是左小括号报错并返回空
				ErrHandle();
				return NULL;
			}
			//	调用处理表达式的子程序（结束符号为反小括号）
			//	正确时得到条件表达式子树结点指针
			w = gettoken();
			root->left = p;
			p->type = EXP;
			p->middle = Exp(RP);
			if(w != LB) {ErrHandle(); return NULL;}
			w = gettoken();
			if(w != ENTER) {ErrHandle(); return NULL;}
			w = gettoken();
			//	调用处理一条语句的子程序，得到WHILE子句的子树根指针。
			root->middle = SentList();
			if(w != RB) {ErrHandle(); return NULL;}
			w = gettoken();
			if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
			break;
		case FOR:
			root->type = w;
			w = gettoken();
			if (w != LP){ //w不是左小括号报错并返回空
				ErrHandle();
				return NULL;
			}
			//	调用处理表达式的子程序
			//	正确时得到三个条件表达式子树结点指针
			root->left = p;
			p->type = EXP;
			w = gettoken();
			p->left = Exp(SEMI);
			p->middle = Exp(SEMI);
			p->right = Exp(RP);
			//	调用处理一条语句的子程序，得到WHILE子句的子树根指针。
			if(w != LB) {ErrHandle(); return NULL;}
			w = gettoken();
			if(w != ENTER) {ErrHandle(); return NULL;}
			w = gettoken();
			root->middle = SentList();
			if(w != RB) {ErrHandle(); return NULL;}
			w = gettoken();	
			if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
			break;
		case LP:      //各种表达式语句，含赋值等，形式为表达式以分号结束
			root->type = EXP; 
			root->middle = Exp(SEMI);
			break;
		case IDENT:
		case INT_CONST:
  		case FLOAT_CONST:
		case CHAR_CONST:
  			//调用表达式处理子程序（结束符号为分号）
			//正确时，w=gettoken();  返回表达式语句子树的根结点指针
			root->type = EXP; 
			root->middle = Exp(SEMI);
			if(w != ENTER && w != EOF) {ErrHandle(); return NULL;}
			break;
  		//调用处理复合语句子程序,返回得到的子树根指针
		case LB: root = CompSen(); break;
		//语句序列结束符号，如果语言支持switch语句，结束符号还会有case和default
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
    	   	return NULL;	//报错并返回NULL
	}
	w = gettoken();
	if(errnum == 0) return root;
	else return NULL;
}

/*******************************************
  	函数名称：Exp
	函数参数：枚举常量
	函数功能：表达式
	返回值：子树根结点指针
*******************************************/
//结点指针类型  exp(int endsym)
//表达式结束符号endsym可以是分号，如表达式语句，可以是反小括号，作为条件时使用
pTree Exp(int endsym){  //调用该算法时，在调用者已经读入了第一个单词在w中
	int numlp = 0; //未配对的左括号计数
	int i, j;
	char c;
   	SqStack op, opn;
    InitStack(&op); //创建空运算符栈
    InitStack(&opn); //创建空操作数栈
	pTree q = (pTree)malloc(sizeof(Tree));
    q->left = q->right = q->middle = NULL;
	pTree m = (pTree)malloc(sizeof(Tree));
    m->left = m->right = m->middle = NULL;
	pTree n = (pTree)malloc(sizeof(Tree));
    n->left = n->right = n->middle = NULL;
    while(1){  //将中缀表达式转换成后缀表达式。存储在操作数栈中
		pTree p = (pTree)malloc(sizeof(Tree)); //创建结点
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
		if(check(w) == OPN) {  //操作数，则进入操作数栈
			p->type = OPN;
	  		Push(&opn, p);
   		}
   		else if(check(w) == OP){  //运算符
   			//非括号运算符，运算符栈顶优先级不低于该运算符的进入操作数栈
			while(1){
				if(GetTop(op, &q) ==ERROR) break; //栈顶无元素
				if(compare(p->type, q->type) == OK) break;//p优先级高于q
				else { //p优先级低于或等于q
					Pop(&op, &q);
				 	Push(&opn, q);
				}
			}
			Push(&op, p);
		}
   		else if(w == LP){ //左括号，则进入运算符栈
			 Push(&op, p);
			 numlp++;
		}
   		else if(w == RP){ //右括号，则运算符栈顶非左括号的运算符进入操作数栈
        	if(endsym == RP && numlp == 0){ //结束符号为右括号且没有未配对的左括号
     			while(op.base != op.top) { //运算符栈所有元素进入操作数栈
   					Pop(&op, &q);
   					Push(&opn, q);
				}
				break;
			}
			else if(endsym == SEMI && numlp == 0){//结束符号为分号且没有未配对的左括号
		    	 ErrHandle();
    	   		 return NULL;
			}
			while(1){     //有未配对的左括号
   				Pop(&op, &q);
   				if(q->type == LP) break;
   				else Push(&opn, q);
			}
            numlp--;
		}
    	else if(w == endsym) { //分号结束字符
    		while(op.base != op.top) { //运算符栈所有元素进入操作数栈
   				Pop(&op, &q);
   				Push(&opn, q);
			}
			break;
		}
		else { //报错，返回NULL
	 		ErrHandle();
    	   	return NULL;
		}
		w = gettoken();
	}
	while (opn.base != opn.top)
	{//将操作数栈中的后缀表达式，倒序存放至运算符栈中
		Pop(&opn, &m);
		Push(&op, m);
	}
	while(op.base != op.top){//构建后缀表达式的二叉树
		Pop(&op, &m);
		if(m->type == OPN) Push(&opn, m);//操作数则直接进栈
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
int check(int w) {  //判断操作数和运算符
	switch(w){
		case EQ:	case UEQ: 	case MORE: 	case LESS: 		case MOEQ:
		case LEEQ:	case ASSIGN:case PLUS: 	case SUBTRACT: 	case MULTIPLY:
		case DIVIDE:case PERC: 	case OR: 	case AND:
        	return OP;   //运算符
		case IDENT: case INT_CONST: case FLOAT_CONST: case CHAR_CONST:
			return OPN;  //操作数
        default :
			return ERROR;//非法字符
	}
}
int compare(int p, int q){ //比较优先级，前者高于后者返回OK
	if(p == ASSIGN) {//赋值优先级最低
		if(q == ASSIGN) return OK; //同级 从右到左
		else return ERROR;
	} 
	if(q == ASSIGN || q == LP) return OK;//赋值和左括号优先级最低
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
  	函数名称：Destroy
	函数参数：根结点指针
	函数功能：销毁
	返回值：子树根结点指针
*******************************************/
int Destroy(pTree T) {
	if(!T) return ERROR;
	if(T->left) Destroy(T->left); //递归
	if(T->right) Destroy(T->right);
	free(T);       //释放T指向的结点存储空间
	T = NULL;      //T为空指针
	return OK;
}
/*******************************************
  	函数名称：InitStack
	函数参数：栈S指针
	函数功能：创建空栈
	返回值：OK or ERROR
*******************************************/
int InitStack(SqStack *S) {
	S->base = (SElemType*)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if(!S->base) exit(OVERFLOW); //存储空间分配失败
	S->top = S->base;  //栈顶指针等于栈底指针，即空栈
	S->stacksize = STACK_INIT_SIZE; //更新存储空间
	return OK;
}
/*******************************************
  	函数名称：GetTop
	函数参数：栈S，栈元素指针
	函数功能：返回栈顶元素
	返回值：OK or ERROR
*******************************************/
int GetTop(SqStack S, SElemType *p) {
	if(S.base == S.top) return ERROR; //栈为空，无元素
	*p = *(S.top-1);  //传出栈顶元素值
	return OK;
}
/*******************************************
  	函数名称：Push
	函数参数：栈S指针，栈元素
	函数功能：插入栈顶元素
	返回值：OK or ERROR
*******************************************/
int Push(SqStack *S, SElemType p) {
	if(S->top - S->base >= S->stacksize) { //栈存储空间已满
		S->base = (SElemType*)realloc(S->base, (S->stacksize+STACKINCREMENT)*sizeof(SElemType));
		if(!S->base) exit(OVERFLOW); //存储空间分配失败
		S->top = S->base + S->stacksize;  //更新栈顶指针
		S->stacksize += STACKINCREMENT; //更新存储空间
	}
	*S->top = p;  //将元素插入栈顶
	S->top++;     //栈顶指针自增
	return OK;
}
/*******************************************
  	函数名称：Pop
	函数参数：栈S指针，栈元素指针
	函数功能：删除栈顶元素
	返回值：OK or ERROR
*******************************************/
int Pop(SqStack *S, SElemType *p) {
	if(S->base == S->top) return ERROR; //栈为空，无栈顶元素
	S->top--;  //栈顶指针自减
	*p = *(S->top); //传出栈顶元素的值
	return OK;
}
/*******************************************
  	函数名称：Lexan
	函数参数：
	函数功能：词法分析
	返回值：OK or ERROR
*******************************************/
int Lexan(){
	int ret;
 	if((fp = fopen("test.txt", "rt")) == NULL) {  //以只读方式打开文件
		printf("测试文件打开错误！\n");    //指针为空，出错
		return ERROR;
	}
	ret = rownum;  //初始化ret
	printf("第%d行\n", rownum);
	do{
		w = gettoken();
		if(rownum > ret){
			printf("第%d行\n", rownum);
			ret = rownum;
		}
		switch(w) {
			case INT: printf("关键字: int\n"); break;
			case FLOAT: printf("关键字: float\n"); break;
			case CHAR: printf("关键字: char\n"); break;
			case IF: printf("关键字: if\n"); break;
			case ELSE: printf("关键字: else\n"); break;
			case DO: printf("关键字: do\n"); break;
			case WHILE: printf("关键字: while\n"); break;
			case FOR: printf("关键字：for\n"); break;
			case BREAK: printf("关键字: break\n"); break;
			case CONTINUE: printf("关键字: continue\n"); break;
			case RETURN: printf("关键字: return\n"); break;
			case INCLUDE: printf("关键字: include\n"); break;
			case DEFINE: printf("关键字: define\n"); break;

			case IDENT: printf("标识符: %s\n", token_text); break;
			case INT_CONST: printf("整型数字: %s\n", token_text); break;
			case FLOAT_CONST: printf("浮点型数字: %s\n", token_text); break;
			case CHAR_CONST: printf("字符型数字: %s\n", token_text); break;

			case EQ: printf("相等:   ==\n"); break;
			case UEQ: printf("不等:   !=\n"); break;
			case MORE: printf("大于:   >\n"); break;
			case LESS: printf("小于:   <\n"); break;
			case MOEQ: printf("大于等于: >=\n"); break;
			case LEEQ: printf("小于等于: <=\n"); break;
			case ASSIGN: printf("赋值:   =\n"); break;

			case PLUS: printf("加号:   +\n"); break;
			case SUBTRACT: printf("减号:   -\n"); break;
			case MULTIPLY: printf("乘号:   *\n"); break;
			case DIVIDE: printf("除号:   /\n"); break;

			case LP: printf("左圆括: (\n"); break;
			case RP: printf("右圆括: )\n"); break;
			case LSB: printf("左方括: [\n"); break;
			case RSB: printf("右方括: ]\n"); break;
			case LB: printf("左花括: {\n"); break;
			case RB: printf("右花括: }\n"); break;
			case SEMI: printf("分号:   ;\n"); break;
			case COMMA: printf("逗号:   ,\n"); break;
			case HASH: printf("井号:   #\n"); break;
			case SQM: printf("单引号: '\n"); break;
			case DQM: printf("双引号: \"\n"); break;
			case PERC: printf("百分号: %%\n"); break;
			case OR: printf("或:     ||\n"); break;
			case AND: printf("与:     &&\n"); break;
			case DOT: printf("点：    .\n"); break;
			case ENTER: break;
			default :
				if(feof(fp)) break;  //文件已读完
				else printf("错误编码");  //错误编码
				break;
		}
	}while(!feof(fp));  //文件读取完
	fclose(fp);
	system("pause");
	return OK;
}
/*******************************************
  	函数名称：ErrHandle
	函数参数：
	函数功能：错误处理
	返回值：OK or ERROR
*******************************************/
int ErrHandle(){
	errnum++;
	if(errnum != 1) return ERROR;
	printf("第%d行 ：", rownum);
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
		case ENTER: printf("行尾缺少\n"); break;
		case EOF: printf("文件已读取完\n"); break;
		default :
			if(feof(fp)) break;  //文件已读完
			else printf("错误编码\n");  //错误编码
			break;
	}
	skip();
	return OK;
}
/*******************************************
  	函数名称：OutProgram
	函数参数：树根结点
	函数功能：输出程序或其语法树
	返回值：OK or ERROR
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
		case EXTVARDEF: //输出外部变量定义
		case LOCVARDEF: //输出局部变量定义
			OutExtVarDef(root); break;
		case FUNCDEF: //输出函数
			OutFuncDef(root); break; 
		case INCLUDE: //输出文件包含指令
			if(flag){
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("文件包含指令：\n"); numtab1++;
				for(i = 0; i < numtab; i++) printf("    ");//缩进
				printf("文件名：%s\n", root->info);
				numtab1--;
			}
			else printf("#include %s\n", root->info); break;
		case DEFINE: ///输出宏定义指令
			if(flag){
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("宏定义指令：\n"); numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("标识符：");
				for(j = 0; root->info[j] != ' '; j++){
					printf("%c", root->info[j]);
				}
				printf("\n");
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("数值：");
				for(j++; root->info[j] != '\0'; j++){
					printf("%c", root->info[j]);
				}
				printf("\n");
				numtab1--;
			}
			else printf("#define %s\n", root->info);
			break;
		case EXP: //输出表达式
			if(flag){ //输出语法树
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("表达式：");
				OutExp(root->middle);
				printf("\n");
			}
			else OutExp(root->middle); //输出程序
			break;
	}
	return OK;
}
/*******************************************
  	函数名称：OutExtVarDef
	函数参数：树根结点
	函数功能：输出外部变量定义或局部变量定义或二者的语法树
	返回值：OK or ERROR
*******************************************/
int OutExtVarDef(pTree root){
	int i;
	pTree p = (pTree)malloc(sizeof(Tree));
	pTree q = (pTree)malloc(sizeof(Tree));
	if(flag){ //输出语法树
		for(i = 0; i < numtab1; i++) printf("    ");//缩进
		if(root->type = EXTVARDEF) printf("外部变量定义：\n");
		else if(root->type = LOCVARDEF) printf("局部变量定义：\n");
		numtab1++;
		for(i = 0; i < numtab1; i++) printf("    ");//缩进
		printf("类型：");
		p = root->left;	
		switch(p->type){ //输出变量类型
			case INT: printf("int \n"); break;
			case FLOAT: printf("float \n"); break;
			case CHAR: printf("char \n"); break;
		}
		for(i = 0; i < numtab1; i++) printf("    ");//缩进		
		printf("变量名：\n"); numtab1++;
		for(p = root->right; p != NULL; p = p->right){
			q = p->left;	//输出变量名
			for(i = 0; i < numtab1; i++) printf("    ");//缩进
			printf("ID: ");
			printf("%s \n", q->info);
		}	
		numtab1--; numtab1--;
	}
	else{	//输出程序
		for(i = 0; i < numtab; i++) printf("    ");//缩进
		p = root->left;	
		switch(p->type){ //输出变量类型
			case INT: printf("int "); break;
			case FLOAT: printf("float "); break;
			case CHAR: printf("char "); break;
		}
		for(p = root->right; p != NULL; p = p->right){
			q = p->left;	//输出变量名
			printf("%s", q->info);
			if(p->right != NULL) printf(", ");
		}
		printf(";\n");
	}
	return OK;
}
/*******************************************
  	函数名称：OutFuncDef
	函数参数：树根结点
	函数功能：输出函数或其语法树
	返回值：OK or ERROR
*******************************************/
int OutFuncDef(pTree root){
	int i = 0;
	pTree p = (pTree)malloc(sizeof(Tree));
	pTree q = (pTree)malloc(sizeof(Tree));
	pTree m = (pTree)malloc(sizeof(Tree));
	if(flag){
		for(i = 0; i < numtab1; i++) printf("    ");//缩进
		p = root->right;
		if(p == NULL) printf("函数声明：\n");//函数声明
		else printf("函数定义：\n");
		numtab1++;
		for(i = 0; i < numtab1; i++) printf("    ");//缩进
		printf("类型：");
		p = root->left;
		switch(p->type){	//输出函数类型
			case INT: printf("int \n"); break;
			case FLOAT: printf("float \n"); break;
			case CHAR: printf("char \n"); break;
		}
		for(i = 0; i < numtab1; i++) printf("    ");//缩进
		printf("函数名：");
		p = root->middle;
		printf("%s \n", p->info); //输出函数名
		for(i = 0; i < numtab1; i++) printf("    ");//缩进
		printf("函数形参：\n");
		p = p->middle;
		if(p == NULL) printf("\n"); //无形参
		else{
			numtab1++;
			for(; p != NULL; p = p->right){
				q = p->left;
				m = q->left;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("类型：");//输出形参
				switch(m->type){ //输出形参类型
					case INT: printf("int    "); break;
					case FLOAT: printf("float    "); break;
					case CHAR: printf("char    "); break;				
				}
				printf("参数名：");
				m = q->right; //输出形参名
				printf("%s\n", m->info); 
			}
			numtab1--;
		}
		p = root->right;
		if(p != NULL) OutCompSen(p);
		numtab1--;
	}
	else{	
		for(i = 0; i < numtab; i++) printf("    ");//缩进
		p = root->left;
		switch(p->type){	//输出函数类型
			case INT: printf("int "); break;
			case FLOAT: printf("float "); break;
			case CHAR: printf("char "); break;
		}
		p = root->middle;
		printf("%s", p->info); //输出函数名
		p = p->middle;
		if(p == NULL) printf("()"); //无形参
		else {
			printf("(");//输出形参
			for(; p != NULL; p = p->right){
				q = p->left;
				m = q->left;
				switch(m->type){ //输出形参类型
					case INT: printf("int "); break;
					case FLOAT: printf("float "); break;
					case CHAR: printf("char "); break;				
				}
				m = q->right; //输出形参名
				printf("%s", m->info); 
				if(p->right != NULL) printf(", ");
			}
			printf(")");
		}
		p = root->right;
		if(p == NULL) printf(";\n");//函数声明
		else{		//函数定义，输出函数体
			printf("{\n");
			numtab++;
			OutCompSen(p);
			numtab--;
			for(i = 0; i < numtab; i++) printf("    ");//缩进
			printf("}\n");
		}
	}
	return OK;
}
/*******************************************
  	函数名称：OutCompSen
	函数参数：树根结点
	函数功能：输出复合语句或其语法树
	返回值：OK or ERROR
*******************************************/
int OutCompSen(pTree root){
	int i;
	pTree p = (pTree)malloc(sizeof(Tree));
	pTree q = (pTree)malloc(sizeof(Tree));
	if(flag){
		for(i = 0; i < numtab1; i++) printf("    ");//缩进
		printf("复合语句：\n");
		if(root == NULL){
			printf("\n");
		}
		else{
			numtab1++;
			for(i = 0; i < numtab1; i++) printf("    ");//缩进
			printf("复合语句的变量定义：\n"); numtab1++;
			p = root->left;
			if(p == NULL) printf("\n");
			else{ //输出局部变量
				for(; p != NULL; p = p->right){
					q = p->left;
					q->type = LOCVARDEF;
					OutExtVarDef(q);
				}				
			}
			numtab1--;
			for(i = 0; i < numtab1; i++) printf("    ");//缩进
			printf("复合语句的语句部分：\n"); numtab1++;
			p = root->right;
			if(p == NULL) printf("\n");
			else{
				for(; p != NULL; p = p->right){ //输出语句序列
					q = p->left;
					OutSentence(q); //输出语句
				}
			}
			numtab1--;
		}
	}
	else{	
		p = root->left;
		if(p != NULL) { //输出内部变量
			for(; p != NULL; p = p->right){
				q = p->left;
				q->type = LOCVARDEF;
				OutExtVarDef(q);
			}
				
		}
		p = root->right;
		for(; p != NULL; p = p->right){ //输出语句序列
			q = p->left;
			OutSentence(q); //输出语句
		}
	}	
	return OK;
}
/*******************************************
  	函数名称：OutSentence
	函数参数：树根结点
	函数功能：输出语句或其语法树
	返回值：OK or ERROR
*******************************************/
int OutSentence(pTree root){
	if(root == NULL) return ERROR;
	int i;
	pTree p = (pTree)malloc(sizeof(Tree));
	pTree q = (pTree)malloc(sizeof(Tree));
	switch(root->type){
		case IF: //输出if语句或其语法树
			if(flag){//flag为1则输出if语句的语法树
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("条件语句");
				if(root->right == NULL) printf("(IF_THEN):\n");
				else printf("(IF_THEN_ELSE):\n");
				numtab1++;
				p = root->left;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("条件：\n");
				numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				if(p != NULL) OutExp(p->middle); //输出条件表达式
				numtab1--;
				printf("\n");
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("IF子句：\n");
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
					for(i = 0; i < numtab1; i++) printf("    ");//缩进
					printf("ELSE子句：\n");
					numtab1++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab1--;
				}
				numtab1--;
			}
			else{	//flag为0则输出if语句的代码
				for(i = 0; i < numtab; i++) printf("    ");//缩进
				printf("if(");
				p = root->left; //输出条件表达式
				if(p != NULL) OutExp(p->middle);
				printf(")");
				p = root->middle;//输出IF子句
				if(p != NULL) {
					printf("{\n");
					numtab++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab--;
					for(i = 0; i < numtab; i++) printf("    ");//缩进
					printf("}\n");
				}
				else printf("\n");
				p = root->right;
				if(p != NULL){
					for(i = 0; i < numtab; i++) printf("    ");//缩进
					printf("else{\n");
					numtab++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab--;
					for(i = 0; i < numtab; i++) printf("    ");//缩进
					printf("}\n");
				}
			}
			break;
		case WHILE://输出while语句或其语法树
			if(flag){//flag为1则输出while语句的语法树
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("while循环语句：\n"); numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("条件：\n"); numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				p = root->left;
				if(p != NULL) OutExp(p->middle); //输出条件表达式
				numtab1--;
				printf("\n");
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("while子句：\n");
				p = root->middle;//输出WHILE子句
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
			else{	//flag为0则输出while语句
				for(i = 0; i < numtab; i++) printf("    ");//缩进
				printf("while(");
				p = root->left; //输出条件表达式
				if(p != NULL) OutExp(p->middle);
				printf(")");
				p = root->middle;//输出WHILE子句
				if(p != NULL) {
					printf("{\n");
					numtab++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab--;
					for(i = 0; i < numtab; i++) printf("    ");//缩进
					printf("}\n");
				}
				else printf(";\n");
			}
			break;
		case FOR: //输出for语句或其语法树
			if(flag){//flag为1则输出for语句的语法树
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("for循环语句：\n"); numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("条件：\n"); numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				p = root->left; //输出三个条件表达式
				if(p->left != NULL) OutExp(p->left); 
				printf("\n");
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				if(p->middle != NULL) OutExp(p->middle);
				printf("\n");
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				if(p->right != NULL) OutExp(p->right);
				printf("\n");
				numtab1--;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("for子句：\n"); numtab1++;
				
				p = root->middle;//输出FOR子句
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
			else{	 //flag为0则输出for语句
				for(i = 0; i < numtab; i++) printf("    ");//缩进
				printf("for(");
				p = root->left; //输出三个条件表达式
				if(p->left != NULL) OutExp(p->left); 
				printf("; ");
				if(p->middle != NULL) OutExp(p->middle);
				printf(";");
				if(p->right != NULL) OutExp(p->right);
				printf(")");
				p = root->middle;//输出FOR子句
				if(p != NULL) {
					printf("{\n");
					numtab++;
					for(; p != NULL; p = p->right){
						q = p->left;
						OutSentence(q);
					}
					numtab--;
					for(i = 0; i < numtab; i++) printf("    ");//缩进
					printf("}\n");
				}
				else printf(";\n");
			}
			break;
		case EXP: //输出表达式或其语法树
			if(flag){ //flag为1输出表达式的语法树
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("表达式语句：\n"); numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				OutExp(root->middle);
				printf("\n");
				numtab1--;
			}
			else{	//flag为0则输出表达式
				for(i = 0; i < numtab; i++) printf("    ");//缩进
				OutExp(root->middle);
				printf(";\n");
			}
			break;
		case CONTINUE: //输出continue语句或其语法树
			if(flag){ //flag为1则输出continue语句的语法树
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("continue语句：\n");
				numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("continue;\n");
				numtab1--;
			}
			else{	//flag为0则输出continue语句
				for(i = 0; i < numtab; i++) printf("    ");//缩进
				printf("continue;\n");}
			break;
		case BREAK: //输出break语句或其语法树
			if(flag){ //flag为1则输出break语句的语法树
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("break语句：\n");
				numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("break;\n");
				numtab1--;
			}
			else{ //flag为0则输出break语句
				for(i = 0; i < numtab; i++) printf("    ");//缩进
				printf("break;\n");
			}
			break;
		case RETURN://输出return语句或其语法树
			if(flag){ //flag为1则输出return语句的语法树
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				printf("return语句：\n");
				numtab1++;
				for(i = 0; i < numtab1; i++) printf("    ");//缩进
				OutExp(root->middle);
				numtab1--;
				printf("\n");
			}
			else{	//flag为0则输出return语句
				for(i = 0; i < numtab; i++) printf("    ");//缩进
				printf("return ");
				OutExp(root->middle);
				printf(";\n");
			}
			break;
	}	
	return OK;
}
/*******************************************
  	函数名称：OutExp
	函数参数：树根结点
	函数功能：输出表达式
	返回值：OK or ERROR
*******************************************/
int OutExp(pTree T) { //输出表达式
	if(T == NULL) return OK;
	outexp(T->left); //中序遍历左子树
	if(T->type == OPN) printf("%s ", T->info); //输出结点信息
	else{
		switch (T->type){ //输出运算符
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
	outexp(T->right); //中序遍历右子树
	return OK;
}
/*******************************************
  	函数名称：outexp
	函数参数：树根结点
	函数功能：输出表达式递归子程序，辅助输出表达式
	返回值：OK or ERROR
*******************************************/
int outexp(pTree T) { //输出表达式递归子程序
	if(T == NULL) return OK;
	if(T->left != NULL && T->right != NULL) printf("( ");
	outexp(T->left); //中序遍历左子树
	if(T->type == OPN) printf("%s ", T->info); //输出结点信息
	else{
		switch (T->type){ //输出运算符
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
	outexp(T->right); //中序遍历右子树
	if(T->left != NULL && T->right != NULL) printf(") ");
	return OK;
}
