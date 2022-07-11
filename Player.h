#pragma once
#include<model.h>
#include<WorldTransform.h>
#include<Input.h>
#include<DebugText.h>
#include"PlayerBullet.h"
#include<memory>
#include<list>

class Player
{
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	uint32_t textureHandle_ = 0;
	//PlayerBullet* bullet_ = nullptr;
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

public:
	void Initialize(Model* model,uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Attack();
};

