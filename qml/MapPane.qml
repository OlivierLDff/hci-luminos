import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import LuminosModel 1.0

Pane
{
	id: control
	GroupBox
	{
		title: qsTr("Restaurant")
		anchors.fill: parent
		/*Image
		{
			id: colorpickerim

			//image attribute
			//sourceSize.width: 128
			//sourceSize.height: 128
			fillMode: Image.PreserveAspectFit
			source: "RestaurantMap"
			anchors.fill: parent
			//layout attribyte
			Layout.alignment: Qt.AlignCenter
		}*/
		/*ListView 
		{
			//width: 200; height: 250
			anchors.fill: parent
			//model: FixturesModel
			model: FixturesModel
			delegate: Text 
			{ 
				text: ("fixture dimmer : " + dimmer )
			}
		}*/
		Flickable
		{
			id: flick
			property real zoom: 0.5;
			anchors.fill: parent
			//width: parent.width
			//height: parent.height
			contentWidth: 730*restaurantMap.scale
			contentHeight: 730*restaurantMap.scale
			clip: true
			ScrollIndicator.vertical: ScrollIndicator { }
			ScrollIndicator.horizontal: ScrollIndicator { }
			PinchArea 
			{
				width: Math.max(flick.contentWidth, flick.width)
				height: Math.max(flick.contentHeight, flick.height)

				property real initialWidth
				property real initialHeight
				onPinchStarted: 
				{
					initialWidth = flick.contentWidth
					initialHeight = flick.contentHeight
				}

				onPinchUpdated: 
				{
					// adjust content pos due to drag
					flick.contentX += pinch.previousCenter.x - pinch.center.x
					flick.contentY += pinch.previousCenter.y - pinch.center.y

					// resize content
					flick.resizeContent(initialWidth * pinch.scale, initialHeight * pinch.scale, pinch.center)
				}
				onPinchFinished: 
				{
					// Move its content within bounds.
					flick.returnToBounds()
				}
			}
			MouseArea 
			{
				anchors.fill: parent
				onClicked: console.log("click")
				onWheel: 
				{
					restaurantMap.scale += restaurantMap.scale * wheel.angleDelta.y/1200;
					if(restaurantMap.scale < 0.7) restaurantMap.scale = 0.7
					else if(restaurantMap.scale > 1) restaurantMap.scale = 1

					console.log("contentWidth : " + flick.contentWidth)
                }
            }
			Image 
			{
				id: restaurantMap
				anchors.fill: parent
				Behavior on scale { PropertyAnimation {properties: "scale"; easing.type: Easing.InQuad } }
				source: "RestaurantMap"
            }
			Rectangle
			{
				color: "red"
				width: 2
				height: 2
				x: flick.contentWidth*0.3*restaurant.scale
				y: flick.contentHeight*0.3*restaurant.scale
			}
		}
	}
}