#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#include <cstdlib>

#define ENEMY 5
#define MAXHP 20
#define GU 1
#define CHOKI 2
#define PA 3


typedef struct {
	int number = 0;
	char name[64] = "";
	int hp = 0;
	int atk = 0;
	int deathFlag = 0;
} enemyStatus;

typedef struct {
	char name[64] = "";
	int hp = 0;
	int walkcount = 0;
	int cardflaggu = 0;
	int cardflagchoki = 0;
	int cardflagpa = 0;
	char cardnamegu[1024] = "なし";
	char cardnamechoki[1024] = "なし";
	char cardnamepa[1024] = "なし";
	char gueffect[1024] = "なし";
	char chokieffect[1024] = "なし";
	char paeffect[1024] = "なし";
	int atkgu = 0;
	int atkchoki = 0;
	int atkpa = 0;
} myStatus;

typedef struct {
	int playerflag = 0;
	int playergone = 0;
	int enemyflag = 0; //0だったらいない、1～5で敵の種類を判別
	int eventflag = 0;
} roomStatus;

typedef struct {
	char name[1024];
	char effect[1024];
	int atk;
	int flag;
} cardStatus;

typedef struct {
	int treasure;
} treasureStatus;

void charaInit();
void playerInit();
void shufful(int[25], int);
void dungeonInit();
void cardInit();
void showTitle();
void showRule();
void showDungeon();
void movePlayer();
void showRoom();
void showStatus();
int battle(myStatus*, enemyStatus*);
void treasure(int, int);
void spring(int, int);
void checkCard(int);
void drawEnemy(int);
void enemyText(int, int);
void resultDisplay(int);
void drawEvent(int);

cardStatus card[9];
cardStatus* c[9];
myStatus status;
myStatus* p;
enemyStatus enemy[ENEMY];
enemyStatus* e[ENEMY];
roomStatus room[5][5];
roomStatus* r[5][5];

int gameFlag = 0; //ゲームの状態を保持する変数(0ならゲーム続行　1ならゲームオーバー　2ならゲームクリア)
int win = 0;	  //じゃんけんの勝敗結果を格納する変数
int lose = 0;
int draw = 0;

int main(void) {
	p = &status;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			r[i][j] = &room[i][j];
		}
	}

	for (int i = 0; i < ENEMY; i++) {
		e[i] = &enemy[i];
	}
	for (int i = 0; i < 9; i++) {
		c[i] = &card[i];
	}

	while (1) {
		int gamemode = 0;
		gameFlag = 0;
		win = 0;
		lose = 0;
		draw = 0;

		showTitle();
		gamemode = getchar();
		while (getchar() != '\n');
		switch (gamemode) {
		case '1':

			//設定の初期化
			srand((unsigned int)time(NULL));
			dungeonInit();
			charaInit();
			playerInit();
			cardInit();

			system("cls");

			//ゲームの本部分
			while (gameFlag == 0) {
				showRoom();
				//
				if (gameFlag != 0) {
					break;
				}
				showDungeon();
				movePlayer();
				system("cls");
			}
			if (gameFlag == 1) {
				resultDisplay(1);
			}
			else if (gameFlag == 2) {
				resultDisplay(2);
			}
			break;
		case '2':
			//ルールの表示
			showRule();
			system("cls");
			break;
		case '3':
			//ゲーム終了
			return 0;
		default:
			//エラー処理
			printf("\t\t                             もう一度入力してください\n");
			break;
		}
	}

}


//C0117101木村勇大
//ダンジョンの中身をシャッフルする関数
void shufful(int ary[25], int size) {
	unsigned int Time;
	Time = time(NULL);
	srand(Time);

	for (int i = 0; i < 25; i++) {
		int j = rand() % 25;

		int t = ary[i];
		ary[i] = ary[j];
		ary[j] = t;
	}
}

//C0117101木村勇大
//ダンジョンの初期化を行う関数
void dungeonInit() {
	int map_rand[25] = { 0 };
	int map[5][5] = { 0 };

	//room変数の初期化
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			room[i][j].playerflag = 0;
			room[i][j].enemyflag = 0;
			room[i][j].eventflag = 0;
			room[i][j].playergone = 0;
		}
	}

	//map_rand変数に0～24までの数を格納
	for (int i = 0; i < 25; i++) {
		map_rand[i] = i;
	}

	//map_randの中身をシャッフル
	shufful(map_rand, 25);

	int count = 0;

	//map変数にmap_randの中身をコピー
	for (int m = 0; m < 5; m++) {
		for (int n = 0; n < 5; n++) {
			map[m][n] = map_rand[count];
			count++;
		}
	}

	//map変数の中身に応じてプレイヤーの位置等を決める
	for (int map_count1 = 0; map_count1 < 5; map_count1++) {
		for (int map_count2 = 0; map_count2 < 5; map_count2++) {

			//map変数の中身が0の場所にプレイヤーを配置
			if (map[map_count1][map_count2] == 0) {
				r[map_count1][map_count2]->playerflag = 1;
				r[map_count1][map_count2]->playergone = 1;
			}
			else {
				//map変数1～5の場所に敵を配置
				if (map[map_count1][map_count2] == 1) {
					r[map_count1][map_count2]->enemyflag = 1;
				}

				if (map[map_count1][map_count2] == 2) {
					r[map_count1][map_count2]->enemyflag = 2;
				}

				if (map[map_count1][map_count2] == 3) {
					r[map_count1][map_count2]->enemyflag = 3;
				}

				if (map[map_count1][map_count2] == 4) {
					r[map_count1][map_count2]->enemyflag = 4;
				}

				//map変数5～7の場所に宝箱を配置
				if (map[map_count1][map_count2] == 5) {
					r[map_count1][map_count2]->eventflag = 1;
				}

				if (map[map_count1][map_count2] == 6) {
					r[map_count1][map_count2]->eventflag = 1;
				}

				if (map[map_count1][map_count2] == 7) {
					r[map_count1][map_count2]->eventflag = 1;
				}

				//map変数8,9の場所に泉を配置
				if (map[map_count1][map_count2] == 8) {
					r[map_count1][map_count2]->eventflag = 2;
				}

				if (map[map_count1][map_count2] == 9) {
					r[map_count1][map_count2]->eventflag = 2;

				}
				//プレイヤー以外の場所は非表示にするためにplayergoneを0にする
				r[map_count1][map_count2]->playergone = 0;
			}
		}
	}
}

