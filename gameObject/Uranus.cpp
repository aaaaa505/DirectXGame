#include "Uranus.h"
#include "SafeDelete.h"
#define _USE_MATH_DEFINES
#include <math.h>

Uranus* Uranus::Create()
{
	// �C���X�^���X�̐���
	Uranus* instance = new Uranus();

	// �C���X�^���X�̏�����
	instance->Initialize();

	// �C���X�^���X��Ԃ�
	return instance;
}

Uranus::~Uranus()
{
	//�{��
	safe_delete(boss.obj);
	safe_delete(boss.model);
	//�R���W����
	safe_delete(boss.collision);
	safe_delete(model_Collision);
	//�T�[�N��
	for (int i = 0; i < 9; i++)
	{
		safe_delete(circle.obj[i]);
	}
	safe_delete(circle.model);
	//�E�B���h
	for (int i = 0; i < 10; i++)
	{
		safe_delete(wind.obj[i]);
	}
	safe_delete(wind.model);

}

void Uranus::Initialize()
{
#pragma region �����g�p���f��
	model_Collision = Model::CreateFromOBJ("sphere");
#pragma endregion

#pragma region �{�X������

	//�I�[�v�X�P�E�Q�E�R���f���ǂݍ���
	boss.model = Model::CreateFromOBJ("uranus");

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
	circle.model = Model::CreateFromOBJ("circle_L");

	for (int i = 0; i < 9; i++)
	{
		//����
		circle.obj[i] = Object3d::Create();
		circle.obj[i]->SetModel(circle.model);

		//�������W
		//�O��
		circle.pos[0] = { -2.7f, 0.1f, +2.7f };
		circle.pos[1] = { 0.0f, 0.1f, +2.7f };
		circle.pos[2] = { +2.7f, 0.1f, +2.7f };
		//������
		circle.pos[3] = { -2.7f, 0.1f, 0.0f };
		circle.pos[4] = { 0.0f, 0.1f, 0.0f };
		circle.pos[5] = { +2.7f, 0.1f, 0.0f };
		//���
		circle.pos[6] = { -2.7f, 0.1f, -2.7f };
		circle.pos[7] = { 0.0f, 0.1f, -2.7f };
		circle.pos[8] = { +2.7f, 0.1f, -2.7f };
		circle.obj[i]->SetPosition(circle.pos[i]);

		//������]�p
		circle.rot = { 0.0f, 0.0f, 0.0f };

		//�����X�P�[��
		circle.scale = { 0.0f, 1.0f, 0.0f };
	}
#pragma endregion

#pragma region �E�B���h�֐�
	//���f��
	wind.model = Model::CreateFromOBJ("wind");
	for (int i = 0; i < 9; i++)
	{
		//����
		wind.obj[i] = Object3d::Create();
		wind.obj[i]->SetModel(wind.model);

		//�������W
		wind.pos[i] = { circle.pos[i].x, circle.pos[i].y - 1.0f, circle.pos[i].z };
		wind.obj[i]->SetPosition(wind.pos[i]);

		//������]�p
		wind.rot = { 0.0f, 0.0f, 0.0f };

		//�T�C�Y
		wind.scale[i] = { 0.0f, 0.0f, 0.0f };

		//��ԕω��ϐ�
		wind.outBreakFlag[i] = false;
	}
	//�����A���t�@�l
	wind.alpha = 0.8f;
	wind.outBreakTimer = 10;
	wind.delayTimer = 0;
	wind.delayFlag = false;

#pragma region �e���y�X�g�p
	//����
	wind.obj[9] = Object3d::Create();
	wind.obj[9]->SetModel(wind.model);

	//�������W
	wind.pos[9] = {1.25f, 1.5f, 7.0f};
	wind.obj[9]->SetPosition(wind.pos[9]);

	//������]�p
	wind.tempestRot = { -90.0f, 0.0f, 0.0f };

	//�T�C�Y
	wind.scale[9] = { 0.0f, 0.0f, 0.0f };

	//��ԕω��ϐ�
	wind.outBreakFlag[9] = false;
#pragma endregion

	//��ԕω��ϐ�������
	wind.outBreakTimer = 10;
	wind.delayTimer = 5;

	//�p�[�e�B�N��
	particleMan = ParticleManager::GetInstance();
#pragma endregion

#pragma region �`���[�W������
	charge.angle = 0.0f;
	charge.radius = 5.0f;
	for (int i = 0; i < TEMPESTCHARGEBULLET_MAX; i++)
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

#pragma region �E���k�X�֐�
void Uranus::Appearance()
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

void Uranus::ModeChange()
{
	if (boss.modeChangeFlag == true)
	{
		if (boss.rot.y < 180.0f)
		{
			boss.rot.y += 3.0f;
		}
		else
		{
			boss.rot.y = 180.0f;
		}
	}
}

void Uranus::StatusChange()
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

void Uranus::AttackPreparation()
{
	//�U�������t���b�O��ON�Ȃ�
	if (boss.attackPreparationFlag == true)
	{
		ModeChange();
		//�ݒ�l�܂ŃT�[�N���̃X�P�[�����g��A�����ɉ�]����
		if (circle.scale.x <= 0.02f && circle.scale.z <= 0.02f)
		{
			//�g��
			circle.scale.x += 0.0005f;
			circle.scale.z += 0.0005f;
			//��]
			circle.rot.y -= 100.0f;
		}
		//���W��ύX
		if (boss.pos.z <= 20.0f)
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
			circle.rot = { 0.0f, 0.0f, 0.0f };
		}
	}
}

