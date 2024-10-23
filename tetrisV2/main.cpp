#define _CRT_SECURE_NO_WARNINGS
// ==============================================
// main.cpp [テトリスV2]
// 
// 制作者:ushi34      日付:2024/10/10
// 
// X座標は右が正方向
// Y座標は下が正方向
// 左上が原点
// ==============================================
#include <iostream>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <ios>
#include <iomanip>

// ==============================================
// マクロ定義
// ==============================================

//画面表示関連
#define WINDOW_WIDTH	(23)	//ウィンドウの幅
#define WINDOW_HEIGHT	(24)	//ウィンドウの高さ
#define WINDOW_OFFSET_X	(15)	//ウィンドウ場所調整のための左側に空ける列の数(半角)
#define WINDOW_OFFSET_Y	(2)		//ウィンドウ場所調整のための上側に空ける行の数
#define MAIN_WIDTH	(12)	//メインマップの幅
#define MAIN_HEIGHT	(24)	//メインマップの高さ
#define NEXT_WIDTH	(6)		//NEXTマップの幅
#define NEXT_HEIGHT	(19)	//NEXTマップの高さ
#define HOLD_WIDTH	(6)		//HOLDマップの幅
#define HOLD_HEIGHT	(7)		//HOLDマップの高さ

//テトリミノ関連
#define MINO_SIZE	(4)		//１つのテトリミノのパターンを保存する領域(正方形)の一辺の長さ
#define NEXT_QUEUE_SIZE (14) //次のテトリミノの種類を保存するキューのサイズ

//ゲームルール関連
#define GAMEOVER_LINE_HEIGHT (2) //ゲームオーバーラインのＹ座標
#define MINOSTART_X (4) //テトリミノのスポーン地点(X座標)
#define MINOSTART_Y (0) //テトリミノのスポーン地点(Y座標)

//UI関連
#define MAP_MAIN (0)	//メインマップ
#define MAP_NEXT (1)	//NEXTマップ
#define MAP_HOLD (2)	//HOLDマップ
#define NEXT_DISPLAY_NUM (4) //ネクストミノの画面表示最大数
#define STR_SIZE (32)	 //文字列のサイズ

//文字出力関連
#define SPACE			(0)	//全角スペースのキー
#define WALL			(1)	//壁ブロック
#define BLOCK_CYAN		(2) //■シアン
#define BLOCK_YELLOW	(3)	//■きいろ
#define BLOCK_GREEN		(4)	//■みどり
#define BLOCK_RED		(5)	//■あか
#define BLOCK_BLUE		(6)	//■あお
#define BLOCK_WHITE		(7)	//■しろ
#define BLOCK_MAGENTA	(8)	//■マゼンタ
#define GHOSTBLOCK_OFFSET	(10) //ゴーストブロックのオフセット
#define CHAR_BAR			(9)	 //━

// ==============================================
// 列挙体宣言
// ==============================================
enum MinoType
{
	I,O,S,Z,J,L,T,
	MINO_MAX
};

// ==============================================
// 構造体宣言
// ==============================================
struct BASIC_MINO
{
	MinoType type;		//テトリミノのタイプ
	char shape[MINO_SIZE][MINO_SIZE];	//テトリミノの形
};

struct GAME_MINO
{
	BASIC_MINO basicInfo; //形状
	int angle;		//角度(0:上,1:右,2:下,3:左)
	int	x;			//X座標
	int	y;			//Y座標
};

struct NEXTQUEUE
{
	BASIC_MINO mino[NEXT_QUEUE_SIZE];
	int queueNum;					//キューの個数
};


// ==============================================
// グローバル変数宣言
// ==============================================

long long g_frameCount = 0;

char g_mainMap[MAIN_HEIGHT][MAIN_WIDTH] = { 0 }; //メインマップ
char g_nextMap[NEXT_HEIGHT][NEXT_WIDTH] = { 0 }; //NEXTマップ
char g_holdMap[HOLD_HEIGHT][HOLD_WIDTH] = { 0 }; //HOLDマップ
GAME_MINO g_playerMino;		//プレイヤーの動かすテトリミノ
NEXTQUEUE g_nextMinos;		//ネクストの列

// ==============================================
// プロトタイプ宣言
// ==============================================
int	GameMain(int);
void InitializeMap();
void InitializeNextMino();
void AddNextQueue();
struct BASIC_MINO CreateMino(MinoType);
void DrawNextMino();
void ChangePlayerMino(BASIC_MINO);
void RenderScreen();
bool CanMove(char[MINO_SIZE][MINO_SIZE], int, int);
void OutputChar(char);
void ShuffleArray(int array[], int size);
void UpdateNextMap();
void UpdateHoldMap(BASIC_MINO, bool);
bool CheckLineFull(int);
void DeleteOneLine(int);
int  DeleteMapLine();
void SuperRotationSystem(int);
void RotateShape(char[MINO_SIZE][MINO_SIZE], MinoType, bool);
void MoveCursor(int, int);
void DeleteCursor();

