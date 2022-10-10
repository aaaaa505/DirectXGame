#include "FireOopth.h"
#include "SafeDelete.h"

FireOopth* FireOopth::Create()
{
	// �C���X�^���X�̐���
	FireOopth* instance = new FireOopth();

	// �C���X�^���X�̏�����
	instance->Initialize();

	// �C���X�^���X��Ԃ�
	return instance;
}

FireOopth::~FireOopth()
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
	for (int  i = 0; i < 3; i++)
	{
		safe_delete(circle.obj[i]);
	}
	safe_delete(circle.model);
	

}

void FireOopth::Initialize()
{
#pragma region �����g�p���f��
	model_Collision = Model::CreateFromOBJ("sphere");
#pragma endregion

#pragma region �I�[�v�X������
	//�I�[�v�X�P�E�Q�E�R���f���ǂݍ���
	oopth.model_1 = Model::CreateFromOBJ("oopth1_F");
	oopth.model_2 = Model::CreateFromOBJ("oopth2_F");
	oopth.model_3 = Model::CreateFromOBJ("oopth3_F");

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
	oopth.damegeShakeFlag = false;
	oopth.damegeShakeTimer = 0.0f;
	oopth.attackShakeTimer = 0.2f;
	for (int i = 0; i < 3; i++)
	{
		oopth.attackOrder[i] = false;
	}
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
	circle.model = Model::CreateFromOBJ("circle_F");

	for (int i = 0; i < 3; i++)
	{
		//����
		circle.obj[i] = Object3d::Create();
		circle.obj[i]->SetModel(circle.model);

		//�������W
		circle.pos[0] = { -2.7f, 1.5f, 5.0f };
		circle.pos[1] = {  0.0f, 1.5f, 5.0f };
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

#pragma region �o���b�g�E�p�[�e�B�N��
	for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
	{
		//�������W
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

		bullet.hitFlag[i] = false;
	}

	//��ԕω��ϐ�������
	bullet.allFireFlag = false;
	bullet.delayFlag = false;
	bullet.delayTimer = 0;

	//�p�[�e�B�N��
	particleMan = ParticleManager::GetInstance();
#pragma endregion

#pragma region �I�[�f�B�I
	audio = Audio::GetInstance();
	audio->LoadWave("shotSE.wav");
	audio->SetVolume("shotSE.wav", 0.3f);
#pragma endregion


#pragma region �O�����ϐ�
	shakeFlag = false;
#pragma endregion

}

#pragma region �I�[�v�X�֐�
void FireOopth::Reset()
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
		for (int i = 0; i < 3; i++)
		{
			oopth.attackOrder[i] = false;
		}
		oopth.attackEndFlag = false;
		//Hp�ő�l
		oopth.hp = 300;
		
#pragma endregion

#pragma region �T�[�N���ċN��
		for (int i = 0; i < 3; i++)
		{
			//��]�p
			circle.rot = { 0.0f, 0.0f, 0.0f };
			//�X�P�[��
			circle.scale = { 0.0f, 0.0f, 1.0f };
		}
#pragma endregion

#pragma region �o���b�g�ċN��
		for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
		{
			//���W
			bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

			//�U������t���b�O
			bullet.hitFlag[i] = false;
		}
		//��ԕω��ϐ�������
		bullet.allFireFlag = false;
		bullet.delayFlag = false;
		bullet.delayTimer = 0;
#pragma endregion

		oopth.resetFlag = false;
	}

}

void FireOopth::ModelChange(int turnNomber)
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

void FireOopth::StatusChange()
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

void FireOopth::AttackPreparation()
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
			circle.rot.z -= 100.0f;
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
			//�e�𑕓U
			Loading();
			//�U��������Ԃ���O���
			oopth.attackPreparationFlag = false;
			//�T�[�N���̉�]�p�����ɖ߂�
			circle.rot = { 0.0f, 0.0f, 0.0f };
		}

	}

}

void FireOopth::AttackEnd()
{
	if (oopth.attackEndFlag == true)
	{
		for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
		{
			bullet.hitFlag[i] = false;
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
			circle.rot.z += 100.0f;
		}

		//��L�������I��������
		else
		{
			//StatusChange���ċN��
			oopth.statusChangeFlag = true;
			oopth.changeTimer = 150;
			//AllFireFlag��OFF
			bullet.allFireFlag = false;
			//AttackEnd��OFF
			oopth.attackEndFlag = false;
		}
	}
}

void FireOopth::Appearance()
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

