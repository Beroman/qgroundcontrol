/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "RTKSettings.h"
#include "QGCApplication.h"

#include <QQmlEngine>
#include <QtQml>
#include <QDebug>

DECLARE_SETTINGGROUP(RTK, "RTK")
{
    qmlRegisterUncreatableType<RTKSettings>("QGroundControl.SettingsManager", 1, 0, "RTKSettings", "Reference only"); \
}

void RTKSettings::testUDPConnection()
{
    qDebug() << "\n qgcApp()->toolbox()->linkManager()->testRTKUDPConnection();";
    qgcApp()->toolbox()->linkManager()->testRTKUDPConnection();
}

DECLARE_SETTINGSFACT(RTKSettings, useUDP)
DECLARE_SETTINGSFACT(RTKSettings, udpPort)
DECLARE_SETTINGSFACT(RTKSettings, surveyInAccuracyLimit)
DECLARE_SETTINGSFACT(RTKSettings, surveyInMinObservationDuration)
DECLARE_SETTINGSFACT(RTKSettings, useFixedBasePosition)
DECLARE_SETTINGSFACT(RTKSettings, fixedBasePositionLatitude)
DECLARE_SETTINGSFACT(RTKSettings, fixedBasePositionLongitude)
DECLARE_SETTINGSFACT(RTKSettings, fixedBasePositionAltitude)
DECLARE_SETTINGSFACT(RTKSettings, fixedBasePositionAccuracy)
