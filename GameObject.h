#pragma once

class GameObject // �������z�֐�������N���X�͉��z�N���X�Ƃ��Ė������ʂ����@�p�����邾���̃N���X
{
	bool        objectDestroy = false;

protected: // �p���N���X�ɃA�N�Z�X������ׂ�protected���g��  public > protected > private �̏��ԂŗD�����Ȃ�
	D3DXVECTOR3 objPosition; // �ʒu
	D3DXVECTOR3 objRotation; // ��]
	D3DXVECTOR3 objScale;	// �g��k��
	D3DXVECTOR3 forwardVec;		//���ʃx�N�g��
	D3DXVECTOR3 wallDir;	//�ǂ���x�N�g��
	D3DXVECTOR3 prevPos;	//1f�O�̍��W
	D3DXVECTOR3 moveVec;	//�ړ����Ă������
	D3DXVECTOR3 unWallPos;	//�ǂɓ�����O�̍��W
	float centerLen;
	int enemyId;
	int hp;
	int damage;
	float speed;
	bool colWall;
	float lastDamage;	//�_���[�W�v�Z��̃_���[�W
	float attackPower;		//�U����
	float deffencePower;	//�h���
	float falterCnt;	//���ݎ���
	bool onGround;	//�ڒn
	bool targetCam;
	bool falterAction;
	bool knockBack;	//�Ђ�݃t���O
	bool blowBack;	//�ӂ��Ƃуt���O
	float knockPow;
	bool antiInduction;//�U����؂��Ă邩
	bool pass;
	int stopTime;
	int revengeCnt;	//���x���W�l�J�E���^�[
	int revengeNum;	//���x���W�l���v
	int addRevenge;	//���Z���郊�x���W�l
	int attackRevenge;	//�J�E���^�[���d�|����l
	bool superArmor;		//�X�[�p�[�A�[�}�[
	

	D3DXVECTOR3 knockVector;

	//�V�F�[�_�ۑ�
	ID3D11VertexShader* objVertexShader = NULL;
	ID3D11PixelShader* objPixelShader = NULL;
	ID3D11InputLayout* objVertexLayout = NULL;

	enum MODE
	{
		DEFAULT, //0 �������͂��Ȃ��Ƃ��܂��͈ړ����Ă��鎞
		ATTACK,  //1 �U����
		CHASE,	 //2�ǂ�
		GUARD,	 //3�K�[�h���Ă��鎞
		INVICIBLE,	//���G��
		FALTER,		//�Ђ�ݒ�
		BLOW,		//�ӂ��Ƃ�
		REVENGE,	//�J�E���^�[�i�G�j
		ENCOUNTER	
	};

	int enemyType;
	MODE mode;
	
public:

	GameObject() {};//�@�R���X�g���N�^   {}�͒��g����Ƃ����Ӗ���
	virtual~GameObject() {};//�@�f�X�g���N�^�@virtual �|�����[�t�B�Y�����g�p���邽�߂�

	// ���z�֐� = 0 �ŏ������z�֐�
	virtual void Init() = 0;   // �������֐�
	virtual void Uninit() = 0;   // �I���֐�
	virtual void Update() = 0;   // �X�V�֐�
	virtual void Draw() = 0;   // �`��֐�

	// �Q�b�^�[
	D3DXVECTOR3 GetPosition() { return objPosition; }
	D3DXVECTOR3 GetRotation() { return objRotation; }
	D3DXVECTOR3 GetUnWall()	  { return unWallPos; }
	bool GetArmor() { return superArmor; }
	MODE GetMode() { return mode; }
	bool TargetON(){return targetCam;}
	int GetHP() { return hp; }
	bool GetAntiInduction() { return antiInduction; }

	
	//�Z�b�^�[
	void SetPosition(D3DXVECTOR3 Position) { objPosition = Position; }
	void SetRotation(D3DXVECTOR3 Rotation) { objRotation = Rotation; }
	void SetFVector(D3DXVECTOR3 Fvec) { forwardVec = Fvec; }
	void SetScale(D3DXVECTOR3 Scale) { objScale = Scale; }
	void SetDestroy() { objectDestroy = true; }
	void SetDamage(int damage) {  lastDamage = damage;  }
	void SetATKDamage(int dam, int atk) { damage = dam; attackPower = atk; }
	void SetRevenge(int rev) { addRevenge = rev; }
	void SetKnock(float power, D3DXVECTOR3 KnockVec) { knockPow = power; knockVector = KnockVec; knockBack = true; }
	void SetBlow(float Bpower, D3DXVECTOR3 BlowVec) { knockPow = Bpower, knockVector = BlowVec, blowBack = true; }
	void SetMode(MODE m) { mode = m; }
	void SetID(int id) { enemyId = id; }
	void SetType(int e) { enemyType = e; }
	void SetAntiInduction(bool i) { antiInduction = i; }
	void SetPass(bool p) { pass = p; }
	void SetStates(int Atk, int Def, float sp) { attackPower = Atk; deffencePower = Def; speed = sp; }
	void SetStop(int stoptime) { stopTime = stoptime; }
	
	
	D3DXVECTOR3 GetObbX()
	{
		D3DXMATRIX rot, scale,world;
		D3DXMatrixScaling(&scale, objScale.x, objScale.y, objScale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, objRotation.y, objRotation.x, objRotation.z);
		world = scale * rot;

		D3DXVECTOR3 vx;
		vx.x = world._11;
		vx.y = world._12;
		vx.z = world._13;

		return vx;
	}

