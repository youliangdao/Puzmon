/*=== puzmon0: ソースコードひな形 ===*/
/*** インクルード宣言 ***/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


/*** 列挙型宣言 ***/
//属性を0~5まで割り当てる
typedef enum {FIRE, WATER, WIND, EARTH, LIFE, EMPTY} Element;

/*** グローバル定数の宣言 ***/
//属性別の記号の割り当て
const char ELEMENT_SYMBOLS[EMPTY+1] = {'$','~','@','#','&', ' '};

//属性別のカラーコード（ディスプレイ制御シーケンス用）
const char ELEMENT_COLORS[EMPTY+1] = {1,6,2,3,5,0};

//バトルフィールドに発生させる宝石の個数
enum {MAX_GEMS = 14};

//2つの属性を添字に指定すると、ダメージ増幅率を取り出せるdouble２次元配列
double ELEMENT_BOOST[4][4] = {{1.0, 0.5, 2.0, 1.0}, {2.0, 1.0, 1.0, 0.5}, {0.5, 1.0, 1.0, 2.0}, {1.0, 2.0, 0.5, 1.0}};


/*** 構造体型宣言 ***/
//各モンスターのステータスを格納するMonster型の設定
typedef struct MONSTER {
  char* name;
  Element element;
  int maxHp;
  int hp;
  int attack;
  int defense;
} Monster;

//各ダンジョン別のモンスターの種類と数を格納するDungeon型の設定
typedef struct DUNGEON {
  Monster* monsters;
  const int numMonsters;
} Dungeon;

//パーティ関連情報をまとめて管理する構造体Party型の設定
typedef struct PARTY
{
  Monster* monsters;
  const int numPartyMonsters;
  char* player;
  int hp;
  int averageDefense;
} Party;

//バトルの場に登場する「パーティー」「敵モンスター」「宝石スロット」をまとめて管理する構造体BattleFiled型の設定
typedef struct BATTLEFIELD
{
  Party* pBattleParty;
  Monster* pBattleMonster;
  Element gems[MAX_GEMS];
} Battle_Field;

/*消去可能な宝石の並びに関する情報（属性・開始位置・連続数）
をまとめて管理するための構造体Banishinfo型の設定*/
typedef struct BANISHINFO
{
  Element type;
  int position;
  int contNum;
} BanishInfo;


/*** プロトタイプ宣言 ***/
int goDungeon(Dungeon* pDungenon, Party* pParty);
int doBattle(Monster* pMonster, Party* pParty);
Party organizeParty(Monster* monsters, int monsterNum, char* playerName);
void showParty(Party* pParty);
void onPlayerTurn(Battle_Field* pBattleField);
void doAttack(Battle_Field* pBattleField, BanishInfo* pBanishInfo, int combo);
void onEnemyTurn(Monster* pMonster, Party* pParty);
void doEnemyAttack(Monster* pMonster, Party* pParty);
void showBattleField(Battle_Field* pBattleFiled);
bool checkValidCommand(char* commands);
void evaluateGems(Battle_Field* pBattleField);
BanishInfo checkBanishable(Element* elements);
void banishGems(Battle_Field* pBattleField, BanishInfo* pBanishInfo, int combo);
void shiftGems(BanishInfo* pBanishInfo, Element* elements);
void spawnGems(Battle_Field* pBattleField, int combo);
void doRecover(Battle_Field* pBattleField, BanishInfo* pBanishInfo, int combo);
void printCombo(int combo);

