#include "WaterOopth.h"
#include "SafeDelete.h"

WaterOopth* WaterOopth::Create()
{
	// �C���X�^���X�̐���
	WaterOopth* instance = new WaterOopth();

	// �C���X�^���X�̏�����
	instance->Initialize();

	// �C���X�^���X��Ԃ�
	return instance;
}

WaterOopth::~WaterOopth()
{
	//�I�[�v�X
	safe_delete(oopth.obj);
	safe_delete(oopth.model_1);
	safe_delete(oopth.model_2);
	safe_delete(oopth.model_3);
	//�R���W����
	safe_delete(oopth.collision);
	safe_delete(model_Collision);
	//�T�[�N��
	for (int i = 0; i < 3; i++)
	{
		safe_delete(circle.obj[i]);
	}
	safe_delete(circle.model);
	//�o���b�g
	for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
	{
		safe_delete(bullet.obj[i]);
	}
	safe_delete(bullet.model);
}


void WaterOopth::Initialize()
{
#pragma region �����g�p���f��
	model_Collision = Model::CreateFromOBJ("sphere");
#pragma endregion

#pragma region �I�[�v�X������
	//�I�[�v�X�P�E�Q�E�R���f���ǂݍ���
	oopth.model_1 = Model::CreateFromOBJ("oopth1_W");
	oopth.model_2 = Model::CreateFromOBJ("oopth2_W");
	oopth.model_3 = Model::CreateFromOBJ("oopth3_W");

	//����
	oopth.obj = Object3d::Create();

	//�������W
	oopth.pos = { 0.0f, 50.0f, 5.0f };
	oopth.obj->SetPosition(oopth.pos);

	//�������x
	oopth.vel = { 0.0f, 0.0f, 0.0f };

	//���������x
	oopth.acc = 0.01f;

	//������]�p
	oopth.rot = { 0.0f, 0.0f, 0.0f };

	//�T�C�Y
	oopth.obj->SetScale({ 0.3f, 0.3f, 0.3f });

	//���������x
	oopth.alpha1 = 1.0f;
	oopth.alpha2 = 1.0f;
	oopth.alpha3 = 1.0f;

	//�����V�F�C�N�l
	oopth.shake = { 0.0f, 0.0f };

	//��ԕω��ϐ�������
	oopth.startFlag = false;
	oopth.attackPreparationFlag = false;
	oopth.attackFlag = false;
	oopth.statusChangeFlag = false;
	oopth.changeTimer = 150;
	oopth.attackShakeTimer = 0.2f;
	oopth.damegeShakeFlag = false;
	oopth.damegeShakeTimer = 0.0f;
	oopth.attackOrder = 0;
	oopth.attackEndFlag = false;
	oopth.fixFlag = false;
	oopth.fixTimer = 20;

	//Hp�ő�l
	oopth.hp = 300;

	//�R���W����
	oopth.collision = Object3d::Create();
	oopth.collision->SetModel(model_Collision);
	oopth.collision->SetPosition({ oopth.pos.x, oopth.pos.y + 0.5f, oopth.pos.z });
	oopth.radius = 1.0f;
	oopth.collision->SetScale({ oopth.radius, oopth.radius, oopth.radius });

#pragma endregion

#pragma region �T�[�N��������
	//���f���ǂݍ���
	circle.model = Model::CreateFromOBJ("circle_W");

	for (int i = 0; i < 6; i++)
	{
		//����
		circle.obj[i] = Object3d::Create();
		circle.obj[i]->SetModel(circle.model);

		//�������W
		//����
		circle.pos[0] = { -4.5f, 1.5f, +2.7f };
		circle.pos[1] = { -4.5f, 1.5f,  0.0f };
		circle.pos[2] = { -4.5f, 1.5f, -2.7f };
		//�E��
		circle.pos[3] = { +4.5f, 1.5f, +2.7f };
		circle.pos[4] = { +4.5f, 1.5f,  0.0f };
		circle.pos[5] = { +4.5f, 1.5f, -2.7f };
		circle.obj[i]->SetPosition(circle.pos[i]);

		//������]�p
		circle.rot = { 0.0f, 90.0f, 0.0f };

		//�����X�P�[��
		circle.scale = { 0.0f, 0.0f, 1.0f };
	}
#pragma endregion

#pragma region �o���b�g

	//���f��
	bullet.model = Model::CreateFromOBJ("drop");
	for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
	{
		//����
		bullet.obj[i] = Object3d::Create();
		bullet.obj[i]->SetModel(bullet.model);

		//�������W
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

		//������]�p
		if (i < 60)
		{
			//����
			bullet.rot[i] = { 0.0f, 0.0f, +90.0f };
		}
		else
		{
			//�E��
			bullet.rot[i] = { 0.0f, 0.0f, -90.0f };
		}

		//�T�C�Y
		bullet.obj[i]->SetScale({ 0.25f, 0.25f, 0.25f });

		//��ԕω��ϐ�
		bullet.hitFlag[i] = { false };
		bullet.fireFlag[i] = { false };
	}


	//��ԕω��ϐ�������
	bullet.delayTimer = 3;

	//�p�[�e�B�N��
	particleMan = ParticleManager::GetInstance();
#pragma endregion

#pragma region �I�[�f�B�I
	audio = Audio::GetInstance();
#pragma endregion

#pragma region �O�����p�ϐ�
	bool shakeFlag = false;
#pragma endregion
}

