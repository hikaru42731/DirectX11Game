#ifndef DEBUG_FONT_H_
#define DEBUG_FONT_H_

// �f�o�b�O�t�H���g���W���[���̏�����
void DebugFont_Init(void);

// �f�o�b�O�t�H���g�̕\��
// 
// ����  pString...�f�o�b�O������ւ̃|�C���^
//         dx,dy...�f�o�b�O������̕`����W
void DebugFont_Draw(const char* pString, float dx, float dy);

// �f�o�b�O�t�H���g���W���[���̏I��
void DebugFont_Fin(void);


#endif // !DEBUG_FONT_H_