//C0117101木村勇大
void charaInit() {
	enemy[0] = { 0, "こうかとん",5,1,0 };
	enemy[1] = { 1, "ぱっちぃ",3,2,0 };
	enemy[2] = { 2, "黒猫", 8, 1, 0 };
	enemy[3] = { 3, "松下　宗一郎",20,3,0 };
	enemy[4] = { 4, "Reijer Grimbergen",25,4,0 };
}

//C0117101木村勇大
void playerInit() {
	status = { "あなた",MAXHP,0,0,0,0,"なし","なし","なし","[効果]なし","[効果]なし","[効果]なし",1,1,1 };
}

//C0117101木村勇大
void cardInit() {
	card[0] = { "ノーマルカード","[効果]グーで与えるダメージが5になる", 5, 1 };
	card[1] = { "ノーマルカード","[効果]チョキで与えるダメージが5になる", 5, 1 };
	card[2] = { "ノーマルカード","[効果]パーで与えるダメージが5になる", 5, 1 };
	card[3] = { "力と引き換えに...","[効果]グーで与えるダメージが15になるが、反動で2ダメージ受ける", 15, 2 };
	card[4] = { "バイバイ","[効果]3ダメージを与える。次のターン連続で勝つと与えるダメージが4倍になる(あいこ、負けだと効果は失われる)", 3, 2 };
	card[5] = { "ギガドレイン","[効果]パーで勝った際に3ダメージ与えて自分のHPが3回復する", 3, 2 };
	card[6] = { "AI上の奇跡","[効果]相手に2回ランダムでダメージを与える(0～5ダメージ)",0,3 };
	card[7] = { "あいこボーナス","[効果]次のあいこで2ダメージ相手に多く与える",2,3 };
	card[8] = { "ベホマ","[効果]パーで勝った時1ダメージを与えてHPを5回復する",1,3 };
}



//C0117070 大渕公平
void showDungeon() {
	for (int i = 0; i < 25; i++) {
		printf("     ");
		for (int j = 0; j < 5; j++) {

			//まだ訪れたことがない場所なら空白にして表示
			if (r[i / 5][j]->playergone == 0) {
				printf("　　　　　");
			}
			else {
				//プレイヤーのいる部屋の真ん中にPと表示
				if ((i % 5 == 2) && r[i / 5][j]->playerflag == 1) {
					if (j == 0) {
						printf("■　Ｐ　　");
					}
					else if (j == 4) {
						printf("　　Ｐ　■");
					}
					else {
						printf("　　Ｐ　　");
					}
				}
				/*イベントがあるかどうか確認する。敵が生きていてボス出なければEを、ボスならBを、倒された後なら墓に見立てた士を出力する。
				  さらに、eventflagをみて宝箱があるなら宝、泉があるなら泉と表示する。*/
				else if (i % 5 == 1) {
					if (r[i / 5][j]->enemyflag != 0) {
						if (e[r[i / 5][j]->enemyflag - 1]->deathFlag == 0
							&& (r[i / 5][j]->enemyflag > 0 && r[i / 5][j]->enemyflag < 4)) {
							printf("■　Ｅ　■");
						}
						else if (e[r[i / 5][j]->enemyflag - 1]->deathFlag == 0
							&& (r[i / 5][j]->enemyflag == 4 || r[i / 5][j]->enemyflag == 5)) {
							printf("■　Ｂ　■");
						}
						else {
							printf("■　†　■");
						}
					}
					else if (r[i / 5][j]->eventflag != 0) {
						if (r[i / 5][j]->eventflag == 1) {
							printf("■　宝　■");
						}
						else if (r[i / 5][j]->eventflag == 2) {
							printf("■　泉　■");
						}
					}
					//イベントがなければ空白を出力
					else {
						printf("■　　　■");
					}
				}
				//部屋の壁を描画
				else if (i == 0 || i == 24) {
					printf("■■■■■");
				}

				//部屋と部屋の間に通路を作る
				else if (i % 5 == 2) {
					if (j == 0) {
						printf("■　　　　");
					}
					else if (j == 4) {
						printf("　　　　■");
					}
					else {
						printf("　　　　　");
					}
				}
				else if (i % 5 == 0 || i % 5 == 4) {
					printf("■■　■■");
				}
				else {
					printf("■　　　■");
				}
			}
		}
		//次の列に行くために改行
		printf("\n");
	}
}

//C0117070 大渕公平
void showRoom() {
	int playerX = 0, playerY = 0;
	int result = 0;
	int boss = 0;

	//現在のプレイヤーのいる座標をplayerX,playerYに格納
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (r[i][j]->playerflag == 1) {
				playerX = j;
				playerY = i;
			}
		}
	}

	//プレイヤーの位置に敵がいたら戦闘開始
	if (r[playerY][playerX]->enemyflag != 0) {
		if (e[r[playerY][playerX]->enemyflag - 1]->deathFlag == 0) {
			result = battle(p, e[r[playerY][playerX]->enemyflag - 1]);

			//戦闘に勝ったら敵の種類に応じてカードをプレイヤーに渡す
			if (result == 1) {
				switch (r[playerY][playerX]->enemyflag) {
				case 1:
					checkCard(0);
					e[0]->deathFlag = 1;
					break;
				case 2:
					checkCard(1);
					e[1]->deathFlag = 1;
					break;
				case 3:
					checkCard(2);
					e[2]->deathFlag = 1;
					break;
				case 4:
					//松下との闘いに勝つとそのままグリムベルゲンとの戦闘
					printf("\n  何とあまのじゃくな松下がHPを5回復してくれた！\n");
					Sleep(1000);
					printf("  %sは研究室の奥へ足を進めた...\n\n", p->name);
					p->hp += 5;
					if (p->hp >= MAXHP) {
						p->hp = MAXHP;
					}
					system("PAUSE");
					system("cls");

					drawEnemy(5);

					Sleep(1500);
					printf("  呼吸が速い...身体が震える...脳が警鐘を鳴らす...\n");
					Sleep(1500);
					printf("  単位がヤバいと全細胞が告げるッ\n");
					Sleep(1500);
					printf("  研究室の奥から誰かが来る...\n");
					Sleep(1500);

					system("PAUSE");
					system("cls");

					boss = battle(p, e[4]);
					if (boss == 1) {
						gameFlag = 2;
					}
					else {
						gameFlag = 1;
					}
					break;
				default:
					printf("エラー\n");
				}
			}
			else {
				//戦闘に負けるとゲームオーバー
				gameFlag = 1;
			}
		}
		else {
			//敵がいて、すでに倒された後だったら表示
			printf("\n  %sのお墓がある。\n\n", e[r[playerY][playerX]->enemyflag - 1]->name);
		}
	}

	//宝箱のマス、泉のマスだったらイベント発生
	if (r[playerY][playerX]->eventflag == 1) {
		drawEvent(1);
		printf("  これ見よがしに宝箱がある。\n");
		treasure(playerX, playerY);
	}
	else if (r[playerY][playerX]->eventflag == 2) {
		printf("  泉をみつけた。\n");
		//プレイヤーの体力がMAXHP以上だったら体力回復せずそのままスルー
		if (p->hp >= MAXHP) {
			printf("\n  まだ舞える!(HP満タンなので)スルーした。\n\n");
		}
		else {
			spring(playerX, playerY);
		}
	}
}

