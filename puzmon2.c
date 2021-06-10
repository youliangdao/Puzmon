/*=== puzmon0: ソースコードひな形 ===*/
/*** インクルード宣言 ***/

#include <stdio.h>

/*** 列挙型宣言 ***/

/*** グローバル定数の宣言 ***/

/*** 構造体型宣言 ***/

/*** プロトタイプ宣言 ***/
void goDungeon(char* playerName);
void doBattle(char* monster);

/*** 関数宣言 ***/

/*
ゲーム開始〜終了までの処理を行う関数
*/
int main(int argc, char** argv)
{
  if (argc == 1)
  {
    printf("エラー：プレイヤー名を指定して起動してください");
  }
  else
  {
    //タイトル表示
    printf("*** Puzzle & Monsters ***\n");
    //ダンジョンに開始〜終了
    goDungeon(argv[1]);
    //クリアー表示
    printf("*** GAME CLEARED! ***\n");
    //倒したモンスター数の表示
    printf("倒したモンスター数＝5");
  }

  return 0;
}

/*
ダンジョン開始〜終了までの処理を行う関数
*/
void goDungeon (char* playerName)
{

  printf("%sはダンジョンに到着した\n", playerName);
  doBattle("スライム");
  doBattle("ゴブリン");
  doBattle("オオコウモリ");
  doBattle("ウェアウルフ");
  doBattle("ドラゴン");
  printf("%sはダンジョンを制覇した！\n", playerName);
}

/*
1回のバトル開始〜終了までの処理を行う関数
*/
void doBattle (char* monster)
{
  printf("%sが現れた！\n", monster);
  printf("%sを倒した！\n", monster);
}

/*** ユーティリティ関数宣言 ***/
