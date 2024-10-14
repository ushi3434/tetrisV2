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
#define COLOR_CYAN		(2)
#define COLOR_YELLOW	(3)
#define COLOR_GREEN		(4)
#define COLOR_RED		(5)
#define COLOR_BLUE		(6)
#define COLOR_WHITE		(7)
#define COLOR_MAGENTA	(8)
#define NEXT_QUEUE_SIZE (14) //���̃e�g���~�m�̎�ނ�ۑ�����L���[�̃T�C�Y


//�Q�[�����[���֘A
#define GAMEOVER_LINE_HEIGHT (2) //�Q�[���I�[�o�[���C���̂x���W
#define MINOSTART_X (4) //�e�g���~�m�̃X�|�[���n�_(X���W)
#define MINOSTART_Y (0) //�e�g���~�m�̃X�|�[���n�_(Y���W)

//�����o�͊֘A
#define CHAR_NULL	(0)	//�S�p�X�y�[�X
#define CHAR_SQUARE (1)	//��
#define CHAR_BAR	(9)	//��


// ==============================================
// �\���̐錾
// ==============================================
struct MAP
{
	char main[MAIN_HEIGHT][MAIN_WIDTH] = { 0 }; //���C���}�b�v
	char next[NEXT_HEIGHT][NEXT_WIDTH] = { 0 }; //NEXT�}�b�v
	char hold[HOLD_HEIGHT][HOLD_WIDTH] = { 0 }; //HOLD�}�b�v
};

struct MINO
{
	char shape[MINO_SIZE][MINO_SIZE];	//�p�^�[��
	int  color;		//�F
	int	 x;			//X���W
	int	 y;			//Y���W
	int  rotation;	//�p�x
};

struct NEXTMINO
{
	MINO minoQueue[NEXT_QUEUE_SIZE];
	int queueNum;					//�L���[�̌�
};

