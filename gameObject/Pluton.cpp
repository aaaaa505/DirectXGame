#include "Pluton.h"
#include "SafeDelete.h"
#define _USE_MATH_DEFINES
#include <math.h>


Pluton* Pluton::Create()
{
	// �C���X�^���X�̐���
	Pluton* instance = new Pluton();

	// �C���X�^���X�̏�����
	instance->Initialize();

	// �C���X�^���X��Ԃ�
	return instance;
}

Pluton::~Pluton()
{
	//�{��
	safe_delete(boss.obj);
	safe_delete(boss.model);
	//�R���W����
	safe_delete(boss.collision);
	safe_delete(model_Collision);
	//�T�[�N��
	for (int i = 0; i < 3; i++)
	{
		safe_delete(circle.obj[i]);
	}
	safe_delete(circle.model);
}

void Pluton::Initialize()
{
#pragma region �����g�p���f��
	model_Collision = Model::CreateFromOBJ("sphere");
#pragma endregion

#pragma region �{�X������
	//�I�[�v�X�P�E�Q�E�R���f���ǂݍ���
	boss.model = Model::CreateFromOBJ("pluton");

	//����
	boss.obj = Object3d::Create();
	boss.obj->SetModel(boss.model);

	//�������W
	boss.pos = { 0.0f, 50.0f, 11.0f };//�ό`�U���� z=17 �U���� z = 17 �ʏ펞 z = 11
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
	boss.deathFlag = false;
	boss.attackPreparationFlag = false;
	boss.attackFlag = false;
	boss.statusChangeFlag = false;
	boss.changeTimer = 150;
	boss.attackShakeTimer = 0.2f;
	boss.damegeShakeFlag = false;
	boss.damegeShakeTimer = 0.0f;
	for (int i = 0; i < 10; i++)
	{
		boss.attackOrder[i] = false;
	}
	boss.attackEndFlag = false;
	boss.fixFlag = false;
	boss.fixTimer = 20;
	boss.modeChangeFlag = false;

	//Hp�ő�l
	boss.hp = 400;

	//�R���W����
	boss.collision = Object3d::Create();
	boss.radius = 5.0f;
	boss.collision->SetScale({ boss.radius, boss.radius, boss.radius });

#pragma endregion

#pragma region �T�[�N��������
	//���f���ǂݍ���
	circle.model = Model::CreateFromOBJ("circle_F");

	for (int i = 0; i < 3; i++)
	{
		//����
		circle.obj[i] = Object3d::Create();
		circle.obj[i]->SetModel(circle.model);

		//�������W
		circle.pos[0] = { -2.7f, 1.5f, 5.0f };
		circle.pos[1] = { 0.0f, 1.5f, 5.0f };
		circle.pos[2] = { +2.7f, 1.5f, 5.0f };
		circle.obj[i]->SetPosition(circle.pos[i]);

		//������]�p
		circle.rot = { 0.0f, 0.0f, 0.0f };
		circle.obj[i]->SetRotation(circle.rot);

		//�����X�P�[��
		circle.scale = { 0.0f, 0.0f, 1.0f };
		circle.obj[i]->SetScale(circle.scale);
	}
#pragma endregion

#pragma region �o���b�g�E�p�[�e�B�N��������
	for (int i = 0; i < PLUTONBULLET_MAX; i++)
	{
		//�������W
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

		bullet.hitFlag[i] = false;
	}

	//���a
	bullet.radius = 1.0f;

	//��ԕω��ϐ�������
	bullet.allFireFlag = false;
	bullet.delayFlag = false;
	bullet.delayTimer = 0;

	//�p�[�e�B�N��
	particleMan = ParticleManager::GetInstance();
#pragma endregion

#pragma region �`���[�W������
	charge.angle = 0.0f;
	charge.radius = 15.0f;
	for (int i = 0; i < FLAMECHARGEBULLET_MAX; i++)
	{
		//�������W
		charge.pos[i] = { boss.pos.x + cos(charge.angle + i * 0.628f) * charge.radius, 
						  boss.pos.y + sin(charge.angle + i * 0.628f) * charge.radius,
						  boss.pos.z - 5.0f };
	}
#pragma endregion

#pragma region �t���C��������
	flame.pos = { 0.0f, 100.0f, 0.0f};

	flame.radius = 5.0f;

	//��ԕω��ϐ�
	flame.hitFlag = false;
#pragma endregion

#pragma region �I�[�f�B�I
	audio = Audio::GetInstance();
#pragma endregion


#pragma region �O�����ϐ�
	shakeFlag = false;
#pragma endregion
}

