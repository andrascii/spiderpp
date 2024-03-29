#pragma once

namespace spiderpp
{

//
// Deleting objects stored in ThreadManager is in responsibilities of ThreadManager
//

class ThreadManager final
{
public:
	static ThreadManager& instance();
	static void destroy();

	void moveObjectToThread(QObject* object, const QByteArray& threadName);

private:
	ThreadManager() = default;
	~ThreadManager();
	static void deleteAllLater(const QList<QObject*>& objects);

private:
	struct ObjectsThreadAffinity
	{
		QList<QObject*> objects;
		QThread* thread;
	};

	QMap<QByteArray, ObjectsThreadAffinity> m_objectsThreadAffinity;
};

}
