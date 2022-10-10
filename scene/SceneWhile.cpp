#include "SceneWhile.h"
#include "LoadSprite.h"
#include "SafeDelete.h"

#include "GameScene.h"

SceneWhile* SceneWhile::Create()
{
	//�C���X�^���X�̐���
	SceneWhile* instance = new SceneWhile();

	//�C���X�^���X�̏�����
	instance->Initialize();
	
	//�C���X�^���X��Ԃ�
	return instance;
}

SceneWhile::~SceneWhile()
{
	safe_delete(stageName.sprite);
	safe_delete(illustration.sprite);
	safe_delete(roundCount.sprite);
}

void SceneWhile::Initialize()
{
#pragma region �}�G�\���̏������y�ѐ���
	//alpha�l
	illustration.alpha = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	//�X�v���C�g����
	illustration.sprite = Sprite::Create(IllustrationNumber, { 0.0f, 0.0f }, illustration.alpha);

	//��ԕω��ϐ�
	illustration.transDownFlag = false;
	illustration.completionFlag2 = false;
	illustration.resetFlag = false;

#pragma endregion

#pragma region �X�e�[�W���\���̏������y�ѐ���
	//alpha�l
	stageName.alpha = { 1.0f, 1.0f, 1.0f, 0.0f };

	//�X�v���C�g����
	stageName.sprite = Sprite::Create(stageF_Name, { 0.0f, 0.0f }, stageName.alpha);

	//��ԕω��ϐ�
	//��ԕω��ϐ�
	stageName.transUpFlag = false;
	stageName.transDownFlag = false;
	stageName.completionFlag1 = false;
	stageName.completionFlag2 = false;
	stageName.resetFlag = false;

#pragma endregion

#pragma region ���E���h�J�E���g�\���̂̏������y�ѐ���
	//���W
	roundCount.pos = { 1280.0f, 0.0f };

	//�X�v���C�g����
	roundCount.sprite = Sprite::Create(round1Number, roundCount.pos);

	//��ԕω��ϐ�
	roundCount.moveFlag = false;
	roundCount.completionFlag = false;
	roundCount.resetFlag = false;

	roundCount.stopFlag = false;
	roundCount.stopTimer = 70;
#pragma endregion

#pragma region �G���h�V�[���\���̏������y�ѐ���
	//alpha�l
	endScene.alpha = { 1.0f, 1.0f, 1.0f, 0.0f };

	//�X�v���C�g����
	endScene.sprite = Sprite::Create(clearNumber, { 0.0f, 0.0f }, endScene.alpha);

	//��ԕω��ϐ�
	endScene.transUpFlag = false;
	endScene.transDownFlag = false;
	endScene.completionFlag1 = false;
	endScene.completionFlag2 = false;
	endScene.clearFlag = false;
	endScene.overFlag = false;
	endScene.resetFlag = false;
#pragma endregion
}

#pragma region �}�G�֐�

void SceneWhile::Illustration::TransUp()
{
	//�N���t���b�O��ON�Ȃ�
	if (transUpFlag == true)
	{
		//�K��l�܂�alpha�l��������
		if (alpha.w <= 1.0f)
		{
			alpha.w += 0.01f;
		}
		//�K��l�ɂȂ�����
		else
		{
			//�덷�C��
			alpha.w = 1.0f;
			//�N���t���b�O��OFF&�N�������t���b�O��ON
			transUpFlag = false;
			completionFlag1 = true;
		}
		sprite->SetAlpha(alpha.w);
	}
}

void SceneWhile::Illustration::TransDown()
{
	//�N���t���b�O��ON�Ȃ�
	if (transDownFlag == true)
	{
		//�K��l�܂�alpha�l��������
		if (alpha.w >= 0.0f)
		{
			alpha.w -= 0.01f;
		}
		//�K��l�ɂȂ�����
		else
		{
			//�덷�C��
			alpha.w = 0.0f;
			//�N���t���b�O��OFF&�N�������t���b�O��ON
			transDownFlag = false;
			completionFlag2 = true;
		}
		sprite->SetAlpha(alpha.w);
	}
}

void SceneWhile::Illustration::Reset()
{
	if (resetFlag == true)
	{
		alpha.w = 0.0f;
		transDownFlag = false;
		completionFlag2 = false;

		resetFlag = false;
	}
}
#pragma endregion

#pragma region �X�e�[�W���֐�
void SceneWhile::StageName::TransUp()
{
	//�N���t���b�O��ON�Ȃ�
	if (transUpFlag == true)
	{
		//�K��l�܂�alpha�l���グ��
		if (alpha.w <= 1.0f)
		{
			alpha.w += 0.01f;
		}
		//�K��l�ɂȂ�����
		else
		{
			//�덷�C��
			alpha.w = 1.0f;
			//�N���t���b�O��OFF&�N�������t���b�O��ON
			transUpFlag = false;
			completionFlag1 = true;
		}
		sprite->SetAlpha(alpha.w);
	}
}

