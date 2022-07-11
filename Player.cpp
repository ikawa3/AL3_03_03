#include "Player.h"
#include<cassert>


void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);
	model_= model;
	textureHandle_ = textureHandle;


	input_ = Input::GetInstance();
	debugText_=DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update()
{
	// �L�����N�^�[�̈ړ�����
	const float speed = 0.5f;

	// ��L�[����������
	if (input_->PushKey(DIK_UP))
	{
		worldTransform_.translation_.y += speed;
	}
	// ���L�[����������
	if (input_->PushKey(DIK_DOWN))
	{
		worldTransform_.translation_.y -= speed;
	}
	// ���L�[����������
	if (input_->PushKey(DIK_LEFT))
	{
		worldTransform_.translation_.x -= speed;
	}
	// �E�L�[����������
	if (input_->PushKey(DIK_RIGHT))
	{
		worldTransform_.translation_.x += speed;
	}

	//A�L�[����������
	if (input_->PushKey(DIK_A))
	{
		//�v���X��]
		worldTransform_.rotation_.y += 0.01f;
	}

	//D�L�[����������
	if (input_->PushKey(DIK_D))
	{
		//�}�C�i�X��]
		worldTransform_.rotation_.y -= 0.01f;
	}

	//�s��̍X�V
	//Y����]
	Matrix4 matRotY = MathUtility::Matrix4Identity();
	matRotY.m[0][0] = cosf(worldTransform_.rotation_.y);
	matRotY.m[0][2] = sinf(worldTransform_.rotation_.y);
	matRotY.m[2][0] = -sinf(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cosf(worldTransform_.rotation_.y);

	// ���s�ړ��s���錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();// �P�ʍs��
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	// ���[���h�s���P�ʍs��ŏ�����
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	//Y���̉�]�s���������
	worldTransform_.matWorld_ *= matRotY;
	// ���[���h�s��ɕ��s�ړ��s���������
	worldTransform_.matWorld_ *= matTrans;
	// ���[���h�s��̓]��
	worldTransform_.TransferMatrix();

	const float kMoveLimitX = 33; 
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//�U������
	Attack();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//�e�̓o�^
		bullets_.push_back(std::move(newBullet));
	}
}