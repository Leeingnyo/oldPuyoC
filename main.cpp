#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <conio.h>

using namespace std;

#include "NannaC.h"
#include "MCI.h"
#include "resource.h"
Player p("BGM");
Player eff_combo[8];

#define MH 13
#define MW 6 //���� ũ�� Height Width 
#define FX 1
#define FY 1 //���� ��ġ X Y 

#define SX 3
#define SY 1 //�ѿ� ���� ��ġ 

#define TG 10 //�ڵ����� �������µ� �ɸ��� �ð� 

#define STRSIZE MH*MW*2+100 //������ ���� ũ�� 

enum{yet, alc}; //count
enum{no, yes}; //ifdes
enum{empty, red, yellow, black, pink, purple, white, blue, lightbrown, obstacle}; //color
//�� ��, ����, ���, ���, ��������, ����, �Ͼ�, �Ķ�, ��������, ���ع� 
enum{Up, Left, Down, Right}; //direc
enum{single, multi, vscom, explain}; //playmode
enum{server, client}; //mypos

class Bbuyo{ //�� �������... 
      private:
      public:
          int color;
          friend class Bigbbuyo;
          friend class Map;
          Bbuyo(){
              color = empty;
          }
          Bbuyo(int c){
              color = c;
          }
          void print(){
               switch (color){
                      case empty:
                           SetColor(BLACK*16+7);
                           cout<<"  ";
                           break;
                      case red:
                           SetColor(BLACK*16+RED);
                           cout<<"��";
                           break;
                      case yellow:
                           SetColor(BLACK*16+YELLOW);
                           cout<<"��";
                           break;
                      case black:
                           SetColor(BLACK*16+DARK_GRAY);
                           cout<<"��";
                           break;
                      case pink:
                           SetColor(BLACK*16+VIOLET);
                           cout<<"��";
                           break;
                      case purple:
                           SetColor(BLACK*16+DARK_VIOLET);
                           cout<<"��";
                           break;
                      case white:
                           SetColor(BLACK*16+WHITE);
                           cout<<"��";
                           break;
                      case blue:
                           SetColor(BLACK*16+BLUE);
                           cout<<"��";
                           break;
                      case lightbrown:
                           SetColor(BLACK*16+DARK_YELLOW);
                           cout<<"��";
                           break;
                      case obstacle:
                           SetColor(BLACK*16+GRAY);
                           cout<<"��";
                           break;
                      default:
                           SetColor(BLACK*16+BLACK);
                           cout<<"  ";
                           break;
               }
          }
};

class Map{
      private:
      public:
          Bbuyo b;
          bool count;
          bool ifdes;
          Map(){
              b.color=empty;
          }
          void Clear(){
               b.color=empty;
          }
};

Map m_mine[MH][MW];
Map m_other[MH][MW];
Map m_temp[MH][MW];

class Bigbbuyo{
      private:
      public:
          Bbuyo bbuyo[2];
          int x, y;
          int direc;
          Map (*m)[MW];
          Bigbbuyo(){
               bbuyo[0].color = empty;
               bbuyo[1].color = empty;
               x=y=0;
               direc=Up;
               m=NULL;
          }
          Bigbbuyo(Map (*map)[MW]){
               bbuyo[0].color = empty;
               bbuyo[1].color = empty;
               x=y=0;
               direc=Up;
               m=map;
          }
          void SetColor(int c1, int c2){
               bbuyo[0].color = c1;
               bbuyo[1].color = c2;
          }
          void SetXY(int x1, int y1){
               x = x1; y = y1;
          }
          void SetDirec(int d){
               direc = d;
          }
          void turn();
          int move(int input);
          void drop(){
               while(!move(Down)); //�̵����� �� �� ������ �ݺ� 
          }
          void swap(){
               int tmp;
               tmp = bbuyo[0].color;
               bbuyo[0].color = bbuyo[1].color;
               bbuyo[1].color = tmp;
          }
          void tomap(){
              m[y][x].b.color = bbuyo[1].color;
              switch (direc){
                     case Up:
                          m[y-1][x].b.color = bbuyo[0].color;
                          break;
                     case Left:
                          m[y][x-1].b.color = bbuyo[0].color;
                          break;
                     case Down:
                          m[y+1][x].b.color = bbuyo[0].color;
                          break;
                     case Right:
                          m[y][x+1].b.color = bbuyo[0].color;
                          break;
                     default:
                          break;
              }
          }
          void print(){
              gotoxy((FX+x)*2,FY+y);
              bbuyo[1].print();
              switch (direc){
                     case Up:
                          gotoxy((FX+x)*2,FY+y-1);
                          break;
                     case Left:
                          gotoxy((FX+x-1)*2,FY+y);
                          break;
                     case Down:
                          gotoxy((FX+x)*2,FY+y+1);
                          break;
                     case Right:
                          gotoxy((FX+x+1)*2,FY+y);
                          break;
                     default:
                          gotoxy((FX+x)*2,FY+y-1);
                          break;
              }
              bbuyo[0].print();
          } 
};

int num;

