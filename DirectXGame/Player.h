#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Input.h>
#include <memory>
#include "BaseCharacter.h"

class Player : public BaseCharacter {
public:
	Player();
	~Player();

	//初期化
	void Initialize(const std::vector<Model*>& models) override;

	//更新
	void Update() override;

	//描画
	void Draw(const ViewProjection& viewProjection) override;

	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:

	const int kModelIndexBody = 0;
	const int kModelIndexHead = 1;
	const int kModelIndexL_arm = 2;
	const int kModelIndexR_arm = 3;

	//浮遊ギミック初期化
	void InitializeFloatingGimmick();
	
	//浮遊ギミック更新
	void UpdateFloatingGimmick();

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	// 浮遊移動のサイクル<frame>
	int cycle = 120;

	// 浮遊の振幅<m>
	float swing = 1.0f;

	//ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

};