#pragma region �I�[�v�X�֐�
void WaterOopth::Reset()
{
	if (oopth.resetFlag == true)
	{
#pragma region �I�[�v�X�ċN��
		//���W
		oopth.pos = { 0.0f, 50.0f, 5.0f };
		//���x
		oopth.vel = { 0.0f, 0.0f, 0.0f };
		//��]�p
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		//�V�F�C�N�l
		oopth.shake = { 0.0f, 0.0f };
		//��ԕω��ϐ�������
		oopth.startFlag = false;
		oopth.attackPreparationFlag = false;
		oopth.attackFlag = false;
		oopth.statusChangeFlag = false;
		oopth.changeTimer = 150;
		oopth.attackShakeTimer = 0.2f;
		oopth.damegeShakeFlag = false;
		oopth.damegeShakeTimer = 0.0f;
		oopth.attackOrder = 0;
		oopth.attackEndFlag = false;
		//Hp�ő�l
		oopth.hp = 300;

#pragma endregion

#pragma region �T�[�N���ċN��
		for (int i = 0; i < 6; i++)
		{
			//��]�p
			circle.rot = { 0.0f, 90.0f, 0.0f };
			//�X�P�[��
			circle.scale = { 0.0f, 0.0f, 1.0f };
		}
#pragma endregion

#pragma region �o���b�g�ċN��
		for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
		{
			//���W
			bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

			//�U������t���b�O
			bullet.hitFlag[i] = false;

			//���˃t���b�O
			bullet.fireFlag[i] = false;
		}
		//��ԕω��ϐ�������
		bullet.delayTimer = 0;
#pragma endregion

		oopth.resetFlag = false;
	}
}

void WaterOopth::Appearance()
{
	if (oopth.pos.y >= 2.0f)
	{
		oopth.pos.y -= oopth.vel.y;
		oopth.vel.y += oopth.acc;
	}
	else
	{
		audio->PlayWave("arrivalSE1.wav", false);
		shakeFlag = true;
		oopth.startFlag = true;
		oopth.statusChangeFlag = true;

	}
}

void WaterOopth::ModelChange(int turnNomber)
{
	//�I�[�v�X1
	if (turnNomber == 1)
	{
		oopth.obj->SetModel(oopth.model_1);
	}

	//�I�[�v�X2
	if (turnNomber == 2)
	{
		oopth.obj->SetModel(oopth.model_2);
	}

	//�I�[�v�X3
	if (turnNomber == 3)
	{
		oopth.obj->SetModel(oopth.model_3);
	}
}

