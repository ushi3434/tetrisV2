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
#define CHAR_SPACE			(0)	//�S�p�X�y�[�X
#define CHAR_BLOCK			(1)	//��(���F)
#define CHAR_BLOCK_CYAN		(2) //���F�Ⴂ
#define CHAR_BLOCK_YELLOW	(3)
#define CHAR_BLOCK_GREEN	(4)
#define CHAR_BLOCK_RED		(5)
#define CHAR_BLOCK_BLUE		(6)
#define CHAR_BLOCK_WHITE	(7)
#define CHAR_BLOCK_MAGENTA	(8)
#define CHAR_BAR			(9)	//��
#define STR_HOLD0			(30)//������
#define STR_HOLD1			(31)
#define STR_NEXT0			(32)



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
	int rotation;	//�p�x
	int	x;			//X���W
	int	y;			//Y���W
};

struct NEXTQUEUE
{
	BASIC_MINO mino[NEXT_QUEUE_SIZE];
	int queueNum;					//�L���[�̌�
};

struct STR_DICTIONARY
{
	char hold0[STR_SIZE];
	char hold1[STR_SIZE];
	char next0[STR_SIZE];
};

// ==============================================
// �O���[�o���ϐ��錾
// ==============================================

char g_mainMap[MAIN_HEIGHT][MAIN_WIDTH] = { 0 }; //���C���}�b�v
char g_nextMap[NEXT_HEIGHT][NEXT_WIDTH] = { 0 }; //NEXT�}�b�v
char g_holdMap[HOLD_HEIGHT][HOLD_WIDTH] = { 0 }; //HOLD�}�b�v
GAME_MINO g_playerMino;		//�v���C���[�̓������e�g���~�m
NEXTQUEUE g_nextMinos;		//�l�N�X�g�̗�
STR_DICTIONARY strDictionary; //������f�[�^�̎���

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
	bool canHoldFlg = true;		//�z�[���h���\���ǂ���
	bool holdEmptyFlg = true;	//�z�[���h���󂩂ǂ���
	BASIC_MINO holdMino = { 0 };		//�z�[���h�̃~�m

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
			}
			RenderScreen();
		}
		
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
	strcpy(strDictionary.next0, "�m�d�w�s");
	g_nextMap[1][1] = STR_NEXT0;
	for (int i = 0; i < strlen(strDictionary.next0) / 2 - 1; i++)
	{
		g_nextMap[1][2 + i] = -1;
	}

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
	
	strcpy(strDictionary.hold0, "�g�n�k�c");
	g_holdMap[1][1] = STR_HOLD0;
	for (int i = 0; i < strlen(strDictionary.hold0) / 2 - 1; i++)
	{
		g_holdMap[1][2 + i] = -1;
	}

	strcpy(strDictionary.hold1, "�n�j�@�@");
	g_holdMap[5][1] = STR_HOLD1;
	for (int i = 0; i < strlen(strDictionary.hold1) / 2 - 1; i++)
	{
		g_holdMap[5][2 + i] = -1;
	}

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
	g_playerMino.rotation = 0;
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
	g_playerMino.rotation = 0;
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
	char displayBuffer[WINDOW_HEIGHT][WINDOW_WIDTH] = { 0 };

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

	case CHAR_BLOCK:
		std::cout << "\x1b[39m��\x1b[39m";
		break;

	case CHAR_BLOCK_CYAN:
		std::cout << "\x1b[36m��";
		break;

	case CHAR_BLOCK_YELLOW:
		std::cout << "\x1b[33m��";
		break;

	case CHAR_BLOCK_GREEN:
		std::cout << "\x1b[32m��";
		break;

	case CHAR_BLOCK_RED:
		std::cout << "\x1b[31m��";
		break;

	case CHAR_BLOCK_BLUE:
		std::cout << "\x1b[34m��";
		break;

	case CHAR_BLOCK_WHITE:
		std::cout << "\x1b[37m��";
		break;

	case CHAR_BLOCK_MAGENTA:
		std::cout << "\x1b[35m��";
		break;

	case CHAR_BAR:
		std::cout << "\x1b[31m����\x1b[39m";
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
	if(canHoldFlg)
		strcpy(strDictionary.hold1, "�n�j�@�@");
	else
		strcpy(strDictionary.hold1, "�m�f�@�@");

}