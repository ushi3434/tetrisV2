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
#define COLOR_CYAN		(2)
#define COLOR_YELLOW	(3)
#define COLOR_GREEN		(4)
#define COLOR_RED		(5)
#define COLOR_BLUE		(6)
#define COLOR_WHITE		(7)
#define COLOR_MAGENTA	(8)
#define NEXT_QUEUE_SIZE (14) //次のテトリミノの種類を保存するキューのサイズ


//ゲームルール関連
#define GAMEOVER_LINE_HEIGHT (2) //ゲームオーバーラインのＹ座標
#define MINOSTART_X (4) //テトリミノのスポーン地点(X座標)
#define MINOSTART_Y (0) //テトリミノのスポーン地点(Y座標)

//文字出力関連
#define CHAR_NULL	(0)	//全角スペース
#define CHAR_SQUARE (1)	//■
#define CHAR_BAR	(9)	//━


// ==============================================
// 構造体宣言
// ==============================================
struct MAP
{
	char main[MAIN_HEIGHT][MAIN_WIDTH] = { 0 }; //メインマップ
	char next[NEXT_HEIGHT][NEXT_WIDTH] = { 0 }; //NEXTマップ
	char hold[HOLD_HEIGHT][HOLD_WIDTH] = { 0 }; //HOLDマップ
};

struct MINO
{
	char shape[MINO_SIZE][MINO_SIZE];	//パターン
	int  color;		//色
	int	 x;			//X座標
	int	 y;			//Y座標
	int  rotation;	//角度
};

struct NEXTMINO
{
	MINO minoQueue[NEXT_QUEUE_SIZE];
	int queueNum;					//キューの個数
};

// ==============================================
// プロトタイプ宣言
// ==============================================
int GameMain(int);
struct MAP	InitializeMap();
struct NEXTMINO InitializeNextMino();
struct MINO	CreateMino(int);
struct NEXTMINO AddNextMino(NEXTMINO);
void UpdateScreen(MAP, MINO);
bool CanMove(char[MAIN_HEIGHT][MAIN_WIDTH], char[MINO_SIZE][MINO_SIZE], int, int);
void OutputChar(char);
void ShuffleArray(int array[], int size);


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
	MINO holdMino;				//ホールドのミノを保存する
	bool canHoldFlg = false;		//ホールドが可能かどうか
	bool holdEmptyFlg = true;		//ホールドが空かどうか

	MAP	gameMap = InitializeMap();			//マップ生成
	NEXTMINO nextMino = InitializeNextMino();	//テトリミノの抽選
	MINO playerMino = nextMino.minoQueue[0]; //テトリミノのツモ
	
	UpdateScreen(gameMap, playerMino);

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
					if (CanMove(gameMap.main, playerMino.shape, playerMino.x, playerMino.y + 1))
						playerMino.y++;
					else
					{
						placeMinoFlg = true; //テトリミノ設置するコードは下にあるよ
						break;
					}
				}

			case 'a':
				//左移動
				if (CanMove(gameMap.main, playerMino.shape, playerMino.x - 1, playerMino.y))
					playerMino.x--;
				break;

			case 'd':
				//右移動
				if (CanMove(gameMap.main, playerMino.shape, playerMino.x + 1, playerMino.y))
					playerMino.x++;
				break;

			case 's':
				//下移動
				if (CanMove(gameMap.main, playerMino.shape, playerMino.x, playerMino.y + 1))
					playerMino.y++;
				break;

			case 0x1B:
				//ESCでタイトルに戻る
				return 2;

			case 'c':
			case 'l':

				break;

			}

			UpdateScreen(gameMap, playerMino);
		}


		
	}

	return 0;
}

// ==============================================
// マップ初期化関数
// ==============================================
struct MAP InitializeMap()
{
	MAP gameMap = { 0 };

	//メインマップに壁・地面を配置する
	for (int i = 0; i < MAIN_HEIGHT; i++)
	{
		for (int j = 0; j < MAIN_WIDTH; j++)
		{
			if (j == 0 || j == MAIN_WIDTH - 1 || i == MAIN_HEIGHT - 1)
				gameMap.main[i][j] = 1;
		}
	}

	//NEXTマップに外壁を配置する
	for (int i = 0; i < NEXT_HEIGHT; i++)
	{
		for (int j = 0; j < NEXT_WIDTH; j++)
		{
			if (j == 0 || j == NEXT_WIDTH - 1 ||
				i == 0 || i == NEXT_HEIGHT - 1)
				gameMap.next[i][j] = 1; //外壁	
		}
	}

	//HOLDマップに外壁を配置する
	for (int i = 0; i < HOLD_HEIGHT; i++)
	{
		for (int j = 0; j < HOLD_WIDTH; j++)
		{
			if (j == 0 || j == HOLD_WIDTH - 1 ||
				i == 0 || i == HOLD_HEIGHT - 1)
				gameMap.hold[i][j] = 1; //外壁	
		}
	}

