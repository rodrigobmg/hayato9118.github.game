// �}�b�v��` (Map.h)
#ifndef ___H_MAP
#define ___H_MAP

#include <nn/types.h>

#define MAX_WORLDWIDTH	3			// ���}�b�v��
#define MAX_WORLDDEPTH	7			// �c�}�b�v��
#define MAX_MAP			(MAX_WORLDWIDTH*MAX_WORLDDEPTH)	// �}�b�v��
#define MAX_MAPWIDTH	25			// �}�b�v��
#define MAX_MAPDEPTH	15			// �}�b�v����

#define CHIP_WIDTH		16			// �}�b�v�`�b�v��
#define CHIP_DEPTH		16			// �}�b�v�`�b�v����

s32 InitMap();						// ������
void SetMap(u32 mapNo);				// �}�b�vNo.�ݒ�
void DrawMap();						// �`��
void FinMap();						// ���
u8 GetMapChip(f32 x, f32 y, s32 mapNo = -1);	// �}�b�v�`�b�vNo.�擾
u8 GetMapAttr(f32 x, f32 y, s32 mapNo = -1);	// �}�b�v�����擾
u32 GetMap();						// �}�b�vNo.�擾
void SetMapChip(int chipNo = -1);
void SetMapDoor(bool open = true);

#endif // ___H_MAP
