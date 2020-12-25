/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QThread>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QMetaType>
#include <QSharedPointer>
#include <QDebug>
#include <QTimer>

#include <memory>

#include "QGCMAVLink.h"
#include "LinkConfiguration.h"
#include "MavlinkMessagesTimer.h"

class LinkManager;

/**
* @brief The link interface defines the interface for all links used to communicate
* with the ground station application.
**/
class LinkInterface : public QThread
{
    Q_OBJECT

    friend class LinkManager;

public:    
    virtual ~LinkInterface();

    Q_PROPERTY(bool isPX4Flow   READ isPX4Flow  CONSTANT)
    Q_PROPERTY(bool isMockLink  READ isMockLink CONSTANT)

    // Property accessors
    bool isPX4Flow(void) const { return _isPX4Flow; }
#ifdef UNITTEST_BUILD
    bool isMockLink(void);
#else
    bool isMockLink(void) { return false; }
#endif

    SharedLinkConfigurationPtr linkConfiguration(void) { return _config; }

    Q_INVOKABLE virtual void    disconnect  (void) = 0;

    virtual bool isConnected    (void) const = 0;
    virtual bool isLogReplay    (void) { return false; }

    uint8_t mavlinkChannel              (void) const;
    bool    decodedFirstMavlinkPacket   (void) const { return _decodedFirstMavlinkPacket; }
    bool    setDecodedFirstMavlinkPacket(bool decodedFirstMavlinkPacket) { return _decodedFirstMavlinkPacket = decodedFirstMavlinkPacket; }
    void    writeBytesThreadSafe        (const char *bytes, int length);
    void    addVehicleReference         (void);
    void    removeVehicleReference      (void);

signals:
    void bytesReceived      (LinkInterface* link, QByteArray data);
    void bytesSent          (LinkInterface* link, QByteArray data);
    void connected          (void);
    void disconnected       (void);
    void communicationError (const QString& title, const QString& error);

protected:
    // Links are only created by LinkManager so constructor is not public
    LinkInterface(SharedLinkConfigurationPtr& config, bool isPX4Flow = false);

    void _connectionRemoved(void);

    SharedLinkConfigurationPtr _config;

private:
    // connect is private since all links should be created through LinkManager::createConnectedLink calls
    virtual bool _connect(void) = 0;

    virtual void _writeBytes(const QByteArray) = 0; // Not thread safe, only writeBytesThreadSafe is thread safe

    void _setMavlinkChannel(uint8_t channel);

    bool    _mavlinkChannelSet          = false;
    uint8_t _mavlinkChannel;
    bool    _decodedFirstMavlinkPacket  = false;
    bool    _isPX4Flow                  = false;
    int     _vehicleReferenceCount      = 0;

    mutable QMutex _writeBytesMutex;

    QMap<int /* vehicle id */, MavlinkMessagesTimer*> _mavlinkMessagesTimers;
};

typedef std::shared_ptr<LinkInterface>  SharedLinkInterfacePtr;
typedef std::weak_ptr<LinkInterface>    WeakLinkInterfacePtr;