//C0117206 田畑佑大
//戦闘を行うプログラム
int battle(myStatus* p, enemyStatus* e) {
	int playerHand = 0, enemyHand = 0, flag = 0, aiko = 0, damage = 0, turn = 0;

	while (1) {
		//現在の状態を表示
		drawEnemy(e->number);
		printf("=========================================\n");
		printf("\t%-10s", p->name);
		printf("%s\n", e->name);
		printf("\tHP:%-8d", p->hp);
		printf("HP:%d  ATK:%d\n", e->hp, e->atk);
		printf("=========================================\n\n");
		enemyText(e->number, turn);
		while (1) {
			//自分の手を入力
			printf("  【あなたの手は？】\n  1:グー　2:チョキ　3:パー\n　>");
			playerHand = getchar();
			while (getchar() != '\n');
			if (playerHand == '1' || playerHand == '2' || playerHand == '3') {
				break;
			}
			else {
				printf("  もう一度入力してください\n>");
				continue;
			}
		}
		//敵の手をランダムに決定
		enemyHand = rand() % 3 + 1;
		printf("\n");
		printf("  さいしょはグー、じゃんけん");
		Sleep(100);
		printf(".");
		Sleep(300);
		printf(".");
		Sleep(300);
		printf(".");
		Sleep(300);
		printf("ぽん！\n");

		//自分の手、敵の手を比較して勝敗を決定
		switch (playerHand) {
			//自分の手がグー
		case '1':
			printf("  %s:グー", p->name);
			switch (enemyHand) {
				//相手の手もグーの時
			case GU:
				printf("\t%s:グー\n\n", e->name);
				draw++;
				Sleep(1000);
				printf("  ");
				//カード効果が発動している時
				if (aiko == 1) {
					printf("あいこだ！特殊効果発動！自分は1ダメージ、相手は3ダメージ受けた！\n");
					e->hp -= 3;
					p->hp -= 1;
					aiko = 0;
					Sleep(1000);
				}
				else {
					e->hp -= 1;
					p->hp -= 1;
					flag = 0;
					printf("あいこだ！お互い1ダメージ受けた！\n");
					Sleep(1000);
				}
				break;
				//相手の手がチョキの時
			case CHOKI:
				printf("\t%s:チョキ\n\n", e->name);
				win++;
				Sleep(1000);
				printf("  ");
				//特殊効果を持つカードを持っている時
				if (p->cardflaggu == 3) {
					printf("%sの勝ち！2回攻撃！\n", p->name);
					Sleep(1000);
					damage = rand() % 6;
					if (flag == 1) {
						printf("効果発動!与えるダメージが4倍になるぞ!\n");
						Sleep(1000);
						damage *= 4;
					}
					e->hp -= damage;
					printf("  1回目！%sに%dダメージ！\n", e->name, damage);
					damage = 0;
					Sleep(1000);
					damage = rand() % 6;
					if (flag == 1) {
						damage *= 4;
					}
					e->hp -= damage;
					printf("  2回目！%sに%dダメージ！\n", e->name, damage);
					Sleep(1000);
					damage = 0;
				}
				else if (flag == 1) {
					e->hp -= p->atkgu * 2;
					printf("%sの勝ち！特殊効果発動！%sに4倍の%dダメージ与えた！\n", p->name, e->name, p->atkgu * 4);
					Sleep(1000);
				}
				//特殊効果を持つカードがなければそのままATK分ダメージ
				else {
					e->hp -= p->atkgu;
					printf("%sの勝ち！%sに%dダメージ与えた！\n", p->name, e->name, p->atkgu);
					Sleep(1000);
				}
				//特殊効果を持つカードで反動ありなら自分にもダメージ
				if (p->cardflaggu == 2) {
					printf("  %sは反動で2ダメージうけた！\n", p->name);
					p->hp -= 2;
				}
				flag = 0;
				break;
				//敵の手がパー
			case PA:
				printf("\t%s:パー\n\n", e->name);
				lose++;
				Sleep(1000);
				printf("  ");
				p->hp -= e->atk;
				flag = 0;
				//敵のATK分ダメージ
				printf("%sの勝ち！%sは%dダメージくらった！\n", e->name, p->name, e->atk);
				Sleep(1000);
				break;
			}
			break;
			//自分の手がチョキ
		case '2':
			printf("  %s:チョキ", p->name);
			switch (enemyHand) {
				//敵の手がグー
			case GU:
				printf("\t%s:グー\n\n", e->name);
				lose++;
				Sleep(1000);
				printf("  ");
				p->hp -= e->atk;
				flag = 0;
				//敵のATK分ダメージ
				printf("%sの勝ち！%sは%dダメージくらった！\n", e->name, p->name, e->atk);
				Sleep(1000);
				break;
				//敵の手がチョキ
			case CHOKI:
				printf("\t%s:チョキ\n\n", e->name);
				draw++;
				Sleep(1000);
				printf("  ");
				//特殊効果を持つカード特殊効果を持つカードの処理
				if (aiko == 1) {
					printf("あいこだ！特殊効果発動！自分は1ダメージ、相手は3ダメージ受けた！\n");
					e->hp -= 3;
					p->hp -= 1;
					aiko = 0;
					Sleep(1000);
				}
				else {
					e->hp -= 1;
					p->hp -= 1;
					flag = 0;
					printf("あいこだ！お互い1ダメージ受けた！\n");
					Sleep(1000);
				}
				break;
				//相手の手がパー
			case PA:
				printf("\t%s:パー\n\n", e->name);
				win++;
				Sleep(1000);
				printf("  ");
				//特殊効果を持つカードの処理
				if (flag == 1) {
					e->hp -= p->atkchoki * 2;
					printf("%sの勝ち！特殊効果発動！%sに4倍の%dダメージ与えた！\n", p->name, e->name, p->atkchoki * 4);
					Sleep(1000);
				}
				else {
					e->hp -= p->atkchoki;
					printf("%sの勝ち！%sに%dダメージ与えた！\n", p->name, e->name, p->atkchoki);
					Sleep(1000);
				}
				//特殊効果を持つカードの処理
				if (p->cardflagchoki == 2) {
					Sleep(600);
					printf("  カード効果発動！次のターン勝つと与えるダメージが4倍になるぞ！\n");
					Sleep(1000);
					flag = 1;
				}
				else if (p->cardflagchoki == 3) {
					Sleep(600);
					printf("  カード効果発動！あいこだった場合1回だけ%sに3ダメージ与えるぞ!\n", e->name);
					Sleep(1000);
					aiko = 1;
				}
				break;
			}
			break;
			//自分の手がパー
		case '3':
			printf("  %s:パー", p->name);
			switch (enemyHand) {
				//相手の手がグー
			case GU:
				printf("\t%s:グー\n\n", e->name);
				win++;
				Sleep(1000);
				printf("  ");
				//特殊効果を持つカードの処理特殊効果を持つカードの処理
				if (flag == 1) {
					e->hp -= p->atkpa * 2;
					printf("%sの勝ち！特殊効果発動！%sに4倍の%dダメージ与えた！\n", p->name, e->name, p->atkpa * 4);
					Sleep(1000);
				}
				else {
					e->hp -= p->atkpa;
					printf("%sの勝ち！%sに%dダメージ与えた！\n", p->name, e->name, p->atkpa);
					Sleep(1000);
				}
				//特殊効果を持つカードの処理
				if (p->cardflagpa == 2) {
					Sleep(600);
					printf("  特殊効果発動！HPが3回復した！\n");
					Sleep(1000);
					p->hp += 3;
					if (p->hp >= 20) {
						p->hp = MAXHP;
					}
				}
				else if (p->cardflagpa == 3) {
					Sleep(600);
					printf("  特殊効果発動！HPが5回復した！\n");
					Sleep(1000);
					p->hp += 5;
					if (p->hp >= 20) {
						p->hp = MAXHP;
					}
				}
				flag = 0;
				break;
				//敵の手がチョキ
			case CHOKI:
				printf("\t%s:チョキ\n\n", e->name);
				lose++;
				Sleep(1000);
				printf("  ");
				p->hp -= e->atk;
				flag = 0;
				//敵のATK分ダメージ
				printf("%sの勝ち！%sは%dダメージくらった！\n", e->name, p->name, e->atk);
				Sleep(1000);
				break;
				//敵の手がパー
			case PA:
				printf("\t%s:パー\n\n", e->name);
				draw++;
				Sleep(1000);
				printf("  ");
				//特殊効果を持つカードの処理
				if (aiko == 1) {
					printf("あいこだ！特殊効果発動！自分は1ダメージ、相手は3ダメージ受けた！\n");
					e->hp -= 3;
					p->hp -= 1;
					aiko = 0;
					Sleep(1000);
				}
				else {
					e->hp -= 1;
					p->hp -= 1;
					flag = 0;
					printf("あいこだ！お互い1ダメージ受けた！\n");
					Sleep(1000);
				}
				break;
			}
			break;
		}
		turn++;
		printf("\n");

		//敵を倒した時1(勝利)を返して終了
		if (e->hp <= 0) {
			printf("  %sをやっつけた！\n\n", e->name);
			Sleep(1000);
			return 1;
		}
		//自分が負けたとき2(ゲームオーバー)を返して終了
		if (p->hp <= 0) {
			return 2;
		}
		//それ以外なら戦闘続行
		else {
			printf("\n\n");
			system("PAUSE");
			system("cls");
			continue;
		}
	}

}

