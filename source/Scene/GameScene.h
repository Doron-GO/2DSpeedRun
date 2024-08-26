#pragma once
#include<memory>
#include<map>
#include<vector>
#include"../Object/Camera/OutSide.h"
#include"../Object/Camera/Camera.h"
#include"../Object/Stage/Stage.h"
#include"../Object/Player/Player.h"
#include"../Object/Stage/CheckPoint.h"
#include"../Object/Shader/PostEfffect.h"
#include"../Object/Player/PlayerManager.h"
#include"../Input/Input.h"
#include "Scene.h"

class TimeCount;

class GameScene :
    public Scene
{

public:

    GameScene(SceneMng& manager, int number_of_Players, Transitor& transit);
    ~GameScene();

    virtual void Update(Input& input) override;
    virtual void Draw() override;

private:

    //爆発音タイプ
    enum class SOUND_TYPE
    {
        PUSH,
        CURSOR,
        GO,
        READY
    };

    //画像タイプ
    enum class IMG_TYPE
    {
        GO,
        READY
    };

    static constexpr int IMG_SELECT_NUM =2;

    //効果音管理配列
    std::map<SOUND_TYPE, int> sounds_;

    //画像配列
    std::map<IMG_TYPE, int> img_;

    //ステージ
    std::unique_ptr<Stage> stage_;
    //カメラ
    std::unique_ptr<Camera> camera_;
    //死亡範囲
    std::unique_ptr<OutSide> outSide_;
    //チェックポイント
    std::unique_ptr<CheckPoint> checkPoint_;
    //プレイヤーの管理
    std::unique_ptr<PlayerManager>playerManager_;
    //時間計測
    std::unique_ptr<TimeCount>timeCount_;

    void (GameScene::* _update)(Input& input,float elapsedTime);

    //参加プレイヤーの数
    int playerNum_;

    //スタート演出の計測用
    float startTime_;

    //タイトルに戻るかもう一戦するかの選択用画像
    int selectImg_[IMG_SELECT_NUM];

    //タイトルに戻るかもう一戦するかの選択用ナンバー
    int selectModeNum_;

    //効果音の初期化
    void SoundInit(void);

    //画像の初期化
    void ImageInit(void);

    //一番先頭のプレイヤーに追従させる
    //ゲームシーンで先頭を決めて、カメラクラスに渡す
    void DecideOnTheBeginning();

    //クリア後のモード選択(タイトルに戻るや、もう一戦など)
    bool  Select(Input& input);

    //クリア後のモード選択描画(タイトルに戻るや、もう一戦など)
    void SelectDraw(void);

    //複数人対戦モードアップデート
    void MultiPlayUpdate(Input& input, float elapsedTime);

    //一人用モードアップデート
    void SinglePlayUpdate(Input& input, float elapsedTime);

    //一人用モードに変更する
    void SinglePlayModeChange(void);

    //対戦、一人用、関係なく必要な初期化
    void StandardInit(void);

    //スタート演出を表示する
    void ReadyGoDraw(void);

};