#pragma region �v���g���֐�
void Pluton::Appearance()
{
	if (boss.pos.y >= 3.0f)
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

void Pluton::AttackPreparation()
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
			circle.rot.z -= 100.0f;
		}
		//���W��ύX
		if (boss.pos.z <= 20.0f)//50
		{
			boss.pos.z += 0.1f;
		}
		//��L�̏������I��
		else
		{
			//�U����Ԃֈڍs
			boss.attackFlag = true;
			//�e�𑕓U
			Loading();
			//�U��������Ԃ���O���
			boss.attackPreparationFlag = false;
			//�T�[�N���̉�]�p�����ɖ߂�
			circle.rot = { 0.0f, 0.0f, 0.0f };
		}
	}
}

void Pluton::StatusChange()
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

void Pluton::ModeChange()
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

void Pluton::AttackShake()
{
	boss.shake.x = (float)rand() / RAND_MAX * boss.attackShakeTimer - boss.attackShakeTimer / 2.0f;
	boss.shake.y = (float)rand() / RAND_MAX * boss.attackShakeTimer - boss.attackShakeTimer / 2.0f;
}

void Pluton::AttackEnd()
{
	if (boss.attackEndFlag == true)
	{
		for (int i = 0; i < PLUTONBULLET_MAX; i++)
		{
			bullet.hitFlag[i] = false;
		}

		flame.hitFlag = false;

		//�ݒ�l�܂ŃT�[�N���̃X�P�[�����k���A�����ɉ�]����
		if (circle.scale.x >= 0.0f && circle.scale.y >= 0.0f)
		{
			//�g��
			circle.scale.x -= 0.0005f;
			circle.scale.y -= 0.0005f;
			//��]
			circle.rot.z += 100.0f;

			circle.pos[0].y = 1.5f;
			circle.pos[1].y = 1.5f;
			circle.pos[2].y = 1.5f;
		}
		if(boss.pos.z >= 11.0f)
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
			//AllFireFlag��OFF
			bullet.allFireFlag = false;
			//AttackEnd��OFF
			boss.attackEndFlag = false;
		}

		if (boss.modeChangeFlag == true)
		{
			if (boss.rot.y > 0.0f)
			{
				boss.rot.y -= 3.0f;
			}
			else
			{
				boss.rot.y = 0.0f;
			}
		}
	}
}

void Pluton::DamegeShake()
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

void Pluton::Damage(short player_TypeFlag)
{
	if (player_TypeFlag == 0 || player_TypeFlag == 1)
	{
		boss.hp -= 3;
	}
	if (player_TypeFlag == 2)
	{
		boss.hp -= 7;
	}
	if (player_TypeFlag == 3)
	{
		boss.hp -= 2;
	}
	if (boss.hp < 200)
	{
		boss.modeChangeFlag = true;
	}
}

void Pluton::Death(int turnNomber)
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
void Pluton::Loading()
{
	//�N�����Ɋe�T�[�N���̗��ɒe���O���z�u
	for (int i = 0; i < PLUTONBULLET_MAX; i++)
	{
		if (i < 6)
		{
			bullet.pos[i] = { circle.pos[0].x, circle.pos[0].y, circle.pos[0].z + 0.6f };
		}
		else if (i < 12)
		{
			bullet.pos[i] = { circle.pos[1].x, circle.pos[1].y, circle.pos[1].z + 0.6f };
		}
		else
		{
			bullet.pos[i] = { circle.pos[2].x, circle.pos[2].y, circle.pos[2].z + 0.6f };
		}
	}
}

void Pluton::FireEffect()
{
	for (int i = 0; i < PLUTONBULLET_MAX; i++)
	{
		const float rnd_pos = 0.1f;
		particle.pos.x = bullet.pos[i].x + (float)rand() / RAND_MAX * rnd_pos;
		particle.pos.y = bullet.pos[i].y + (float)rand() / RAND_MAX * rnd_pos;
		particle.pos.z = bullet.pos[i].z + (float)rand() / RAND_MAX * rnd_pos;

		particle.vel.z = 0.01f;

		const float rnd_acc = 0.001f;
		particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

		particleMan->Add(18, particle.pos, particle.vel, particle.acc, 1.8f, 0.1f, { 2.0f, 1.0f, 0.0f, 1.0f });
	}
}

