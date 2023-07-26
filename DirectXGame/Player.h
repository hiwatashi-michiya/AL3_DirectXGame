#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Input.h>
#include <memory>

class Player {
public:
	Player();
	~Player();

	//初期化
	void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:

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
	//モデル
	Model* modelFighterBody_ = nullptr;
	Model* modelFighterHead_ = nullptr;
	Model* modelFighterL_arm_ = nullptr;
	Model* modelFighterR_arm_ = nullptr;

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

};

