#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	globalVariables_ = GlobalVariables::GetInstance();
	score_ = Score::GetInstance();
	score_->Initialize();
	//現在シーンの設定
	currentScene_ = INGAME;
	nextScene_ = INGAME;

	viewProjection_.Initialize();
	viewProjection_.translation_.y = 5.0f;
	viewProjection_.farZ = 2000.0f;
	model_.reset(Model::Create());
	// 自キャラの生成
	modelFighterBody_.reset(Model::CreateFromOBJ("player_body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("player_head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelWeapon_.reset(Model::CreateFromOBJ("weapon", true));
	modelArrow_.reset(Model::CreateFromOBJ("arrow", true));
	//自キャラモデル
	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get(), modelWeapon_.get(), modelArrow_.get()};
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize(playerModels);
	//敵キャラモデルの生成
	modelEnemyBody_.reset(Model::CreateFromOBJ("enemy", true));
	modelEnemyArm_.reset(Model::CreateFromOBJ("enemy_arm", true));
	LoadEnemyPopData();
	
	//天球初期化
	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());
	//地面初期化
	groundModel_.reset(Model::CreateFromOBJ("ground", true));
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());
	//エリア初期化
	areaModel_.reset(Model::CreateFromOBJ("area", true));
	areaTex_ = TextureManager::Load("area/area.png");
	worldTransformArea_.Initialize();
	worldTransformArea_.scale_ *= 100.0f;
	worldTransformArea_.UpdateMatrix();
	//ゲームタイマー初期化
	gameTimer_ = GameTimer::GetInstance();
	gameTimer_->Initialize();
	gameTimer_->ResetGameTime();

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

	collisionManager_ = std::make_unique<CollisionManager>();

	changeSceneTex_ = TextureManager::Load("UI/changescene.png");
	spriteChangeScene_.reset(Sprite::Create(changeSceneTex_, {0.0f, 0.0f}));
	spriteChangeScene_->SetSize({1280.0f, 720.0f});

}

void GameScene::Update() { 
	
	spriteChangeScene_->SetColor(Vector4(1.0f,1.0f,1.0f, 1.0f * float(changeSceneTimer_) / 60.0f));

	switch (currentScene_) {
	default:
	case TITLE:
		TitleUpdate();
		break;
	case INGAME:
		GamePlayUpdate();
		break;
	case RESULT:
		ResultUpdate();
		break;
	}

	

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

	switch (currentScene_) {
	default:
	case TITLE:

	case INGAME:
		
		skydome_->Draw(viewProjection_);
		ground_->Draw(viewProjection_);
		player_->Draw(viewProjection_);

		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Draw(viewProjection_);
		}

		for (Enemy* enemy : enemies_) {
			enemy->Draw(viewProjection_);
		}

		areaModel_->Draw(worldTransformArea_, viewProjection_, areaTex_);

		break;
	case RESULT:
		
		break;
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

	switch (currentScene_) {
	default:
	case TITLE:

		break;
	case INGAME:

		score_->Draw();
		gameTimer_->Draw();

		if (gameTimer_->GetGameTime() <= 0) {

		}

		break;
	case RESULT:

		break;
	}

	spriteChangeScene_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//タイトル
void GameScene::TitleUpdate() {

	if (isReset_) {
		isReset_ = false;
	}

	if (isChangeScene_ && nextScene_ == INGAME) {

		if (++changeSceneTimer_ >= 60) {

			player_->Reset();
			currentScene_ = nextScene_;
			gameTimer_->SetIsCount(true);

		}

	}
	else if (isChangeScene_ && nextScene_ == TITLE) {

		if (--changeSceneTimer_ <= 0) {
			isChangeScene_ = false;
		}

	}

	player_->Update();

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		if (((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
		     joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ||
		    input_->TriggerKey(DIK_SPACE)) && isChangeScene_ == false) {

			isChangeScene_ = true;
			nextScene_ = INGAME;
		}
	}

	// 追従カメラの更新
	followCamera_->Update();

}

//インゲーム
void GameScene::GamePlayUpdate() {

	if (gameTimer_->GetIsCount() == false) {

		XINPUT_STATE joyState;

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {

			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && isReset_ == false) {

				isReset_ = true;
				isChangeScene_ = true;
				nextScene_ = TITLE;

			}

		}

	}

	if (isChangeScene_ && nextScene_ == TITLE) {

		if (++changeSceneTimer_ >= 60) {
			Reset();
			currentScene_ = nextScene_;
		}

	}
	else if (isChangeScene_ && nextScene_ == INGAME) {

		if (--changeSceneTimer_ <= 0) {
			isChangeScene_ = false;
		}

	}
	else if (isReset_ == false) {

		gameTimer_->Update();

		UpdateEnemyPopCommands();

	

	}

		enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}

		return false;
	});

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead() && enemy->IsDeadEffect()) {

			delete enemy;
			return true;
		}

		return false;
	});

	player_->Update();

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	if (gameTimer_->GetGameTime() > 0) {

		UpdateCollisionManager();
	}

	score_->Update();

	// 追従カメラの更新
	followCamera_->Update();

}

