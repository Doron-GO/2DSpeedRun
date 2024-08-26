#include"../../Object/Item/ItemBase.h"
#include"../../Object/Stage/Blocks.h"
#include "PlayerManager.h"
#include "Player.h"

#pragma region 定数宣言


const std::string IMG_WIN_PASS = "Src/Img/UIimage/Win.png";
const std::string IMG_RESTART_PASS = "Src/Img/UIimage/Restart.png";

//勝負あり画像パラメータ
//座標
const float WIN_IMAGE_POS_X = 800.0f;
const float WIN_IMAGE_POS_Y = 300.0f;
//中心の座標
const float WIN_IMAGE_CENTER_X = 200.0f;
const float WIN_IMAGE_CENTER_Y = 20.0f;
//大きさ
const float WIN_IMAGE_SCALE = 2.0;

//先頭プレイヤーの番号
constexpr int FIRST_PLAYER_NUM = 0;

#pragma endregion

PlayerManager::PlayerManager(bool& conclusion, Blocks& blocks):conclusion_(conclusion),blocks_(blocks)
{
	singlePlay_ = false;
	winImg_=LoadGraph(IMG_WIN_PASS.c_str());
	restertImg_ =LoadGraph(IMG_RESTART_PASS.c_str());
	goalFlag_ = false;
	newFirstPlayerNum_ = PLAYER_NUM::P_1;
	oldFirstPlayerNum_ = PLAYER_NUM::P_1;
	winner_ = 0;
}

PlayerManager::~PlayerManager()
{
	DeleteGraph(winImg_);
	DeleteGraph(restertImg_);
	
}

void PlayerManager::Init(int playerNum, ColList gruound, ColList Wall, ColList wire)
{
	for (int Num = 1; Num <= playerNum; Num++)
	{	
		std::shared_ptr<Player> player;
		player = std::make_shared<Player>(Num, blocks_);
		player->Init(gruound, Wall, wire);
		players_.push_back(player);
	}
	goalFlag_ = false;
}

void PlayerManager::Update(Input& input)
{

	//シングルプレイモードでなければ
	if (!singlePlay_)
	{
		//プレイヤーから一番近くの他プレイヤーを検索する
		SearchHormingPlayer();
		//アイテムとの当たり判定
		CollisionItem(); 
	}	
	//ゲームセット判定
	BattleConclusion();
	for (const auto& player : players_)
	{
		//プレイヤーが生きていたらアップデートをする
		if (player->IsAlive())
		{
			player->Update(input);
		}
	}
	
}

void PlayerManager::Draw(Vector2DFloat cameraPos)
{
	for (const auto& player : players_)
	{
		//プレイヤーが生きていたらプレイヤーを描画
		if (player->IsAlive())
		{
			player->Draw(cameraPos);
		}
	}
	//勝敗が決まったら勝負あり画像を表示
	if (conclusion_)
	{
		DrawRotaGraph2F(WIN_IMAGE_POS_X, WIN_IMAGE_POS_Y, WIN_IMAGE_CENTER_X, WIN_IMAGE_CENTER_Y, WIN_IMAGE_SCALE, 0.0, winImg_,true);
	}
}

const Vector2DFloat& PlayerManager::GetPlayerPos(int playerNum)
{
	return players_[playerNum]->GetPlayerPos();
}

const Players PlayerManager::GetPlayers()
{
	return players_;
}

void PlayerManager::SearchFirstPlayer(std::pair<bool, Vector2DFloat>checkPoint)
{
	//プレイヤーとチェックポイント間の距離
	std::vector< std::pair<float,int >> checkPointToPlayerDistance;

	//プレイヤーとチェックポイントとの距離を格納している。
	for (auto& p : players_)
	{
		//生きてなければ戻る
		if (!p->IsAlive())
		{
			continue;
		}
		//チェックポイントの座標を初期化
		Vector2DFloat checkPointPos= {0.0f, 0.0f};

		//プレイヤーのパッドナンバーを取得
		auto padNum = p->GetPadNum()-1;

		//firstはプレイヤーからチェックポイント座標まで距離
		//srcondはパッドナンバー
		std::pair<float,int > info2;

		//セカンドにパッドナンバーを格納
		info2.second = padNum;

		//firstがtrueならチェックポイントY座標に一番近いプレイヤーを探す
		if (checkPoint.first)
		{
			checkPointPos = { players_[padNum]->GetPos().x,checkPoint.second.y };
		}
		//firstがfalseならチェックポイントX座標に一番近いプレイヤーを探す
		else
		{
			checkPointPos = {checkPoint.second.x, players_[padNum]->GetPos().y};
		}
		//プレイヤーからチェックポイント座標まで距離を格納
		info2.first = players_[padNum]->GetPos().distance(checkPointPos);
		//配列に格納
		checkPointToPlayerDistance.push_back(info2);
	}
	//近い順にソートする
	std::sort(checkPointToPlayerDistance.begin(), checkPointToPlayerDistance.end());
	//一番近いプレイヤーの番号を格納
	newFirstPlayerNum_ = static_cast<PLAYER_NUM>(checkPointToPlayerDistance[FIRST_PLAYER_NUM].second);
}

