#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Input.h>
#include <memory>
#include "BaseCharacter.h"
#include <optional>

//振る舞い
enum class Behavior {
	kRoot, //通常状態
	kAttack, //攻撃中
};

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
	const int kModelIndexWeapon = 4;

	//振る舞い
	Behavior behavior_ = Behavior::kRoot;

	//次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

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

	//通常行動更新
	void BehaviorRootUpdate();

	//通常行動初期化
	void BehaviorRootInitialize();

	//攻撃行動更新
	void BehaviorAttackUpdate();

	//攻撃行動初期化
	void BehaviorAttackInitialize();

	int attackFrame = 0;

	//ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

};