void WaterOopth::StatusChange()
{
	//��U����Ԃ̎�
	if (oopth.statusChangeFlag == true)
	{
		oopth.changeTimer -= 1;
		if (oopth.changeTimer <= 0)
		{
			oopth.attackPreparationFlag = true;
			oopth.statusChangeFlag = false;
		}
	}
}

void WaterOopth::AttackPreparation()
{
	//�U�������t���b�O��ON�Ȃ�
	if (oopth.attackPreparationFlag == true)
	{
		//�ݒ�l�܂ŃT�[�N���̃X�P�[�����g��A�����ɉ�]����
		if (circle.scale.x <= 0.02f && circle.scale.y <= 0.02f)
		{
			//�g��
			circle.scale.x += 0.0005f;
			circle.scale.y += 0.0005f;
			//��]
			circle.rot.x -= 100.0f;
		}
		//���W��ύX
		if (oopth.pos.y <= 5.0f)
		{
			oopth.pos.y += 0.1f;
		}
		//��L�̏������I��
		else
		{
			//�U����Ԃֈڍs
			oopth.attackFlag = true;
			//�U��������Ԃ���O���
			oopth.attackPreparationFlag = false;
			//�T�[�N���̉�]�p�����ɖ߂�
			circle.rot = { 0.0f, 90.0f, 0.0f };
		}

	}
}

void WaterOopth::AttackShake()
{
	oopth.shake.x = (float)rand() / RAND_MAX * oopth.attackShakeTimer - oopth.attackShakeTimer / 2.0f;
	oopth.shake.y = (float)rand() / RAND_MAX * oopth.attackShakeTimer - oopth.attackShakeTimer / 2.0f;
}

void WaterOopth::AttackEnd()
{
	if (oopth.attackEndFlag == true)
	{
		for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
		{
			bullet.hitFlag[i] = false;
			bullet.fireFlag[i] = false;
		}

		if (oopth.pos.y >= 1.0f)
		{
			oopth.pos.y -= 0.1f;
		}

		//�ݒ�l�܂ŃT�[�N���̃X�P�[�����k���A�����ɉ�]����
		if (circle.scale.x >= 0.0f && circle.scale.y >= 0.0f)
		{
			//�g��
			circle.scale.x -= 0.0005f;
			circle.scale.y -= 0.0005f;
			//��]
			circle.rot.x += 100.0f;
		}

		//��L�������I��������
		else
		{
			//StatusChange���ċN��
			oopth.statusChangeFlag = true;
			oopth.changeTimer = 150;
			//AttackEnd��OFF
			oopth.attackEndFlag = false;
		}
	}
}

void WaterOopth::Damage(short player_TypeFlag)
{
	if (player_TypeFlag == 0 || player_TypeFlag == 2)
	{
		oopth.hp -= 3;
	}
	if (player_TypeFlag == 3)
	{
		oopth.hp -= 7;
	}
	if (player_TypeFlag == 1)
	{
		oopth.hp -= 2;
	}
}

void WaterOopth::DamegeShake()
{
	if (oopth.damegeShakeFlag == true)
	{
		oopth.damegeShakeTimer = 0.2f;
		oopth.damegeShakeFlag = false;
	}

	if (oopth.damegeShakeTimer > 0)
	{
		oopth.shake.x = (float)rand() / RAND_MAX * oopth.damegeShakeTimer - oopth.damegeShakeTimer / 2.0f;
		oopth.shake.y = (float)rand() / RAND_MAX * oopth.damegeShakeTimer - oopth.damegeShakeTimer / 2.0f;
		oopth.damegeShakeTimer -= 0.005f;
	}
}

