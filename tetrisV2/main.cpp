// ==============================================
// main.cpp [�e�g���XV2]
// 
// �����:���۔��l      ���t:2024/10/10
// ==============================================

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
#define MINO_TYPE	(7)		//�e�g���~�m�̎��
#define CHUNK_SIZE	(4)		//�P�̃e�g���~�m�̃p�^�[����ۑ�����̈�(�����`)�̈�ӂ̒���
#define COLOR_CYAN		(0)
#define COLOR_YELLOW	(1)
#define COLOR_GREEN		(2)
#define COLOR_RED		(3)
#define COLOR_BLUE		(4)
#define COLOR_WHITE		(5)
#define COLOR_MAGENTA	(6)

// ==============================================
// �\���̐錾
// ==============================================
struct MINO_ORIGIN
{
	int  color; //�F
	char pattern[4][4]; //�`
};

struct MINO_BLOCK
{
	int minoType;	//�e�g���~�m�̎��
	int x;			//X���W
	int y;			//Y���W
	int rotation;	//�p�x(90�x����)
};

// ==============================================
// ���C���֐�
// ==============================================
int main(void)
{

}

// ==============================================
// �Q�[�����C���֐�
// ==============================================
int GameMain(int)
{
	//�ϐ��錾
	char mainMap[MAIN_HEIGHT][MAIN_WIDTH];

}