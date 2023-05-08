#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>

enum class Phase {
	Approach, //接近
	Leave, //離脱
};

class BaseEnemyState;

class Enemy {
public:
	Enemy();
	~Enemy();

	//初期化
	void Initialize(Model* model, uint32_t textureHandle);
	//更新
	void Update();
	//描画
	void Draw(ViewProjection viewProjection);
	//座標を変更する関数
	void Move(const Vector3& velocity);
	//座標のゲッター
	Vector3 GetPosition();
	//状態遷移
	void ChangeState(BaseEnemyState* state);

private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//フェーズ
	BaseEnemyState* state_;

};

class BaseEnemyState {
public:
	BaseEnemyState();
	~BaseEnemyState();

	virtual void Update();
	virtual void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

protected:
	Enemy* enemy_;
};

class EnemyStateApproach : public BaseEnemyState {
public:
	
	void Update();
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

private:
};

class EnemyStateLeave : public BaseEnemyState {
public:
	
	void Update();
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

private:
};