//ユーティリティ関数
void printMonsterName(Monster* pMonster);
void fillGems(Element* elements, bool emptyOnly);
void printGems(Element* elements);
void printGem(char gem);
void moveGem(int start, int end, Element* elements, bool printProcess);
void swapGem(int std, Element* elements, bool dir);
int calcRecoverDamage(BanishInfo* pBanishInfo);
int blurDamage(double damage);
int calcAttackDamage(Battle_Field* pBattleField, Monster* pAttackMonster, BanishInfo* pBanishInfo);
int calcEnemyDamage(Monster* pMonster, Party* pParty);

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

  //ダンジョン内の敵モンスターの情報を格納するMonster[5]型配列の設定
  Monster dungeonMonsters [] = {
    {"スライム", WATER, 100, 100, 10, 5},
    {"ゴブリン", EARTH, 200, 200, 20, 15},
    {"オオコウモリ", WIND, 300, 300, 30, 25},
    {"ウェアウルフ", WIND, 400, 400, 40, 30},
    {"ドラゴン", FIRE, 800, 800, 50, 40},
  };

  //味方モンスターの情報を格納するMonster[4]型配列の設定
  Monster supportMonsters [] = {
    {"朱雀", FIRE, 150, 150, 25, 10},
    {"青龍", WIND, 150, 150, 15, 10},
    {"白虎", EARTH, 150, 150, 20, 5},
    {"玄武", WATER, 150, 150, 20, 15}
  };

  //Dungeon型の変数の設定
  Dungeon dungeon = {dungeonMonsters, 5};

  //タイトル表示
  printf("*** Puzzle & Monsters ***\n");

  //パーティを編成する
  Party party = organizeParty(supportMonsters, 4, argv[1]);

  //ダンジョン開始
  int winCount = goDungeon(&dungeon, &party);

  //ダンジョン終了後
  if (winCount == 5){
    printf("%sはダンジョンを制覇した！\n", argv[1]);
    printf("***GAME CLEARED!***\n");
  } else {
    printf("***GAME OVER***\n");
  }
  printf("倒したモンスター数＝%d", winCount);
  return 0;
}

//ダンジョン開始〜終了まで
int goDungeon(Dungeon* pDungeon, Party* pParty){
  printf("%sのパーティー（HP=%d）はダンジョンに到着した\n\n", pParty->player, pParty->hp);
  showParty(pParty);

  int winCount = 0;
  for (int i = 0; i < pDungeon->numMonsters; i++)
  {
    printMonsterName(&(pDungeon->monsters[i]));
    printf("が現れた！\n\n");
    while (pDungeon->monsters[i].hp > 0)
    {
      winCount += doBattle(&(pDungeon->monsters[i]), pParty);
      if (pParty->hp <= 0)
      {
        printf("%sはダンジョンから逃げ出した…\n", pParty->player);
        break;
      }
    }
  }
  return winCount;
}

//敵とのバトル
int doBattle(Monster* pMonster, Party* pParty){

  //バトルフィールドの宝石スロットの準備と初期化
  Battle_Field battleField = {pParty, pMonster};
  fillGems(battleField.gems, false);

  //交互ターンの実現
  while (true)
  {
    //味方ターン
    onPlayerTurn(&battleField);
    if (pMonster->hp <= 0)
    {
      printMonsterName(pMonster);
      printf("を倒した！\n");
      printf("%sはさらに奥へと進んだ\n\n", pParty->player);
      printf("=====\n\n");
      return 1;
    }

    //敵ターン
    onEnemyTurn(pMonster, pParty);
    if (pParty->hp <= 0)
    {
      printf("%sは倒れた…\n", pParty->player);
      return 0;
    }
  }
}

//パーティ編成を行う
Party organizeParty(Monster* monsters, int monsterNum, char* playerName){
  int sumHp = 0;
  int sumDefense = 0;
  for (int i = 0; i < monsterNum; i++)
  {
    sumHp += monsters[i].hp;
    sumDefense += monsters[i].defense;
  }
  int avgDefense = sumDefense / monsterNum;

  Party p ={monsters, monsterNum, playerName, sumHp, avgDefense};
  return p;
}

