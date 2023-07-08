#include "JobSystem.h"

int main()
{
	// ジョブを登録
	Job job;
	job.SetFunc([](float deletaTime) { printf("call method\n"); });

	// 実行タイミングを指定。
	JobSystem::Get().Get().AddJob(FunctionType::Update, &job);


	while (true)
	{
		JobSystem::Get().Execute(0.01f, FunctionType::PreUpdate);
		JobSystem::Get().Execute(0.01f, FunctionType::Update);
		JobSystem::Get().Execute(0.01f, FunctionType::LateUpdate);
	}
}