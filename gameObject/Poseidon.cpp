#include "Poseidon.h"
#include "SafeDelete.h"
#define _USE_MATH_DEFINES
#include <math.h>

Poseidon* Poseidon::Create()
{
	// �C���X�^���X�̐���
	Poseidon* instance = new Poseidon();

	// �C���X�^���X�̏�����
	instance->Initialize();

	// �C���X�^���X��Ԃ�
	return instance;
}

Poseidon::~Poseidon()
{
	//�{��
	safe_delete(boss.obj);
	safe_delete(boss.model);
	//�R���W����
	safe_delete(boss.collision);
	safe_delete(model_Collision);
	//�T�[�N��
	for (int i = 0; i < 6; i++)
	{
		safe_delete(circle.obj[i]);
	}
	safe_delete(circle.model);
	//�e
	for (int i = 0; i < POSEIDONBULLET_MAX; i++)
	{
		safe_delete(bullet.obj[i]);
	}
	safe_delete(bullet.model);
}

void Poseidon::Initialize()
{
#pragma region �����g�p���f��
	model_Collision = Model::CreateFromOBJ("sphere");
#pragma endregion

#pragma region �{�X������

	//�I�[�v�X�P�E�Q�E�R���f���ǂݍ���
	boss.model = Model::CreateFromOBJ("poseidon");

	//����
	boss.obj = Object3d::Create();
	boss.obj->SetModel(boss.model);

	//�������W
	boss.pos = { 0.0f, 50.0f, 10.0f };//�ό`�U���� z=17 �U���� z = 17 �ʏ펞 z = 10
	boss.obj->SetPosition(boss.pos);

	//�������x
	boss.vel = { 0.0f, 0.0f, 0.0f };

	//���������x
	boss.acc = 0.01f;

	//������]�p
	boss.rot = { 0.0f, 0.0f, 0.0f };

	//���������x
	boss.alpha = 1.0f;

	//�����V�F�C�N�l
	boss.shake = { 0.0f, 0.0f };

	//��ԕω��ϐ�������
	boss.startFlag = false;
	boss.attackPreparationFlag = false;
	boss.attackFlag = false;
	boss.statusChangeFlag = false;
	boss.changeTimer = 150;
	boss.attackShakeTimer = 0.2f;
	boss.damegeShakeFlag = false;
	boss.damegeShakeTimer = 0.0f;
	boss.attackOrder = 0;
	boss.attackEndFlag = false;
	boss.fixFlag = false;
	boss.fixTimer = 20;
	boss.modeChangeFlag = false;
	boss.deathFlag = false;

	//Hp�ő�l
	boss.hp = 400;

	//�R���W����
	boss.collision = Object3d::Create();
	boss.radius = 5.0f;
	boss.collision->SetScale({ boss.radius, boss.radius, boss.radius });

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
	for (int i = 0; i < POSEIDONBULLET_MAX; i++)
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

#pragma region �`���[�W������
	charge.angle = 0.0f;
	charge.radius = 5.0f;
	for (int i = 0; i < SPLASHCHARGEBULLET_MAX; i++)
	{
		//�������W
		charge.pos[i] = { boss.pos.x + cos(charge.angle + i * 0.628f) * charge.radius,
						  boss.pos.y + sin(charge.angle + i * 0.628f) * charge.radius,
						  boss.pos.z - 5.0f };
	}
#pragma endregion

#pragma region �I�[�f�B�I
	audio = Audio::GetInstance();
#pragma endregion


#pragma region �O�����p�ϐ�
	shakeFlag = false;
#pragma endregion
}

#pragma region �|�Z�C�h���֐�
void Poseidon::Appearance()
{
	if (boss.pos.y >= 2.0f)
	{
		boss.pos.y -= boss.vel.y;
		boss.vel.y += boss.acc;
	}
	else
	{
		audio->PlayWave("arrivalSE2.wav", false);
		shakeFlag = true;
		boss.startFlag = true;
		boss.statusChangeFlag = true;

	}
}

