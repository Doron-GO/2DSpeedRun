#pragma once
#include "Scene.h"
#include<memory>
#include<map>
#include"Transition/TileTransitor.h"


class TitleScene :
    public Scene
{
public:
    TitleScene(SceneMng& manager, int number_of_Players, Transitor& transit);
    ~TitleScene();
    virtual void Update(Input& input) override;
    virtual void Draw() override;

private:

    //効果音タイプ
    enum class SOUND_TYPE
    {
        PUSH,
        CURSOR
    };

    //画像タイプ
    enum class IMG_TYPE
    {
        GO,
        READY
    };

    //効果音管理配列
    std::map<SOUND_TYPE, int> sounds_;
    //画像配列
    std::map<IMG_TYPE, int> img_;

    bool startFlag_;
    int titleImg_;
    int startImg_;
    int selectImg_[4];
    //プレイ人数
    int playModeNum_;
};

