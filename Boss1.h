// �{�X1��` (Boss1.h)

// �萔��`
#define BOSS1_STAT_HIDE			0		// ��\��(0�ȉ��S��)
#define BOSS1_STAT_PAUSE		1		// �ꎞ��~
#define BOSS1_STAT_MOVE			2		// 8�̎��Ɉړ�
#define BOSS1_STAT_EXPLOSION	-997	// ������(�`-1)
#define BOSS1_STAT_HIDE_PAUSE	-997	// �ꎞ��~(��\��)
#define BOSS1_STAT_HIDE_MOVE	-998	// 8�̎��Ɉړ�(��\��)
#define BOSS1_STAT_UNINIT		-999	// ��������

// �֐��v���g�^�C�v
void InitBoss1();		// ����������
void DrawBoss1();		// �\������
void DrawBoss1HP(f32 hp_x, f32 hp_y);
void UpdateBoss1();		// �ړ�����
s32 CollisionBoss1(f32 x, f32 y, f32 w, f32 h, s32 str);	// �����蔻��
s32 GetBoss1Stat();		// �{�X1�X�e�[�^�X�擾
void ShowBoss1();		// �\���J�n
void HideBoss1();		// �\���I��
void ResetBoss1();		// �{�X1������
nn::math::VEC2 GetBoss1Pos();	// �{�X1���W�擾
