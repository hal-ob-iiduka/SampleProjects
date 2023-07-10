#pragma once
#include <functional>
#include <vector>

class Job
{
public:

	// ジョブの登録用関数
	void SetFunc(std::function<void(float)> func);

	// 前提条件を追加するための関数
	void AddPrerequisite(Job* job);

	// どのスレッドでも動作可能かの設定のための関数
	void SetThreadSafe(bool threadSafe);

	// ジョブキューへの登録を試みる。
	// 前提条件が設定されている場合はキューへ登録されない。
	void TryAddQueue();

	// ジョブの実行用関数
	void Execute(float deletaTime);

private:

	// 前提条件の登録用関数
	void SetupPrerequisite();

	// 前提条件が一つ完了するごとに呼ばれるイベント関数
	void PrerequisiteCompleted();

	// 自分自身をジョブキューへ登録する。
	void AddQueue();

private:

	// ジョブ処理
	std::function<void(float)> m_func;

	// ジョブ終了時イベント
	std::vector<std::function<void()>> m_completeEvents;

	// 前提条件となるJob一覧
	std::vector<Job*> m_prerequisites;

	// 残り完了待ち、前提条件数
	int m_numPrerequisites;

	// trueの場合に別スレッドの実行キューに追加する
	bool m_isThreadSafe;
};