
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "Main.h"
#include "Renderer.h"
#include "Model.h"



void Model::Draw()
{

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &modelVertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(modelIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	for( unsigned int i = 0; i < modelSubsetNum; i++ )
	{
		// �}�e���A���ݒ�
		Renderer::SetMaterial( modelSubsetArray[i].Material.Material );

		// �e�N�X�`���ݒ�
		Renderer::GetDeviceContext()->PSSetShaderResources( 0, 1, &modelSubsetArray[i].Material.materialTexture );

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed( modelSubsetArray[i].subsetIndexNum, modelSubsetArray[i].subsetStartIndex, 0 );
	}

}



void Model::Load( const char *FileName )
{

	MODEL model;
	LoadObj( FileName, &model );

	// ���_�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( VERTEX_3D ) * model.modelVertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory( &sd, sizeof(sd) );
		sd.pSysMem = model.modelVertexArray;

		Renderer::GetDevice()->CreateBuffer( &bd, &sd, &modelVertexBuffer );
	}


	// �C���f�b�N�X�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( unsigned int ) * model.modelIndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory( &sd, sizeof(sd) );
		sd.pSysMem = model.modelIndexArray;

		Renderer::GetDevice()->CreateBuffer( &bd, &sd, &modelIndexBuffer );
	}

	// �T�u�Z�b�g�ݒ�
	{
		modelSubsetArray = new SUBSET[ model.modelSubsetNum ];
		modelSubsetNum = model.modelSubsetNum;

		for( unsigned int i = 0; i < model.modelSubsetNum; i++ )
		{
			modelSubsetArray[i].subsetStartIndex = model.modelSubsetArray[i].subsetStartIndex;
			modelSubsetArray[i].subsetIndexNum = model.modelSubsetArray[i].subsetIndexNum;

			modelSubsetArray[i].Material.Material = model.modelSubsetArray[i].Material.Material;

			modelSubsetArray[i].Material.materialTexture = NULL;

			D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
				model.modelSubsetArray[i].Material.materialTextureName,
				NULL,
				NULL,
				&modelSubsetArray[i].Material.materialTexture,
				NULL);

			assert(modelSubsetArray[i].Material.materialTexture);

		}
	}

	delete[] model.modelVertexArray;
	delete[] model.modelIndexArray;
	delete[] model.modelSubsetArray;

}

void Model::Unload()
{
	modelVertexBuffer->Release();
	modelIndexBuffer->Release();

	/*for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		m_SubsetArray[i].Material.Texture->Release();
	}*/

	delete[] modelSubsetArray;

}