void PlayerManager::SearchHormingPlayer()
{
	//プレイヤー(近いほかプレイヤーを探す側)
	for (auto& player1 : players_)
	{
		//プレイヤーと他プレイヤー間の距離とパッドナンバーを格納する配列
		std::vector< std::pair<float, int>> playerToPlayerDistance;
		
		//レイヤーと他プレイヤー間の距離とパッドナンバー情報
		std::pair<float,int > info;

		//プレイヤーが生きていなければ戻る
		if (!player1->IsAlive())
		{
			continue;
		}
		//検索対象プレイヤーのパッドナンバー
		auto padNum1 = player1->GetPadNum()-1;

		//検索対象プレイヤー
		for (auto& player2 : players_)
		{
			//検索対象プレイヤーのパッドナンバー
			auto padNum2 = player2->GetPadNum()-1;

			//パッドナンバーが一緒なら、同じプレイヤー間の距離になってしまうので戻す
			if (padNum1 == padNum2)
			{ 
				continue; 
			}
			//検索対象プレイヤーのパッドナンバーを格納
			info.second= padNum2;

			//プレイヤーと検索対象プレイヤー間の距離を格納
			info.first = player1->GetPos().distance(player2->GetPos());

			//プレイヤーと他プレイヤー間の距離とパッドナンバー情報を配列に格納
			playerToPlayerDistance.push_back(info);
		}
		//プレイヤーとの距離が近い順にソートする
		std::sort(playerToPlayerDistance.begin(), playerToPlayerDistance.end());
		//一番近いプレイヤーが決定
		int playerNum = playerToPlayerDistance[FIRST_PLAYER_NUM].second;
		//一番近いプレイヤーの座標を格納
		player1->SetTarget(players_[playerNum]->GetPos());
	}	 
}

void PlayerManager::BattleConclusion()
{
	if (!conclusion_ && !goalFlag_)
	{
		return;
	}
	for (auto& player : players_)
	{
		//プレイヤーのパッドナンバー
		auto padNum = player->GetPadNum()-1;

		//プレイヤーが生きていなければ戻る
		if (!player->IsAlive())
		{
			continue;
		}
		//生きていたらそのプレイヤーが勝者
		else
		{
			winner_ = padNum;
		}
	}
	//(勝敗が決まった時に、一回だけこの関数を呼ぶ)
	if (!players_[winner_ ]->IsWin())
	{
		//プレイヤーを勝利状態にする
		players_[winner_]->Conclusion();
	}
	
}

void PlayerManager::SetGoalSingleMode()
{
	goalFlag_ = true;
}

void PlayerManager::CollisionItem()
{
	//アイテム使用側プレイヤー
	for (auto& player1 :players_)
	{
		//アイテム使用状態でなければ抜ける
		if (!(player1->GetItem()->IsActivate()))
		{ 
			continue;
		}
		//アイテム使用側プレイヤーのパッドナンバー
		auto& padNum1 = player1->GetPadNum();

		//アイテムがあてられる側プレイヤー
		for (auto& player2 : players_)
		{
			//アイテムがあてられる側プレイヤーのパッドナンバー
			auto& padNum2 = player2->GetPadNum();

			//パッドナンバーが一緒なら、同じプレイヤーなので戻す
			if (padNum1 == padNum2)
			{ 
				continue; 
			}
			//プレイヤーのアイテム情報
			auto item = player1->GetItem();
			//アイテムの矩形情報
			auto iCol = item->col_;
			//プレイヤーの矩形情報
			auto pCol = player2->GetCol_Rect();

			//プレイヤーとアイテムの矩形が重なっていたら当たっている
			if (IsRectCollision(pCol.min_, pCol.max_, iCol.min_, iCol.max_))
			{
				//アイテムを非アクティブ状態にする
				item->End();
				//そのアイテム接触時の、ダメージリアクションを行う
				player2->Damage(item->type_);
 			}
		}
	}
}

const PlayerManager:: PLAYER_NUM PlayerManager::GetOldFirstPlayerNum()
{
	return oldFirstPlayerNum_;
}

const PlayerManager:: PLAYER_NUM PlayerManager::GetNewFirstPlayerNum()
{
	return newFirstPlayerNum_;
}

void PlayerManager::UpdateFirstPlayerNum()
{
	oldFirstPlayerNum_ = newFirstPlayerNum_;
}

bool PlayerManager::IsRectCollision(Vec pMin, Vec pMax, Vec iMin, Vec iMax)
{
	if (IsCollisionRightSide(pMax, iMin)&& IsCollisionLeftSide(iMax, pMin)&&
		IsCollisionTopSide(pMax, iMin)&& IsCollisionDownSide(iMax, pMin))
	{	
		return true;
	}
	return false;
}

bool PlayerManager::IsCollisionTopSide(Vec Max, Vec Min)
{
	return (Max.y<=Min.y);
}

bool PlayerManager::IsCollisionDownSide(Vec Max, Vec Min)
{
	return (Max.y <= Min.y);
}

bool PlayerManager::IsCollisionLeftSide(Vec Max, Vec Min)
{
	return (Min.x <= Max.x);
}
 
bool PlayerManager::IsCollisionRightSide(Vec Max, Vec Min)
{
	return (Min.x <= Max.x);
}

void PlayerManager::SetSinglePlayMode()
{
	singlePlay_ = true;
}

