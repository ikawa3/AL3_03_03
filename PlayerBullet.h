#pragma once
#include<model.h>
#include<WorldTransform.h>
#include<Input.h>
#include<DebugText.h>

class PlayerBullet
{
public:
	void Initialize(Model* model,const Vector3& position);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	uint32_t textureHandle_ = 0;
};

