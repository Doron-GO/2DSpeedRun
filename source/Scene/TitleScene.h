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

    //���ʉ��^�C�v
    enum class SOUND_TYPE
    {
        PUSH,
        CURSOR
    };

    //�摜�^�C�v
    enum class IMG_TYPE
    {
        TITLE,
        START,
        SELECT
    };

    static constexpr int SELECT_IMG_MAX =4;

    //���ʉ��Ǘ��z��
    std::map<SOUND_TYPE, int> sounds_;
    //�摜�z��
    std::map<IMG_TYPE, int> img_;

    bool startFlag_;
    int selectImg_[SELECT_IMG_MAX];
    //�v���C�l��
    int playModeNum_;
};

