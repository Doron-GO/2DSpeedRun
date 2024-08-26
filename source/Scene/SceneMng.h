#pragma once
#include<memory>
#include<list>
class Input;
class Scene;

class SceneMng
{
public:

	/// <summary>
	/// 先頭の(Updateが呼ばれる)シーンを切り替える
	/// </summary>
	/// <param name="scene">切り替え先のシーン</param>
	void ChangeScene(std::shared_ptr<Scene>scene);
	/// <summary>
	/// スタックの頭のシーンを削除する
	/// ただし、スタック上に１つしかシーンがない場合は
	/// 削除しない
	/// </summary>
	/// <param name="scene"></param>
	/// <summary>
	void PopScene();
	/// シーンをプッシュする。スタックの数が増える
	/// 一番上Updateしか呼ばれません
	/// </summary>
	/// <param name="scene"></param>
	void PushScene(std::shared_ptr<Scene>scene);
	/// <summary>
	/// 持っているシーンのアップデートを呼びさす
	/// </summary>
	/// <param name="input"></param>
	void Update(Input& input);
	void Draw();

private:

	//現在進行中のシーン
	std::list<std::shared_ptr<Scene>> scenes_;


};