void Uranus::AttackEnd()
{
	if (boss.attackEndFlag == true)
	{
		audio->StopWave("windSE.wav");
		for (int i = 0; i < 10; i++)
		{
			wind.outBreakTimer = 10;
			wind.outBreakFlag[i] = false;
			wind.scale[i] = { 0.0f, 0.0f, 0.0f };
		}

		if (boss.pos.z >= 11.0f)
		{
			boss.pos.x = 0.0f;
			boss.pos.z -= 0.1f;
		}
		//��L�������I��������
		else
		{
			//StatusChange���ċN��
			boss.statusChangeFlag = true;
			boss.changeTimer = 150;
			//AttackEnd��OFF
			boss.attackEndFlag = false;
		}

		//�ݒ�l�܂ŃT�[�N���̃X�P�[�����k���A�����ɉ�]����
		if (circle.scale.x >= 0.0f && circle.scale.z >= 0.0f)
		{
			//�g��
			circle.scale.x -= 0.0005f;
			circle.scale.z -= 0.0005f;

			//��]
			circle.rot.y += 100.0f;

			for (int i = 0; i < 9; i++)
			{
				circle.pos[i].y = 0.1f;
			}
		}

		if (boss.modeChangeFlag == true)
		{
			if (boss.rot.y > 0.0f)
			{
				boss.rot.y -= 4.0f;
			}
			else
			{
				boss.rot.y = 0.0f;
			}
		}
	}
}

void Uranus::Damage(short player_TypeFlag)
{
	if (player_TypeFlag == 0 || player_TypeFlag == 3)
	{
		boss.hp -= 3;
	}
	if (player_TypeFlag == 1)
	{
		boss.hp -= 7;
	}
	if (player_TypeFlag == 2)
	{
		boss.hp -= 2;
	}
	if (boss.hp < 200)
	{
		boss.modeChangeFlag = true;
	}
}

void Uranus::Death(int turnNomber)
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

void Uranus::AttackShake()
{
	boss.shake.x = (float)rand() / RAND_MAX * boss.attackShakeTimer - boss.attackShakeTimer / 2.0f;
	boss.shake.y = (float)rand() / RAND_MAX * boss.attackShakeTimer - boss.attackShakeTimer / 2.0f;
}

void Uranus::DamegeShake()
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
#pragma endregion

#pragma region �E�B���h�֐�
void Uranus::WindMove()
{
	for (int i = 0; i < 9; i++)
	{
		if (wind.outBreakFlag[i] == true)
		{
			wind.alpha = 0.8f;
			if (wind.scale[i].x <= 1.5f)
			{
				wind.scale[i].x += 0.05f;
				wind.scale[i].z += 0.05f;
			}

			if (wind.scale[i].y <= 1.5f)
			{
				wind.scale[i].y += 0.1f;
			}
		}
		else
		{
			if (wind.alpha >= 0.0f)
			{
				wind.alpha -= 0.05f;
			}
			else
			{
				wind.scale[i] = { 0.0f, 0.0f, 0.0f };
			}
		}
	}
}

void Uranus::TempestMove()
{
	if (wind.outBreakFlag[9] == true)
	{
		TempestEfect();
		wind.tempestRot.y += 20.0f;
		wind.alpha = 1.0f;
		if (wind.scale[9].x <= 2.0f)
		{
			wind.scale[9].x += 0.1f;
			wind.scale[9].z += 0.1f;
		}

		if (wind.scale[9].y <= 3.0f)
		{
			wind.scale[9].y += 0.1f;
		}
	}
	else
	{
		if (wind.alpha >= 0.0f)
		{
			wind.alpha -= 0.01f;
		}
		else
		{
			wind.scale[9] = { 0.0f, 0.0f, 0.0f };
		}
	}

	if (boss.attackOrder == 13)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[0] = false;
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[6] = false;
			wind.outBreakFlag[7] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[9] = true;
		}
	}
}

