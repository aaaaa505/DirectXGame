#include "LeafOopth.h"
#include "SafeDelete.h"

LeafOopth* LeafOopth::Create()
{
	// �C���X�^���X�̐���
	LeafOopth* instance = new LeafOopth();

	// �C���X�^���X�̏�����
	instance->Initialize();

	// �C���X�^���X��Ԃ�
	return instance;
}

LeafOopth::~LeafOopth()
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
	for (int i = 0; i < 9; i++)
	{
		safe_delete(circle.obj[i]);
	}
	safe_delete(circle.model);
	//�E�B���h
	for (int i = 0; i < 9; i++)
	{
		safe_delete(wind.obj[i]);
	}
	safe_delete(wind.model);
}

void LeafOopth::Initialize()
{
#pragma region �����g�p���f��
	model_Collision = Model::CreateFromOBJ("sphere");
#pragma endregion

#pragma region �I�[�v�X������
	//�I�[�v�X�P�E�Q�E�R���f���ǂݍ���
	oopth.model_1 = Model::CreateFromOBJ("oopth1_L");
	oopth.model_2 = Model::CreateFromOBJ("oopth2_L");
	oopth.model_3 = Model::CreateFromOBJ("oopth3_L");

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
	circle.model = Model::CreateFromOBJ("circle_L");

	for (int i = 0; i < 9; i++)
	{
		//����
		circle.obj[i] = Object3d::Create();
		circle.obj[i]->SetModel(circle.model);

		//�������W
		//�O��
		circle.pos[0] = { -2.7f, 0.1f, +2.7f };
		circle.pos[1] = {  0.0f, 0.1f, +2.7f };
		circle.pos[2] = { +2.7f, 0.1f, +2.7f };
		//������
		circle.pos[3] = { -2.7f, 0.1f, 0.0f };
		circle.pos[4] = {  0.0f, 0.1f, 0.0f };
		circle.pos[5] = { +2.7f, 0.1f, 0.0f };
		//���
		circle.pos[6] = { -2.7f, 0.1f, -2.7f };
		circle.pos[7] = {  0.0f, 0.1f, -2.7f };
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
		wind.pos[i] = {circle.pos[i].x, circle.pos[i].y - 1.0f, circle.pos[i].z};
		wind.obj[i]->SetPosition(wind.pos[i]);

		//������]�p
		wind.rot = { 0.0f, 0.0f, 0.0f };

		//�T�C�Y
		wind.scale[i] = { 0.0f, 0.0f, 0.0f };

		//�����A���t�@�l
		wind.alpha = 0.8f;

		//��ԕω��ϐ�
		wind.outBreakFlag[i] = false;
	}

	//��ԕω��ϐ�������
	wind.outBreakTimer = 10;
	wind.delayTimer = 5;

	//�p�[�e�B�N��
	particleMan = ParticleManager::GetInstance();
#pragma endregion

#pragma region �I�[�f�B�I
	audio = Audio::GetInstance();
#pragma endregion

#pragma region �O�����p�ϐ�
	shakeFlag = false;
#pragma endregion
}

#pragma region �I�[�v�X�֐�
void LeafOopth::Reset()
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
		oopth.damegeShakeFlag = false;
		oopth.damegeShakeTimer = 0.0f;
		oopth.attackShakeTimer = 0.2f;
		oopth.attackOrder = 0;

		oopth.attackEndFlag = false;
		//Hp�ő�l
		oopth.hp = 300;

#pragma endregion

#pragma region �T�[�N���ċN��
		for (int i = 0; i < 9; i++)
		{
			//��]�p
			circle.rot = { 0.0f, 0.0f, 0.0f };
			//�X�P�[��
			circle.scale = { 0.0f, 1.0f, 0.0f };
		}
#pragma endregion

#pragma region �E�B���h�ċN��
		for (int i = 0; i < 9; i++)
		{
			//������]�p
			wind.rot = { 0.0f, 0.0f, 0.0f };

			//�T�C�Y
			wind.scale[i] = { 0.0f, 0.0f, 0.0f };

			//��ԕω��ϐ�
			wind.outBreakFlag[i] = false;
		}

		//��ԕω��ϐ�������
		wind.outBreakTimer = 10;
		wind.delayTimer = 0;
#pragma endregion

		oopth.resetFlag = false;
	}
}

void LeafOopth::ModelChange(int turnNomber)
{
	//�I�[�v�X�P
	if (turnNomber == 1)
	{
		oopth.obj->SetModel(oopth.model_1);
	}

	//�I�[�v�X�Q
	if (turnNomber == 2)
	{
		oopth.obj->SetModel(oopth.model_2);
	}

	//�I�[�v�X�R
	if (turnNomber == 3)
	{
		oopth.obj->SetModel(oopth.model_3);
	}
}