//パーティ編成情報を一覧表示
void showParty(Party* pParty){
  printf("<パーティ編成情報>--------\n");
  for (int i = 0; i < pParty->numPartyMonsters; i++)
  {
    printMonsterName(&(pParty->monsters[i]));
    printf(" HP= %d 攻撃= %d 防御= %d\n", pParty->monsters[i].hp, pParty->monsters[i].attack, pParty->monsters[i].defense); }
    printf("--------------------------\n\n");
}

//味方ターンの処理を行う関数
void onPlayerTurn(Battle_Field* pBattleFiled){
  printf("【%sのターン】\n", pBattleFiled->pBattleParty->player);
  showBattleField(pBattleFiled);

  char command[3];
  bool check;
  do {
    //プレイヤーにコマンドを入力させる
    printf("コマンド？>");
    scanf("%2s", command);
    //適切なコマンドかどうかをチェックする
  } while (checkValidCommand(command) == false);

  //宝石を移動させる
  moveGem(command[0] - 'A', command[1] - 'A', pBattleFiled->gems, true);

  //宝石スロットを評価する
  evaluateGems(pBattleFiled);
}

//敵に攻撃を加える関数
void doAttack(Battle_Field* pBattleField, BanishInfo* pBanishInfo, int combo){

  Monster* attackMonster;
  switch (pBanishInfo->type)
  {
  case FIRE:
    attackMonster = &pBattleField->pBattleParty->monsters[0];
    break;
  case WATER:
    attackMonster = &pBattleField->pBattleParty->monsters[3];
    break;
  case WIND:
    attackMonster = &pBattleField->pBattleParty->monsters[1];
    break;
  case EARTH:
    attackMonster = &pBattleField->pBattleParty->monsters[2];
    break;
  default:
    break;
  }

  int partyDamage = calcAttackDamage(pBattleField, attackMonster, pBanishInfo);
  if (combo >= 2)
  {
    printf("%sの攻撃！", attackMonster->name);
    printCombo(combo);
  } else
  {
    printf("%sの攻撃！\n", attackMonster->name);
  }
  printf("%sに%dのダメージ！\n", pBattleField->pBattleMonster->name, partyDamage);
  pBattleField->pBattleMonster->hp -= partyDamage;
}

//敵のターン
void onEnemyTurn(Monster* pMonster, Party* pParty){
  printf("\n【%sのターン】\n", pMonster->name);
  doEnemyAttack(pMonster, pParty);
}

//敵モンスターの攻撃
void doEnemyAttack(Monster* pMonster, Party* pParty){
  int enemyAttack = calcEnemyDamage(pMonster, pParty);
  printMonsterName(pMonster);
  printf("の攻撃！%dのダメージを受けた\n\n\n", enemyAttack);
  pParty->hp -= enemyAttack;
}

//バトルフィールド情報を表示
void showBattleField(Battle_Field* pBattleField){
  printf("---------------------------------------\n\n");
  printf("       ");
  printMonsterName(pBattleField->pBattleMonster);
  printf("\n       HP= %4d / %4d\n", pBattleField->pBattleMonster->hp, pBattleField->pBattleMonster->maxHp);
  printf("\n\n");
  int sumHp = 0;
  for (int i = 0; i < pBattleField->pBattleParty->numPartyMonsters; i++)
  {
    int d = pBattleField->pBattleParty->monsters[i].hp;
    sumHp += d;
    printMonsterName(&(pBattleField->pBattleParty->monsters[i]));
    printf("  ");
  }
  printf("\n       HP= %4d / %4d\n\n", pBattleField->pBattleParty->hp, sumHp);
  printf("---------------------------------------\n");
  for (int i = 0; i < MAX_GEMS; i++)
  {
    printf(" ");
    printf("%c", (65 + i));
  }
  printf("\n");

  printGems(pBattleField->gems);

  printf("---------------------------------------\n\n");
}

