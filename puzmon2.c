/*=== puzmon0: ソースコードひな形 ===*/
/*** インクルード宣言 ***/

#include <stdio.h>

/*** 列挙型宣言 ***/
typedef enum {
  FIRE,
  WATER,
  WIND,
  EARTH,
  LIFE,
  EMPTY
} Element;

/*** グローバル定数の宣言 ***/
char ELEMENT_SYMBOLS[6] = {'$', '~', '@', '#', '&', ' '};
int ELEMENT_COLORS[5] = {1, 6, 2, 3, 5, 0};

/*** 構造体型宣言 ***/
typedef char str[20];
//1体のモンスター情報をまとめて管理するためのMonster型の定義
typedef struct
{
  str name;
  int hp;
  int maxHp;
  Element element;
  int attack;
  int defense;
} Monster;

//1つのダンジョン情報をまとめて管理するためのDungeon型の定義
typedef struct
{
  Monster monster1;
  Monster monster2;
  Monster monster3;
  Monster monster4;
  Monster monster5;
} Dungeon;

/*** プロトタイプ宣言 ***/
void goDungeon(
  char* playerName,
  str enemy1,
  int elementNumber1,
  str enemy2,
  int elementNumber2,
  str enemy3,
  int elementNumber3,
  str enemy4,
  int elementNumber4,
  str enemy5,
  int elementNumber5
);
void doBattle(str enemy, int elementNumber);
void printMonsterName(str monster, int elementNuber);
/*** 関数宣言 ***/

/*
ゲーム開始〜終了までの処理を行う関数
*/
int main(int argc, char** argv)
{
  if (argc == 1)
  {
    printf("エラー：プレイヤー名を指定して起動してください");
    return 1;
  }

    //敵モンスターの定義
    Monster slime = {"スライム", 100, 100, WATER, 10, 5};
    Monster goblin = {"ゴブリン", 200, 200, EARTH, 20, 15};
    Monster bat = {"オオコウモリ", 300, 300, WIND, 30, 25};
    Monster wolf = {"ウェアウルフ", 400, 400, WIND, 40, 30};
    Monster dragon = {"ドラゴン", 800, 800, FIRE, 50, 40};

    //ダンジョン内の敵モンスターの定義
    Dungeon dungeon = {slime, goblin, bat, wolf, dragon};

    //タイトル表示
    printf("*** Puzzle & Monsters ***\n");
    //ダンジョンに開始〜終了
    goDungeon(
      argv[1],
      dungeon.monster1.name,
      dungeon.monster1.element,
      dungeon.monster2.name,
      dungeon.monster2.element,
      dungeon.monster3.name,
      dungeon.monster3.element,
      dungeon.monster4.name,
      dungeon.monster4.element,
      dungeon.monster5.name,
      dungeon.monster5.element
    );
    //クリアー表示
    printf("*** GAME CLEARED! ***\n");
    //倒したモンスター数の表示
    printf("倒したモンスター数＝5");

  return 0;
}

/*
ダンジョン開始〜終了までの処理を行う関数
*/
void goDungeon (
  char* playerName,
  str enemy1,
  int elementNumber1,
  str enemy2,
  int elementNumber2,
  str enemy3,
  int elementNumber3,
  str enemy4,
  int elementNumber4,
  str enemy5,
  int elementNumber5
  )
{

  printf("%sはダンジョンに到着した\n", playerName);
  doBattle(enemy1, elementNumber1);
  doBattle(enemy2, elementNumber2);
  doBattle(enemy3, elementNumber3);
  doBattle(enemy4, elementNumber4);
  doBattle(enemy5, elementNumber5);
  printf("%sはダンジョンを制覇した！\n", playerName);
}

/*
1回のバトル開始〜終了までの処理を行う関数
*/
void doBattle (str enemy, int elementNumber)
{
  printMonsterName(enemy, elementNumber);
}

/*** ユーティリティ関数宣言 ***/

/*
モンスター1体の情報を渡され、その名前の前後に記号を付け、適切な属性の色で
画面に表示する関数
*/
void printMonsterName(str monster, int elementNumber)
{
  //「モンスターが現れた」表示
  printf("\x1b[3%dm", ELEMENT_COLORS[elementNumber]);
  printf("%c", ELEMENT_SYMBOLS[elementNumber]);
  printf("%s", monster);
  printf("%c", ELEMENT_SYMBOLS[elementNumber]);
  printf("\x1b[0m");
  printf("が現れた！\n");

  //「モンスターを倒した」表示
  printf("\x1b[3%dm", ELEMENT_COLORS[elementNumber]);
  printf("%c", ELEMENT_SYMBOLS[elementNumber]);
  printf("%s", monster);
  printf("%c", ELEMENT_SYMBOLS[elementNumber]);
  printf("\x1b[0m");
  printf("を倒した！\n");
}