//C0117070 大渕公平
//泉の動作を処理する関数
void spring(int x, int y) {
	int flag = 0;
	printf("  ペロッ...これは水素水!!\n");
	printf("  なんだか からだに よさそうだ!　飲んじゃう？(y/n)\n  >");
	while (1) {
		flag = getchar();
		while (getchar() != '\n');
		//泉を使うなら体力を回復して泉のeventflagを0にして終了
		if (flag == 'y') {
			system("cls");
			drawEvent(0);
			printf("  うぉぉぉぉ!!漲ってきたぁぁ!! HPが%d回復した\n", MAXHP - p->hp);
			printf("  水素の力ってすげー!\n");
			p->hp = MAXHP;
			r[y][x]->eventflag = 0;
			printf("\n\n");
			system("PAUSE");
			system("cls");
			break;
		}
		//使わないならそのまま終了
		else if (flag == 'n') {
			system("cls");
			break;
		}
		else {
			printf("エラー\nもう一度入力してください>");
			continue;
		}
	}
}

//C0117070 大渕公平
//宝箱の動作を処理する関数
void treasure(int x, int y) {
	int open = 0, hit = 0, treasure = 0;
	while (1) {
		printf("  宝箱を開きますか？(y/n)\n>");
		open = getchar();
		while (getchar() != '\n');
		if (open == 'y') {
			printf("  宝箱をあけてみた");
			Sleep(600);
			printf(".");
			Sleep(600);
			printf(".");
			Sleep(600);
			printf(".\n");
			Sleep(800);
			//宝箱の中身があたりか罠か判別する(0～3まで,0～2ならあたり、3だと罠)
			hit = rand() % 4;
			if (hit < 3) {
				//宝箱の中身を決める。(宝箱の中に入っているカードはc[3]～c[8]まで)
				treasure = rand() % 6;
				system("cls");
				drawEvent(3);
				checkCard(treasure + 3);
			}
			else {
				//中身がはずれの場合
				system("cls");
				drawEvent(2);
				printf("  平凡な日々、退屈してませんか？\n");
				printf("  そんなあなたに刺激をプレゼント！\n  5ダメージをうけた！\n");
				p->hp -= 5;
				//罠でHPが0になった時表示
				if (p->hp <= 0) {
					printf("      ＿人 人 人 人＿\n");
					printf("      ＞ 突然の死 ＜\n");
					printf("      ￣Y^ Y^ Y^ Y^￣\n");
					gameFlag = 1;
				}
				printf("\n\n");
				system("PAUSE");
				system("cls");
			}
			//eventflagを0にして終了
			r[y][x]->eventflag = 0;
			break;
		}
		//空けない場合はそのまま
		else if (open == 'n') {
			break;
		}
		else {
			printf("エラー\n");
			continue;
		}

		printf("\n\n");
		system("PAUSE");
		system("cls");
	}
}

