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
#define MW 6 //맵의 크기 Height Width 
#define FX 1
#define FY 1 //맵의 위치 X Y 

#define SX 3
#define SY 1 //뿌요 시작 위치 

#define TG 10 //자동으로 내려가는데 걸리는 시간 

#define STRSIZE MH*MW*2+100 //전송할 파일 크기 

enum{yet, alc}; //count
enum{no, yes}; //ifdes
enum{empty, red, yellow, black, pink, purple, white, blue, lightbrown, obstacle}; //color
//빈 곳, 빨강, 노랑, 까망, 밝은보라, 보라, 하양, 파랑, 누리끼리, 방해물 
enum{Up, Left, Down, Right}; //direc
enum{single, multi, vscom, explain}; //playmode
enum{server, client}; //mypos

class Bbuyo{ //왜 만들었지... 
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
                           cout<<"●";
                           break;
                      case yellow:
                           SetColor(BLACK*16+YELLOW);
                           cout<<"●";
                           break;
                      case black:
                           SetColor(BLACK*16+DARK_GRAY);
                           cout<<"●";
                           break;
                      case pink:
                           SetColor(BLACK*16+VIOLET);
                           cout<<"●";
                           break;
                      case purple:
                           SetColor(BLACK*16+DARK_VIOLET);
                           cout<<"●";
                           break;
                      case white:
                           SetColor(BLACK*16+WHITE);
                           cout<<"●";
                           break;
                      case blue:
                           SetColor(BLACK*16+BLUE);
                           cout<<"●";
                           break;
                      case lightbrown:
                           SetColor(BLACK*16+DARK_YELLOW);
                           cout<<"●";
                           break;
                      case obstacle:
                           SetColor(BLACK*16+GRAY);
                           cout<<"○";
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
               while(!move(Down)); //이동하지 못 할 때까지 반복 
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

