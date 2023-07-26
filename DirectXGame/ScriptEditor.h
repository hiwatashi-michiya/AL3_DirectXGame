#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <list>
#include "Enemy.h"
#include <DebugCamera.h>
#include <Input.h>
#include <vector>
#include <string>
#include <Sprite.h>

//敵情報の構造体
struct EnemyData {

	WorldTransform worldTransform;
	int type;
	int spawnTimer;

	void Update();

};

class ScriptEditor {
public:
	ScriptEditor();
	~ScriptEditor();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const ViewProjection& viewProjection);

	void Draw(ViewProjection viewProjection);

	void DrawUI();

	//エディター開いているかどうかの判定
	bool GetIsEdit() { return isEdit_; }

	Matrix4x4 GetMatView() const { return matView_; }

	Matrix4x4 GetMatProjection() const { return matProjection_; }

	WorldTransform GetWorldTransform3DReticle() const { return worldTransform3DReticle_; }

private:

	Sprite* sprite_ = nullptr;

	bool isEdit_;

	bool canMoveCamera_;

	char filename_[256] = "";

	std::list<EnemyData*> enemyData_;

	Input* input_ = nullptr;

	// 表示用のモデル
	Model* modelObject_ = nullptr;

	//スクリプト編集モード用のカメラ
	Matrix4x4 matCameraMain_;
	
	//ビュー行列
	Matrix4x4 matView_;

	//プロジェクション行列
	Matrix4x4 matProjection_;

	//メインスケール
	Vector3 scaleMain_;

	//メイン回転
	Vector3 rotateMain_;

	//メイン平行移動
	Vector3 translateMain_;

	//軸固定のカメラ用スケール
	Vector3 scalesSub_[3];

	//軸固定のカメラ用回転
	Vector3 rotatesSub_[3];

	//軸固定のカメラ用平行移動
	Vector3 translateSub_[3];

	//3Dレティクルのワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//カメラからレティクルへの距離
	float distance_;

	//3Dレティクルモデル
	Model* model3DReticle_ = nullptr;

	//テクスチャハンドル
	uint32_t enemyEasyTex_ = 0u;
	uint32_t enemyNormalTex_ = 0u;
	uint32_t enemyHardTex_ = 0u;
	uint32_t textureSelect_ = 0u;

	//敵タイプの文字列格納リスト
	std::vector<const char*> typeString_ = {"EASY", "NORMAL", "HARD"};
	
	//データの総数
	int dataNumber = 0;
	//現在選択しているデータ
	int dataSelectNumber = 0;

private:

	//スクリプト出力
	void Save(const char* filename);

	//ImGuiウィンドウのの範囲内かどうか
	bool CheckWithinWindow();

	//カメラを動かせるか判定
	void JudgeCanMoveCamera();

	//軸固定カメラの初期化
	void SetAxisCamera();

};


