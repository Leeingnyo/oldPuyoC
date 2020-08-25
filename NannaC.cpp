#include <windows.h>
#include <string.h>

enum {
  BLACK,      /*  0 : 까망 */
  DARK_BLUE,    /*  1 : 어두운 파랑 */
  DARK_GREEN,    /*  2 : 어두운 초록 */
  DARK_SKY_BLUE,  /*  3 : 어두운 하늘 */
  DARK_RED,    /*  4 : 어두운 빨강 */
  DARK_VOILET,  /*  5 : 어두운 보라 */
  DARK_YELLOW,  /*  6 : 어두운 노랑 */
  GRAY,      /*  7 : 회색 */
  DARK_GRAY,    /*  8 : 어두운 회색 */
  BLUE,      /*  9 : 파랑 */
  GREEN,      /* 10 : 초록 */
  SKY_BLUE,    /* 11 : 하늘 */
  RED,      /* 12 : 빨강 */
  VIOLET,      /* 13 : 보라 */
  YELLOW,      /* 14 : 노랑 */
  WHITE,      /* 15 : 하양 */
};

void SetColor(int color)
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void CursorView(char show)
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;

    SetConsoleCursorInfo(hConsole , &ConsoleCursor);
}

void gotoxy(int x, int y)
{
    COORD XY = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), XY);
}

void PlayEffectF(char *Name){
     PlaySound(TEXT(Name), NULL,SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
}

void PlayEffectR(int ID){
     PlaySound(MAKEINTRESOURCE(ID), NULL,SND_RESOURCE | SND_ASYNC | SND_NODEFAULT);
}

void StopEffect(){
     PlaySound(NULL,0,0);
}

void trim(char *str, int num, int len){
    if (strlen(str) <= num){
       str[0]='\0';
       return;
    }
    for (int i=0;i<num;i++)
        for (int j=0;j<len && str[j]!='\0';j++)
            str[j]=str[j+1];
}