void Pluton::ChargeEffect(bool field_completionFlag)
{
	if (boss.modeChangeFlag == true && field_completionFlag == true && boss.attackOrder[9] == false)
	{
		audio->PlayWave("chargeSE.wav", false);
		flame.pos = { boss.pos.x, 1.7f, boss.pos.z - 0.5f };
		boss.pos.x = -1.25f;

		if (charge.radius >= 0.0f)
		{
			charge.radius -= 0.3f;
			charge.angle += 1.54f;
		}
		else
		{
			charge.radius = 15.0f;
		}

		for (int i = 0; i < FLAMECHARGEBULLET_MAX; i++)
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

			particleMan->Add(18, particle.pos, particle.vel, particle.acc, 1.8f, 0.1f, { 2.0f, 0.5f, 0.0f, 1.0f });
		}
	}
}

void Pluton::FlameEffect()
{

	const float rnd_pos = 0.1f;
	particle.pos.x = flame.pos.x + (float)rand() / RAND_MAX * rnd_pos;
	particle.pos.y = flame.pos.y + (float)rand() / RAND_MAX * rnd_pos;
	particle.pos.z = flame.pos.z + (float)rand() / RAND_MAX * rnd_pos;

	particle.vel.z = 0.01f;

	const float rnd_acc = 0.001f;
	particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

	particleMan->Add(18, particle.pos, particle.vel, particle.acc, 5.0f, 4.0f, { 2.0f, 1.0f, 0.0f, 1.0f });
}

void Pluton::OneLineOrderShot()
{
	//�I�[�_�[�P
	if (boss.attackOrder[0] == true)
	{
		bullet.pos[6].z -= 0.4f;
	}

	//�I�[�_�[�Q
	if (boss.attackOrder[1] == true)
	{
		bullet.pos[0].z -= 0.25f;
	}

	//�I�[�_�[�R
	if (boss.attackOrder[2] == true)
	{
		bullet.pos[12].z -= 0.25;
	}
}

void Pluton::TwinLineOrderShot()
{
	//�I�[�_�[�S
	if (boss.attackOrder[3] == true)
	{
		bullet.pos[1].z -= 0.25f;
		bullet.pos[7].z -= 0.25f;
		
	}

	//�I�[�_�[�T
	if (boss.attackOrder[4] == true)
	{
		bullet.pos[8].z -= 0.25f;
		bullet.pos[13].z -= 0.25f;
	}

	//�I�[�_�[�U
	if (boss.attackOrder[5] == true)
	{
		bullet.pos[9].z -= 0.25f;
		bullet.pos[2].z -= 0.25f;
	}
}

void Pluton::AllLineOrderShot()
{
	//�I�[�_�[�V
	if (boss.attackOrder[6] == true)
	{
		bullet.pos[3].z -= 0.25f;
		bullet.pos[10].z -= 0.25f;
		bullet.pos[14].z -= 0.15f;
	}

	//�I�[�_�[�W
	if (boss.attackOrder[7] == true)
	{
		bullet.pos[4].z -= 0.15f;
		bullet.pos[15].z -= 0.15f;
	}

	//�I�[�_�[�X
	if (boss.attackOrder[8] == true)
	{
		bullet.pos[5].z -= 0.15f;
		bullet.pos[11].z -= 0.25f;
		bullet.pos[16].z -= 0.25f;
	}
}

void Pluton::TwinLineFlame()
{
	if (boss.attackOrder[9] == true)
	{
		audio->StopWave("chargeSE.wav");
		flame.pos.z -= 0.4f;
		FlameEffect();
		bullet.allFireFlag = true;
		if (flame.pos.z <= -8.0f)
		{
			boss.fixFlag = true;
		}
		
	}
}

