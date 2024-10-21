#define _CRT_SECURE_NO_WARNINGS
// ==============================================
// main.cpp [�e�g���XV2]
// 
// �����:ushi34      ���t:2024/10/10
// 
// X���W�͉E��������
// Y���W�͉���������
// ���オ���_
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
// �}�N����`
// ==============================================

//��ʕ\���֘A
#define WINDOW_WIDTH	(23)	//�E�B���h�E�̕�
#define WINDOW_HEIGHT	(24)	//�E�B���h�E�̍���
#define WINDOW_OFFSET_X	(15)	//�E�B���h�E�ꏊ�����̂��߂̍����ɋ󂯂��̐�(���p)
#define WINDOW_OFFSET_Y	(2)		//�E�B���h�E�ꏊ�����̂��߂̏㑤�ɋ󂯂�s�̐�
#define MAIN_WIDTH	(12)	//���C���}�b�v�̕�
#define MAIN_HEIGHT	(24)	//���C���}�b�v�̍���
#define NEXT_WIDTH	(6)		//NEXT�}�b�v�̕�
#define NEXT_HEIGHT	(19)	//NEXT�}�b�v�̍���
#define HOLD_WIDTH	(6)		//HOLD�}�b�v�̕�
#define HOLD_HEIGHT	(7)		//HOLD�}�b�v�̍���

//�e�g���~�m�֘A
#define MINO_MAX	(7)		//�e�g���~�m�̎��
#define MINO_I		(0)		//I�~�m
#define MINO_O		(1)		//O�~�m
#define MINO_S		(2)		//S�~�m
#define MINO_Z		(3)		//Z�~�m
#define MINO_J		(4)		//J�~�m
#define MINO_L		(5)		//L�~�m
#define MINO_T		(6)		//T�~�m
#define MINO_SIZE	(4)		//�P�̃e�g���~�m�̃p�^�[����ۑ�����̈�(�����`)�̈�ӂ̒���
#define NEXT_QUEUE_SIZE (14) //���̃e�g���~�m�̎�ނ�ۑ�����L���[�̃T�C�Y

//�Q�[�����[���֘A
#define GAMEOVER_LINE_HEIGHT (2) //�Q�[���I�[�o�[���C���̂x���W
#define MINOSTART_X (4) //�e�g���~�m�̃X�|�[���n�_(X���W)
#define MINOSTART_Y (0) //�e�g���~�m�̃X�|�[���n�_(Y���W)

//UI�֘A
#define MAP_MAIN (0)	//���C���}�b�v
#define MAP_NEXT (1)	//NEXT�}�b�v
#define MAP_HOLD (2)	//HOLD�}�b�v
#define NEXT_DISPLAY_NUM (4) //�l�N�X�g�~�m�̉�ʕ\���ő吔
#define STR_SIZE (32)	 //������̃T�C�Y

//�����o�͊֘A
#define CHAR_SPACE			(0)	//�S�p�X�y�[�X�̃L�[
#define CHAR_WALL			(1)	//��(���F)
#define CHAR_BLOCK_CYAN		(2) //���F�Ⴂ
#define CHAR_BLOCK_YELLOW	(3)
#define CHAR_BLOCK_GREEN	(4)
#define CHAR_BLOCK_RED		(5)
#define CHAR_BLOCK_BLUE		(6)
#define CHAR_BLOCK_WHITE	(7)
#define CHAR_BLOCK_MAGENTA	(8)
#define GHOSTBLOCK_OFFSET	(10) //�S�[�X�g�u���b�N�̃I�t�Z�b�g
#define CHAR_BAR			(9)	 //��
#define STRPOS_HOLD0_X		(1)  //������(HOLD)���o�͂����
#define STRPOS_HOLD0_Y		(1)  //������(HOLD)���o�͂���s



// ==============================================
// �\���̐錾
// ==============================================
struct BASIC_MINO
{
	int type;		//�e�g���~�m�̃^�C�v
	char shape[MINO_SIZE][MINO_SIZE];	//�e�g���~�m�̌`
};

struct GAME_MINO
{
	BASIC_MINO basicInfo; //�`��
	int angle;		//�p�x(0:��,1:�E,2:��,3:��)
	int	x;			//X���W
	int	y;			//Y���W
};

struct NEXTQUEUE
{
	BASIC_MINO mino[NEXT_QUEUE_SIZE];
	int queueNum;					//�L���[�̌�
};


// ==============================================
// �O���[�o���ϐ��錾
// ==============================================

long long g_frameCount = 0;

