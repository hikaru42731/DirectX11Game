#include "Main.h"
#include "Renderer.h"
#include "AnimationModel.h"


void AnimationModel::Draw()
{
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�}�e���A��
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	for (unsigned int m = 0; m < animeModelAiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = animeModelAiScene->mMeshes[m];

		aiMaterial* material = animeModelAiScene->mMaterials[mesh->mMaterialIndex];
		
		//�e�N�X�`���ݒ�
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &animeModelTexture[path.data]);
		//���_�o�b�t�@
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &animeModelVertexBuffer[m], &stride, &offset);

		//�C���f�b�N�X�o�b�t�@
		Renderer::GetDeviceContext()->IASetIndexBuffer(animeModelIndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		//�|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);

	}
}

void AnimationModel::LoadAnimation(const char* FileName, const char* AnimationName)
{
	animeModelAnimation[AnimationName] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(animeModelAnimation[AnimationName]);
}

void AnimationModel::UnLoadAnimation(const char* AnimationName)
{
	delete animeModelAnimation[AnimationName];
	animeModelAnimation[AnimationName] = NULL;
}

void AnimationModel::CreateBone(aiNode* node)
{
	BONE bone;
	animeModelBone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}



void AnimationModel::Load(const char * FileName)
{
	const std::string modelPath(FileName);

	animeModelAiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_ConvertToLeftHanded);
	assert(animeModelAiScene);

	animeModelVertexBuffer = new ID3D11Buffer*[animeModelAiScene->mNumMeshes];
	animeModelIndexBuffer = new ID3D11Buffer*[animeModelAiScene->mNumMeshes];

	//�ό`�㒸�_�z�񐶐�
	animeModelDeformVertex = new std::vector<DEFORM_VERTEX>[animeModelAiScene->mNumMeshes];
	//�ċA�I�Ƀ{�[������
	CreateBone(animeModelAiScene->mRootNode);

	for (unsigned int m = 0; m < animeModelAiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = animeModelAiScene->mMeshes[m];

		//���_�o�b�t�@����
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].texCoord = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &animeModelVertexBuffer[m]);

			delete[] vertex;
		}

		//�C���f�b�N�X�o�b�t�@
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];
			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &animeModelIndexBuffer[m]);
			delete[] index;
		}

		//�ό`�㒸�_�f�[�^������
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX deformVertex;
			deformVertex.position = mesh->mVertices[v];
			deformVertex.normal = mesh->mNormals[v];
			deformVertex.boneNum = 0;
			for (unsigned int b = 0; b < 4; b++)
			{
				deformVertex.boneName[b] = "";
				deformVertex.boneWeight[b] = 0.0f;
			}

			animeModelDeformVertex[m].push_back(deformVertex);
		}

		//�{�[���f�[�^������
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			animeModelBone[bone->mName.C_Str()].offsetMatrix = bone->mOffsetMatrix;

			//�ό`�㒸�_�Ƀ{�[���f�[�^�i�[
			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];

				int num = animeModelDeformVertex[m][weight.mVertexId].boneNum;

				animeModelDeformVertex[m][weight.mVertexId].boneWeight[num] = weight.mWeight;
				animeModelDeformVertex[m][weight.mVertexId].boneName[num] = bone->mName.C_Str();
				animeModelDeformVertex[m][weight.mVertexId].boneNum++;

				assert(animeModelDeformVertex[m][weight.mVertexId].boneNum <= 4);
			}
		}
	}

	{//�e�N�X�`���ǂݍ���
		for (unsigned int m = 0; m < animeModelAiScene->mNumMaterials; m++)
		{
			aiString path;
			if (animeModelAiScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				if (path.data[0] == '*')
				{
					//FBX�t�@�C��������ǂݍ���
					if (animeModelTexture[path.data] == NULL) 
					{
						ID3D11ShaderResourceView* texture;
						int id = atoi(&path.data[1]);

						D3DX11CreateShaderResourceViewFromMemory(
							Renderer::GetDevice(),
							(const unsigned char*)animeModelAiScene->mTextures[id]->pcData,
							animeModelAiScene->mTextures[id]->mWidth, NULL, NULL, &texture, NULL);

						animeModelTexture[path.data] = texture;
					}
				}
				else
				{
					//�O���t�@�C������ǂݍ���
				}
			}
			else
			{
				animeModelTexture[path.data] = NULL;
			}
		}
	}
}

void AnimationModel::UnLoad()
{
	for (unsigned int m = 0; m < animeModelAiScene->mNumMeshes; m++)
	{
		animeModelVertexBuffer[m]->Release();
		animeModelIndexBuffer[m]->Release();
	}

	delete[] animeModelVertexBuffer;
	delete[] animeModelIndexBuffer;

	delete[] animeModelDeformVertex;

	for (std::pair<std::string, ID3D11ShaderResourceView*>pair : animeModelTexture)
	{
		pair.second->Release();
	}

	aiReleaseImport(animeModelAiScene);

}

