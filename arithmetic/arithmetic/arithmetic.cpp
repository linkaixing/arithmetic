#include<iostream>
#include<string>
#include<stdlib.h>
#include<ctime>
#include<deque>
#include<stack>
#include<fstream>
#include<windows.h>
using namespace std;
bool isOK=false;//�ж����ɵ���ʽ�Ƿ�Ϸ�

//������
class Calculate{
	public:
	deque<string> coll1;  //ʢ����׺���ʽ  
	stack<string> coll2;  //ʢ�Ų�����  
	deque<string> coll3;    //ʢ�ź�׺���ʽ  
	stack<int>	coll4;  //�����׺���ʽ�ĸ�������

	//�ж��Ƿ��������
	bool isOperator(string c)
	{
		if(c=="+" || c=="-" || c=="*" || c=="/" || c=="(" || c==")")
			return true;
		return false;
	}

	//�Ƿ�������
	bool isPra(string c)   
	{  
		if(c=="("||c==")")   
			return true;   
		else   
			return false;  
	}  
  
	//��÷��ŵ�������  
	int getPri(string c)   
	{  
		switch(c[0])   
		{  
		case '+':  
		case '-':  
			return 0;   //����ǼӼ�������0  
			break;  
		case '*':  
		case '/':  
			return 1;   //����ǳ˳�������1  
			break;  
		case '(':  
		case ')':  
			return -1;  //ע�⣬���ｫ������Ϊ������ȼ���������Ų��ᱻ��������������������  
			break;  
		 }  
	}  
  
	//�жϷ��ŵ�������  
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
				//��������Ԫ��ֱ������������  
				while(coll2.top()!="(")   
				{    
					string ch = coll2.top();  
					coll3.push_back(ch);  
					coll2.pop();  
				}  
  
				//������������ʱ��������������coll3(��׺���ʽ�У�  
				coll2.pop();    
			}  
		}  
		else    //�����������  
		{  
			//ȡ��ջ��Ԫ�أ��뵱ǰ���Ž��������ԱȽ�  
			string sym = coll2.top();    
  
			//�Ƚ������ŵ�������  
			if(getPri(c)<=getPri(sym))    
			{  
				//���c�������Ա�ջ������С����ڣ�����ջ��Ԫ��  
				coll2.pop();  
				//������ѹ��coll3����׺���ʽ����  
				coll3.push_back(sym);  
				//�ݹ����check,�Ƚϵ�ǰ����c����һ��ջ�����ŵ�������  
				check(c,coll2,coll3);     
			}  
			else   
			{  
				//���c��ջ���������ȼ����ǽ�cѹ��coll2(������ջ����  
				coll2.push(c);    
			}  
		}  
	}  
  
	//��coll��ȡ��Ԫ�أ�����Ԫ�ص�coll2��coll3��  
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
				//����check��������Բ�ͬ���������ͬ����  
				check(c,coll2,coll3);    
			}  
  
		}  
  
		//��������������coll2��Ԫ��ȫ�������������׺���ʽ��  
		while(!coll2.empty())   
		{    
			string c = coll2.top();  
			coll3.push_back(c);  
			coll2.pop();  
		}  
	}  
  
	//�����׺���ʽ  
	void calculate()   
	{    
		while(!coll3.empty())   
		{  
			string c = coll3.front();  
			coll3.pop_front();  
          
			//����ǲ�������ѹ��ջ��  
			if(!isOperator(c))   
			{  
				//��ȥ'0'�ó�ƫ��ֵ����Ϊ��ʵ��ֵ�����ֱ��ת����int��������ԣ���Ϊchar ת��Ϊint�������ֵ������'1'�ı���ֵΪ49  
				int op =atoi(c.c_str());      
				coll4.push(op);       
			}  
			else     //����ǲ���������ջ�е���Ԫ�ؽ��м���  
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
					coll4.push(op2/op1);  //ע����op2(op)op1������op1(op)op2  
					break;  
				}  
			}  
		}  
		isOK=true;
	}
};
//������ʽ�����ʽ��
class OutputAri{
	public:
	int N,R;
	string *question;//��Ŀ
	int Num[4];//��Ŀ�к��е����֣�������ɣ�
	int operCharNumber;//��Ŀ�а����������������������ɣ����Ϊ3������Ϊ1��
	char operatorChar[4];//��Ŀ�к��е��������������ɣ�
	int **addCover;//�ö�ά�����ж�һ���ӷ���������ظ�
	int **mulCover;//�ö�ά�����ж�һ���˷���������ظ�
	int one;//��һά��������ķ����ж�һλ��������ͳ����ظ�
	string *oneOperator;
	int two;//��һά��������ķ����ж϶�����������ظ�
	string *twoOperator;
	int three;//��һά��������ķ����ж�������������ظ�
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
		//�����������
		for(int j=0;j<=operCharNumber;j++){
			Num[j]=rand()%R;
		}
		//������������
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

