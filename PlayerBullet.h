#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Input.h>

//自キャラの弾
class PlayerBullet {
public:
	PlayerBullet();
	~PlayerBullet();
	
	//初期化
	void Initialize(Model* model, const Vector3& position);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

};

