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

    _metaDataMap[_colorName]->setRawDefaultValue(QColor(Qt::green).rgb());
    _colorFact.setMetaData(_metaDataMap[_colorName]);
    _colorFact.setRawValue(QColor(Qt::green).rgb());

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
        _available = available;
        emit availableChanged(available);
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

    if (_specifyColor) {
        QColor color = QColor::fromRgb(_colorFact.rawValue().toUInt());

        auto toPVM = [](int value) -> int {
            const int COLOR_MAX = 255;
            const int COLOR_MIN = 0;

            const int PVM_MIN = 0;
            const int PVM_MAX = 20000;

            int converted = PVM_MAX + (value - COLOR_MAX) * (PVM_MAX - PVM_MIN) / (COLOR_MAX - COLOR_MIN);
            return converted;
        };
        qDebug() << "\n set Color" << color.red() << color.green() << color.blue()
                 << toPVM(color.red()) << toPVM(color.green()) << toPVM(color.blue());
        MissionItem* item1 = new MissionItem(seqNum++,
                                            MAV_CMD_DO_SET_SERVO,
                                            MAV_FRAME_MISSION,
                                            7,
                                            toPVM(color.red()),
                                            0, 0, 0, 0, 0,                                                      // param 2-7 not used
                                            true,                                                               // autoContinue
                                            false,                                                              // isCurrentItem
                                            missionItemParent);
        items.append(item1);
        MissionItem* item2 = new MissionItem(seqNum++,
                                            MAV_CMD_DO_SET_SERVO,
                                            MAV_FRAME_MISSION,
                                            8,
                                            toPVM(color.green()),
                                            0, 0, 0, 0, 0,                                                      // param 2-7 not used
                                            true,                                                               // autoContinue
                                            false,                                                              // isCurrentItem
                                            missionItemParent);
        items.append(item2);
        MissionItem* item3 = new MissionItem(seqNum++,
                                            MAV_CMD_DO_SET_SERVO,
                                            MAV_FRAME_MISSION,
                                            9,
                                            toPVM(color.blue()),
                                            0, 0, 0, 0, 0,                                                      // param 2-7 not used
                                            true,                                                               // autoContinue
                                            false,                                                              // isCurrentItem
                                            missionItemParent);
        items.append(item3);
    }
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

    // See LEDSection::appendMissionItems for specs on what consitutes a known speed setting

    if (missionItem.command() == MAV_CMD_DO_SET_SERVO)
    {
        QColor color = QColor::fromRgb(_colorFact.rawValue().toUInt());
        int servo = static_cast<int>(missionItem.param1());
        switch (servo)
        {
            case 7: color.setRed(missionItem.param2());   break;
            case 8: color.setGreen(missionItem.param2()); break;
            case 9: color.setBlue(missionItem.param2());  break;
        }
        visualItems->removeAt(scanIndex)->deleteLater();
        _colorFact.setRawValue(color.rgb());
        setSpecifyColor(true);
        return true;
    }

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
    // We only set the dirty bit if specify color it set. This allows us to change defaults for flight speed
    // without affecting dirty.
    if (_specifyColor) {
        setDirty(true);
    }
}