// ==============================================
// メイン関数
// ==============================================
int main(void)
{
	DeleteCursor(); //カーソル消す
	srand((unsigned int)time(NULL)); //乱数初期化

	GameMain(1);
}

// ==============================================
// ゲームメイン関数
// ==============================================
int GameMain(int)
{
	//////////////////////////////
	////////////初期化////////////
	//////////////////////////////

	//変数宣言
	bool placeMinoFlg = false;	//テトリミノ設置処理を行うかどうか
	bool canHoldFlg = true;		//ホールドが可能かどうか
	bool holdEmptyFlg = true;	//ホールドが空かどうか
	BASIC_MINO holdMino = {};		//ホールドのミノ
	int deletedLines = 0;		//ライン消去数
	int fallingInterval = 50;	//落下間隔
	int timerSec = 0;
	int timerMin = 0;

	InitializeMap();		//マップ生成
	InitializeNextMino();	//ネクストミノ生成
	DrawNextMino();			//次のテトリミノを引く
	RenderScreen();			//画面出力

	//////////////////////////////
	/////////ゲームループ/////////
	//////////////////////////////
	while (true)
	{
		////////////////////////////////
		///////テトリミノ自然落下///////
		////////////////////////////////
		if (g_frameCount % fallingInterval == 0)
		{
			if (CanMove(g_playerMino.basicInfo.shape, g_playerMino.x, g_playerMino.y + 1))
				g_playerMino.y++;
			else
				placeMinoFlg = true; //テトリミノ設置するコードは下にあるよ	
		}

		////////////////////////////////
		//////キーボード操作の実装//////
		////////////////////////////////

		if (_kbhit())
		{
			int key = _getch();
			switch (key)
			{
			case 'w':
				//ハードドロップ
				while (true)
				{
					if (CanMove(g_playerMino.basicInfo.shape, g_playerMino.x, g_playerMino.y + 1))
						g_playerMino.y++;
					else
					{
						placeMinoFlg = true; //テトリミノ設置するコードは下にあるよ
						break;
					}
				}
				break;

			case 'a':
				//左移動
				if (CanMove(g_playerMino.basicInfo.shape, g_playerMino.x - 1, g_playerMino.y))
					g_playerMino.x--;
				break;

			case 'd':
				//右移動
				if (CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + 1, g_playerMino.y))
					g_playerMino.x++;
				break;

			case 's':
				//下移動
				if (CanMove(g_playerMino.basicInfo.shape, g_playerMino.x, g_playerMino.y + 1))
					g_playerMino.y++;
				else
					placeMinoFlg = true; //テトリミノ設置するコードは下にあるよ
				break;

			case 0x1B:
				//ESCでタイトルに戻る
				return 2;

			case 'c':
			case 'l':
				//ホールド処理
				if (canHoldFlg)
				{
					if (holdEmptyFlg)
					{	
						//プレイヤーのテトリミノをHOLDにうつす
						holdMino = CreateMino(g_playerMino.basicInfo.type);

						DrawNextMino();

						holdEmptyFlg = false;
					}
					else
					{
						//ホールドミノをtempに保存
						BASIC_MINO holdTemp = holdMino;

						//プレイヤーのテトリミノをHOLDにうつす
						holdMino = CreateMino(g_playerMino.basicInfo.type);

						//プレイヤーのミノをtempのミノに変更
						ChangePlayerMino(holdTemp);
					}

					canHoldFlg = false; //ホールドを使用したのでfalseにする

					UpdateHoldMap(holdMino, canHoldFlg); //HOLDマップの描画更新
				}
				break;

			case 'p':
			case 'o':
				if (g_playerMino.basicInfo.type != O)
					//Oミノは回転しないので無視
					SuperRotationSystem(key);
			}

		}
		
		////////////////////////////////
		///////テトリミノ設置処理///////
		////////////////////////////////
		if (placeMinoFlg)
		{ //今までの処理で設置フラグを獲得してた場合、設置処理を行う

			//マップに操作中のテトリミノを書き込む
			for (int i = 0; i < MINO_SIZE; i++)
			{
				for (int j = 0; j < MINO_SIZE; j++)
				{
					if (g_playerMino.basicInfo.shape[i][j] != 0)
						g_mainMap[i + g_playerMino.y][j + g_playerMino.x] = g_playerMino.basicInfo.shape[i][j];
				}
			}

			//揃ったラインの消去
			deletedLines += DeleteMapLine();

			DrawNextMino();

			canHoldFlg = true;

			UpdateHoldMap(holdMino, canHoldFlg);

			placeMinoFlg = false;
		}

		////////////////////////////////
		////////フレーム制御処理////////
		////////////////////////////////

		g_frameCount++;
		RenderScreen();
		Sleep(16);

		////////////////////////////////
		//////////タイマー処理//////////
		////////////////////////////////

		if (g_frameCount % 33 == 0)
		{
			timerSec++;
			if (timerSec == 60)
			{
				timerMin++;
				timerSec = 0;
			}
		}

		MoveCursor(10, 0);
		std::cout << "TIME";
		MoveCursor(11, 0);
		std::cout << std::setfill('0') << std::right << std::setw(2) << timerMin;
		std::cout << ":";
		std::cout << std::setfill('0') << std::right << std::setw(2) << timerSec;

	}

	return 0;
}

