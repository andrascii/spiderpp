#pragma once

//https://developercommunity.visualstudio.com/content/problem/185399/error-c2760-in-combaseapih-with-windows-sdk-81-and.html
struct IUnknown;

#define _CRT_SECURE_NO_WARNINGS

//
// C/C++
//
#include <vector>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <atomic>
#include <memory>
#include <utility>
#include <cassert>
#include <functional>
#include <optional>
#include <sstream>
#include <algorithm>

#pragma warning(disable:4127)
#pragma warning(disable:745)
#pragma warning(disable:749)
#pragma warning(disable:333)
#pragma warning(disable:317)
#pragma warning(disable:597)
#pragma warning(disable:4251)
#pragma warning(disable:4702)
#pragma warning(disable:4456)

//
// Qt
//
#include <QCoreApplication>
#include <QObject>
#include <QThread>
#include <QTextCodec>
#include <QAbstractListModel>
#include <QMetaObject>
#include <QMetaMethod>
#include <QMetaType>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMutex>
#include <QMutexLocker>
#include <QReadWriteLock>
#include <QSemaphore>
#include <QWaitCondition>
#include <QQueue>
#include <QDebug>
#include <QProcess>
#include <QDateTime>
#include <QItemSelectionModel>
#include <QBuffer>
#include <QStack>
#include <QNetworkInterface>
#include <QLibrary>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include "common_macro_helpers.h"

using namespace std::chrono_literals;
using std::size_t;

#ifndef QT_DEBUG
#define PRODUCTION
#endif