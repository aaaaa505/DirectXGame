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
	//モデル読み込み
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

	// オブジェクトの生成処理
	obj = Object3d::Create(model);

	//座標系変数の初期化
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
