#include<iostream>
#include<string>
#include<stdlib.h>
#include<ctime>
#include<deque>
#include<stack>
#include<fstream>
#include<windows.h>
using namespace std;
bool isOK=false;//判断生成的算式是否合法

//计算类
class Calculate{
	public:
	deque<string> coll1;  //盛放中缀表达式  
	stack<string> coll2;  //盛放操作符  
	deque<string> coll3;    //盛放后缀表达式  
	stack<int>	coll4;  //计算后缀表达式的辅助容器

	//判断是否是运算符
	bool isOperator(string c)
	{
		if(c=="+" || c=="-" || c=="*" || c=="/" || c=="(" || c==")")
			return true;
		return false;
	}

	//是否是括号
	bool isPra(string c)   
	{  
		if(c=="("||c==")")   
			return true;   
		else   
			return false;  
	}  
  
	//获得符号的优先性  
	int getPri(string c)   
	{  
		switch(c[0])   
		{  
		case '+':  
		case '-':  
			return 0;   //如果是加减，返回0  
			break;  
		case '*':  
		case '/':  
			return 1;   //如果是乘除，返回1  
			break;  
		case '(':  
		case ')':  
			return -1;  //注意，这里将括号设为最低优先级，因此括号不会被弹出，除非遇到右括号  
			break;  
		 }  
	}  
  
	//判断符号的优先性  
	void check(string c, stack<string>& coll2, deque<string>& coll3)   
	{    
		if(coll2.empty())   
		{  
			coll2.push(c);  
			return;  
		}  
   
		if(isPra(c))   
		{  
			if(c=="(")   
				coll2.push(c);  
			else   
			{  
				//弹出所有元素直到遇到左括号  
				while(coll2.top()!="(")   
				{    
					string ch = coll2.top();  
					coll3.push_back(ch);  
					coll2.pop();  
				}  
  
				//当遇到左括号时，弹出但不加入coll3(后缀表达式中）  
				coll2.pop();    
			}  
		}  
		else    //如果不是括号  
		{  
			//取出栈顶元素，与当前符号进行优先性比较  
			string sym = coll2.top();    
  
			//比较两符号的优先性  
			if(getPri(c)<=getPri(sym))    
			{  
				//如果c的优先性比栈顶符号小或等于，弹出栈顶元素  
				coll2.pop();  
				//并将其压入coll3（后缀表达式）中  
				coll3.push_back(sym);  
				//递归调用check,比较当前符号c与下一个栈顶符号的优先性  
				check(c,coll2,coll3);     
			}  
			else   
			{  
				//如果c比栈顶符号优先级大，那将c压入coll2(操作符栈）中  
				coll2.push(c);    
			}  
		}  
	}  
  
	//从coll中取出元素，分配元素到coll2和coll3中  
	void allocate()   
	{    
		while(!coll1.empty())   
		{  
			string c = coll1.front();  
			coll1.pop_front();  
  
			if(!isOperator(c))  
			{  
				coll3.push_back(c);  
			}  
			else   
			{  
				//调用check函数，针对不同情况作出不同操作  
				check(c,coll2,coll3);    
			}  
  
		}  
  
		//如果输入结束，将coll2的元素全部弹出，加入后缀表达式中  
		while(!coll2.empty())   
		{    
			string c = coll2.top();  
			coll3.push_back(c);  
			coll2.pop();  
		}  
	}  
  
	//计算后缀表达式  
	void calculate()   
	{    
		while(!coll3.empty())   
		{  
			string c = coll3.front();  
			coll3.pop_front();  
          
			//如果是操作数，压入栈中  
			if(!isOperator(c))   
			{  
				//减去'0'得出偏移值，即为真实数值（如果直接转换成int，结果不对，因为char 转换为int是其编码值，例如'1'的编码值为49  
				int op =atoi(c.c_str());      
				coll4.push(op);       
			}  
			else     //如果是操作符，从栈中弹出元素进行计算  
			{   
				int op1 = coll4.top();  
				coll4.pop();  
				int op2 = coll4.top();  
				coll4.pop();  
				switch(c[0])   
				{  
				case '+':  
					coll4.push(op2+op1);  
					break;  
				case '-':  
					if(op2<op1)
						return;
					coll4.push(op2-op1);  
					break;  
				case '*':  
					coll4.push(op2*op1);  
					break;  
				case '/':  
					if(op1==0 || op2%op1!=0)
						return;
					coll4.push(op2/op1);  //注意是op2(op)op1而不是op1(op)op2  
					break;  
				}  
			}  
		}  
		isOK=true;
	}
};
//生成算式输出算式类
class OutputAri{
	public:
	int N,R;
	string *question;//题目
	int Num[4];//题目中含有的数字（随机生成）
	int operCharNumber;//题目中包含运算符的数量（随机生成，最多为3，最少为1）
	char operatorChar[4];//题目中含有的运算符（随机生成）
	int **addCover;//用二维数组判断一个加法运算符的重复
	int **mulCover;//用二维数组判断一个乘法运算符的重复
	int one;//用一维数组遍历的方法判断一位运算符减和除的重复
	string *oneOperator;
	int two;//用一维数组遍历的方法判断二个运算符的重复
	string *twoOperator;
	int three;//用一维数组遍历的方法判断三个运算符的重复
	string *threeOperator;
	string operString;
	bool twoCover;
	bool threeCover;
	bool cover;

