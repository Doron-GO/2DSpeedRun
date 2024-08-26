#pragma once
#include<string>
#include<map>
#include<vector>
#include"../../Common/Collision.h"
#include"../../Common/Vector2D.h"
#include"../../Src/Json/json.hpp"

using MapData = std::map <std::string, std::vector <int>>;
using json = nlohmann::json;


class LoadMap
{
public:

	bool LoadTSX(std::string fileName);
	bool LoadStage(std::string fileName);

	bool SetMap();

	const ColList& GetColList(void)const;
	const ColList& WallGetColList(void)const;
	const ColList& WireGetColList(void)const;
	const ColList& itemBoxGetColList(void)const;
	const ColList& BlockGetColList(void)const;
	const PointColList& CheckPointGetColList(void)const;
	const Vector2D& GetWorldArea(void)const;
	const Vector2D& GetTileSize(void)const;
	const  MapData& GetMapData(void)const;
	const std::string& GetMapKey(void)const;
	
private:
	Vector2D layerSize_;
	Vector2D WorldArea_;	//	世界の大きさ
	Vector2D tileSize_;
	ColList colList_;
	ColList WallCollList_;
	ColList WireCollList_;
	ColList itemBoxCollList_;
	PointColList checkPointCollList_;
	ColList blockCollList_;

	std::string mapKey_;
	MapData mapData;//マップにストリングはあまり使わない方がいいenumの方がいい

	json json_;
};