void WaterOopth::Death(int turnNomber)
{
	if (oopth.hp <= 10)
	{
		if (turnNomber == 1)
		{
			if (oopth.alpha1 >= 0.0f)
			{
				oopth.alpha1 -= 0.05f;
			}
			//���S�Ɠ����ɏ�����
			else
			{
				oopth.resetFlag = true;
			}

			oopth.model_1->SetAlpha(oopth.alpha1);
		}

		if (turnNomber == 2)
		{
			if (oopth.alpha2 >= 0.0f)
			{
				oopth.alpha2 -= 0.05f;
			}
			//���S�Ɠ����ɏ�����
			else
			{
				oopth.resetFlag = true;
			}

			oopth.model_2->SetAlpha(oopth.alpha2);
		}

		if (turnNomber == 3)
		{
			if (oopth.alpha3 >= 0.0f)
			{
				oopth.alpha3 -= 0.05f;
			}
			//���S�Ɠ����ɏ�����
			else
			{
				oopth.pos = { 0.0f, 50.0f, 5.0f };
				oopth.startFlag = false;
				for (int i = 0; i < 6; i++)
				{
					//��]�p
					circle.rot = { 0.0f, 90.0f, 0.0f };
					//�X�P�[��
					circle.scale = { 0.0f, 0.0f, 1.0f };
				}
				for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
				{
					//���W
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

					//�U������t���b�O
					bullet.hitFlag[i] = false;

					//���˃t���b�O
					bullet.fireFlag[i] = false;
				}
				oopth.resetFlag = true;
			}

			oopth.model_3->SetAlpha(oopth.alpha3);
		}
	}
}
#pragma endregion