//C0117070 大渕公平
//カードを手に入れたときに重複がないかチェックする関数
void checkCard(int num) {
	int change = 0;
	//与えられた引数でカードの種類を分ける
	switch (num % 3 + 1) {
	case GU:
		printf("  グー:%sをみつけた！\n", c[num]->name);
		Sleep(800);
		//すでにカードを装備している場合、装備するか捨てるか選択する
		if (p->cardflaggu != 0) {
			while (1) {
				printf("  すでにカードを装備しています。\n");
				printf("  現在装備しているカード;%s\n", p->gueffect);
				printf("　  　　　入手したカード:%s\n", c[num]->effect);
				printf("  装備を変えますか？(装備しなかったカードは失われます)(y/n)\n>");
				change = getchar();
				while (getchar() != '\n');
				//装備を変えるならカードの中身をコピーする
				if (change == 'y') {
					printf("  新たなカードを装備した！\n");
					strcpy_s(p->cardnamegu, c[num]->name);
					strcpy_s(p->gueffect, c[num]->effect);
					p->cardflaggu = c[num]->flag;
					p->atkgu = c[num]->atk;
					break;
				}
				//変えないならそのまま
				else if (change == 'n') {
					printf("  手に入れたカードを捨てた\n");
					break;
				}
				else {
					printf("  エラー\n");
					continue;
				}
			}
		}
		//カードを持っていなかったら、カードの内容をコピー
		else {
			printf("  新たなカードを装備した！\n\n");
			printf("  -%s-\n  %s\n", c[num]->name, c[num]->effect);
			Sleep(800);
			strcpy_s(p->cardnamegu, c[num]->name);
			strcpy_s(p->gueffect, c[num]->effect);
			p->cardflaggu = c[num]->flag;
			p->atkgu = c[num]->atk;
		}
		break;
	case CHOKI:
		printf("  チョキ:%sをみつけた！\n", c[num]->name);
		Sleep(800);
		if (p->cardflagchoki != 0) {
			while (1) {
				printf("  すでにカードを装備しています。\n");
				printf("  現在装備しているカード;%s\n", p->chokieffect);
				printf("  　　　　入手したカード:%s\n", c[num]->effect);
				printf("  装備を変えますか？(装備しなかったカードは失われます)(y/n)\n>");
				change = getchar();
				while (getchar() != '\n');
				if (change == 'y') {
					printf("  新たなカードを装備した！\n");
					strcpy_s(p->cardnamechoki, c[num]->name);
					strcpy_s(p->chokieffect, c[num]->effect);
					p->cardflagchoki = c[num]->flag;
					p->atkchoki = c[num]->atk;
					break;
				}
				else if (change == 'n') {
					printf("  カードを捨てた\n");
					break;
				}
				else {
					printf("エラー\n");
					continue;
				}
			}
		}
		else {
			printf("  新たなカードを装備した！\n\n");
			printf("  -%s-\n  %s\n", c[num]->name, c[num]->effect);
			Sleep(800);
			strcpy_s(p->cardnamechoki, c[num]->name);
			strcpy_s(p->chokieffect, c[num]->effect);
			p->cardflagchoki = c[num]->flag;
			p->atkchoki = c[num]->atk;
		}
		break;
	case PA:
		printf("  パー:%sをみつけた！\n", c[num]->name);
		Sleep(800);
		if (p->cardflagpa != 0) {
			while (1) {
				printf("  すでにカードを装備しています。\n");
				printf("  現在装備しているカード;%s\n", p->paeffect);
				printf("　  　　　入手したカード:%s\n", c[num]->effect);
				printf("  装備を変えますか？(装備しなかったカードは失われます)(y/n)\n>");
				change = getchar();
				while (getchar() != '\n');
				if (change == 'y') {
					printf("  新たなカードを装備した！\n");
					strcpy_s(p->cardnamepa, c[num]->name);
					strcpy_s(p->paeffect, c[num]->effect);
					p->cardflagpa = c[num]->flag;
					p->atkpa = c[num]->atk;
					break;
				}
				else if (change == 'n') {
					printf("  カードを捨てた\n");
					break;
				}
				else {
					printf("  エラー\n");
					continue;
				}
			}
		}
		else {
			printf("  新たなカードを装備した！\n\n");
			printf("  -%s-\n  %s\n", c[num]->name, c[num]->effect);
			Sleep(800);
			strcpy_s(p->cardnamepa, c[num]->name);
			strcpy_s(p->paeffect, c[num]->effect);
			p->cardflagpa = c[num]->flag;
			p->atkpa = c[num]->atk;
		}
		break;
	default:
		break;
	}
	printf("\n\n");
	system("PAUSE");
	system("cls");
}