// ==============================================
// マップ初期化関数
// ==============================================
void InitializeMap()
{
	////////////////
	//メインマップ//
	////////////////

	//壁・地面を配置する
	for (int i = 0; i < MAIN_HEIGHT; i++)
	{
		for (int j = 0; j < MAIN_WIDTH; j++)
		{
			if (j == 0 || j == MAIN_WIDTH - 1 || i == MAIN_HEIGHT - 1)
				g_mainMap[i][j] = 1;
		}
	}

	////////////////
	///NEXTマップ///
	////////////////

	//外壁を配置する
	for (int i = 0; i < NEXT_HEIGHT; i++)
	{
		for (int j = 0; j < NEXT_WIDTH; j++)
		{
			if (j == 0 || j == NEXT_WIDTH - 1 ||
				i == 0 || i == NEXT_HEIGHT - 1)
				g_nextMap[i][j] = 1; //外壁	
		}
	}

	//文字列の配置

	MoveCursor(1, 17);
	std::cout << "ＮＥＸＴ";
	

	////////////////
	///HOLDマップ///
	////////////////

	//外壁を配置する
	for (int i = 0; i < HOLD_HEIGHT; i++)
	{
		for (int j = 0; j < HOLD_WIDTH; j++)
		{
			if (j == 0 || j == HOLD_WIDTH - 1 ||
				i == 0 || i == HOLD_HEIGHT - 1)
				g_holdMap[i][j] = WALL; //外壁	
		}
	}

	//文字列の配置
	MoveCursor(1, 1);
	std::cout << "ＨＯＬＤ";
	MoveCursor(5, 1);
	std::cout << "ＯＫ";

}

// ==============================================
// ネクストミノ初期化関数
// ==============================================
void InitializeNextMino()
{
	for (int i = 0; i < 2; i++)
	{
		AddNextQueue();
	}
}

// ==============================================
// ネクストミノ抽選関数
// ==============================================
void AddNextQueue()
{
	int data[MINO_MAX];

	for (int j = 0; j < MINO_MAX; j++)
	{
		data[j] = j;
	}

	ShuffleArray(data, MINO_MAX);

	for (int j = 0; j < MINO_MAX; j++)
	{
		g_nextMinos.mino[g_nextMinos.queueNum] = CreateMino((MinoType)data[j]);
		g_nextMinos.queueNum++;
	}

}

// ==============================================
// 次のテトリミノに変更する関数
// ==============================================
void DrawNextMino()
{
	ChangePlayerMino(g_nextMinos.mino[0]);

	g_playerMino.basicInfo = g_nextMinos.mino[0]; //NEXTの先頭のテトリミノを除列

	g_nextMinos.queueNum--; //除列したので-1

	for (int i = 0; i < g_nextMinos.queueNum; i++)
	{
		g_nextMinos.mino[i] = g_nextMinos.mino[i + 1]; //すべてのNEXTミノを１個前に進める
	}

	if (NEXT_QUEUE_SIZE - g_nextMinos.queueNum >= 7)
	{	//ネクストの列の空き数が7個になったら
		//ネクストを補充する
		AddNextQueue(); //一度の補充で7個のミノを作る
	}

	UpdateNextMap(); //ネクストの描画更新

}

// ==============================================
// 操作するテトリミノの変更を行う関数
// 引数に渡したミノに変更する
// ==============================================
void ChangePlayerMino(BASIC_MINO changeMino)
{
	g_playerMino.basicInfo = changeMino;

	//座標・回転の初期化
	g_playerMino.x = MINOSTART_X;
	g_playerMino.y = MINOSTART_Y;
	g_playerMino.angle = 0;
}

// ==============================================
// テトリミノ作成関数
// ==============================================
struct BASIC_MINO CreateMino(MinoType sourceType)
{
	BASIC_MINO basicMino = {};

	basicMino.type = sourceType; //タイプ情報の更新

