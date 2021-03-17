/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "Section.h"
#include "FactSystem.h"
#include "QmlObjectListModel.h"

class PlanMasterController;

class LEDSection : public Section
{
    Q_OBJECT

public:
    LEDSection(PlanMasterController* masterController, QObject* parent = nullptr);

    Q_PROPERTY(bool     specifyColor  READ specifyColor WRITE setSpecifyColor NOTIFY specifyColorChanged)
    Q_PROPERTY(Fact*    color         READ color                              CONSTANT)

    bool    specifyColor      (void) const { return _specifyColor; }
    Fact*   color             (void) { return &_colorFact; }
    void    setSpecifyColor   (bool specifyColor);

    ///< Signals specifiedColorChanged
    ///< @return The color specified by this item, NaN if not specified
    QColor specifiedColor(void) const;

    // Overrides from Section
    bool available          (void) const override { return _available; }
    bool dirty              (void) const override { return _dirty; }
    void setAvailable       (bool available) override;
    void setDirty           (bool dirty) override;
    bool scanForSection     (QmlObjectListModel* visualItems, int scanIndex) override;
    void appendSectionItems (QList<MissionItem*>& items, QObject* missionItemParent, int& seqNum) override;
    int  itemCount          (void) const override;
    bool settingsSpecified  (void) const override;

signals:
    void specifyColorChanged      (bool specifyColorSpeed);
    void specifiedColorChanged    (QColor colorSpeed);

private slots:
    void _updateSpecifiedColor(void);
    void _colorChanged        (void);

private:
    bool    _available;
    bool    _dirty;
    bool    _specifyColor;
    Fact    _colorFact;

    static QMap<QString, FactMetaData*> _metaDataMap;

    static const char* _colorName;
};
