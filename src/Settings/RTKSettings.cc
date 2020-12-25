/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "RTKSettings.h"
#include "LinkManager.h"
#include "QGCApplication.h"

#include <QQmlEngine>
#include <QtQml>
#include <QDebug>

DECLARE_SETTINGGROUP(RTK, "RTK")
{
    tcpConnected()->setRawValue(false);
    connect(useTCP(), &SettingsFact::rawValueChanged, this, [this]() {
        if (!useTCP()->rawValue().toBool() && _tcpConnectedFact->rawValue().toBool())
        {
            qgcApp()->toolbox()->linkManager()->connectRTKViaTcp(false);
        }
    });
    qmlRegisterUncreatableType<RTKSettings>("QGroundControl.SettingsManager", 1, 0, "RTKSettings", "Reference only"); \
}

void RTKSettings::onConnectBtnClicked()
{
    qgcApp()->toolbox()->linkManager()->connectRTKViaTcp(!tcpConnected()->rawValue().toBool());
}

DECLARE_SETTINGSFACT(RTKSettings, useTCP)
DECLARE_SETTINGSFACT(RTKSettings, tcpHost)
DECLARE_SETTINGSFACT(RTKSettings, tcpPort)
DECLARE_SETTINGSFACT(RTKSettings, tcpConnected)
DECLARE_SETTINGSFACT(RTKSettings, surveyInAccuracyLimit)
DECLARE_SETTINGSFACT(RTKSettings, surveyInMinObservationDuration)
DECLARE_SETTINGSFACT(RTKSettings, useFixedBasePosition)
DECLARE_SETTINGSFACT(RTKSettings, fixedBasePositionLatitude)
DECLARE_SETTINGSFACT(RTKSettings, fixedBasePositionLongitude)
DECLARE_SETTINGSFACT(RTKSettings, fixedBasePositionAltitude)
DECLARE_SETTINGSFACT(RTKSettings, fixedBasePositionAccuracy)
