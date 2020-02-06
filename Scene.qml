import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import com.myinc.MyItem 1.0
import Model 1.0

Frame
{
    property int itemSize: 100

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

        MyItem//my item, developed on C++
        {
            id: myItem
            edgeNum: model.edges
            color: model.color
            property string itemColor: model.color
            property string itemColor2: model.color2

            width: itemSize
            height: itemSize

            radius: width/2

            smooth: true

            x: model.x * (paintField.width - width)
            y: model.y * (paintField.height - height)

            signal clicked(var i)

            state: "initial"

            MouseArea
            {
                anchors.fill: parent

                onClicked:
                {
                    console.log("Mouse: " + mouseX + ", " + mouseY)
                    if (myItem.insideItem(mouseX, mouseY))
                        myItem.state = "dissapearing"
                }
            }

            //dissapearing animation timer
            property bool byTimer: false
            Timer
            {
                interval: lifeSpanSlider.value - transitionAnimation.duration
                running: true
                onTriggered:
                {
                    byTimer = true
                    myItem.state = "dissapearing"
                }
            }

            SequentialAnimation on color
            {
                loops: Animation.Infinite

                ColorAnimation
                {
                    to: myItem.itemColor2
                    duration: 1500
                }

                ColorAnimation
                {
                    to: myItem.itemColor
                    duration: 1500
                }
            }

            RotationAnimation on rotation
            {
                from: 0
                to: 360
                duration: 15000
                loops: Animation.Infinite
                easing.type: Easing.Linear
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

            states:
                [
                State
                {
                    name: "initial"
                    PropertyChanges {target: myItem; width: itemSize; height: itemSize}
                },
                State
                {
                    name: "dissapearing"
                    PropertyChanges {target: myItem; width: 0; height: 0}
                }

            ]
            transitions:
                [
                Transition
                {
                    id: transition
                    from: "initial";
                    to: "dissapearing"
                    PropertyAnimation
                    {
                        id: transitionAnimation
                        target: myItem
                        properties: "width, height"
                        easing.type: Easing.InCirc
                        duration: 400
                    }
                    onRunningChanged:
                    {
                        if (!running && !myItem.byTimer)
                            ItemManager.itemClicked(index)
                    }
                }

            ]
        }//my item
    }//repeater
}
