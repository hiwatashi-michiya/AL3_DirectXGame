#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <list>
#include "Enemy.h"
#include <DebugCamera.h>
#include <Input.h>

//敵情報の構造体
struct EnemyData {

	Vector3 position;
	WorldTransform worldTransform;
	uint32_t textureHandle;
	TYPE type;

};

class ScriptEditor {
public:
	ScriptEditor();
	~ScriptEditor();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void Draw(ViewProjection viewProjection);

	//エディター開いているかどうかの判定
	bool GetIsEdit() { return isEdit_; }

private:

	bool isEdit_;

	std::list<EnemyData> enemyData_;

	Input* input_ = nullptr;

	Model* model_ = nullptr;

};