//C0117070 大渕公平
//プレイヤーの移動を行う関数
void movePlayer() {
	int playerX, playerY, direction = 0, flag = 0, dirX = 0, dirY = 0;

	printf("%sのHP: %d\n", p->name, p->hp);

	//現在プレイヤーのいる位置を保存
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (r[i][j]->playerflag == 1) {
				playerX = j;
				playerY = i;
			}
		}
	}

	while (1) {
		printf("\n進む方向を選択してください(↑;w ←:a ↓:s →:d p:ステータス)\n>");
		direction = getchar();
		while (getchar() != '\n');
		switch (direction) {
		case 'w':
			//上に行く場合、壁があったら入力しなおし
			if (playerY <= 0) {
				printf("これ以上上には行けません\n");
				continue;
			}
			//上にボス部屋がある場合、警告のためflagを2にして入力した方向を保存しておく
			if (r[playerY - 1][playerX]->enemyflag == 4) {
				flag = 2;
				dirY = -1;
			}
			//それ以外なら移動する
			else {
				r[playerY][playerX]->playerflag = 0;
				r[playerY - 1][playerX]->playerflag = 1;
				r[playerY - 1][playerX]->playergone = 1;
				flag = 1;
			}
			break;
		case 'a':
			//左に行く場合、壁があったら入力しなおし
			if (playerX <= 0) {
				printf("これ以上左には行けません\n");
				continue;
			}
			//左にボス部屋がある場合、flagを2にして入力した方向を保存
			if (r[playerY][playerX - 1]->enemyflag == 4) {
				flag = 2;
				dirX = -1;
			}
			//それ以外なら移動する
			else {
				r[playerY][playerX]->playerflag = 0;
				r[playerY][playerX - 1]->playerflag = 1;
				r[playerY][playerX - 1]->playergone = 1;
				flag = 1;
			}
			break;
		case 's':
			//下に行く場合、壁があったら入力しなおし
			if (playerY >= 4) {
				printf("これ以上下には行けません\n");
				continue;
			}
			//下にボス部屋がある場合、flagを2にして入力した方向を保存
			if (r[playerY + 1][playerX]->enemyflag == 4) {
				flag = 2;
				dirY = 1;
			}
			//それ以外なら移動する
			else {
				r[playerY][playerX]->playerflag = 0;
				r[playerY + 1][playerX]->playerflag = 1;
				r[playerY + 1][playerX]->playergone = 1;
				flag = 1;
			}
			break;
		case 'd':
			//右に行く場合、壁があったら入力しなおし
			if (playerX >= 4) {
				printf("これ以上右には行けません\n");
				continue;
			}
			//右にボス部屋がある場合、flagを2にして入力した方向を保存
			if (r[playerY][playerX + 1]->enemyflag == 4) {
				flag = 2;
				dirX = 1;
			}
			//それ以外なら移動する
			else {
				r[playerY][playerX]->playerflag = 0;
				r[playerY][playerX + 1]->playerflag = 1;
				r[playerY][playerX + 1]->playergone = 1;
				flag = 1;
			}
			break;
		case 'p':
			//ステータスを表示
			showStatus();
			break;
		default:
			printf("もう一度入力しなおしてください\n");
			break;
		}

		//入力した先にボス部屋がある場合、警告を行う
		if (flag == 2) {
			int ans = 0;
			printf("この先の部屋から強そうな雰囲気を感じる・・・このまま進みますか？(y/n)\n>");
			while (1) {
				ans = getchar();
				if (ans == 'y') {
					//そのまま進むなら移動処理を行う
					while (getchar() != '\n');
					r[playerY][playerX]->playerflag = 0;
					r[playerY + dirY][playerX + dirX]->playerflag = 1;
					r[playerY + dirY][playerX + dirX]->playergone = 1;
					flag = 1;
					break;
				}
				else if (ans == 'n') {
					//移動しないなら入力画面に戻る
					while (getchar() != '\n');
					break;
				}
				else {
					while (getchar() != '\n');
					printf("もう一度入力してください\n");
					continue;
				}
			}
			dirX = 0;
			dirY = 0;
		}
		if (flag == 1) {
			break;
		}
	}
	flag = 0;
	p->walkcount++;

	//5部屋分進んだら1ダメージを受ける
	if (p->walkcount == 5) {
		printf("%sは疲労で体力が1減った。\n", p->name);
		Sleep(500);
		p->hp -= 1;
		p->walkcount = 0;
		if (p->hp <= 0) {
			gameFlag = 1;
		}
	}
}

//C0117070 大渕公平、C0117206 田畑佑大
//ゲーム結果を表示する関数
void  resultDisplay(int mode) {
	//勝率の計算
	int total = 0;
	float rate = 0, percent = 0;
	total = win + lose + draw;
	rate = (float)win / total;
	percent = rate * 100;
	//ゲームオーバー画面
	if (mode == 1) {
		printf("　%sはちからつきた\n", p->name);
		Sleep(2000);
		printf("  意識が、単位が\n");
		Sleep(2000);
		printf("  ...卒業が、遠のく\n");
		Sleep(2000);
		printf("  次、こそはッ");
		Sleep(1000);
		printf(".");
		Sleep(1000);
		printf(".");
		Sleep(1000);
		printf(".\n");
		printf("\n  　　　　         　 ------GAME OVER------\n\n");
	}
	//ゲームクリア画面
	else if (mode == 2) {
		system("PAUSE");
		system("cls");
		printf("	       　　　　  --Congratulations!!!!--\n");
		printf("\n");
		printf("                         　　)　　　　  　　　) \n");
		printf("                      　　 ／　　　　　　　 ／  \n");
		printf("                    　　[／　Ｖ]―[￣〝”]／    \n");
		printf("                     　　 ￣￣　　　￣￣        \n");
		printf("'                      , △　　　　'　 。     \n");
		printf("===================================================================\n\n");
		Sleep(1000);
		printf("           やったー！　グリムベルゲンの説得に成功したぞ!!\n\n");
		Sleep(2000);
		printf("       あなたは見事単位を取得し、グリムベルゲン研究室に配属された!!\n");
		Sleep(2000);
		printf("\n\n                      -----GAME CLEAR!!!!------\n\n");
		Sleep(1000);
		printf("                   ※この作品はフィクションです。\n");
		printf("              実在の人物、団体等とは一切関係ありません。\n\n\n");
	}
	else {
		printf("ERROR!!!!\n");
	}
	Sleep(1000);

	//今までのじゃんけん結果を表示
	printf("                      =========SCORE=======\n");
	printf("                           TOTAL: %d\n\n", total);
	printf("                             WIN: %d\n", win);
	printf("                 　　　　　 LOSE: %d\n", lose);
	printf("              　　　　　　　DRAW: %d\n\n", draw);
	printf("         　　　　　　　 WIN RATE: %.1f％\n", percent);
	printf("                      =====================\n\n");

	if (percent < 10) {
		printf("                       もっと頑張りましょう\n");
	}
	else if (percent >= 10 && percent < 20) {
		printf("                          　頑張りましょう\n");
	}
	else if (percent >= 20 && percent < 30) {
		printf("                             頑張りました\n");
		if (mode == 1) {
			printf("                       ゲームオーバーだけどね...\n");
		}
	}
	else if (percent >= 30 && percent < 40) {
		printf("                            よくできました\n");
		if (mode == 1) {
			printf("                       ゲームオーバーだけどね...\n");
		}
	}
	else if (percent > 40) {
		printf("                      たいへんよくできました\n");
		if (mode == 1) {
			printf("                       ゲームオーバーだけどね...\n");
		}
	}

	printf("\n");
	system("PAUSE");
	system("cls");
}



//以下ＡＡ部分