void Uranus::WindEffect()
{
	for (int i = 0; i < 10; i++)
	{
		if (wind.outBreakFlag[i] == true)
		{
			const XMFLOAT3 rnd_pos = { 3.0f, 5.0f, 1.0f };
			particle.pos.x = wind.pos[i].x + (float)rand() / RAND_MAX * rnd_pos.x - rnd_pos.x / 2.0f;
			particle.pos.y = wind.pos[i].y + 3.0f + (float)rand() / RAND_MAX * rnd_pos.y - rnd_pos.y / 2.0f;
			particle.pos.z = wind.pos[i].z + (float)rand() / RAND_MAX * rnd_pos.z - rnd_pos.z / 2.0f;

			const float rnd_vel = 0.01f;
			particle.vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			particle.vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			particle.vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			const float rnd_acc = 0.001f;
			particle.acc.y = (float)rand() / RAND_MAX * rnd_acc;

			particleMan->Add(20, particle.pos, particle.vel, particle.acc, 0.5f, 0.1f, { 0.5f, 2.0f, 0.0f, 1.0f });
		}

	}
}

void Uranus::ChargeEffect(bool field_completionFlag)
{
	if (boss.modeChangeFlag == true && field_completionFlag == true && boss.attackOrder != 13)
	{
		audio->PlayWave("chargeSE.wav", false);
		boss.pos.x = 1.25f;

		if (charge.radius >= 0.0f)
		{
			charge.radius -= 0.3f;
			charge.angle += 0.7f;
		}
		else
		{
			charge.radius = 15.0f;
		}

		for (int i = 0; i < TEMPESTCHARGEBULLET_MAX; i++)
		{
			charge.pos[i].x = boss.pos.x + cos(charge.angle + i * 0.628f) * charge.radius;
			charge.pos[i].y = 1.7f + sin(charge.angle + i * 0.628f) * charge.radius;

			const float rnd_pos = 0.01f;
			particle.pos.x = charge.pos[i].x + (float)rand() / RAND_MAX * rnd_pos;
			particle.pos.y = charge.pos[i].y + (float)rand() / RAND_MAX * rnd_pos;
			particle.pos.z = charge.pos[i].z + (float)rand() / RAND_MAX * rnd_pos;

			particle.vel.x = 0.01f;

			const float rnd_acc = 0.0f;
			particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

			particleMan->Add(18, particle.pos, particle.vel, particle.acc, 1.8f, 0.1f, { 0.1f, 2.0f, 0.0f, 1.0f });
		}
	}
}

void Uranus::TempestEfect()
{
	if (wind.outBreakFlag[9] == true)
	{
		audio->StopWave("chargeSE.wav");
		const XMFLOAT3 rnd_pos = { 3.0f, 1.0f, 5.0f };
		particle.pos.x = wind.pos[9].x + (float)rand() / RAND_MAX * rnd_pos.x - rnd_pos.x / 2.0f;
		particle.pos.y = wind.pos[9].y + (float)rand() / RAND_MAX * rnd_pos.y - rnd_pos.y / 2.0f;
		particle.pos.z = wind.pos[9].z - 10.0f + (float)rand() / RAND_MAX * rnd_pos.z - rnd_pos.z / 2.0f;

		const float rnd_vel = 0.01f;
		particle.vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		particle.vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		particle.vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		const float rnd_acc = 0.001f;
		particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

		particleMan->Add(20, particle.pos, particle.vel, particle.acc, 1.0f, 0.1f, { 0.5f, 2.0f, 0.0f, 1.0f });
	}

	
}

void Uranus::CrossWind()
{
	//�I�[�_�[�P
	if (boss.attackOrder == 1)
	{
		wind.outBreakFlag[0] = true;
		wind.outBreakFlag[1] = true;
		wind.outBreakFlag[2] = true;
		wind.outBreakFlag[5] = true;
		wind.outBreakFlag[8] = true;
	}

	//�I�[�_�[�Q
	if (boss.attackOrder == 2)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[0] = false;
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[2] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[8] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[7] = true;
		}

	}

	//�I�[�_�[�R
	if (boss.attackOrder == 3)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[7] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[0] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[6] = true;
			wind.outBreakFlag[7] = true;
			wind.outBreakFlag[8] = true;
		}
	}

	//�I�[�_�[�S
	if (boss.attackOrder == 4)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[0] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[6] = false;
			wind.outBreakFlag[7] = false;
			wind.outBreakFlag[8] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[7] = true;
		}
	}
}

void Uranus::SlantingWind()
{
	//�I�[�_�[�P
	if (boss.attackOrder == 5)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[7] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[2] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[6] = true;
		}
	}

	//�I�[�_�[�Q
	if (boss.attackOrder == 6)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[2] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[6] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[7] = true;
		}

	}

	//�I�[�_�[�R
	if (boss.attackOrder == 7)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[7] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[0] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[8] = true;
		}
	}

	//�I�[�_�[�S
	if (boss.attackOrder == 8)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[0] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[8] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[7] = true;
		}
	}
}

