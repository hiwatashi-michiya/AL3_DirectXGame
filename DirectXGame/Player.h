#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Input.h>
#include <memory>
#include "BaseCharacter.h"
#include <optional>
#include <list>
#include <vector>
#include "Bullet.h"
#include "Collider.h"
#include <TextureManager.h>
#include <Audio.h>

//振る舞い
enum class Behavior {
	kRoot, //通常状態
	kAttack, //攻撃中
};

class Player : public BaseCharacter, public Collider {
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

	void OnCollision(Collider* collider) override;

	Vector3 GetWorldPosition() override;

	// 弾
	const std::list<Bullet*> GetBullets() { return bullets_; }

	float GetBurstRadius() { return kBurstRadius; }

	void BurstAttack();

	//バーストのゲッターとセッター
	bool GetIsBurst() { return isBurst_; }

	void SetIsBurst(bool flag) { isBurst_ = flag; }

	int32_t GetBurstCoolTimer() { return burstCoolTimer_; }

	//無敵中かどうかのゲッター
	bool GetIsInvincible() { return isInvincible_; }

	void SetPosition(const Vector3& position) { worldTransformBase_.translation_ = position; }

	void Reset();

private:

	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	const int kModelIndexBody = 0;
	const int kModelIndexHead = 1;
	const int kModelIndexL_arm = 2;
	const int kModelIndexR_arm = 3;
	const int kModelIndexWeapon = 4;
	const int kModelIndexArrow = 5;

	const float kBurstRadius = 40.0f;

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

	//調整項目の適用
	void ApplyGlobalVariables();

	//攻撃
	void Attack();

	//音鳴らしフラグ
	bool isPlaySE_ = false;

	//クールタイム
	const int32_t kCoolTime = 10;

	//バースト攻撃をしたか
	bool isBurst_ = false;

	//バーストクールタイム
	const int32_t kBurstCoolTime = 300;

	int32_t burstCoolTimer_ = 0;

	//無敵時間かどうか
	bool isInvincible_ = false;

	//無敵時間
	const int kInvincibleTime = 180;

	//ダメージ受けたときの無敵時間
	int32_t invincibleTimer_ = 0;

	//弾発射のクールタイム
	int32_t coolTimer_ = 0;

	//弾
	std::list<Bullet*> bullets_;

	//弾モデル
	std::unique_ptr<Model> modelBullet_ = nullptr;

	std::unique_ptr<Model> modelBurst_ = nullptr;

	std::unique_ptr<Model> modelEffect_ = nullptr;

	//エフェクトフラグ
	bool isEffect_ = false;

	const int32_t kMaxEffectTime = 10;

	int32_t effectTimer_ = 0;

	//ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;
	WorldTransform worldTransformBurst_;
	WorldTransform worldTransformArrow_;
	WorldTransform worldTransformEffect_;

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	int32_t bulletColor_ = Type::C_RED;

	const int32_t kColorRed = 0;
	const int32_t kColorGreen = 1;
	const int32_t kColorBlue = 2;

	uint32_t colorTex_[3]{};

	uint32_t playerColorTex_[3]{};

	uint32_t burstTex_ = 0u;

	uint32_t playerTex_ = 0u;

	uint32_t changeColorSE_ = 0u;
	uint32_t burstSE_ = 0u;

};

