#pragma once
#include "GameObject.h"

class R_Texture : public GameObject
{
private:

	ID3D11Buffer* resultTextureVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	static ID3D11ShaderResourceView* resultTexture;
	static bool win;
public:

	//Polygon2D();//�R���X�g���N�^�i�C���X�^���X�𐶐������Ƃ��Ɏ����I�ɌĂ΂��j
	//~Polygon2D()//�f�R���X�g���N�^�i�C���X�^���X���I������Ƃ��Ɏ����I�ɌĂ΂��j
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static void Load(bool W);
	bool GetWin() { return win; }
};