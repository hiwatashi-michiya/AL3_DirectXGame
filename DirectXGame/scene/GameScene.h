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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	std::unique_ptr<Model> model_ = nullptr;

	ViewProjection viewProjection_;

	//自キャラ
	std::unique_ptr<Player> player_ = nullptr;
	//自キャラモデル
	std::unique_ptr<Model> playerModel_ = nullptr;
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

	//デバッグカメラ有効化フラグ
	bool isDebugCameraActive_ = false;

};

