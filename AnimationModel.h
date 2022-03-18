#include <unordered_map>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment(lib,"assimp.lib")

//�ό`�㒸�_�\����
struct DEFORM_VERTEX
{
	aiVector3D position;
	aiVector3D normal;
	int boneNum;
	std::string boneName[4];
	float boneWeight[4];
};

//�{�[���\����
struct BONE
{
	aiMatrix4x4 matrix;
	aiMatrix4x4 animationMatrix;
	aiMatrix4x4 offsetMatrix;
};

class AnimationModel
{
private:
	const aiScene* animeModelAiScene;
	std::unordered_map<std::string, const aiScene*> animeModelAnimation;
	ID3D11Buffer** animeModelVertexBuffer;
	ID3D11Buffer** animeModelIndexBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> animeModelTexture;
	std::vector<DEFORM_VERTEX>* animeModelDeformVertex;//�ό`�㒸�_�f�[�^
	std::unordered_map<std::string, BONE> animeModelBone;	//�{�[���f�[�^�i���O�ŎQ�Ɓj

	void CreateBone(aiNode* Node);
	void UpdateBoneMatrix(aiNode* Node, aiMatrix4x4 Matrix);

public:
	void Load(const char* FileName);
	void LoadAnimation(const char* FileName, const char* AnimationName);
	void UnLoadAnimation(const char* AnimationName);
	void UnLoad();
	void Update(const char* AnimationName1, const char* AnimationName2, float BlendRate, int Frame);
	void Draw();

	void SetAiScene(aiScene* AiScene) { animeModelAiScene = AiScene; }
	aiScene* GetAiScene() { return (aiScene*)animeModelAiScene; }
};