// ==============================================
// �v���g�^�C�v�錾
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
// ���C���֐�
// ==============================================
int main(void)
{
	srand((unsigned int)time(NULL));

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
	MINO holdMino;				//�z�[���h�̃~�m��ۑ�����
	bool canHoldFlg = false;		//�z�[���h���\���ǂ���
	bool holdEmptyFlg = true;		//�z�[���h���󂩂ǂ���

	MAP	gameMap = InitializeMap();			//�}�b�v����
	NEXTMINO nextMino = InitializeNextMino();	//�e�g���~�m�̒��I
	MINO playerMino = nextMino.minoQueue[0]; //�e�g���~�m�̃c��
	
	UpdateScreen(gameMap, playerMino);

	//////////////////////////////
	/////////�Q�[�����[�v/////////
	//////////////////////////////
	while (true)
	{
		////////////////////////////////
		//////�L�[�{�[�h����̎���//////
		////////////////////////////////

		if (_kbhit())
		{
			switch (_getch())
			{
			case 'w':
				//�n�[�h�h���b�v
				while (true)
				{
					if (CanMove(gameMap.main, playerMino.shape, playerMino.x, playerMino.y + 1))
						playerMino.y++;
					else
					{
						placeMinoFlg = true; //�e�g���~�m�ݒu����R�[�h�͉��ɂ����
						break;
					}
				}

			case 'a':
				//���ړ�
				if (CanMove(gameMap.main, playerMino.shape, playerMino.x - 1, playerMino.y))
					playerMino.x--;
				break;

			case 'd':
				//�E�ړ�
				if (CanMove(gameMap.main, playerMino.shape, playerMino.x + 1, playerMino.y))
					playerMino.x++;
				break;

			case 's':
				//���ړ�
				if (CanMove(gameMap.main, playerMino.shape, playerMino.x, playerMino.y + 1))
					playerMino.y++;
				break;

			case 0x1B:
				//ESC�Ń^�C�g���ɖ߂�
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
// �}�b�v�������֐�
// ==============================================
struct MAP InitializeMap()
{
	MAP gameMap = { 0 };

	//���C���}�b�v�ɕǁE�n�ʂ�z�u����
	for (int i = 0; i < MAIN_HEIGHT; i++)
	{
		for (int j = 0; j < MAIN_WIDTH; j++)
		{
			if (j == 0 || j == MAIN_WIDTH - 1 || i == MAIN_HEIGHT - 1)
				gameMap.main[i][j] = 1;
		}
	}

	//NEXT�}�b�v�ɊO�ǂ�z�u����
	for (int i = 0; i < NEXT_HEIGHT; i++)
	{
		for (int j = 0; j < NEXT_WIDTH; j++)
		{
			if (j == 0 || j == NEXT_WIDTH - 1 ||
				i == 0 || i == NEXT_HEIGHT - 1)
				gameMap.next[i][j] = 1; //�O��	
		}
	}

	//HOLD�}�b�v�ɊO�ǂ�z�u����
	for (int i = 0; i < HOLD_HEIGHT; i++)
	{
		for (int j = 0; j < HOLD_WIDTH; j++)
		{
			if (j == 0 || j == HOLD_WIDTH - 1 ||
				i == 0 || i == HOLD_HEIGHT - 1)
				gameMap.hold[i][j] = 1; //�O��	
		}
	}

	return gameMap;
}

// ==============================================
// �l�N�X�g�~�m�������֐�
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
// �l�N�X�g�~�m���I�֐�
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
// �e�g���~�m�f�[�^�ύX�֐�
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
// ��ʏo�͊֐�
// ==============================================
void UpdateScreen(MAP gameMap, MINO minoData)
{
	char displayBuffer[WINDOW_HEIGHT][WINDOW_WIDTH] = { 0 };

	//////////////////
	//�z�����������//
	//////////////////

	//���C���}�b�v�̏�������
	for (int i = 0; i < MAIN_HEIGHT; i++)
	{
		for (int j = 0; j < MAIN_WIDTH; j++)
		{
			displayBuffer[i][j + HOLD_WIDTH - 1] = gameMap.main[i][j];
		}
	}

	//�Q�[���I�[�o�[���C���̏�������
	for (int i = 0; i < MAIN_WIDTH - 2; i++)
	{
		displayBuffer[GAMEOVER_LINE_HEIGHT][HOLD_WIDTH + i] = CHAR_BAR;
	}

	//���쒆�̃e�g���~�m�̏�������
	for (int i = 0; i < MINO_SIZE; i++)
	{
		for (int j = 0; j < MINO_SIZE; j++)
		{
			if (minoData.shape[i][j] != 0)
				displayBuffer[i + minoData.y][j + HOLD_WIDTH - 1 + minoData.x] = minoData.color;
		}
	}

	//////////////////
	////��ʂ̏o��////
	//////////////////

	system("cls"); //��ʏ���

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
	std::cout << "�@�@�@�@�@�@A:�Ђ��� D:�݂�   S:����       W:�n�[�h�h���b�v\n";
	std::cout << "�@�@�@�@�@�@O:����] P:�E��] C/L:�z�[���h ESC:�^�C�g���ɂ��ǂ�\n";

}

// ==============================================
// �ړ��\�����肷��֐�
// ==============================================
bool CanMove(char mainMap[MAIN_HEIGHT][MAIN_WIDTH], char sourceShape[MINO_SIZE][MINO_SIZE], int futurePosX, int futurePosY)
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
			if (sourceShape[i][j] != 0 &&					//�`�����N�̒��Ńu���b�N������ꏊ��
				mainMap[futurePosY + i][futurePosX + j] != 0)	//�ړ����map��ŋ�(0)����Ȃ�������
				return false;									//false��Ԃ�
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
	case CHAR_NULL:
		std::cout << "  ";
		break;

	case CHAR_SQUARE:
		std::cout << "\x1b[39m��\x1b[39m";
		break;

	case COLOR_CYAN:
		std::cout << "\x1b[36m��";
		break;

	case COLOR_YELLOW:
		std::cout << "\x1b[33m��";
		break;

	case COLOR_GREEN:
		std::cout << "\x1b[32m��";
		break;

	case COLOR_RED:
		std::cout << "\x1b[31m��";
		break;

	case COLOR_BLUE:
		std::cout << "\x1b[34m��";
		break;

	case COLOR_WHITE:
		std::cout << "\x1b[37m��";
		break;

	case COLOR_MAGENTA:
		std::cout << "\x1b[35m��";
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