//���f���Ǎ�////////////////////////////////////////////
void Model::LoadObj( const char *FileName, MODEL *Model )
{

	char dir[MAX_PATH];
	strcpy (dir, FileName );
	PathRemoveFileSpec(dir);





	D3DXVECTOR3	*positionArray;
	D3DXVECTOR3	*normalArray;
	D3DXVECTOR2	*texcoordArray;

	unsigned int	positionNum = 0;
	unsigned int	normalNum = 0;
	unsigned int	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned int	indexNum = 0;
	unsigned int	in = 0;
	unsigned int	subsetNum = 0;

	MODEL_MATERIAL	*materialArray = NULL;
	unsigned int	materialNum = 0;

	char str[256];
	char *s;
	char c;


	FILE *file;
	file = fopen( FileName, "rt" );
	assert(file);



	//�v�f���J�E���g
	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;

		if( strcmp( str, "v" ) == 0 )
		{
			positionNum++;
		}
		else if( strcmp( str, "vn" ) == 0 )
		{
			normalNum++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			texcoordNum++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			subsetNum++;
		}
		else if( strcmp( str, "f" ) == 0 )
		{
			in = 0;

			do
			{
				fscanf( file, "%s", str );
				vertexNum++;
				in++;
				c = fgetc( file );
			}
			while( c != '\n' && c!= '\r' );

			//�l�p�͎O�p�ɕ���
			if( in == 4 )
				in = 6;

			indexNum += in;
		}
	}


	//�������m��
	positionArray = new D3DXVECTOR3[ positionNum ];
	normalArray = new D3DXVECTOR3[ normalNum ];
	texcoordArray = new D3DXVECTOR2[ texcoordNum ];


	Model->modelVertexArray = new VERTEX_3D[ vertexNum ];
	Model->modelVertexNum = vertexNum;

	Model->modelIndexArray = new unsigned int[ indexNum ];
	Model->modelIndexNum = indexNum;

	Model->modelSubsetArray = new SUBSET[ subsetNum ];
	Model->modelSubsetNum = subsetNum;




	//�v�f�Ǎ�
	D3DXVECTOR3 *position = positionArray;
	D3DXVECTOR3 *normal = normalArray;
	D3DXVECTOR2 *texcoord = texcoordArray;

	unsigned int vc = 0;
	unsigned int ic = 0;
	unsigned int sc = 0;


	fseek( file, 0, SEEK_SET );

	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;

		if( strcmp( str, "mtllib" ) == 0 )
		{
			//�}�e���A���t�@�C��
			fscanf( file, "%s", str );

			char path[256];
			strcpy( path, dir );
			strcat( path, "\\" );
			strcat( path, str );

			LoadMaterial( path, &materialArray, &materialNum );
		}
		else if( strcmp( str, "o" ) == 0 )
		{
			//�I�u�W�F�N�g��
			fscanf( file, "%s", str );
		}
		else if( strcmp( str, "v" ) == 0 )
		{
			//���_���W
			fscanf( file, "%f", &position->x );
			fscanf( file, "%f", &position->y );
			fscanf( file, "%f", &position->z );
			position++;
		}
		else if( strcmp( str, "vn" ) == 0 )
		{
			//�@��
			fscanf( file, "%f", &normal->x );
			fscanf( file, "%f", &normal->y );
			fscanf( file, "%f", &normal->z );
			normal++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			//�e�N�X�`�����W
			fscanf( file, "%f", &texcoord->x );
			fscanf( file, "%f", &texcoord->y );
			texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			//�}�e���A��
			fscanf( file, "%s", str );

			if( sc != 0 )
				Model->modelSubsetArray[ sc - 1 ].subsetIndexNum = ic - Model->modelSubsetArray[ sc - 1 ].subsetStartIndex;

			Model->modelSubsetArray[ sc ].subsetStartIndex = ic;


			for( unsigned int i = 0; i < materialNum; i++ )
			{
				if( strcmp( str, materialArray[i].materialName ) == 0 )
				{
					Model->modelSubsetArray[ sc ].Material.Material = materialArray[i].Material;
					strcpy( Model->modelSubsetArray[ sc ].Material.materialTextureName, materialArray[i].materialTextureName );
					strcpy( Model->modelSubsetArray[ sc ].Material.materialName, materialArray[i].materialName );

					break;
				}
			}

			sc++;
			
		}
		else if( strcmp( str, "f" ) == 0 )
		{
			//��
			in = 0;

			do
			{
				fscanf( file, "%s", str );

				s = strtok( str, "/" );	
				Model->modelVertexArray[vc].position = positionArray[ atoi( s ) - 1 ];
				if( s[ strlen( s ) + 1 ] != '/' )
				{
					//�e�N�X�`�����W�����݂��Ȃ��ꍇ������
					s = strtok( NULL, "/" );
					Model->modelVertexArray[vc].texCoord = texcoordArray[ atoi( s ) - 1 ];
				}
				s = strtok( NULL, "/" );	
				Model->modelVertexArray[vc].normal = normalArray[ atoi( s ) - 1 ];

				Model->modelVertexArray[vc].diffuse = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );

				Model->modelIndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc( file );
			}
			while( c != '\n' && c != '\r' );

			//�l�p�͎O�p�ɕ���
			if( in == 4 )
			{
				Model->modelIndexArray[ic] = vc - 4;
				ic++;
				Model->modelIndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}


	if( sc != 0 )
		Model->modelSubsetArray[ sc - 1 ].subsetIndexNum = ic - Model->modelSubsetArray[ sc - 1 ].subsetStartIndex;


	fclose( file );


	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}




//�}�e���A���ǂݍ���///////////////////////////////////////////////////////////////////
void Model::LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum )
{

	char dir[MAX_PATH];
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);



	char str[256];

	FILE *file;
	file = fopen( FileName, "rt" );
	assert(file);

	MODEL_MATERIAL *materialArray;
	unsigned int materialNum = 0;

	//�v�f���J�E���g
	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			materialNum++;
		}
	}


	//�������m��
	materialArray = new MODEL_MATERIAL[ materialNum ];


	//�v�f�Ǎ�
	int mc = -1;

	fseek( file, 0, SEEK_SET );

	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			//�}�e���A����
			mc++;
			fscanf( file, "%s", materialArray[ mc ].materialName );
			strcpy( materialArray[ mc ].materialTextureName, "" );

			materialArray[mc].Material.Emission.r = 0.0f;
			materialArray[mc].Material.Emission.g = 0.0f;
			materialArray[mc].Material.Emission.b = 0.0f;
			materialArray[mc].Material.Emission.a = 0.0f;
		}
		else if( strcmp( str, "Ka" ) == 0 )
		{
			//�A���r�G���g
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.b );
			materialArray[ mc ].Material.Ambient.a = 1.0f;
		}
		else if( strcmp( str, "Kd" ) == 0 )
		{
			//�f�B�t���[�Y
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.b );
			materialArray[ mc ].Material.Diffuse.a = 1.0f;
		}
		else if( strcmp( str, "Ks" ) == 0 )
		{
			//�X�y�L����
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.b );
			materialArray[ mc ].Material.Specular.a = 1.0f;
		}
		else if( strcmp( str, "Ns" ) == 0 )
		{
			//�X�y�L�������x
			fscanf( file, "%f", &materialArray[ mc ].Material.Shininess );
		}
		else if( strcmp( str, "d" ) == 0 )
		{
			//�A���t�@
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.a );
		}
		else if( strcmp( str, "map_Kd" ) == 0 )
		{
			//�e�N�X�`��
			fscanf( file, "%s", str );

			char path[256];
			strcpy( path, dir );
			strcat( path, "\\" );
			strcat( path, str );

			strcat( materialArray[ mc ].materialTextureName, path );
		}
	}

	fclose(file);

	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}