void Uranus::BlockWind()
{
	//�I�[�_�[�P
	if (boss.attackOrder == 9)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[7] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[0] = true;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[2] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[5] = true;
		}
	}

	//�I�[�_�[�Q
	if (boss.attackOrder == 10)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[0] = false;
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[2] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[5] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[6] = true;
			wind.outBreakFlag[7] = true;
			wind.outBreakFlag[8] = true;
		}

	}

	//�I�[�_�[�R
	if (boss.attackOrder == 11)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[6] = false;
			wind.outBreakFlag[7] = false;
			wind.outBreakFlag[8] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[2] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[7] = true;
			wind.outBreakFlag[8] = true;
		}
	}

	//�I�[�_�[�S
	if (boss.attackOrder == 12)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[2] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[7] = false;
			wind.outBreakFlag[8] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[0] = true;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[6] = true;
			wind.outBreakFlag[7] = true;
		}
	}
}

void Uranus::TempestWind1(bool field_completionFlag)
{
	if (boss.attackFlag == true && field_completionFlag == true)
	{
		audio->PlayWave("windSE.wav", true);
		//��]���o
		circle.rot.y += 200.0f;
		wind.rot.y += 20.0f;


		if (wind.delayFlag == false)
		{
			wind.outBreakTimer -= 1;
		}
		else
		{
			audio->StopWave("windSE.wav");
			wind.delayTimer -= 1;
		}
		if (wind.outBreakTimer <= 0)
		{
			if (boss.attackOrder <= 11)
			{
				boss.attackOrder += 1;
				wind.outBreakTimer = 150;
				wind.delayFlag = true;
				wind.delayTimer = 30;
				if (boss.attackOrder == 1)
				{
					wind.delayFlag = false;
				}
			}
			else
			{
				//�U���\�t���b�OOFF
				boss.attackFlag = false;
				//�U���I���t���b�OON
				boss.attackEndFlag = true;
			}
		}

		CrossWind();
		SlantingWind();
		BlockWind();

		

		//�E�B���h����
		WindMove();
		//�o���b�g�G�t�F�N�g�N��
		WindEffect();
		//�V�F�C�N���o
		AttackShake();

	}
	else if (boss.attackFlag == false)
	{
		boss.shake = { 0.0f, 0.0f };
		boss.attackOrder = 0;
	}
}

void Uranus::TempestWind2(bool field_completionFlag)
{
	if (boss.attackFlag == true && field_completionFlag == true)
	{
		audio->PlayWave("windSE.wav", true);
		//��]���o
		circle.rot.y += 200.0f;
		wind.rot.y += 20.0f;


		if (wind.delayFlag == false)
		{
			wind.outBreakTimer -= 1;
		}
		else
		{
			audio->StopWave("windSE.wav");
			wind.delayTimer -= 1;
		}
		if (wind.outBreakTimer <= 0)
		{
			if (boss.attackOrder <= 12)
			{
				boss.attackOrder += 1;
				wind.outBreakTimer = 150;
				wind.delayFlag = true;
				wind.delayTimer = 30;
				if (boss.attackOrder == 1)
				{
					wind.delayFlag = false;
				}
			}
			else
			{
				//�U���\�t���b�OOFF
				boss.attackFlag = false;
				//�U���I���t���b�OON
				boss.attackEndFlag = true;
			}
		}

		CrossWind();
		SlantingWind();
		BlockWind();

		//�E�B���h����
		WindMove();
		//�e���y�X�g����
		TempestMove();
		//�o���b�g�G�t�F�N�g�N��
		WindEffect();
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


void Uranus::Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag)
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
				TempestWind1(field_completionFlag);
			}
			else
			{
				//�U���p�^�[���Q
				TempestWind2(field_completionFlag);
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
	wind.model->SetAlpha(wind.alpha);
	for (int i = 0; i < 9; i++)
	{
		circle.obj[i]->SetPosition(circle.pos[i]);
		circle.obj[i]->SetScale(circle.scale);
		circle.obj[i]->SetRotation(circle.rot);
		circle.obj[i]->Update();

		wind.obj[i]->SetScale(wind.scale[i]);
		wind.obj[i]->SetRotation(wind.rot);
		wind.obj[i]->Update();

	}
	wind.obj[9]->SetScale(wind.scale[9]);
	wind.obj[9]->SetRotation(wind.tempestRot);
	wind.obj[9]->Update();
}

void Uranus::Draw()
{
	boss.obj->Draw();
	for (int i = 0; i < 9; i++)
	{
		circle.obj[i]->Draw();

		wind.obj[i]->Draw();
	}

	wind.obj[9]->Draw();
}
