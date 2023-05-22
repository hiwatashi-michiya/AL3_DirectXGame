#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();
	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyTextureHandle_ = TextureManager::Load("enemy.png");
	model_ = Model::Create();
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	//敵キャラの生成
	enemy_ = new Enemy();
	//敵キャラの初期化
	enemy_->Initialize(model_, enemyTextureHandle_);
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() { 
	
	player_->Update();
	if (enemy_) {
		enemy_->Update();
	}

	CheckAllCollisions();

#ifdef _DEBUG

	//特定のキー入力でデバッグカメラの切り替え
	if (input_->TriggerKey(DIK_0)) {

		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		}
		else {
			isDebugCameraActive_ = false;
		}

	}

#endif // _DEBUG


	//カメラの処理
	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクションの転送
		viewProjection_.TransferMatrix();
	}
	else {
		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
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
	player_->Draw(viewProjection_);
	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}

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

void GameScene::CheckAllCollisions() {

	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		

		//球と球の交差判定
		if (Distance(posA, posB) <= HitRadius(player_->GetRadius(), bullet->GetRadius())) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}

	#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定

	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();

	// 敵キャラと自弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		posB = bullet->GetWorldPosition();

		// 球と球の交差判定
		if (Distance(posA, posB) <= HitRadius(enemy_->GetRadius(), bullet->GetRadius())) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

	#pragma endregion

	#pragma region 自弾と敵弾の当たり判定

	//自弾と敵弾全ての当たり判定
	for (PlayerBullet* pBullet : playerBullets) {
		//自弾の座標
		posA = pBullet->GetWorldPosition();
		for (EnemyBullet* eBullet : enemyBullets) {
			//敵弾の座標
			posB = eBullet->GetWorldPosition();
			// 球と球の交差判定
			if (Distance(posA, posB) <= HitRadius(pBullet->GetRadius(), eBullet->GetRadius())) {
				// 自弾の衝突時コールバックを呼び出す
				pBullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				eBullet->OnCollision();
			}

		}

	}

	#pragma endregion

}