class Connectbbuyo{
      private:
      public:
          int x, y;
          Connectbbuyo *next;
          Connectbbuyo(){
              x=y=0;
              next=NULL;
          }
          Connectbbuyo(int x1, int y1){
              x=x1;y=y1;
              next=NULL;
          }
          ~Connectbbuyo(){
              if (next != NULL)
                 delete next;
          }
};

          void Bigbbuyo::turn(){ //�ѿ䵹���� 
               switch (direc){
                      case Up:
                           if (y == 0) //�� ���̸�
                              ; //�ƹ��͵� �ƴ� 
                           else{ //�� ���� �ƴϸ� 
                              if (x == MW-1){ //�� �������̸� 
                                 if (m[y-1][x-1].b.color == empty){ //��������� 
                                    direc=Left; //��! 
                                    y--; //���� 
                                 }
                                 else //�� ��������� 
                                    swap(); //���� ���� �ٲ۴�. 
                              }
                              else{ //�� �������� �ƴϸ� 
                                 if (m[y-1][x+1].b.color == empty){ //��������� 
                                    direc=Left; //��! 
                                    x++; 
                                    y--; //������! 
                                 }
                                 else{ //�� ��������� 
                                    if (m[y-1][x-1].b.color == empty){ //�ٸ� ���� ��������� 
                                       direc=Left; //��! 
                                       y--; //���� 
                                    }
                                    else //�� ��������� 
                                       swap(); //���� ���� �ٲ۴�. 
                                 }
                              }
                           }
                           break;
                      case Left:
                           if ( y == MH-1 ){ //�� �Ʒ��̸� 
                              y--; //���� 
                              direc=Down; //��! 
                           }
                           else
                           if ( y == 0 ){ //�� ���̸� 
                              if (m[y+1][x].b.color == empty){ //��������� 
                                 direc=Down; //��! 
                              }
                              else //�� ������� 
                                 swap(); //���� ���� �ٲ۴�. 
                           }
                           else{ //�߰��̸� 
                              if (m[y+1][x].b.color == empty){ //��������� 
                                 direc=Down; //��!
                              }
                              else{
                                 y--; //���� 
                                 direc=Down; //��!
                              }
                           }
                           break;
                      case Down:
                           if ( x == 0 ){ //�� �����̸� 
                              if (m[y+1][x+1].b.color == empty){ //��������� 
                                 y++; //�Ʒ��� 
                                 direc=Right; //��! 
                              }
                              else //�� ��������� 
                                 swap(); //���� ���� �ٲ۴�. 
                           }
                           else
                           if ( x == MW-1 ){ //���� �������̸� 
                              if (m[y+1][x-1].b.color == empty){ //��������� 
                                 x--;
                                 y++; //�� �Ʒ��� 
                                 direc=Right; //��! 
                              }
                              else //�Ⱥ�������� 
                                 swap(); //���� ���� �ٲ۴�. 
                           }
                           else{ //�ƴϸ� 
                              if (m[y+1][x-1].b.color == empty){ //��������� 
                                 x--;
                                 y++; //�� �Ʒ��� 
                                 direc=Right; //��! 
                              }
                              else{ //�� ��������� 
                                 if (m[y+1][x+1].b.color == empty){ //�ٸ� ���� ��������� 
                                    y++; //�Ʒ��� 
                                    direc=Right; //�� 
                                 }
                                 else //�� ���������
                                    swap(); //���� ���� �ٲ۴�. 
                              }
                           }
                           break;
                      case Right:
                           if ( y == 0 ){ //�� ���̸� 
                              if (m[y+1][x].b.color == empty){ //��������� 
                                 y++; //�Ʒ��� 
                                 direc=Up; //��! 
                              }
                              else //�� ������� 
                                 swap(); //���� ���� �ٲ۴�. 
                           }
                           else{ //�߰��̸� 
                              direc=Up; //��! 
                           }
                           break;
                      default:
                           direc=Up;
                           break;
               }
          }
          int Bigbbuyo::move(int input){
              switch (input){
                     case Up:
                          y--; 
                          break;
                     case Left:
                          switch (direc){
                                 case Up:
                                      if ( x == 0 ){ //�� �����̸� 
                                         return 1; //�ƹ��͵� �� �Ѵ�. 
                                      }
                                      else //�� ������ �ƴϸ� 
                                      if (m[y][x-1].b.color == empty){ //������ ������� 
                                         x--; //�������� 
                                      }
                                      else //�� ������� 
                                         return 1; //�ƹ��͵� �� �Ѵ�. 
                                      break;
                                 case Left:
                                      if ( x <= 1 ){ //�� �����̸� 
                                         return 1; //�ƹ��͵� �� �Ѵ�. 
                                      }
                                      else //�� ������ �ƴϸ� 
                                      if (m[y][x-2].b.color == empty){ //������ ������� 
                                         x--; //�������� 
                                      }
                                      else //�� ������� 
                                         return 1; //�ƹ��͵� �� �Ѵ�. 
                                      break;
                                 case Down:
                                      if ( x == 0 ){ //�� �����̸� 
                                         return 1; //�ƹ��͵� �� �Ѵ�. 
                                      }
                                      else //�� ������ �ƴϸ� 
                                      if (m[y+1][x-1].b.color == empty){ //������ ������� 
                                         x--; //�������� 
                                      }
                                      else //�� ������� 
                                         return 1; //�ƹ��͵� �� �Ѵ�. 
                                      break;
                                 case Right:
                                      if ( x == 0 ){ //�� �����̸� 
                                         return 1; //�ƹ��͵� �� �Ѵ�. 
                                      }
                                      else //�� ������ �ƴϸ� 
                                      if (m[y][x-1].b.color == empty){ //������ ������� 
                                         x--; //�������� 
                                      }
                                      else //�� ������� 
                                         return 1; //�ƹ��͵� �� �Ѵ�. 
                                      break;
                                 default:
                                      
                                      break;
                          }
                          break;
                     case Down:
                          switch (direc){
                                 case Up:
                                      if ( y == MH-1 ){ //�� �Ʒ����̸� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      }
                                      else //�� �Ʒ����� �ƴϸ� 
                                      if (m[y+1][x].b.color == empty){ //�Ʒ����� ������� 
                                         y++; //�Ʒ������� 
                                      }
                                      else //�� ������� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      break;
                                 case Left:
                                      if ( y == MH-1 ){ //�� �Ʒ����̸� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      }
                                      else //�� �Ʒ����� �ƴϸ� 
                                      if (m[y+1][x].b.color == empty && m[y+1][x-1].b.color == empty){ //�Ʒ����� �Ѵ� ������� 
                                         y++; //�Ʒ������� 
                                      }
                                      else //�� ������� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      break;
                                 case Down:
                                      if ( y >= MH-2 ){ //�� �Ʒ����̸� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      }
                                      else //�� �Ʒ����� �ƴϸ� 
                                      if (m[y+2][x].b.color == empty){ //�Ʒ����� ������� 
                                         y++; //�Ʒ������� 
                                      }
                                      else //�� ������� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      break;
                                 case Right:
                                      if ( y == MH-1 ){ //�� �Ʒ����̸� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      }
                                      else //�� �Ʒ����� �ƴϸ� 
                                      if (m[y+1][x].b.color == empty && m[y+1][x+1].b.color == empty){ //�Ʒ����� �Ѵ� ������� 
                                         y++; //�Ʒ������� 
                                      }
                                      else //�� ������� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      break;
                                 default:
                                      
                                      break;
                          }
                          break;
                     case Right:
                          switch (direc){
                                 case Up:
                                     if ( x == MW-1 ){ //�� �������̸� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      }
                                      else //�� �������� �ƴϸ� 
                                      if (m[y][x+1].b.color == empty){ //�������� ������� 
                                         x++; //���������� 
                                      }
                                      else //�� ������� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      break;
                                 case Left:
                                      if ( x == MW-1 ){ //�� �������̸� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      }
                                      else //�� �������� �ƴϸ� 
                                      if (m[y][x+1].b.color == empty){ //�������� ������� 
                                         x++; //���������� 
                                      }
                                      else //�� ������� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      break;
                                 case Down:
                                      if ( x == MW-1 ){ //�� �������̸� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      }
                                      else //�� �������� �ƴϸ� 
                                      if (m[y+1][x+1].b.color == empty){ //�������� ������� 
                                         x++; //���������� 
                                      }
                                      else //�� ������� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      break;
                                 case Right:
                                      if ( x >= MW-2 ){ //�� �������̸� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      }
                                      else //�� �������� �ƴϸ� 
                                      if (m[y][x+2].b.color == empty){ //�������� ������� 
                                         x++; //���������� 
                                      }
                                      else //�� ������� 
                                         return 1; //�ƹ��͵� ���Ѵ�. 
                                      break;
                                 default:
                                      return 1;
                                      break;
                          }
                          break;
                     default:
                          return 1;
                          break;
              }
              return 0; //�̵� ����! 
         }

void print();
void calculate();
void multicalculate(int mypos, SOCKET *sock, SOCKET *csock, SOCKET *lsock);
void multicalculate2(int mypos, SOCKET *sock, SOCKET *csock, SOCKET *lsock);
void AIcalculate1();
void obstacledrop();

bool check(Map (*m)[MW]);
int floodfill(Map (*m)[MW], int x, int y, Connectbbuyo *t);
int cleaning(Map (*m)[MW]);
void gravity(Map (*m)[MW]);
    
    //first
    int playmode; //2p���� 
    int difficulty; //���̵� 
    //common
    int random; //�������� 
    bool gameover=true; //true�� ���� false�� ���ӿ��� 
    bool delay=0; //����ϱ� 
    int time_delay=0; //���ð� 
    bool ifcombo=0; //�޺����� 
    int combonum=0; //�޺��� 
    int max_combo=0; //�ƽ��޺� 
    int desnum; //�ѹ��� �μ��� ���� 
    double scorenum; //������ ���� �� 
    int score=0; //���� 
    int time_gravity=0; //�߷½ð� 
    bool iffloor=0; //�ٴڿ� ��Ҵ��� 
    bool ifpressx=0; //x�� �ѹ� �������� 
    bool turndel=0; //������������ 
    int time_turndel=0; //�� ������ ���� �ð���� 
    Bigbbuyo b(m_mine);
    int nextc1, nextc2; //�������� 
    Bigbbuyo next; //���� �ѿ� 
    //single 
    bool BGMon=true; //true�� ���, �ƴϸ� ���� 
    bool pause=false; //true�� ����, �ƴϸ� ���� 
    //mulit 
    int obstaclenum; //���� ���ػѿ䰳�� 
    int obstaclenum_o; //����� �����ִ� ���ػѿ� ���� 
    int obstaclenum_r=0; //������ �������ִ� ���ػѿ� ���� 
    int obstaclenum_s; //��뿡�� ���� ���ػѿ䰳�� 
    int obstacle_x=0; //���� ���ػѿ� x��ǥ 
    bool ifobs=false; //���ػѿ� ����߷��� �ϴ��� 
    bool ifobsover=false; //���ػѿ� ����߷ȴ��� 
    bool ifwin=true; //true�� �̱�� false�� ���� 
    int ifwin_o=true; //true�� �̱�� false�� ���� 
    bool ifeffect=0; //�� ����Ʈ true�� ��� 
    int ifeffect_o=0; //����� ����Ʈ ��� 
    int effectmove; //�׸� 
    int effectmove_o; //��� �׸� //1~6 : 11~168
    bool ifeffectp; //�׸����� 
    int ifeffectp_o; //�׸����� 
    int combonum_o=0; //��� �޺��� 
    int max_combo_o=0; //��� �ƽ��޺� 
    int score_o=0; //��� ���� 
    Bigbbuyo b_o(m_other); //����� ���� �ѿ� 
    Bigbbuyo next_o; //����� ���� �ѿ� 
    //AI 
    int obstaclenum_s_o; //��ǻ�Ͱ� ���� ���ػѿ䰳�� 
    int obstacle_x_o=0; //��ǻ�Ͱ� ���� ���ػѿ� x��ǥ 
    bool ifobs_o=false; //���ػѿ� ����߷��� �ϴ��� 
    bool ifobsover_o=false; //���ػѿ� ����߷ȴ��� 
    int desnum_o; 
    bool delay_o;
    bool ifcombo_o;
    double scorenum_o; 
    int time_delay_o;
    bool iffloor_o;
    int nextc1_o, nextc2_o;
    int time_gravity_o;
    Bigbbuyo b_t(m_temp); //��ǻ�� ������ ���� �ѿ� 
    
void Restart(){
    for (int i = 0 ; i < MH ; i++){
        for (int j = 0 ; j < MW ; j++){
            m_mine[i][j].Clear();
            m_other[i][j].Clear();
        }
    }
    //common
    gameover=true; //true�� ���� false�� ���ӿ��� 
    delay=0;
    time_delay=0;
    ifcombo=0;
    combonum=0;
    max_combo=0;
    score=0;
    time_gravity=0;
    iffloor=0;
    ifpressx=0;
    turndel=0;
    time_turndel=0;
    //single
    BGMon=true;
    pause=false;
    //mulit
    obstaclenum=0;
    obstaclenum_r=0;
    ifobs=false;
    ifwin=true;
    ifwin_o=true;
    ifeffect=0;
    ifobs_o=false; //���ػѿ� ����߷��� �ϴ��� 
    ifobsover_o=false; //���ػѿ� ����߷ȴ��� 
}
    
int main(){
    system("title �ѿ�ѿ� by ���ο�. ����� Ctrl + C");
RE:
    system("cls");
    cout << "���� ����� �������ּ���.\n1. ȥ���ϱ�\n2. �����ϱ�\n3. �İ��ϱ�\n4. �ϴ¹��\n���� : ";
    cin >> playmode;
    if (playmode != 1 && playmode != 2 && playmode != 3 && playmode != 4) //�ٸ� �� ��������� 
       playmode = 1; //������ 1 
    playmode--;
    system("cls");
    if (playmode == single){
       cout << "���̵��� �������ּ���.\n���̵� 1 - 4��, ���Ѹ�\n���̵� 2 - 8��, �ſ��\n���̵� 3 - ������\n���� : ";
       cin >> difficulty;
       if (difficulty != 1 && difficulty != 2 && difficulty != 3) //�ٸ� �� ��������� 
          difficulty = 1; //������ 1 
       
       system("cls");
       if (difficulty == 3)
          goto SINGLEEND;
       cout << "Loading";
       p.Open("ȿ����\\bimbim.bbu");
       
       p.Play();
       
       system("cls");
       CursorView(0);
       
       random = rand();
       
       {
            m_mine[MH-1][0].b.color = obstacle;
            m_mine[MH-1][1].b.color = obstacle;
            m_mine[MH-1][2].b.color = obstacle;
            m_mine[MH-1][3].b.color = obstacle;
            m_mine[MH-1][4].b.color = obstacle;
            m_mine[MH-1][5].b.color = obstacle;
       }
       //���� 
       
       srand(random); //�ѿ䳪�ð� ���� 
       
       b.SetColor(rand()%(4*difficulty)+1,rand()%(4*difficulty)+1);
       b.SetXY(SX,SY); //���ο� �ѿ���� 
       next.SetXY((MW+3),FY+1);
       nextc1 = rand()%(4*difficulty)+1;
       nextc2 = rand()%(4*difficulty)+1;
       next.SetColor(nextc1,nextc2); //���� �ѿ� ���� 
       while (gameover){
             if (pause == false){
                calculate();
                //����ϱ� 
                
                if ( GetAsyncKeyState(VK_UP) & 0x8000 ){
                   // b.move(Up);
                }
                if ( GetAsyncKeyState(VK_LEFT) & 0x8000 ){
                   b.move(Left);
                }
                if ( GetAsyncKeyState(VK_DOWN) & 0x8000 ){
                   b.move(Down);
                }
                if ( GetAsyncKeyState(VK_RIGHT) & 0x8000 ){
                   b.move(Right);
                }
                if ( GetAsyncKeyState('Z') & 0x8000 && !turndel){
                   b.turn();
                   PlayEffectR(ID_TURN);
                   turndel=true;
                }
                if ( GetAsyncKeyState('X') & 0x8000 && ifpressx == false){
                   time_gravity=TG*(3-difficulty)-2;
                   ifpressx=true;
                   b.drop();
                }
                if ( GetAsyncKeyState(VK_ESCAPE) & 0x8000 ){
                    if (BGMon){
                       BGMon=false;
                       p.Pause();
                    }
                    else{
                       BGMon=true;
                       p.SetPosition(0);
                       p.Play();
                    }
                }
             }
             if ( GetAsyncKeyState(VK_SPACE) & 0x8000 ){
                if (pause) pause=false;
                else pause=true;
                Sleep(500);
             }
             //�Է¹ޱ� 
             
             print();
             //����ϱ� 
             
             Sleep(50);
             //����ϱ� 
       }
       system("cls");
       
       SetColor(WHITE);
       cout << "Gameover" << "\n";
       cout << "score : " << score << "\n";
       cout << "maxcombo : " << max_combo << endl;
            system("pause");
       p.Stop();
       system("cls");
       SetColor(GRAY);
       
       SINGLEEND:
       Restart();
    }
    else if (playmode == multi){
       difficulty = 1;
       int mypos;
       int move;
       
       int retval;
       WSADATA wsa;
       SOCKET sock, lsock, csock;
       SOCKADDR_IN serveraddr, clientaddr;
       int addrlen;
       char sendb[STRSIZE];
       char recvb[STRSIZE];
       char strtemp[STRSIZE]; //���ƾ� 
       
       { //�����ʱ�ȭ 
           for (int i=0;i<STRSIZE;i++) sendb[i]=' ';sendb[STRSIZE-1]='\0';
           for (int i=0;i<STRSIZE;i++) recvb[i]=' ';sendb[STRSIZE-1]='\0';
           for (int i=0;i<STRSIZE;i++) strtemp[i]=' ';sendb[STRSIZE-1]='\0';
       }
       
       if (WSAStartup(MAKEWORD(2,2), &wsa) != 0){ //���ӽ�ŸƮ�� 
          MessageBox(NULL,TEXT("���� �ʱ�ȭ ����\n�޸𸮰� ������ �����̴� ������� ���մϴ�.\n�˼��մϴ�." ),TEXT("�ȳ�"),MB_OK);
          return -1;
       }
       
       SetColor(WHITE);
       cout << "�ȳ��ϼ���^^ ��Ƽ�ѿ�ѿ信 ���� ���� ȯ���մϴ�.\n";
       cout << "���� ��Ƽ���� ��ü�� ó�� ���� �̻��ϰ� ����� ���� �ִ�ϴ�!\n";
       cout << "�����Ͱ� 256����Ʈ��ŭ 0.02�ʸ��� ��ȯ�Ǵ�\n";
       cout << "�翡 ����Ͽ� ����� �ΰ��Ǵ� ���(�ֽ��� ��)���δ� ���� ������!\n";
       SetColor(YELLOW);
       cout << "\n���� ����� ������ �����ּ���.\n";
       cout << "������ ���� ����� �����̰�,\n";
       cout << "Ŭ���̾�Ʈ�� ���� �濡 ���� �����Դϴ�.\n";
       SetColor(GREEN);
       cout << "\n�����̽� ���� �ڽ��� �������ּҸ� Ŭ���̾�Ʈ���� �Ѱ��ּ���(IP�� ������ �սô�)\n";
       cout << "������ �ּҴ� ���� - ��� ���α׷� - ���� ���α׷� - ���������Ʈ\n";
       cout << "ipconfig�� ġ�ð� �� ������ ��Ͽ��� ���� �� ����ͳ�\n�̴��� ������� IPv4�� �˷��ֽø� �˴ϴ�.\n";
       SetColor(RED);
       cout << "\n������ �����⸦ ����ϰ� �ִٸ� ��Ʈ�������� �ؾ��մϴ�.\n";
       cout << "������ ���� ����Ʈ�� ���ż� ��Ʈ�������� ��Ʈ�� 9595�� ���ֽø� �˴ϴ�.\n";
       cout << "��Ʈ�������� ������ ���ͳ��� �����Ͻñ� �ٶ��ϴ�.\n";
       cout << "(�����⸶�� �ٸ��� �׸����� �����ϱ� ���� �׷�)\n";
       SetColor(GRAY);
       cout << "\n�̻��� ���� �Է��Ͻ� �� ������ �ڵ���ȯ�˴ϴ�.\n";
       cout << "1. ����\n2. Ŭ���̾�Ʈ\n3. ������\n���� : ";
       cin >> mypos;
       if (mypos != 1 && mypos != 2 && mypos != 3) //�ٸ� �� ��������� 
          mypos = 1; //������ 1 
       mypos--;
       system("cls");
       if (mypos == server){ //������ 
          lsock = socket(AF_INET, SOCK_STREAM, 0); //�� ������Ѷ�! 
          
          ZeroMemory(&serveraddr, sizeof(serveraddr));
          serveraddr.sin_family = AF_INET;
          serveraddr.sin_port = htons(9595);
          serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
          retval = bind(lsock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
          if (retval == SOCKET_ERROR){
             MessageBox(NULL,TEXT("���ε� ����\n������ �ΰ��� Ų �� �����ϴ�.\nȮ���� ���ð� �� �Ǹ� ������ϼ���." ),TEXT("����"),MB_OK);
             return -1;
          }
          //���ε� 
          
          retval = listen(lsock, SOMAXCONN);
          if (retval == SOCKET_ERROR){
             MessageBox(NULL,TEXT("���� ����" ),TEXT("����"),MB_OK);
             return -1;
          }
          //���� 
          
          printf("������ ��ٸ��� ���Դϴ�...");
          Sleep(1000);
          addrlen = sizeof(clientaddr);
          csock = accept(lsock, (SOCKADDR *)&clientaddr, &addrlen);
          printf("\n�����߽��ϴ�!\nIP %s, ��Ʈ %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
          //�׻�Ʈ 
          Sleep(2000);
       }
       else if (mypos == client){
          sock = socket(AF_INET, SOCK_STREAM, 0);
          
          SOCKADDR_IN serveraddr;
          ZeroMemory(&serveraddr, sizeof(serveraddr));
          serveraddr.sin_family = AF_INET;
          serveraddr.sin_port = htons(9595);
       RECONNECT:
          printf("������ IP�� �Է��ϼ��� ��) xxx.xxx.xxx.xxx + ����\n�Է� : ");
          scanf("%s",strtemp);
          serveraddr.sin_addr.s_addr = inet_addr(strtemp);
          
          retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
          if (retval == SOCKET_ERROR){
             MessageBox(NULL,TEXT("Ŀ��Ʈ ����\n�ùٸ��� ���� IP�̰ų� ������ ������ �ʾҽ��ϴ�.\n�ٽ� �Է����ּ���" ),TEXT("Ŭ��"),MB_OK);
             system("cls");
             goto RECONNECT;
          }
          printf("\n�����߽��ϴ�!");
          //Ŀ��Ʈ 
          Sleep(2000);
       }
       else{
          difficulty = 3;
          goto MULTIEND;
       }
       
       system("cls");
       cout << "Loading";
       eff_combo[0].SetName("C01");
       eff_combo[1].SetName("C02");
       eff_combo[2].SetName("C03");
       eff_combo[3].SetName("C04");
       eff_combo[4].SetName("C05");
       eff_combo[5].SetName("C06");
       eff_combo[6].SetName("C07");
       eff_combo[7].SetName("C08");
       
       p.Open("ȿ����\\bimbim.bbu");
       eff_combo[0].Open("ȿ����\\C01.bbu");
       eff_combo[1].Open("ȿ����\\C02.bbu");
       eff_combo[2].Open("ȿ����\\C03.bbu");
       eff_combo[3].Open("ȿ����\\C04.bbu");
       eff_combo[4].Open("ȿ����\\C05.bbu");
       eff_combo[5].Open("ȿ����\\C06.bbu");
       eff_combo[6].Open("ȿ����\\C07.bbu");
       eff_combo[7].Open("ȿ����\\C08.bbu");
       
       p.Play();
       
       if (mypos == server){
          retval = recv(csock, recvb, 3, 0);
          if (retval == SOCKET_ERROR){
              MessageBox(NULL,TEXT("�ޱ⸦ �����߽��ϴ�. �����մϴ�." ),TEXT("����"),MB_OK);
              return -1;
          }
          sscanf(recvb, "%s", strtemp);
          if (strcmp(strtemp, "OK")!=0) return -1;
          sprintf(sendb, "%s", "OK");
          retval = send(csock, sendb, 3, 0);
          if (retval == SOCKET_ERROR){
              MessageBox(NULL,TEXT("�����⸦ �����߽��ϴ�. �����մϴ�." ),TEXT("����"),MB_OK);
              return -1;
          }
       }
       else if (mypos == client){
          sprintf(sendb, "%s", "OK");
          retval = send(sock, sendb, 3, 0);
          if (retval == SOCKET_ERROR){
              MessageBox(NULL,TEXT("�����⸦ �����߽��ϴ�. �����մϴ�." ),TEXT("Ŭ��"),MB_OK);
              return -1;
          }
          retval = recv(sock, recvb, 3, 0);
          if (retval == SOCKET_ERROR){
              MessageBox(NULL,TEXT("�ޱ⸦ �����߽��ϴ�. �����մϴ�." ),TEXT("Ŭ��"),MB_OK);
              return -1;
          }
          sscanf(recvb, "%s", strtemp);
          if (strcmp(strtemp, "OK")!=0) return -1;
       }
       //��ȣ��ȯ 
       system("cls");
       CursorView(0);
       
       { //�����ʱ�ȭ 
           for (int i=0;i<STRSIZE;i++) sendb[i]=' ';sendb[STRSIZE-1]='\0';
           for (int i=0;i<STRSIZE;i++) recvb[i]=' ';sendb[STRSIZE-1]='\0';
           for (int i=0;i<STRSIZE;i++) strtemp[i]=' ';sendb[STRSIZE-1]='\0';
       }
       if (mypos == server){
          random = rand();
          sprintf(sendb, "%10d", random);
          retval = send(csock, sendb, 11, 0);
          if (retval == SOCKET_ERROR){
              MessageBox(NULL,TEXT("�����⸦ �����߽��ϴ�. �����մϴ�." ),TEXT("����"),MB_OK);
              return -1;
          }
       }
       else if (mypos == client){
          retval = recv(sock, recvb, 11, 0);
          if (retval == SOCKET_ERROR){
              MessageBox(NULL,TEXT("�ޱ⸦ �����߽��ϴ�. �����մϴ�." ),TEXT("Ŭ��"),MB_OK);
              return -1;
          }
          sscanf(recvb, "%d", &random);
       }
       //�����Ѱ� ��ȯ 
       
       srand(random);
        
       b.SetColor(rand()%(4*difficulty)+1,rand()%(4*difficulty)+1);
       b.SetXY(SX,SY); //���ο� �ѿ���� 
       next.SetXY((MW+3),FY+1);
       nextc1 = rand()%(4*difficulty)+1;
       nextc2 = rand()%(4*difficulty)+1;
       next.SetColor(nextc1,nextc2); //���� �ѿ� ���� 
       next_o.SetXY((MW+9), FY+1); //����� ���� �ѿ� ��ġ 
       
       while (gameover){
             calculate();
             multicalculate(mypos, &sock, &csock, &lsock);
             multicalculate2(mypos, &sock, &csock, &lsock);
             if (!ifwin_o){ //�̱�� 
                ifwin = 1; //�̱�! 
                gameover=false;
             }
             if (ifobs && combonum_o == 0){ //����Ʈ�����ϸ�, ����� �޺��� �� �������� 
                obstacledrop(); //���ع���� 
             }
             //����ϱ� 
             /*
             if ( GetAsyncKeyState(VK_UP) & 0x8000 ){
                // b.move(Up);
             }
             if ( GetAsyncKeyState(VK_LEFT) & 0x8000 ){
                b.move(Left);
             }
             if ( GetAsyncKeyState(VK_DOWN) & 0x8000 ){
                b.move(Down);
             }
             if ( GetAsyncKeyState(VK_RIGHT) & 0x8000 ){
                b.move(Right);
             }
             if ( GetAsyncKeyState('Z') & 0x8000 && !turndel){
                b.turn();
                PlayEffectR(ID_TURN);
                turndel=true;
             }
             if ( GetAsyncKeyState('X') & 0x8000 && ifpressx == false){
                time_gravity=TG*(3-difficulty)-2;
                ifpressx=true;
                b.drop();
             }
             if ( GetAsyncKeyState(VK_ESCAPE) & 0x8000 ){
                 if (BGMon){
                    BGMon=false;
                    p.Pause();
                 }
                 else{
                    BGMon=true;
                    p.SetPosition(0);
                    p.Play();
                 }
             }
             */
             if (kbhit()){
                REINPUT:
                move = getch();
                switch (move){
                       case 72: //Up
                            // b.move(Up);
                            break;
                       case 75: //Left
                            b.move(Left);
                            break;
                       case 80: //Down
                            b.move(Down);
                            break;
                       case 77: //Right
                            b.move(Right);
                            break;
                       case 'Z':case 'z': //Z
                            b.turn();
                            PlayEffectR(ID_TURN);
                            turndel=true;
                            break;
                       case 'X':case 'x': //X
                            time_gravity=TG*(3-difficulty)-2;
                            ifpressx=true;
                            b.drop();
                            break;
                       case 27: //ESC
                            if (BGMon){
                               BGMon=false;
                               p.Pause();
                            }
                            else{
                               BGMon=true;
                               p.SetPosition(0);
                               p.Play();
                            }
                            break;
                       case 224:
                            goto REINPUT;
                }
             }
             //�Է¹ޱ� 
             
             print();
             //����ϱ� 
             
             Sleep(20);
             //����ϱ� 
       }
       system("cls");
       
       if (mypos == server){
          closesocket(csock);
          closesocket(lsock);
       }
       else if (mypos == client){
          closesocket(sock);
       }
       //������� 
       
       WSACleanup();
       
       SetColor(WHITE);
       if (ifwin)
          cout << "YOU WIN!" << "\n";
       else
          cout << "YOU LOSE" << "\n";
       cout << "score : " << score << "\n";
       cout << "maxcombo : " << max_combo << endl;
            system("pause");
       p.Stop();
       system("cls");
       SetColor(GRAY);
       
       MULTIEND:
       Restart();
    }
    else
    if (playmode == vscom){
       SetColor(WHITE);
       cout << "�ȳ��ϼ���^^ ��ǻ�Ϳ��� ��ῡ ���� ���� ȯ���մϴ�.\n";
       cout << "���� �ΰ����� ��ü�� ó�� ���� �̻��ϰ� ����� ���� �ִ�ϴ�!\n";
       SetColor(YELLOW);
       cout << "\n���� ��ǻ���� ����ť�� �������ּ���\n";
       cout << "\n";
       cout << "\n";
       SetColor(GREEN);
       cout << "\n�׷��� ���� ������ ���� �ʾҴ�ϴ�^^;\n";
       cout << "\n";
       cout << "\n";
       SetColor(RED);
       cout << "\n�������� ����� ������ �������� ���ð��.\n";
       cout << "\n";
       cout << "\n";
       cout << "\n";
       SetColor(GRAY);
       cout << "\n�ٽ� �ʱ�ȭ������ ���ư��ϴ�.\n";
       cout << "";
       SetColor(GRAY);
       cout << "\n";
       system("pause");
       goto RE;
    }
    else
    if (playmode == explain){
       SetColor(WHITE);
       cout << " �ȳ��ϼ���^^\n";
       cout << "���ο��� �ѿ�ѿ��Դϴ�. �������ּż� �����մϴ�.\n";
       cout << "���� 1.2.2S\n"; //Version 
       cout << "��¥ 2014.2.24.��\n";
       SetColor(YELLOW);
       cout << "\n �ϴ� ��\n";
       cout << "����Ű ���� - �̵�\n";
       cout << "Z - ȸ��\n";
       cout << "X - �����ȱ�\n";
       cout << "ESC - ���������/�ѱ�\n";
       cout << "SPACE - ���߱�(ȥ���ϱ⿡����^^)\n";
       SetColor(GREEN);
       cout << "\n ȥ���ϱ�\n";
       cout << "���̵� 1�� 2�� �������� �ֽ��ϴ�.\n";
       cout << "���̵� 1������ �޺������� ���̵� 2������ ������ ���� �򵵷� �غ��ô�.\n";
       cout << "(�� �ִ��޺�(���̵� 1)�� 5�̰�, �ִ�����(���̵� 2)�� 3000���Դϴ�^^)\n";
       SetColor(RED);
       cout << "\n �����ϱ�\n";
       cout << "���ͳ��� ����Ǿ� �ִ� �ٸ� ��ǻ�Ϳ� ������ �� �ֽ��ϴ�.\n";
       cout << "(�ڼ��� ������ �ش� �׸����� �Ѿ�ּ���.)\n";
       SetColor(SKY_BLUE);
       cout << "\n �İ��ϱ� // ���� ��\n";
       cout << "��ǻ�Ϳ� ������ �� �ֽ��ϴ�.\n";
       cout << "(�ڼ��� ������ �ش� �׸����� �Ѿ�ּ���.)\n";
       SetColor(GRAY);
       cout << "\n";
       system("pause");
       goto RE; 
    }
    if (difficulty == 3){
       goto RE;
    }
    
    cout << "�ٽ� �Ͻðڽ��ϱ�?\n1. ��\n2. �ƴϿ�\n���� : ";
    cin >> playmode; //��?
    if (playmode == 1){
        goto RE;
    }
    
	return 0;
}

void print(){
    SetColor(BLACK*16+GREEN);
    gotoxy((FX-1)*2,FY-1);
    cout << "��" ;
    for (int i = 0 ; i < MW ; i++){
        if (i == 3)
            continue;
        gotoxy((FX+i)*2,FY-1);
        cout << "��" ;
    }
    cout << "��" ;
    for (int i = 0 ; i < MH ; i++){
        gotoxy((FX-1)*2,i+FY);
        cout << "��" ;
        gotoxy((FX+MW)*2,i+FY);
        cout << "��" ;
    }
    gotoxy((FX-1)*2,MH+FY);
    cout << "��" ;
    for (int i = 0 ; i < MW ; i++){
        gotoxy((FX+i)*2,MH+FY);
        cout << "��" ;
    }
    cout << "��";
    //��Ʋ 
    gotoxy((FX+MW+1)*2,FY-1);
    cout << "����������" ;
    for (int i = 0 ; i < MH ; i++){
        gotoxy((FX+MW+1)*2,i+FY);
        cout << "��" ;
        gotoxy((FX+MW+5)*2,i+FY);
        cout << "��" ;
    }
    gotoxy((FX+MW+1)*2,FY+MH);
    cout << "����������" ;
    gotoxy((FX+MW+2)*2,FY);
    cout<<" NEXT ";
    //����Ʋ 
    next.print();
    SetColor(BLACK*16+GREEN);
    gotoxy((FX+MW+2)*2,FY+4);
    cout.width(6);
    cout << "Combo";
    gotoxy((FX+MW+2)*2,FY+5);
    cout.width(6);
    cout << combonum;
    gotoxy((FX+MW+2)*2,FY+6);
    cout.width(6);
    cout << "MaxCom";
    gotoxy((FX+MW+2)*2,FY+7);
    cout.width(6);
    cout << max_combo;
    gotoxy((FX+MW+2)*2,FY+8);
    cout.width(6);
    cout << "Score";
    gotoxy((FX+MW+2)*2,FY+9);
    cout.width(6);
    cout << score;
    gotoxy((FX+MW+2)*2,FY+10);
    cout << "BGM";
    if (BGMon){
       SetColor(WHITE);
       cout << "ON ";
    }
    else{
       SetColor(RED);
       cout << "OFF";
    }
    SetColor(GREEN);
    gotoxy((FX+MW+2)*2,FY+11);
    cout << "Obsnum";
    gotoxy((FX+MW+2)*2,FY+12);
    cout << "  :";
    cout.width(3);
    cout << obstaclenum;
    //������� 
    if (pause == false){
       for (int i = 0 ; i < MH ; i++){
           for (int j = 0 ; j < MW ; j++){
               if (i == b.y && j == b.x)
                  continue;
               switch (b.direc){
                      case Up:
                           if (i == b.y-1 && j == b.x)
                              continue;
                           break;
                      case Left:
                           if (i == b.y && j == b.x-1)
                              continue;
                           break;
                      case Down:
                           if (i == b.y+1 && j == b.x)
                              continue;
                           break;
                      case Right:
                           if (i == b.y && j == b.x+1)
                              continue;
                           break;
                      default:
                           break;
               }
               gotoxy((FX+j)*2,FY+i);
               m_mine[i][j].b.print();
           }
       }
       //�ѿ�ѿ�� 
       
        b.print();
       //�������»ѿ���� 
    }
    else{
        SetColor(YELLOW);
        gotoxy((FX+MW/2-3)*2,FY+MH/2);
        cout << " P A U S E" ;
    }
    
    if (playmode == multi){
       SetColor(BLACK*16+GREEN);
       gotoxy((FX-1+MW+13)*2,FY-1);
       cout << "��" ;
       for (int i = 0 ; i < MW ; i++){
           if (i == 3)
               continue;
           gotoxy((FX+i+MW+13)*2,FY-1);
           cout << "��" ;
       }
       cout << "��" ;
       for (int i = 0 ; i < MH ; i++){
           gotoxy((FX-1+MW+13)*2,i+FY);
           cout << "��" ;
           gotoxy((FX+MW+MW+13)*2,i+FY);
           cout << "��" ;
       }
       gotoxy((FX-1+MW+13)*2,MH+FY);
       cout << "��" ;
       for (int i = 0 ; i < MW ; i++){
           gotoxy((FX+i+MW+13)*2,MH+FY);
           cout << "��" ;
       }
       cout << "��";
       //����Ʋ 
       gotoxy((FX+MW+1+6)*2,FY-1);
       cout << "����������" ;
       for (int i = 0 ; i < MH ; i++){
           gotoxy((FX+MW+1+6)*2,i+FY);
           cout << "��" ;
           gotoxy((FX+MW+5+6)*2,i+FY);
           cout << "��" ;
       }
       gotoxy((FX+MW+1+6)*2,FY+MH);
       cout << "����������" ;
       gotoxy((FX+MW+2+6)*2,FY);
       cout<<" NEXT ";
       //��뼳��Ʋ 
       next_o.print();
       SetColor(BLACK*16+GREEN);
       gotoxy((FX+MW+2+6)*2,FY+4);
       cout.width(6);
       cout << "Combo";
       gotoxy((FX+MW+2+6)*2,FY+5);
       cout.width(6);
       cout << combonum_o;
       gotoxy((FX+MW+2+6)*2,FY+6);
       cout.width(6);
       cout << "MaxCom";
       gotoxy((FX+MW+2+6)*2,FY+7);
       cout.width(6);
       cout << max_combo_o;
       gotoxy((FX+MW+2+6)*2,FY+8);
       cout.width(6);
       cout << "Score";
       gotoxy((FX+MW+2+6)*2,FY+9);
       cout.width(6);
       cout << score_o;
       gotoxy((FX+MW+2+6)*2,FY+10);
       cout << "      ";
       SetColor(GREEN);
       gotoxy((FX+MW+2+6)*2,FY+11);
       cout << "Obsnum";
       gotoxy((FX+MW+2+6)*2,FY+12);
       cout << "  :";
       cout.width(3);
       cout << obstaclenum_o;
       //���������� 
       for (int i = 0 ; i < MH ; i++){
           for (int j = 0 ; j < MW ; j++){
               if (i == b_o.y && j == b_o.x)
                  continue;
               switch (b_o.direc){
                      case Up:
                           if (i == b_o.y-1 && j == b_o.x)
                              continue;
                           break;
                      case Left:
                           if (i == b_o.y && j == b_o.x-1)
                              continue;
                           break;
                      case Down:
                           if (i == b_o.y+1 && j == b_o.x)
                              continue;
                           break;
                      case Right:
                           if (i == b_o.y && j == b_o.x+1)
                              continue;
                           break;
                      default:
                           break;
               }
               gotoxy((FX+j+MW+13)*2,FY+i);
               m_other[i][j].b.print();
           }
       }
       //�ѿ�ѿ���� 
       b_o.SetXY(b_o.x+MW+13,b_o.y);
       b_o.print();
       //����� �������� �ѿ� ��� 
       gotoxy((FX+MW+6)*2,FY+9);
       cout << "  ";
       //Ŭ���� (?) 
       
       switch (effectmove){
              case 1:case 11: //�������� ������ �� 
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW/2-1)*2,FY+6-1);
                   printf("**");
                   gotoxy((FX+MW/2+1)*2,FY+6-1);
                   printf("**");
                   gotoxy((FX+MW/2-1)*2,FY+6+1);
                   printf("**");
                   gotoxy((FX+MW/2+1)*2,FY+6+1);
                   printf("**");
                   break;
              case 2:case 12:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW/2)*2,FY+6);
                   printf("@@");
                   break;
              case 3:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+1)*2,FY+7);
                   printf("@@");
                   break;
              case 4:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+3)*2,FY+8);
                   printf("@@");
                   break;
              case 5:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+6)*2,FY+9);
                   printf("@@");
                   break;
              case 6:case 16: 
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+9-1)*2,FY+11-1);
                   printf("**");
                   gotoxy((FX+MW+9+1)*2,FY+11-1);
                   printf("**");
                   gotoxy((FX+MW+9-1)*2,FY+11+1);
                   printf("**");
                   gotoxy((FX+MW+9+1)*2,FY+11+1);
                   printf("**");
                   gotoxy((FX+MW+9)*2,FY+11);
                   printf("@@");
                   break;
                   
              case 13:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW)*2,FY+8);
                   printf("@@");
                   break;
              case 14:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+3)*2,FY+11);
                   printf("@@");
                   if (ifeffectp)
                      effectmove = 0;
                   break;
              case 15:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+7)*2,FY+9);
                   printf("@@");
                   break;
              default:
                   effectmove = 0;
                   break;
       }
       switch (effectmove_o){
              case 1:case 11: //�������� ������ �� 
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+13+MW/2-1)*2,FY+6-1);
                   printf("**");
                   gotoxy((FX+MW+13+MW/2+1)*2,FY+6-1);
                   printf("**");
                   gotoxy((FX+MW+13+MW/2-1)*2,FY+6+1);
                   printf("**");
                   gotoxy((FX+MW+13+MW/2+1)*2,FY+6+1);
                   printf("**");
                   break;
              case 2:case 12:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+13+MW/2)*2,FY+6);
                   printf("@@");
                   break;
              case 3:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+13-2)*2,FY+7);
                   printf("@@");
                   break;
              case 4:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+13-5)*2,FY+8);
                   printf("@@");
                   break;
              case 5:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+13-7)*2,FY+9);
                   printf("@@");
                   break;
              case 6:case 16: 
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+3-1)*2,FY+11-1);
                   printf("**");
                   gotoxy((FX+MW+3+1)*2,FY+11-1);
                   printf("**");
                   gotoxy((FX+MW+3-1)*2,FY+11+1);
                   printf("**");
                   gotoxy((FX+MW+3+1)*2,FY+11+1);
                   printf("**");
                   gotoxy((FX+MW+3)*2,FY+11);
                   printf("@@");
                   break;
                   
              case 13:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+13-1)*2,FY+8);
                   printf("@@");
                   break;
              case 14:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+13-4)*2,FY+11);
                   printf("@@");
                   break;
              case 15:
                   SetColor(SKY_BLUE);
                   gotoxy((FX+MW+13-8)*2,FY+9);
                   printf("@@");
                   break;
              default:
                   effectmove_o = 0;
                   break;
       }
       //�hŷ ����Ʈ 
    }
}

