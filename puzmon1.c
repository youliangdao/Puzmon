/*=== puzmon0: ソースコードひな形 ===*/
/*** インクルード宣言 ***/

#include <stdio.h>

/*** 列挙型宣言 ***/

/*** グローバル定数の宣言 ***/

/*** 構造体型宣言 ***/

/*** プロトタイプ宣言 ***/
int goDungeon(char* playerName);
int doBattle(char* enemy);

/*** 関数宣言 ***/

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    printf("エラー：プレイヤー名を指定して起動してください");
    return 1;
  }

  printf("*** Puzzle & Monsters ***\n");
  int winCount = goDungeon(argv[1]);
  if (winCount == 5){
    printf("***GAME CLEARED!***\n");
  } else {
    printf("***GAME OVER***\n");
  }
  printf("倒したモンスター数＝%d", winCount);
  return 0;
}

int goDungeon(char* playerName){
  printf("%sはダンジョンに到着した\n", playerName);

  int winCount = 0;
  winCount += doBattle("スライム");
  winCount += doBattle("ゴブリン");
  winCount += doBattle("オオコウモリ");
  winCount += doBattle("ウェアウルフ");
  winCount += doBattle("ドラゴン");
  printf("%sはダンジョンを制覇した！\n", playerName);
  return winCount;
}

int doBattle(char* enemy){
  printf("%sが現れた！\n", enemy);
  printf("%sを倒した！\n", enemy);
  return 1;
}

/*** ユーティリティ関数宣言 ***/