//C0117070 大渕　公平
void showTitle() {
	printf("\t　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　■\n");
	printf("\t　　　　　　　　　　　　　　　　　　　　　　　　　　■■　　　　　　　　　　　　　　　　　　　　　　　　　■　　　　■　　■\n");
	printf("\t　　　■■■■　　　　　　■■■■■　■　　　　　　■　　　　　　　　■■　　　　　　　　　　　　　　　　■　　　　　■\n");
	printf("\t　　■　　　　■　　　　■■　　　　■■　　　　　　■　■■■　　　　　　■　　　　　　　　　　　　　　■■■■■■　　\n");
	printf("\t　■■　　　　　　　　■　　　　　　　■　　　　　■■■　　　　　　　　　　■　　　　　　　■　　　　■■　　　　■■\n");
	printf("\t　■■　　　　　　　■■　　　　　　　　　　■■■　■　　　　　　　　　　　　　　　　　　■　　　　　■　　　　　■\n");
	printf("\t　　■■　　　　　　■　　　　　　　　　　　　　　　　■　　　■■　　　　　　　　　　　■　　　　　■　　　　　■\n");
	printf("\t　　　　■■　　　　■　　　　　　　　　　　　　　　■■■■■　　　　　　　　　　　　■　　　　　■　　　　　■■\n");
	printf("\t　　　　　　■■　　■　　　　　　　　　　■■■■■　■　　　　　　　　　　　　　　■　　　　　　　　　　　　■\n");
	printf("\t　　　　　　　■■　■■　　　　　　　　　　　　　　　■　　　　　　　　　　　　　■　　　　　　　　　　　　■\n");
	printf("\t　■　　　　　■■　　■　　　　　　　■　　　　　　　■■　　　　　　　　　　■■　　　　　　　　　　　　■\n");
	printf("\t　■■　　　　■　　　■■■　　　　■　　　　　　　　　■　　　　　　　　　■　　　　　　　　　　　　■■\n");
	printf("\t　■　■■■■　　　　　　■■■■■　　　　　　　　　　■　　　　　　　■■　　　　　　　　　　　■■\n");
	printf("\t　　　　　　　　　　　　　　　　　　　　　　　　　　　　■\n");
	/*
	printf("\t\t\t\t\t　　　　　　　　　　　　　　　　　　単位王者　　　　　　　　　　　　  　　　　　　　\n");
	printf("\t\t\t\t\t　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　■■　\n");
	printf("\t\t\t\t\t　　■■■■■　　　■■■■■　　　　　■　　　　　　　　　■　　■■■■■　■■　\n");
	printf("\t\t\t\t\t　■　　　　　■　■　　　　　■　■■■■■■■　■■　　　■　　■　　　■　　　　\n");
	printf("\t\t\t\t\t　■　　　　　　　■　　　　　　　　　　■　　　　　　■　　■　　■　　　■　　　　\n");
	printf("\t\t\t\t\t　　■■■■■　　■　　　　　　　■■■■■■■　　　　　　■　■　　　　■　　　　\n");
	printf("\t\t\t\t\t　　　　　　　■　■　　　　　　　　　　■　　　　　　　　　■　　　　　　■　　　　\n");
	printf("\t\t\t\t\t　■　　　　　■　■　　　　　■　　　　■　　　　　　　　■■　　　　■■　　　　　\n");
	printf("\t\t\t\t\t　　■■■■■　　　■■■■■　　　　　■　　　　■■■■■　　　　■■　　　　　　\n");
	printf("\t\t\t\t\t　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　\n");*/
	//printf("\t\t\t\t\t　　　　　　　　　　　　　　　～Scool Credit King～　　　　　　　　　　　　　　　　 \n");
	printf("\t\t\t\t\t　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　\n");
	printf("\t\t\t\t\t　　　　　　　　　　　　　　　＝＝＝＝＝＝＝＝＝＝＝　　　　　　　　　　　　　　　　\n");
	printf("\t\t\t\t\t　　　　　　　　　　　　　　　　1>　ゲームを始める　　　　　　　　　　　　　　　　　\n");
	printf("\t\t\t\t\t　　　　　　　　　　　　　　　　2>　ルール　　　　　　　　　　　　　　　　　　　　　\n");
	printf("\t\t\t\t\t　　　　　　　　　　　　　　　　3>　ゲーム終了　　　　　　　　　　　　　　　　　　　\n");
	printf("\t\t\t\t\t　　　　　　　　　　　　　　　＝＝＝＝＝＝＝＝＝＝＝　　　　　　　　　　　　　　　　\n");
	printf("\t\t\t\t\t　　　　　　　　　　　　　　 何をするか選んで下さい>");
}

void enemyText(int number, int turn) {
	if (turn == 0) {
		printf("　%sがあらわれた!\n\n", e[number]->name);
	}
	else {
		printf("　%sがいる。\n\n", e[number]->name);
	}
}