void multicalculate(int mypos, SOCKET *sock, SOCKET *csock, SOCKET *lsock){
     int retval;
     char sendb[STRSIZE]="";
     char recvb[STRSIZE]="";
     char strtemp[STRSIZE]="";
     int bb;
     
     sprintf(strtemp, " %d", ifwin); strcat(sendb, strtemp); //������ ���θ� ������. 
     for (int i = 0 ; i < MH ; i++){
         for (int j = 0 ; j < MW ; j++){
             sprintf(strtemp, " %d", m_mine[i][j].b.color); strcat(sendb, strtemp); //���� ������. 
         }
     }
     sprintf(strtemp, " %d", combonum); strcat(sendb, strtemp);
     sprintf(strtemp, " %d", max_combo); strcat(sendb, strtemp);
     sprintf(strtemp, " %d", score); strcat(sendb, strtemp);
     sprintf(strtemp, " %d", ifeffect); strcat(sendb, strtemp); //�� ���¸� ������. 
     
     sprintf(strtemp, " %d", b.bbuyo[0].color); strcat(sendb, strtemp); //
     sprintf(strtemp, " %d", b.bbuyo[1].color); strcat(sendb, strtemp);
     sprintf(strtemp, " %d", b.direc); strcat(sendb, strtemp);
     sprintf(strtemp, " %d", b.x); strcat(sendb, strtemp);
     sprintf(strtemp, " %d", b.y); strcat(sendb, strtemp);
     
     sprintf(strtemp, " %d", next.bbuyo[0].color); strcat(sendb, strtemp);
     sprintf(strtemp, " %d", next.bbuyo[1].color); strcat(sendb, strtemp);
     sprintf(strtemp, " %d", next.direc); strcat(sendb, strtemp);
     
     sprintf(strtemp, " %d", effectmove); strcat(sendb, strtemp);
     
     if (mypos == server){
        retval = recv(*csock, recvb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("������ ������ϴ�..." ),TEXT("����"),MB_OK);
            gameover = false;
            return;
        }
        retval = send(*csock, sendb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("������ ������ϴ�..." ),TEXT("����"),MB_OK);
            gameover = false;
            return;
        }
     }
     else if (mypos == client){
        retval = send(*sock, sendb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("������ ������ϴ�..." ),TEXT("Ŭ��"),MB_OK);
            gameover = false;
            return;
        }
        retval = recv(*sock, recvb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("������ ������ϴ�..." ),TEXT("Ŭ��"),MB_OK);
            gameover = false;
            return;
        }
     }
     
     strcpy(strtemp, recvb);
     
     sscanf(strtemp, "%d%n", &ifwin_o, &bb);trim(strtemp, bb, strlen(strtemp));
     for (int i = 0 ; i < MH ; i++){
         for (int j = 0 ; j < MW ; j++){
             sscanf(strtemp, "%d%n", &m_other[i][j].b.color, &bb);trim(strtemp, bb, strlen(strtemp));
         }
     }
     sscanf(strtemp, "%d%n", &combonum_o, &bb);trim(strtemp, bb, strlen(strtemp));
     sscanf(strtemp, "%d%n", &max_combo_o, &bb);trim(strtemp, bb, strlen(strtemp));
     sscanf(strtemp, "%d%n", &score_o, &bb);trim(strtemp, bb, strlen(strtemp));
     sscanf(strtemp, "%d%n", &ifeffect_o, &bb);trim(strtemp, bb, strlen(strtemp));
     
     sscanf(strtemp, "%d%n", &b_o.bbuyo[0].color, &bb);trim(strtemp, bb, strlen(strtemp));
     sscanf(strtemp, "%d%n", &b_o.bbuyo[1].color, &bb);trim(strtemp, bb, strlen(strtemp));
     sscanf(strtemp, "%d%n", &b_o.direc, &bb);trim(strtemp, bb, strlen(strtemp));
     sscanf(strtemp, "%d%n", &b_o.x, &bb);trim(strtemp, bb, strlen(strtemp));
     sscanf(strtemp, "%d%n", &b_o.y, &bb);trim(strtemp, bb, strlen(strtemp));
    
     sscanf(strtemp, "%d%n", &next_o.bbuyo[0].color, &bb);trim(strtemp, bb, strlen(strtemp));
     sscanf(strtemp, "%d%n", &next_o.bbuyo[1].color, &bb);trim(strtemp, bb, strlen(strtemp));
     sscanf(strtemp, "%d%n", &next_o.direc, &bb);trim(strtemp, bb, strlen(strtemp));
     
     sscanf(strtemp, "%d%n", &effectmove_o, &bb);trim(strtemp, bb, strlen(strtemp));
     
     ifeffect=0;
}