void LeafOopth::Appearance()
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

void LeafOopth::StatusChange()
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

void LeafOopth::AttackPreparation()
{
	//�U�������t���b�O��ON�Ȃ�
	if (oopth.attackPreparationFlag == true)
	{
		//�ݒ�l�܂ŃT�[�N���̃X�P�[�����g��A�����ɉ�]����
		if (circle.scale.x <= 0.022f && circle.scale.z <= 0.022f)
		{
			//�g��
			circle.scale.x += 0.0005f;
			circle.scale.z += 0.0005f;
			//��]
			circle.rot.y -= 100.0f;
		}
		//��L�̏������I��
		else
		{
			//�U����Ԃֈڍs
			oopth.attackFlag = true;
			//�U��������Ԃ���O���
			oopth.attackPreparationFlag = false;
			//�T�[�N���̉�]�p�����ɖ߂�
			circle.rot = { 0.0f, 0.0f, 0.0f };
		}

		//���W��ύX
		if (oopth.pos.y <= 5.0f)
		{
			oopth.pos.y += 0.1f;
		}
	}
}

void LeafOopth::AttackEnd()
{
	if (oopth.attackEndFlag == true)
	{
		audio->StopWave("windSE.wav");
		for (int i = 0; i < 9; i++)
		{
			wind.outBreakTimer = 10;
			wind.outBreakFlag[i] = false;
			wind.scale[i] = { 0.0f, 0.0f, 0.0f };
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
			circle.scale.z -= 0.0005f;
			//��]
			circle.rot.y += 100.0f;
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

void LeafOopth::AttackShake()
{
	oopth.shake.x = (float)rand() / RAND_MAX * oopth.attackShakeTimer - oopth.attackShakeTimer / 2.0f;
	oopth.shake.y = (float)rand() / RAND_MAX * oopth.attackShakeTimer - oopth.attackShakeTimer / 2.0f;
}

void LeafOopth::Damage(short player_TypeFlag)
{
	if (player_TypeFlag == 0 || player_TypeFlag == 3)
	{
		oopth.hp -= 3;
	}
	if (player_TypeFlag == 1)
	{
		oopth.hp -= 7;
	}
	if (player_TypeFlag == 2)
	{
		oopth.hp -= 2;
	}
}

void LeafOopth::DamegeShake()
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

void LeafOopth::Death(int turnNomber)
{
	if (oopth.hp <= 5)
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
				for (int i = 0; i < 9; i++)
				{
					//��]�p
					circle.rot = { 0.0f, 0.0f, 0.0f };
					//�X�P�[��
					circle.scale = { 0.0f, 1.0f, 0.0f };
				}
				for (int i = 0; i < 9; i++)
				{
					//������]�p
					wind.rot = { 0.0f, 0.0f, 0.0f };

					//�T�C�Y
					wind.scale[i] = { 0.0f, 0.0f, 0.0f };

					//��ԕω��ϐ�
					wind.outBreakFlag[i] = false;
				}
				oopth.resetFlag = true;
			}

			oopth.model_3->SetAlpha(oopth.alpha3);
		}
	}
}
#pragma endregion

#pragma region �E�B���h�֐�
void LeafOopth::WindMove()
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
				wind.alpha -= 0.1f;
			}
			else
			{
				wind.scale[i] = { 0.0f, 0.0f, 0.0f };
			}	
		}
	}
}

void LeafOopth::WindEffect()
{
	for (int i = 0; i < 9; i++)
	{
		if (wind.outBreakFlag[i] == true)
		{
			const XMFLOAT3 rnd_pos = {3.0f, 5.0f, 1.0f};
			particle.pos.x = wind.pos[i].x + (float)rand() / RAND_MAX * rnd_pos.x - rnd_pos.x / 2.0f;
			particle.pos.y = wind.pos[i].y + 3.0f + (float)rand() / RAND_MAX * rnd_pos.y - rnd_pos.y / 2.0f;
			particle.pos.z = wind.pos[i].z + (float)rand() / RAND_MAX * rnd_pos.z - rnd_pos.z / 2.0f;

			const float rnd_vel = 0.01f;
			particle.vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			particle.vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			particle.vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			const float rnd_acc = 0.001f;
			particle.acc.y = (float)rand() / RAND_MAX * rnd_acc;

			particleMan->Add(20, particle.pos, particle.vel, particle.acc, 0.5f, 0.1f, {0.5f, 2.0f, 0.0f, 1.0f});
		}
		
	}
}

