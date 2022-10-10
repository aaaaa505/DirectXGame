#include "Field.h"
#include "SafeDelete.h"

Field* Field::Create()
{
	Field* instance = new Field;

	instance->Initialize();

	return instance;
}

Field::~Field()
{
	safe_delete(model);
	for (short x = 0; x < 3; x++)
	{
		for (short y = 0; y < 3; y++)
		{
			safe_delete(object3d[x][y]);
		}
	}
} 

void Field::Initialize()
{
	model = Model::CreateFromOBJ("field");
	modelRed = Model::CreateFromOBJ("field2");
	
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			object3d[x][y] = Object3d::Create();
			object3d[x][y]->SetModel(model);
			object3d[x][y]->SetScale({0.2f, 0.2f, 0.2f});
		}
	}

	object3d[0][0]->SetPosition({ -2.6f,0.0f,+2.5f });
	object3d[1][0]->SetPosition({  0.0f,0.0f,+2.5f });
	object3d[2][0]->SetPosition({ +2.6f,0.0f,+2.5f });
	object3d[0][1]->SetPosition({ -2.6f,0.0f, 0.0f });
	object3d[1][1]->SetPosition({  0.0f,0.0f, 0.0f });
	object3d[2][1]->SetPosition({ +2.6f,0.0f, 0.0f });
	object3d[0][2]->SetPosition({ -2.6f,0.0f,-2.5f });
	object3d[1][2]->SetPosition({  0.0f,0.0f,-2.5f });
	object3d[2][2]->SetPosition({ +2.6f,0.0f,-2.5f });
}

void Field::Update(bool fieldFlag[3][3])
{
	for (short x = 0; x < 3; x++)
	{
		for (short y = 0; y < 3; y++)
		{
			if (!fieldFlag[x][y])
			{
				object3d[x][y]->SetModel(model);
			}
			else if (fieldFlag[x][y])
			{
				object3d[x][y]->SetModel(modelRed);
			}

			object3d[x][y]->Update();
		}
	}
}

void Field::Draw()
{
	for (short x = 0; x < 3; x++)
	{
		for (short y = 0; y < 3; y++) {

			object3d[x][y]->Draw();
		}
	}
}
