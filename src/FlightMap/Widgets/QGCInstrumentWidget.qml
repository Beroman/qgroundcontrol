/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick          2.12
import QtQuick.Layouts  1.12

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.Palette       1.0

ColumnLayout {
    id:         root
    spacing:    ScreenTools.defaultFontPixelHeight / 4

    property real   _innerRadius:           (width - (_topBottomMargin * 3)) / 4
    property real   _outerRadius:           _innerRadius + _topBottomMargin
    property real   _spacing:               ScreenTools.defaultFontPixelHeight * 0.33
    property real   _topBottomMargin:       (width * 0.05) / 2

    QGCPalette { id: qgcPal }

    Rectangle {
        id:                 visualInstrument
        height:             _outerRadius * 2
        Layout.fillWidth:   true
        radius:             _outerRadius
        color:              qgcPal.window

        DeadMouseArea { anchors.fill: parent }

        QGCAttitudeWidget {
            id:                     attitude
            anchors.leftMargin:     _topBottomMargin
            anchors.left:           parent.left
            size:                   _innerRadius * 2
            vehicle:                globals.activeVehicle
            anchors.verticalCenter: parent.verticalCenter
        }

        QGCCompassWidget {
            id:                     compass
            anchors.leftMargin:     _spacing
            anchors.left:           attitude.right
            size:                   _innerRadius * 2
            vehicle:                globals.activeVehicle
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Item {
        id:                 _colorItem
        Layout.fillWidth:   true
        height:             140

        DeadMouseArea { anchors.fill: parent }

        Rectangle {
            anchors.fill:   parent
            color:          qgcPal.window
        }

        Column {
            spacing: ScreenTools.defaultFontPixelWidth
            anchors.centerIn: parent
            QGCSlider {
                id:             redSlider
                width:          200
                stepSize:       1
                minimumValue:   0
                maximumValue:   255
                height: 20
                value: 0
                displayValue: true
                onValueChanged: {
                    activeVehicle.setColor(redSlider.value, greenSlider.value, blueSlider.value)
                }
            }
            QGCSlider {
                id:             greenSlider
                width:          200
                stepSize:       1
                minimumValue:   0
                maximumValue:   255
                height: 20
                value: 0
                displayValue: true
                onValueChanged: {
                    activeVehicle.setColor(redSlider.value, greenSlider.value, blueSlider.value)
                }
            }
            QGCSlider {
                id:             blueSlider
                width:          200
                stepSize:       1
                minimumValue:   0
                maximumValue:   255
                height: 20
                value: 0
                displayValue: true
                onValueChanged: {
                    activeVehicle.setColor(redSlider.value, greenSlider.value, blueSlider.value)
                }
            }
            Rectangle {
                id: rect
                color: Qt.rgba(redSlider.value/255, greenSlider.value/255, blueSlider.value/255, 1)
                width: 30
                height: 30
                border.width: 1
                border.color: "white"
            }
        }
    }

    TerrainProgress {
        Layout.fillWidth: true
    }
}