void multicalculate2(int mypos, SOCKET *sock, SOCKET *csock, SOCKET *lsock){
     int recvdobs;
     int recvdobsover;
     
     int retval;
     char sendb[STRSIZE]="";
     char recvb[STRSIZE]="";
     char strtemp[STRSIZE]="";
     int bb;
     
     sprintf(strtemp, " %d", obstaclenum_s); strcat(sendb, strtemp); //��ֹ��� ������. 
     sprintf(strtemp, " %d", ifobsover); strcat(sendb, strtemp); //
     
     if (mypos == server){
        retval = recv(*csock, recvb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("������ ������ϴ�..." ),TEXT("����"),MB_OK);
            gameover = false;
            return;
        }
        retval = send(*csock, sendb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("������ ������ϴ�..." ),TEXT("����"),MB_OK);
            gameover = false;
            return;
        }
     }
     else if (mypos == client){
        retval = send(*sock, sendb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("������ ������ϴ�..." ),TEXT("Ŭ��"),MB_OK);
            gameover = false;
            return;
        }
        retval = recv(*sock, recvb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("������ ������ϴ�..." ),TEXT("Ŭ��"),MB_OK);
            gameover = false;
            return;
        }
     }
     
     strcpy(strtemp, recvb);
     
     sscanf(strtemp, "%d%n", &recvdobs, &bb);trim(strtemp, bb, strlen(strtemp));
     sscanf(strtemp, "%d%n", &recvdobsover, &bb);trim(strtemp, bb, strlen(strtemp));
     
     if (ifobsover || recvdobsover){
        obstaclenum_r = 0;
     }
     
     obstaclenum_r+=recvdobs; //���� �� ���ϰ� 
     obstaclenum_r-=obstaclenum_s; //���� �� ����. 
     
     if (obstaclenum_r > 0){
        obstaclenum = obstaclenum_r;
        obstaclenum_o = 0;
     }
     else if (obstaclenum_r < 0){
        obstaclenum = 0;
        obstaclenum_o = abs(obstaclenum_r);
     }
     else{
        obstaclenum = 0;
        obstaclenum_o = 0;
     }
     //���� ���� 
     
     ifobsover=0;
}