void Poseidon::AttackPreparation()
{
	//�U�������t���b�O��ON�Ȃ�
	if (boss.attackPreparationFlag == true)
	{
		ModeChange();
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
		if (boss.pos.z <= 15.0f)
		{
			boss.pos.z += 0.1f;
		}
		//��L�̏������I��
		else
		{
			//�U����Ԃֈڍs
			boss.attackFlag = true;
			//�U��������Ԃ���O���
			boss.attackPreparationFlag = false;
			//�T�[�N���̉�]�p�����ɖ߂�
			circle.rot = { 0.0f, 90.0f, 0.0f };
		}
	}
}

void Poseidon::StatusChange()
{
	//��U����Ԃ̎�
	if (boss.statusChangeFlag == true)
	{
		boss.changeTimer -= 1;
		if (boss.changeTimer <= 0)
		{
			boss.attackPreparationFlag = true;
			boss.statusChangeFlag = false;
		}
	}
}

void Poseidon::ModeChange()
{
	if (boss.modeChangeFlag == true)
	{
		if (boss.rot.y < 180.0f)
		{
			boss.rot.y += 5.0f;
		}
		else
		{
			boss.rot.y = 180.0f;
		}
	}
}

void Poseidon::AttackShake()
{
	boss.shake.x = (float)rand() / RAND_MAX * boss.attackShakeTimer - boss.attackShakeTimer / 2.0f;
	boss.shake.y = (float)rand() / RAND_MAX * boss.attackShakeTimer - boss.attackShakeTimer / 2.0f;
}

void Poseidon::DamegeShake()
{
	if (boss.damegeShakeFlag == true)
	{
		boss.damegeShakeTimer = 0.2f;
		boss.damegeShakeFlag = false;
	}

	if (boss.damegeShakeTimer > 0)
	{
		boss.shake.x = (float)rand() / RAND_MAX * boss.damegeShakeTimer - boss.damegeShakeTimer / 2.0f;
		boss.shake.y = (float)rand() / RAND_MAX * boss.damegeShakeTimer - boss.damegeShakeTimer / 2.0f;
		boss.damegeShakeTimer -= 0.005f;
	}
}