char g_mainMap[MAIN_HEIGHT][MAIN_WIDTH] = { 0 }; //���C���}�b�v
char g_nextMap[NEXT_HEIGHT][NEXT_WIDTH] = { 0 }; //NEXT�}�b�v
char g_holdMap[HOLD_HEIGHT][HOLD_WIDTH] = { 0 }; //HOLD�}�b�v
GAME_MINO g_playerMino;		//�v���C���[�̓������e�g���~�m
NEXTQUEUE g_nextMinos;		//�l�N�X�g�̗�

// ==============================================
// �v���g�^�C�v�錾
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
bool CheckLineFull(int);
void DeleteOneLine(int);
int  DeleteMapLine();
void SuperRotationSystem(int);
void RotateShape(char[MINO_SIZE][MINO_SIZE], int, bool);
void MoveCursor(int, int);


// ==============================================
// ���C���֐�
// ==============================================
int main(void)
{
	srand((unsigned int)time(NULL)); //����������

	GameMain(1);
}

// ==============================================
// �Q�[�����C���֐�
// ==============================================
int GameMain(int)
{
	//////////////////////////////
	////////////������////////////
	//////////////////////////////

	//�ϐ��錾
	bool placeMinoFlg = false;	//�e�g���~�m�ݒu�������s�����ǂ���
	bool canHoldFlg = true;		//�z�[���h���\���ǂ���
	bool holdEmptyFlg = true;	//�z�[���h���󂩂ǂ���
	BASIC_MINO holdMino = { 0 };	//�z�[���h�̃~�m
	int deletedLines = 0;			//���C��������
	int fallingInterval = 50;		//�����Ԋu
	int timerSec = 0;
	int timerMin = 0;

	InitializeMap();		//�}�b�v����
	InitializeNextMino();	//�l�N�X�g�~�m����
	ChangePlayerMino();		//�l�N�X�g�擪�̃~�m���Z�b�g
	UpdateNextMap();		//�l�N�X�g�`��X�V
	RenderScreen();			//��ʏo��

	//////////////////////////////
	/////////�Q�[�����[�v/////////
	//////////////////////////////
	while (true)
	{
		////////////////////////////////
		///////�e�g���~�m���R����///////
		////////////////////////////////
		if (g_frameCount % fallingInterval == 0)
		{
			if (CanMove(g_playerMino.basicInfo.shape, g_playerMino.x, g_playerMino.y + 1))
				g_playerMino.y++;
			else
				placeMinoFlg = true; //�e�g���~�m�ݒu����R�[�h�͉��ɂ����	
		}

		////////////////////////////////
		//////�L�[�{�[�h����̎���//////
		////////////////////////////////

		if (_kbhit())
		{
			int key = _getch();
			switch (key)
			{
			case 'w':
				//�n�[�h�h���b�v
				while (true)
				{
					if (CanMove(g_playerMino.basicInfo.shape, g_playerMino.x, g_playerMino.y + 1))
						g_playerMino.y++;
					else
					{
						placeMinoFlg = true; //�e�g���~�m�ݒu����R�[�h�͉��ɂ����
						break;
					}
				}
				break;

			case 'a':
				//���ړ�
				if (CanMove(g_playerMino.basicInfo.shape, g_playerMino.x - 1, g_playerMino.y))
					g_playerMino.x--;
				break;

			case 'd':
				//�E�ړ�
				if (CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + 1, g_playerMino.y))
					g_playerMino.x++;
				break;

			case 's':
				//���ړ�
				if (CanMove(g_playerMino.basicInfo.shape, g_playerMino.x, g_playerMino.y + 1))
					g_playerMino.y++;
				else
					placeMinoFlg = true; //�e�g���~�m�ݒu����R�[�h�͉��ɂ����
				break;

			case 0x1B:
				//ESC�Ń^�C�g���ɖ߂�
				return 2;

			case 'c':
			case 'l':
				//�z�[���h����
				if (canHoldFlg)
				{
					if (holdEmptyFlg)
					{	
						//�v���C���[�̃e�g���~�m��HOLD�ɂ���
						holdMino = CreateMino(g_playerMino.basicInfo.type);

						ChangePlayerMino(); //���̃e�g���~�m�ɕύX

						if (NEXT_QUEUE_SIZE - g_nextMinos.queueNum >= 7)
						{	//�l�N�X�g�̗�̋󂫐���7�ɂȂ�����
							//�l�N�X�g���[����
							AddNextQueue(); //��x�̕�[��7�̃~�m�����
						}

						holdEmptyFlg = false;
					}
					else
					{
						//�z�[���h�~�m��temp�ɕۑ�
						BASIC_MINO holdTemp = holdMino;

						//�v���C���[�̃e�g���~�m��HOLD�ɂ���
						holdMino = CreateMino(g_playerMino.basicInfo.type);

						//�v���C���[�̃~�m��temp�̃~�m�ɕύX
						ChangePlayerMino(holdTemp);
					}

					canHoldFlg = false; //�z�[���h���g�p�����̂�false�ɂ���

					UpdateHoldMap(holdMino, canHoldFlg); //HOLD�}�b�v�̕`��X�V
				}
				break;

			case 'p':
			case 'o':
				if (g_playerMino.basicInfo.type != MINO_O)
					//O�~�m�͉�]���Ȃ��̂Ŗ���
					SuperRotationSystem(key);
			}

		}
		
		////////////////////////////////
		///////�e�g���~�m�ݒu����///////
		////////////////////////////////
		if (placeMinoFlg)
		{ //���܂ł̏����Őݒu�t���O���l�����Ă��ꍇ�A�ݒu�������s��

			//�}�b�v�ɑ��쒆�̃e�g���~�m����������
			for (int i = 0; i < MINO_SIZE; i++)
			{
				for (int j = 0; j < MINO_SIZE; j++)
				{
					if (g_playerMino.basicInfo.shape[i][j] != 0)
						g_mainMap[i + g_playerMino.y][j + g_playerMino.x] = g_playerMino.basicInfo.shape[i][j];
				}
			}

			//���������C���̏���
			deletedLines += DeleteMapLine();

			ChangePlayerMino();			

			if (NEXT_QUEUE_SIZE - g_nextMinos.queueNum >= 7)
			{
				AddNextQueue();
			}

			UpdateNextMap();

			canHoldFlg = true;

			UpdateHoldMap(holdMino, canHoldFlg);

			placeMinoFlg = false;
		}

		////////////////////////////////
		////////�t���[�����䏈��////////
		////////////////////////////////

		g_frameCount++;
		RenderScreen();
		MoveCursor(0, 0);
		Sleep(16);

		////////////////////////////////
		//////////�^�C�}�[����//////////
		////////////////////////////////

		if (g_frameCount % 62 == 0)
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
// �}�b�v�������֐�
// ==============================================
void InitializeMap()
{
	////////////////
	//���C���}�b�v//
	////////////////

	//�ǁE�n�ʂ�z�u����
	for (int i = 0; i < MAIN_HEIGHT; i++)
	{
		for (int j = 0; j < MAIN_WIDTH; j++)
		{
			if (j == 0 || j == MAIN_WIDTH - 1 || i == MAIN_HEIGHT - 1)
				g_mainMap[i][j] = 1;
		}
	}

	//�Q�[���I�[�o�[���C���̏�������
	for (int i = 0; i < MAIN_WIDTH - 2; i++)
	{
		g_mainMap[GAMEOVER_LINE_HEIGHT][i + 1] = CHAR_BAR;
	}

	////////////////
	///NEXT�}�b�v///
	////////////////

	//�O�ǂ�z�u����
	for (int i = 0; i < NEXT_HEIGHT; i++)
	{
		for (int j = 0; j < NEXT_WIDTH; j++)
		{
			if (j == 0 || j == NEXT_WIDTH - 1 ||
				i == 0 || i == NEXT_HEIGHT - 1)
				g_nextMap[i][j] = 1; //�O��	
		}
	}

	//������̔z�u

	MoveCursor(1, 17);
	std::cout << "�m�d�w�s";
	

	////////////////
	///HOLD�}�b�v///
	////////////////

	//�O�ǂ�z�u����
	for (int i = 0; i < HOLD_HEIGHT; i++)
	{
		for (int j = 0; j < HOLD_WIDTH; j++)
		{
			if (j == 0 || j == HOLD_WIDTH - 1 ||
				i == 0 || i == HOLD_HEIGHT - 1)
				g_holdMap[i][j] = 1; //�O��	
		}
	}

	//������̔z�u
	MoveCursor(1, 1);
	std::cout << "�g�n�k�c";
	MoveCursor(5, 1);
	std::cout << "�n�j";

}

// ==============================================
// �l�N�X�g�~�m�������֐�
// ==============================================
void InitializeNextMino()
{
	for (int i = 0; i < 2; i++)
	{
		AddNextQueue();
	}
}

// ==============================================
// �l�N�X�g�~�m���I�֐�
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
// ���삷��e�g���~�m�̕ύX���s���֐�
// �����Ȃ�:�l�N�X�g�擪�̃~�m�ɕύX����
// ��������:�����ɓn�����~�m�ɕύX����
// ==============================================
void ChangePlayerMino()
{
	g_playerMino.basicInfo = g_nextMinos.mino[0]; //NEXT�̐擪�̃e�g���~�m������

	g_nextMinos.queueNum--; //���񂵂��̂�-1

	for (int i = 0; i < g_nextMinos.queueNum; i++)
	{
		g_nextMinos.mino[i] = g_nextMinos.mino[i + 1]; //���ׂĂ�NEXT�~�m���P�O�ɐi�߂�
	}

	//���W�E��]�̏�����
	g_playerMino.x = MINOSTART_X;
	g_playerMino.y = MINOSTART_Y;
	g_playerMino.angle = 0;
}

// ==============================================
// ���삷��e�g���~�m�̕ύX���s���֐�
// �����Ȃ�:�l�N�X�g�擪�̃~�m�ɕύX����
// ��������:�����ɓn�����~�m�ɕύX����
// ==============================================
void ChangePlayerMino(BASIC_MINO changeMino)
{
	g_playerMino.basicInfo = changeMino;

	//���W�E��]�̏�����
	g_playerMino.x = MINOSTART_X;
	g_playerMino.y = MINOSTART_Y;
	g_playerMino.angle = 0;
}

// ==============================================
// �e�g���~�m�쐬�֐�
// ==============================================
struct BASIC_MINO CreateMino(int minoType)
{
	BASIC_MINO basicMino = { 0 };

	basicMino.type = minoType; //�^�C�v���̍X�V
	
	//�`�̏��̍X�V
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
// ��ʏo�͊֐�
// ==============================================
void RenderScreen()
{
	static char displayData[WINDOW_HEIGHT][WINDOW_WIDTH] = { 0 };
	static char displayBuffer[WINDOW_HEIGHT][WINDOW_WIDTH] = { 0 };

	//////////////////
	//�z�����������//
	//////////////////

	//���C���}�b�v�̏�������
	for (int i = 0; i < MAIN_HEIGHT; i++)
	{
		for (int j = 0; j < MAIN_WIDTH; j++)
		{
			displayBuffer[i][j + HOLD_WIDTH - 1] = g_mainMap[i][j];
		}
	}

	//���쒆�̃e�g���~�m�̌��ݒn����^���̃u���b�N�܂ł̋������v�Z����
	int minoMargin = 0;

	while (true)
	{
		if (CanMove(g_playerMino.basicInfo.shape,g_playerMino.x, g_playerMino.y + 1 + minoMargin))
			minoMargin++;
		else
			break;
	}

	//���߂�minoMargin���g���āA���쒆�̃e�g���~�m�̃S�[�X�g�u���b�N�̏�������

	for (int i = 0; i < MINO_SIZE; i++)
	{
		for (int j = 0; j < MINO_SIZE; j++)
		{
			if (g_playerMino.basicInfo.shape[i][j] != 0)
				displayBuffer[minoMargin + i + g_playerMino.y][j + HOLD_WIDTH - 1 + g_playerMino.x] = GHOSTBLOCK_OFFSET +  g_playerMino.basicInfo.shape[i][j];
		}
	}
	

	//���쒆�̃e�g���~�m�̏�������
	for (int i = 0; i < MINO_SIZE; i++)
	{
		for (int j = 0; j < MINO_SIZE; j++)
		{
			if (g_playerMino.basicInfo.shape[i][j] != 0)
				displayBuffer[i + g_playerMino.y][j + HOLD_WIDTH - 1 + g_playerMino.x] = g_playerMino.basicInfo.shape[i][j];
		}
	}

	//NEXT�}�b�v�̏�������
	for (int i = 0; i < NEXT_HEIGHT; i++)
	{
		for (int j = 0; j < NEXT_WIDTH; j++)
		{
			displayBuffer[i][j + NEXT_WIDTH + MAIN_WIDTH - 2] = g_nextMap[i][j];
		}
	}

	//HOLD�}�b�v�̏�������
	for (int i = 0; i < HOLD_HEIGHT; i++)
	{
		for (int j = 0; j < HOLD_WIDTH; j++)
		{
			displayBuffer[i][j] = g_holdMap[i][j];
		}
	}



	//////////////////
	////��ʂ̏o��////
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
	std::cout << "�@�@�@�@�@�@A:�Ђ��� D:�݂�   S:����       W:�n�[�h�h���b�v\n";
	std::cout << "�@�@�@�@�@�@O:����] P:�E��] C/L:�z�[���h ESC:�^�C�g���ɂ��ǂ�\n";*/

}

// ==============================================
// �ړ��\�����肷��֐�
// ==============================================
bool CanMove(char sourceShape[MINO_SIZE][MINO_SIZE], int futurePosX, int futurePosY)
{
	//////////////////////
	///////�ϐ��錾///////
	//////////////////////

	int i, j; //���������p

	//////////////////////
	/////�����邩����/////
	//////////////////////

	//�����̃e�g���~�m�̓����蔻����v�Z���āA�Փ˂𔻒肷��

	for (i = 0; i < MINO_SIZE; i++)
	{
		for (j = 0; j < MINO_SIZE; j++)
		{
			//�e�g���~�m�̒��Ńu���b�N������ꏊ��
			//��(0)�������̓Q�[���I�[�o�[���C��(9)����Ȃ�������
			//false��Ԃ�
			if (sourceShape[i][j] != 0 &&
				g_mainMap[futurePosY + i][futurePosX + j] != CHAR_SPACE &&
				g_mainMap[futurePosY + i][futurePosX + j] != CHAR_BAR)
				return false;
		}
	}

	return true;

}

// ==============================================
// �������o�͂���֐�
// ==============================================
void OutputChar(char key)
{
	switch (key)
	{
	case CHAR_SPACE:
		std::cout << "  ";
		break;

	case CHAR_WALL:
		std::cout << "\x1b[47m�@\x1b[49m";
		break;

	case CHAR_BLOCK_CYAN:
		std::cout << "\x1b[36m��\x1b[39m";
		break;

	case CHAR_BLOCK_CYAN + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[36m��\x1b[39m\x1b[0m";
		break;

	case CHAR_BLOCK_YELLOW:
		std::cout << "\x1b[33m��\x1b[39m";
		break;

	case CHAR_BLOCK_YELLOW + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[33m��\x1b[39m\x1b[0m";
		break;

	case CHAR_BLOCK_GREEN:
		std::cout << "\x1b[32m��\x1b[39m";
		break;

	case CHAR_BLOCK_GREEN + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[32m��\x1b[39m\x1b[0m";
		break;

	case CHAR_BLOCK_RED:
		std::cout << "\x1b[31m��\x1b[39m";
		break;

	case CHAR_BLOCK_RED + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[31m��\x1b[39m\x1b[0m";
		break;

	case CHAR_BLOCK_BLUE:
		std::cout << "\x1b[34m��\x1b[39m";
		break;

	case CHAR_BLOCK_BLUE + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[34m��\x1b[39m\x1b[0m";
		break;

	case CHAR_BLOCK_WHITE:
		std::cout << "\x1b[37m��\x1b[39m";
		break;

	case CHAR_BLOCK_WHITE + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[37m��\x1b[39m\x1b[0m";
		break;

	case CHAR_BLOCK_MAGENTA:
		std::cout << "\x1b[35m��\x1b[39m";
		break;

	case CHAR_BLOCK_MAGENTA + GHOSTBLOCK_OFFSET:
		std::cout << "\x1b[7m\x1b[35m��\x1b[39m\x1b[0m";
		break;


	case CHAR_BAR:
		std::cout << "\x1b[31m����\x1b[39m";
		break;

	default:
		break;
	}

}

// ==============================================
// �z����V���b�t������֐�
// ==============================================
void ShuffleArray(int array[], int size)
{
	for (int i = 0; i < MINO_MAX - 1; i++)
	{ //�z��̃V���b�t��
		//�z��̌�납��i�Ԗڂ̗v�f��randomIndex�̗v�f�Ɠ���ւ���

		int randomIndex = rand() % (MINO_MAX - i);

		int tempData = array[MINO_MAX - 1 - i];
		array[MINO_MAX - 1 - i] = array[randomIndex];
		array[randomIndex] = tempData;
	}
}

// ==============================================
// NEXT�}�b�v�X�V�֐�
// ==============================================
void UpdateNextMap()
{
	for (int i = 0; i < NEXT_DISPLAY_NUM; i++)
	{
		for (int j = 0; j < MINO_SIZE; j++)
		{
			for (int k = 0; k < MINO_SIZE; k++)
			{
				//NEXT�̃e�g���~�m�̃p�^�[����`��
				g_nextMap[i * 4 + j + 2][k + 1] = g_nextMinos.mino[i].shape[j][k]; 
			}
		}
	}
}

// ==============================================
// HOLD�}�b�v�X�V�֐�
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

	//�t�h�\���X�V
	if (canHoldFlg)
	{
		MoveCursor(5, 1);
		std::cout << "�n�j";
	}
	else
	{
		MoveCursor(5, 1);
		std::cout << "�m�f";
	}
}

// ==============================================
// ���C���������Ă��邩�𔻒肷��֐�
// ==============================================
bool CheckLineFull(int y)
{
	//////////////////////
	///////�ϐ��錾///////
	//////////////////////

	int i;

	//////////////////////
	///////���蕔��///////
	//////////////////////

	for (i = 1; i < MAIN_WIDTH - 1; i++)
	{
		if (g_mainMap[y][i] == 0)
			return false; //�󔒂���������false��Ԃ�
	}

	return true;
}

// ==============================================
// ���C�����P�s�����ď�̃u���b�N�����ɗ��Ƃ��֐�
// ==============================================
void DeleteOneLine(int y)
{
	//////////////////////
	///////�ϐ��錾///////
	//////////////////////

	int i, j;

	//////////////////////
	///////��������///////
	//////////////////////

	for (i = y; i > 0; i--)
	{
		for (j = 1; j < MAIN_WIDTH - 1; j++)
			g_mainMap[i][j] = g_mainMap[i - 1][j];
	}

}

// ==============================================
// ���ׂĂ̑��������C������������֐�
// ==============================================
int DeleteMapLine()
{
	//////////////////////
	///////�ϐ��錾///////
	//////////////////////

	int i;
	int DeleteCnt = 0;

	//////////////////////
	///////��������///////
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
// �e�g���~�m�̉�]�������s���֐�
// SRS(Super Rotation System)���s���֐�
// ->�e�g���~�m�̉�]�𒼊��I�ɍs����悤�ɁA
//   ����ɕ␳�������鏈���̂���
// ==============================================
void SuperRotationSystem(int key)
{
	//��x�̉�]�ɂ��A5�̔z�u�p�^�[���𔻒肵�čs��

	//��̏����̂��߂ɉ�]�O�̃A���O�����R�s�[����
	int oldAngle = g_playerMino.angle;

	//�p�^�[��0.�ʏ�̉�]

	if (key == 'p')
	{ //�E��]
		g_playerMino.angle++;
		RotateShape(g_playerMino.basicInfo.shape, g_playerMino.basicInfo.type, true);
	}

	else if (key == 'o')
	{ //����]
		g_playerMino.angle--;
		RotateShape(g_playerMino.basicInfo.shape, g_playerMino.basicInfo.type, false);

	}

	//臒l�̐ݒ�(�l��0~3�Ń��[�v������)
	g_playerMino.angle = (g_playerMino.angle + 4) % 4;

	if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x, g_playerMino.y))
	{ //�ړ��\�ł���΂����ŏC���A�s�\�̏ꍇ�͑����̃p�^�[��1��

		//�l�̏�����
		int deltaX = 0, deltaY = 0; //���̈ړ���

		//��������h�~�m�̏ꍇ�ƁA����ȊO�ŏ������قȂ�
		if (g_playerMino.basicInfo.type == 0)
		{ //�h�~�m�̂Ƃ�

			//�p�^�[��1.�p�^�[��0����}�`�����E�ɓ�����
			//	�p�^�[��0��̐}�`�̃A���O����
			//	1(�E����)�̏ꍇ�͉E�ֈړ�
			//	3(������)�̏ꍇ�͍��ֈړ�
			//	0,2(������E������)�̏ꍇ�͉�]�O�̋t�̌����ֈړ� 
			//  (0�̂Ƃ���2�}�X�ړ�)

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
					deltaX *= 2; //0�̂Ƃ���2�}�X�ړ�
			}

			int tempDeltaX1 = deltaX; //��̃p�^�[������Ŏg�p���邽�߁A�ꎞ�I�ɕۑ�����

			if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
			{ //�ړ��\�ł���΂����Ō���A�s�\�̏ꍇ�p�^�[��2��

				//�p�^�[��2.�p�^�[��0����}�`�����E�ɓ�����
				//	�p�^�[��0��̐}�`�̃A���O����
				//	1(�E����)�̏ꍇ�͍��ֈړ�
				//	3(������)�̏ꍇ�͉E�ֈړ�
				//	0,2(������E������)�̏ꍇ�͉�]�O�̋t�̌����ֈړ� 
				//  (2�̂Ƃ���2�}�X�ړ�)

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
						deltaX *= 2; //2�̂Ƃ���2�}�X�ړ�
				}

				int tempDeltaX2 = deltaX; //��̃p�^�[������Ŏg�p���邽�߁A�ꎞ�I�ɕۑ�����

				if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
				{ //�ړ��\�ł���΂����Ō���A�s�\�̏ꍇ�p�^�[��3��

					//�p�^�[��3.�㉺�ɓ�����
					//	�p�^�[��0��̐}�`�̃A���O����
					//	1(�E����)�̏ꍇ�̓p�^�[��1��̐}�`�����ֈړ�
					//	3(������)�̏ꍇ�̓p�^�[��1��̐}�`����ֈړ�
					//	0,2(������E������)�̏ꍇ��
					//		��]�O�̃A���O����1�Ȃ�p�^�[��1��̐}�`����ֈړ�
					//		��]�O�̃A���O����3�Ȃ�p�^�[��2��̐}�`�����ֈړ�
					//  ����]�Ȃ�㉺�̈ړ��ʂ�2�}�X�ƂȂ�

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
						deltaY *= 2; //����]�Ȃ�㉺�̈ړ��ʂ�2�}�X�ƂȂ�

					if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
					{ //�ړ��\�ł���΂����Ō���A�s�\�̏ꍇ�p�^�[��4��

						//�p�^�[��4.�㉺�ɓ�����
						//	�p�^�[��0��̐}�`�̃A���O����
						//	1(�E����)�̏ꍇ�̓p�^�[��2��̐}�`����ֈړ�
						//	3(������)�̏ꍇ�̓p�^�[��2��̐}�`�����ֈړ�
						//	0,2(������E������)�̏ꍇ��
						//		��]�O�̃A���O����1�Ȃ�p�^�[��2��̐}�`�����ֈړ�
						//		��]�O�̃A���O����3�Ȃ�p�^�[��1��̐}�`����ֈړ�
						//  �E��]�Ȃ�㉺�̈ړ��ʂ�2�}�X�ƂȂ�

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
							deltaY *= 2; //�E��]�Ȃ�㉺�̈ړ��ʂ�2�}�X�ƂȂ�

						if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
						{ //����œ����Ȃ�������ړ�����]���L�����Z������
							deltaX = 0;
							deltaY = 0;

							if (key == 'p')
							{
								g_playerMino.angle--;
								RotateShape(g_playerMino.basicInfo.shape, g_playerMino.basicInfo.type, false); //��]��ł�����
							}

							else if (key == 'o')
							{
								g_playerMino.angle++;
								RotateShape(g_playerMino.basicInfo.shape, g_playerMino.basicInfo.type, true); //��]��ł�����
							}
						}
					}

				}
			}

		}

		else
		{ //�h�~�m�ȊO�̂Ƃ�

			//�p�^�[��1.�p�^�[��0����}�`�����E��1�}�X������
			//	�p�^�[��0��̐}�`�̃A���O����
			//	1(�E����)�̏ꍇ�͍��ֈړ�
			//	3(������)�̏ꍇ�͉E�ֈړ�
			//	0,2(������E������)�̏ꍇ�͉�]�O�̋t�̌����ֈړ�

			if (g_playerMino.angle == 1) //�E�����̂Ƃ�
				deltaX = -1;

			else if (g_playerMino.angle == 3) //�������̂Ƃ�
				deltaX = 1;

			else
			{ //������E�������̎�
				if (oldAngle == 1) //���A���O�����E�̂Ƃ�
					deltaX = 1;

				else					//���A���O�������̂Ƃ�
					deltaX = -1;
			}

			if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
			{ //�ړ��\�ł���΂����Ō���A�s�\�̏ꍇ�p�^�[��2��

				//�p�^�[��2.�p�^�[��1����㉺�ɂP�}�X������
				//	�p�^�[��0��̐}�`�̃A���O����
				//	1,3(�E�����E������)�̏ꍇ�͏�ֈړ�
				//	0,2(������E������)�̏ꍇ�͉��ֈړ�

				if (g_playerMino.angle == 1 || g_playerMino.angle == 3)
					deltaY = -1;

				else
					deltaY = 1;

				if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
				{ //�ړ��\�ł���΂����Ō���A�s�\�̏ꍇ�p�^�[��3��

					//�p�^�[��3.�p�^�[��0����㉺�ɂQ�}�X������
					//	1,3(�E�����E������)�̏ꍇ�͉��ֈړ�
					//	0,2(������E������)�̏ꍇ�͏�ֈړ�

					//�p�^�[��0����Ȃ̂ŕψʂ����Z�b�g
					deltaX = 0;
					deltaY = 0;

					if (g_playerMino.angle == 1 || g_playerMino.angle == 3)
						deltaY = -2;

					else
						deltaY = 2;

					if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
					{ //�ړ��\�ł���΂����Ō���A�s�\�̏ꍇ�p�^�[��4��

						//�p�^�[��4.�p�^�[��3����}�`�����E�ɓ�����
						//	�p�^�[��0��̐}�`�̃A���O����
						//	1(�E����)�̏ꍇ�͍�
						//	3(������)�̏ꍇ�͉E�ֈړ�
						//	0,2(������E������)�̏ꍇ�͉�]�O�̋t�̌����ֈړ�

						if (g_playerMino.angle == 1)
							deltaX = -1;

						else if (g_playerMino.angle == 3)
							deltaX = 1;

						else
						{ //������E�������̂Ƃ�
							if (oldAngle == 1)
								deltaX = 1;

							else
								deltaX = -1;
						}

						if (!CanMove(g_playerMino.basicInfo.shape, g_playerMino.x + deltaX, g_playerMino.y + deltaY))
						{ //����œ����Ȃ�������ړ�����]���L�����Z������
							deltaX = 0;
							deltaY = 0;

							if (key == 'p')
							{
								g_playerMino.angle--;
								RotateShape(g_playerMino.basicInfo.shape, g_playerMino.basicInfo.type, false); //��]��ł�����
							}

							else if (key == 'o')
							{
								g_playerMino.angle++;
								RotateShape(g_playerMino.basicInfo.shape, g_playerMino.basicInfo.type, true); //��]��ł�����
							}
						}
					}
				}
			}
		}

		g_playerMino.x += deltaX;
		g_playerMino.y += deltaY;

		//臒l�̐ݒ�(�l��0~3�Ń��[�v������)
		g_playerMino.angle = (g_playerMino.angle + 4) % 4;
	}
}

// ==============================================
// �}90���}�`����]������֐�
// ==============================================
void RotateShape(char sourceShape[MINO_SIZE][MINO_SIZE], int minoType, bool rotateClockwise)
{
	//�P�ʉ~��̔C�ӂ̓_(x,y)��
	// x=cos��,y=sin�Ƃƕ\����
	// 90�x���v���ɉ�]��������̓_(x��,y��)��
	// x��=cos(��-90��)=cos��cos90��+sin��sin90��=sin��=y
	// y��=sin(��-90��)=sin��cos90��-cos��sin90��=-cos��=-x
	// ���Ȃ킿
	//�@�s�Ɨ�����ւ���(y=x�����ɑΏ̈ړ�)
	//�A���E���]������
	//����ɂ�莞�v����90����]�������ƂɂȂ�̂ŁA�����p���Đ}�`����]������
	//(�����v���̏ꍇ�͇A�̂Ƃ��ɏ㉺���])

	//////////////////
	/////�ϐ��錾/////
	//////////////////

	char tmp[MINO_SIZE][MINO_SIZE] = { 0 }; //��L�̇@�̏�����̃p�^�[����ۑ�����ꎞ�I�Ȕz��

	//////////////////
	////�}�`�̉�]////
	//////////////////

	//�h�~�m�����傫���̂ŁA�h�~�m�Ƃ��̑��̃~�m�ŏ����𕪂���
	if (minoType == MINO_I)
	{ //�h�~�m�̂Ƃ�

		//�@i=j�����ɑΏ̈ړ�
		for (int i = 0; i < MINO_SIZE; i++)
		{
			for (int j = 0; j < MINO_SIZE; j++)
			{
				tmp[i][j] = sourceShape[j][i];
			}
		}

		if (rotateClockwise)
		{ //���v���̏ꍇ

			//�A���E���]
			for (int i = 0; i < MINO_SIZE; i++)
			{
				for (int j = 0; j < MINO_SIZE; j++)
				{
					sourceShape[i][j] = tmp[i][MINO_SIZE - 1 - j];
				}
			}

		}

		else
		{ //�����v���̏ꍇ
			//�A�㉺���]
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
	{ //�h�~�m�ȊO�̎�

		//�@i=j�����ɑΏ̈ړ�
		for (int i = 0; i < MINO_SIZE - 1; i++)
		{
			for (int j = 0; j < MINO_SIZE - 1; j++)
			{
				tmp[i][j] = sourceShape[j][i];
			}
		}

		if (rotateClockwise)
		{ //���v���̏ꍇ

			//�A���E���]
			for (int i = 0; i < MINO_SIZE - 1; i++)
			{
				for (int j = 0; j < MINO_SIZE - 1; j++)
				{
					sourceShape[i][j] = tmp[i][MINO_SIZE - 2 - j];
				}
			}

		}

		else
		{ //�����v���̏ꍇ
			//�A�㉺���]
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
// �J�[�\�����ړ�������֐�
// ==============================================
void MoveCursor(int Y, int X) 
{
	//�o�͂�S�p�����œ��ꂵ�Ă���̂ŁAX��2�{�ɂ���
	std::cout << "\033[" << Y + 1 << ";" << 2 * X + 1 << "H";
}