void LeafOopth::CrossWind(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		audio->PlayWave("windSE.wav", true);
		//��]���o
		oopth.rot.y += 200.0f;
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
			if (oopth.attackOrder <= 3)
			{
				oopth.attackOrder += 1;
				wind.outBreakTimer = 150;
				wind.delayFlag = true;
				wind.delayTimer = 50;
				if (oopth.attackOrder == 1)
				{
					wind.delayFlag = false;
				}
			}
			else
			{
				//�U���\�t���b�OOFF
				oopth.attackFlag = false;
				//�U���I���t���b�OON
				oopth.attackEndFlag = true;
			}
		}
		
		//�I�[�_�[�P
		if (oopth.attackOrder == 1)
		{
			wind.outBreakFlag[0] = true;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[2] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[8] = true;
		}

		//�I�[�_�[�Q
		if (oopth.attackOrder == 2)
		{
			if (wind.delayFlag == true)
			{
				wind.outBreakFlag[0] = false;
				wind.outBreakFlag[1] = false;
				wind.outBreakFlag[2] = false;
				wind.outBreakFlag[5] = false;
				wind.outBreakFlag[8] = false;
			}
			if(wind.delayTimer <= 0)
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
		if (oopth.attackOrder == 3)
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
		if (oopth.attackOrder == 4)
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

		WindMove();

		//�o���b�g�G�t�F�N�g�N��
		WindEffect();
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

void LeafOopth::SlantingWind(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		audio->PlayWave("windSE.wav", true);
		//��]���o
		oopth.rot.y += 200.0f;
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
			if (oopth.attackOrder <= 3)
			{
				oopth.attackOrder += 1;
				wind.outBreakTimer = 150;
				wind.delayFlag = true;
				wind.delayTimer = 50;
				if (oopth.attackOrder == 1)
				{
					wind.delayFlag = false;
				}
			}
			else
			{
				//�U���\�t���b�OOFF
				oopth.attackFlag = false;
				//�U���I���t���b�OON
				oopth.attackEndFlag = true;
			}
		}

		//�I�[�_�[�P
		if (oopth.attackOrder == 1)
		{
			wind.outBreakFlag[2] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[6] = true;
		}

		//�I�[�_�[�Q
		if (oopth.attackOrder == 2)
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
		if (oopth.attackOrder == 3)
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
		if (oopth.attackOrder == 4)
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

		WindMove();

		//�o���b�g�G�t�F�N�g�N��
		WindEffect();
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

void LeafOopth::BlockWind(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		audio->PlayWave("windSE.wav", true);
		//��]���o
		oopth.rot.y += 200.0f;
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
			if (oopth.attackOrder <= 3)
			{
				oopth.attackOrder += 1;
				wind.outBreakTimer = 150;
				wind.delayFlag = true;
				wind.delayTimer = 50;
				if (oopth.attackOrder == 1)
				{
					wind.delayFlag = false;
				}
			}
			else
			{
				//�U���\�t���b�OOFF
				oopth.attackFlag = false;
				//�U���I���t���b�OON
				oopth.attackEndFlag = true;
			}
		}

		//�I�[�_�[�P
		if (oopth.attackOrder == 1)
		{
			wind.outBreakFlag[0] = true;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[2] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[5] = true;
		}

		//�I�[�_�[�Q
		if (oopth.attackOrder == 2)
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
		if (oopth.attackOrder == 3)
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
		if (oopth.attackOrder == 4)
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

		WindMove();

		//�o���b�g�G�t�F�N�g�N��
		WindEffect();
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


void LeafOopth::Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag)
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
				CrossWind(field_completionFlag);
			}
			if (turnNomber == 2)
			{
				//�U���p�^�[��2
				SlantingWind(field_completionFlag);
			}
			if (turnNomber == 3)
			{
				//�U���p�^�[��3
				BlockWind(field_completionFlag);
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
	wind.model->SetAlpha(wind.alpha);
	for (int i = 0; i < 9; i++)
	{
		circle.obj[i]->SetScale(circle.scale);
		circle.obj[i]->SetRotation(circle.rot);
		circle.obj[i]->Update();


		wind.obj[i]->SetScale(wind.scale[i]);
		wind.obj[i]->SetRotation(wind.rot);
		wind.obj[i]->Update();
	}
}

void LeafOopth::Draw()
{
	oopth.obj->Draw();
	for (int i = 0; i < 9; i++)
	{
		circle.obj[i]->Draw();
		
		wind.obj[i]->Draw();
	}
}
