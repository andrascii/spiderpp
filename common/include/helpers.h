#pragma once

namespace Common
{

class Helpers
{
public:
	using ParsedSerialNumberData = std::tuple<QByteArray, QByteArray, QDate>;

	static std::vector<QMetaMethod> allUserSignals(QObject* qobject, QVector<QByteArray> exclude = QVector<QByteArray>());

	static void connectMetaMethods(QObject* sender, QMetaMethod senderMethod, QObject* receiver, QMetaMethod receiverMethod);
	static void connectSignalsToMetaMethod(QObject* sender, std::vector<QMetaMethod> senderMethods, QObject* receiver, QMetaMethod receiverMethod);
	static void connectSignalsToMetaMethods(QObject* sender, std::vector<QMetaMethod> senderMethods, QObject* receiver, std::vector<QMetaMethod> receiverMethods);
	static QMetaMethod metaMethod(QObject* qobject, const char* methodSignature);
	static QMetaMethod metaMethodOfSlot(QObject* qobject, const char* slotSignature);
	static QMetaMethod metaMethodOfSignal(QObject* qobject, const char* signalSignature);

	static void connectSignalsToMetaMethodsWithTheSameName(
		QObject* sender,
		std::vector<QMetaMethod> senderMethods,
		QObject* receiver,
		std::vector<QMetaMethod> receiverMethods
	);

	template <
	    typename DestinationType,
	    typename SourceType,
	    typename = typename std::enable_if<std::is_pointer<DestinationType>::value>::type>
	static DestinationType fast_cast(SourceType* p)
	{
		DEBUG_ASSERT(dynamic_cast<DestinationType>(p));
		return static_cast<DestinationType>(p);
	}

	template <
	    typename DestinationType,
	    typename SourceType,
	    typename = typename std::enable_if<std::is_reference<DestinationType>::value>::type>
	static DestinationType fast_cast(SourceType&& p)
	{
#ifdef DEBUG
		try
		{
			dynamic_cast<DestinationType>(p);
		}
		catch (...)
		{
			DEBUG_ASSERT(!"Actual type differs from DestinationType");
		}
#endif
		return static_cast<DestinationType>(p);
	}
};

namespace MetaHelpers
{

struct CastHelper
{
	std::size_t dummy;

	template <typename T>
	constexpr operator T const&() const noexcept;

	template <typename T>
	constexpr operator T&() const noexcept;

	template <typename T>
	constexpr operator T&& () const noexcept;
};

template <typename T, std::size_t... Idx>
constexpr auto sfinaeConstructibleHelper(std::index_sequence<Idx...>) -> decltype(T{ CastHelper{ Idx }... }, std::size_t());

template <typename T, std::size_t N>
constexpr auto fieldsCountHelper(long) -> decltype(sfinaeConstructibleHelper<T>(std::make_index_sequence<N>()))
{
	return N;
}

template <typename T, std::size_t N>
constexpr size_t fieldsCountHelper(int)
{
	return fieldsCountHelper<T, N - 1>(0L);
}

}

template <typename T>
constexpr std::size_t fieldsCount()
{
	return MetaHelpers::fieldsCountHelper<T, sizeof(T)>(0L);
}

}