void AnimationModel::Update(const char* AnimationName1, const char* AnimationName2, float BlendRate, int frame)
{
	if (animeModelAnimation[AnimationName1]->HasAnimations());
	if (animeModelAnimation[AnimationName2]->HasAnimations());

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = animeModelAnimation[AnimationName1]->mAnimations[0];
	aiAnimation* animation2 = animeModelAnimation[AnimationName2]->mAnimations[0];

	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim1 = animation1->mChannels[c];
		aiNodeAnim* nodeAnim2 = animation2->mChannels[c];
		BONE* bone = &animeModelBone[nodeAnim1->mNodeName.C_Str()];

		int f;
		f = frame % nodeAnim1->mNumRotationKeys;//�ȈՎ���
		aiQuaternion rot1 = nodeAnim1->mRotationKeys[f].mValue;

		f = frame % nodeAnim1->mNumPositionKeys;//�ȈՎ���
		aiVector3D pos1 = nodeAnim1->mPositionKeys[f].mValue;

		f = frame % nodeAnim2->mNumRotationKeys;//�ȈՎ���
		aiQuaternion rot2 = nodeAnim2->mRotationKeys[f].mValue;

		f = frame % nodeAnim2->mNumPositionKeys;//�ȈՎ���
		aiVector3D pos2 = nodeAnim2->mPositionKeys[f].mValue;

		aiVector3D pos = pos1 * (1.0f - BlendRate) + pos2 * BlendRate; //���`���
		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot1, rot2, BlendRate);//���ʐ��`���
		bone->animationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//�ċA�I�Ƀ{�[���}�g���N�X���X�V
	UpdateBoneMatrix(animeModelAiScene->mRootNode, aiMatrix4x4());

	//���_�ϊ�(CPU�X�L�j���O�j
	for (unsigned int m = 0; m < animeModelAiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = animeModelAiScene->mMeshes[m];

		D3D11_MAPPED_SUBRESOURCE ms;
		Renderer::GetDeviceContext()->Map(animeModelVertexBuffer[m], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);

		VERTEX_3D* vertex = (VERTEX_3D*)ms.pData;

		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX* deformVertex = &animeModelDeformVertex[m][v];

			aiMatrix4x4 matrix[4];
			aiMatrix4x4 outMatrix;
			matrix[0] = animeModelBone[deformVertex->boneName[0]].matrix;
			matrix[1] = animeModelBone[deformVertex->boneName[1]].matrix;
			matrix[2] = animeModelBone[deformVertex->boneName[2]].matrix;
			matrix[3] = animeModelBone[deformVertex->boneName[3]].matrix;

			//�E�F�C�g���l�����ă}�g���N�X�Z�o
			/*outMatrix=matrix[0]*deformVertex->BoneWeight[0]
				+ matrix[1] * deformVertex->BoneWeight[1]
				+ matrix[2] * deformVertex->BoneWeight[2]
				+ matrix[3] * deformVertex->BoneWeight[3]*/

			{
				outMatrix.a1 = matrix[0].a1 * deformVertex->boneWeight[0]
					+ matrix[1].a1 * deformVertex->boneWeight[1]
					+ matrix[2].a1 * deformVertex->boneWeight[2]
					+ matrix[3].a1 * deformVertex->boneWeight[3];

				outMatrix.a2 = matrix[0].a2 * deformVertex->boneWeight[0]
					+ matrix[1].a2 * deformVertex->boneWeight[1]
					+ matrix[2].a2 * deformVertex->boneWeight[2]
					+ matrix[3].a2 * deformVertex->boneWeight[3];

				outMatrix.a3 = matrix[0].a3 * deformVertex->boneWeight[0]
					+ matrix[1].a3 * deformVertex->boneWeight[1]
					+ matrix[2].a3 * deformVertex->boneWeight[2]
					+ matrix[3].a3 * deformVertex->boneWeight[3];

				outMatrix.a4 = matrix[0].a4 * deformVertex->boneWeight[0]
					+ matrix[1].a4 * deformVertex->boneWeight[1]
					+ matrix[2].a4 * deformVertex->boneWeight[2]
					+ matrix[3].a4 * deformVertex->boneWeight[3];

				outMatrix.b1 = matrix[0].b1 * deformVertex->boneWeight[0]
					+ matrix[1].b1 * deformVertex->boneWeight[1]
					+ matrix[2].b1 * deformVertex->boneWeight[2]
					+ matrix[3].b1 * deformVertex->boneWeight[3];

				outMatrix.b2 = matrix[0].b2 * deformVertex->boneWeight[0]
					+ matrix[1].b2 * deformVertex->boneWeight[1]
					+ matrix[2].b2 * deformVertex->boneWeight[2]
					+ matrix[3].b2 * deformVertex->boneWeight[3];

				outMatrix.b3 = matrix[0].b3 * deformVertex->boneWeight[0]
					+ matrix[1].b3 * deformVertex->boneWeight[1]
					+ matrix[2].b3 * deformVertex->boneWeight[2]
					+ matrix[3].b3 * deformVertex->boneWeight[3];

				outMatrix.b4 = matrix[0].b4 * deformVertex->boneWeight[0]
					+ matrix[1].b4 * deformVertex->boneWeight[1]
					+ matrix[2].b4 * deformVertex->boneWeight[2]
					+ matrix[3].b4 * deformVertex->boneWeight[3];

				outMatrix.c1 = matrix[0].c1 * deformVertex->boneWeight[0]
					+ matrix[1].c1 * deformVertex->boneWeight[1]
					+ matrix[2].c1 * deformVertex->boneWeight[2]
					+ matrix[3].c1 * deformVertex->boneWeight[3];

				outMatrix.c2 = matrix[0].c2 * deformVertex->boneWeight[0]
					+ matrix[1].c2 * deformVertex->boneWeight[1]
					+ matrix[2].c2 * deformVertex->boneWeight[2]
					+ matrix[3].c2 * deformVertex->boneWeight[3];

				outMatrix.c3 = matrix[0].c3 * deformVertex->boneWeight[0]
					+ matrix[1].c3 * deformVertex->boneWeight[1]
					+ matrix[2].c3 * deformVertex->boneWeight[2]
					+ matrix[3].c3 * deformVertex->boneWeight[3];

				outMatrix.c4 = matrix[0].c4 * deformVertex->boneWeight[0]
					+ matrix[1].c4 * deformVertex->boneWeight[1]
					+ matrix[2].c4 * deformVertex->boneWeight[2]
					+ matrix[3].c4 * deformVertex->boneWeight[3];

				outMatrix.d1 = matrix[0].d1 * deformVertex->boneWeight[0]
					+ matrix[1].d1 * deformVertex->boneWeight[1]
					+ matrix[2].d1 * deformVertex->boneWeight[2]
					+ matrix[3].d1 * deformVertex->boneWeight[3];

				outMatrix.d2 = matrix[0].d2 * deformVertex->boneWeight[0]
					+ matrix[1].d2 * deformVertex->boneWeight[1]
					+ matrix[2].d2 * deformVertex->boneWeight[2]
					+ matrix[3].d2 * deformVertex->boneWeight[3];

				outMatrix.d3 = matrix[0].d3 * deformVertex->boneWeight[0]
					+ matrix[1].d3 * deformVertex->boneWeight[1]
					+ matrix[2].d3 * deformVertex->boneWeight[2]
					+ matrix[3].d3 * deformVertex->boneWeight[3];

				outMatrix.d4 = matrix[0].d4 * deformVertex->boneWeight[0]
					+ matrix[1].d4 * deformVertex->boneWeight[1]
					+ matrix[2].d4 * deformVertex->boneWeight[2]
					+ matrix[3].d4 * deformVertex->boneWeight[3];
			}
			deformVertex->position = mesh->mVertices[v];

			deformVertex->position *= outMatrix;
			//�@���ϊ��p�Ɉړ��������폜
			outMatrix.a4 = 0.0f;
			outMatrix.b4 = 0.0f;
			outMatrix.c4 = 0.0f;
		

			deformVertex->normal = mesh->mNormals[v];
			deformVertex->normal *= outMatrix;

			//���_�f�[�^�֏�������
			vertex[v].position.x = deformVertex->position.x;
			vertex[v].position.y = deformVertex->position.y;
			vertex[v].position.z = deformVertex->position.z;

			vertex[v].normal.x = deformVertex->normal.x;
			vertex[v].normal.y = deformVertex->normal.y;
			vertex[v].normal.z = deformVertex->normal.z;

			vertex[v].texCoord.x = mesh->mTextureCoords[0][v].x;
			vertex[v].texCoord.y = mesh->mTextureCoords[0][v].y;

			vertex[v].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		Renderer::GetDeviceContext()->Unmap(animeModelVertexBuffer[m], 0);
	}
}

void AnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	BONE* bone = &animeModelBone[node->mName.C_Str()];

	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;

	worldMatrix = matrix;
	worldMatrix *= bone->animationMatrix;

	bone->matrix = worldMatrix;
	bone->matrix *= bone->offsetMatrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}