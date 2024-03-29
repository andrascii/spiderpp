#include "helpers.h"

namespace Common
{

std::vector<QMetaMethod> Helpers::allUserSignals(QObject* qobject, QVector<QByteArray> exclude)
{
	const QMetaObject* metaObject = qobject->metaObject();
	std::vector<QMetaMethod> signalsCollection;

	for (int i = 0; i < metaObject->methodCount(); ++i)
	{
		QMetaMethod metaMethod = metaObject->method(i);
		QByteArray methodSignature = metaMethod.methodSignature();

		if (metaMethod.methodType() != QMetaMethod::Signal ||
			methodSignature.contains("destroyed") ||
			methodSignature.contains("objectNameChanged") ||
			exclude.contains(methodSignature.data()))
		{
			continue;
		}

		signalsCollection.push_back(metaMethod);
	}

	return signalsCollection;
}

// TODO: this method is unnecessary, remove this and use QObject::connect instead
void Helpers::connectMetaMethods(QObject* sender, QMetaMethod senderMethod, QObject* receiver, QMetaMethod receiverMethod)
{
	ASSERT(senderMethod.methodType() == QMetaMethod::Signal);
	ASSERT(receiverMethod.methodType() == QMetaMethod::Signal || receiverMethod.methodType() == QMetaMethod::Slot);

	QByteArray senderSignalSignature = "2" + senderMethod.methodSignature();
	QByteArray receiverMethodSignature = receiverMethod.methodSignature();

	if (receiverMethod.methodType() == QMetaMethod::Signal)
	{
		receiverMethodSignature = "2" + receiverMethodSignature;
	}
	else
	{
		receiverMethodSignature = "1" + receiverMethodSignature;
	}

	VERIFY(QObject::connect(sender, senderSignalSignature, receiver, receiverMethodSignature));
}

void Helpers::connectSignalsToMetaMethod(QObject* sender, std::vector<QMetaMethod> senderMethods, QObject* receiver, QMetaMethod receiverMethod)
{
	for (const auto& metaMethod : senderMethods)
	{
		connectMetaMethods(sender, metaMethod, receiver, receiverMethod);
	}
}

void Helpers::connectSignalsToMetaMethods(QObject* sender, std::vector<QMetaMethod> senderMethods, QObject* receiver, std::vector<QMetaMethod> receiverMethods)
{
	ASSERT(senderMethods.size() == receiverMethods.size());

	for (std::size_t i = 0, commonSize = senderMethods.size(); i < commonSize; ++i)
	{
		connectMetaMethods(sender, senderMethods[i], receiver, receiverMethods[i]);
	}
}

QMetaMethod Helpers::metaMethod(QObject* qobject, const char* methodSignature)
{
	const QMetaObject* metaObject = qobject->metaObject();
	const int methodIndex = metaObject->indexOfMethod(methodSignature);

	ASSERT(methodIndex != -1 && "Check existence of the specified method");

	return metaObject->method(methodIndex);
}

QMetaMethod Helpers::metaMethodOfSlot(QObject* qobject, const char* slotSignature)
{
	const QMetaObject* metaObject = qobject->metaObject();
	const int slotIndex = metaObject->indexOfSlot(slotSignature);

	ASSERT(slotIndex != -1 && "Check existence of the specified method and that it's actually a slot");

	return metaObject->method(slotIndex);
}

QMetaMethod Helpers::metaMethodOfSignal(QObject* qobject, const char* signalSignature)
{
	const QMetaObject* metaObject = qobject->metaObject();
	const int signalIndex = metaObject->indexOfSignal(signalSignature);

	ASSERT(signalIndex != -1 && "Check existence of the specified method and that it's actually a signal");

	return metaObject->method(signalIndex);
}

void Helpers::connectSignalsToMetaMethodsWithTheSameName(QObject* sender,
	std::vector<QMetaMethod> senderMethods,
	QObject* receiver,
	std::vector<QMetaMethod> receiverMethods)
{
	ASSERT(senderMethods.size() == receiverMethods.size());

	for (std::size_t i = 0, commonSize = senderMethods.size(); i < commonSize; ++i)
	{
		QByteArray methodSignature = senderMethods[i].methodSignature();

		for (std::size_t j = 0; j < commonSize; ++j)
		{
			QByteArray receiverMethodSignature = receiverMethods[j].methodSignature();

			if (methodSignature == receiverMethodSignature)
			{
				connectMetaMethods(sender, senderMethods[i], receiver, receiverMethods[j]);
			}
		}
	}
}

}