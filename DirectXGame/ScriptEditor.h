#pragma once

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


private:

	bool isEdit_ = true;

};


