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
#define MINO_MAX	(7)		//テトリミノの種類
#define MINO_I		(0)		//Iミノ
#define MINO_O		(1)		//Oミノ
#define MINO_S		(2)		//Sミノ
#define MINO_Z		(3)		//Zミノ
#define MINO_J		(4)		//Jミノ
#define MINO_L		(5)		//Lミノ
#define MINO_T		(6)		//Tミノ
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
#define CHAR_SPACE			(0)	//全角スペース
#define CHAR_BLOCK			(1)	//■(無色)
#define CHAR_BLOCK_CYAN		(2) //■色違い
#define CHAR_BLOCK_YELLOW	(3)
#define CHAR_BLOCK_GREEN	(4)
#define CHAR_BLOCK_RED		(5)
#define CHAR_BLOCK_BLUE		(6)
#define CHAR_BLOCK_WHITE	(7)
#define CHAR_BLOCK_MAGENTA	(8)
#define CHAR_BAR			(9)	//━
#define STR_HOLD0			(30)//文字列
#define STR_HOLD1			(31)
#define STR_NEXT0			(32)



// ==============================================
// 構造体宣言
// ==============================================
struct BASIC_MINO
{
	int type;		//テトリミノのタイプ
	char shape[MINO_SIZE][MINO_SIZE];	//テトリミノの形
};

struct GAME_MINO
{
	BASIC_MINO basicInfo; //形状
	int rotation;	//角度
	int	x;			//X座標
	int	y;			//Y座標
};

struct NEXTQUEUE
{
	BASIC_MINO mino[NEXT_QUEUE_SIZE];
	int queueNum;					//キューの個数
};

struct STR_DICTIONARY
{
	char hold0[STR_SIZE];
	char hold1[STR_SIZE];
	char next0[STR_SIZE];
};

// ==============================================
// グローバル変数宣言
// ==============================================

char g_mainMap[MAIN_HEIGHT][MAIN_WIDTH] = { 0 }; //メインマップ
char g_nextMap[NEXT_HEIGHT][NEXT_WIDTH] = { 0 }; //NEXTマップ
char g_holdMap[HOLD_HEIGHT][HOLD_WIDTH] = { 0 }; //HOLDマップ
GAME_MINO g_playerMino;		//プレイヤーの動かすテトリミノ
NEXTQUEUE g_nextMinos;		//ネクストの列
STR_DICTIONARY strDictionary; //文字列データの辞書

// ==============================================
// プロトタイプ宣言
// ==============================================
int	GameMain(int);
void InitializeMap();
void InitializeNextMino();
void AddNextQueue();
struct BASIC_MINO CreateMino(int);
void ChangePlayerMino();
void ChangePlayerMino(BASIC_MINO);
void RenderScreen();
bool CanMove(char[MINO_SIZE][MINO_SIZE], int, int);
void OutputChar(char);
void ShuffleArray(int array[], int size);
void UpdateNextMap();
void UpdateHoldMap(BASIC_MINO, bool);