void calculate(){
     obstaclenum_s=0; //���� ���� �ʱ�ȭ 
     ifobs = false; //����Ʈ���� �ʱ�ȭ 
     
     if (p.IsStopped() && BGMon){
        p.SetPosition(0);
        p.Play();
     }
     for (int i=0;i<8;i++){
         if (eff_combo[i].IsStopped()){
            eff_combo[i].Pause();
            eff_combo[i].SetPosition(0);
         }
     }
     if (playmode == multi){
        if (ifeffect_o){
           switch (combonum_o){
                  case 1:case 2:case 3:
                  case 4:case 5:case 6:
                  case 7:eff_combo[combonum_o-1].Play();break;
                  default: eff_combo[7].Play(); break;
           }
        }
     }
     if (playmode == vscom){
        ;
     }
     //�뷡���! 
     
     if (!ifcombo) //�޺��� �������� ���̶�� 
        scorenum = 0.0; //0 
        
     if (playmode == multi){
        if (effectmove) //0�� �ƴϸ� 
           effectmove++; //����! 
     }
     if (playmode == vscom){
        if (effectmove) //0�� �ƴϸ� 
           effectmove++; //����! 
        if (effectmove_o) //0�� �ƴϸ� 
           effectmove_o++; //����! 
     }
     //����Ʈ ���� 
     
     desnum = 0; //0 
     if (delay == 0){ //���� �� �ν��� �� 
        if (check(m_mine)){ //�ν��� �� �ֳ� üũ 
           if (playmode == single)
              PlayEffectR(ID_DEST);
           combonum++; //�޺��� ����. 
           delay = 1; //�����µ� �ð��� �ش�. 
           ifcombo=1; //�޺������� �� 
           
           scorenum += desnum * pow(2,combonum) / 4;
           //(���� ���� �� + ����) * 2 ^ �޺� �� / 4
           
           if (playmode == multi){
              ifeffect = true; //��뿡�� ����Ʈ������ 
              
              if (combonum==1)
                 obstaclenum_s += 1 + 2 * (desnum-4);
              else
                 obstaclenum_s += (desnum-3) * combonum * 2;
                 //(���� ���� �� - 3) * �޺� �� * 2
                 
              for (int i=0;i<8;i++){
                  eff_combo[i].Pause();
                  eff_combo[i].SetPosition(0);
              }
              switch (combonum){
                     case 1:case 2:case 3:
                     case 4:case 5:case 6:
                     case 7:eff_combo[combonum-1].Play();break;
                     default: eff_combo[7].Play(); break;
              }
              //�� ����Ʈ ��� 
              
              if (obstaclenum_r > 0){ //������ ������ 
                  effectmove = 11; 
                  if (obstaclenum_r - obstaclenum_s >= 0) //������ ������ �ִ°Ÿ� 
                     ifeffectp = 1; //�׸� 
                  else
                     ifeffectp = 0; //
              }
              else{ //������ ������ 
                  effectmove = 1;
              }
              //�׸��׸��� ��ȣ 
              
              ifobs = false;
           }
           if (playmode == vscom){
              ;
           }
        }
        else if (ifcombo){ //�޺��� ����� 
           score += scorenum * 10; //���� 
           if (max_combo < combonum) //�ִ� �޺��� �ѱ�� 
              max_combo = combonum; //�װ� �ִ� �޺� 
           combonum = 0;
           ifcombo = 0; //�ʱ�ȭ 
        }
     }
     if (delay){ //�ð��� �ְ� �Ǿ��ִٸ� 
        time_delay++; //�ð��� ���. 
     }
     if (time_delay >= 5){ //�ð��� ������ 
        time_delay = 0; 
        delay = 0; //�ٽ� ������ ����! 
     }
     //�ν��� ���� üũ�ϰ�, �޺����θ� �Ǵ��ϰ�, 0.25�� ���Ŀ� �������� ���� 
     
     cleaning(m_mine); //üũ������ �μ��� �μ��� 
     gravity(m_mine); //�߷¿� ���� ����߸��� 
     
     if (!ifcombo && iffloor){ //�ٴڿ� ��Ұ� �޺��� �ƴϸ� 
        b.SetColor(nextc1,nextc2);
        b.SetDirec(Up);
        b.SetXY(SX,SY); //���ο� �ѿ���� 
        nextc1 = rand()%(4*difficulty)+1;
        nextc2 = rand()%(4*difficulty)+1;
        next.SetColor(nextc1,nextc2); //���� �ѿ� ���� 
        
        ifpressx = 0; //�ٽ� x�� ������ �ְ� 
        iffloor = 0; //�ٴڿ� �� ���� 
        time_gravity = 0;
        ifobs = true;
     }
     
     if (time_gravity < TG*(3-difficulty))
        time_gravity++;
     else{
        if (b.move(Down)){ //�ٴڿ� ������ 
           PlayEffectR(ID_DROP);
           b.tomap(); //������ ������. 
           iffloor=1; //�ٴڿ� ��Ҵ�. 
           b.bbuyo[1].color = empty;
           b.bbuyo[0].color = empty;
           b.SetXY(1*2,FY+MH+2);
        }
        time_gravity = 0;
     }
     gravity(m_mine); //�߷¿� ���� ����߸��� 
     
     if (m_mine[SY-1][SX].b.color != empty){ //�׷��� ���Ⱑ �� ������� 
        gameover=false; //���� 
        ifwin = false;
     }
     
     if (time_turndel >= 3) //������ 0.15�ʰ� ������ 
        turndel = false; //�ٽ� ���� �� �ְ� 
     if (turndel) //���� �����ٸ� 
        time_turndel++; //��Ÿ���� �ø��� 
     else time_turndel = 0; //�ƴϸ� �׳� 0 
     //���� ������ �ϴ� ȿ�� 
}

