#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();
	viewProjection_.translation_.y = 5.0f;
	viewProjection_.farZ = 2000.0f;
	model_.reset(Model::Create());
	// 自キャラの生成
	modelFighterBody_.reset(Model::CreateFromOBJ("float_body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelWeapon_.reset(Model::CreateFromOBJ("weapon", true));
	//自キャラモデル
	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get(), modelWeapon_.get()};
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize(playerModels);
	//敵キャラモデルの生成
	modelEnemyBody_.reset(Model::CreateFromOBJ("enemy", true));
	modelEnemyArm_.reset(Model::CreateFromOBJ("enemy_arm", true));
	std::vector<Model*> enemyModels = {
	    modelEnemyBody_.get(), modelEnemyArm_.get(), modelEnemyArm_.get()};
	//敵キャラ初期化
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModels);
	//天球初期化
	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());
	//地面初期化
	groundModel_.reset(Model::CreateFromOBJ("ground", true));
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());
	//デバッグカメラ初期化
	debugCamera_.reset(new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight));
	debugCamera_->SetFarZ(2000.0f);
	//追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	//自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	//自キャラに追従カメラをセット
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() { 
	
	player_->Update();

	enemy_->Update();

	//追従カメラの更新
	followCamera_->Update();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_E)) {

		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}

	}


#endif // _DEBUG

	if (isDebugCameraActive_) {

		//デバッグカメラ更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクションの転送
		viewProjection_.TransferMatrix();

	}
	else {

		//追従カメラのビュー行列、プロジェクション行列をそれぞれコピー
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション更新
		viewProjection_.TransferMatrix();
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
