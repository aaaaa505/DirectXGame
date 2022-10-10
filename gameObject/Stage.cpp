#include "Stage.h"

#include "SafeDelete.h"

Stage::Stage()
{
}

Stage::~Stage()
{
	safe_delete(obj);
	safe_delete(model);
}

void Stage::Initialize(const USHORT& stageNo)
{
	//���f���ǂݍ���
	switch (stageNo)
	{
	case WATER_ELEMENT:
		model = Model::CreateFromOBJ("stage_W");
		break;
	case FIRE_ELEMENT:
		model = Model::CreateFromOBJ("stage_F");
		break;
	case LEAF_ELEMENT:
		model = Model::CreateFromOBJ("stage_L");
		break;
	}

	// �I�u�W�F�N�g�̐�������
	obj = Object3d::Create(model);

	//���W�n�ϐ��̏�����
	obj->SetPosition({ 0.0f, 8.5f,0.0f });
	obj->SetRotation({ 0.0f, 180.0f, 0.0f });
	obj->SetScale({ 2.0f, 2.0f, 2.0f });
}

void Stage::Update()
{
	obj->Update();
}

void Stage::Draw()
{
	obj->Draw();
}