	OutputAri(string n,string r){
		N=atoi(n.c_str());
		R=atoi(r.c_str());
		question=new string[N];
		addCover=new int*[R];
		mulCover=new int*[R];
		for(int i=0;i<R;i++){
			addCover[i]=new int[R];
			mulCover[i]=new int[R];
			memset(addCover[i], 0, sizeof(int)*R);
			memset(mulCover[i], 0, sizeof(int)*R);
		}
		one=0;
		oneOperator=new string[N];
		two=0;
		twoOperator=new string[N];
		three=0;
		threeOperator=new string[N];
	}

	void bulidAri(){
		operString="";
		//随机生成数字
		for(int j=0;j<=operCharNumber;j++){
			Num[j]=rand()%R;
		}
		//随机生成运算符
		for(int j=0;j<operCharNumber;j++){
			switch((rand()%4)+1){
				case 1:operatorChar[j]='+';break;
				case 2:operatorChar[j]='-';break;
				case 3:operatorChar[j]='*';break;
				case 4:operatorChar[j]='/';break;
			}
			operString+=operatorChar[j];
		}
	}

	//将数字与运算符整合成算式并将元素压入桟
	string addParentheses(int i,deque<string>& coll1)
	{
		twoCover=false;
		threeCover=false;
		string t="";
		bool first=false;
		bool second=false;
		bool third=false;
		//当运算符为3个的时候加括号算法
		if(operString.length()==3){
			if(firstOper() && rand()%2==1){
				t+=intToString(Num[0])+operString[0]+'('+intToString(Num[1])+operString[1]+intToString(Num[2])+operString[2]+intToString(Num[3])+')';
				coll1.push_back(intToString(Num[0]));
				coll1.push_back(charToString(operString[0]));
				coll1.push_back("(");
				coll1.push_back(intToString(Num[1]));
				coll1.push_back(charToString(operString[1]));
				coll1.push_back(intToString(Num[2]));
				coll1.push_back(charToString(operString[2]));
				coll1.push_back(intToString(Num[3]));
				coll1.push_back(")");
				third=true;
			}
			if(!third && threeOper() && rand()%2==1){
				t+='('+intToString(Num[0])+operString[0]+intToString(Num[1])+operString[1]+intToString(Num[2])+')'+operString[2]+intToString(Num[3]);
				coll1.push_back("(");
				coll1.push_back(intToString(Num[0]));
				coll1.push_back(charToString(operString[0]));
				coll1.push_back(intToString(Num[1]));
				coll1.push_back(charToString(operString[1]));
				coll1.push_back(intToString(Num[2]));
				coll1.push_back(")");
				coll1.push_back(charToString(operString[2]));
				coll1.push_back(intToString(Num[3]));
				third=true;
			}
			if(!third && ykykykk() && rand()%2==1){
				t+=intToString(Num[0])+operString[0]+'('+intToString(Num[1])+operString[1]+'('+intToString(Num[2])+operString[2]+intToString(Num[3])+')'+')';
				coll1.push_back(intToString(Num[0]));
				coll1.push_back(charToString(operString[0]));
				coll1.push_back("(");
				coll1.push_back(intToString(Num[1]));
				coll1.push_back(charToString(operString[1]));
				coll1.push_back("(");
				coll1.push_back(intToString(Num[2]));
				coll1.push_back(charToString(operString[2]));
				coll1.push_back(intToString(Num[3]));
				coll1.push_back(")");
				coll1.push_back(")");
				third=true;
			}
			if(!third && ykkykyk() && rand()%2==1){
				t+=intToString(Num[0])+operString[0]+'('+'('+intToString(Num[1])+operString[1]+intToString(Num[2])+')'+operString[2]+intToString(Num[3])+')';
				coll1.push_back(intToString(Num[0]));
				coll1.push_back(charToString(operString[0]));
				coll1.push_back("(");
				coll1.push_back("(");
				coll1.push_back(intToString(Num[1]));
				coll1.push_back(charToString(operString[1]));
				coll1.push_back(intToString(Num[2]));
				coll1.push_back(")");
				coll1.push_back(charToString(operString[2]));
				coll1.push_back(intToString(Num[3]));
				coll1.push_back(")");
				third=true;
			}
			if(!third && kykykky() && rand()%2==1){
				t+='('+intToString(Num[0])+operString[0]+'('+intToString(Num[1])+operString[1]+intToString(Num[2])+')'+')'+operString[2]+intToString(Num[3]);
				coll1.push_back("(");
				coll1.push_back(intToString(Num[0]));
				coll1.push_back(charToString(operString[0]));
				coll1.push_back("(");
				coll1.push_back(intToString(Num[1]));
				coll1.push_back(charToString(operString[1]));
				coll1.push_back(intToString(Num[2]));
				coll1.push_back(")");
				coll1.push_back(")");
				coll1.push_back(charToString(operString[2]));
				coll1.push_back(intToString(Num[3]));
				third=true;
			}
		}
		//当不符合上面的情况没有加括号时进行下面的加括号算法
		if(!third){
			//判断第一个运算符能否加括号
			if(rand()%2==1 && rightOper(0)){
				first=true;
				t+='('+intToString(Num[0])+operString[0]+intToString(Num[1])+')';
				coll1.push_back("(");
				coll1.push_back(intToString(Num[0]));
				coll1.push_back(charToString(operString[0]));
				coll1.push_back(intToString(Num[1]));
				coll1.push_back(")");
			}
			else{
				t+=intToString(Num[0])+operString[0];
				coll1.push_back(intToString(Num[0]));
				coll1.push_back(charToString(operString[0]));
			}
			//判断第二个运算符能否加括号
			if(rand()%2==1 && !first && (leftOper(1) || (operString.length()==2 && rightOper(1)))){
				second=true;
				t+='('+intToString(Num[1])+operString[1]+intToString(Num[2])+')';
				coll1.push_back("(");
				coll1.push_back(intToString(Num[1]));
				coll1.push_back(charToString(operString[1]));
				coll1.push_back(intToString(Num[2]));
				coll1.push_back(")");
			}
			else if(first){
				t+=operString[1];
				coll1.push_back(charToString(operString[1]));
			}
			else{
				t+=intToString(Num[1])+operString[1];
				coll1.push_back(intToString(Num[1]));
				coll1.push_back(charToString(operString[1]));
			}
			if(operString.length()==3){
				if(rand()%2==1 && !second && leftOper(2)){
					t+='('+intToString(Num[2])+operString[2]+intToString(Num[3])+')';
					coll1.push_back("(");
					coll1.push_back(intToString(Num[2]));
					coll1.push_back(charToString(operString[2]));
					coll1.push_back(intToString(Num[3]));
					coll1.push_back(")");
				}
				else if(!second){
					t+=intToString(Num[2])+operString[2]+intToString(Num[3]);
					coll1.push_back(intToString(Num[2]));
					coll1.push_back(charToString(operString[2]));
					coll1.push_back(intToString(Num[3]));
				}
				else{
					t+=operString[2]+intToString(Num[3]);
					coll1.push_back(charToString(operString[2]));
					coll1.push_back(intToString(Num[3]));
				}
			}
			else if(!second){
				t+=intToString(Num[2]);
				coll1.push_back(intToString(Num[2]));
			}
		}
		//cout<<operString<<"      "<<t<<endl;
		return t;
	}

