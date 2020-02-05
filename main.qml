import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import com.myinc.MyItem 1.0
import Model 1.0
import "JSScripts.js" as JSScripts

ApplicationWindow
{
    objectName: "MainWindow"
    visible: true
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("Polygons")

    property int margin: 10
    property int fontSize: 16
    property int itemSize: 100


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

        //Flickable?
        Frame {
            id: paintField
            Layout.fillWidth: true
            Layout.fillHeight: true

            Repeater
            {
                id: repeater
                objectName: "repeater"

                implicitWidth: 250
                implicitHeight: 250
                anchors.fill: parent

                model: Model
                {
                    manager: ItemManager
                }

                MyItem
                {
                    id: myItem
                    edgeNum: model.edges
                    color: model.color

                    width: itemSize
                    height: itemSize

                    radius: width/2

                    smooth: true

                    x: model.x * (paintField.width - width)
                    y: model.y * (paintField.height - height)

                    //                    x: JSScripts.getRandomArbitary(width, parent.width - width)
                    //                    y: JSScripts.getRandomArbitary(height, parent.height - height)



                    ColorAnimation on color
                    {
                        target: myItem
                        to: Qt.rgba(0.5, 0.5, 0.8, 1)
                        duration: 500
                        running: true
                        loops: Animation.Infinite
                    }

                    RotationAnimation on rotation
                    {
                        from: 0
                        to: 360
                        duration: 5000
                        loops: Animation.Infinite
                        easing.type: Easing.InOutSine
                    }

                    SequentialAnimation on width
                    {
                        loops: Animation.Infinite
                        PropertyAnimation
                        {
                            to: itemSize*0.9
                            duration: 500
                        }
                        PropertyAnimation
                        {
                            to: itemSize
                            duration: 500
                        }
                    }

                    states: [
                        State {
                            name: "state1"
                            PropertyChanges {target: myItem; width: itemSize; height: itemSize}
                        },
                        State {
                            name: "state2"
                            PropertyChanges {target: myItem; width: itemSize*0.1; height: itemSize*0.1}
                        }

                    ]
                    transitions: [
                    Transition{
                        from: "state1";
                        to: "state2"
                        PropertyAnimation{
                                target: myItem
                                properties: "width, height"
                                easing.type: Easing.InCirc
                                duration: 500
                            }
                    }
                    ]

                    signal clicked(var i)

                    state: "state1"

                    MouseArea
                    {
                        anchors.fill: parent

                        onClicked:
                        {
                            myItem.state = "state2"
                            ItemManager.itemClicked(index)
                        }
                    }
                }//my item
            }//repeater
        }
    }
}


//transitions
//states: [
//    State {
//        name: "state1"
//        PropertyChanges {target: myItem; x: myItem.x; y: myItem.y}
//    },
//    State {
//        name: "state2"
//        PropertyChanges {target: myItem; x: myItem.x + margin; y: myItem.y + margin}
//    }

//]
//transitions: [
//Transition{
//    from: "state1";
//    to: "state2"
//    PropertyAnimation{
//            target: myItem
//            properties: "x,y"
//            easing.type: Easing.InCirc
//            duration: 1000
//        }
//},
//    Transition {
//        from: "state2"
//        to: "state1"
//        PropertyAnimation{
//                target: myItem
//                properties: "x,y"
//                easing.type: Easing.InCirc
//                duration: 1000
//            }
//    }
//]
//end of transitions