	return gameMap;
}

// ==============================================
// ネクストミノ初期化関数
// ==============================================
struct NEXTMINO InitializeNextMino()
{
	NEXTMINO next = { 0 };
	
	for (int i = 0; i < 2; i++)
	{
		int data[MINO_MAX];

		for (int j = 0; j < MINO_MAX; j++)
		{
			data[j] = j;
		}

		ShuffleArray(data, MINO_MAX);

		for (int j = 0; j < MINO_MAX; j++)
		{
			next.minoQueue[next.queueNum] = CreateMino(data[j]);
			next.queueNum++;
		}
	}
	return next;
}

// ==============================================
// ネクストミノ抽選関数
// ==============================================
struct NEXTMINO AddNextMino(NEXTMINO next)
{
	int data[MINO_MAX];

	for (int j = 0; j < MINO_MAX; j++)
	{
		data[j] = j;
	}

	ShuffleArray(data, MINO_MAX);

	for (int j = 0; j < MINO_MAX; j++)
	{
		next.minoQueue[next.queueNum] = CreateMino(data[j]);
		next.queueNum++;
	}

	return next;
}

// ==============================================
// テトリミノデータ変更関数
// ==============================================
struct MINO	CreateMino(int minoType)
{
	MINO mino = { 0 };

	switch (minoType)
	{

	case MINO_I:
		mino.shape[1][0] = 1;
		mino.shape[1][1] = 1;
		mino.shape[1][2] = 1;
		mino.shape[1][3] = 1;
		mino.color = COLOR_CYAN;
		break;

	case MINO_O:
		mino.shape[1][1] = 1;
		mino.shape[1][2] = 1;
		mino.shape[2][1] = 1;
		mino.shape[2][2] = 1;
		mino.color = COLOR_YELLOW;
		break;

	case MINO_S:
		mino.shape[0][1] = 1;
		mino.shape[0][2] = 1;
		mino.shape[1][0] = 1;
		mino.shape[1][1] = 1;
		mino.color = COLOR_GREEN;
		break;

	case MINO_Z:
		mino.shape[0][0] = 1;
		mino.shape[0][1] = 1;
		mino.shape[1][1] = 1;
		mino.shape[1][2] = 1;
		mino.color = COLOR_RED;
		break;

	case MINO_J:
		mino.shape[0][0] = 1;
		mino.shape[1][0] = 1;
		mino.shape[1][1] = 1;
		mino.shape[1][2] = 1;
		mino.color = COLOR_BLUE;
		break;

	case MINO_L:
		mino.shape[0][2] = 1;
		mino.shape[1][0] = 1;
		mino.shape[1][1] = 1;
		mino.shape[1][2] = 1;
		mino.color = COLOR_WHITE;
		break;

	case MINO_T:
		mino.shape[0][1] = 1;
		mino.shape[1][0] = 1;
		mino.shape[1][1] = 1;
		mino.shape[1][2] = 1;
		mino.color = COLOR_MAGENTA;
		break;
	}
	
	mino.x = MINOSTART_X;
	mino.y = MINOSTART_Y;
	mino.rotation = 0;

	return mino;
}

// ==============================================
// 画面出力関数
// ==============================================
void UpdateScreen(MAP gameMap, MINO minoData)
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
			displayBuffer[i][j + HOLD_WIDTH - 1] = gameMap.main[i][j];
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
			if (minoData.shape[i][j] != 0)
				displayBuffer[i + minoData.y][j + HOLD_WIDTH - 1 + minoData.x] = minoData.color;
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
bool CanMove(char mainMap[MAIN_HEIGHT][MAIN_WIDTH], char sourceShape[MINO_SIZE][MINO_SIZE], int futurePosX, int futurePosY)
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
			if (sourceShape[i][j] != 0 &&					//チャンクの中でブロックがある場所が
				mainMap[futurePosY + i][futurePosX + j] != 0)	//移動先のmap上で空白(0)じゃなかったら
				return false;									//falseを返す
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
	case CHAR_NULL:
		std::cout << "  ";
		break;

	case CHAR_SQUARE:
		std::cout << "\x1b[39m■\x1b[39m";
		break;

	case COLOR_CYAN:
		std::cout << "\x1b[36m■";
		break;

	case COLOR_YELLOW:
		std::cout << "\x1b[33m■";
		break;

	case COLOR_GREEN:
		std::cout << "\x1b[32m■";
		break;

	case COLOR_RED:
		std::cout << "\x1b[31m■";
		break;

	case COLOR_BLUE:
		std::cout << "\x1b[34m■";
		break;

	case COLOR_WHITE:
		std::cout << "\x1b[37m■";
		break;

	case COLOR_MAGENTA:
		std::cout << "\x1b[35m■";
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