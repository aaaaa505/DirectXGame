#include "LeafField.h"
#include "SafeDelete.h"

LeafField* LeafField::Create()
{
	LeafField* instance = new LeafField;

	instance->Initialize();

	return instance;
}

LeafField::~LeafField()
{
	safe_delete(model_Normal);
	safe_delete(model_Scope);
	for (short x = 0; x < 3; x++)
	{
		for (short y = 0; y < 3; y++)
		{
			safe_delete(obj_Field[x][y]);
		}
	}
}

void LeafField::Initialize()
{
	model_Normal = Model::CreateFromOBJ("field");
	model_Scope = Model::CreateFromOBJ("field2");

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			obj_Field[x][y] = Object3d::Create();
			obj_Field[x][y]->SetModel(model_Normal);
			obj_Field[x][y]->SetScale({ 0.2f, 0.2f, 0.2f });
			scopeFlag[x][y] = false;
		}
	}

#pragma region �����
	obj_Field[0][0]->SetPosition({ -2.7f,0.0f,+2.7f });
	obj_Field[0][1]->SetPosition({ -2.7f,0.0f, 0.0f });
	obj_Field[0][2]->SetPosition({ -2.7f,0.0f,-2.7f });
#pragma endregion

#pragma region �������
	obj_Field[1][0]->SetPosition({ 0.0f,0.0f,+2.7f });
	obj_Field[1][1]->SetPosition({ 0.0f,0.0f, 0.0f });
	obj_Field[1][2]->SetPosition({ 0.0f,0.0f,-2.7f });
#pragma endregion

#pragma region �E���
	obj_Field[2][0]->SetPosition({ +2.7f,0.0f,+2.7f });
	obj_Field[2][1]->SetPosition({ +2.7f,0.0f, 0.0f });
	obj_Field[2][2]->SetPosition({ +2.7f,0.0f,-2.7f });
#pragma endregion

#pragma region ��ԕω��ϐ�������
	showRangeFlag = false;
	completionFlag = false;
	processNomber = 1;
	showTimer = 50;
#pragma endregion

#pragma region �I�[�f�B�I
	audio = Audio::GetInstance();
#pragma endregion
	
}

void LeafField::CrossWindScope()
{
	if (showRangeFlag == true && completionFlag == false)
	{
		//�\�����Ԃ�����������
		showTimer -= 1;
		if (showTimer <= 0)
		{
			//�K��l�ɓ��B������b����߂��Ď��̍U���菇��\��
			processNomber += 1;
			showTimer = 60;
		}

		if (processNomber == 1)
		{
			audio->PlayWave("scopeSE.wav", true);
			scopeFlag[0][0] = true;
			scopeFlag[1][0] = true;
			scopeFlag[2][0] = true;
			scopeFlag[2][1] = true;
			scopeFlag[2][2] = true;
		}

		if (processNomber == 2)
		{
			scopeFlag[0][0] = false;
			scopeFlag[2][0] = false;
			scopeFlag[2][2] = false;
			scopeFlag[0][1] = true;
			scopeFlag[1][1] = true;
			scopeFlag[1][2] = true;
		}

		if (processNomber == 3)
		{
			scopeFlag[1][0] = false;
			scopeFlag[1][1] = false;
			scopeFlag[2][1] = false;
			scopeFlag[0][0] = true;
			scopeFlag[0][2] = true;
			scopeFlag[2][2] = true;
		}

		if (processNomber == 4)
		{
			scopeFlag[0][0] = false;
			scopeFlag[0][2] = false;
			scopeFlag[2][2] = false;
			scopeFlag[1][0] = true;
			scopeFlag[1][1] = true;
			scopeFlag[2][1] = true;
		}

		if (processNomber == 5)
		{
			audio->StopWave("scopeSE.wav");
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					scopeFlag[x][y] = false;
				}
			}

			processNomber = 1;
			showRangeFlag = false;
			completionFlag = true;
		}

	}
}