void SceneWhile::StageName::TransDown()
{
	//�N���t���b�O��ON�Ȃ�
	if (transDownFlag == true)
	{
		//�K��l�܂�alpha�l��������
		if (alpha.w >= 0.0f)
		{
			alpha.w -= 0.01f;
		}
		//�K��l�ɂȂ�����
		else
		{
			//�덷�C��
			alpha.w = 0.0f;
			//�N���t���b�O��OFF&�N�������t���b�O��ON
			transDownFlag = false;
			completionFlag2 = true;
		}
		sprite->SetAlpha(alpha.w);
	}
}

void SceneWhile::StageName::Reset()
{
	if (resetFlag == true)
	{
		alpha.w = 0.0f;
		transUpFlag = false;
		transDownFlag = false;
		completionFlag1 = false;
		completionFlag2 = false;

		resetFlag = false;
	}
}

void SceneWhile::StageName::NameChange(int sceneNo)
{
	if (sceneNo == WATER_ELEMENT)
	{
		sprite->SetTextureNumber(stageW_Name);
	}
	if (sceneNo == FIRE_ELEMENT)
	{
		sprite->SetTextureNumber(stageF_Name);
	}
	if (sceneNo == LEAF_ELEMENT)
	{
		sprite->SetTextureNumber(stageL_Name);
	}
}

#pragma endregion

#pragma region ���E���h�J�E���g�֐�
void SceneWhile::RoundCount::NomberChange(int roundNomber)
{
	//���݂̃��E���h���ɂ���ăX�v���C�g��ύX
	if (roundNomber == 1)
	{
		sprite->SetTextureNumber(round1Number);
	}
	if (roundNomber == 2)
	{
		sprite->SetTextureNumber(round2Number);
	}
	if (roundNomber == 3)
	{
		sprite->SetTextureNumber(round3Number);
	}
	if (roundNomber == 4)
	{
		sprite->SetTextureNumber(round4Number);
	}
}

void SceneWhile::RoundCount::Move(Illustration illustration)
{
	//�N���t���b�O��ON�Ȃ�
	if (moveFlag == true)
	{
		illustration.alpha.w = 0.5f;
		//�X�g�b�v�t���b�OOFF
		if (stopFlag == false)
		{
			pos.x -= 64.0f;
			//���Ԓn�_�ɓ��B������
			if (pos.x == 0.0f)
			{
				stopFlag = true;
			}

			else if (pos.x <= -1280.0f)
			{
				illustration.alpha.w = 0.0f;
				completionFlag = true;
				moveFlag = false;
			}
		}
		//ON�Ȃ�
		else
		{
			//��~���Ԃ��J�E���g
			stopTimer -= 1;
			//0�ɂȂ�����X�g�b�v�t���b�O��OFF
			if (stopTimer <= 0)
			{
				stopFlag = false;
			}
		}

		illustration.sprite->SetAlpha(illustration.alpha.w);
		sprite->SetPosition(pos);
	}
}

void SceneWhile::RoundCount::Reset()
{
	if (resetFlag == true)
	{
		pos = { 1280.0f, 0.0f };
		moveFlag = false;
		completionFlag = false;
		stopFlag = false;
		stopTimer = 70;
	}
	
	resetFlag = false;
}
#pragma endregion

#pragma region �G���h�V�[���֐�
void SceneWhile::EndScene::SpriteChange()
{
	if (clearFlag == true)
	{
		sprite->SetTextureNumber(clearNumber);
	}
	if (overFlag == true)
	{
		sprite->SetTextureNumber(overNumber);
	}
}
void SceneWhile::EndScene::TransUp()
{
	if (transUpFlag == true && completionFlag1 == false)
	{
		if (alpha.w <= 1.0f)
		{
			alpha.w += 0.01f;
		}
		else
		{
			alpha.w = 1.0f;
			transUpFlag = false;
			completionFlag1 = true;
		}

		sprite->SetAlpha(alpha.w);
	}
}

void SceneWhile::EndScene::TransDown()
{
	if (transDownFlag == true && completionFlag2 == false)
	{
		if (alpha.w >= 0.0f)
		{
			alpha.w -= 0.01f;
		}
		else
		{
			alpha.w = 0.0f;
			transDownFlag = false;
			completionFlag2 = true;
		}

		sprite->SetAlpha(alpha.w);
	}
}
#pragma endregion


void SceneWhile::Update(int roundNo, int sceneNo)
{
	illustration.TransUp();
	illustration.TransDown();
	illustration.Reset();

	stageName.TransUp();
	stageName.TransDown();
	stageName.Reset();
	stageName.NameChange(sceneNo);

	roundCount.NomberChange(roundNo);
	roundCount.Move(illustration);
	roundCount.Reset();

	endScene.SpriteChange();
	endScene.TransUp();
	endScene.TransDown();
}

void SceneWhile::Draw()
{
	if (endScene.clearFlag == true || endScene.overFlag == true)
	{
		endScene.sprite->Draw();
	}
	illustration.sprite->Draw();
	stageName.sprite->Draw();
	roundCount.sprite->Draw();
}