//リザルト
void GameScene::ResultUpdate() {

}

void GameScene::UpdateCollisionManager() {

	collisionManager_->ClearList();

	// 自弾リストの取得
	const std::list<Bullet*>& playerBullets = player_->GetBullets();
	
	for (Enemy* enemy : enemies_) {

		if (enemy->IsDead() == false) {
			collisionManager_->PushCollider(enemy);
		}

	}

	for (EnemyBullet* bullet : enemyBullets_) {
		collisionManager_->PushCollider(bullet);
	}

	//無敵中でなければプレイヤーの当たり判定を追加
	if (player_->GetIsInvincible() == false) {
		collisionManager_->PushCollider(player_.get());
	}
	// 自弾全てについて
	for (Bullet* bullet : playerBullets) {
		collisionManager_->PushCollider(bullet);
	}

	

	collisionManager_->CheckAllCollisions();

	//バーストしたら範囲内の同色エネミーを全員倒す
	if (player_->GetIsBurst()) {
		
		for (Enemy* enemy : enemies_) {

			if (Distance(player_->GetWorldPosition(), enemy->GetWorldPosition()) <=
				HitRadius(player_->GetBurstRadius(), enemy->GetRadius()) &&
				player_->GetColorType() == enemy->GetColorType()) {
				enemy->Burst();
			}

		}

		//バースト終了
		player_->SetIsBurst(false);

	}

}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData() {

	enemyPopCommands.clear();

	// ファイルを開く
	std::ifstream file;
	file.open("./Resources/Commands/enemyPopAct.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::EnemyPop(Vector3 position, int32_t color) {

	// 敵キャラの生成
	Enemy* newEnemy = new Enemy();
	std::vector<Model*> enemyModels = {
	    modelEnemyBody_.get()};
	// 敵キャラの初期化
	newEnemy->Initialize(enemyModels);
	newEnemy->SetGameScene(this);
	// 初期位置を決める
	newEnemy->SetStartPosition(position);
	newEnemy->SetColorType(color);
	newEnemy->UpdateColor();
	// 敵キャラに自キャラのアドレスを渡す
	/*newEnemy->SetPlayer(player_);*/

	// リストに敵キャラを登録
	enemies_.push_back(newEnemy);
}

void GameScene::UpdateEnemyPopCommands() {

	// 待機処理
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			isWait = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//色
			getline(line_stream, word, ',');
			int32_t color = (int32_t)std::atof(word.c_str());

			// 敵を発生させる
			EnemyPop(Vector3(x, y, z), color);

		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {

			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWait = true;
			waitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::Reset() {

	LoadEnemyPopData();

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		delete bullet;
		return true;
	});

	enemies_.remove_if([](Enemy* enemy) {
		delete enemy;
		return true;
	});

	score_->ResetScore();

	player_->Reset();

	gameTimer_->ResetGameTime();

}
