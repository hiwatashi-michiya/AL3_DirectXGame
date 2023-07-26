#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>

class Skydome {
public:
	Skydome();
	~Skydome();

	//初期化
	void Initialize(Model* model);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

private:

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;

};