	//判断两个和三个运算符是否重复
	void coverGtOne(int i){
		if(operString.length()==2){
			for(int j=0;j<two;j++){
				if(twoOperator[j]==question[i]){
					twoCover=true;
					break;
				}
			}
		}
		else
		{
			for(int j=0;j<three;j++){
				if(threeOperator[j]==question[i]){
					threeCover=true;
					break;
				}
			}
		}
	}

	//判断一个除或减运算符是否重复
	void coverOne(int i){
		if(operatorChar[0]=='/' || operatorChar[0]=='-'){
			for(int j=0;j<one;j++){
				if(oneOperator[j]==question[i]){
					cover=true;
					break;
				}
			}
		}
	}

	//判断一个运算符是否合法
	void legalOne(){
		if(operatorChar[0]=='/'){
			while(Num[1]==0){
				Num[1]=rand()%R;
			}
		}
		if(operatorChar[0]=='/'){
			while(Num[0]%Num[1]!=0){
				Num[1]=rand()%R;
				while(Num[1]==0){
					Num[1]=rand()%R;
				}
				Num[0]=rand()%R;
			}
		}
		if(operatorChar[0]=='-'){
			while(Num[0]<Num[1]){
				if(Num[1]>=R/2 && Num[0]>=R/2){
					Num[1]=rand()%R;
				}
				else if(Num[1]>=R/2 && Num[0]<=R/2){
					Num[1]=rand()%R;
					Num[0]=rand()%R;
				}
				else{
					Num[0]=rand()%R;
				}
			}
		}
		switch(operatorChar[0]){
			case '+':
				while(repeatAddCover(0)){
					Num[0]=rand()%R;
					Num[1]=rand()%R;
					}
				break;
			case '*':
				while(repeatMulCover(0)){
					Num[0]=rand()%R;
					Num[1]=rand()%R;
					}
				break;
		}
	}

