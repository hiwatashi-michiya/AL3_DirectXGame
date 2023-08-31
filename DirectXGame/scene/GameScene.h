#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include <memory>
#include "Skydome.h"
#include "DebugCamera.h"
#include "Ground.h"
#include "FollowCamera.h"
#include "Enemy.h"
#include "GlobalVariables.h"
#include "CollisionManager.h"
#include <list>
#include"EnemyBullet.h"
#include <sstream>
#include "Score.h"
#include "Effect.h"
#include "GameTimer.h"

enum Scene {
	TITLE,
	INGAME,
	RESULT
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	Player* GetPlayer() const { return player_.get(); }

	void AddKillCount() { killCount_++; }

	void ResetKillCount() { killCount_ = 0; }

	void SetMagScoreTimer() { resetMagScoreTimer_ = kResetMagScoreTime; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	GlobalVariables* globalVariables_ = nullptr;
	Score* score_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	std::unique_ptr<Model> model_ = nullptr;

	ViewProjection viewProjection_;

	//自キャラ
	std::unique_ptr<Player> player_ = nullptr;
	//自キャラモデル
	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelArrow_;

	
	//敵キャラモデル
	std::unique_ptr<Model> modelEnemyBody_;
	std::unique_ptr<Model> modelEnemyArm_;

	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//天球モデル
	std::unique_ptr<Model> skydomeModel_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;
	//地面モデル
	std::unique_ptr<Model> groundModel_ = nullptr;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
	// エリアモデル
	std::unique_ptr<Model> areaModel_ = nullptr;
	//エリアテクスチャ
	uint32_t areaTex_ = 0u;
	//エリアのワールド変換データ
	WorldTransform worldTransformArea_;

	//デバッグカメラ有効化フラグ
	bool isDebugCameraActive_ = false;

	// 衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	void UpdateCollisionManager();

	// 敵キャラ
	std::list<Enemy*> enemies_;

	// 敵弾
	std::list<EnemyBullet*> enemyBullets_;

	// 敵生成関数
	void EnemyPop(Vector3 position, int32_t color);

	// 待機フラグ
	bool isWait = false;

	// 待機タイマー
	int32_t waitTimer = 0;

	int32_t killCount_ = 0;

	//スコア倍率リセットまでのタイマー
	const int32_t kResetMagScoreTime = 180;

	int32_t resetMagScoreTimer_ = 0;

	//ゲーム中の制限時間
	GameTimer* gameTimer_ = nullptr;

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetEnemyBullets() { return enemyBullets_; }

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// タイトル画面更新
	/// </summary>
	void TitleUpdate();

	/// <summary>
	/// インゲーム更新
	/// </summary>
	void GamePlayUpdate();

	/// <summary>
	/// リザルト更新
	/// </summary>
	void ResultUpdate();

	/// <summary>
	/// ゲームリセット
	/// </summary>
	void Reset();
	
	//リセットフラグ
	bool isReset_ = false;

	//画面切り替えの時間
	const int32_t kChangeSceneTimer = 60;

	int32_t changeSceneTimer_ = 0;

	uint32_t changeSceneTex_ = 0u;
	std::unique_ptr<Sprite> spriteChangeScene_ = nullptr;

	bool isChangeScene_ = false;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	
	//ゲームシーン切り替え
	Scene currentScene_;
	Scene nextScene_;

	//UI
	uint32_t pressTitleTex_ = 0u;
	uint32_t pressIngameTex_ = 0u;
	uint32_t changeColorTex_ = 0u;
	uint32_t attackTex_ = 0u;
	uint32_t burstTex_ = 0u;
	uint32_t moveTex_ = 0u;
	uint32_t titleTex_ = 0u;

	std::unique_ptr<Sprite> spritePressTitle_;
	std::unique_ptr<Sprite> spritePressIngame_;
	std::unique_ptr<Sprite> spriteChangeColor_;
	std::unique_ptr<Sprite> spriteAttack_;
	std::unique_ptr<Sprite> spriteBurst_;
	std::unique_ptr<Sprite> spriteMove_;
	std::unique_ptr<Sprite> spriteTitle_;

	//audio
	uint32_t BGM_ = 0u;
	uint32_t selectSE_ = 0u;
	uint32_t gameoverSE_ = 0u;

};

