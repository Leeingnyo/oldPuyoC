#include <windows.h>
#include <string.h>

enum {
  BLACK,      /*  0 : ��� */
  DARK_BLUE,    /*  1 : ��ο� �Ķ� */
  DARK_GREEN,    /*  2 : ��ο� �ʷ� */
  DARK_SKY_BLUE,  /*  3 : ��ο� �ϴ� */
  DARK_RED,    /*  4 : ��ο� ���� */
  DARK_VIOLET,  /*  5 : ��ο� ���� */
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

void SetColor(int color);

void CursorView(char show);

void gotoxy(int x, int y);

void PlayEffectF(char *Name);
void PlayEffectR(int ID);
void StopEffect();

void trim(char *str, int num, int len);
