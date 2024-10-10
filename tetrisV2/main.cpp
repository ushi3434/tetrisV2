// ==============================================
// main.cpp [テトリスV2]
// 
// 制作者:牛丸隼人      日付:2024/10/10
// ==============================================

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
#define MINO_TYPE	(7)		//テトリミノの種類
#define CHUNK_SIZE	(4)		//１つのテトリミノのパターンを保存する領域(正方形)の一辺の長さ
#define COLOR_CYAN		(0)
#define COLOR_YELLOW	(1)
#define COLOR_GREEN		(2)
#define COLOR_RED		(3)
#define COLOR_BLUE		(4)
#define COLOR_WHITE		(5)
#define COLOR_MAGENTA	(6)

// ==============================================
// 構造体宣言
// ==============================================
struct MINO_ORIGIN
{
	int  color; //色
	char pattern[4][4]; //形
};

struct MINO_BLOCK
{
	int minoType;	//テトリミノの種類
	int x;			//X座標
	int y;			//Y座標
	int rotation;	//角度(90度ずつ)
};

// ==============================================
// メイン関数
// ==============================================
int main(void)
{

}

// ==============================================
// ゲームメイン関数
// ==============================================
int GameMain(int)
{
	//変数宣言
	char mainMap[MAIN_HEIGHT][MAIN_WIDTH];

}