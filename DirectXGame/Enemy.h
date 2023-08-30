#pragma once
#include"BaseCharacter.h"
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <vector>
#include "Collider.h"
#include "EnemyBullet.h"
#include <list>
#include "Effect.h"
#include <memory>

// ゲームシーンの前方宣言
class GameScene;

class Enemy : public BaseCharacter, public Collider {
public:

	Enemy();
	~Enemy();

	// 初期化
	void Initialize(const std::vector<Model*>& models) override;

	// 更新
	void Update() override;

	// 描画
	void Draw(const ViewProjection& viewProjection) override;

	void OnCollision(Collider* collider) override;

	Vector3 GetWorldPosition() override;

	//攻撃間隔
	static const int32_t kAttackTime = 150;

	// ゲームシーンのセット
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	// 初期座標のセット
	void SetStartPosition(Vector3 position);

	// 生きているかどうかの判定取得
	bool IsDead() const { return isDead_; }

	//エフェクトの死亡フラグ取得
	bool IsDeadEffect() const { return effect_->IsDead(); }

	void UpdateColor();

	void Burst();

private:

	const int kModelIndexBody = 0;
	const int kModelIndexL_arm = 1;
	const int kModelIndexR_arm = 2;

	WorldTransform worldTransformBody_;
	/*WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;*/

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	//エフェクト
	std::unique_ptr<Effect> effect_;

	//速度
	Vector3 velocity_{};

	bool isDead_ = false;

	bool isEffectTime_ = false;

	//攻撃
	void Attack();

	int32_t attackTimer_ = kAttackTime;

	//体力
	const int32_t kMaxLife = 3;

	int32_t life_ = kMaxLife;

	//エフェクト開始
	void StartEffect();

	const int32_t kColorRed = 0;
	const int32_t kColorGreen = 1;
	const int32_t kColorBlue = 2;

	uint32_t colorTex_[3]{};

	uint32_t enemyTex_ = 0u;

};
