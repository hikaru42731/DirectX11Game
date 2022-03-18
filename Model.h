#pragma once



// �}�e���A���\����
struct MODEL_MATERIAL
{
	char						materialName[256];
	MATERIAL					Material;
	char						materialTextureName[256];
	ID3D11ShaderResourceView*	materialTexture;

};


// �`��T�u�Z�b�g�\����
struct SUBSET
{
	unsigned int	subsetStartIndex;
	unsigned int	subsetIndexNum;
	MODEL_MATERIAL	Material;
};


// ���f���\����
struct MODEL
{
	VERTEX_3D		*modelVertexArray;
	unsigned int	modelVertexNum;

	unsigned int	*modelIndexArray;
	unsigned int	modelIndexNum;

	SUBSET			*modelSubsetArray;
	unsigned int	modelSubsetNum;
};





class Model
{
private:

	ID3D11Buffer*	modelVertexBuffer;
	ID3D11Buffer*	modelIndexBuffer;

	SUBSET*	modelSubsetArray;
	unsigned int	modelSubsetNum;

	void LoadObj( const char *FileName, MODEL *Model );
	void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum );

public:

	void Draw();

	void Load( const char *FileName );
	void Unload();

};