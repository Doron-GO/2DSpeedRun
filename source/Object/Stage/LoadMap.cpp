#include "LoadMap.h"
#include<DxLib.h>
#include<fstream>
#include <stdlib.h>
#include"../../Manager/ImageMng.h"

bool LoadMap::LoadTSX(std::string fileName)
{
	//jsonファイルを開く

	std::ifstream f(fileName.c_str());
	json_ = json::parse(f);

	std::string source;

	//TSX.jsonに書いている要素を読み込む
	auto width = json_["imagewidth"].get<int>();
	auto height = json_["imageheight"].get<int>();
	auto tilewidth = json_["tilewidth"].get<int>();
	auto tilecount = json_["tilecount"].get<int>();
	auto tileheight = json_["tileheight"].get<int>();
	auto columns = json_["columns"].get<int>();
	source = json_["image"];

	mapKey_ = "mapChip";
	//GetIDで画僧を読み込む(内部でLoadDiveGraph)
	ImageMng::GetInstsnce().GetID(source, mapKey_, { tilewidth, tileheight }, { columns ,tilecount / columns });
	return true;

}

bool LoadMap::LoadStage(std::string fileName)
{
	std::ifstream f(fileName.c_str());
	json_ = json::parse(f);

	WorldArea_.x = json_["width"].get<int>();
	WorldArea_.y = json_["height"].get<int>();
	tileSize_.x = json_["tilewidth"].get<int>();
	tileSize_.y = json_["tileheight"].get<int>();

	SetMap();
	return true;
}

bool LoadMap::SetMap()
{

	auto& layer = json_["layers"][0];
	layerSize_.x = layer["width"].get<int>();
	layerSize_.y = layer["height"].get<int>();

	int firstgid = layer["id"].get<int>();

	std::string layerName = layer["name"];
	auto layerData = mapData.try_emplace(layerName);
	if (layerData.second)
	{
		layerData.first->second.resize(WorldArea_.x * WorldArea_.y);
	}

	//0とか1とかのマップ情報を一個一個格納してる
	auto& data = layer["data"];
	int cnt = 0;
	for (auto& vecData : layerData.first->second)
	{
		vecData = data[cnt].get<int>()-firstgid;
		cnt++;
	}

	//ステージの当たり判定を読み込むラムダ式
	auto LoadCol = [](json json, std::list<Collision> &col)
	{
		for (int cnt = 0; cnt <= json.size() - 1; cnt++)
		{
			float x = float(json[cnt]["x"].get<int>());
			float y = float(json[cnt]["y"].get<int>());
			float w = float(json[cnt]["width"].get<int>());
			float h = float(json[cnt]["height"].get<int>());
			col.push_back(Collision{ Vector2DFloat{x,y},Vector2DFloat{w,h} });
		}
	};
	auto LoadItemCol = [](json json, std::list<Collision> &col)
	{
		for (int cnt = 0; cnt <= json.size() - 1; cnt++)
		{
			float x = float(json[cnt]["x"].get<int>());
			float y = float(json[cnt]["y"].get<int>());
			float w = float(json[cnt]["width"].get<int>());
			float h = float(json[cnt]["height"].get<int>());
			col.push_back(Collision{ Vector2DFloat{x,y+h},Vector2DFloat{x+w,y} });
		}
	};
	auto LoadWireCol = [](json json, std::list<Collision> &col)
	{
		for (int cnt = 0; cnt <= json.size() - 1; cnt++)
		{
			float x = float(json[cnt]["x"].get<int>());
			float y = float(json[cnt]["y"].get<int>());
			float w = float(json[cnt]["width"].get<int>());
			float h = float(json[cnt]["height"].get<int>());
			col.push_back(Collision{ Vector2DFloat{x,y},Vector2DFloat{x+w,y+h} });
		}
	};
	auto LoadCheckCol = [](json json, std::vector<Collision> &col)
	{
		for (int cnt = 0; cnt <= json.size() - 1; cnt++)
		{
			float x = float(json[cnt]["x"].get<int>());
			float y = float(json[cnt]["y"].get<int>());
			float w = float(json[cnt]["width"].get<int>());
			float h = float(json[cnt]["height"].get<int>());
			col.push_back(Collision{ Vector2DFloat{x,y+h},Vector2DFloat{x+w,y} });
		}
	};
	auto LoadBlockCol = [](json json, std::list<Collision> &col)
	{
		for (int cnt = 0; cnt <= json.size() - 1; cnt++)
		{
			float x = float(json[cnt]["x"].get<int>());
			float y = float(json[cnt]["y"].get<int>());
			float w = float(json[cnt]["width"].get<int>());
			float h = float(json[cnt]["height"].get<int>());
			col.push_back(Collision{ Vector2DFloat{x,y+h},Vector2DFloat{x+w,y} });
		}
	};
	auto & col = json_["layers"][1]["objects"];
	auto& wallCol = json_["layers"][2]["objects"];
	auto& wireCol = json_["layers"][3]["objects"];
	auto& itemBoxCol = json_["layers"][4]["objects"];
	auto& checkPointCol = json_["layers"][5]["objects"];
	auto& checkBlockCol = json_["layers"][6]["objects"];

	LoadCol(col, colList_);
	LoadCol(wallCol, WallCollList_);
	LoadWireCol(wireCol, WireCollList_);
	LoadItemCol(itemBoxCol, itemBoxCollList_);
	LoadCheckCol(checkPointCol, checkPointCollList_);
	LoadBlockCol(checkBlockCol, blockCollList_);

	return true;
}

const ColList& LoadMap::GetColList(void)const
{
	return colList_;
}

const ColList& LoadMap::WallGetColList(void)const
{
	return WallCollList_;
}

const ColList& LoadMap::WireGetColList(void) const
{
	return WireCollList_;
}

const ColList& LoadMap::itemBoxGetColList(void)const
{
	return itemBoxCollList_;
}

const ColList& LoadMap::BlockGetColList(void)const
{
	return blockCollList_;
}

const PointColList& LoadMap::CheckPointGetColList(void)const
{
	return checkPointCollList_;
}

const Vector2D& LoadMap::GetWorldArea(void)const
{
	return  WorldArea_;
}

const Vector2D& LoadMap::GetTileSize(void)const
{
	return  tileSize_;
}

const MapData& LoadMap::GetMapData(void)const
{
	return  mapData;
}

const std::string& LoadMap::GetMapKey(void)const
{
	return  mapKey_;
}