	//通过判断运算符右边的运算符来判断这个运算符能否加括号
	bool rightOper(int pos)
	{
		if((operString[pos]=='+' || operString[pos]=='-') && (operString[pos+1]=='*' || operString[pos+1]=='/'))
			return true;
		return false;
	}

	//通过判断运算符左边的运算符来判断这个运算符能否加括号
	bool leftOper(int pos)
	{
		if(((operString[pos]=='+' || operString[pos]=='-') && operString[pos-1]=='*') || operString[pos-1]=='/')
			return true;
		return false;
	}

	//整形转string
	string intToString(int a){
		char turnChar[10];
		sprintf(turnChar,"%d",a);
		return turnChar;
	}

	//char转string
	string charToString(char c)
	{
		string t(&c);
		return t;
	}

	//判断加法重复
	bool repeatAddCover(int left)
	{
		if(addCover[Num[left]][Num[left+1]]==1)
			return false;
		addCover[Num[left]][Num[left+1]]=1;
		addCover[Num[left+1]][Num[left]]=1;
		return true;
	}

	//判断乘法重复
	bool repeatMulCover(int left)
	{
		if(mulCover[Num[left]][Num[left+1]]==1)
			return false;
		mulCover[Num[left]][Num[left+1]]=1;
		mulCover[Num[left+1]][Num[left]]=1;
		return true;
	}

	//当运算符为3个时判断能否为 运算符（运算符运算符） （运算符运算符）运算符 
	bool firstOper()
	{
		if((operString[0]=='-' && (operString[1]=='+' || operString[1]=='-')) || operString[0]=='/')
			return true;
		if(operString[0]=='*' && (operString[1]=='+' || operString[1]=='-'))
			return true;
		return false;
	}
	bool threeOper()
	{
		if((operString[2]=='*' || operString[2]=='/') && (operString[1]=='+' || operString[1]=='-')  && (operString[0]=='+' || operString[0]=='-'))
			return true;
		return false;
	}
	//运算符（运算符（运算符））
	bool ykykykk()
	{
		if(operString[0]=='-' || operString[0]=='/' || operString[0]=='*')
			if(operString[1]=='-' || operString[1]=='/' || operString[1]=='*')
				if(operString[2]=='-' || operString[2]=='+')
					return true;
		return false;
	}
	//运算符（（运算符）运算符）
	bool ykkykyk()
	{
		if(operString[0]=='/' || operString[0]=='*')
			if(operString[1]=='-' || operString[1]=='+')
				if(operString[2]=='*' || operString[2]=='/')
					return true;
		return false;
	}
	//（运算符（运算符））运算符
	bool kykykky(){
		if(operString[0]=='-')
			if(operString[1]=='-' || operString[1]=='+')
				if(operString[2]=='*' || operString[2]=='/')
					return true;
		return false;
	}
};