void LeafField::SlantingWindScope()
{
	if (showRangeFlag == true && completionFlag == false)
	{
		//�\�����Ԃ�����������
		showTimer -= 1;
		if (showTimer <= 0)
		{
			//�K��l�ɓ��B������b����߂��Ď��̍U���菇��\��
			processNomber += 1;
			showTimer = 60;
		}

		if (processNomber == 1)
		{
			audio->PlayWave("scopeSE.wav", true);
			scopeFlag[2][0] = true;
			scopeFlag[1][1] = true;
			scopeFlag[0][2] = true;
		}

		if (processNomber == 2)
		{
			scopeFlag[2][0] = false;
			scopeFlag[1][1] = false;
			scopeFlag[0][2] = false;
			scopeFlag[1][0] = true;
			scopeFlag[0][1] = true;
			scopeFlag[1][2] = true;
			scopeFlag[2][1] = true;
		}

		if (processNomber == 3)
		{
			scopeFlag[1][0] = false;
			scopeFlag[0][1] = false;
			scopeFlag[1][2] = false;
			scopeFlag[2][1] = false;
			scopeFlag[0][0] = true;
			scopeFlag[1][1] = true;
			scopeFlag[2][2] = true;
		}

		if (processNomber == 4)
		{
			scopeFlag[0][0] = false;
			scopeFlag[1][1] = false;
			scopeFlag[2][2] = false;
			scopeFlag[1][0] = true;
			scopeFlag[0][1] = true;
			scopeFlag[1][2] = true;
			scopeFlag[2][1] = true;
		}

		if (processNomber == 5)
		{
			audio->StopWave("scopeSE.wav");
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					scopeFlag[x][y] = false;
				}
			}

			processNomber = 1;
			showRangeFlag = false;
			completionFlag = true;
		}

	}
}

void LeafField::BlockWindScope()
{
	if (showRangeFlag == true && completionFlag == false)
	{
		//�\�����Ԃ�����������
		showTimer -= 1;
		if (showTimer <= 0)
		{
			//�K��l�ɓ��B������b����߂��Ď��̍U���菇��\��
			processNomber += 1;
			showTimer = 60;
		}

		if (processNomber == 1)
		{
			audio->PlayWave("scopeSE.wav", true);
			scopeFlag[0][0] = true;
			scopeFlag[1][0] = true;
			scopeFlag[2][0] = true;
			scopeFlag[0][1] = true;
			scopeFlag[1][1] = true;
			scopeFlag[2][1] = true;
		}

		if (processNomber == 2)
		{
			scopeFlag[0][0] = false;
			scopeFlag[1][0] = false;
			scopeFlag[2][0] = false;
			scopeFlag[0][2] = true;
			scopeFlag[1][2] = true;
			scopeFlag[2][2] = true;
		}

		if (processNomber == 3)
		{
			scopeFlag[0][1] = false;
			scopeFlag[0][2] = false;
			scopeFlag[1][0] = true;
			scopeFlag[2][0] = true;
		}

		if (processNomber == 4)
		{
			scopeFlag[2][0] = false;
			scopeFlag[2][1] = false;
			scopeFlag[2][2] = false;
			scopeFlag[0][0] = true;
			scopeFlag[0][1] = true;
			scopeFlag[0][2] = true;
		}

		if (processNomber == 5)
		{
			audio->StopWave("scopeSE.wav");
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					scopeFlag[x][y] = false;
				}
			}

			processNomber = 1;
			showRangeFlag = false;
			completionFlag = true;
		}

	}
}