	//形の情報の更新
	switch (basicMino.type)
	{

	case I:
		basicMino.shape[1][0] = BLOCK_CYAN;
		basicMino.shape[1][1] = BLOCK_CYAN;
		basicMino.shape[1][2] = BLOCK_CYAN;
		basicMino.shape[1][3] = BLOCK_CYAN;
		break;

	case O:
		basicMino.shape[1][1] = BLOCK_YELLOW;
		basicMino.shape[1][2] = BLOCK_YELLOW;
		basicMino.shape[2][1] = BLOCK_YELLOW;
		basicMino.shape[2][2] = BLOCK_YELLOW;
		break;

	case S:
		basicMino.shape[0][1] = BLOCK_GREEN;
		basicMino.shape[0][2] = BLOCK_GREEN;
		basicMino.shape[1][0] = BLOCK_GREEN;
		basicMino.shape[1][1] = BLOCK_GREEN;
		break;

	case Z:
		basicMino.shape[0][0] = BLOCK_RED;
		basicMino.shape[0][1] = BLOCK_RED;
		basicMino.shape[1][1] = BLOCK_RED;
		basicMino.shape[1][2] = BLOCK_RED;
		break;

	case J:
		basicMino.shape[0][0] = BLOCK_BLUE;
		basicMino.shape[1][0] = BLOCK_BLUE;
		basicMino.shape[1][1] = BLOCK_BLUE;
		basicMino.shape[1][2] = BLOCK_BLUE;
		break;

	case L:
		basicMino.shape[0][2] = BLOCK_WHITE;
		basicMino.shape[1][0] = BLOCK_WHITE;
		basicMino.shape[1][1] = BLOCK_WHITE;
		basicMino.shape[1][2] = BLOCK_WHITE;
		break;

	case T:
		basicMino.shape[0][1] = BLOCK_MAGENTA;
		basicMino.shape[1][0] = BLOCK_MAGENTA;
		basicMino.shape[1][1] = BLOCK_MAGENTA;
		basicMino.shape[1][2] = BLOCK_MAGENTA;
		break;
	}

	return basicMino;
}

// ==============================================
// 画面出力関数
// ==============================================
void RenderScreen()
{
	static char displayData[WINDOW_HEIGHT][WINDOW_WIDTH] = { 0 };
	static char displayBuffer[WINDOW_HEIGHT][WINDOW_WIDTH] = { 0 };

	//////////////////
	//配列を書き込む//
	//////////////////

	//メインマップの書き込み
	for (int i = 0; i < MAIN_HEIGHT; i++)
	{
		for (int j = 0; j < MAIN_WIDTH; j++)
		{
			displayBuffer[i][j + HOLD_WIDTH - 1] = g_mainMap[i][j];
		}
	}

	//操作中のテトリミノの現在地から真下のブロックまでの距離を計算する
	int minoMargin = 0;

	while (true)
	{
		if (CanMove(g_playerMino.basicInfo.shape,g_playerMino.x, g_playerMino.y + 1 + minoMargin))
			minoMargin++;
		else
			break;
	}

	//求めたminoMarginを使って、操作中のテトリミノのゴーストブロックの書き込み

	for (int i = 0; i < MINO_SIZE; i++)
	{
		for (int j = 0; j < MINO_SIZE; j++)
		{
			if (g_playerMino.basicInfo.shape[i][j] != 0)
				displayBuffer[minoMargin + i + g_playerMino.y][j + HOLD_WIDTH - 1 + g_playerMino.x] = GHOSTBLOCK_OFFSET +  g_playerMino.basicInfo.shape[i][j];
		}
	}
	
	//ゲームオーバーラインの書き込み
	for (int i = 0; i < MAIN_WIDTH - 2; i++)
	{
		displayBuffer[GAMEOVER_LINE_HEIGHT][HOLD_WIDTH + i] = CHAR_BAR;
	}

	//操作中のテトリミノの書き込み
	for (int i = 0; i < MINO_SIZE; i++)
	{
		for (int j = 0; j < MINO_SIZE; j++)
		{
			if (g_playerMino.basicInfo.shape[i][j] != 0)
				displayBuffer[i + g_playerMino.y][j + HOLD_WIDTH - 1 + g_playerMino.x] = g_playerMino.basicInfo.shape[i][j];
		}
	}

	//NEXTマップの書き込み
	for (int i = 0; i < NEXT_HEIGHT; i++)
	{
		for (int j = 0; j < NEXT_WIDTH; j++)
		{
			displayBuffer[i][j + NEXT_WIDTH + MAIN_WIDTH - 2] = g_nextMap[i][j];
		}
	}

	//HOLDマップの書き込み
	for (int i = 0; i < HOLD_HEIGHT; i++)
	{
		for (int j = 0; j < HOLD_WIDTH; j++)
		{
			displayBuffer[i][j] = g_holdMap[i][j];
		}
	}



	//////////////////
	////画面の出力////
	//////////////////

	for (int i = 0; i < WINDOW_HEIGHT; i++)
	{


		for (int j = 0; j < WINDOW_WIDTH; j++)
		{
			if (displayData[i][j] != displayBuffer[i][j])
			{
				displayData[i][j] = displayBuffer[i][j];

				MoveCursor(i, j);
				OutputChar(displayBuffer[i][j]);

			}
		}

	}

	/*std::cout << "\n";
	std::cout << "　　　　　　A:ひだり D:みぎ   S:した       W:ハードドロップ\n";
	std::cout << "　　　　　　O:左回転 P:右回転 C/L:ホールド ESC:タイトルにもどる\n";*/

}

