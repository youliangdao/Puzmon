/*=== puzmon0: ソースコードひな形 ===*/
/*** インクルード宣言 ***/

#include <stdio.h>

/*** 列挙型宣言 ***/
//属性を0~5まで割り当てる
typedef enum {FIRE, WATER, WIND, EARTH, LIFE, EMPTY} Element;

/*** グローバル定数の宣言 ***/
//属性別の記号の割り当て
const char ELEMENT_SYMBOLS[EMPTY+1] = {'$','~','@','#','&', ' '};

//属性別のカラーコード（ディスプレイ制御シーケンス用）
const char ELEMENT_COLORS[EMPTY+1] = {1,6,2,3,5,0};

/*** 構造体型宣言 ***/
//各モンスターのステータスを格納するMonster型の設定
typedef struct MONSTER {
  char* name;
  Element element;
  int maxhp;
  int hp;
  int attack;
  int defense;
} Monster;

//各ダンジョン別のモンスターの種類と数を格納するDungeon型の設定
typedef struct DUNGEON {
  Monster* monsters;
  const int numMonsters;
} Dungeon;

/*** プロトタイプ宣言 ***/
int goDungeon(char* playerName, Dungeon* dungeonPtr);
int doBattle(Monster* enemyPtr);
void printMonsterName(Monster* monsterPtr);
/*** 関数宣言 ***/

//ゲーム開始〜終了までの流れ
int main(int argc, char** argv)
{
  //プレイヤー名指定無しの場合のエラー表示
  if (argc != 2)
  {
    printf("エラー：プレイヤー名を指定して起動してください");
    return 1;
  }

  //Monster型の配列の設定
  Monster dungeonMonsters [] = {
    {"スライム", WATER, 100, 100, 10, 5},
    {"ゴブリン", EARTH, 200, 200, 20, 15},
    {"オオコウモリ", WIND, 300, 300, 30, 25},
    {"ウェアウルフ", WIND, 400, 400, 40, 30},
    {"ドラゴン", FIRE, 800, 800, 50, 40},
  };

  //Dungeon型の変数の設定
  Dungeon dungeon = {dungeonMonsters, 5};

  //タイトル表示
  printf("*** Puzzle & Monsters ***\n");

  //ダンジョン開始
  int winCount = goDungeon(argv[1], &dungeon);

  //ダンジョン終了後
  if (winCount == 5){
    printf("***GAME CLEARED!***\n");
  } else {
    printf("***GAME OVER***\n");
  }
  printf("倒したモンスター数＝%d", winCount);
  return 0;
}

//ダンジョン開始〜終了まで
int goDungeon(char* playerName, Dungeon* dungeonPtr){
  printf("%sはダンジョンに到着した\n", playerName);

  int winCount = 0;
  for (int i = 0; i < dungeonPtr->numMonsters; i++)
  {
    winCount += doBattle(&(dungeonPtr->monsters[i]));
  }
  printf("%sはダンジョンを制覇した！\n", playerName);
  return winCount;
}

//敵とのバトル
int doBattle(Monster* enemyPtr){
  printMonsterName(enemyPtr);
  return 1;
}

/*** ユーティリティ関数宣言 ***/
//モンスターの名前に適切な記号と属性の色を付与し、画面に表示する関数
void printMonsterName(Monster* monsterPtr){
  char symbol = ELEMENT_SYMBOLS[monsterPtr->element];
  int color = ELEMENT_COLORS[monsterPtr->element];

  printf("\x1b[3%dm%c%s%c\x1b[0mが現れた！\n", color, symbol, monsterPtr->name, symbol);
  printf("\x1b[3%dm%c%s%c\x1b[0mを倒した！\n", color, symbol, monsterPtr->name, symbol);
}