void AIcalculate1(){
     bool decide = false;
     
     Connectbbuyo *c;
     Connectbbuyo t;
     
     for (int i = 0 ; i < MH ; i++){
         for (int j = 0 ; j < MW ; j++){
              m_temp[i][j].b.color = m_other[i][j].b.color;
         }
     }
     b_t=b_o;
     
     //���� ��Ȳ ���� 
     ;
     
     for (int i = 0 ; i < MH && !decide ; i++){
         for (int j = 0 ; j < MW && !decide ; j++){
            if (m_temp[i][j].count == yet && m_temp[i][j].b.color != empty && m_temp[i][j].b.color != obstacle){ //���� �� ������ 
               c = new Connectbbuyo(j, i);
               t.next = c;
               num=1;
               floodfill(m_temp, j, i, &t);
               if (num == 3){
                   
                   //��ֹ� �ƴ� �� ���� Ȯ�� 
               }
               delete c;
               num = 0;
               t.next=NULL;
            }
         }
     }
     //3�� ¥���� �ֳ����� 
     //2�� ¥���� �ֳ����� 
     //1�� ¥���� �ֳ����� 
     //�� ������ �� ������ �ֳ����� 
     //����� ������ �˻� 
     //���������� �������� �˻� 
     //Ŀ�ǵ� ���� 
     //�Է� 
     
     if (effectmove_o) //0�� �ƴϸ� 
        effectmove_o++; //����! 
     //
     
     desnum_o = 0; //0 
     if (delay_o == 0){ //���� �� �ν��� �� 
        if (check(m_other)){ //�ν��� �� �ֳ� üũ 
           combonum_o++; //�޺��� ����. 
           delay_o = 1; //�����µ� �ð��� �ش�. 
           ifcombo_o=1; //�޺������� �� 
           
           scorenum += desnum * pow(2,combonum) / 4;
           //(���� ���� �� + ����) * 2 ^ �޺� �� / 4
           
           ifeffect_o = true;
           
           if (combonum_o==1)
              obstaclenum_s_o += 1 + 2 * (desnum-4);
           else
              obstaclenum_s_o += (desnum_o-3) * combonum_o * 2;
              //(���� ���� �� - 3) * �޺� �� * 2
           
           if (obstaclenum_r < 0){
               effectmove_o = 11; 
               if (obstaclenum_r - obstaclenum_s_o >= 0)
                  ifeffectp_o = 1; //�׸� 
               else
                  ifeffectp_o = 0;
           }
           else{
               effectmove_o = 1;
           }
           //�׸��׸��� ��ȣ 
           
           ifobs_o = false;
        }
        else if (ifcombo_o){ //�޺��� ����� 
           score_o += scorenum_o * 10; //���� 
           if (max_combo_o < combonum_o) //�ִ� �޺��� �ѱ�� 
              max_combo_o = combonum_o; //�װ� �ִ� �޺� 
           combonum_o = 0;
           ifcombo_o = 0; //�ʱ�ȭ 
        }
     }
     if (delay_o){ //�ð��� �ְ� �Ǿ��ִٸ� 
        time_delay_o++; //�ð��� ���. 
     }
     if (time_delay_o >= 5){ //�ð��� ������ 
        time_delay_o = 0; 
        delay_o = 0; //�ٽ� ������ ����! 
     }
     //�ν��� ���� üũ�ϰ�, �޺����θ� �Ǵ��ϰ�, 0.25�� ���Ŀ� �������� ���� 
     
     cleaning(m_other); //üũ������ �μ��� �μ��� 
     gravity(m_other); //�߷¿� ���� ����߸��� 
     
     if (!ifcombo_o && iffloor_o){ //�ٴڿ� ��Ұ� �޺��� �ƴϸ� 
        b_o.SetColor(nextc1_o,nextc2_o);
        b_o.SetDirec(Up);
        b_o.SetXY(SX,SY); //���ο� �ѿ���� 
        nextc1_o = rand()%(4*difficulty)+1;
        nextc2_o = rand()%(4*difficulty)+1;
        next_o.SetColor(nextc1,nextc2); //���� �ѿ� ���� 
        
        iffloor_o = 0; //�ٴڿ� �� ���� 
        time_gravity_o = 0;
        ifobs_o = true;
     }
     
     if (time_gravity_o < TG*(3-difficulty))
        time_gravity_o++;
     else{
        if (b_o.move(Down)){ //�ٴڿ� ������ 
           PlayEffectR(ID_DROP);
           b_o.tomap(); //������ ������. 
           iffloor=1; //�ٴڿ� ��Ҵ�. 
           b_o.bbuyo[1].color = empty;
           b_o.bbuyo[0].color = empty;
           b_o.SetXY(1*2,FY+MH+2);
        }
        time_gravity_o = 0;
     }
     gravity(m_other); //�߷¿� ���� ����߸��� 
     
     if (m_other[SY-1][SX].b.color != empty){ //�׷��� ���Ⱑ �� ������� 
        gameover=false; //���� 
        ifwin = true;
     }
}