// ==============================================
// メイン関数
// ==============================================
int main(void)
{
	srand((unsigned int)time(NULL));

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
	BASIC_MINO holdMino = { 0 };		//ホールドのミノ

	InitializeMap();		//マップ生成
	InitializeNextMino();	//ネクストミノ生成
	ChangePlayerMino();		//ネクスト先頭のミノをセット
	UpdateNextMap();		//ネクスト描画更新
	
	RenderScreen();			//画面出力

	//////////////////////////////
	/////////ゲームループ/////////
	//////////////////////////////
	while (true)
	{
		////////////////////////////////
		//////キーボード操作の実装//////
		////////////////////////////////

		if (_kbhit())
		{
			switch (_getch())
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

						ChangePlayerMino(); //次のテトリミノに変更

						if (NEXT_QUEUE_SIZE - g_nextMinos.queueNum >= 7)
						{	//ネクストの列の空き数が7個になったら
							//ネクストを補充する
							AddNextQueue(); //一度の補充で7個のミノを作る
						}
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
			}
			RenderScreen();
		}
		
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

	//ゲームオーバーラインの書き込み
	for (int i = 0; i < MAIN_WIDTH - 2; i++)
	{
		g_mainMap[GAMEOVER_LINE_HEIGHT][i + 1] = CHAR_BAR;
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
	strcpy(strDictionary.next0, "ＮＥＸＴ");
	g_nextMap[1][1] = STR_NEXT0;
	for (int i = 0; i < strlen(strDictionary.next0) / 2 - 1; i++)
	{
		g_nextMap[1][2 + i] = -1;
	}

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
				g_holdMap[i][j] = 1; //外壁	
		}
	}

	//文字列の配置
	
	strcpy(strDictionary.hold0, "ＨＯＬＤ");
	g_holdMap[1][1] = STR_HOLD0;
	for (int i = 0; i < strlen(strDictionary.hold0) / 2 - 1; i++)
	{
		g_holdMap[1][2 + i] = -1;
	}

	strcpy(strDictionary.hold1, "ＯＫ　　");
	g_holdMap[5][1] = STR_HOLD1;
	for (int i = 0; i < strlen(strDictionary.hold1) / 2 - 1; i++)
	{
		g_holdMap[5][2 + i] = -1;
	}

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
		g_nextMinos.mino[g_nextMinos.queueNum] = CreateMino(data[j]);
		g_nextMinos.queueNum++;
	}

}

// ==============================================
// 操作するテトリミノの変更を行う関数
// 引数なし:ネクスト先頭のミノに変更する
// 引数あり:引数に渡したミノに変更する
// ==============================================
void ChangePlayerMino()
{
	g_playerMino.basicInfo = g_nextMinos.mino[0]; //NEXTの先頭のテトリミノを除列

	g_nextMinos.queueNum--; //除列したので-1

	for (int i = 0; i < g_nextMinos.queueNum; i++)
	{
		g_nextMinos.mino[i] = g_nextMinos.mino[i + 1]; //すべてのNEXTミノを１個前に進める
	}

	//座標・回転の初期化
	g_playerMino.x = MINOSTART_X;
	g_playerMino.y = MINOSTART_Y;
	g_playerMino.rotation = 0;
}

// ==============================================
// 操作するテトリミノの変更を行う関数
// 引数なし:ネクスト先頭のミノに変更する
// 引数あり:引数に渡したミノに変更する
// ==============================================
void ChangePlayerMino(BASIC_MINO changeMino)
{
	g_playerMino.basicInfo = changeMino;

	//座標・回転の初期化
	g_playerMino.x = MINOSTART_X;
	g_playerMino.y = MINOSTART_Y;
	g_playerMino.rotation = 0;
}

// ==============================================
// テトリミノ作成関数
// ==============================================
struct BASIC_MINO CreateMino(int minoType)
{
	BASIC_MINO basicMino = { 0 };

	basicMino.type = minoType; //タイプ情報の更新
	