void LeafField::TempestWind1Scope()
{
	if (showRangeFlag == true && completionFlag == false)
	{
		//�\�����Ԃ�����������
		showTimer -= 1;
		if (showTimer <= 0)
		{
			//�K��l�ɓ��B������b����߂��Ď��̍U���菇��\��
			processNomber += 1;
			showTimer = 60;
		}

#pragma region CrossWindScope
		if (processNomber == 1)
		{
			audio->PlayWave("scopeSE.wav", true);
			scopeFlag[0][0] = true;
			scopeFlag[1][0] = true;
			scopeFlag[2][0] = true;
			scopeFlag[2][1] = true;
			scopeFlag[2][2] = true;
		}

		if (processNomber == 2)
		{
			scopeFlag[0][0] = false;
			scopeFlag[2][0] = false;
			scopeFlag[2][2] = false;
			scopeFlag[0][1] = true;
			scopeFlag[1][1] = true;
			scopeFlag[1][2] = true;
		}

		if (processNomber == 3)
		{
			scopeFlag[1][0] = false;
			scopeFlag[1][1] = false;
			scopeFlag[2][1] = false;
			scopeFlag[0][0] = true;
			scopeFlag[0][2] = true;
			scopeFlag[2][2] = true;
		}

		if (processNomber == 4)
		{
			scopeFlag[0][0] = false;
			scopeFlag[0][2] = false;
			scopeFlag[2][2] = false;
			scopeFlag[1][0] = true;
			scopeFlag[1][1] = true;
			scopeFlag[2][1] = true;
		}
#pragma endregion

#pragma region SlantingWindScope
		if (processNomber == 5)
		{
			scopeFlag[1][0] = false;
			scopeFlag[0][1] = false;
			scopeFlag[1][2] = false;
			scopeFlag[2][1] = false;
			scopeFlag[2][0] = true;
			scopeFlag[0][2] = true;
		}

		if (processNomber == 6)
		{
			scopeFlag[2][0] = false;
			scopeFlag[1][1] = false;
			scopeFlag[0][2] = false;
			scopeFlag[1][0] = true;
			scopeFlag[0][1] = true;
			scopeFlag[1][2] = true;
			scopeFlag[2][1] = true;
		}

		if (processNomber == 7)
		{
			scopeFlag[1][0] = false;
			scopeFlag[0][1] = false;
			scopeFlag[1][2] = false;
			scopeFlag[2][1] = false;
			scopeFlag[0][0] = true;
			scopeFlag[1][1] = true;
			scopeFlag[2][2] = true;
		}

		if (processNomber == 8)
		{
			scopeFlag[0][0] = false;
			scopeFlag[1][1] = false;
			scopeFlag[2][2] = false;
			scopeFlag[1][0] = true;
			scopeFlag[0][1] = true;
			scopeFlag[1][2] = true;
			scopeFlag[2][1] = true;
		}
#pragma endregion

#pragma region  BlockWindScope
		if (processNomber == 9)
		{
			scopeFlag[1][2] = false;
			scopeFlag[0][0] = true;
			scopeFlag[2][0] = true;
			scopeFlag[1][1] = true;
		}

		if (processNomber == 10)
		{
			scopeFlag[0][0] = false;
			scopeFlag[1][0] = false;
			scopeFlag[2][0] = false;
			scopeFlag[0][2] = true;
			scopeFlag[1][2] = true;
			scopeFlag[2][2] = true;
		}

		if (processNomber == 11)
		{
			scopeFlag[0][1] = false;
			scopeFlag[0][2] = false;
			scopeFlag[1][0] = true;
			scopeFlag[2][0] = true;
		}

		if (processNomber == 12)
		{
			scopeFlag[2][0] = false;
			scopeFlag[2][1] = false;
			scopeFlag[2][2] = false;
			scopeFlag[0][0] = true;
			scopeFlag[0][1] = true;
			scopeFlag[0][2] = true;
		}
#pragma endregion

		if (processNomber == 13)
		{
			audio->StopWave("scopeSE.wav");
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					scopeFlag[x][y] = false;
				}
			}

			processNomber = 1;
			showRangeFlag = false;
			completionFlag = true;
		}

	}
}

