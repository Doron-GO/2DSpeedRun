#pragma once
#include<DxLib.h>
#include<string>
#include<unordered_map>
#include<vector>

//入力種別
enum class InputType {

	keybord,//キーボード
	gamepad,//ゲームパッド
	mouse	//マウス
};

//入力情報(一つ当たり)
struct InputInfo {
	InputType type;//入力種別
	//実際に調べる値(PADやマウスならビットを表しキーボードならキーコードを)
	//表す
	int inputID;
};


//ゲーム中の意味と入力されたかどうかをペアにしたもの
//とにかく対象の「意味」が押されたらああ地がtrueになる型
using InputState_t = std::unordered_map<std::string, bool>;

//ゲーム中の"意味"と実際のキーコードのペア
//キー(first)は命令の意味、
//ゲーム中の意味と実際の意味の入力(ハード入力)の対応表を表す「型」
using InputTable_t =
std::unordered_map<std::string, std::vector<InputInfo>>;

class KeyConfigScene;

class Input
{
	friend KeyConfigScene;

private:
	
	std::vector<std::string>excludeOnCongigList_;//キーコンフィグ除外リスト

	//意味とハード入力の対応表
	InputTable_t  inputTable_;
	InputTable_t  tempTable_;

	//押した記録
	InputState_t lastState_={};//さっき
	InputState_t currentState_={};//今

	//値の代入とコピーの禁止
	Input(const Input&) = delete;
	void operator = (const Input&) = delete;

	//現在そのキーが押されているか
	bool GetCurrentInput(const std::string& input)const;
	//直前にそのキーが押されているか
	bool GetLastInput(const std::string& input)const;

public:

	DINPUT_JOYSTATE joyDInState_;

	Input();
	/// <summary>
	/// キー入力情報を更新する(毎フレーム)
	/// </summary>
	void Update(int padNum);
	/// <summary>
	/// 今押した瞬間かどうかを返す(押しっぱなしはfalse)
	/// </summary>
	/// <param name="input">入力識別(nextやpause)</param>
	/// <returns></returns>
	bool IsTriggerd(const std::string& input)const;
	/// <summary>
	/// 今押したかどうかを返す
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	bool IsPrassed(const std::string& input)const;

	/// <summary>
	/// 入力テーブルの書き換え
	/// </summary>
	/// <param name="key">命令文字列</param>
	/// <param name="inputinfo">機器からの入力情報(ハード)</param>
	void RewriteInput(const std::string& key,const InputInfo& inputinfo);
	void ComitInputConfig();

	//入力テーブル自体を表示する
	const InputTable_t GetInputTable()const;

	void SaveInputTable();
	void LoadInputTable();

	/// <summary>
	/// 現在の入力が除外入力に当たるのかどうかをチェックする
	/// </summary>
	/// <returns>true:除外入力に当たる,false:当たらない</returns>
	bool CheckExclusiveInput();


};