void FireOopth::Damage(short player_TypeFlag)
{
	if (player_TypeFlag == 0 || player_TypeFlag == 1)
	{
		oopth.hp -= 3;
	}
	if (player_TypeFlag == 2)
	{
		oopth.hp -= 7;
	}
	if (player_TypeFlag == 3)
	{
		oopth.hp -= 2;
	}
}

void FireOopth::Death(int turnNomber)
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
				for (int i = 0; i < 3; i++)
				{
					//��]�p
					circle.rot = { 0.0f, 0.0f, 0.0f };
					//�X�P�[��
					circle.scale = { 0.0f, 0.0f, 1.0f };
				}
				for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
				{
					//���W
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

					//�U������t���b�O
					bullet.hitFlag[i] = false;
				}
				oopth.resetFlag = true;
			}

			oopth.model_3->SetAlpha(oopth.alpha3);
		}
	}

}

void FireOopth::AttackShake()
{
	oopth.shake.x = (float)rand() / RAND_MAX * oopth.attackShakeTimer - oopth.attackShakeTimer / 2.0f;
	oopth.shake.y = (float)rand() / RAND_MAX * oopth.attackShakeTimer - oopth.attackShakeTimer / 2.0f;
}

void FireOopth::DamegeShake()
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

#pragma endregion

#pragma region �o���b�g�֐�
void FireOopth::Loading()
{
	//�N�����Ɋe�T�[�N���̗��ɒe���O���z�u
	for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
	{
		if (i < 3)
		{
			bullet.pos[i] = { circle.pos[0].x, circle.pos[0].y, circle.pos[0].z + 0.6f };
		}
		else if (i < 6)
		{
			bullet.pos[i] = { circle.pos[1].x, circle.pos[1].y, circle.pos[1].z + 0.6f };
		}
		else
		{
			bullet.pos[i] = { circle.pos[2].x, circle.pos[2].y, circle.pos[2].z + 0.6f };
		}
	}
	
}

void FireOopth::FireEffect()
{
	for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
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

void FireOopth::OneLineOrderShot(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		//��]���o
		oopth.rot.y += 200.0f;
		circle.rot.z += 200.0f;
		
		//�S�e���˂��Ă��Ȃ��Ȃ�
		if (bullet.allFireFlag == false)
		{
			//�f�B���C�^�C�}�[�N��
			bullet.delayTimer -= 1;
		}
		for (int i = 0; i < 3 && bullet.delayTimer <= 0; i++)
		{
			if (oopth.attackOrder[i] == false)
			{
				audio->StopWave("shotSE.wav");
				audio->PlayWave("shotSE.wav", false);
				oopth.attackOrder[i] = true;
				bullet.delayTimer = 50;
				break;
			}
		}

		//�I�[�_�[�P
		if (oopth.attackOrder[0] == true)
		{
			bullet.pos[3].z -= 0.2f;
			if (bullet.hitFlag[3] == true)
			{
				bullet.pos[3].y = 100.0f;
			}
		}

		//�I�[�_�[�Q
		if (oopth.attackOrder[1] == true)
		{
			bullet.pos[0].z -= 0.2f;
			if (bullet.hitFlag[0] == true)
			{
				bullet.pos[0].y = 100.0f;
			}
		}

		//�I�[�_�[�R
		if (oopth.attackOrder[2] == true)
		{
			bullet.allFireFlag = true;
			bullet.pos[6].z -= 0.2f;
			if (bullet.hitFlag[6] == true)
			{
				bullet.pos[6].y = 100.0f;
			}
			if (bullet.pos[6].z <= -8.0f)
			{
				oopth.fixFlag = true;
			}
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
		//�t�@�C���[�G�t�F�N�g�N��
		FireEffect();
		//�V�F�C�N���o
		AttackShake();
	}
	else if(oopth.attackFlag == false)
	{
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; i++)
		{
			oopth.attackOrder[i] = false;
		}
	}
}

