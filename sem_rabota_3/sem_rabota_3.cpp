#include <iostream>
#include <fstream>
using namespace std;
struct s_nazv_iden{
		char c;
		s_nazv_iden *next;
	};
//структура списка для хранения названий идентификаторов
struct sp_iden{
	s_nazv_iden *nazv_iden;
	bool init;
	int kod[8];
	sp_iden *next;
};
//структура списка для хранения значений и названий переменных
struct oper{
	int kod[8];
	oper *next;
	oper *prev;
	oper *up;
	oper *down;
	char tip_oper;
};
//структура для выполнений операции, содержащие скобки
bool iden(ifstream &prg, s_nazv_iden *&p){
	s_nazv_iden *p1 = p;	
	if((prg.peek()>='a' && prg.peek()<='z') || (prg.peek()>='A' 
		&& prg.peek()<='Z')){		
		prg.get(p1->c);		
		while((prg.peek()>='a' && prg.peek()<='z') || (prg.peek()>='A' && prg.peek()<='Z') ||
			(prg.peek()>='0' && prg.peek()<='9')){
			p1->next= new s_nazv_iden;
			p1=p1->next;
			prg.get(p1->c);
		}
		p1->next=0;
		return true;
	}
	p1->next=0;	
	return false;
}
/*если можно считать название идентификатора, он возвращает 
истину  и указатель на название идентификатор*/
bool find_id(sp_iden *sp, s_nazv_iden *n_id, sp_iden *&p1){
	s_nazv_iden *id1,*id2;
	p1=sp;
	bool find=false;
	while(!find && p1){
		id1 = p1->nazv_iden;
		id2 = n_id;
		while(id1 && id2 && (id1->c == id2->c)){
			id1=id1->next;
			id2=id2->next;
		}
		if( !id1 && !id2)
			find=true;
		if(!find)
			p1=p1->next;
	}
	return find;
}
//ищет идентификатор в списке переменных
bool kod(ifstream &prg, int a[8]){
	int i=0;
	char t;
	while(prg.peek()=='0' || prg.peek()=='1')
	{
		prg.get(t);
		a[i]=t-'0';
		i++;
	}
	if(i==8)
		return true;	
	return false;
}
//проверка последовательности символов на код
void operation(oper *op){			
	oper *op1=op, *op2=op->next;
	char znak1 = op1->tip_oper;
	if(op2->next)
		op1->tip_oper=op2->tip_oper;
	if(znak1=='-')	
		for(int i=0;i<8;i++)
			if(op1->kod[i] && !(op2->kod[i]))
				op1->kod[i]=0;
			else
				op1->kod[i]=1;
	if(znak1=='+')
		for(int i=0;i<8;i++)
			if((op1->kod[i])==(op2->kod[i]))
				op1->kod[i]=0;
			else
				op1->kod[i]=1;
	if(znak1=='|')
		for(int i=0;i<8;i++)
			if((op1->kod[i]) || (op2->kod[i]))
				op1->kod[i]=1;
			else
				op1->kod[i]=0;
	if(znak1=='&')
		for(int i=0;i<8;i++)
			if((op1->kod[i]) && (op2->kod[i]))
				op1->kod[i]=1;
			else
				op1->kod[i]=0;
	op1->next=op2->next;
	if(op2->next)
		op2->next->prev=op1;
	delete op2;
}	
//результат операции над 2-мя переменными записывает в первый
//и удаляет второй
void main(){
	ifstream prg("program.txt");
	int q=1,q2=1,gl;  //q и q2 - конечные состояния, 
	//gl - разность открывающих 
	//и закрывающих скобок
	s_nazv_iden *p1,*p; //вспомогательные указатели
	//на названия идентификаторов
	sp_iden *uk_id, *aux_sp; /*uk_id - указатель на  переменную, 
	aux_sp - вспомогательный указатель*/
	sp_iden *spisok1,*spisok; //spisok1 - указатель на болванчик 
	//списка переменных,spisok - для создания следующих элементов
	spisok1 = new sp_iden;
	spisok1->next=0;	
	spisok=spisok1;	
	char zn;  
	oper *op1,*opv,*op; //переменные для выполнений операций с кодом
	op1=new oper;
	op1->next =
	op1->prev =
	op1->down =
	op1->up = 0;
	op=op1;
	while( prg.peek()!=EOF && q!=0){
		switch(q)
		{
			case 1:
				p1=new s_nazv_iden;
				if(iden(prg, p1))
					{ 
						if(find_id(spisok1->next, p1, uk_id))
							while(p1){
								p=p1->next;
								delete p1;
								p1=p;
							}
						else{							
							spisok->next = new sp_iden;
							spisok = spisok->next;
							spisok->nazv_iden=p1;							
							spisok->init = false;
							spisok->next=0;
							uk_id = spisok;							
						}
						q=2;
					}
				else
				{
					if((prg.peek()==' ') || (prg.peek()=='\n'))
						prg.get(zn);
					else
					{ 
						cout<<"identificator is missed"<<'\n';
						q=0;
					}
				}
						
				break;				
			case 2:
				prg.get(zn);
				if(zn==':')
					q=3;
				else
				{
					if((prg.peek()==' ') || (prg.peek()=='\n'))
						prg.get(zn);
					else{
						q=0;
						cout<<"':' is missed"<<'\n';
					}
				}
				break;
			case 3:
				prg.get(zn);
				if(zn=='=')
					q=4;
				else
				{
					if((prg.peek()==' ') || (prg.peek()=='\n'))
						prg.get(zn);
					else{
						q=0;
						cout<<"'=' is missed"<<'\n';
					}
				}
				break;
			case 4:	
				q2=1;
				gl=0;
				while(q2) 
				{					
					switch (q2)
					{						
						case 1:							
							if(prg.peek()=='('){
								prg.get(zn);
								gl--;
								op->down = new oper;
								op->down->up=op;
								op=op->down;
								op->prev = 
								op->next = 0;
								op->down=0;																
							}
							else{
								p1=new s_nazv_iden;
								if(iden(prg, p1))
								{
									if(find_id(spisok1->next, p1, aux_sp))
										if(aux_sp->init)
										{											
											for(int i=0;i<8;i++)									
												op->kod[i]=	aux_sp->kod[i];
											q2=2;										
										}
										else
										{
											cout<<"variable ";
											p=p1;
											while(p)
											{ cout<<p->c; p = p->next; }
											cout<<" is not initialized"<<' ';
											q=0;
											q2=0;									
										}
									else
									{
										cout<<"variable ";
											p=p1;
											while(p)
											{ cout<<p->c; p=p->next; }
											cout<<" is not initialized"<<' ';
										q=0;
										q2=0;
									}
								}								
								else{
									int cod[8];
									if(kod(prg, cod))
									{										
										for(int i=0;i<8;i++)
											op->kod[i]=cod[i];
										q2=2;
									}
									else{
										if((prg.peek()==' ') || (prg.peek()=='\n'))
											prg.get(zn);
										else{											
											cout<<"incorrect cod"<<' ';
											q=0; q2=0;
										}
									}
								}
								p=p1;
								while(p1)
								{
									p=p1->next;
									delete p1;
									p1=p;
								}
							}							
							break;
						case 2:
							if(prg.peek()==')')
							{								
								prg.get(zn);								
								gl++;								
								if(gl<=0){										
									while(op->prev)
										op=op->prev;
									while(op->next)
										operation(op);
									for(int i=0;i<8;i++)
										op->up->kod[i]=op->kod[i];
									op=op->up;
									delete op->down;									
									op->down=0;
								}
								else
									q2=3;									
								
							}
							else{
								if( (prg.peek()=='+') || (prg.peek()=='-') ||
									(prg.peek()=='&') || (prg.peek()=='|') ){
									prg.get(zn);
									op->tip_oper=zn;
									op->next=new oper;
									op->next->prev = op;
									op=op->next;
									op->next =
									op->down =
									op->up = 0;
									q2=1;
								}
								else{
									if(prg.peek()==';'){
										prg.get(zn);
										q2=3;
									}
									else
									{
										if((prg.peek()==' ') ||(prg.peek()=='\n'))
											prg.get(zn);
										else{
											q2=0; q=0;
											cout<<"error"<<'\n';
										}
									}
								}											
							}														
							break;
						case 3:								
							if(gl==0)
							{
								op=op1;								
								while(op->next)
									operation(op);		
								uk_id->init=true;
								for(int i=0;i<8;i++)
									uk_id->kod[i]=op->kod[i];
								q2=0;
								q=1;
							}
							else 
							{
								if(gl<0)
									cout<<"syntax error '('";
								else
									cout<<"syntax error ')'";
								q2=0;
								q=0;
							}							
							break;
						default : 
							q=0;
							q2=0;
							break;
					}
				}
			}
		}
		prg.close();
		ofstream res("results.txt");
		spisok=spisok1->next;
		if(q)
			while(spisok){
				p = spisok->nazv_iden;
				while(p){	
					res<<p->c;
					p = p->next;
				}
				res<<'=';
				for(int i=0;i<8;i++)
					res<<spisok->kod[i];
				res<<' ';
				spisok=spisok->next;
			}
}