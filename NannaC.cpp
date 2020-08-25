#include <windows.h>
#include <string.h>

enum {
  BLACK,      /*  0 : ��� */
  DARK_BLUE,    /*  1 : ��ο� �Ķ� */
  DARK_GREEN,    /*  2 : ��ο� �ʷ� */
  DARK_SKY_BLUE,  /*  3 : ��ο� �ϴ� */
  DARK_RED,    /*  4 : ��ο� ���� */
  DARK_VOILET,  /*  5 : ��ο� ���� */
  DARK_YELLOW,  /*  6 : ��ο� ��� */
  GRAY,      /*  7 : ȸ�� */
  DARK_GRAY,    /*  8 : ��ο� ȸ�� */
  BLUE,      /*  9 : �Ķ� */
  GREEN,      /* 10 : �ʷ� */
  SKY_BLUE,    /* 11 : �ϴ� */
  RED,      /* 12 : ���� */
  VIOLET,      /* 13 : ���� */
  YELLOW,      /* 14 : ��� */
  WHITE,      /* 15 : �Ͼ� */
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