	//����������������ϳ���ʽ����Ԫ��ѹ��C
	string addParentheses(int i,deque<string>& coll1)
	{
		twoCover=false;
		threeCover=false;
		string t="";
		bool first=false;
		bool second=false;
		bool third=false;
		//�������Ϊ3����ʱ��������㷨
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
		//����������������û�м�����ʱ��������ļ������㷨
		if(!third){
			//�жϵ�һ��������ܷ������
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
			//�жϵڶ���������ܷ������
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

	//�ж�����������������Ƿ��ظ�
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

	//�ж�һ�������������Ƿ��ظ�
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

	//�ж�һ��������Ƿ�Ϸ�
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

	//ͨ���ж�������ұߵ���������ж����������ܷ������
	bool rightOper(int pos)
	{
		if((operString[pos]=='+' || operString[pos]=='-') && (operString[pos+1]=='*' || operString[pos+1]=='/'))
			return true;
		return false;
	}

	//ͨ���ж��������ߵ���������ж����������ܷ������
	bool leftOper(int pos)
	{
		if(((operString[pos]=='+' || operString[pos]=='-') && operString[pos-1]=='*') || operString[pos-1]=='/')
			return true;
		return false;
	}

	//����תstring
	string intToString(int a){
		char turnChar[10];
		sprintf(turnChar,"%d",a);
		return turnChar;
	}

	//charתstring
	string charToString(char c)
	{
		string t(&c);
		return t;
	}

	//�жϼӷ��ظ�
	bool repeatAddCover(int left)
	{
		if(addCover[Num[left]][Num[left+1]]==1)
			return false;
		addCover[Num[left]][Num[left+1]]=1;
		addCover[Num[left+1]][Num[left]]=1;
		return true;
	}

	//�жϳ˷��ظ�
	bool repeatMulCover(int left)
	{
		if(mulCover[Num[left]][Num[left+1]]==1)
			return false;
		mulCover[Num[left]][Num[left+1]]=1;
		mulCover[Num[left+1]][Num[left]]=1;
		return true;
	}

	//�������Ϊ3��ʱ�ж��ܷ�Ϊ ������������������� ������������������� 
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
	//�����������������������
	bool ykykykk()
	{
		if(operString[0]=='-' || operString[0]=='/' || operString[0]=='*')
			if(operString[1]=='-' || operString[1]=='/' || operString[1]=='*')
				if(operString[2]=='-' || operString[2]=='+')
					return true;
		return false;
	}
	//�����������������������
	bool ykkykyk()
	{
		if(operString[0]=='/' || operString[0]=='*')
			if(operString[1]=='-' || operString[1]=='+')
				if(operString[2]=='*' || operString[2]=='/')
					return true;
		return false;
	}
	//�����������������������
	bool kykykky(){
		if(operString[0]=='-')
			if(operString[1]=='-' || operString[1]=='+')
				if(operString[2]=='*' || operString[2]=='/')
					return true;
		return false;
	}
};

//��ȡ�ı���ʽ����������
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
			//��һ����ÿ���ַ�������ʹ��deque��Ϊdeque������ɾ����ӵ��ٶ����
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

	//�ж��Ƿ����������������char��
	bool isOperatorForChar(char c)
	{
		if(c=='+' || c=='-' || c=='*' || c=='/' || c=='(' || c==')')
			return true;
		return false;
	}

	//charתstring
	string charToString(char c)
	{
		string t(&c);
		return t;
	}
};

//�ļ�������
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
	//��ȡִ���ļ�·��
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

//��ʼ������������
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
						cout<<"������-n��ֵ��";
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
						cout<<"������-r��ֵ��";
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
						cout<<"������-e��ֵ��";
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
						cout<<"������-a��ֵ��";
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
			isOK=false;//�ж���ʽ�Ƿ�Ϸ������Ϸ�������������ʽ
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
						//cout<<"��Ŀ��"<<i<<"   "<<question[i]<<coll4.top()<<endl;
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
					//cout<<"��Ŀ��"<<i<<"   "<<question[i]<<result<<endl;
				}
			}
		}
		operFile.oufQuestion.close();
		operFile.oufAnswer.close();
		cout<<"�ļ�����ɹ���";
	}
	else if(e!="" && a!=""){
		InputAri inputAri;
		operFile.openQuestion.open(operFile.programDir+e,ios::in);
		if(!operFile.openQuestion.good()){
			cout<<e+"�ļ���ʧ��";
			return 0;
		}
		operFile.openAnswer.open(operFile.programDir+a,ios::in);
		if(!operFile.openAnswer.good()){
			cout<<a+"�ļ���ʧ��";
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
				//��coll��ȡ��Ԫ�أ�����Ԫ�ص�coll2��coll3�� 
				cal.allocate();
				//�����׺���ʽ  
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
		cout<<"�ļ�����ɹ���";
	}
	else{
		cout<<"��������ȷ������";
	}
	return 0;
}