bool check(Map (*m)[MW]){ //from top, left to bottom, right
    Connectbbuyo *c;
    Connectbbuyo t;

    bool ifbreak=0;
    for (int i = 0 ; i < MH ; i++){
        for (int j = 0 ; j < MW ; j++){
            if (m[i][j].count == yet && m[i][j].b.color != empty && m[i][j].b.color != obstacle){ //���� �� ������ 
               c = new Connectbbuyo(j, i);
               t.next = c;
               num=1;
               floodfill(m, j, i, &t);
               if (num >=4){
                   for (Connectbbuyo *n = c ; n != NULL ; n = n->next){
                       m[n->y][n->x].ifdes = yes;
                   }
                   desnum+=num; 
                   ifbreak=1;
               }
               delete c;
               num = 0;
               t.next=NULL;
            }
        }
    }
    return ifbreak;
}

int floodfill(Map (*m)[MW], int x, int y, Connectbbuyo *t){ //������ �Ȱ��� �� �ֳ�? //xy�� �˻����� �ʵ� 
    bool check[4]={false,false,false,false};
    
    m[y][x].count = alc;
    
    if ( x == 0 && y == 0 ){ //�� �� 
       check[Up] = false;check[Left] = false;check[Down] = true;check[Right] = true;
    }
    else
    if ( x == 0 && y == MH-1 ){ //�Ʒ� �� 
       check[Up] = true;check[Left] = false;check[Down] = false;check[Right] = true;
    }
    else
    if ( x == MW-1 && y == MH-1 ){ //�Ʒ� ���� 
       check[Up] = true;check[Left] = true;check[Down] = false;check[Right] = false;
    }
    else
    if ( x == MW-1 && y == 0 ){ //�� ���� 
       check[Up] = false;check[Left] = true;check[Down] =true ;check[Right] = false;
    }
    else
    if ( y == 0 ){ //���� 
       check[Up] = false;check[Left] = true;check[Down] = true;check[Right] = true;
    }
    else
    if ( x == 0 ){ //���� 
       check[Up] = true;check[Left] = false;check[Down] = true;check[Right] = true;
    }
    else
    if ( y == MH-1 ){ //�Ʒ��� 
       check[Up] = true;check[Left] = true;check[Down] = false;check[Right] = true;
    }
    else
    if ( x == MW-1 ){ //������ 
       check[Up] = true;check[Left] = true;check[Down] = true;check[Right] = false;
    }
    else{ //�߰� 
       check[Up] = true;check[Left] = true;check[Down] = true;check[Right] = true;
    }
    
    if (check[Up]){
       if (m[y-1][x].count == yet){
          if (m[y][x].b.color == m[y-1][x].b.color){
             t->next->next = new Connectbbuyo(x,y-1);
             t->next = t->next->next;
             num++;
             floodfill(m,x,y-1,t);
          }
          if (m[y-1][x].b.color == obstacle){
             t->next->next = new Connectbbuyo(x,y-1);
             t->next = t->next->next;
          }
       }
    }
    if (check[Left]){
       if (m[y][x-1].count == yet){
          if (m[y][x].b.color == m[y][x-1].b.color){
             t->next->next = new Connectbbuyo(x-1,y);
             t->next = t->next->next;
             num++;
             floodfill(m,x-1,y,t);
          }
          if (m[y][x-1].b.color == obstacle){
             t->next->next = new Connectbbuyo(x-1,y);
             t->next = t->next->next;
          }
       }
    }
    if (check[Down]){
       if (m[y+1][x].count == yet){
          if (m[y][x].b.color == m[y+1][x].b.color){
             t->next->next = new Connectbbuyo(x,y+1);
             t->next = t->next->next;
             num++;
             floodfill(m,x,y+1,t);
          }
          if (m[y+1][x].b.color == obstacle){
             t->next->next = new Connectbbuyo(x,y+1);
             t->next = t->next->next;
          }
       }
    }
    if (check[Right]){
       if (m[y][x+1].count == yet){
          if (m[y][x].b.color == m[y][x+1].b.color){
             t->next->next = new Connectbbuyo(x+1,y);
             t->next = t->next->next;
             num++;
             floodfill(m,x+1,y,t);
          }
          if (m[y][x+1].b.color == obstacle){
             t->next->next = new Connectbbuyo(x+1,y);
             t->next = t->next->next;
          }
       }
    }
}