//適切なコマンドかどうかをチェックする関数
bool checkValidCommand(char* commands){

  //コマンドの長さは必ず2であるべき
  if (strlen(commands) != 2) return false;
  //１文字目と２文字目が同じであれば不正
  if (commands[0] == commands[1]) return false;
  //１文字目がA~Nの範囲でなければ不正
  if (commands[0] < 'A' || commands[0] > 'A' + MAX_GEMS - 1) return false;
  //２文字目もA〜Nの範囲でなければ不正
  if (commands[1] < 'A' || commands[1] > 'A' + MAX_GEMS - 1) return false;
  //それ以外は有効
  return true;
}

//宝石スロットを評価解決する
void evaluateGems(Battle_Field* pBattleField){
  BanishInfo banishInfo = checkBanishable(pBattleField->gems);
  int comboCount = 0;
  //消去可能箇所がある場合
  while (banishInfo.contNum != 0)
  {
    comboCount++;
    banishGems(pBattleField, &banishInfo, comboCount);
    shiftGems(&banishInfo, pBattleField->gems);
    banishInfo = checkBanishable(pBattleField->gems);
  }
  spawnGems(pBattleField, comboCount);
}

//宝石の消滅可能箇所判定
BanishInfo checkBanishable(Element* elements){
  const int BANISH_GEMS = 3; ////消滅に必要な連続数

  for (int i = 0; i < MAX_GEMS - BANISH_GEMS + 1; i++)
  {
    Element targetGem = elements[i];
    int count = 1;
    if (targetGem == EMPTY) continue;
    for (int j = i + 1; i < MAX_GEMS; j++)
    {
      if (elements[i] == elements[j])
      {
        count++;
      }
      else
      {
        break;
      }
    }

    if (count >= BANISH_GEMS)
    {
      BanishInfo found = {targetGem, i, count};
      return found;
    }
  }

  //見つからなかった
  BanishInfo notFound = {EMPTY, 0, 0};
  return notFound;
}

//指定箇所の宝石を消滅させ効果発動
void banishGems(Battle_Field* pBattleField, BanishInfo* pBanishInfo, int combo)
{
  //宝石の消滅→空きスロットの表示
  for (int i = 0; i < pBanishInfo->contNum; i++)
  {
    pBattleField->gems[pBanishInfo->position + i] = EMPTY;
  }
  printGems(pBattleField->gems);

  //効果発動→パーティ攻撃か回復か？
  if (pBanishInfo->type == LIFE)
  {
    doRecover(pBattleField, pBanishInfo, combo);
  }
  else
  {
    doAttack(pBattleField, pBanishInfo, combo);
  }

  //パーティ攻撃後の宝石スロットの表示
  printGems(pBattleField->gems);
}

//空いている部分を左詰していく
void shiftGems(BanishInfo* pBanishInfo, Element* elements){
  int endEmpPos = pBanishInfo->position + pBanishInfo->contNum -1;

  for (int i = 0; i < pBanishInfo->contNum; i++)
  {
    moveGem(endEmpPos - i, 13 - i, elements, false);
    printGems(elements);
  }
}

//空き領域に宝石が沸く
void spawnGems(Battle_Field* pBattleField, int combo){
  fillGems(pBattleField->gems, true);
  printGems(pBattleField->gems);

  //ランダム発生した宝石が消滅の可能性があるかどうか吟味
  BanishInfo banishInfo = checkBanishable(pBattleField->gems);
  if (banishInfo.contNum != 0)
  {
    combo++;
    banishGems(pBattleField, &banishInfo, combo);
    shiftGems(&banishInfo, pBattleField->gems);
    banishInfo = checkBanishable(pBattleField->gems);
    spawnGems(pBattleField, combo);
  }
}

//宝石の消滅による回復
void doRecover(Battle_Field* pBattleField, BanishInfo* pBanishInfo, int combo){
  if (combo >= 2)
  {
    printf("%sは命の宝石を使った！", pBattleField->pBattleParty->player);
    printCombo(combo);
  } else
  {
    printf("%sは命の宝石を使った！\n", pBattleField->pBattleParty->player);
  }


  pBattleField->pBattleParty->hp += calcRecoverDamage(pBanishInfo);
  printf("HPが%d回復した\n", calcRecoverDamage(pBanishInfo));
}

