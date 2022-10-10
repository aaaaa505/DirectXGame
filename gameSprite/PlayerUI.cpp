#include "PlayerUI.h"
#include "LoadSprite.h"

#include "GameScene.h"
#include "SafeDelete.h"

PlayerUI* PlayerUI::Create()
{
	//�C���X�^���X�̐���
	PlayerUI* instance = new PlayerUI();

	//�C���X�^���X�̏�����
	instance->Initialize();

	//�C���X�^���X��Ԃ�
	return instance;
}

PlayerUI::PlayerUI()
{
}

PlayerUI::~PlayerUI()
{
	for (auto sprite : sprites) {
		safe_delete(sprite.second);
	}
}

void PlayerUI::Initialize()
{
	// �������W
	pos = { 0.0f, 0.0f };

	// ���n
	sprites["back"] = Sprite::Create(parts_Back, pos);

	// HP�Q�[�W
	sprites["hpGage"] = Sprite::Create(parts_Hp, { pos.x + 138.0f,pos.y + 630.0f });

	// �^�C�}�[
	sprites["timer"] = Sprite::Create(parts_ChangeTimer, { pos.x + 116.0f,pos.y + 670.0f });

	// �J�o�[
	sprites["front"] = Sprite::Create(parts_Front, pos);

	// �}�[�NF
	sprites["mark_F"] = Sprite::Create(parts_Mark_F, pos);

	// �}�[�NW
	sprites["mark_W"] = Sprite::Create(parts_Mark_W, pos);

	// �}�[�NL
	sprites["mark_L"] = Sprite::Create(parts_Mark_L, pos);

	//��ԕω��ϐ�
	markFlag = NORMAL_TYPE;
	stageEndFlag = false;
	completionFlag = false;
}

void PlayerUI::EndMove()
{
	if (stageEndFlag == true)
	{
		if (pos.y < 210)
		{
			pos.y += 1.0f;
		}
		else
		{
			completionFlag = true;
		}
		sprites["back"]->SetPosition(pos);
		sprites["hpGage"]->SetPosition({pos.x + 138.0f,pos.y + 630.0f});
		sprites["timer"]->SetPosition({ pos.x + 116.0f,pos.y + 670.0f });
		sprites["front"]->SetPosition(pos);
		sprites["mark_F"]->SetPosition(pos);
		sprites["mark_W"]->SetPosition(pos);
		sprites["mark_L"]->SetPosition(pos);
	}
}

void PlayerUI::Update(int playerHp, int changeTimer, short typeFlag)
{
	//�v���C���[�̑����ɂ���ă}�[�N��ύX
	markFlag = typeFlag;
	//�v���C���[��Hp���Q�[�W��
	sprites["hpGage"]->SetSize({(float)playerHp * 0.94f, 30.0f});
	//�v���C���[�̕ϐg���Ԃ��Q�[�W��
	sprites["timer"]->SetSize({ (float)changeTimer * 0.83f, 20.0f });

	EndMove();
}

void PlayerUI::Draw()
{
	sprites["back"]->Draw();
	sprites["hpGage"]->Draw();
	sprites["timer"]->Draw();
	sprites["front"]->Draw();

	switch (markFlag)
	{
	case WATER_TYPE:
		sprites["mark_W"]->Draw();
		break;
	case FIRE_TYPE:
		sprites["mark_F"]->Draw();
		break;
	case LEAF_TYPE:
		sprites["mark_L"]->Draw();
		break;
	}
}