//读取文本算式并输出结果类
class InputAri{
	public:
	string strQuestion[10000]; 
	string strAnswer[10000];
	int correct[10000];
	int cor;
	int wrong[10000];
	int wro;
	InputAri(){
		cor=0;
		wro=0;
	}
	void inDeque(string question,deque<string>& coll1){
		string s="";
		for(int k=0;k!=question.length()-1;++k)
		{  
			//逐一加入每个字符，这里使用deque因为deque在两端删除添加的速度最快
			if(question[k]=='('){
				coll1.push_back("(");
				continue;
			}
			if(isOperatorForChar(question[k])){
				if(s!="")
					coll1.push_back(s);
				string str=charToString(question[k]);
				coll1.push_back(str);
				s="";
				continue;
			}
			s+=question[k];
		}  
		if(question[question.length()-2]!=')')
			coll1.push_back(s);
	}

	//判断是否是运算符（参数：char）
	bool isOperatorForChar(char c)
	{
		if(c=='+' || c=='-' || c=='*' || c=='/' || c=='(' || c==')')
			return true;
		return false;
	}

	//char转string
	string charToString(char c)
	{
		string t(&c);
		return t;
	}
};

//文件操作类
class OperFile{
	public:
	ofstream oufQuestion;
	ofstream oufAnswer;
	fstream openQuestion;
	fstream openAnswer;
	ofstream grade;
	string programDir;
	OperFile(){
		programDir=GetProgramDir();
	}
	//获取执行文件路径
	string GetProgramDir()  
	{   
		char exeFullPath[MAX_PATH]; // Full path
		string strPath = "";
		GetModuleFileName(NULL,exeFullPath,MAX_PATH);
		strPath=(string)exeFullPath;    // Get full path of the file
		int pos = strPath.find_last_of('\\', strPath.length());
		strPath=strPath.substr(0, pos);
		int t=1,j=0;
		string s="";
		for(int i=0;i<strPath.length();i++){
			if(strPath[i]=='\\'){
				s+=strPath.substr(j,t);
				s+="\\";
				t=0;
				j=i+1;
			}
			t++;
		}
		s+=strPath.substr(j,t);
		strPath=s;
		strPath+="\\";
		strPath+="\\";
		return strPath;  // Return the directory without the file name
	}
};

//初始化输入数据类
class InitInput{
	public:
	void init(int argc,char *argv[],string &n,string &r,string &e,string &a){
		for(int i=1;i<argc;i++){
			string s(argv[i]);
			if(s=="-n"){
				if(i+1<argc){
					string str(argv[i+1]);
					if(str!="-n" && str!="-r" && str!="-e" && str!="-a"){
						n=str;
					}
					if(n==""){
						cout<<"请输入-n的值！";
						return;
					}
				}
			}
			if(s=="-r"){
				if(i+1<argc){
					string str(argv[i+1]);
					if(str!="-n" && str!="-r" && str!="-e" && str!="-a"){
						r=str;
					}
					if(r==""){
						cout<<"请输入-r的值！";
						return;
					}
				}
			}
			if(s=="-e"){
				if(i+1<argc){
					string str(argv[i+1]);
					if(str!="-n" && str!="-r" && str!="-e" && str!="-a"){
						e=str;
					}
					if(e==""){
						cout<<"请输入-e的值！";
						return;
					}
				}
			}
			if(s=="-a"){
				if(i+1<argc){
					string str(argv[i+1]);
					if(str!="-n" && str!="-r" && str!="-e" && str!="-a"){
						a=str;
					}
					if(a==""){
						cout<<"请输入-a的值！";
						return;
					}
				}
			}
		}
	}
};