	D3DXVECTOR3 GetObbZ()
	{
		D3DXMATRIX rot, scale, world;
		
		D3DXMatrixScaling(&scale, objScale.x, objScale.y, objScale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, objRotation.y, objRotation.x, objRotation.z);
		world = scale * rot;

		D3DXVECTOR3 vz;
		vz.x = world._31;
		vz.y = world._32;
		vz.z = world._33;

		return vz;
	}

	D3DXVECTOR3 GetObbY()
	{
		D3DXMATRIX rot, scale, world;
		D3DXMatrixScaling(&scale, objScale.x, objScale.y, objScale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, objRotation.y, objRotation.x, objRotation.z);
		world = scale * rot;

		D3DXVECTOR3 vy;
		vy.x = world._21;
		vy.y = world._22;
		vy.z = world._23;

		return vy;
	}

	// �������ɓ��e���ꂽ���������瓊�e���������Z�o
	float LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = 0)
	{
		// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
		// ������Sep�͕W��������Ă��邱��
		float r1 = fabs(D3DXVec3Dot(Sep, e1));
		float r2 = fabs(D3DXVec3Dot(Sep, e2));
		float r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
		return r1 + r2 + r3;
	}

	D3DXVECTOR3* WallShaving(D3DXVECTOR3* out, const D3DXVECTOR3& front, const D3DXVECTOR3& normal)		//�ǂ���x�N�g��
	{
		D3DXVECTOR3 normal_n;

		D3DXVec3Normalize(&normal_n, &normal);
		return D3DXVec3Normalize(out, &(front - D3DXVec3Dot(&front, &normal_n) * normal_n));
	}

	bool Colision(GameObject* a, GameObject* b)
	{
		const float EPSILON = 1.175494e-37;

		float R[3][3], AbsR[3][3];

		R[0][0] = D3DXVec3Dot(&a->GetObbX(), &b->GetObbX());
		AbsR[0][0] = fabsf(R[0][0]) + EPSILON;
		R[0][1] = D3DXVec3Dot(&a->GetObbX(), &b->GetObbY());
		AbsR[0][1] = fabsf(R[0][1]) + EPSILON;
		R[0][2] = D3DXVec3Dot(&a->GetObbX(), &b->GetObbZ());
		AbsR[0][2] = fabsf(R[0][2]) + EPSILON;

		R[1][0] = D3DXVec3Dot(&a->GetObbY(), &b->GetObbX());
		AbsR[1][0] = fabsf(R[1][0]) + EPSILON;
		R[1][1] = D3DXVec3Dot(&a->GetObbY(), &b->GetObbY());
		AbsR[1][1] = fabsf(R[1][1]) + EPSILON;
		R[1][2] = D3DXVec3Dot(&a->GetObbY(), &b->GetObbZ());
		AbsR[1][2] = fabsf(R[1][2]) + EPSILON;

		R[2][0] = D3DXVec3Dot(&a->GetObbZ(), &b->GetObbX());
		AbsR[2][0] = fabsf(R[2][0]) + EPSILON;
		R[2][1] = D3DXVec3Dot(&a->GetObbZ(), &b->GetObbY());
		AbsR[2][1] = fabsf(R[2][1]) + EPSILON;
		R[2][2] = D3DXVec3Dot(&a->GetObbZ(), &b->GetObbZ());
		AbsR[2][2] = fabsf(R[2][2]) + EPSILON;


		D3DXVECTOR3 t = b->GetPosition() - a->GetPosition();
		t = D3DXVECTOR3(D3DXVec3Dot(&t, &a->GetObbX()), D3DXVec3Dot(&t, &a->GetObbY()), D3DXVec3Dot(&t, &a->GetObbZ()));

		//��L=A0, L=A1, L=A2����
		float ra, rb;
		float ALenx, ALeny, ALenz, BLenx, BLeny, BLenz;
		ALenx = D3DXVec3Length(&a->GetObbX()) * 0.5f; ALeny = D3DXVec3Length(&a->GetObbY()) * 0.5f; ALenz = D3DXVec3Length(&a->GetObbZ()) * 0.5f;
		BLenx = D3DXVec3Length(&b->GetObbX()) * 0.1f; BLeny = D3DXVec3Length(&b->GetObbY()) * 0.1f; BLenz = D3DXVec3Length(&b->GetObbZ()) * 0.1f;

		ra = ALenx;
		rb = BLenx * AbsR[0][0] + BLeny * AbsR[0][1] + BLenz * AbsR[0][2];
		if (fabsf(t[0]) > ra + rb)return false;

		ra = ALeny;
		rb = BLenx * AbsR[1][0] + BLeny * AbsR[1][1] + BLenz * AbsR[1][2];
		if (fabsf(t[1]) > ra + rb)return false;

		ra = ALenz;
		rb = BLenx * AbsR[2][0] + BLeny * AbsR[2][1] + BLenz * AbsR[2][2];
		if (fabsf(t[2]) > ra + rb)return false;

		//��L=B0, L=B1, L=B2����

		ra = ALenx * AbsR[0][0] + ALeny * AbsR[1][0] + ALenz * AbsR[2][0];
		rb = BLenx;
		if (fabsf(t[0] * R[0][0] + t[1] * R[1][0] + t[2] * R[2][0]) > ra + rb)return false;

		ra = ALenx * AbsR[0][1] + ALeny * AbsR[1][1] + ALenz * AbsR[2][1];
		rb = BLeny;
		if (fabsf(t[0] * R[0][1] + t[1] * R[1][1] + t[2] * R[2][1]) > ra + rb)return false;

		ra = ALenx * AbsR[0][2] + ALeny * AbsR[1][2] + ALenz * AbsR[2][2];
		rb = BLenz;
		if (fabsf(t[0] * R[0][2] + t[1] * R[1][2] + t[2] * R[2][2]) > ra + rb)return false;


		//��L=A0 X B0����
		ra = ALeny * AbsR[2][0] + ALenz * AbsR[1][0];
		rb = BLeny * AbsR[0][2] + BLenz * AbsR[0][1];
		if (fabsf(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb)return false;

		//��L=A0 X B1����
		ra = ALeny * AbsR[2][1] + ALenz * AbsR[1][1];
		rb = BLenx * AbsR[0][2] + BLenz * AbsR[0][0];
		if (fabsf(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb)return false;

		//��L=A0 X B2����
		ra = ALeny * AbsR[2][2] + ALenz * AbsR[1][2];
		rb = BLenx * AbsR[0][1] + BLeny * AbsR[0][0];
		if (fabsf(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb)return false;

		//��L=A1 X B0����
		ra = ALenx * AbsR[2][0] + ALenz * AbsR[0][0];
		rb = BLeny * AbsR[1][2] + BLenz * AbsR[1][1];
		if (fabsf(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)return false;

		//��L=A1 X B1����
		ra = ALenx * AbsR[2][1] + ALenz * AbsR[0][1];
		rb = BLenx * AbsR[1][2] + BLenz * AbsR[1][0];
		if (fabsf(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb)return false;

		//��L=A1 X B2����
		ra = ALenx * AbsR[2][2] + ALenz * AbsR[0][2];
		rb = BLenx * AbsR[1][1] + BLeny * AbsR[1][0];
		if (fabsf(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)return false;

		//��L=A2 X B0����
		ra = ALenx * AbsR[1][0] + ALeny * AbsR[0][0];
		rb = BLeny * AbsR[2][2] + BLenz * AbsR[2][1];
		if (fabsf(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb)return false;

		//��L=A2 X B1����
		ra = ALenx * AbsR[1][1] + ALeny * AbsR[0][1];
		rb = BLenx * AbsR[2][2] + BLenz * AbsR[2][0];
		if (fabsf(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb)return false;

		//��L=A2 X B2����
		ra = ALenx * AbsR[1][2] + ALeny * AbsR[0][2];
		rb = BLenx * AbsR[2][1] + BLeny * AbsR[2][0];
		if (fabsf(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb)return false;

		return true;

	}

	bool ColisionJudge(GameObject& a, GameObject& b,float Scaling)
	{
			D3DXVECTOR3 direction = a.GetPosition() - b.GetPosition(); 

			D3DXVECTOR3 Aobbx, Aobbz, Aobby, Bobbx, Bobbz, Bobby;	//��
			D3DXVECTOR3  NAobbx, NAobbz, NAobby, NBobbx, NBobbz, NBobby;	//���K����̎�
			float AobbLenx, AobbLenz, AobbLeny, BobbLenx, BobbLenz, BobbLeny;
			//-------------A�̃I�u�W�F�N�g���l�擾--------------------
			NAobbx = a.GetObbX();
			AobbLenx = D3DXVec3Length(&NAobbx); //�X�P�[����ς��Ă��Ή��ł���悤�ɐݒ�(bl)
			Aobbx = Aobbx / AobbLenx; //���K���Œ�����1��
			AobbLenx *= Scaling;

			NAobbz = a.GetObbZ();
			AobbLenz = D3DXVec3Length(&NAobbz);
			Aobbz = Aobbz / AobbLenz;
			AobbLenz *= Scaling;

			NAobby = a.GetObbY();
			AobbLeny = D3DXVec3Length(&NAobby);
			Aobby = Aobby / AobbLeny;
			AobbLeny *= Scaling;

			float AlenX, AlenZ, AlenY;
			AlenX = D3DXVec3Dot(&NAobbx, &direction);
			AlenZ = D3DXVec3Dot(&NAobbz, &direction);
			AlenY = D3DXVec3Dot(&NAobby, &direction);

			//-------------B�̃I�u�W�F�N�g���l�擾--------------------
			Bobbx = b.GetObbX();
			BobbLenx = D3DXVec3Length(&Bobbx); //�X�P�[����ς��Ă��Ή��ł���悤�ɐݒ�(bl)
			NBobbx = Bobbx / BobbLenx; //���K���Œ�����1��

			Bobbz = b.GetObbZ();
			BobbLenz = D3DXVec3Length(&Bobbz);
			NBobbz = Bobbz / BobbLenz;

			Bobby = b.GetObbY();
			BobbLeny = D3DXVec3Length(&Bobby);
			NBobby = Bobby / BobbLeny;

			float BlenX, BlenZ, BlenY;
			BlenX = D3DXVec3Dot(&NBobbx, &direction);
			BlenZ = D3DXVec3Dot(&NBobbz, &direction);
			BlenY = D3DXVec3Dot(&NBobby, &direction);

			//-----------------------��r-------------------------------
			float rB = LenSegOnSeparateAxis(&NAobbx, &Bobbx, &Bobby, &Bobbz);
			if (fabs(AlenX) > AobbLenx + rB) return false;	//1
			rB = LenSegOnSeparateAxis(&NAobby, &Bobbx, &Bobby, &Bobbz);
			if (fabs(AlenY) > AobbLeny + rB) return false;	//2
			rB = LenSegOnSeparateAxis(&NAobbz, &Bobbx, &Bobby, &Bobbz);
			if (fabs(AlenZ) > AobbLenz + rB) return false;	//3

			return true;
	}

	bool Destroy()
	{
	  if (objectDestroy)
	  {
		  Uninit();
	      delete this;
	      return true;
	  }
	  else
	  {
			return false;
	  }
	}

	

	D3DXVECTOR3 GetForward()//�O�����x�N�g���擾
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			objRotation.y, objRotation.x, objRotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	D3DXVECTOR3 GetUp()//������x�N�g���擾
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			objRotation.y, objRotation.x, objRotation.z);

		D3DXVECTOR3 up;
		up.x = rot._21;
		up.y = rot._22;
		up.z = rot._23;

		return up;
	}

	D3DXVECTOR3 GetRight()//�������x�N�g���擾
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			objRotation.y, objRotation.x, objRotation.z);

		D3DXVECTOR3 right;
		right.x = rot._11;
		right.y = rot._12;
		right.z = rot._13;

		return right;
	}
	
};