// ==============================================
// 移動可能か判定する関数
// ==============================================
bool CanMove(char sourceShape[MINO_SIZE][MINO_SIZE], int futurePosX, int futurePosY)
{
	//////////////////////
	///////変数宣言///////
	//////////////////////

	int i, j; //反復処理用

	//////////////////////
	/////動けるか判定/////
	//////////////////////

	//未来のテトリミノの当たり判定を計算して、衝突を判定する

	for (i = 0; i < MINO_SIZE; i++)
	{
		for (j = 0; j < MINO_SIZE; j++)
		{
			//テトリミノの中でブロックがある場所が
			//空白(0)もしくはゲームオーバーライン(9)じゃなかったら
			//falseを返す
			if (sourceShape[i][j] != 0 &&
				g_mainMap[futurePosY + i][futurePosX + j] != SPACE &&
				g_mainMap[futurePosY + i][futurePosX + j] != CHAR_BAR)
				return false;
		}
	}

	return true;

}

// ==============================================
// 文字を出力する関数
// ==============================================
void OutputChar(char key)
{
	switch (key)
	{
	case SPACE:
		std::cout << "  ";
		break;

	case WALL:
		std::cout << "\x1b[47m　\x1b[49m";
		break;

	case BLOCK_CYAN:
		std::cout << "\x1b[36m■\x1b[39m";
		break;

	case BLOCK_CYAN + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[36m■\x1b[39m\x1b[0m";
		break;

	case BLOCK_YELLOW:
		std::cout << "\x1b[33m■\x1b[39m";
		break;

	case BLOCK_YELLOW + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[33m■\x1b[39m\x1b[0m";
		break;

	case BLOCK_GREEN:
		std::cout << "\x1b[32m■\x1b[39m";
		break;

	case BLOCK_GREEN + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[32m■\x1b[39m\x1b[0m";
		break;

	case BLOCK_RED:
		std::cout << "\x1b[31m■\x1b[39m";
		break;

	case BLOCK_RED + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[31m■\x1b[39m\x1b[0m";
		break;

	case BLOCK_BLUE:
		std::cout << "\x1b[34m■\x1b[39m";
		break;

	case BLOCK_BLUE + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[34m■\x1b[39m\x1b[0m";
		break;

	case BLOCK_WHITE:
		std::cout << "\x1b[37m■\x1b[39m";
		break;

	case BLOCK_WHITE + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[37m■\x1b[39m\x1b[0m";
		break;

	case BLOCK_MAGENTA:
		std::cout << "\x1b[35m■\x1b[39m";
		break;

	case BLOCK_MAGENTA + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[35m■\x1b[39m\x1b[0m";
		break;


	case CHAR_BAR:
		std::cout << "\x1b[31m━━\x1b[39m";
		break;

	default:
		break;
	}

}

// ==============================================
// 配列をシャッフルする関数
// ==============================================
void ShuffleArray(int array[], int size)
{
	for (int i = 0; i < MINO_MAX - 1; i++)
	{ //配列のシャッフル
		//配列の後ろからi番目の要素をrandomIndexの要素と入れ替える

		int randomIndex = rand() % (MINO_MAX - i);

		int tempData = array[MINO_MAX - 1 - i];
		array[MINO_MAX - 1 - i] = array[randomIndex];
		array[randomIndex] = tempData;
	}
}

// ==============================================
// NEXTマップ更新関数
// ==============================================
void UpdateNextMap()
{
	for (int i = 0; i < NEXT_DISPLAY_NUM; i++)
	{
		for (int j = 0; j < MINO_SIZE; j++)
		{
			for (int k = 0; k < MINO_SIZE; k++)
			{
				//NEXTのテトリミノのパターンを描画
				g_nextMap[i * 4 + j + 2][k + 1] = g_nextMinos.mino[i].shape[j][k]; 
			}
		}
	}
}

// ==============================================
// HOLDマップ更新関数
// ==============================================
void UpdateHoldMap(BASIC_MINO newHold, bool canHoldFlg)
{
	for (int i = 0; i < MINO_SIZE - 1; i++)
	{
		for (int j = 0; j < MINO_SIZE; j++)
		{
			g_holdMap[i + 2][j + 1] = newHold.shape[i][j];
		}
	}

	//ＵＩ表示更新
	if (canHoldFlg)
	{
		MoveCursor(5, 1);
		std::cout << "ＯＫ";
	}
	else
	{
		MoveCursor(5, 1);
		std::cout << "ＮＧ";
	}
}

// ==============================================
// ラインが揃っているかを判定する関数
// ==============================================
bool CheckLineFull(int y)
{
	//////////////////////
	///////変数宣言///////
	//////////////////////

	int i;

	//////////////////////
	///////判定部分///////
	//////////////////////

	for (i = 1; i < MAIN_WIDTH - 1; i++)
	{
		if (g_mainMap[y][i] == SPACE ||
			g_mainMap[y][i] == CHAR_BAR)
			return false; //空白があったらfalseを返す
	}

	return true;
}

// ==============================================
// ラインを１行消して上のブロックを下に落とす関数
// ==============================================
void DeleteOneLine(int y)
{
	//////////////////////
	///////変数宣言///////
	//////////////////////

	int i, j;

	//////////////////////
	///////処理部分///////
	//////////////////////

	for (i = y; i > 0; i--)
	{
		for (j = 1; j < MAIN_WIDTH - 1; j++)
			g_mainMap[i][j] = g_mainMap[i - 1][j];
	}

}

// ==============================================
// すべての揃ったラインを消去する関数
// ==============================================
int DeleteMapLine()
{
	//////////////////////
	///////変数宣言///////
	//////////////////////

	int i;
	int DeleteCnt = 0;

	//////////////////////
	///////処理部分///////
	//////////////////////

	for (i = 0; i < MAIN_HEIGHT - 1; i++)
	{
		if (CheckLineFull(i))
		{
			DeleteOneLine(i);
			DeleteCnt++;
		}
	}

	return DeleteCnt;
}

// ==============================================
// テトリミノの回転処理を行う関数
// SRS(Super Rotation System)を行う関数
// ->テトリミノの回転を直感的に行えるように、
//   操作に補正を加える処理のこと
// ==============================================
void SuperRotationSystem(int key)
{
	//一度の回転につき、5つの配置パターンを判定して行う

	//後の処理のために回転前のアングルをコピーする
	int oldAngle = g_playerMino.angle;

	//パターン0.通常の回転

	if (key == 'p')
	{ //右回転
		g_playerMino.angle++;
		RotateShape(g_playerMino.basicInfo.shape, g_playerMino.basicInfo.type, true);
	}

	else if (key == 'o')
	{ //左回転
		g_playerMino.angle--;
		RotateShape(g_playerMino.basicInfo.shape, g_playerMino.basicInfo.type, false);

	}

	//閾値の設定(値を0~3でループさせる)
	g_playerMino.angle = (g_playerMino.angle + 4) % 4;

	if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x, g_playerMino.y))
	{ //移動可能であればここで修了、不可能の場合は続きのパターン1へ

		//値の初期化
		int deltaX = 0, deltaY = 0; //仮の移動量

		//ここからＩミノの場合と、それ以外で処理が異なる
		if (g_playerMino.basicInfo.type == 0)
		{ //Ｉミノのとき

			//パターン1.パターン0から図形を左右に動かす
			//	パターン0後の図形のアングルが
			//	1(右向き)の場合は右へ移動
			//	3(左向き)の場合は左へ移動
			//	0,2(上向き・下向き)の場合は回転前の逆の向きへ移動 
			//  (0のときは2マス移動)

			if (g_playerMino.angle == 1)
				deltaX = 1;

			else if (g_playerMino.angle == 3)
				deltaX = -1;

			else
			{
				if (key == 'o')
					deltaX = 1;
				else
					deltaX = -1;

				if (g_playerMino.angle == 0)
					deltaX *= 2; //0のときは2マス移動
			}

			int tempDeltaX1 = deltaX; //後のパターン判定で使用するため、一時的に保存する

			if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
			{ //移動可能であればここで決定、不可能の場合パターン2へ

				//パターン2.パターン0から図形を左右に動かす
				//	パターン0後の図形のアングルが
				//	1(右向き)の場合は左へ移動
				//	3(左向き)の場合は右へ移動
				//	0,2(上向き・下向き)の場合は回転前の逆の向きへ移動 
				//  (2のときは2マス移動)

				if (g_playerMino.angle == 1)
					deltaX = -1;

				else if (g_playerMino.angle == 3)
					deltaX = 1;

				else
				{
					if (key == 'o')
						deltaX = -1;
					else
						deltaX = 1;

					if (g_playerMino.angle == 2)
						deltaX *= 2; //2のときは2マス移動
				}

				int tempDeltaX2 = deltaX; //後のパターン判定で使用するため、一時的に保存する

				if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
				{ //移動可能であればここで決定、不可能の場合パターン3へ

					//パターン3.上下に動かす
					//	パターン0後の図形のアングルが
					//	1(右向き)の場合はパターン1後の図形を下へ移動
					//	3(左向き)の場合はパターン1後の図形を上へ移動
					//	0,2(上向き・下向き)の場合は
					//		回転前のアングルが1ならパターン1後の図形を上へ移動
					//		回転前のアングルが3ならパターン2後の図形を下へ移動
					//  左回転なら上下の移動量は2マスとなる

					if (g_playerMino.angle == 1)
					{
						deltaX = tempDeltaX1;
						deltaY = 1;
					}

					else if (g_playerMino.angle == 3)
					{
						deltaX = tempDeltaX1;
						deltaY = -1;
					}

					else
					{
						if (oldAngle == 1)
						{
							deltaX = tempDeltaX1;
							deltaY = -1;
						}

						else
						{
							deltaX = tempDeltaX2;
							deltaY = 1;
						}
					}

					if (key == 'o')
						deltaY *= 2; //左回転なら上下の移動量は2マスとなる

					if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
					{ //移動可能であればここで決定、不可能の場合パターン4へ

						//パターン4.上下に動かす
						//	パターン0後の図形のアングルが
						//	1(右向き)の場合はパターン2後の図形を上へ移動
						//	3(左向き)の場合はパターン2後の図形を下へ移動
						//	0,2(上向き・下向き)の場合は
						//		回転前のアングルが1ならパターン2後の図形を下へ移動
						//		回転前のアングルが3ならパターン1後の図形を上へ移動
						//  右回転なら上下の移動量は2マスとなる

						if (g_playerMino.angle == 1)
						{
							deltaX = tempDeltaX2;
							deltaY = -1;
						}

						else if (g_playerMino.angle == 3)
						{
							deltaX = tempDeltaX2;
							deltaY = 1;
						}

						else
						{
							if (oldAngle == 1)
							{
								deltaX = tempDeltaX2;
								deltaY = 1;
							}

							else
							{
								deltaX = tempDeltaX1;
								deltaY = -1;
							}
						}

						if (key == 'p')
							deltaY *= 2; //右回転なら上下の移動量は2マスとなる

						if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
						{ //これで動けなかったら移動も回転もキャンセルする
							deltaX = 0;
							deltaY = 0;

							if (key == 'p')
							{
								g_playerMino.angle--;
								RotateShape(g_playerMino.basicInfo.shape, g_playerMino.basicInfo.type, false); //回転を打ち消す
							}

							else if (key == 'o')
							{
								g_playerMino.angle++;
								RotateShape(g_playerMino.basicInfo.shape, g_playerMino.basicInfo.type, true); //回転を打ち消す
							}
						}
					}

				}
			}

		}

		else
		{ //Ｉミノ以外のとき

			//パターン1.パターン0から図形を左右に1マス動かす
			//	パターン0後の図形のアングルが
			//	1(右向き)の場合は左へ移動
			//	3(左向き)の場合は右へ移動
			//	0,2(上向き・下向き)の場合は回転前の逆の向きへ移動

			if (g_playerMino.angle == 1) //右向きのとき
				deltaX = -1;

			else if (g_playerMino.angle == 3) //左向きのとき
				deltaX = 1;

			else
			{ //上向き・下向きの時
				if (oldAngle == 1) //旧アングルが右のとき
					deltaX = 1;

				else					//旧アングルが左のとき
					deltaX = -1;
			}

			if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
			{ //移動可能であればここで決定、不可能の場合パターン2へ

				//パターン2.パターン1から上下に１マス動かす
				//	パターン0後の図形のアングルが
				//	1,3(右向き・左向き)の場合は上へ移動
				//	0,2(上向き・下向き)の場合は下へ移動

				if (g_playerMino.angle == 1 || g_playerMino.angle == 3)
					deltaY = -1;

				else
					deltaY = 1;

				if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
				{ //移動可能であればここで決定、不可能の場合パターン3へ

					//パターン3.パターン0から上下に２マス動かす
					//	1,3(右向き・左向き)の場合は下へ移動
					//	0,2(上向き・下向き)の場合は上へ移動

					//パターン0からなので変位をリセット
					deltaX = 0;
					deltaY = 0;

					if (g_playerMino.angle == 1 || g_playerMino.angle == 3)
						deltaY = -2;

					else
						deltaY = 2;

					if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
					{ //移動可能であればここで決定、不可能の場合パターン4へ

						//パターン4.パターン3から図形を左右に動かす
						//	パターン0後の図形のアングルが
						//	1(右向き)の場合は左
						//	3(左向き)の場合は右へ移動
						//	0,2(上向き・下向き)の場合は回転前の逆の向きへ移動

						if (g_playerMino.angle == 1)
							deltaX = -1;

						else if (g_playerMino.angle == 3)
							deltaX = 1;

						else
						{ //上向き・下向きのとき
							if (oldAngle == 1)
								deltaX = 1;

							else
								deltaX = -1;
						}

						if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
						{ //これで動けなかったら移動も回転もキャンセルする
							deltaX = 0;
							deltaY = 0;

							if (key == 'p')
							{
								g_playerMino.angle--;
								RotateShape(g_playerMino.basicInfo.shape, g_playerMino.basicInfo.type, false); //回転を打ち消す
							}

							else if (key == 'o')
							{
								g_playerMino.angle++;
								RotateShape(g_playerMino.basicInfo.shape, g_playerMino.basicInfo.type, true); //回転を打ち消す
							}
						}
					}
				}
			}
		}

		g_playerMino.x += deltaX;
		g_playerMino.y += deltaY;

		//閾値の設定(値を0~3でループさせる)
		g_playerMino.angle = (g_playerMino.angle + 4) % 4;
	}
}

// ==============================================
// ±90°図形を回転させる関数
// ==============================================
void RotateShape(char sourceShape[MINO_SIZE][MINO_SIZE], MinoType minoType, bool rotateClockwise)
{
	//単位円上の任意の点(x,y)は
	// x=cosθ,y=sinθと表せる
	// 90度時計回りに回転させた後の点(x′,y′)は
	// x′=cos(θ-90°)=cosθcos90°+sinθsin90°=sinθ=y
	// y′=sin(θ-90°)=sinθcos90°-cosθsin90°=-cosθ=-x
	// すなわち
	//①行と列を入れ替える(y=xを軸に対称移動)
	//②左右反転させる
	//これにより時計回りに90°回転したことになるので、これを用いて図形を回転させる
	//(反時計回りの場合は②のときに上下反転)

	//////////////////
	/////変数宣言/////
	//////////////////

	char tmp[MINO_SIZE][MINO_SIZE] = { 0 }; //上記の①の処理後のパターンを保存する一時的な配列

	//////////////////
	////図形の回転////
	//////////////////

	//Ｉミノだけ大きいので、Ｉミノとその他のミノで処理を分ける
	if (minoType == I)
	{ //Ｉミノのとき

		//①i=jを軸に対称移動
		for (int i = 0; i < MINO_SIZE; i++)
		{
			for (int j = 0; j < MINO_SIZE; j++)
			{
				tmp[i][j] = sourceShape[j][i];
			}
		}

		if (rotateClockwise)
		{ //時計回りの場合

			//②左右反転
			for (int i = 0; i < MINO_SIZE; i++)
			{
				for (int j = 0; j < MINO_SIZE; j++)
				{
					sourceShape[i][j] = tmp[i][MINO_SIZE - 1 - j];
				}
			}

		}

		else
		{ //反時計回りの場合
			//②上下反転
			for (int i = 0; i < MINO_SIZE; i++)
			{
				for (int j = 0; j < MINO_SIZE; j++)
				{
					sourceShape[i][j] = tmp[MINO_SIZE - 1 - i][j];
				}
			}
		}
	}

	else
	{ //Ｉミノ以外の時

		//①i=jを軸に対称移動
		for (int i = 0; i < MINO_SIZE - 1; i++)
		{
			for (int j = 0; j < MINO_SIZE - 1; j++)
			{
				tmp[i][j] = sourceShape[j][i];
			}
		}

		if (rotateClockwise)
		{ //時計回りの場合

			//②左右反転
			for (int i = 0; i < MINO_SIZE - 1; i++)
			{
				for (int j = 0; j < MINO_SIZE - 1; j++)
				{
					sourceShape[i][j] = tmp[i][MINO_SIZE - 2 - j];
				}
			}

		}

		else
		{ //反時計回りの場合
			//②上下反転
			for (int i = 0; i < MINO_SIZE - 1; i++)
			{
				for (int j = 0; j < MINO_SIZE - 1; j++)
				{
					sourceShape[i][j] = tmp[MINO_SIZE - 2 - i][j];
				}
			}
		}
	}

	return;
}

// ==============================================
// カーソルを移動させる関数
// ==============================================
void MoveCursor(int Y, int X) 
{
	//出力を全角文字で統一しているので、Xは2倍にする
	std::cout << "\033[" << Y + 1 << ";" << 2 * X + 1 << "H";
}

// ==============================================
// カーソルを非表示にする関数
// ==============================================
void DeleteCursor()
{
	HANDLE hOut;
	CONSOLE_CURSOR_INFO cci;

	// 出力用ハンドルの取得
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// CONSOLE_CURSOR_INFO構造体の現在の状態を取得する
	GetConsoleCursorInfo(hOut, &cci);

	// メンバ変数であるbVisibleがカーソルの表示・非表示を制御する変数なので、これをFALSEにする事でカーソルを非表示にできる
	cci.bVisible = FALSE;

	// 変更した構造体情報をコンソールWindowにセットする
	SetConsoleCursorInfo(hOut, &cci);
}