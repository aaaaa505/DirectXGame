#include "Mana.h"
#include <stdlib.h>
#include <time.h>
#include "SafeDelete.h"

Mana* Mana::Create()
{
	// �C���X�^���X�̐���
	Mana* instance = new Mana();

	// �C���X�^���X�̏�����
	instance->Initialize();

	// �C���X�^���X��Ԃ�
	return instance;
}

Mana::~Mana()
{
	safe_delete(mana);
	safe_delete(model_F);
	safe_delete(model_W);
	safe_delete(model_L);
	safe_delete(guide);
	safe_delete(model_guide);
}

void Mana::Initialize()
{
#pragma region �}�i
	//���f���ǂݍ���
	model_F = Model::CreateFromOBJ("mana_F");
	model_W = Model::CreateFromOBJ("mana_W");
	model_L = Model::CreateFromOBJ("mana_L");

	//����
	mana = Object3d::Create();

	//�ړ���
	vel = { 0.0f, 0.0f, 0.0f };

	//�X�P�[��
	radius = 0.35f;
	mana->SetScale({ radius, radius, radius });

	//�A���t�@
	alpha = 1.0f;

	//�����_���ϐ�
	srand(time(NULL));
	placementFlag = rand() % 9;
	typeFlag = rand() % 3;
	moveTimer = (rand() % 101) + 50;

	//�p�[�e�B�N��
	particleMan = ParticleManager::GetInstance();

	//��ԕω��ϐ�
	completionFlag = false;
	moveFlag = false;
	settingFlag = true;
	hitFlag = false;
	resetFlag = false;
#pragma endregion

#pragma region �K�C�h�I�u�W�F�N�g
	//���f���ǂݍ���
	model_guide = Model::CreateFromOBJ("guide_Get");

	//�����E���f���Z�b�g
	guide = Object3d::Create();
	guide->SetModel(model_guide);

	//��]�p����
	guide->SetRotation({ 0.0f, -90.0f, 0.0f });
	//�X�P�[������
	guide->SetScale({ 0.2f, 0.2f, 0.2f });
#pragma endregion


}

void Mana::TypeChange()
{

	if (typeFlag == WATER_ELEMENT)
	{
		particle.color = { 0.0f, 0.5f, 5.0f, 1.0f };
		mana->SetModel(model_W);
	}
	if (typeFlag == FIRE_ELEMENT)
	{
		particle.color = { 2.0f, 0.5f, 0.0f, 1.0f };
		mana->SetModel(model_F);
	}
	if (typeFlag == LEAF_ELEMENT)
	{
		particle.color = { 0.2f, 2.0f, 0.0f, 1.0f };
		mana->SetModel(model_L);
	}
}

void Mana::ManaEffect()
{
	const float rnd_pos = 1.0f;
	particle.pos.x = pos.x + (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	particle.pos.y = pos.y + (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	particle.pos.z = pos.z + (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	const float rnd_vel = 0.01f;
	particle.vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	particle.vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	particle.vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	particle.acc = { 0.0f, 0.0f, 0.0f };


	particleMan->Add(100, particle.pos, particle.vel, particle.acc, 0.3f, 0.1f, particle.color);
}

void Mana::RandamSetting()
{
	if (settingFlag == true)
	{
#pragma region �����
		if (placementFlag == 0)
		{
			pos = { -2.7f,14.0f,+2.7f };
		}
		if (placementFlag == 1)
		{
			pos = { -2.7f,14.0f, 0.0f };
		}
		if (placementFlag == 2)
		{
			pos = { -2.7f,14.0f,-2.7f };
		}
#pragma endregion

#pragma region �������
		if (placementFlag == 3)
		{
			pos = { 0.0f,14.0f,+2.7f };
		}
		if (placementFlag == 4)
		{
			pos = { 0.0f,14.0f, 0.0f };
		}
		if (placementFlag == 5)
		{
			pos = { 0.0f,14.0f,-2.7f };
		}
#pragma endregion

#pragma region �E���
		if (placementFlag == 6)
		{
			pos = { +2.7f,14.0f,+2.7f };
		}
		if (placementFlag == 7)
		{
			pos = { +2.7f,14.0f, 0.0f };
		}
		if (placementFlag == 8)
		{
			pos = { +2.7f,14.0f,-2.7f };
		}
#pragma endregion

		settingFlag = false;
	}
}

void Mana::Move()
{
	if (moveFlag == false)
	{
		if (moveTimer >= 0)
		{
			moveTimer -= 1;
		}
		else
		{
			moveFlag = true;
		}
	}

	if (completionFlag == false && moveFlag == true)
	{
		if (pos.y >= 0.5f)
		{
			vel.y = 0.1f;
			pos.y -= vel.y;
		}
		else
		{
			vel.y = 0.0f;
			if (alpha >= 0.0f)
			{
				alpha -= 0.05f;
			}
			else
			{
				completionFlag = true;
				moveFlag = false;
				pos.y = 14.0f;
				resetFlag = true;
			}
		}

	}
}

void Mana::Reset()
{
	if (resetFlag == true)
	{
		alpha = 1.0f;
		placementFlag = rand() % 9;
		typeFlag = rand() % 3;
		moveTimer = (rand() % 101) + 50;
		moveFlag = false;
		completionFlag = false;
		settingFlag = true;
		hitFlag = false;

		resetFlag = false;
	}

}

void Mana::Update(bool oopth_StartFlag, bool boss_StartFlag)
{
	//���Z�b�g
	Reset();
	//�����ύX
	TypeChange();
	//�����_���z�u
	RandamSetting();
	if (oopth_StartFlag == true || boss_StartFlag == true)
	{
		//����
		Move();
		//�G�t�F�N�g
		ManaEffect();
	}
	mana->SetPosition(pos);
	guide->SetPosition({ pos.x + 0.5f, pos.y + 0.5f, pos.z });
	model_F->SetAlpha(alpha);
	model_W->SetAlpha(alpha);
	model_L->SetAlpha(alpha);
	model_guide->SetAlpha(alpha);
	mana->Update();
	guide->Update();
}

void Mana::Draw()
{
	mana->Draw();
	guide->Draw();
}
