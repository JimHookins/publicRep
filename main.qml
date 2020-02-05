import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ApplicationWindow
{
    objectName: "MainWindow"
    visible: true
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("Polygons")

    property int margin: 10
    property int fontSize: 16

    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 10
        anchors.margins: margin

        RowLayout
        {
            Label
            {
                //                Layout.fillWidth: true
                text: "Polygon's initial count: " + initialCountSlider.value
                font.pixelSize: fontSize
                padding: 10
            }

            Slider
            {
                Layout.fillWidth: true
                id: initialCountSlider
                objectName: "initialCountSlider"

                from: 1
                to: 100
                value: 3

                stepSize: 1
                snapMode: RangeSlider.SnapOnRelease

                signal initialCountChanged(int value)

                onValueChanged:
                {
                    console.log("initial count changed to " + value)
                    initialCountChanged(value)
                }
            }

            RoundButton
            {
                Layout.fillWidth: true
                text: "Generate"
                font.pixelSize: fontSize
                width: parent.width/3
                radius: width/2
                onClicked: ItemManager.create(initialCountSlider.value)
            }
        }//row1

        RowLayout
        {
            spacing: 10

            Label
            {
                text: "Polygon's sides range: (" + Math.round(rangeSlider.first.value) + ","
                      + Math.round(rangeSlider.second.value) + ")"
                font.pixelSize: fontSize
                padding: 10
            }

            RangeSlider
            {
                id: rangeSlider
                objectName: "rangeSlider"

                Layout.fillWidth: true

                from: 3.0
                to: 10.0

                first.value: ItemManager.getSidesNumFrom()
                second.value: ItemManager.getSidesNumTo()

                stepSize: 1.0
                snapMode: RangeSlider.SnapAlways

                signal edgesChanged(int value, int value2)

                first.onValueChanged:
                {
                    console.log("Sides first value changed to " + first.value)
                    edgesChanged(Math.round(first.value), Math.round(second.value))
                }
                second.onValueChanged:
                {
                    console.log("Sides second value changed to " + second.value)
                    edgesChanged(Math.round(first.value), Math.round(second.value))
                }
            }

            Label
            {
                text: "Polygon's lifespan: " + lifeSpanSlider.value
                font.pixelSize: fontSize
                padding: 10
            }

            Slider
            {
                id: lifeSpanSlider
                objectName: "lifeSpanSlider"

                Layout.fillWidth: true

                from: 1000
                to: 10000

                stepSize: 100
                snapMode: RangeSlider.SnapOnRelease

                signal lifeSpanChanged(int value)

                onValueChanged:
                {
                    console.log("Lifespan changed to " + value)
                    lifeSpanChanged(value)
                }
            }
        }//row2

        Scene
        {
            id: paintField
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

    }
}