void obstacledrop(){
     bool ee;
     obstacle_x = rand()%6; 
     
     for (int o = 0 ; o < obstaclenum ; o++){
         ee=true;
         for (int i = 0 ; i < MH && ee ; i++){
             if (m_mine[i][obstacle_x].b.color == empty){
                m_mine[i][obstacle_x].b.color = obstacle;
                ee = false;
             }
         }
         obstacle_x++;
         if (obstacle_x >= 6)
            obstacle_x = 0;
         ifobsover=1;
     }
     ifobs=false;
}

int cleaning(Map (*m)[MW]){ //���� �� ���ְ�, �߷��ۿ� 
    for (int i = 0 ; i < MH ; i++){
        for (int j = 0 ; j < MW ; j++){
            m[i][j].count = yet;
            if (m[i][j].ifdes == yes){
               m[i][j].b.color = empty;
               m[i][j].ifdes = no;
            }
        }
    }
}

void gravity(Map (*m)[MW]){
    for (int j = 0 ; j < MW ; j++){ //���ʺ��� 
        for (int i = MH-1 ; i >= 0 ; i--){ //�ٴں��� �ö󰡴ٰ� 
            for (int l = 0 ; l < MH && m[i][j].b.color == empty ; l++){ //������� 
                  for (int n = i ; n >= 1 ; n--){
                      m[n][j].b.color = m[n-1][j].b.color;
                  }
                  m[0][j].b.color = empty; //��ĭ�� ������. 
            } //�� �� ��� ������ �׸��Ѵ�.
        }
    }
}

/*
              switch (direc){
                     case Up:
                          
                          break;
                     case Left:
                          
                          break;
                     case Down:
                          
                          break;
                     case Right:
                          
                          break;
                     default:
                          
                          break;
              }
              //���⿡ ���� 
*/

/*
    if ( x == 0 && y == 0 ){ //�� �� 
       check[Up] = false;check[Left] = false;check[Down] = true;check[Right] = true;
    }
    else
    if ( x == 0 && y == MH-1 ){ //�Ʒ� �� 
       check[Up] = true;check[Left] = false;check[Down] = false;check[Right] = true;
    }
    else
    if ( x == MW-1 && y == MH-1 ){ //�Ʒ� ���� 
       check[Up] = true;check[Left] = true;check[Down] = false;check[Right] = false;
    }
    else
    if ( x == MW-1 && y == 0 ){ //�� ���� 
       check[Up] = false;check[Left] = true;check[Down] =true ;check[Right] = false;
    }
    else
    if ( y == 0 ){ //���� 
       check[Up] = false;check[Left] = true;check[Down] = true;check[Right] = true;
    }
    else
    if ( x == 0 ){ //���� 
       check[Up] = true;check[Left] = false;check[Down] = true;check[Right] = true;
    }
    else
    if ( y == MH-1 ){ //�Ʒ��� 
       check[Up] = true;check[Left] = true;check[Down] = false;check[Right] = true;
    }
    else
    if ( x == MW-1 ){ //������ 
       check[Up] = true;check[Left] = true;check[Down] = true;check[Right] = false;
    }
    else{ //�߰� 
       check[Up] = true;check[Left] = true;check[Down] = true;check[Right] = true;
    }
    //�� ��ġ�� ���� 
*/

/*
    for (int i = 0 ; i < MH ; i++){
        for (int j = 0 ; j < MW ; j++){
            //�� ��ü ��ȸ 
        }
    }
*/

/*
       if (mypos == server){
          ;
       }
       else if (mypos == client){
          ;
       }
*/
