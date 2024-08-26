#pragma once
#include<vector>
#include<memory>
#include"../../Common/Raycast.h"
#include"../../Common/Collision.h"
#include"../../Common/Vector2D.h"

class LoadMap;
class Player;

class Blocks
{

public:

	// ※コメント必要

	static constexpr int BLOCK_COUNT_MAX= 30;

	struct Block
	{
		bool blockFlag_;
		Collision col_;
		int count_;
	};

	Blocks(LoadMap& loadMap);
	~Blocks();

	void Update();
	void Draw(Vector2DFloat cameraPos);
	std::list<Block> &GetBlockList();

private:
	std::list<Block> blocks_;
	Raycast rayCast_;
	LoadMap& loadMap_;


};

