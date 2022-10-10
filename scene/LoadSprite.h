#pragma once

/// <summary>
/// �X�v���C�g�i���o�[
/// </summary>
enum SpriteNumbr
{
	// �f�o�b�N�e�L�X�g
	debugTextTexNumber,
	// �^�C�g���X�v���C�g
	titleNumber,
	// �}�j���A���X�v���C�g
	manualNumber,
	// �N���A�X�v���C�g
	clearNumber,
	// �I�[�o�[�X�v���C�g
	overNumber,
	// �X�e�[�WF
	stageF_Name,
	// �X�e�[�WW
	stageW_Name,
	// �X�e�[�WL
	stageL_Name,
	// �C�����[�W�����X�v���C�g
	IllustrationNumber,
	// �X�e�[�W1�X�v���C�g
	round1Number,
	// �X�e�[�W2�X�v���C�g
	round2Number,
	// �X�e�[�W3�X�v���C�g
	round3Number,
	//�X�e�[�W4�X�v���C�g
	round4Number,
	//�v���C���[UI
	parts_Back,
	parts_Front,
	parts_Hp,
	parts_Mark_F,
	parts_Mark_W,
	parts_Mark_L,
	parts_ChangeTimer,
	//�G�l�~�[UI
	parts_Bar,
	parts_Hp_F,
	parts_Hp_W,
	parts_Hp_L,
	parts_Mark_N,
	parts_Mark1_F,
	parts_Mark2_F,
	parts_Mark3_F,
	parts_Mark4_F,
	parts_Mark1_W,
	parts_Mark2_W,
	parts_Mark3_W,
	parts_Mark4_W,
	parts_Mark1_L,
	parts_Mark2_L,
	parts_Mark3_L,
	parts_Mark4_L,
	//�X�e�[�W�Z���N�g
	select_BackGround,
	select_MarkF,
	select_MarkW,
	select_MarkL,
	select_Complete

};


/// <summary>
/// �X�v���C�g���[�h�N���X
/// </summary>
class LoadSprite
{
public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize();

};