          void Bigbbuyo::turn(){ //뿌요돌리기 
               switch (direc){
                      case Up:
                           if (y == 0) //맨 위이면
                              ; //아무것도 아님 
                           else{ //맨 위가 아니면 
                              if (x == MW-1){ //맨 오른쪽이면 
                                 if (m[y-1][x-1].b.color == empty){ //비어있으면 
                                    direc=Left; //턴! 
                                    y--; //위로 
                                 }
                                 else //안 비어있으면 
                                    swap(); //둘의 색을 바꾼다. 
                              }
                              else{ //맨 오른쪽이 아니면 
                                 if (m[y-1][x+1].b.color == empty){ //비어있으면 
                                    direc=Left; //턴! 
                                    x++; 
                                    y--; //오위로! 
                                 }
                                 else{ //안 비어있으면 
                                    if (m[y-1][x-1].b.color == empty){ //다른 쪽이 비어있으면 
                                       direc=Left; //턴! 
                                       y--; //위로 
                                    }
                                    else //안 비어있으면 
                                       swap(); //둘의 색을 바꾼다. 
                                 }
                              }
                           }
                           break;
                      case Left:
                           if ( y == MH-1 ){ //맨 아래이면 
                              y--; //위로 
                              direc=Down; //턴! 
                           }
                           else
                           if ( y == 0 ){ //맨 위이면 
                              if (m[y+1][x].b.color == empty){ //비어있으면 
                                 direc=Down; //턴! 
                              }
                              else //안 비었으면 
                                 swap(); //둘의 색을 바꾼다. 
                           }
                           else{ //중간이면 
                              if (m[y+1][x].b.color == empty){ //비어있으면 
                                 direc=Down; //턴!
                              }
                              else{
                                 y--; //위로 
                                 direc=Down; //턴!
                              }
                           }
                           break;
                      case Down:
                           if ( x == 0 ){ //맨 왼쪽이면 
                              if (m[y+1][x+1].b.color == empty){ //비어있으면 
                                 y++; //아래로 
                                 direc=Right; //턴! 
                              }
                              else //안 비어있으면 
                                 swap(); //둘의 색을 바꾼다. 
                           }
                           else
                           if ( x == MW-1 ){ //제일 오른쪽이면 
                              if (m[y+1][x-1].b.color == empty){ //비어있으면 
                                 x--;
                                 y++; //왼 아래로 
                                 direc=Right; //턴! 
                              }
                              else //안비어있으면 
                                 swap(); //둘의 색을 바꾼다. 
                           }
                           else{ //아니면 
                              if (m[y+1][x-1].b.color == empty){ //비어있으면 
                                 x--;
                                 y++; //왼 아래로 
                                 direc=Right; //턴! 
                              }
                              else{ //안 비어있으면 
                                 if (m[y+1][x+1].b.color == empty){ //다른 곳이 비어있으면 
                                    y++; //아래로 
                                    direc=Right; //턴 
                                 }
                                 else //안 비어있으면
                                    swap(); //둘의 색을 바꾼다. 
                              }
                           }
                           break;
                      case Right:
                           if ( y == 0 ){ //맨 위이면 
                              if (m[y+1][x].b.color == empty){ //비어있으면 
                                 y++; //아래로 
                                 direc=Up; //턴! 
                              }
                              else //안 비었으면 
                                 swap(); //둘의 색을 바꾼다. 
                           }
                           else{ //중간이면 
                              direc=Up; //턴! 
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
                                      if ( x == 0 ){ //맨 왼쪽이면 
                                         return 1; //아무것도 안 한다. 
                                      }
                                      else //맨 왼쪽이 아니면 
                                      if (m[y][x-1].b.color == empty){ //왼쪽이 비었으면 
                                         x--; //왼쪽으로 
                                      }
                                      else //안 비었으면 
                                         return 1; //아무것도 안 한다. 
                                      break;
                                 case Left:
                                      if ( x <= 1 ){ //맨 왼쪽이면 
                                         return 1; //아무것도 안 한다. 
                                      }
                                      else //맨 왼쪽이 아니면 
                                      if (m[y][x-2].b.color == empty){ //왼쪽이 비었으면 
                                         x--; //왼쪽으로 
                                      }
                                      else //안 비었으면 
                                         return 1; //아무것도 안 한다. 
                                      break;
                                 case Down:
                                      if ( x == 0 ){ //맨 왼쪽이면 
                                         return 1; //아무것도 안 한다. 
                                      }
                                      else //맨 왼쪽이 아니면 
                                      if (m[y+1][x-1].b.color == empty){ //왼쪽이 비었으면 
                                         x--; //왼쪽으로 
                                      }
                                      else //안 비었으면 
                                         return 1; //아무것도 안 한다. 
                                      break;
                                 case Right:
                                      if ( x == 0 ){ //맨 왼쪽이면 
                                         return 1; //아무것도 안 한다. 
                                      }
                                      else //맨 왼쪽이 아니면 
                                      if (m[y][x-1].b.color == empty){ //왼쪽이 비었으면 
                                         x--; //왼쪽으로 
                                      }
                                      else //안 비었으면 
                                         return 1; //아무것도 안 한다. 
                                      break;
                                 default:
                                      
                                      break;
                          }
                          break;
                     case Down:
                          switch (direc){
                                 case Up:
                                      if ( y == MH-1 ){ //맨 아래쪽이면 
                                         return 1; //아무것도 안한다. 
                                      }
                                      else //맨 아래쪽이 아니면 
                                      if (m[y+1][x].b.color == empty){ //아래쪽이 비었으면 
                                         y++; //아래쪽으로 
                                      }
                                      else //안 비었으면 
                                         return 1; //아무것도 안한다. 
                                      break;
                                 case Left:
                                      if ( y == MH-1 ){ //맨 아래쪽이면 
                                         return 1; //아무것도 안한다. 
                                      }
                                      else //맨 아래쪽이 아니면 
                                      if (m[y+1][x].b.color == empty && m[y+1][x-1].b.color == empty){ //아래쪽이 둘다 비었으면 
                                         y++; //아래쪽으로 
                                      }
                                      else //안 비었으면 
                                         return 1; //아무것도 안한다. 
                                      break;
                                 case Down:
                                      if ( y >= MH-2 ){ //맨 아래쪽이면 
                                         return 1; //아무것도 안한다. 
                                      }
                                      else //맨 아래쪽이 아니면 
                                      if (m[y+2][x].b.color == empty){ //아래쪽이 비었으면 
                                         y++; //아래쪽으로 
                                      }
                                      else //안 비었으면 
                                         return 1; //아무것도 안한다. 
                                      break;
                                 case Right:
                                      if ( y == MH-1 ){ //맨 아래쪽이면 
                                         return 1; //아무것도 안한다. 
                                      }
                                      else //맨 아래쪽이 아니면 
                                      if (m[y+1][x].b.color == empty && m[y+1][x+1].b.color == empty){ //아래쪽이 둘다 비었으면 
                                         y++; //아래쪽으로 
                                      }
                                      else //안 비었으면 
                                         return 1; //아무것도 안한다. 
                                      break;
                                 default:
                                      
                                      break;
                          }
                          break;
                     case Right:
                          switch (direc){
                                 case Up:
                                     if ( x == MW-1 ){ //맨 오른쪽이면 
                                         return 1; //아무것도 안한다. 
                                      }
                                      else //맨 오른쪽이 아니면 
                                      if (m[y][x+1].b.color == empty){ //오른쪽이 비었으면 
                                         x++; //오른쪽으로 
                                      }
                                      else //안 비었으면 
                                         return 1; //아무것도 안한다. 
                                      break;
                                 case Left:
                                      if ( x == MW-1 ){ //맨 오른쪽이면 
                                         return 1; //아무것도 안한다. 
                                      }
                                      else //맨 오른쪽이 아니면 
                                      if (m[y][x+1].b.color == empty){ //오른쪽이 비었으면 
                                         x++; //오른쪽으로 
                                      }
                                      else //안 비었으면 
                                         return 1; //아무것도 안한다. 
                                      break;
                                 case Down:
                                      if ( x == MW-1 ){ //맨 오른쪽이면 
                                         return 1; //아무것도 안한다. 
                                      }
                                      else //맨 오른쪽이 아니면 
                                      if (m[y+1][x+1].b.color == empty){ //오른쪽이 비었으면 
                                         x++; //오른쪽으로 
                                      }
                                      else //안 비었으면 
                                         return 1; //아무것도 안한다. 
                                      break;
                                 case Right:
                                      if ( x >= MW-2 ){ //맨 오른쪽이면 
                                         return 1; //아무것도 안한다. 
                                      }
                                      else //맨 오른쪽이 아니면 
                                      if (m[y][x+2].b.color == empty){ //오른쪽이 비었으면 
                                         x++; //오른쪽으로 
                                      }
                                      else //안 비었으면 
                                         return 1; //아무것도 안한다. 
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
              return 0; //이동 성공! 
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
    int playmode; //2p여부 
    int difficulty; //난이도 
    //common
    int random; //랜덤변수 
    bool gameover=true; //true면 진행 false면 게임오버 
    bool delay=0; //대기하기 
    int time_delay=0; //대기시간 
    bool ifcombo=0; //콤보여부 
    int combonum=0; //콤보수 
    int max_combo=0; //맥스콤보 
    int desnum; //한번에 부수는 개수 
    double scorenum; //점수에 곱할 것 
    int score=0; //점수 
    int time_gravity=0; //중력시간 
    bool iffloor=0; //바닥에 닿았는지 
    bool ifpressx=0; //x를 한번 눌렀는지 
    bool turndel=0; //턴을눌렀는지 
    int time_turndel=0; //턴 누르고 나서 시간재기 
    Bigbbuyo b(m_mine);
    int nextc1, nextc2; //다음색상 
    Bigbbuyo next; //다음 뿌요 
    //single 
    bool BGMon=true; //true면 재생, 아니면 멈춤 
    bool pause=false; //true면 멈춤, 아니면 진행 
    //mulit 
    int obstaclenum; //받은 방해뿌요개수 
    int obstaclenum_o; //상대의 남아있는 방해뿌요 개수 
    int obstaclenum_r=0; //서버에 존재해있는 방해뿌요 개수 
    int obstaclenum_s; //상대에게 보낼 방해뿌요개수 
    int obstacle_x=0; //받을 방해뿌요 x좌표 
    bool ifobs=false; //방해뿌요 떨어뜨려야 하는지 
    bool ifobsover=false; //방해뿌요 떨어뜨렸는지 
    bool ifwin=true; //true면 이긴거 false면 진거 
    int ifwin_o=true; //true면 이긴거 false면 진거 
    bool ifeffect=0; //내 이펙트 true면 재생 
    int ifeffect_o=0; //상대의 이펙트 재생 
    int effectmove; //그림 
    int effectmove_o; //상대 그림 //1~6 : 11~168
    bool ifeffectp; //그림종류 
    int ifeffectp_o; //그림종류 
    int combonum_o=0; //상대 콤보수 
    int max_combo_o=0; //상대 맥스콤보 
    int score_o=0; //상대 점수 
    Bigbbuyo b_o(m_other); //상대의 현재 뿌요 
    Bigbbuyo next_o; //상대의 다음 뿌요 
    //AI 
    int obstaclenum_s_o; //컴퓨터가 보낼 방해뿌요개수 
    int obstacle_x_o=0; //컴퓨터가 받을 방해뿌요 x좌표 
    bool ifobs_o=false; //방해뿌요 떨어뜨려야 하는지 
    bool ifobsover_o=false; //방해뿌요 떨어뜨렸는지 
    int desnum_o; 
    bool delay_o;
    bool ifcombo_o;
    double scorenum_o; 
    int time_delay_o;
    bool iffloor_o;
    int nextc1_o, nextc2_o;
    int time_gravity_o;
    Bigbbuyo b_t(m_temp); //컴퓨터 생각할 여지 뿌요 
    
void Restart(){
    for (int i = 0 ; i < MH ; i++){
        for (int j = 0 ; j < MW ; j++){
            m_mine[i][j].Clear();
            m_other[i][j].Clear();
        }
    }
    //common
    gameover=true; //true면 진행 false면 게임오버 
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
    ifobs_o=false; //방해뿌요 떨어뜨려야 하는지 
    ifobsover_o=false; //방해뿌요 떨어뜨렸는지 
}
    
int main(){
    system("title 뿌요뿌요 by 이인용. 종료는 Ctrl + C");
RE:
    system("cls");
    cout << "게임 방법을 선택해주세요.\n1. 혼자하기\n2. 둘이하기\n3. 컴과하기\n4. 하는방법\n선택 : ";
    cin >> playmode;
    if (playmode != 1 && playmode != 2 && playmode != 3 && playmode != 4) //다른 거 대답했으면 
       playmode = 1; //강제로 1 
    playmode--;
    system("cls");
    if (playmode == single){
       cout << "난이도를 선택해주세요.\n난이도 1 - 4색, 순한맛\n난이도 2 - 8색, 매운맛\n난이도 3 - 나가기\n선택 : ";
       cin >> difficulty;
       if (difficulty != 1 && difficulty != 2 && difficulty != 3) //다른 거 대답했으면 
          difficulty = 1; //강제로 1 
       
       system("cls");
       if (difficulty == 3)
          goto SINGLEEND;
       cout << "Loading";
       p.Open("효과음\\bimbim.bbu");
       
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
       //조작 
       
       srand(random); //뿌요나올것 결정 
       
       b.SetColor(rand()%(4*difficulty)+1,rand()%(4*difficulty)+1);
       b.SetXY(SX,SY); //새로운 뿌요등장 
       next.SetXY((MW+3),FY+1);
       nextc1 = rand()%(4*difficulty)+1;
       nextc2 = rand()%(4*difficulty)+1;
       next.SetColor(nextc1,nextc2); //다음 뿌요 결정 
       while (gameover){
             if (pause == false){
                calculate();
                //계산하기 
                
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
             //입력받기 
             
             print();
             //출력하기 
             
             Sleep(50);
             //대기하기 
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
       char strtemp[STRSIZE]; //으아아 
       
       { //문자초기화 
           for (int i=0;i<STRSIZE;i++) sendb[i]=' ';sendb[STRSIZE-1]='\0';
           for (int i=0;i<STRSIZE;i++) recvb[i]=' ';sendb[STRSIZE-1]='\0';
           for (int i=0;i<STRSIZE;i++) strtemp[i]=' ';sendb[STRSIZE-1]='\0';
       }
       
       if (WSAStartup(MAKEWORD(2,2), &wsa) != 0){ //윈속스타트업 
          MessageBox(NULL,TEXT("윈속 초기화 실패\n메모리가 부족한 현상이니 재부팅을 권합니다.\n죄송합니다." ),TEXT("안내"),MB_OK);
          return -1;
       }
       
       SetColor(WHITE);
       cout << "안녕하세요^^ 멀티뿌요뿌요에 오신 것을 환영합니다.\n";
       cout << "제가 멀티게임 자체를 처음 만들어서 이상하게 진행될 수도 있답니다!\n";
       cout << "데이터가 256바이트만큼 0.02초마다 교환되니\n";
       cout << "양에 비례하여 요금이 부과되는 통신(핫스팟 등)으로는 하지 마세요!\n";
       SetColor(YELLOW);
       cout << "\n먼저 당신의 역할을 정해주세요.\n";
       cout << "서버는 방을 만드는 개념이고,\n";
       cout << "클라이언트는 만든 방에 들어가는 개념입니다.\n";
       SetColor(GREEN);
       cout << "\n서버이신 분은 자신의 아이피주소를 클라이언트에게 넘겨주세요(IP를 소중히 합시다)\n";
       cout << "아이피 주소는 시작 - 모든 프로그램 - 보조 프로그램 - 명령프롬프트\n";
       cout << "ipconfig를 치시고 쭉 나오는 목록에서 무선 랜 어댑터나\n이더넷 어댑터의 IPv4를 알려주시면 됩니다.\n";
       SetColor(RED);
       cout << "\n서버가 공유기를 사용하고 있다면 포트포워딩을 해야합니다.\n";
       cout << "공유기 제어 사이트에 들어가셔서 포트포워딩의 포트를 9595로 해주시면 됩니다.\n";
       cout << "포트포워딩이 뭔지는 인터넷을 참조하시길 바랍니다.\n";
       cout << "(공유기마다 다르며 그림없이 설명하기 조금 그럼)\n";
       SetColor(GRAY);
       cout << "\n이상한 것을 입력하실 시 서버로 자동전환됩니다.\n";
       cout << "1. 서버\n2. 클라이언트\n3. 나가기\n선택 : ";
       cin >> mypos;
       if (mypos != 1 && mypos != 2 && mypos != 3) //다른 거 대답했으면 
          mypos = 1; //강제로 1 
       mypos--;
       system("cls");
       if (mypos == server){ //서버면 
          lsock = socket(AF_INET, SOCK_STREAM, 0); //날 연결시켜라! 
          
          ZeroMemory(&serveraddr, sizeof(serveraddr));
          serveraddr.sin_family = AF_INET;
          serveraddr.sin_port = htons(9595);
          serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
          retval = bind(lsock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
          if (retval == SOCKET_ERROR){
             MessageBox(NULL,TEXT("바인드 에러\n서버를 두개나 킨 것 같습니다.\n확인해 보시고 안 되면 재부팅하세요." ),TEXT("서버"),MB_OK);
             return -1;
          }
          //바인드 
          
          retval = listen(lsock, SOMAXCONN);
          if (retval == SOCKET_ERROR){
             MessageBox(NULL,TEXT("리슨 에러" ),TEXT("서버"),MB_OK);
             return -1;
          }
          //리슨 
          
          printf("접속을 기다리는 중입니다...");
          Sleep(1000);
          addrlen = sizeof(clientaddr);
          csock = accept(lsock, (SOCKADDR *)&clientaddr, &addrlen);
          printf("\n접속했습니다!\nIP %s, 포트 %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
          //액샙트 
          Sleep(2000);
       }
       else if (mypos == client){
          sock = socket(AF_INET, SOCK_STREAM, 0);
          
          SOCKADDR_IN serveraddr;
          ZeroMemory(&serveraddr, sizeof(serveraddr));
          serveraddr.sin_family = AF_INET;
          serveraddr.sin_port = htons(9595);
       RECONNECT:
          printf("서버의 IP를 입력하세요 예) xxx.xxx.xxx.xxx + 엔터\n입력 : ");
          scanf("%s",strtemp);
          serveraddr.sin_addr.s_addr = inet_addr(strtemp);
          
          retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
          if (retval == SOCKET_ERROR){
             MessageBox(NULL,TEXT("커넥트 에러\n올바르지 않은 IP이거나 서버가 열리지 않았습니다.\n다시 입력해주세요" ),TEXT("클라"),MB_OK);
             system("cls");
             goto RECONNECT;
          }
          printf("\n접속했습니다!");
          //커넥트 
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
       
       p.Open("효과음\\bimbim.bbu");
       eff_combo[0].Open("효과음\\C01.bbu");
       eff_combo[1].Open("효과음\\C02.bbu");
       eff_combo[2].Open("효과음\\C03.bbu");
       eff_combo[3].Open("효과음\\C04.bbu");
       eff_combo[4].Open("효과음\\C05.bbu");
       eff_combo[5].Open("효과음\\C06.bbu");
       eff_combo[6].Open("효과음\\C07.bbu");
       eff_combo[7].Open("효과음\\C08.bbu");
       
       p.Play();
       
       if (mypos == server){
          retval = recv(csock, recvb, 3, 0);
          if (retval == SOCKET_ERROR){
              MessageBox(NULL,TEXT("받기를 실패했습니다. 종료합니다." ),TEXT("서버"),MB_OK);
              return -1;
          }
          sscanf(recvb, "%s", strtemp);
          if (strcmp(strtemp, "OK")!=0) return -1;
          sprintf(sendb, "%s", "OK");
          retval = send(csock, sendb, 3, 0);
          if (retval == SOCKET_ERROR){
              MessageBox(NULL,TEXT("보내기를 실패했습니다. 종료합니다." ),TEXT("서버"),MB_OK);
              return -1;
          }
       }
       else if (mypos == client){
          sprintf(sendb, "%s", "OK");
          retval = send(sock, sendb, 3, 0);
          if (retval == SOCKET_ERROR){
              MessageBox(NULL,TEXT("보내기를 실패했습니다. 종료합니다." ),TEXT("클라"),MB_OK);
              return -1;
          }
          retval = recv(sock, recvb, 3, 0);
          if (retval == SOCKET_ERROR){
              MessageBox(NULL,TEXT("받기를 실패했습니다. 종료합니다." ),TEXT("클라"),MB_OK);
              return -1;
          }
          sscanf(recvb, "%s", strtemp);
          if (strcmp(strtemp, "OK")!=0) return -1;
       }
       //신호교환 
       system("cls");
       CursorView(0);
       
       { //문자초기화 
           for (int i=0;i<STRSIZE;i++) sendb[i]=' ';sendb[STRSIZE-1]='\0';
           for (int i=0;i<STRSIZE;i++) recvb[i]=' ';sendb[STRSIZE-1]='\0';
           for (int i=0;i<STRSIZE;i++) strtemp[i]=' ';sendb[STRSIZE-1]='\0';
       }
       if (mypos == server){
          random = rand();
          sprintf(sendb, "%10d", random);
          retval = send(csock, sendb, 11, 0);
          if (retval == SOCKET_ERROR){
              MessageBox(NULL,TEXT("보내기를 실패했습니다. 종료합니다." ),TEXT("서버"),MB_OK);
              return -1;
          }
       }
       else if (mypos == client){
          retval = recv(sock, recvb, 11, 0);
          if (retval == SOCKET_ERROR){
              MessageBox(NULL,TEXT("받기를 실패했습니다. 종료합니다." ),TEXT("클라"),MB_OK);
              return -1;
          }
          sscanf(recvb, "%d", &random);
       }
       //랜덤한거 교환 
       
       srand(random);
        
       b.SetColor(rand()%(4*difficulty)+1,rand()%(4*difficulty)+1);
       b.SetXY(SX,SY); //새로운 뿌요등장 
       next.SetXY((MW+3),FY+1);
       nextc1 = rand()%(4*difficulty)+1;
       nextc2 = rand()%(4*difficulty)+1;
       next.SetColor(nextc1,nextc2); //다음 뿌요 결정 
       next_o.SetXY((MW+9), FY+1); //상대의 다음 뿌요 위치 
       
       while (gameover){
             calculate();
             multicalculate(mypos, &sock, &csock, &lsock);
             multicalculate2(mypos, &sock, &csock, &lsock);
             if (!ifwin_o){ //이기면 
                ifwin = 1; //이김! 
                gameover=false;
             }
             if (ifobs && combonum_o == 0){ //떨어트려야하면, 상대의 콤보가 다 끝났으면 
                obstacledrop(); //방해물드랍 
             }
             //계산하기 
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
             //입력받기 
             
             print();
             //출력하기 
             
             Sleep(20);
             //대기하기 
       }
       system("cls");
       
       if (mypos == server){
          closesocket(csock);
          closesocket(lsock);
       }
       else if (mypos == client){
          closesocket(sock);
       }
       //통신종료 
       
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
       cout << "안녕하세요^^ 컴퓨터와의 대결에 오신 것을 환영합니다.\n";
       cout << "제가 인공지능 자체를 처음 만들어서 이상하게 진행될 수도 있답니다!\n";
       SetColor(YELLOW);
       cout << "\n먼저 컴퓨터의 아이큐를 선택해주세요\n";
       cout << "\n";
       cout << "\n";
       SetColor(GREEN);
       cout << "\n그런데 아직 구현이 되질 않았답니다^^;\n";
       cout << "\n";
       cout << "\n";
       SetColor(RED);
       cout << "\n차근차근 만들고 있으니 걱정하지 마시고요.\n";
       cout << "\n";
       cout << "\n";
       cout << "\n";
       SetColor(GRAY);
       cout << "\n다시 초기화면으로 돌아갑니다.\n";
       cout << "";
       SetColor(GRAY);
       cout << "\n";
       system("pause");
       goto RE;
    }
    else
    if (playmode == explain){
       SetColor(WHITE);
       cout << " 안녕하세요^^\n";
       cout << "이인용의 뿌요뿌요입니다. 실행해주셔서 감사합니다.\n";
       cout << "버전 1.2.2S\n"; //Version 
       cout << "날짜 2014.2.24.월\n";
       SetColor(YELLOW);
       cout << "\n 하는 법\n";
       cout << "방향키 ←↓→ - 이동\n";
       cout << "Z - 회전\n";
       cout << "X - 내리꽂기\n";
       cout << "ESC - 배경음끄기/켜기\n";
       cout << "SPACE - 멈추기(혼자하기에서만^^)\n";
       SetColor(GREEN);
       cout << "\n 혼자하기\n";
       cout << "난이도 1과 2로 나뉘어져 있습니다.\n";
       cout << "난이도 1에서는 콤보연습을 난이도 2에서는 점수를 많이 얻도록 해봅시다.\n";
       cout << "(제 최대콤보(난이도 1)는 5이고, 최대점수(난이도 2)는 3000점입니다^^)\n";
       SetColor(RED);
       cout << "\n 둘이하기\n";
       cout << "인터넷이 연결되어 있는 다른 컴퓨터와 대전할 수 있습니다.\n";
       cout << "(자세한 사항은 해당 항목으로 넘어가주세요.)\n";
       SetColor(SKY_BLUE);
       cout << "\n 컴과하기 // 구현 중\n";
       cout << "컴퓨터와 대전할 수 있습니다.\n";
       cout << "(자세한 사항은 해당 항목으로 넘어가주세요.)\n";
       SetColor(GRAY);
       cout << "\n";
       system("pause");
       goto RE; 
    }
    if (difficulty == 3){
       goto RE;
    }
    
    cout << "다시 하시겠습니까?\n1. 예\n2. 아니오\n선택 : ";
    cin >> playmode; //응?
    if (playmode == 1){
        goto RE;
    }
    
	return 0;
}

void print(){
    SetColor(BLACK*16+GREEN);
    gotoxy((FX-1)*2,FY-1);
    cout << "┌" ;
    for (int i = 0 ; i < MW ; i++){
        if (i == 3)
            continue;
        gotoxy((FX+i)*2,FY-1);
        cout << "─" ;
    }
    cout << "┐" ;
    for (int i = 0 ; i < MH ; i++){
        gotoxy((FX-1)*2,i+FY);
        cout << "│" ;
        gotoxy((FX+MW)*2,i+FY);
        cout << "│" ;
    }
    gotoxy((FX-1)*2,MH+FY);
    cout << "└" ;
    for (int i = 0 ; i < MW ; i++){
        gotoxy((FX+i)*2,MH+FY);
        cout << "─" ;
    }
    cout << "┘";
    //맵틀 
    gotoxy((FX+MW+1)*2,FY-1);
    cout << "┌───┐" ;
    for (int i = 0 ; i < MH ; i++){
        gotoxy((FX+MW+1)*2,i+FY);
        cout << "│" ;
        gotoxy((FX+MW+5)*2,i+FY);
        cout << "│" ;
    }
    gotoxy((FX+MW+1)*2,FY+MH);
    cout << "└───┘" ;
    gotoxy((FX+MW+2)*2,FY);
    cout<<" NEXT ";
    //설명틀 
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
    //정보출력 
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
       //뿌요뿌요맵 
       
        b.print();
       //내려오는뿌요출력 
    }
    else{
        SetColor(YELLOW);
        gotoxy((FX+MW/2-3)*2,FY+MH/2);
        cout << " P A U S E" ;
    }
    
    if (playmode == multi){
       SetColor(BLACK*16+GREEN);
       gotoxy((FX-1+MW+13)*2,FY-1);
       cout << "┌" ;
       for (int i = 0 ; i < MW ; i++){
           if (i == 3)
               continue;
           gotoxy((FX+i+MW+13)*2,FY-1);
           cout << "─" ;
       }
       cout << "┐" ;
       for (int i = 0 ; i < MH ; i++){
           gotoxy((FX-1+MW+13)*2,i+FY);
           cout << "│" ;
           gotoxy((FX+MW+MW+13)*2,i+FY);
           cout << "│" ;
       }
       gotoxy((FX-1+MW+13)*2,MH+FY);
       cout << "└" ;
       for (int i = 0 ; i < MW ; i++){
           gotoxy((FX+i+MW+13)*2,MH+FY);
           cout << "─" ;
       }
       cout << "┘";
       //상대맵틀 
       gotoxy((FX+MW+1+6)*2,FY-1);
       cout << "┌───┐" ;
       for (int i = 0 ; i < MH ; i++){
           gotoxy((FX+MW+1+6)*2,i+FY);
           cout << "│" ;
           gotoxy((FX+MW+5+6)*2,i+FY);
           cout << "│" ;
       }
       gotoxy((FX+MW+1+6)*2,FY+MH);
       cout << "└───┘" ;
       gotoxy((FX+MW+2+6)*2,FY);
       cout<<" NEXT ";
       //상대설명틀 
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
       //상대정보출력 
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
       //뿌요뿌요상대맵 
       b_o.SetXY(b_o.x+MW+13,b_o.y);
       b_o.print();
       //상대의 내려오는 뿌요 출력 
       gotoxy((FX+MW+6)*2,FY+9);
       cout << "  ";
       //클리닝 (?) 
       
       switch (effectmove){
              case 1:case 11: //직빵으로 보내는 것 
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
              case 1:case 11: //직빵으로 보내는 것 
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
       //펔킹 이펙트 
    }
}

void multicalculate(int mypos, SOCKET *sock, SOCKET *csock, SOCKET *lsock){
     int retval;
     char sendb[STRSIZE]="";
     char recvb[STRSIZE]="";
     char strtemp[STRSIZE]="";
     int bb;
     
     sprintf(strtemp, " %d", ifwin); strcat(sendb, strtemp); //졌는지 여부를 보낸다. 
     for (int i = 0 ; i < MH ; i++){
         for (int j = 0 ; j < MW ; j++){
             sprintf(strtemp, " %d", m_mine[i][j].b.color); strcat(sendb, strtemp); //맵을 보낸다. 
         }
     }
     sprintf(strtemp, " %d", combonum); strcat(sendb, strtemp);
     sprintf(strtemp, " %d", max_combo); strcat(sendb, strtemp);
     sprintf(strtemp, " %d", score); strcat(sendb, strtemp);
     sprintf(strtemp, " %d", ifeffect); strcat(sendb, strtemp); //내 상태를 보낸다. 
     
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
            MessageBox(NULL,TEXT("연결이 끊겼습니다..." ),TEXT("서버"),MB_OK);
            gameover = false;
            return;
        }
        retval = send(*csock, sendb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("연결이 끊겼습니다..." ),TEXT("서버"),MB_OK);
            gameover = false;
            return;
        }
     }
     else if (mypos == client){
        retval = send(*sock, sendb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("연결이 끊겼습니다..." ),TEXT("클라"),MB_OK);
            gameover = false;
            return;
        }
        retval = recv(*sock, recvb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("연결이 끊겼습니다..." ),TEXT("클라"),MB_OK);
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
     
     sprintf(strtemp, " %d", obstaclenum_s); strcat(sendb, strtemp); //장애물을 보낸다. 
     sprintf(strtemp, " %d", ifobsover); strcat(sendb, strtemp); //
     
     if (mypos == server){
        retval = recv(*csock, recvb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("연결이 끊겼습니다..." ),TEXT("서버"),MB_OK);
            gameover = false;
            return;
        }
        retval = send(*csock, sendb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("연결이 끊겼습니다..." ),TEXT("서버"),MB_OK);
            gameover = false;
            return;
        }
     }
     else if (mypos == client){
        retval = send(*sock, sendb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("연결이 끊겼습니다..." ),TEXT("클라"),MB_OK);
            gameover = false;
            return;
        }
        retval = recv(*sock, recvb, STRSIZE, 0);
        if (retval == SOCKET_ERROR){
            MessageBox(NULL,TEXT("연결이 끊겼습니다..." ),TEXT("클라"),MB_OK);
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
     
     obstaclenum_r+=recvdobs; //받은 건 더하고 
     obstaclenum_r-=obstaclenum_s; //보낸 건 뺀다. 
     
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
     //정보 대입 
     
     ifobsover=0;
}

void calculate(){
     obstaclenum_s=0; //보낼 것은 초기화 
     ifobs = false; //떨어트리기 초기화 
     
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
     //노래재생! 
     
     if (!ifcombo) //콤보가 안터지는 중이라면 
        scorenum = 0.0; //0 
        
     if (playmode == multi){
        if (effectmove) //0이 아니면 
           effectmove++; //가동! 
     }
     if (playmode == vscom){
        if (effectmove) //0이 아니면 
           effectmove++; //가동! 
        if (effectmove_o) //0이 아니면 
           effectmove_o++; //가동! 
     }
     //이펙트 가동 
     
     desnum = 0; //0 
     if (delay == 0){ //아직 안 부쉈을 때 
        if (check(m_mine)){ //부숴진 거 있나 체크 
           if (playmode == single)
              PlayEffectR(ID_DEST);
           combonum++; //콤보를 센다. 
           delay = 1; //터지는데 시간을 준다. 
           ifcombo=1; //콤보터지는 중 
           
           scorenum += desnum * pow(2,combonum) / 4;
           //(지금 터진 수 + 마진) * 2 ^ 콤보 수 / 4
           
           if (playmode == multi){
              ifeffect = true; //상대에게 이펙트보내기 
              
              if (combonum==1)
                 obstaclenum_s += 1 + 2 * (desnum-4);
              else
                 obstaclenum_s += (desnum-3) * combonum * 2;
                 //(지금 터진 수 - 3) * 콤보 수 * 2
                 
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
              //내 이펙트 재생 
              
              if (obstaclenum_r > 0){ //나한테 있으면 
                  effectmove = 11; 
                  if (obstaclenum_r - obstaclenum_s >= 0) //아직도 나에게 있는거면 
                     ifeffectp = 1; //그만 
                  else
                     ifeffectp = 0; //
              }
              else{ //나한테 없으면 
                  effectmove = 1;
              }
              //그림그리기 신호 
              
              ifobs = false;
           }
           if (playmode == vscom){
              ;
           }
        }
        else if (ifcombo){ //콤보가 끊기면 
           score += scorenum * 10; //점수 
           if (max_combo < combonum) //최대 콤보를 넘기면 
              max_combo = combonum; //그게 최대 콤보 
           combonum = 0;
           ifcombo = 0; //초기화 
        }
     }
     if (delay){ //시간을 주게 되어있다면 
        time_delay++; //시간을 잰다. 
     }
     if (time_delay >= 5){ //시간이 지나면 
        time_delay = 0; 
        delay = 0; //다시 터지게 시작! 
     }
     //부숴진 것을 체크하고, 콤보여부를 판단하고, 0.25초 이후에 터지도록 조작 
     
     cleaning(m_mine); //체크해제랑 부술거 부수기 
     gravity(m_mine); //중력에 의해 떨어뜨리기 
     
     if (!ifcombo && iffloor){ //바닥에 닿았고 콤보가 아니면 
        b.SetColor(nextc1,nextc2);
        b.SetDirec(Up);
        b.SetXY(SX,SY); //새로운 뿌요등장 
        nextc1 = rand()%(4*difficulty)+1;
        nextc2 = rand()%(4*difficulty)+1;
        next.SetColor(nextc1,nextc2); //다음 뿌요 결정 
        
        ifpressx = 0; //다시 x를 누를수 있게 
        iffloor = 0; //바닥에 안 닿음 
        time_gravity = 0;
        ifobs = true;
     }
     
     if (time_gravity < TG*(3-difficulty))
        time_gravity++;
     else{
        if (b.move(Down)){ //바닥에 닿으면 
           PlayEffectR(ID_DROP);
           b.tomap(); //맵으로 보낸다. 
           iffloor=1; //바닥에 닿았다. 
           b.bbuyo[1].color = empty;
           b.bbuyo[0].color = empty;
           b.SetXY(1*2,FY+MH+2);
        }
        time_gravity = 0;
     }
     gravity(m_mine); //중력에 의해 떨어뜨리기 
     
     if (m_mine[SY-1][SX].b.color != empty){ //그래도 여기가 안 비었으면 
        gameover=false; //짐ㅋ 
        ifwin = false;
     }
     
     if (time_turndel >= 3) //누른지 0.15초가 지나면 
        turndel = false; //다시 누를 수 있게 
     if (turndel) //턴을 눌러다면 
        time_turndel++; //턴타임을 늘린다 
     else time_turndel = 0; //아니면 그냥 0 
     //턴을 느리게 하는 효과 
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
     
     //현재 상황 복사 
     ;
     
     for (int i = 0 ; i < MH && !decide ; i++){
         for (int j = 0 ; j < MW && !decide ; j++){
            if (m_temp[i][j].count == yet && m_temp[i][j].b.color != empty && m_temp[i][j].b.color != obstacle){ //아직 안 셋으면 
               c = new Connectbbuyo(j, i);
               t.next = c;
               num=1;
               floodfill(m_temp, j, i, &t);
               if (num == 3){
                   
                   //장애물 아닌 것 주위 확인 
               }
               delete c;
               num = 0;
               t.next=NULL;
            }
         }
     }
     //3색 짜리가 있나본다 
     //2색 짜리가 있나본다 
     //1색 짜리가 있나본다 
     //그 주위에 빈 공간이 있나본다 
     //사방을 일일이 검사 
     //돌려놓으면 좋은지도 검사 
     //커맨드 저장 
     //입력 
     
     if (effectmove_o) //0이 아니면 
        effectmove_o++; //가동! 
     //
     
     desnum_o = 0; //0 
     if (delay_o == 0){ //아직 안 부쉈을 때 
        if (check(m_other)){ //부숴진 거 있나 체크 
           combonum_o++; //콤보를 센다. 
           delay_o = 1; //터지는데 시간을 준다. 
           ifcombo_o=1; //콤보터지는 중 
           
           scorenum += desnum * pow(2,combonum) / 4;
           //(지금 터진 수 + 마진) * 2 ^ 콤보 수 / 4
           
           ifeffect_o = true;
           
           if (combonum_o==1)
              obstaclenum_s_o += 1 + 2 * (desnum-4);
           else
              obstaclenum_s_o += (desnum_o-3) * combonum_o * 2;
              //(지금 터진 수 - 3) * 콤보 수 * 2
           
           if (obstaclenum_r < 0){
               effectmove_o = 11; 
               if (obstaclenum_r - obstaclenum_s_o >= 0)
                  ifeffectp_o = 1; //그만 
               else
                  ifeffectp_o = 0;
           }
           else{
               effectmove_o = 1;
           }
           //그림그리기 신호 
           
           ifobs_o = false;
        }
        else if (ifcombo_o){ //콤보가 끊기면 
           score_o += scorenum_o * 10; //점수 
           if (max_combo_o < combonum_o) //최대 콤보를 넘기면 
              max_combo_o = combonum_o; //그게 최대 콤보 
           combonum_o = 0;
           ifcombo_o = 0; //초기화 
        }
     }
     if (delay_o){ //시간을 주게 되어있다면 
        time_delay_o++; //시간을 잰다. 
     }
     if (time_delay_o >= 5){ //시간이 지나면 
        time_delay_o = 0; 
        delay_o = 0; //다시 터지게 시작! 
     }
     //부숴진 것을 체크하고, 콤보여부를 판단하고, 0.25초 이후에 터지도록 조작 
     
     cleaning(m_other); //체크해제랑 부술거 부수기 
     gravity(m_other); //중력에 의해 떨어뜨리기 
     
     if (!ifcombo_o && iffloor_o){ //바닥에 닿았고 콤보가 아니면 
        b_o.SetColor(nextc1_o,nextc2_o);
        b_o.SetDirec(Up);
        b_o.SetXY(SX,SY); //새로운 뿌요등장 
        nextc1_o = rand()%(4*difficulty)+1;
        nextc2_o = rand()%(4*difficulty)+1;
        next_o.SetColor(nextc1,nextc2); //다음 뿌요 결정 
        
        iffloor_o = 0; //바닥에 안 닿음 
        time_gravity_o = 0;
        ifobs_o = true;
     }
     
     if (time_gravity_o < TG*(3-difficulty))
        time_gravity_o++;
     else{
        if (b_o.move(Down)){ //바닥에 닿으면 
           PlayEffectR(ID_DROP);
           b_o.tomap(); //맵으로 보낸다. 
           iffloor=1; //바닥에 닿았다. 
           b_o.bbuyo[1].color = empty;
           b_o.bbuyo[0].color = empty;
           b_o.SetXY(1*2,FY+MH+2);
        }
        time_gravity_o = 0;
     }
     gravity(m_other); //중력에 의해 떨어뜨리기 
     
     if (m_other[SY-1][SX].b.color != empty){ //그래도 여기가 안 비었으면 
        gameover=false; //짐ㅋ 
        ifwin = true;
     }
}

bool check(Map (*m)[MW]){ //from top, left to bottom, right
    Connectbbuyo *c;
    Connectbbuyo t;

    bool ifbreak=0;
    for (int i = 0 ; i < MH ; i++){
        for (int j = 0 ; j < MW ; j++){
            if (m[i][j].count == yet && m[i][j].b.color != empty && m[i][j].b.color != obstacle){ //아직 안 셋으면 
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

int floodfill(Map (*m)[MW], int x, int y, Connectbbuyo *t){ //주위에 똑같은 게 있나? //xy는 검색당할 필드 
    bool check[4]={false,false,false,false};
    
    m[y][x].count = alc;
    
    if ( x == 0 && y == 0 ){ //위 왼 
       check[Up] = false;check[Left] = false;check[Down] = true;check[Right] = true;
    }
    else
    if ( x == 0 && y == MH-1 ){ //아래 왼 
       check[Up] = true;check[Left] = false;check[Down] = false;check[Right] = true;
    }
    else
    if ( x == MW-1 && y == MH-1 ){ //아래 오른 
       check[Up] = true;check[Left] = true;check[Down] = false;check[Right] = false;
    }
    else
    if ( x == MW-1 && y == 0 ){ //위 오른 
       check[Up] = false;check[Left] = true;check[Down] =true ;check[Right] = false;
    }
    else
    if ( y == 0 ){ //위쪽 
       check[Up] = false;check[Left] = true;check[Down] = true;check[Right] = true;
    }
    else
    if ( x == 0 ){ //왼쪽 
       check[Up] = true;check[Left] = false;check[Down] = true;check[Right] = true;
    }
    else
    if ( y == MH-1 ){ //아래쪽 
       check[Up] = true;check[Left] = true;check[Down] = false;check[Right] = true;
    }
    else
    if ( x == MW-1 ){ //오른쪽 
       check[Up] = true;check[Left] = true;check[Down] = true;check[Right] = false;
    }
    else{ //중간 
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

int cleaning(Map (*m)[MW]){ //없앨 건 없애고, 중력작용 
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
    for (int j = 0 ; j < MW ; j++){ //왼쪽부터 
        for (int i = MH-1 ; i >= 0 ; i--){ //바닥부터 올라가다가 
            for (int l = 0 ; l < MH && m[i][j].b.color == empty ; l++){ //비었으면 
                  for (int n = i ; n >= 1 ; n--){
                      m[n][j].b.color = m[n-1][j].b.color;
                  }
                  m[0][j].b.color = empty; //한칸씩 내린다. 
            } //다 비어서 계속 내리면 그만한다.
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
              //방향에 따라서 
*/

/*
    if ( x == 0 && y == 0 ){ //위 왼 
       check[Up] = false;check[Left] = false;check[Down] = true;check[Right] = true;
    }
    else
    if ( x == 0 && y == MH-1 ){ //아래 왼 
       check[Up] = true;check[Left] = false;check[Down] = false;check[Right] = true;
    }
    else
    if ( x == MW-1 && y == MH-1 ){ //아래 오른 
       check[Up] = true;check[Left] = true;check[Down] = false;check[Right] = false;
    }
    else
    if ( x == MW-1 && y == 0 ){ //위 오른 
       check[Up] = false;check[Left] = true;check[Down] =true ;check[Right] = false;
    }
    else
    if ( y == 0 ){ //위쪽 
       check[Up] = false;check[Left] = true;check[Down] = true;check[Right] = true;
    }
    else
    if ( x == 0 ){ //왼쪽 
       check[Up] = true;check[Left] = false;check[Down] = true;check[Right] = true;
    }
    else
    if ( y == MH-1 ){ //아래쪽 
       check[Up] = true;check[Left] = true;check[Down] = false;check[Right] = true;
    }
    else
    if ( x == MW-1 ){ //오른쪽 
       check[Up] = true;check[Left] = true;check[Down] = true;check[Right] = false;
    }
    else{ //중간 
       check[Up] = true;check[Left] = true;check[Down] = true;check[Right] = true;
    }
    //맵 위치에 따라서 
*/

/*
    for (int i = 0 ; i < MH ; i++){
        for (int j = 0 ; j < MW ; j++){
            //맵 전체 순회 
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
