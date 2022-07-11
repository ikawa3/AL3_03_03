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
	// キャラクターの移動速さ
	const float speed = 0.5f;

	// 上キーを押したら
	if (input_->PushKey(DIK_UP))
	{
		worldTransform_.translation_.y += speed;
	}
	// 下キーを押したら
	if (input_->PushKey(DIK_DOWN))
	{
		worldTransform_.translation_.y -= speed;
	}
	// 左キーを押したら
	if (input_->PushKey(DIK_LEFT))
	{
		worldTransform_.translation_.x -= speed;
	}
	// 右キーを押したら
	if (input_->PushKey(DIK_RIGHT))
	{
		worldTransform_.translation_.x += speed;
	}

	//Aキーを押したら
	if (input_->PushKey(DIK_A))
	{
		//プラス回転
		worldTransform_.rotation_.y += 0.01f;
	}

	//Dキーを押したら
	if (input_->PushKey(DIK_D))
	{
		//マイナス回転
		worldTransform_.rotation_.y -= 0.01f;
	}

	//行列の更新
	//Y軸回転
	Matrix4 matRotY = MathUtility::Matrix4Identity();
	matRotY.m[0][0] = cosf(worldTransform_.rotation_.y);
	matRotY.m[0][2] = sinf(worldTransform_.rotation_.y);
	matRotY.m[2][0] = -sinf(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cosf(worldTransform_.rotation_.y);

	// 平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();// 単位行列
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	// ワールド行列を単位行列で初期化
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	//Y軸の回転行列をかける
	worldTransform_.matWorld_ *= matRotY;
	// ワールド行列に平行移動行列をかける
	worldTransform_.matWorld_ *= matTrans;
	// ワールド行列の転送
	worldTransform_.TransferMatrix();

	const float kMoveLimitX = 33; 
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//攻撃処理
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

		//弾の登録
		bullets_.push_back(std::move(newBullet));
	}
}