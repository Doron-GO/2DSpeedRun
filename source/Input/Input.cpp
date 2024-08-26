#include "Input.h"
#include<cassert>
#include<algorithm>

bool Input::GetCurrentInput(const std::string& input) const
{
	auto it = currentState_.find(input);
	if (it == currentState_.end())
	{
		return false;
	}
	return it->second;
}

bool Input::GetLastInput(const std::string& input) const
{
	auto it = lastState_.find(input);
	if (it == lastState_.end())
	{
		return false;
	}
	return it->second;
}

Input::Input()
{
	//左辺がゲームの命令(文字列)
	inputTable_["next"] = {
							{ InputType::keybord, KEY_INPUT_RETURN} ,
							{InputType::gamepad,PAD_INPUT_R } ,//padのstart
							{ InputType::mouse,MOUSE_INPUT_LEFT }
							};
	inputTable_["cancel"] = {
							{ InputType::keybord, KEY_INPUT_ESCAPE} 
							};


	inputTable_["pause"] = { 
							{InputType::keybord, KEY_INPUT_P},
							{ InputType::gamepad,PAD_INPUT_L } //padのselect
							};


	//ジャンプ
	inputTable_["jump"] = {
							{ InputType::keybord, KEY_INPUT_Z} ,
							{InputType::gamepad,PAD_INPUT_B} //padのA　プレステの□
							};

	//hukku
	inputTable_["hook"] = {
							{ InputType::keybord, KEY_INPUT_X} ,
							{InputType::gamepad,PAD_INPUT_C} //プレステの×
							};
	//item
	inputTable_["item"] = {
							{ InputType::keybord, KEY_INPUT_V} ,
							{InputType::gamepad,PAD_INPUT_X} //プレステの×
							};

	//スライディング
	inputTable_["c"] = {
							{ InputType::keybord, KEY_INPUT_C} ,
							{InputType::gamepad,PAD_INPUT_A} //padのA　プレステの△

	};

	//上
	inputTable_["up"] = {
							{ InputType::keybord, KEY_INPUT_UP} ,
							{InputType::gamepad,PAD_INPUT_UP} ,//padのA
	};

	//下
	inputTable_["down"] = {
							{ InputType::keybord, KEY_INPUT_DOWN} ,
							{InputType::gamepad,PAD_INPUT_DOWN} ,//padのA
	};
	//右
	inputTable_["right"] = {
							{ InputType::keybord, KEY_INPUT_RIGHT} ,
							{InputType::gamepad,PAD_INPUT_RIGHT} ,//padのA
	};
	//左
	inputTable_["left"] = {
							{ InputType::keybord, KEY_INPUT_LEFT} ,
							{InputType::gamepad,PAD_INPUT_LEFT} ,//padのA
	};
	//キーコンフィグ
	inputTable_["keyconfig"] = {
							{ InputType::keybord, KEY_INPUT_K} ,
	};

	LoadInputTable();

	tempTable_ = inputTable_;
	currentState_.clear();
	for (const auto& record: inputTable_)
	{
		currentState_[record.first] = false;
	}

	excludeOnCongigList_.reserve(5);
	excludeOnCongigList_.push_back("left");
	excludeOnCongigList_.push_back("right");
	excludeOnCongigList_.push_back("up");
	excludeOnCongigList_.push_back("down");
	excludeOnCongigList_.push_back("c");

}

void Input::Update(int padNum)
{
	
	//ハードの入力自体はチェックします
	char keycode[256];
	GetHitKeyStateAll(keycode);//キーボード入力
	auto a= 0x0000 + padNum;
	int padInfo = GetJoypadInputState(a);//パッド１番入力
	auto padInfo2 = GetJoypadDirectInputState(a, &joyDInState_);
	int mouseInfo = GetMouseInput();//マウス入力

	lastState_ = currentState_;//更新前の情報を記録
	//すべての登録済みの入力をチェックする
	for (const auto& record : inputTable_)
	{
		bool pressed = false;
		for (const auto& inputInfo : record.second)
		{
			//入力種別によて調べ方が違う
			switch (inputInfo.type)
			{
			case InputType::keybord:
				pressed = keycode[inputInfo.inputID];
				break;
			case InputType::gamepad:
				pressed = padInfo &inputInfo.inputID;
				break;
			case InputType::mouse:
				pressed = mouseInfo & inputInfo.inputID;
				break;
			}
			//１つでも押されていたらループが止まる
			if (pressed)//ここ大事
			{
				break;
			}
		}
		currentState_[record.first] = pressed;
	}
}