void Poseidon::AttackEnd()
{
	if (boss.attackEndFlag == true)
	{
		for (int i = 0; i < POSEIDONBULLET_MAX; i++)
		{
			bullet.hitFlag[i] = false;
			bullet.fireFlag[i] = false;
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
		if (boss.pos.z >= 10.0f)
		{
			boss.pos.x = 0.0f;
			boss.pos.z -= 0.1f;
		}
		//��L�������I��������
		else
		{
			for (int i = 0; i < 6; i++)
			{
				circle.pos[i].y = 1.5f;
			}
			//StatusChange���ċN��
			boss.statusChangeFlag = true;
			boss.changeTimer = 150;
			//AttackEnd��OFF
			boss.attackEndFlag = false;
		}

		if (boss.modeChangeFlag == true)
		{
			if (boss.rot.y > 0.0f)
			{
				boss.rot.y -= 5.0f;
			}
			else
			{
				boss.rot.y = 0.0f;
			}
		}
	}
}

void Poseidon::Damage(short player_TypeFlag)
{
	if (player_TypeFlag == 0 || player_TypeFlag == 2)
	{
		boss.hp -= 3;
	}
	if (player_TypeFlag == 3)
	{
		boss.hp -= 7;
	}
	if (player_TypeFlag == 1)
	{
		boss.hp -= 2;
	}
	if (boss.hp < 200)
	{
		boss.modeChangeFlag = true;
	}
}

void Poseidon::Death(int turnNomber)
{
	if (boss.hp <= 0)
	{
		if (boss.alpha >= 0.0f)
		{
			boss.alpha -= 0.05f;
		}
		//���S�Ɠ����ɏ�����
		else
		{
			boss.deathFlag = true;
		}

		boss.model->SetAlpha(boss.alpha);
	}
}
#pragma endregion

#pragma region �o���b�g�֐�
void Poseidon::BulletEffect()
{
	for (int i = 0; i < POSEIDONBULLET_MAX; i++)
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

void Poseidon::ChargeEffect(bool field_completionFlag)
{
	if (boss.modeChangeFlag == true && field_completionFlag == true && boss.attackOrder != 18)
	{
		audio->PlayWave("chargeSE.wav", false);
		if (charge.radius >= 0.0f)
		{
			charge.radius -= 0.05f;
			charge.angle += 0.1f;
		}
		else
		{
			charge.radius = 5.0f;
		}

		for (int i = 0; i < SPLASHCHARGEBULLET_MAX; i++)
		{
			if (i < SPLASHCHARGEBULLET_MAX / 2)
			{
				charge.pos[i].x = boss.pos.x - 2.2f + cos(charge.angle + i * 0.628f) * charge.radius;
			}
			else
			{
				charge.pos[i].x = boss.pos.x + 2.2f + cos(charge.angle + i * 0.628f) * charge.radius;
			}
			charge.pos[i].y = 1.7f + sin(charge.angle + i * 0.628f) * charge.radius;

			const float rnd_pos = 0.01f;
			particle.pos.x = charge.pos[i].x + (float)rand() / RAND_MAX * rnd_pos;
			particle.pos.y = charge.pos[i].y + (float)rand() / RAND_MAX * rnd_pos;
			particle.pos.z = charge.pos[i].z + (float)rand() / RAND_MAX * rnd_pos;

			particle.vel.x = 0.01f;

			const float rnd_acc = 0.0f;
			particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

			particleMan->Add(18, particle.pos, particle.vel, particle.acc, 1.8f, 0.1f, { 0.0f, 0.05f, 2.0f, 0.1f });
		}
	}
}

void Poseidon::BulletLoading_1()
{
	bullet.delayTimer -= 1;
	for (int i = 0; bullet.delayTimer <= 0 && i < 120; i++)
	{
		//�I�[�_�[�P
		if (i < 20 && bullet.fireFlag[i] == false && boss.attackOrder == 0)
		{
			bullet.pos[i].x = circle.pos[0].x;
			bullet.pos[i].y = (circle.pos[0].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[0].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[19] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�Q
		else if (i < 40 && bullet.fireFlag[i] == false && boss.attackOrder == 1)
		{
			bullet.pos[i].x = circle.pos[1].x;
			bullet.pos[i].y = (circle.pos[1].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[1].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[39] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�R
		else if (i < 60 && bullet.fireFlag[i] == false && boss.attackOrder == 2)
		{
			bullet.pos[i].x = circle.pos[2].x;
			bullet.pos[i].y = (circle.pos[2].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[2].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[59] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�S
		else if (i < 80 && bullet.fireFlag[i] == false && boss.attackOrder == 3)
		{
			bullet.pos[i].x = circle.pos[3].x;
			bullet.pos[i].y = (circle.pos[3].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[3].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[79] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�T
		else if (i < 100 && bullet.fireFlag[i] == false && boss.attackOrder == 4)
		{
			bullet.pos[i].x = circle.pos[4].x;
			bullet.pos[i].y = (circle.pos[4].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[4].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[99] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�U
		else if (i < 120 && bullet.fireFlag[i] == false && boss.attackOrder == 5)
		{
			bullet.pos[i].x = circle.pos[5].x;
			bullet.pos[i].y = (circle.pos[5].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[5].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[119] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
	}
}

void Poseidon::BulletLoading_2()
{
	bullet.delayTimer -= 1;
	for (int i = 120; bullet.delayTimer <= 0 && i < 240; i++)
	{
		//�I�[�_�[�P
		if (i < 140 && bullet.fireFlag[i] == false && boss.attackOrder == 6)
		{
			bullet.pos[i].x = circle.pos[4].x;
			bullet.pos[i].y = (circle.pos[4].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[4].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = -90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[139] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�Q
		else if (i < 160 && bullet.fireFlag[i] == false && boss.attackOrder == 7)
		{
			bullet.pos[i].x = circle.pos[2].x;
			bullet.pos[i].y = (circle.pos[2].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[2].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[159] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�R
		else if (i < 180 && bullet.fireFlag[i] == false && boss.attackOrder == 8)
		{
			bullet.pos[i].x = circle.pos[3].x;
			bullet.pos[i].y = (circle.pos[3].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[3].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = -90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[179] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�S
		else if (i < 200 && bullet.fireFlag[i] == false && boss.attackOrder == 9)
		{
			bullet.pos[i].x = circle.pos[1].x;
			bullet.pos[i].y = (circle.pos[1].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[1].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[199] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�T
		else if (i < 220 && bullet.fireFlag[i] == false && boss.attackOrder == 10)
		{
			bullet.pos[i].x = circle.pos[0].x;
			bullet.pos[i].y = (circle.pos[0].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[0].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[219] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//�I�[�_�[�U
		else if (i < 240 && bullet.fireFlag[i] == false && boss.attackOrder == 11)
		{
			bullet.pos[i].x = circle.pos[5].x;
			bullet.pos[i].y = (circle.pos[5].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[5].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = -90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[239] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
	}
}

void Poseidon::BulletLoading_3()
{
	bullet.delayTimer -= 1;
	for (int i = 240; bullet.delayTimer <= 0 && i < 360; i++)
	{
#pragma region �I�[�_�[�P�Q�E�I�[�_�[�S
		if (boss.attackOrder == 12 || boss.attackOrder == 15)
		{
			if (i < 260 && bullet.fireFlag[i] == false)
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
				if (boss.attackOrder == 15)
				{
					FireFlagReset320_359();
				}
				bullet.delayTimer = 3;
				if (bullet.fireFlag[259] == true)
				{
					boss.attackOrder += 1;
					bullet.delayTimer = 10;
				}
				audio->StopWave("blastSE.wav");
				audio->PlayWave("blastSE.wav", false);
				break;
			}
		}
#pragma endregion

#pragma region �I�[�_�[�P�R�E�I�[�_�[�T
		if (boss.attackOrder == 13 || boss.attackOrder == 16)
		{
			if (i >= 280 && i < 300 && bullet.fireFlag[i] == false)
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
				if (bullet.fireFlag[299] == true)
				{
					boss.attackOrder += 1;
					bullet.delayTimer = 10;
				}
				audio->StopWave("blastSE.wav");
				audio->PlayWave("blastSE.wav", false);
				break;
			}
		}
#pragma endregion

#pragma region �I�[�_�[�P�S�E�I�[�_�[�U
		if (boss.attackOrder == 14 || boss.attackOrder == 17)
		{
			//�I�[�_�[�R
			if (i >= 320 && i < 340 && bullet.fireFlag[i] == false)
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
				if (boss.attackOrder == 14)
				{
					if (bullet.fireFlag[339] == true)
					{
						boss.attackOrder += 1;
						bullet.delayTimer = 10;
					}
					FireFlagReset240_279();
					FireFlagReset280_319();
				}
				if (boss.attackOrder == 17 && bullet.fireFlag[339] == true && boss.modeChangeFlag == true)
				{
					boss.attackOrder += 1;
					bullet.delayTimer = 20;
				}
				audio->StopWave("blastSE.wav");
				audio->PlayWave("blastSE.wav", false);
				break;
			}
		}

#pragma endregion
	}
}

void Poseidon::BulletLoading_4()
{
	bullet.delayTimer -= 1;
	//����
	circle.pos[0].y = 100.0f;
	circle.pos[1].y = 100.0f;
	circle.pos[2].y = 100.0f;
	//�E��		
	circle.pos[3].y = 100.0f;
	circle.pos[4].y = 100.0f;
	circle.pos[5].y = 100.0f;
	audio->StopWave("chargeSE.wav");
	for (int i = 360; bullet.delayTimer <= 0 && i < 400; i++)
	{
		if (bullet.fireFlag[i] == false && boss.attackOrder == 18)
		{
			bullet.pos[i].x = ((boss.pos.x - 2.7f) + (float)rand() / RAND_MAX) - 0.5;
			bullet.pos[i].y = (1.5f + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = boss.pos.z - 8.0f;
			bullet.rot[i].x = 90.0f;
			bullet.fireFlag[i] = true;

			bullet.pos[i + 40].x = ((boss.pos.x + 2.7f) + (float)rand() / RAND_MAX) - 0.5;
			bullet.pos[i + 40].y = (1.5f + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i + 40].z = boss.pos.z - 8.0f;
			bullet.rot[i + 40].x = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
	}
}

void Poseidon::BulletMove_1(int i)
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
	else if (i >= 60 && i < 120)
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

void Poseidon::BulletMove_2(int i)
{
	if (i >= 120 && i < 140 || i >= 160 && i < 180 || i >= 220 && i < 240)
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
	else if (i >= 140 && i < 160 || i >= 180 && i < 200 || i >= 200 && i < 220)
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

void Poseidon::BulletMove_3(int i)
{
	if (i >= 240 && i < 260)
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
	else if (i >= 280 && i < 300)
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
	else if (i >= 320 && i < 340)
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

void Poseidon::BulletMove_4(int i)
{
	if (bullet.fireFlag[i] == true)
	{
		if (i >= 360 && i < 400)
		{
			//��ʊO�ɏo�Ă��Ȃ���
			if (bullet.pos[i].y != 100.0f)
			{
				bullet.pos[i].z -= 0.5f;
				//�ݒ���W�ȏ�ɂȂ�����
				if (bullet.pos[i].z <= -8.0f)
				{
					//��ʊO
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
				}
			}

			//��ʊO�ɏo�Ă��Ȃ���
			if (bullet.pos[i + 40].y != 100.0f)
			{
				bullet.pos[i + 40].z -= 0.5f;
				//�ݒ���W�ȏ�ɂȂ�����
				if (bullet.pos[i + 40].z <= -8.0f)
				{
					//��ʊO
					bullet.pos[i + 40] = { 0.0f, 100.0f, 0.0f };
				}
			}
		}
	}
}

void Poseidon::FireFlagReset240_279()
{
	for (int i = 240; i < 280; i++)
	{
		bullet.fireFlag[i] = false;
		bullet.hitFlag[i] = false;
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
	}
}

void Poseidon::FireFlagReset280_319()
{
	for (int i = 280; i < 320; i++)
	{
		bullet.fireFlag[i] = false;
		bullet.hitFlag[i] = false;
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
	}
}

void Poseidon::FireFlagReset320_359()
{
	for (int i = 320; i < 360; i++)
	{
		bullet.fireFlag[i] = false;
		bullet.hitFlag[i] = false;
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
	}
}

void Poseidon::SplashBlast1(bool field_completionFlag)
{
	if (boss.attackFlag == true && field_completionFlag == true)
	{
		//��]���o
		circle.rot.x += 200.0f;

		//���U
		if (boss.attackOrder < 6)
		{
			BulletLoading_1();
		}
		else if (boss.attackOrder >= 6 && boss.attackOrder < 12)
		{
			BulletLoading_2();
		}
		else
		{
			BulletLoading_3();
		}

		for (int i = 0; i < POSEIDONBULLET_MAX; i++)
		{
			//���˃t���b�O��ON�Ȃ�
			if (bullet.fireFlag[i] == true)
			{
				//�N��
				BulletMove_1(i);
				BulletMove_2(i);
				BulletMove_3(i);

				//�����蔻��
				if (bullet.hitFlag[i] == true)
				{
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
				}
			}
		}

		//�Ō�̒e�����˂���A��ʊO�ɏo����
		if (boss.attackOrder == 17 && bullet.fireFlag[359] == true && bullet.pos[359].y == 100.0f)
		{
			boss.fixFlag = true;

		}

		if (boss.fixFlag == true)
		{
			boss.fixTimer -= 1;
			if (boss.fixTimer <= 0)
			{
				boss.fixTimer = 20;
				boss.fixFlag = false;
				boss.attackFlag = false;
				boss.attackEndFlag = true;
			}
		}

		//�o���b�g�G�t�F�N�g�N��
		BulletEffect();
		//�V�F�C�N���o
		AttackShake();

	}
	else if (boss.attackFlag == false)
	{
		boss.shake = { 0.0f, 0.0f };
		boss.rot = { 0.0f, 0.0f, 0.0f };
		boss.attackOrder = 0;
	}
}

void Poseidon::SplashBlast2(bool field_completionFlag)
{
	if (boss.attackFlag == true && field_completionFlag == true)
	{
		//��]���o
		circle.rot.x += 200.0f;

		//���U
		if (boss.attackOrder < 6)
		{
			BulletLoading_1();
		}
		else if (boss.attackOrder >= 6 && boss.attackOrder < 12)
		{
			BulletLoading_2();
		}
		else if (boss.attackOrder >= 12 && boss.attackOrder < 18)
		{
			BulletLoading_3();
		}
		else
		{
			BulletLoading_4();
		}

		for (int i = 0; i < POSEIDONBULLET_MAX; i++)
		{
			//���˃t���b�O��ON�Ȃ�
			if (bullet.fireFlag[i] == true)
			{
				//�N��
				BulletMove_1(i);
				BulletMove_2(i);
				BulletMove_3(i);
				BulletMove_4(i);

				//�����蔻��
				if (bullet.hitFlag[i] == true)
				{
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
				}
			}
		}

		//�Ō�̒e�����˂���A��ʊO�ɏo����
		if (boss.attackOrder == 18 && bullet.fireFlag[399] == true && bullet.pos[399].y == 100.0f)
		{
			boss.fixFlag = true;
		}

		if (boss.fixFlag == true)
		{
			boss.fixTimer -= 1;
			if (boss.fixTimer <= 0)
			{
				boss.fixTimer = 20;
				boss.fixFlag = false;
				boss.attackFlag = false;
				boss.attackEndFlag = true;
			}
		}
		//�o���b�g�G�t�F�N�g�N��
		BulletEffect();
		//�V�F�C�N���o
		AttackShake();

	}
	else if (boss.attackFlag == false)
	{
		boss.shake = { 0.0f, 0.0f };
		boss.attackOrder = 0;
	}
}
#pragma endregion


void Poseidon::Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag)
{
	//�X�^�[�g��
	if (turnNomber == 4)
	{
		if (boss.startFlag == false && scene_completionFlag == true)
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
			if (boss.modeChangeFlag == false)
			{
				//�U���p�^�[���P
				SplashBlast1(field_completionFlag);
			}
			else
			{
				//�U���p�^�[���Q
				SplashBlast2(field_completionFlag);
			}
			//�`���[�W���o
			ChargeEffect(field_completionFlag);
			//�U���I����
			AttackEnd();
			////��e���̃V�F�C�N���o
			DamegeShake();
			//���S��
			Death(turnNomber);
		}
	}

	boss.obj->SetPosition({ boss.pos.x + boss.shake.x, boss.pos.y + boss.shake.y, boss.pos.z });
	boss.obj->SetRotation(boss.rot);
	boss.obj->Update();
	boss.collision->SetPosition(boss.pos);
	boss.collision->Update();
	for (int i = 0; i < 6; i++)
	{
		circle.obj[i]->SetPosition(circle.pos[i]);
		circle.obj[i]->SetScale(circle.scale);
		circle.obj[i]->SetRotation(circle.rot);
		circle.obj[i]->Update();
	}
	for (int i = 0; i < POSEIDONBULLET_MAX; i++)
	{
		bullet.obj[i]->SetPosition(bullet.pos[i]);
		bullet.obj[i]->SetRotation(bullet.rot[i]);
		bullet.obj[i]->Update();
	}
}

void Poseidon::Draw()
{
	boss.obj->Draw();
	for (int i = 0; i < 6; i++)
	{
		circle.obj[i]->Draw();
	}
	for (int i = 0; i < POSEIDONBULLET_MAX; i++)
	{
		if (bullet.fireFlag[i] == true)
		{
			bullet.obj[i]->Draw();
		}
	}
}
