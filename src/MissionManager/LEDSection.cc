/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "LEDSection.h"
#include "JsonHelper.h"
#include "FirmwarePlugin.h"
#include "SimpleMissionItem.h"
#include "PlanMasterController.h"

const char* LEDSection::_colorName = "LEDColor";

QMap<QString, FactMetaData*> LEDSection::_metaDataMap;

LEDSection::LEDSection(PlanMasterController* masterController, QObject* parent)
    : Section         (masterController, parent)
    , _available      (false)
    , _dirty          (false)
    , _specifyColor   (false)
    , _colorFact      (0, _colorName,   FactMetaData::valueTypeUint32)
{
    if (_metaDataMap.isEmpty()) {
        _metaDataMap = FactMetaData::createMapFromJsonFile(QStringLiteral(":/json/LEDSection.FactMetaData.json"), nullptr /* metaDataParent */);
    }

    quint32 ledColor = 0;
//    if (_masterController->controllerVehicle()->multiRotor()) {
//        flightSpeed = _masterController->controllerVehicle()->defaultHoverSpeed();
//    } else {
//        flightSpeed = _masterController->controllerVehicle()->defaultCruiseSpeed();
//    }

    _metaDataMap[_colorName]->setRawDefaultValue(ledColor);
    _colorFact.setMetaData(_metaDataMap[_colorName]);
    _colorFact.setRawValue(ledColor);

    connect(this,         &LEDSection::specifyColorChanged,   this, &LEDSection::settingsSpecifiedChanged);
    connect(&_colorFact,  &Fact::valueChanged,                this, &LEDSection::_colorChanged);

    connect(this,         &LEDSection::specifyColorChanged,   this, &LEDSection::_updateSpecifiedColor);
    connect(&_colorFact,  &Fact::valueChanged,                this, &LEDSection::_updateSpecifiedColor);
}

bool LEDSection::settingsSpecified(void) const
{
    return _specifyColor;
}

void LEDSection::setAvailable(bool available)
{
    if (available != _available) {
        if (available && (_masterController->controllerVehicle()->multiRotor() || _masterController->controllerVehicle()->fixedWing())) {
            _available = available;
            emit availableChanged(available);
        }
    }
}

void LEDSection::setDirty(bool dirty)
{
    if (_dirty != dirty) {
        _dirty = dirty;
        emit dirtyChanged(_dirty);
    }
}

void LEDSection::setSpecifyColor(bool specifyColor)
{
    if (specifyColor != _specifyColor) {
        _specifyColor = specifyColor;
        emit specifyColorChanged(specifyColor);
        setDirty(true);
        emit itemCountChanged(itemCount());
    }
}

int LEDSection::itemCount(void) const
{
    return _specifyColor ? 1: 0;
}

void LEDSection::appendSectionItems(QList<MissionItem*>& items, QObject* missionItemParent, int& seqNum)
{
    // IMPORTANT NOTE: If anything changes here you must also change LEDSection::scanForSettings

//    if (_specifyColor) {
//        MissionItem* item = new MissionItem(seqNum++,
//                                            MAV_CMD_DO_CHANGE_SPEED,
//                                            MAV_FRAME_MISSION,
//                                            _masterController->controllerVehicle()->multiRotor() ? 1 /* groundspeed */ : 0 /* airspeed */,    // Change airspeed or groundspeed
//                                            _colorFact.rawValue().toDouble(),
//                                            -1,                                                                 // No throttle change
//                                            0,                                                                  // Absolute speed change
//                                            0, 0, 0,                                                            // param 5-7 not used
//                                            true,                                                               // autoContinue
//                                            false,                                                              // isCurrentItem
//                                            missionItemParent);
//        items.append(item);
//    }
}

bool LEDSection::scanForSection(QmlObjectListModel* visualItems, int scanIndex)
{
    if (!_available || scanIndex >= visualItems->count()) {
        return false;
    }

    SimpleMissionItem* item = visualItems->value<SimpleMissionItem*>(scanIndex);
    if (!item) {
        // We hit a complex item, there can't be a speed setting
        return false;
    }
    MissionItem& missionItem = item->missionItem();
    Q_UNUSED(missionItem)

    // See LEDSection::appendMissionItems for specs on what consitutes a known speed setting

//    if (missionItem.command() == MAV_CMD_DO_CHANGE_SPEED && missionItem.param3() == -1 && missionItem.param4() == 0 && missionItem.param5() == 0 && missionItem.param6() == 0 && missionItem.param7() == 0) {
//        if (_masterController->controllerVehicle()->multiRotor() && missionItem.param1() != 1) {
//            return false;
//        } else if (_masterController->controllerVehicle()->fixedWing() && missionItem.param1() != 0) {
//            return false;
//        }
//        visualItems->removeAt(scanIndex)->deleteLater();
//        _colorFact.setRawValue(missionItem.param2());
//        setSpecifyColor(true);
//        return true;
//    }

    return false;
}


QColor LEDSection::specifiedColor(void) const
{
    return _specifyColor ? QColor::fromRgba(_colorFact.rawValue().toUInt()) : Qt::black;
}

void LEDSection::_updateSpecifiedColor(void)
{
    if (_specifyColor) {
        emit specifiedColorChanged(specifiedColor());
    }
}

void LEDSection::_colorChanged(void)
{
    // We only set the dirty bit if specify flight speed it set. This allows us to change defaults for flight speed
    // without affecting dirty.
    if (_specifyColor) {
        setDirty(true);
    }
}