#pragma region �o���b�g�֐�
void WaterOopth::BulletLoading_1()
{
	bullet.delayTimer -= 1;
	for (int i = 0; bullet.delayTimer <= 0 && i < WATEROOPTHBULLET_MAX; i++)
	{
		//�I�[�_�[�P
		if (i < 20 && bullet.fireFlag[i] == false && oopth.attackOrder == 0)
		{
			bullet.pos[i].x = circle.pos[0].x;
			bullet.pos[i].y = (circle.pos[0].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[0].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[19] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�Q
		else if (i < 40 && bullet.fireFlag[i] == false && oopth.attackOrder == 1)
		{
			bullet.pos[i].x = circle.pos[1].x;
			bullet.pos[i].y = (circle.pos[1].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[1].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[39] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�R
		else if (i < 60 && bullet.fireFlag[i] == false && oopth.attackOrder == 2)
		{
			bullet.pos[i].x = circle.pos[2].x;
			bullet.pos[i].y = (circle.pos[2].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[2].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[59] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�S
		else if (i < 80 && bullet.fireFlag[i] == false && oopth.attackOrder == 3)
		{
			bullet.pos[i].x = circle.pos[3].x;
			bullet.pos[i].y = (circle.pos[3].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[3].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[79] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�T
		else if (i < 100 && bullet.fireFlag[i] == false && oopth.attackOrder == 4)
		{
			bullet.pos[i].x = circle.pos[4].x;
			bullet.pos[i].y = (circle.pos[4].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[4].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[99] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�U
		else if (i < 120 && bullet.fireFlag[i] == false && oopth.attackOrder == 5)
		{
			bullet.pos[i].x = circle.pos[5].x;
			bullet.pos[i].y = (circle.pos[5].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[5].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
	}
}

void WaterOopth::BulletLoading_2()
{
	bullet.delayTimer -= 1;
	for (int i = 0; bullet.delayTimer <= 0 && i < WATEROOPTHBULLET_MAX; i++)
	{
		//�I�[�_�[�P
		if (i < 20 && bullet.fireFlag[i] == false && oopth.attackOrder == 0)
		{
			bullet.pos[i].x = circle.pos[4].x;
			bullet.pos[i].y = (circle.pos[4].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[4].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = -90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[19] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�Q
		else if (i < 40 && bullet.fireFlag[i] == false && oopth.attackOrder == 1)
		{
			bullet.pos[i].x = circle.pos[2].x;
			bullet.pos[i].y = (circle.pos[2].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[2].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[39] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�R
		else if (i < 60 && bullet.fireFlag[i] == false && oopth.attackOrder == 2)
		{
			bullet.pos[i].x = circle.pos[3].x;
			bullet.pos[i].y = (circle.pos[3].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[3].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = -90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[59] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�S
		else if (i < 80 && bullet.fireFlag[i] == false && oopth.attackOrder == 3)
		{
			bullet.pos[i].x = circle.pos[1].x;
			bullet.pos[i].y = (circle.pos[1].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[1].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[79] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�T
		else if (i < 100 && bullet.fireFlag[i] == false && oopth.attackOrder == 4)
		{
			bullet.pos[i].x = circle.pos[0].x;
			bullet.pos[i].y = (circle.pos[0].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[0].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[99] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�U
		else if (i < 120 && bullet.fireFlag[i] == false && oopth.attackOrder == 5)
		{
			bullet.pos[i].x = circle.pos[5].x;
			bullet.pos[i].y = (circle.pos[5].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[5].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = -90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
	}
}

void WaterOopth::BulletLoading_3()
{
	bullet.delayTimer -= 1;
	for (int i = 0; bullet.delayTimer <= 0 && i < WATEROOPTHBULLET_MAX; i++)
	{
#pragma region �I�[�_�[�P�E�I�[�_�[�S
		if (oopth.attackOrder == 0 || oopth.attackOrder == 3)
		{
			if (i < 20 && bullet.fireFlag[i] == false)
			{
				bullet.pos[i].x = circle.pos[0].x;
				bullet.pos[i].y = (circle.pos[0].y + (float)rand() / RAND_MAX) - 0.5f;
				bullet.pos[i].z = (circle.pos[0].z + (float)rand() / RAND_MAX) - 0.5f;
				bullet.rot[i].z = 90.0f;
				bullet.fireFlag[i] = true;

				bullet.pos[i + 20].x = circle.pos[3].x;
				bullet.pos[i + 20].y = (circle.pos[3].y + (float)rand() / RAND_MAX) - 0.5f;
				bullet.pos[i + 20].z = (circle.pos[3].z + (float)rand() / RAND_MAX) - 0.5f;
				bullet.rot[i + 20].z = -90.0f;
				bullet.fireFlag[i + 20] = true;
				if (oopth.attackOrder == 3)
				{
					FireFlagReset80_119();
				}
				bullet.delayTimer = 3;
				if (bullet.fireFlag[39] == true)
				{
					oopth.attackOrder += 1;
					bullet.delayTimer = 18;
				}
				audio->StopWave("blastSE.wav");
				audio->PlayWave("blastSE.wav", false);
				break;
			}
		}
#pragma endregion

#pragma region �I�[�_�[�Q�E�I�[�_�[�T
		if (oopth.attackOrder == 1 || oopth.attackOrder == 4)
		{
			//�I�[�_�[�Q
			if (i >= 40 && i < 60 && bullet.fireFlag[i] == false)
			{
				bullet.pos[i].x = circle.pos[1].x;
				bullet.pos[i].y = (circle.pos[1].y + (float)rand() / RAND_MAX) - 0.5f;
				bullet.pos[i].z = (circle.pos[1].z + (float)rand() / RAND_MAX) - 0.5f;
				bullet.rot[i].z = 90.0f;
				bullet.fireFlag[i] = true;

				bullet.pos[i + 20].x = circle.pos[4].x;
				bullet.pos[i + 20].y = (circle.pos[4].y + (float)rand() / RAND_MAX) - 0.5f;
				bullet.pos[i + 20].z = (circle.pos[4].z + (float)rand() / RAND_MAX) - 0.5f;
				bullet.rot[i + 20].z = -90.0f;
				bullet.fireFlag[i + 20] = true;
				bullet.delayTimer = 3;
				if (bullet.fireFlag[79] == true)
				{
					oopth.attackOrder += 1;
					bullet.delayTimer = 18;
				}
				audio->StopWave("blastSE.wav");
				audio->PlayWave("blastSE.wav", false);
				break;
			}
		}
#pragma endregion

#pragma region �I�[�_�[�R�E�I�[�_�[�U
		if (oopth.attackOrder == 2 || oopth.attackOrder == 5)
		{
			//�I�[�_�[�R
			if (i >= 80 && i < 100 && bullet.fireFlag[i] == false)
			{
				bullet.pos[i].x = circle.pos[2].x;
				bullet.pos[i].y = (circle.pos[2].y + (float)rand() / RAND_MAX) - 0.5f;
				bullet.pos[i].z = (circle.pos[2].z + (float)rand() / RAND_MAX) - 0.5f;
				bullet.rot[i].z = 90.0f;
				bullet.fireFlag[i] = true;

				bullet.pos[i + 20].x = circle.pos[5].x;
				bullet.pos[i + 20].y = (circle.pos[5].y + (float)rand() / RAND_MAX) - 0.5f;
				bullet.pos[i + 20].z = (circle.pos[5].z + (float)rand() / RAND_MAX) - 0.5f;
				bullet.rot[i + 20].z = -90.0f;
				bullet.fireFlag[i + 20] = true;
				bullet.delayTimer = 3;
				if (oopth.attackOrder == 2)
				{
					if (bullet.fireFlag[119] == true)
					{
						oopth.attackOrder += 1;
						bullet.delayTimer = 18;
					}
					FireFlagReset0_39();
					FireFlagReset40_79();
				}
				audio->StopWave("blastSE.wav");
				audio->PlayWave("blastSE.wav", false);
				break;
			}
		}

#pragma endregion
	}
}

void WaterOopth::BulletMove_1(int i)
{
	//���t�g
	if (i < 60)
	{
		//��ʊO�ɏo�Ă��Ȃ���
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x += 0.5f;
			//�ݒ���W�ȏ�ɂȂ�����
			if (bullet.pos[i].x >= circle.pos[4].x)
			{
				//��ʊO
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
	//���C�g
	else
	{
		//��ʊO�ɏo�Ă��Ȃ���
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x -= 0.5f;
			//�ݒ���W�ȏ�ɂȂ�����
			if (bullet.pos[i].x <= circle.pos[0].x)
			{
				//��ʊO
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
}

void WaterOopth::BulletMove_2(int i)
{
	if (i >= 0 && i < 20 || i >= 40 && i < 60 || i >= 100 && i < 120)
	{
		//��ʊO�ɏo�Ă��Ȃ���
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x -= 0.5f;
			//�ݒ���W�ȏ�ɂȂ�����
			if (bullet.pos[i].x <= circle.pos[0].x)
			{
				//��ʊO
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
	else if (i >= 20 && i < 40 || i >= 60 && i < 80 || i >= 80 && i < 100)
	{
		//��ʊO�ɏo�Ă��Ȃ���
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x += 0.5f;
			//�ݒ���W�ȏ�ɂȂ�����
			if (bullet.pos[i].x >= circle.pos[4].x)
			{
				//��ʊO
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
}

void WaterOopth::BulletMove_3(int i)
{
	if (i >= 0 && i < 20)
	{
		//��ʊO�ɏo�Ă��Ȃ���
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x += 0.5f;
			//�ݒ���W�ȏ�ɂȂ�����
			if (bullet.pos[i].x >= circle.pos[4].x)
			{
				//��ʊO
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}

		//��ʊO�ɏo�Ă��Ȃ���
		if (bullet.pos[i + 20].y != 100.0f)
		{
			bullet.pos[i + 20].x -= 0.5f;
			//�ݒ���W�ȏ�ɂȂ�����
			if (bullet.pos[i + 20].x <= circle.pos[0].x)
			{
				//��ʊO
				bullet.pos[i + 20] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
	else if (i >= 40 && i < 60)
	{
		//��ʊO�ɏo�Ă��Ȃ���
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x += 0.5f;
			//�ݒ���W�ȏ�ɂȂ�����
			if (bullet.pos[i].x >= circle.pos[4].x)
			{
				//��ʊO
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}

		//��ʊO�ɏo�Ă��Ȃ���
		if (bullet.pos[i + 20].y != 100.0f)
		{
			bullet.pos[i + 20].x -= 0.5f;
			//�ݒ���W�ȏ�ɂȂ�����
			if (bullet.pos[i + 20].x <= circle.pos[0].x)
			{
				//��ʊO
				bullet.pos[i + 20] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
	else if (i >= 80 && i < 100)
	{
		//��ʊO�ɏo�Ă��Ȃ���
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x += 0.5f;
			//�ݒ���W�ȏ�ɂȂ�����
			if (bullet.pos[i].x >= circle.pos[4].x)
			{
				//��ʊO
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}

		//��ʊO�ɏo�Ă��Ȃ���
		if (bullet.pos[i + 20].y != 100.0f)
		{
			bullet.pos[i + 20].x -= 0.5f;
			//�ݒ���W�ȏ�ɂȂ�����
			if (bullet.pos[i + 20].x <= circle.pos[0].x)
			{
				//��ʊO
				bullet.pos[i + 20] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
}

void WaterOopth::FireFlagReset0_39()
{
	for (int i = 0; i < 40; i++)
	{
		bullet.fireFlag[i] = false;
		bullet.hitFlag[i] = false;
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
	}
}

void WaterOopth::FireFlagReset40_79()
{
	for (int i = 40; i < 80; i++)
	{
		bullet.fireFlag[i] = false;
		bullet.hitFlag[i] = false;
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
	}
}

void WaterOopth::FireFlagReset80_119()
{
	for (int i = 80; i < 120; i++)
	{
		bullet.fireFlag[i] = false;
		bullet.hitFlag[i] = false;
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
	}
}

void WaterOopth::BulletEffect()
{
	for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
	{
		const float rnd_pos = 0.1f;
		particle.pos.x = bullet.pos[i].x + (float)rand() / RAND_MAX * rnd_pos;
		particle.pos.y = bullet.pos[i].y + (float)rand() / RAND_MAX * rnd_pos;
		particle.pos.z = bullet.pos[i].z + (float)rand() / RAND_MAX * rnd_pos;

		particle.vel.x = 0.01f;

		const float rnd_acc = 0.001f;
		particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

		particleMan->Add(10, particle.pos, particle.vel, particle.acc, 1.0f, 0.1f, { 0.0f, 0.5f, 5.0f, 1.0f });
	}
}

void WaterOopth::OneSideOrderBlast(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		//��]���o
		oopth.rot.y += 200.0f;
		circle.rot.x += 200.0f;

		//���U
		BulletLoading_1();

		for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
		{
			//���˃t���b�O��ON�Ȃ�
			if (bullet.fireFlag[i] == true)
			{
				//�N��
				BulletMove_1(i);

				//�����蔻��
				if (bullet.hitFlag[i] == true)
				{
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
				}
			}
		}

		//�Ō�̒e�����˂���A��ʊO�ɏo����
		if (bullet.fireFlag[119] == true && bullet.pos[119].y == 100.0f)
		{
			oopth.fixFlag = true;
		}

		if (oopth.fixFlag == true)
		{
			oopth.fixTimer -= 1;
			if (oopth.fixTimer <= 0)
			{
				oopth.fixTimer = 20;
				oopth.fixFlag = false;
				oopth.attackFlag = false;
				oopth.attackEndFlag = true;
			}
		}

		//�o���b�g�G�t�F�N�g�N��
		BulletEffect();
		//�V�F�C�N���o
		AttackShake();

	}
	else if (oopth.attackFlag == false)
	{
		oopth.shake = { 0.0f, 0.0f };
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		oopth.attackOrder = 0;
	}
}

void WaterOopth::TwinSideOrderBlast(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		//��]���o
		oopth.rot.y += 200.0f;
		circle.rot.x += 200.0f;

		//���U
		BulletLoading_2();

		for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
		{
			//���˃t���b�O��ON�Ȃ�
			if (bullet.fireFlag[i] == true)
			{
				//�N��
				BulletMove_2(i);

				//�����蔻��
				if (bullet.hitFlag[i] == true)
				{
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
				}
			}
		}

		//�Ō�̒e�����˂���A��ʊO�ɏo����
		if (bullet.fireFlag[119] == true && bullet.pos[119].y == 100.0f)
		{
			oopth.fixFlag = true;
		}

		if (oopth.fixFlag == true)
		{
			oopth.fixTimer -= 1;
			if (oopth.fixTimer <= 0)
			{
				oopth.fixTimer = 20;
				oopth.fixFlag = false;
				oopth.attackFlag = false;
				oopth.attackEndFlag = true;
			}
		}

		//�o���b�g�G�t�F�N�g�N��
		BulletEffect();
		//�V�F�C�N���o
		AttackShake();
	}
	else if (oopth.attackFlag == false)
	{
		oopth.shake = { 0.0f, 0.0f };
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		oopth.attackOrder = 0;
	}
}

void WaterOopth::TwinLineSideBlast(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		//��]���o
		oopth.rot.y += 200.0f;
		circle.rot.x += 200.0f;

		//���U
		BulletLoading_3();

		for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
		{
			//���˃t���b�O��ON�Ȃ�
			if (bullet.fireFlag[i] == true)
			{
				//�N��
				BulletMove_3(i);

				//�����蔻��
				if (bullet.hitFlag[i] == true)
				{
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
				}
			}
		}

		//�Ō�̒e�����˂���A��ʊO�ɏo����
		if (oopth.attackOrder == 5 && bullet.fireFlag[119] == true && bullet.pos[119].y == 100.0f)
		{
			oopth.fixFlag = true;
		}

		if (oopth.fixFlag == true)
		{
			oopth.fixTimer -= 1;
			if (oopth.fixTimer <= 0)
			{
				oopth.fixTimer = 20;
				oopth.fixFlag = false;
				oopth.attackFlag = false;
				oopth.attackEndFlag = true;
			}
		}

		//�o���b�g�G�t�F�N�g�N��
		BulletEffect();
		//�V�F�C�N���o
		AttackShake();
	}
	else if (oopth.attackFlag == false)
	{
		oopth.shake = { 0.0f, 0.0f };
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		oopth.attackOrder = 0;
	}
}
#pragma endregion


void WaterOopth::Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag)
{
	//�I�[�v�X���ċN��
	Reset();
	if (turnNomber != 0 && turnNomber != 4)
	{
		//�X�e�[�W�ɂ���ă��f����؂�ւ���
		ModelChange(turnNomber);
		if (oopth.startFlag == false && scene_completionFlag == true)
		{
			//�X�^�[�g��
			Appearance();
		}
		else
		{
			//��U����Ԃ���U����Ԃւ̈ڍs
			StatusChange();
			//�U��������Ԃ���U����Ԃ�
			AttackPreparation();
			if (turnNomber == 1)
			{
				//�U���p�^�[��1
				OneSideOrderBlast(field_completionFlag);
			}
			if (turnNomber == 2)
			{
				//�U���p�^�[��2
				TwinSideOrderBlast(field_completionFlag);
			}
			if (turnNomber == 3)
			{
				//�U���p�^�[��3
				TwinLineSideBlast(field_completionFlag);
			}
			//�U���I����
			AttackEnd();
			//��e���̃V�F�C�N���o
			DamegeShake();
			//���S��
			Death(turnNomber);
		}
	}

	oopth.obj->SetPosition({ oopth.pos.x + oopth.shake.x, oopth.pos.y + oopth.shake.y, oopth.pos.z });
	oopth.obj->SetRotation(oopth.rot);
	oopth.obj->Update();
	oopth.collision->SetPosition({ oopth.pos.x, oopth.pos.y + 0.5f, oopth.pos.z });
	oopth.collision->Update();
	for (int i = 0; i < 6; i++)
	{
		circle.obj[i]->SetScale(circle.scale);
		circle.obj[i]->SetRotation(circle.rot);
		circle.obj[i]->Update();
	}
	for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
	{
		bullet.obj[i]->SetPosition(bullet.pos[i]);
		bullet.obj[i]->SetRotation(bullet.rot[i]);
		bullet.obj[i]->Update();
	}
}

void WaterOopth::Draw()
{
	oopth.obj->Draw();
	for (int i = 0; i < 6; i++)
	{
		circle.obj[i]->Draw();
	}
	for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
	{
		if (bullet.fireFlag[i] == true)
		{
			bullet.obj[i]->Draw();
		}

	}
}