	//形の情報の更新
	switch (basicMino.type)
	{

	case MINO_I:
		basicMino.shape[1][0] = CHAR_BLOCK_CYAN;
		basicMino.shape[1][1] = CHAR_BLOCK_CYAN;
		basicMino.shape[1][2] = CHAR_BLOCK_CYAN;
		basicMino.shape[1][3] = CHAR_BLOCK_CYAN;
		break;

	case MINO_O:
		basicMino.shape[1][1] = CHAR_BLOCK_YELLOW;
		basicMino.shape[1][2] = CHAR_BLOCK_YELLOW;
		basicMino.shape[2][1] = CHAR_BLOCK_YELLOW;
		basicMino.shape[2][2] = CHAR_BLOCK_YELLOW;
		break;

	case MINO_S:
		basicMino.shape[0][1] = CHAR_BLOCK_GREEN;
		basicMino.shape[0][2] = CHAR_BLOCK_GREEN;
		basicMino.shape[1][0] = CHAR_BLOCK_GREEN;
		basicMino.shape[1][1] = CHAR_BLOCK_GREEN;
		break;

	case MINO_Z:
		basicMino.shape[0][0] = CHAR_BLOCK_RED;
		basicMino.shape[0][1] = CHAR_BLOCK_RED;
		basicMino.shape[1][1] = CHAR_BLOCK_RED;
		basicMino.shape[1][2] = CHAR_BLOCK_RED;
		break;

	case MINO_J:
		basicMino.shape[0][0] = CHAR_BLOCK_BLUE;
		basicMino.shape[1][0] = CHAR_BLOCK_BLUE;
		basicMino.shape[1][1] = CHAR_BLOCK_BLUE;
		basicMino.shape[1][2] = CHAR_BLOCK_BLUE;
		break;

	case MINO_L:
		basicMino.shape[0][2] = CHAR_BLOCK_WHITE;
		basicMino.shape[1][0] = CHAR_BLOCK_WHITE;
		basicMino.shape[1][1] = CHAR_BLOCK_WHITE;
		basicMino.shape[1][2] = CHAR_BLOCK_WHITE;
		break;

	case MINO_T:
		basicMino.shape[0][1] = CHAR_BLOCK_MAGENTA;
		basicMino.shape[1][0] = CHAR_BLOCK_MAGENTA;
		basicMino.shape[1][1] = CHAR_BLOCK_MAGENTA;
		basicMino.shape[1][2] = CHAR_BLOCK_MAGENTA;
		break;
	}

	return basicMino;
}

// ==============================================
// 画面出力関数
// ==============================================
void RenderScreen()
{
	char displayBuffer[WINDOW_HEIGHT][WINDOW_WIDTH] = { 0 };

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

	system("cls"); //画面消去

	for (int i = 0; i < WINDOW_OFFSET_Y; i++)
		std::cout << "\n";

	for (int i = 0; i < WINDOW_HEIGHT; i++)
	{
		for (int j = 0; j < WINDOW_OFFSET_X; j++)
			std::cout << " ";


		for (int j = 0; j < WINDOW_WIDTH; j++)
		{
			OutputChar(displayBuffer[i][j]);
		}

		std::cout << "\n";
	}

	std::cout << "\n";
	std::cout << "　　　　　　A:ひだり D:みぎ   S:した       W:ハードドロップ\n";
	std::cout << "　　　　　　O:左回転 P:右回転 C/L:ホールド ESC:タイトルにもどる\n";

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
				g_mainMap[futurePosY + i][futurePosX + j] != CHAR_SPACE &&
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
	case CHAR_SPACE:
		std::cout << "  ";
		break;

	case CHAR_BLOCK:
		std::cout << "\x1b[39m■\x1b[39m";
		break;

	case CHAR_BLOCK_CYAN:
		std::cout << "\x1b[36m■";
		break;

	case CHAR_BLOCK_YELLOW:
		std::cout << "\x1b[33m■";
		break;

	case CHAR_BLOCK_GREEN:
		std::cout << "\x1b[32m■";
		break;

	case CHAR_BLOCK_RED:
		std::cout << "\x1b[31m■";
		break;

	case CHAR_BLOCK_BLUE:
		std::cout << "\x1b[34m■";
		break;

	case CHAR_BLOCK_WHITE:
		std::cout << "\x1b[37m■";
		break;

	case CHAR_BLOCK_MAGENTA:
		std::cout << "\x1b[35m■";
		break;

	case CHAR_BAR:
		std::cout << "\x1b[31m━━\x1b[39m";
		break;
	
	case STR_HOLD0:
		std::cout << strDictionary.hold0;
		break;

	case STR_HOLD1:
		std::cout << strDictionary.hold1;
		break;

	case STR_NEXT0:
		std::cout << strDictionary.next0;
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
	if(canHoldFlg)
		strcpy(strDictionary.hold1, "ＯＫ　　");
	else
		strcpy(strDictionary.hold1, "ＮＧ　　");

}