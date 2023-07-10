#include "JobSystem.h"

int main()
{
	Job job1;
	Job job2;

	// ジョブを登録
	job1.SetFunc([](float deletaTime) { printf("call Job1\n"); });
	job2.SetFunc([](float deletaTime) { printf("call Job2\n"); });

	// job2 の前に job1 が処理されるようにする。
	job2.AddPrerequisite(&job1);

	// 実行タイミングを指定。
	// ここで job2 を先に登録して前提条件が正常に動作しているか確認している。
	JobSystem::Get().AddJob(FunctionType::Update, &job2);
	JobSystem::Get().AddJob(FunctionType::Update, &job1);

	//while (true)
	{
		JobSystem::Get().Execute(0.01f, FunctionType::PreUpdate);
		JobSystem::Get().Execute(0.01f, FunctionType::Update);
		JobSystem::Get().Execute(0.01f, FunctionType::LateUpdate);
	}
}