void LeafField::TempestWind2Scope()
{
	if (showRangeFlag == true && completionFlag == false)
	{
		//�\�����Ԃ�����������
		showTimer -= 1;
		if (showTimer <= 0)
		{
			//�K��l�ɓ��B������b����߂��Ď��̍U���菇��\��
			processNomber += 1;
			showTimer = 60;
		}

#pragma region CrossWindScope
		if (processNomber == 1)
		{
			audio->PlayWave("scopeSE.wav", true);
			scopeFlag[0][0] = true;
			scopeFlag[1][0] = true;
			scopeFlag[2][0] = true;
			scopeFlag[2][1] = true;
			scopeFlag[2][2] = true;
		}

		if (processNomber == 2)
		{
			scopeFlag[0][0] = false;
			scopeFlag[2][0] = false;
			scopeFlag[2][2] = false;
			scopeFlag[0][1] = true;
			scopeFlag[1][1] = true;
			scopeFlag[1][2] = true;
		}

		if (processNomber == 3)
		{
			scopeFlag[1][0] = false;
			scopeFlag[1][1] = false;
			scopeFlag[2][1] = false;
			scopeFlag[0][0] = true;
			scopeFlag[0][2] = true;
			scopeFlag[2][2] = true;
		}

		if (processNomber == 4)
		{
			scopeFlag[0][0] = false;
			scopeFlag[0][2] = false;
			scopeFlag[2][2] = false;
			scopeFlag[1][0] = true;
			scopeFlag[1][1] = true;
			scopeFlag[2][1] = true;
		}
#pragma endregion

#pragma region SlantingWindScope
		if (processNomber == 5)
		{
			scopeFlag[1][0] = false;
			scopeFlag[0][1] = false;
			scopeFlag[1][2] = false;
			scopeFlag[2][1] = false;
			scopeFlag[2][0] = true;
			scopeFlag[0][2] = true;
		}

		if (processNomber == 6)
		{
			scopeFlag[2][0] = false;
			scopeFlag[1][1] = false;
			scopeFlag[0][2] = false;
			scopeFlag[1][0] = true;
			scopeFlag[0][1] = true;
			scopeFlag[1][2] = true;
			scopeFlag[2][1] = true;
		}

		if (processNomber == 7)
		{
			scopeFlag[1][0] = false;
			scopeFlag[0][1] = false;
			scopeFlag[1][2] = false;
			scopeFlag[2][1] = false;
			scopeFlag[0][0] = true;
			scopeFlag[1][1] = true;
			scopeFlag[2][2] = true;
			
		}

		if (processNomber == 8)
		{
			scopeFlag[0][0] = false;
			scopeFlag[1][1] = false;
			scopeFlag[2][2] = false;
			scopeFlag[1][0] = true;
			scopeFlag[0][1] = true;
			scopeFlag[1][2] = true;
			scopeFlag[2][1] = true;
		}
#pragma endregion

#pragma region  BlockWindScope
		if (processNomber == 9)
		{
			scopeFlag[1][2] = false;
			scopeFlag[0][0] = true;
			scopeFlag[2][0] = true;
			scopeFlag[1][1] = true;
		}

		if (processNomber == 10)
		{
			scopeFlag[0][0] = false;
			scopeFlag[1][0] = false;
			scopeFlag[2][0] = false;
			scopeFlag[0][2] = true;
			scopeFlag[1][2] = true;
			scopeFlag[2][2] = true;
		}

		if (processNomber == 11)
		{
			scopeFlag[0][1] = false;
			scopeFlag[0][2] = false;
			scopeFlag[1][0] = true;
			scopeFlag[2][0] = true;
		}

		if (processNomber == 12)
		{
			scopeFlag[2][0] = false;
			scopeFlag[2][1] = false;
			scopeFlag[2][2] = false;
			scopeFlag[0][0] = true;
			scopeFlag[0][1] = true;
			scopeFlag[0][2] = true;
		}
#pragma endregion

#pragma region WindTyphoon
		if (processNomber == 13)
		{
			scopeFlag[0][0] = false;
			scopeFlag[0][1] = false;
			scopeFlag[0][2] = false;
			scopeFlag[2][0] = true;
			scopeFlag[2][1] = true;
			scopeFlag[2][2] = true;
		}
#pragma endregion


		if (processNomber == 14)
		{
			audio->StopWave("scopeSE.wav");
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					scopeFlag[x][y] = false;
				}
			}

			processNomber = 1;
			showRangeFlag = false;
			completionFlag = true;
		}

	}
}

void LeafField::Reset()
{
	if (resetFlag == true)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				scopeFlag[x][y] = false;
			}
		}
		showRangeFlag = false;
		completionFlag = false;
		processNomber = 1;
		showTimer = 50;
		audio->StopWave("scopeSE.wav");
		resetFlag = false;
	}
}

void LeafField::Update(int turnNomber, bool modeChangeFlag)
{
	//�^�[��1�̍U���͈͕\��
	if (turnNomber == 1)
	{
		CrossWindScope();
	}
	//�^�[��2�̍U���͈͕\��
	if (turnNomber == 2)
	{
		SlantingWindScope();
	}
	//�^�[��3�̍U���͈͕\��
	if (turnNomber == 3)
	{
		BlockWindScope();
	}
	//�^�[���S�̍U���͈͕\��
	if (turnNomber == 4)
	{
		if (modeChangeFlag == false)
		{
			TempestWind1Scope();
		}
		else
		{
			TempestWind2Scope();
		}
	}

	//�ċN��
	Reset();

#pragma region �X�V�E���f���؂�ւ�
	for (short x = 0; x < 3; x++)
	{
		for (short y = 0; y < 3; y++)
		{
			if (!scopeFlag[x][y])
			{
				obj_Field[x][y]->SetModel(model_Normal);
			}
			else if (scopeFlag[x][y])
			{
				obj_Field[x][y]->SetModel(model_Scope);
			}

			obj_Field[x][y]->Update();
		}
	}
#pragma endregion
}

void LeafField::Draw()
{
	for (short x = 0; x < 3; x++)
	{
		for (short y = 0; y < 3; y++) {

			obj_Field[x][y]->Draw();
		}
	}
}