int main(int argc,char *argv[])
{
	string n="",r="",e="",a="";
	InitInput initInput;
	initInput.init(argc,argv,n,r,e,a);
	OperFile operFile;
	if(n!="" && r!=""){
		operFile.oufQuestion.open(operFile.programDir+"Exercises.txt");
		operFile.oufAnswer.open(operFile.programDir+"Answers.txt");
		OutputAri outputAri(n,r);
		srand((unsigned)time(NULL));
		for(int i=0;i<outputAri.N;i++){
			isOK=false;//判断算式是否合法，不合法则重新生成算式
			outputAri.operCharNumber=(rand()%3)+1;
			while(!isOK){
				outputAri.bulidAri();
				Calculate cal;
				if(outputAri.operString.length()>1){
					outputAri.question[i]=outputAri.addParentheses(i,cal.coll1)+"=";
					outputAri.coverGtOne(i);
					if(outputAri.twoCover || outputAri.threeCover)
						continue;
					cal.allocate();
					cal.calculate();
					if(isOK)
					{
						if(outputAri.operString.length()==2)
						{
							outputAri.twoOperator[outputAri.two]=outputAri.question[i];
							outputAri.two++;
						}
						else
						{
							outputAri.threeOperator[outputAri.three]=outputAri.question[i];
							outputAri.three++;
						}
						operFile.oufQuestion<<i+1<<'.'<<outputAri.question[i]<<'\n';
						operFile.oufAnswer<<i+1<<'.'<<cal.coll4.top()<<'\n';
						//cout<<"题目："<<i<<"   "<<question[i]<<coll4.top()<<endl;
					}
				}
				else{
					outputAri.cover=false;
					outputAri.legalOne();
					outputAri.question[i]=outputAri.intToString(outputAri.Num[0])+outputAri.operatorChar[0]+outputAri.intToString(outputAri.Num[1])+"=";
					outputAri.coverOne(i);
					if(outputAri.cover)
						continue;
					outputAri.oneOperator[outputAri.one]=outputAri.question[i];
					outputAri.one++;
					int result=-1;
					switch(outputAri.operatorChar[0]){
						case '+':result=outputAri.Num[0]+outputAri.Num[1];break;
						case '-':result=outputAri.Num[0]-outputAri.Num[1];break;
						case '*':result=outputAri.Num[0]*outputAri.Num[1];break;
						case '/':result=outputAri.Num[0]/outputAri.Num[1];break;
					}
					isOK=true;
					operFile.oufQuestion<<i+1<<'.'<<outputAri.question[i]<<'\n';
					operFile.oufAnswer<<i+1<<'.'<<result<<'\n';
					//cout<<"题目："<<i<<"   "<<question[i]<<result<<endl;
				}
			}
		}
		operFile.oufQuestion.close();
		operFile.oufAnswer.close();
		cout<<"文件输出成功！";
	}
	else if(e!="" && a!=""){
		InputAri inputAri;
		operFile.openQuestion.open(operFile.programDir+e,ios::in);
		if(!operFile.openQuestion.good()){
			cout<<e+"文件打开失败";
			return 0;
		}
		operFile.openAnswer.open(operFile.programDir+a,ios::in);
		if(!operFile.openAnswer.good()){
			cout<<a+"文件打开失败";
			return 0;
		}
		int i=0;
		while(!operFile.openQuestion.eof()){
			char c[100];
			operFile.openQuestion.getline(c,100,'\n');
			inputAri.strQuestion[i]=c;
			char c1[100];
			operFile.openAnswer.getline(c1,100,'\n');
			inputAri.strAnswer[i]=c1;
			i++;
		}
		operFile.openQuestion.close();
		operFile.openAnswer.close();
		for(int j=0;j<i;j++){
			string question,answer;
			for(int k=0;k<inputAri.strQuestion[j].length();k++){
				if(inputAri.strQuestion[j][k]=='.'){
					inputAri.strQuestion[j]=inputAri.strQuestion[j].substr(k+1);
					break;
				}
			}
			question=inputAri.strQuestion[j];
			for(int k=0;k<inputAri.strAnswer[j].length();k++){
				if(inputAri.strAnswer[j][k]=='.'){
					inputAri.strAnswer[j]=inputAri.strAnswer[j].substr(k+1);
					break;
				}
			}
			answer=inputAri.strAnswer[j];
			Calculate cal;
			bool b=false;
			if(question!=""){
				inputAri.inDeque(question,cal.coll1);
				//从coll中取出元素，分配元素到coll2和coll3中 
				cal.allocate();
				//计算后缀表达式  
				cal.calculate();
				if(cal.coll4.top()==atoi(answer.c_str())){
					inputAri.correct[inputAri.cor]=j+1;
					inputAri.cor++;
				}
				else{
					inputAri.wrong[inputAri.wro]=j+1;
					inputAri.wro++;
				}
			}
		}
		operFile.grade.open(operFile.programDir+"Grade.txt");
		operFile.grade<<"Correct:"<<inputAri.cor<<"(";
		for(int j=0;j<inputAri.cor;j++){
			operFile.grade<<inputAri.correct[j];
			if(j!=inputAri.cor-1)
				operFile.grade<<",";
		}
		operFile.grade<<")"<<'\n';
		operFile.grade<<"Wrong:"<<inputAri.wro<<"(";
		for(int j=0;j<inputAri.wro;j++){
			operFile.grade<<inputAri.wrong[j];
			if(j!=inputAri.wro-1)
				operFile.grade<<",";
		}
		operFile.grade<<")";
		operFile.grade.close();
		cout<<"文件输出成功！";
	}
	else{
		cout<<"请输入正确参数！";
	}
	return 0;
}