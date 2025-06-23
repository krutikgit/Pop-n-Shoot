#include<stdio.h>
#include<graphics.h>
#include<dos.h>
#include<stdlib.h>
#include<time.h>

struct node{int x,y;short int b_color,val;struct node* next;};
struct bnode{int x,y;short int b_color;struct node* next,*prev;};
typedef struct node Bullet;
typedef struct bnode Ballon;
Bullet* blt_head=NULL;
Ballon* bln_head=NULL;

void Item_display(void);
void create_shooter(int,int,int);
void getmousecord(int *,int *,int *);
void fire_bullet(void);
void initframe(void);
void loading(void);
void throw_ballon(void);
int bullet_display(Bullet *);
Ballon* ballon_display(Ballon *);
void bullet_design(Bullet *);
Ballon* remove_ballon(Ballon *,int);
Bullet* remove_bullet(Bullet *);
void chk_bullet_hit(void);
void ballon_wasted(void);
void reload(int);

union REGS i,o;
int button=0,tx,ty,bltcol=1,score_count=0,bln_wasted=0,bullet_count=6;
int xmin=0,xmax=0,prev_blt_col=0,prev_bal_col=10,prev_bal_pos=0,exp_count=0;
int loop_count=0;

void main(){
	int gd=DETECT,gm,flag=1;
	initgraph(&gd,&gm,"C:\\TURBOC3\\BGI");
	xmin=getmaxx()/2-200;
	xmax=getmaxx()/2+200;
	loading();
	initframe();
	while(!kbhit()){
		getmousecord(&button,&tx,&ty);
		if(!flag&&button==0)flag=1;
		if(bullet_count){
			if(button==1&&flag){
				fire_bullet();
				flag=0;
				bullet_count--;
				if(bullet_count==0){
					reload(1);
					}
				}
			}
		else{
			if(button==2){
				bullet_count=6;
				reload(0);
				}
			}
		if(loop_count%100==0){
			throw_ballon();
			}
		if(loop_count-exp_count>5){
			nosound();
			exp_count=32000;
			}
		create_shooter(blt_color(0),tx,getmaxy());
		delay(20);
		Item_display();
		create_shooter(getbkcolor(),tx,getmaxy());
		chk_bullet_hit();
		loop_count++;
		}
	closegraph();
	}

void Item_display(){
	Bullet* bltroot=blt_head;
	Ballon* blnroot=bln_head;
	while(bltroot!=NULL&&blnroot!=NULL){
		bullet_display(bltroot);
		blnroot=ballon_display(blnroot);
		//blnroot=blnroot->next;
		if(!bltroot->val){
			bltroot=remove_bullet(bltroot);
			}
		else{
			bltroot=bltroot->next;
			}
		//blnroot=blnroot->next;
		}
	while(bltroot!=NULL){
		bullet_display(bltroot);
		if(!bltroot->val){
			bltroot=remove_bullet(bltroot);
			}
		else{
			bltroot=bltroot->next;
			}
		}
	while(blnroot!=NULL){
		blnroot=ballon_display(blnroot);
		//blnroot=blnroot->next;
		}
	}

Bullet* create_bullet(short int col){
	Bullet* temp=(Bullet *)malloc(sizeof(Bullet));
	temp->next=NULL;
	if(tx<xmin+10)tx=xmin+10;
	else if(tx>xmax-10)tx=xmax-10;
	temp->x=tx;
	temp->y=455;
	temp->b_color=col;
	temp->val=1;
	return temp;
	}

Ballon* create_ballon(int col){
	Ballon* temp=(Ballon *)malloc(sizeof(Ballon));
	temp->x=xmin+12+random_num_ballon()*11;
	temp->y=10;
	temp->b_color=col;
	temp->next=NULL;
	//temp->prev=NULL;
	return temp;
	}

void getmousecord(int *button,int *tx,int *ty){
	i.x.ax=3;
	int86(0x33,&i,&o);
	*button=o.x.bx;
	*tx=o.x.cx;
	*ty=o.x.dx;
	}

void create_shooter(int col,int x,int y){
	setcolor(col);
	if(x>xmax-20){
		x=xmax-20;
		}
	if(x<xmin+20){
		x=xmin+20;
		}
	rectangle(x-19,y-10,x+19,y);
	}

void fire_bullet(){
	Bullet* temp,*root=blt_head;
	temp=create_bullet(blt_color(1));

	if(blt_head==NULL){
		blt_head=temp;
		}
	else{
		while(root->next!=NULL){
			root=root->next;
			}
		root->next=temp;
		}
	}

Bullet* remove_bullet(Bullet* root){
	Bullet* temp;
	temp=root->next;
	free(root);
	blt_head=temp;
	//printf("Blt");
	return temp;
	}

Ballon* remove_ballon(Ballon* root,int flag){
	Ballon* temp;
	//printf("Pop");
	if(flag){
		setcolor(getbkcolor());
		circle(root->x,root->y,10);
		setfillstyle(CLOSE_DOT_FILL,getbkcolor());
		//setfillstyle(SOLID_FILL,getbkcolor());
		floodfill(root->x,root->y,getbkcolor());
		}
	if(root==bln_head){
		temp=root->next;
		temp->prev=NULL;
		bln_head=temp;
		free(root);
		if(!flag){
			ballon_wasted();
			}
		return temp;
		}
	/*if(!flag){
		ballon_wasted();
		}*/
	temp=root->next;
	root->prev->next=temp;
	temp->prev=root->prev;
	free(root);
	return temp;
	}

