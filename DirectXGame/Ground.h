#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>

class Ground {
public:
	Ground();
	~Ground();

	//初期化
	void Initialize(Model* model);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection viewProjection);


private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

};