void Pluton::FlameShot1(bool field_completionFlag)
{
	if (boss.attackFlag == true && field_completionFlag == true)
	{
		//��]���o
		circle.rot.z += 200.0f;

		//�S�e���˂��Ă��Ȃ��Ȃ�
		if (bullet.allFireFlag == false)
		{
			//�f�B���C�^�C�}�[�N��
			bullet.delayTimer -= 1;
		}
		for (int i = 0; i < 10 && bullet.delayTimer <= 0; i++)
		{
			if (boss.attackOrder[i] == false)
			{
				audio->StopWave("shotSE.wav");
				audio->PlayWave("shotSE.wav", false);
				boss.attackOrder[i] = true;
				bullet.delayTimer = 50;
				break;
			}
		}

		for (int i = 0; i < PLUTONBULLET_MAX; i++)
		{
			if (bullet.hitFlag[i] == true)
			{
				bullet.pos[i].y = 100.0f;
			}
		}

		OneLineOrderShot();
		
		TwinLineOrderShot();
		
		AllLineOrderShot();

		if (boss.attackOrder[8] == true)
		{
			bullet.allFireFlag = true;
			if (bullet.pos[5].z <= -8.0f)
			{
				boss.fixFlag = true;
			}
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
		//�t�@�C���[�G�t�F�N�g�N��
		FireEffect();
		//�V�F�C�N�N��
		AttackShake();
	}
	else if (boss.attackFlag == false)
	{
		boss.shake = { 0.0f, 0.0f };
		for (int i = 0; i < 9; i++)
		{
			boss.attackOrder[i] = false;
		}
	}
}

void Pluton::FlameShot2(bool field_completionFlag)
{
	if (boss.attackFlag == true && field_completionFlag == true)
	{
		//��]���o
		circle.rot.z += 200.0f;

		//�S�e���˂��Ă��Ȃ��Ȃ�
		if (bullet.allFireFlag == false)
		{
			//�f�B���C�^�C�}�[�N��
			bullet.delayTimer -= 1;
		}
		for (int i = 0; i < 10 && bullet.delayTimer <= 0; i++)
		{
			if (boss.attackOrder[i] == false)
			{
				audio->StopWave("shotSE.wav");
				audio->PlayWave("shotSE.wav", false);
				boss.attackOrder[i] = true;
				bullet.delayTimer = 50;
				break;
			}
		}

		for (int i = 0; i < PLUTONBULLET_MAX; i++)
		{
			if (bullet.hitFlag[i] == true)
			{
				bullet.pos[i].y = 100.0f;
			}
		}

		OneLineOrderShot();

		TwinLineOrderShot();

		AllLineOrderShot();

		if (boss.attackOrder[8] == true)
		{
			circle.pos[0].y = 100.0f;
			circle.pos[1].y = 100.0f;
			circle.pos[2].y = 100.0f;
		}

		TwinLineFlame();
			
		if (boss.fixFlag == true)
		{
			boss.fixTimer -= 1;
			if (boss.fixTimer <= 0)
			{
				flame.pos = { 0.0f, 100.0f, 0.0f };
				boss.fixTimer = 20;
				boss.fixFlag = false;
				boss.attackFlag = false;
				boss.attackEndFlag = true;
			}
		}
		//�t�@�C���[�G�t�F�N�g�N��
		FireEffect();
		//�V�F�C�N�N��
		AttackShake();
	}
	else if (boss.attackFlag == false)
	{
		boss.shake = { 0.0f, 0.0f };
		for (int i = 0; i < 10; i++)
		{
			boss.attackOrder[i] = false;
		}
	}
}
#pragma endregion

void Pluton::Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag)
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
				FlameShot1(field_completionFlag);
			}
			else
			{
				//�U���p�^�[���Q
				FlameShot2(field_completionFlag);
			}
			//�`���[�W���o
			ChargeEffect(field_completionFlag);
			//�U���I����
			AttackEnd();
			//��e���̃V�F�C�N���o
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
	for (int i = 0; i < 3; i++)
	{
		circle.obj[i]->SetPosition(circle.pos[i]);
		circle.obj[i]->SetScale(circle.scale);
		circle.obj[i]->SetRotation(circle.rot);
		circle.obj[i]->Update();
	}
}

void Pluton::Draw()
{
	boss.obj->Draw();
	for (int i = 0; i < 3; i++)
	{
		circle.obj[i]->Draw();
	}
}
