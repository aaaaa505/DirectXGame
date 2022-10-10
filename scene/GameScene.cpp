#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::SceneDateBufferUpdate()
{
	sceneDateBuffer->sceneNo = sceneNo;
	sceneDateBuffer->stageNo = stageNo;
}
