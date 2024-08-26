#pragma once
#include<string>
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_utils.hpp"

class GetAtr {

public:

	bool operator()(rapidxml::xml_node<>* node, std::string atrName, float& num)
	{
		if (node == nullptr)
		{
			return false;
		}
		auto atrstr = node->first_attribute(atrName.c_str());
		if (atrstr == nullptr)
		{
			//TRACE("アトリビュートの取得エラー:%s\n", atrName.c_str());
			return false;
		}
		num = static_cast<float>( atof(atrstr->value()) );
		return true;

	}
	bool operator()(rapidxml::xml_node<>* node, std::string atrName, int& num)
	{
		if (node == nullptr)
		{
			return false;
		}
		auto atrstr = node->first_attribute(atrName.c_str());
		if (atrstr == nullptr)
		{
			//TRACE("アトリビュートの取得エラー:%s\n",atrName.c_str());
			return false;
		}
		num = atoi(atrstr->value());
		return true;

	}

	bool operator()(rapidxml::xml_node<>* node, std::string atrStr, unsigned int& num)
	{
		if (node == nullptr) {
			return false;
		}
		auto atr = node->first_attribute(atrStr.c_str());
		if (atr == nullptr)
		{
			//TRACE("アトリビュートの取得エラー");
			return false;
		}
		num = atoi(atr->value());
		return true;

	}

	bool operator()(rapidxml::xml_node<>* node, std::string atrName, std::string& str)
	{
		if (node == nullptr) {

			return false;
		}
		auto atr = node->first_attribute(atrName.c_str());
		if (atr == nullptr)
		{
			//TRACE("アトリビュートの取得エラー");
			return false;
		}
		str = atr->value();
		return true;

	}

};