bool Input::IsTriggerd(const std::string& input) const
{

	return !GetLastInput(input) && GetCurrentInput(input);
}

bool Input::IsPrassed(const std::string& input) const
{

	return GetCurrentInput(input);
}

void Input::RewriteInput(const std::string& key, const InputInfo& inputinfo){
	
	auto it = tempTable_.find(key);
	if (it == tempTable_.end())
	{
		assert(0 && "そんな命令は登録されていません");
		return;
	}
	for (auto& record : it->second)
	{
		if (inputinfo.type == record.type)
		{
			record.inputID = inputinfo.inputID;
			return;
		}
	}
	//新規登録
	it->second.push_back(inputinfo);

}

void Input::ComitInputConfig()
{
	inputTable_ = tempTable_;
}

const InputTable_t Input::GetInputTable() const
{
	auto ret = inputTable_;

	for (const auto& ex : excludeOnCongigList_)
	{
		ret.erase(ex);
	}

	return ret;
}

void Input::SaveInputTable()
{
	//FILE* fp=nullptr;
	//fopen_s(&fp, "key.config", "wb");

	//auto size = inputTable_.size();
	//fwrite(&size,sizeof(size),1,fp);

	//for (const auto& record: inputTable_)
	//{
	//	byte strsize = record.first.size();
	//	fwrite(&strsize, sizeof(size), 1, fp);//コマンド文字列数
	//	fwrite(record.first.data(), sizeof(char), strsize, fp);//文字列
	//	//入力の数を書きこむ
	//	byte inputSize = record.second.size();
	//	fwrite(&inputSize, sizeof(inputSize), 1, fp);//入力数
	//	for (const auto& inputInfo : record.second)
	//	{
	//		fwrite(&inputInfo, sizeof(inputInfo), 1, fp);
	//	}
	//}
	//fclose(fp);
}

void Input::LoadInputTable()
{

	//auto handle=FileRead_open("key.config");
	//if (handle == 0)
	//{
	//	return;
	//}
	//int size;
	//FileRead_read(&size, sizeof(size),handle);
	//inputTable_.clear();
	//for (int i =0 ; i<size;i++)
	//{
	//	std::string keyname;
	//	byte strsize = 0;
	//	FileRead_read(&strsize, sizeof(strsize), handle);//コマンド文字列数
	//	keyname.resize(strsize);
	//	FileRead_read(keyname.data(), strsize, handle);//文字数
	//	//入力の数を書きこむ
	//	byte inputSize = 0;
	//	FileRead_read(&inputSize, sizeof(inputSize), handle);//入力数
	//	for (int i = 0; i < size; i++)
	//	{
	//		InputInfo info;
	//		FileRead_read(&info, sizeof(info), handle);
	//		inputTable_[keyname].push_back(info);
	//	}
	//}
	//FileRead_close(handle);

}

bool Input::CheckExclusiveInput()
{
	char keycode[256];
	GetHitKeyStateAll(keycode);//キーボード入力
	int padInfo = GetJoypadInputState(DX_INPUT_PAD1);//パッド１番入力
	int mouseInfo = GetMouseInput();//マウス入力

	for (const auto& exKey: excludeOnCongigList_)
	{
		for (const auto& input : inputTable_[exKey])
		{
			switch (input.type)
			{
			case InputType::keybord:
				if(keycode [input.inputID])
				{
					return true;//無効入力されている
				}
				break;
			case InputType::gamepad:
				if (padInfo& input.inputID)
				{
					return true;//無効入力されている
				}
				break;
			case InputType::mouse:
				if (mouseInfo&input.inputID)
				{
					return true;//無効入力されている
				}
				break;
			default:
				break;
			}
		}
	}

	return false;
}
