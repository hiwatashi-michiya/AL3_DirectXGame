#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Input.h>

class Player {
public:
	Player();
	~Player();

	//初期化
	void Initialize(Model* model);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

};