void FireOopth::TwinLineOrderShot(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		//��]���o
		oopth.rot.y += 200.0f;
		circle.rot.z += 200.0f;

		//�S�e���˂��Ă��Ȃ��Ȃ�
		if (bullet.allFireFlag == false)
		{
			//�f�B���C�^�C�}�[�N��
			bullet.delayTimer -= 1;
		}
		for (int i = 0; i < 3 && bullet.delayTimer <= 0; i++)
		{
			if (oopth.attackOrder[i] == false)
			{
				audio->StopWave("shotSE.wav");
				audio->PlayWave("shotSE.wav", false);
				oopth.attackOrder[i] = true;
				bullet.delayTimer = 50;
				break;
			}
		}

		//�I�[�_�[�P
		if (oopth.attackOrder[0] == true)
		{
			bullet.pos[0].z -= 0.2f;
			bullet.pos[3].z -= 0.2f;
			if (bullet.hitFlag[0] == true)
			{
				bullet.pos[0].y = 100.0f;
			}

			if (bullet.hitFlag[3] == true)
			{
				bullet.pos[3].y = 100.0f;
			}

		}

		//�I�[�_�[�Q
		if (oopth.attackOrder[1] == true)
		{
			bullet.pos[4].z -= 0.2f;
			bullet.pos[6].z -= 0.2f;
			if (bullet.hitFlag[4] == true)
			{
				bullet.pos[4].y = 100.0f;
			}

			if (bullet.hitFlag[6] == true)
			{
				bullet.pos[6].y = 100.0f;
			}
		}

		//�I�[�_�[�R
		if (oopth.attackOrder[2] == true)
		{
			bullet.allFireFlag = true;
			bullet.pos[1].z -= 0.2f;
			bullet.pos[5].z -= 0.2f;
			if (bullet.hitFlag[1] == true)
			{
				bullet.pos[1].y = 100.0f;
			}

			if (bullet.hitFlag[5] == true)
			{
				bullet.pos[5].y = 100.0f;
			}
			if (bullet.pos[1].z <= -8.0f)
			{
				oopth.fixFlag = true;
			}
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
		//�t�@�C���[�G�t�F�N�g�N��
		FireEffect();
		//�V�F�C�N���o
		AttackShake();
	}
	else if (oopth.attackFlag == false)
	{
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; i++)
		{
			oopth.attackOrder[i] = false;
		}
	}
}

void FireOopth::AllLineOrderShot(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		//��]���o
		oopth.rot.y += 200.0f;
		circle.rot.z += 200.0f;

		//�S�e���˂��Ă��Ȃ��Ȃ�
		if (bullet.allFireFlag == false)
		{
			//�f�B���C�^�C�}�[�N��
			bullet.delayTimer -= 1;
		}
		for (int i = 0; i < 3 && bullet.delayTimer <= 0; i++)
		{

			if (oopth.attackOrder[i] == false)
			{
				audio->StopWave("shotSE.wav");
				audio->PlayWave("shotSE.wav", false);
				oopth.attackOrder[i] = true;
				bullet.delayTimer = 100;
				break;
			}
		}

		for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
		{
			if (bullet.hitFlag[i] == true)
			{
				bullet.pos[i].y = 100.0f;
			}
		}

		//�I�[�_�[�P
		if (oopth.attackOrder[0] == true)
		{
			bullet.pos[0].z -= 0.2f;
			bullet.pos[3].z -= 0.2f;
			bullet.pos[6].z -= 0.13f;
		}

		//�I�[�_�[�Q
		if (oopth.attackOrder[1] == true)
		{
			bullet.pos[1].z -= 0.1f;
			bullet.pos[7].z -= 0.1f;
		}

		//�I�[�_�[�R
		if (oopth.attackOrder[2] == true)
		{
			bullet.allFireFlag = true;
			bullet.pos[2].z -= 0.13f;
			bullet.pos[4].z -= 0.2f;
			bullet.pos[8].z -= 0.2f;
			if (bullet.pos[2].z <= -8.0f)
			{
				oopth.fixFlag = true;
			}
		}

		if (oopth.fixFlag == true)
		{
			oopth.fixTimer -= 1;
			if (oopth.fixTimer <= 0)
			{
				oopth.fixTimer = 40;
				oopth.fixFlag = false;
				oopth.attackFlag = false;
				oopth.attackEndFlag = true;
			}
		}
		//�t�@�C���[�G�t�F�N�g�N��
		FireEffect();
		//�V�F�C�N���o
		AttackShake();
	}
	else if (oopth.attackFlag == false)
	{
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; i++)
		{
			oopth.attackOrder[i] = false;
		}
	}
}
#pragma endregion

void FireOopth::Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag)
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
				OneLineOrderShot(field_completionFlag);
			}
			if (turnNomber == 2)
			{
				//�U���p�^�[��2
				TwinLineOrderShot(field_completionFlag);
			}
			if (turnNomber == 3)
			{
				AllLineOrderShot(field_completionFlag);
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
	for (int i = 0; i < 3; i++)
	{
		circle.obj[i]->SetScale(circle.scale);
		circle.obj[i]->SetRotation(circle.rot);
		circle.obj[i]->Update();
	}

}

void FireOopth::Draw()
{
	oopth.obj->Draw();
	for (int i = 0; i < 3; i++)
	{
		circle.obj[i]->Draw();
	}
}