//コンボ発生時にその旨を表示する関数
void printCombo(int combo){
  printf(" \x1b[47m\x1b[30m%d COMBO!\x1b[39m\x1b[49m\n", combo);
}
/*** ユーティリティ関数宣言 ***/

//モンスターの名前に適切な記号と属性の色を付与し、画面に表示する関数
void printMonsterName(Monster* pMonster){
  char symbol = ELEMENT_SYMBOLS[pMonster->element];
  int color = ELEMENT_COLORS[pMonster->element];

  printf("\x1b[3%dm%c%s%c\x1b[0m", color, symbol, pMonster->name, symbol);
}

//バトルフィールドの宝石スロットにランダムに宝石を発生させる関数
void fillGems(Element* elements, bool emptyOnly){
  for (int i = 0; i < MAX_GEMS; i++)
  {
    if (!emptyOnly || elements[i] == EMPTY)
    {
      elements[i] = rand() % (EMPTY);
    }
  }
}

//バトルフィールドの宝石スロットを画面に表示
void printGems(Element* elements){
  for (int i = 0; i < MAX_GEMS; i++)
  {
    printf(" ");
    printGem(elements[i]);
  }
  printf("\n");
}

//1個の宝石の表示
void printGem(char gem){
  char symbol = ELEMENT_SYMBOLS[gem];
  int color = ELEMENT_COLORS[gem];
  printf("\x1b[4%dm\x1b[30m%c\x1b[39m\x1b[49m", color, symbol);
}

//指定位置の宝石を別の指定位置に移動させる関数
void moveGem(int start, int end, Element* elements, bool printProcess){
  if (end > start)
  {
    for (int i = 0; i < (end - start); i++)
    {
      swapGem(start + i, elements, true);
      if (printProcess) printGems(elements);
    }

  } else {
    for (int i = 0; i < (start - end); i++)
    {
      swapGem(start - i, elements, false);
      if (printProcess) printGems(elements);
    }
  }
}

//指定位置の宝石を指定した方向の隣の宝石と入れ替える関数
void swapGem(int std, Element* elements, bool dir){
  char rep = elements[std];
  if (dir)
  {
    elements[std] = elements[std + 1];
    elements[std+ 1] = rep;
  }
  else
  {
    elements[std] = elements[std - 1];
    elements[std - 1] = rep;
  }
}

//パーティーのHP回復量を算出する関数
int calcRecoverDamage(BanishInfo* pBanishInfo){
  double recover = 20 * (pow(1.5, pBanishInfo->contNum -3));
  int recoverDamage = blurDamage(recover);

  return recoverDamage;
}

//指定値を中心に指定の範囲で数値をランダムにぶれさせる関数
int blurDamage(double damage){
  srand((unsigned)time(0UL));
  int r = rand () % 21 + 90;
  damage = damage * r / 100;
  return damage;
}

//パーティーによる攻撃ダメージを算出する関数
int calcAttackDamage(Battle_Field* pBattleField, Monster* pAttackMonster, BanishInfo* pBanishInfo){
  double element_Boost = ELEMENT_BOOST[pBanishInfo->type][pAttackMonster->element];
  double attackDamage = (pAttackMonster->attack - pBattleField->pBattleMonster->defense)
   * element_Boost *(pow(1.5, pBanishInfo->contNum -3));
  int a = blurDamage(attackDamage);
  if (a <= 0)
  {
    return 1;
  }
  else
  {
    return a;
  }
}

//敵モンスターによる攻撃ダメージを算出する関数
int calcEnemyDamage(Monster* pMonster, Party* pParty){
  double enemyDamage = pMonster->attack - pParty->averageDefense;
  int e = blurDamage(enemyDamage);
    if (e <= 0)
  {
    return 1;
  }
  else
  {
    return e;
  }
}