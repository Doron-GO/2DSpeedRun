#include<sstream>
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_utils.hpp"
#include "../Common/GetAtr.h"
#include "../Common/Vector2D.h"
#include "ImageMng.h"
#include "AnimMng.h"

std::string AnimeMng::LoadAnime(std::string fileName)
{
    rapidxml::xml_document <> actDoc;
    rapidxml::file<> xmlFile = fileName.c_str();
    actDoc.parse<0>(xmlFile.data());
    auto actionLiat = actDoc.first_node("actionList");
    auto getAtr = GetAtr();
    std::string CharName;
    if (actionLiat == nullptr)
    {
        return std::string();
        //TRACE("actionLiat取得エラー\n");
    }
    getAtr(actionLiat, "name", CharName);

    std::string source;
    std::string KeyName;
    Vector2D divSize;
    Vector2D divCnt;

    //ぜんぶのactionNodeがとり終わるまで回るループ
    for (auto actionNode = actionLiat->first_node("action");
        actionNode != nullptr;
        actionNode = actionNode->next_sibling()) {

        getAtr(actionNode, "source", source);
        getAtr(actionNode, "name", KeyName);
        getAtr(actionNode, "divSizeX", divSize.x);
        getAtr(actionNode, "divSizeY", divSize.y);
        getAtr(actionNode, "divCntX", divCnt.x);
        getAtr(actionNode, "divCntY", divCnt.y);
        ImageMng::GetInstsnce().GetID(source, CharName + KeyName, divSize, divCnt);

        //↓ここのキーの名前をかぶりやすいのにしないこと
        auto result = animeTbl_.try_emplace(CharName + KeyName);

        if (!result.second)
        {
            return CharName;
        }
        auto animeNode = actionNode->first_node("anime");
        if (animeNode == nullptr)
        {
           // TRACE("animeNode取得エラー\n");
            return std::string();
        }
        int loop;
        getAtr(animeNode, "loop", loop);
        //アニメーションのループ回数を入れてる
        animeTbl_[CharName + KeyName].first = loop;
        auto animeData = animeNode->first_node();

        std::stringstream dataStream;
        dataStream << animeData->value();
        std::string numStr;

        while (getline(dataStream, numStr, ','))
        {
            auto id = atoi(numStr.c_str());
            getline(dataStream, numStr, ',');
            auto freme = atoi(numStr.c_str());
            int csv[4];

            for (int j = 0; j < 4; j++)
            {
                getline(dataStream, numStr, ',');
                csv[j] = atoi(numStr.c_str());
            }
            animeTbl_[CharName + KeyName].second.emplace_back(AnimeData{ id,freme,csv[1],csv[1],csv[2],csv[3] });
        }

    }
    return CharName;
}

bool AnimeMng::UpdateAnime(AnimStr& animCacha)
{
    //再生が終わりまで行ったらbiginに戻す
    //キャラクターのアニメ経過フレームが最大まで行ったら
    if (animCacha.animCount_++ >= (*animCacha.animID_)[InvLimit])
    {
        //経過フレームを０に戻す
        animCacha.animCount_ = 0;
        //アニメーションの再生位置を次にする
          // ++animCache.animID_;

           //アニメーションの再生位置が最後まで行ったら
        if (++animCacha.animID_ == animeTbl_[animCacha.objID_ + animCacha.state_].second.end())
        {
            //アニメーションの現ループ数を増やす
             // animCache.animPlayCount_++;

            if (++animCacha.animPlayCount_ == animeTbl_[animCacha.objID_ + animCacha.state_].first || animeTbl_[animCacha.objID_ + animCacha.state_].first == -1) {


                animCacha.animID_ = animeTbl_[animCacha.objID_ + animCacha.state_].second.begin();

            }
            else
            {
                animCacha.animID_ = --(animeTbl_[animCacha.objID_ + animCacha.state_].second.end());
                animCacha.endFlag = true;
            }
        }
    }
    return false;
}

void AnimeMng::SetAnime(AnimStr& animDate, std::string state)
{
    //状態チェック
    if (state == animDate.state_)
    {
        return;
    }

    animDate = AnimStr{
        animDate.objID_ + state,
        animDate.objID_,
        state,                            //↓セカンドはリスト
        animeTbl_[animDate.objID_ + state].second.begin(),
        0,
        0,
        false
    };

}

AnimeMng::AnimeMng()
{
}

AnimeMng::~AnimeMng()
{
}