void drawEnemy(int number) {
	if (number == 0) {
		printf("\t  ___             \n");
		printf("\t /´･ヽ           \n");
		printf("\tﾉ＾'ｧ,ハ          \n");
		printf("\t`Zｱ'　/           　　　　　　　　現在の所持カード\n");
		printf("\t ,! 〈                         　　  　グー:%s\n", p->cardnamegu);
		printf("\t /   ヽ､_         　　　　　　　　           %s\n", p->gueffect);
		printf("\t l 　 　｀ヽ、    　　　　　　　　　 チョキ:%s\n", p->cardnamechoki);
		printf("\t ヽ　　    　ヾﾂ                             %s\n", p->chokieffect);
		printf("\t   ＼          /                     　パー:%s\n", p->cardnamepa);
		printf("\t     ヽ rーヽ ﾉ                              %s\n", p->paeffect);
		printf("\t    __|| ､  || ､                   \n");
	}
	else if (number == 1) {
		printf("\t    　 _______|＼/＼    \n");
		printf("\t　 　／  ______ ＼  /   \n");
		printf("\t    | ／       ＼  |    \n");
		printf("\t　  | | ・   ・  | |              現在の所持カード\n");
		printf("\t　  | |    _     | |                   グー:%s\n", p->cardnamegu);
		printf("\t     ＼＼      ／／                         %s\n", p->gueffect);
		printf("\t     ／ ￣￣￣￣ ＼                  チョキ:%s\n", p->cardnamechoki);
		printf("\t   ／|  ／￣￣＼  |＼                       %s\n", p->chokieffect);
		printf("\t の  |  |      |  |　の                パー:%s\n", p->cardnamepa);
		printf("\t     ＼ ＼____／ ／                         %s\n", p->paeffect);
		printf("\t       ＼______／       \n");
		printf("\t       __|    |__       \n");
	}
	else if (number == 2) {
		printf("\t       /ﾞﾐヽ､,,___,,／ﾞヽ    \n");
		printf("\t　     i ノ　　 川　｀ヽ'           現在の所持カード\n");
		printf("\t　     /　｀　・　 ．・ i､   　　　      グー:%s\n", p->cardnamegu);
		printf("\t      彡,　　 ミ(_,人_)彡ミ                   %s\n", p->gueffect);
		printf("\t  ∩  / ヽ､,　　 　　　ノ              チョキ:%s\n", p->cardnamechoki);
		printf("\t  || |     '''''''''''ﾉ                       %s\n", p->chokieffect);
		printf("\t  丶ニ|　             |                  パー:%s\n", p->cardnamepa);
		printf("\t　    ∪⌒∪'￣￣￣￣∪                       %s\n", p->paeffect);
	}
	else if (number == 3) {
		printf("\n");
		printf("\t                                 現在の所持カード\n");
		printf("\t               ＊　ネタ切れ           グー:%s\n", p->cardnamegu);
		printf("\t　+    　　*　　　　　　*                  %s\n", p->gueffect);
		printf("\t  　　＊　　　　　＋                チョキ:%s\n", p->cardnamechoki);
		printf("\t　   　  n ∧___∧ n         +             %s\n", p->chokieffect);
		printf("\t　  ＋　(ﾖ(*´∀｀)E)                 パー:%s\n", p->cardnamepa);
		printf("\t     　 　Y 　   Y　　　　＊               %s\n", p->paeffect);
	}
	else if (number == 4) {
		printf("\n");
		printf("　　　　/ｌ 　　  　　　　 　 　　　 　 /^l\n");
		printf("　　　 /　|　　 　　　　　　　　　　　 /　| 現在の所持カード\n");
		printf("　　　/　　　　　　　　　　　　　　　 /	        グー:%s\n", p->cardnamegu);
		printf("　＋ / 　 　 　 　  　　 　 　　　　 /               %s\n", p->gueffect);
		printf("　　/　+　　＋　　　　　　　　　　　/         チョキ:%s\n", p->cardnamechoki);
		printf("　 ４￣￣　￣￣` 　  　 /￣￣￣￣ヾ/ ＋              %s\n", p->chokieffect);
		printf("　　|　　/　/ //|‐ー‐|　/　　.//| +           パー:%s\n", p->cardnamepa);
		printf("+　.|　 /　/ // |　　　|/ 　　//　|                  %s\n", p->paeffect);
		printf("　　`ー――‐‐'　　　 ｀――――' ＋      \n");
		printf("\n");

	}
	else if (number == 5) {
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("=========================================\n");
		printf("\t%-10s", p->name);
		printf("???\n");
		printf("\tHP:%-8d", p->hp);
		printf("HP:??  ATK:?\n");
		printf("=========================================\n\n");
	}
}


//C0117206 田畑佑大
void showRule() {
	printf("SCキング ルール説明\n");
	printf("---------------------\n");
	printf(" 1) ここは大学という名のとあるダンジョン、あなたはグリムベルゲン研究室を志望中\n");
	printf(" 2) 配属のため待ち構える敵を倒しつつ研究室へ向かい、グリムベルゲン先生を打ち負かしましょう\n");
	printf(" 3) 敵がいる部屋に入ると戦闘が始まります\n");
	printf(" 4) 戦闘はジャンケンで行われます\n");
	printf(" 5) 勝てば敵に、負ければ自分に、あいこでお互いにダメージが入ります\n");
	printf(" 6) ダンジョン内では、じゃんけんで勝つと発動するカードを入手することができます\n");
	printf(" 7) カードは装備することで効果が適用されます *戦闘中は装備を変えることはできません\n");
	printf(" 8) ダンジョン内には泉があり、使用するとHPが回復します。ただし使用できるのは1回だけです\n");
	printf(" 9) 部屋を5回移動するごとに疲労でHPが1減ります\n");
	printf("10) HPが0になるとゲームオーバーになります\n");
	printf("11) 見事グリムベルゲン先生を倒すことが出来ればゲームクリア！\n");
	printf("\n");

	system("PAUSE");
}

// C0117206 田畑佑大
void showStatus() {
	printf("-----STATUS-----\n");
	printf("NAME:%s\n", p->name);
	printf("  HP:%d\n", p->hp);
	printf("・攻撃力\nグー:%d\nチョキ:%d\nパー:%d\n", p->atkgu, p->atkchoki, p->atkpa);
	printf("・装備\n");
	printf("　グー:%s\n　　　　%s\nチョキ:%s\n　　　　%s\n　パー:%s\n　　　　%s\n", p->cardnamegu, p->gueffect, p->cardnamechoki, p->chokieffect, p->cardnamepa, p->paeffect);
	printf("----------------\n");

}

void drawEvent(int num) {
	if (num == 0) {
		printf("\t\n");
		printf("   あぁ～！水素の音～～！\n");
		printf("    　　 　　　　　　　　  ∧_∧\n");
		printf("  　   　   ∧_∧ ＼ﾌﾟｼｨ/（∀｀ ）\n");
		printf("  　   　 （*´∀）　　只E)ヽ/ ） .只\n");
		printf("     　　 /　　 ﾄ､ 　○三} ＼／|　{三}\n");
		printf("     　　.l_つ 、!-' .{三} |　 |　{三}\n");
		printf("     |￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣|\n");
	}
	else if (num == 1) {
		printf("\n");
		printf("\t　 ＿＿＿＿＿＿  \n");
		printf("\t　/￣ /三/￣ /⌒i \n");
		printf("\t　L＿ LO_L ＿L／| \n");
		printf("\t　|二二二二二|／| \n");
		printf("\t　|二二二二二|／  \n");
	}
	else if (num == 2) {
		printf("\n");
		printf("\t　　＿＿＿＿_\n");
		printf("\t　∠_∠∠_／ ＼\n");
		printf("\t　ＶＶＶＶ＼　｜\n");
		printf("\t　 >(･ヽノ･)＼/\n");
		printf("\t　∧/⌒⌒∧ ／|\n");
		printf("\t　L｜｜ |二|／|\n");
		printf("\t　L｜｜ |二|／\n");
		printf("\t　 ﾉ ﾉ　|\n");
		printf("\t　(＿＿ノ\n");
	}
	else if (num == 3) {
		printf("\n");
		printf("\t　　 人\n");
		printf("\t　 ／　＼\n");
		printf("\t　(　'〇'）\n");
		printf("\t　( つ━つ＿＿  \n");
		printf("\t　 ＼|　| 　  |＼\n");
		printf("\t　　 |あ|     | ｜\n");
		printf("\t　　 |た| 　  | ｜\n");
		printf("\t　  _|り|_  ＿|／\n");
		printf("\t　／ |  |   ／｜\n");
		printf("\t　|三￣￣二｜ ｜\n");
		printf("\t　|二二二二｜／\n");
	}
	printf("\n=============================================\n");
}