void initframe(){
	setbkcolor(LIGHTGRAY);
	setcolor(DARKGRAY);
	rectangle(xmin,-10,xmax,getmaxy()+2);
	rectangle(xmin-1,-10,xmax+1,getmaxy()+2);
	settextstyle(SANS_SERIF_FONT,HORIZ_DIR,3);
	setcolor(MAGENTA);
	outtextxy(543,100,"SCORE");
	setcolor(RED);
	settextstyle(SANS_SERIF_FONT,HORIZ_DIR,2);
	outtextxy(540,220,"WASTED");
	//graphdefaults();
	}

void loading(){
	int i;
	settextstyle(TRIPLEX_SCR_FONT,HORIZ_DIR,2);
	outtextxy(getmaxx()/2-90,getmaxy()/2-40,"LOADING GAME");
	for(i=0;i<160;i++){
		line(getmaxx()/2-90+i,getmaxy()/2-10,getmaxx()/2-90+i,getmaxy()/2+5);
		delay(5);
		}
	delay(150);
	cleardevice();
	graphdefaults();
	}

int blt_color(int flag){
	int col=bltcol;
	if(flag){
		bltcol++;
		if(bltcol>3)bltcol=1;
		}
	return col;
	}

int random_bal_col(){
	int col;
	randomize();
	col=rand()%4;
	if(col==prev_bal_col||col==getbkcolor())col++;
	prev_bal_col=col;
	if(col>3||col==0)col=1;
	return col;
	}

void throw_ballon(){
	Ballon* temp,*root=bln_head;
	temp=create_ballon(random_bal_col());
	if(bln_head==NULL){
		bln_head=temp;
		}
	else{
		while(root->next!=NULL){
			root=root->next;
			}
		root->next=temp;
		temp->prev=root;
		}
	}

int random_num_ballon(){
	int col;
	randomize();
	col=rand()%35;
	if(col==prev_bal_pos)col--;
	if(col>35)col=35;
	prev_bal_pos=col;
	return col;
	}

int bullet_display(Bullet* root){
	setcolor(getbkcolor());
	bullet_design(root);
	root->y-=2;
	if(root->y>0){
		setcolor(root->b_color);
		bullet_design(root);
		root=root->next;
		}
	else{
		root->val=0;
		return 0;
		}
	return 1;
	}

Ballon* ballon_display(Ballon* root){
	setcolor(getbkcolor());
	circle(root->x,root->y,10);
	setfillstyle(CLOSE_DOT_FILL,getbkcolor());
	//setfillstyle(SOLID_FILL,getbkcolor());
	floodfill(root->x,root->y,getbkcolor());
	root->y+=1;
	if(root->y<getmaxy()-50){
		setcolor(root->b_color);
		circle(root->x,root->y,10);
		setfillstyle(CLOSE_DOT_FILL,root->b_color);
		//setfillstyle(SOLID_FILL,root->b_color);
		floodfill(root->x,root->y,root->b_color);
		root=root->next;
		}
	else{
		root=remove_ballon(root,0);
		}
	return root;
	}

void bullet_design(Bullet* root){
	rectangle(root->x-3,root->y-5,root->x+3,root->y+5);
	line(root->x-3,root->y-5,root->x,root->y-10);
	line(root->x,root->y-10,root->x+3,root->y-5);
	ellipse(root->x,root->y+3,0,180,3,2);
	ellipse(root->x,root->y-1,0,180,3,2);
	}

void chk_bullet_hit(){
	char ch[10];
	Bullet* bltroot=blt_head;
	Ballon* blnroot=bln_head;
	int flag=1;
	while(bltroot!=NULL){
		blnroot=bln_head;
		while(blnroot!=NULL){
			flag=1;
			if(bltroot->x>blnroot->x-13&&bltroot->x<blnroot->x+13)
			if(bltroot->y>blnroot->y-10&&bltroot->y<blnroot->y+10)
			if(bltroot->b_color==blnroot->b_color){
				setcolor(getbkcolor());
				sprintf(ch,"%d",score_count);
				outtextxy(550,140,ch);
				score_count++;
				sprintf(ch,"%d",score_count);
				setcolor(9);
				outtextxy(550,140,ch);
				blnroot=remove_ballon(blnroot,1);
				flag=0;
				exp_count=loop_count;
				sound(100);
				}
			if(flag)blnroot=blnroot->next;
			}
		bltroot=bltroot->next;
		}
	}

void ballon_wasted(){
	char ch[10];
	setcolor(getbkcolor());
	sprintf(ch,"%d",bln_wasted);
	outtextxy(540,250,ch);
	bln_wasted++;
	setcolor(9);
	sprintf(ch,"%d",bln_wasted);
	outtextxy(540,250,ch);
	}

void reload(int flag){
	if(flag){
		setcolor(BROWN);
		outtextxy(540,300,"RELOAD");
		}
	if(!flag){
		setcolor(getbkcolor());
		outtextxy(540,300,"RELOAD");
		}
	}
