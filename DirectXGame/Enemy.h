#pragma once
#include"BaseCharacter.h"
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <vector>

class Enemy : public BaseCharacter {
public:

	// 初期化
	void Initialize(const std::vector<Model*>& models) override;

	// 更新
	void Update() override;

	// 描画
	void Draw(const ViewProjection& viewProjection) override;

private:

	const int kModelIndexBody = 0;
	const int kModelIndexL_arm = 1;
	const int kModelIndexR_arm = 2;

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

};
