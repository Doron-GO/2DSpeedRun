#include<DxLib.h>
#include "ImageMng.h"

const std::vector<int>& ImageMng::GetID(std::string key)
{
	if (ImageMap_.count(key) == 0)
	{
		GetID(key, key);
	}
	return ImageMap_[key];
}

const std::vector<int>& ImageMng::GetID(std::string f_name, std::string key)
{
	if (ImageMap_.count(key) == 0)
	{
		ImageMap_[key].resize(1);

		ImageMap_[key][0] = LoadGraph(f_name.c_str());

	}
	return ImageMap_[key];
}

const std::vector<int>& ImageMng::GetID(std::string f_name, std::string key, Vector2D divSize, Vector2D divCut)
{
	if (ImageMap_.count(key) == 0)
	{

		ImageMap_[key].resize(static_cast<UINT64>(divSize.x) * static_cast<UINT64>(divSize.y));
		if (LoadDivGraph(
			f_name.c_str(),
			divCut.x * divCut.y,
			divCut.x, divCut.y,
			divSize.x, divSize.y,
			&ImageMap_[key][0]
			, true) == -1)
		{
			//TRACE("‰æ‘œ‚Ì“Ç‚İ‚İ¸”s:%s\n", f_name.c_str());
		}
	}
	return ImageMap_[key];
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
