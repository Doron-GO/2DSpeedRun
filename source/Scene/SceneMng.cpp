#include "SceneMng.h"
#include"Scene.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"

void SceneMng::ChangeScene(std::shared_ptr<Scene> scene)
{
	if (scenes_.empty())
	{
		scenes_.push_front(scene);
	}
	else
	{
		scenes_.front() = (scene);
	}
}

void SceneMng::PopScene()
{
	if (scenes_.size() > 1)
	{
		scenes_.pop_front();
	}

}

void SceneMng::PushScene(std::shared_ptr<Scene>scene)
{
	scenes_.push_front(scene);

}

void SceneMng::Update(Input& input)
{
	_dbgStartDraw();

	if (!scenes_.empty())
	{
		scenes_.front()->Update(input);
	}


}

void SceneMng::Draw()
{
	auto rit = scenes_.rbegin();
	for (; rit != scenes_.rend(); rit++)
	{
		(*rit)->Draw();
	}
}
