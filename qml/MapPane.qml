import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import LuminosModel 1.0

Pane
{
	//id: control
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
			//property real zoom: 0.5;
			anchors.fill: parent
			//width: parent.width
			//height: parent.height
			contentWidth: 730//*restaurantMap.scale
			contentHeight: 730//*restaurantMap.scale
			clip: true
			ScrollIndicator.vertical: ScrollIndicator { }
			ScrollIndicator.horizontal: ScrollIndicator { }
			PinchArea 
			{
				//width: Math.max(flick.contentWidth, flick.width)
				//height: Math.max(flick.contentHeight, flick.height)
				anchors.fill:parent

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
				width: Math.max(flick.contentWidth, flick.width)
				height: Math.max(flick.contentHeight, flick.height)
				onClicked: console.log(flick.contentX)
				onWheel: 
				{
					flick.resizeContent(flick.contentWidth + wheel.angleDelta.y/2, flick.contentHeight + wheel.angleDelta.y/2, Qt.point(wheel.x, wheel.y))
					/*restaurantMap.scale += restaurantMap.scale * wheel.angleDelta.y/1200;
					if(restaurantMap.scale < 0.7) restaurantMap.scale = 0.7
					else if(restaurantMap.scale > 1) restaurantMap.scale = 1*/
					flick.returnToBounds()
					console.log("contentWidth : " + flick.contentWidth)

                }
            }
			Image 
			{
				id: restaurantMap
				anchors.fill: parent
				//Behavior on scale { PropertyAnimation {properties: "scale"; easing.type: Easing.InQuad } }
				source: "RestaurantMap"
            }
			Slider
			{
				id: slid
				x: flick.contentWidth*0.2
				y: flick.contentHeight*0.35
			}
			Rectangle
			{
				id: control
				property real xPos: 0.3
				property real yPos: 0.3
				property real ratio : flick.contentWidth/730

				readonly property real sizeX: 64
				readonly property real sizeY: 64

				property real dimmer : slid.value

				color: "#FF0051"
				width: sizeX*ratio
				height: sizeY*ratio
				x: flick.contentWidth*xPos-sizeX/2*ratio
				y: flick.contentHeight*yPos-sizeY/2*ratio
				radius: sizeX/2*ratio
				/*Rectangle
				{
					clip: true
					//anchors.fill: parent
					anchors.bottom : parent.bottom
					anchors.right: parent.right
					anchors.left: parent.left
					//scale: 0.8
					height: control.height*(control.dimmer)//*0.7
					color: "red"*/
					//verticalAlignment: Image.AlignBottom
					Image 
					{
						id: lightbulb
						scale: 0.8
						anchors.bottom: control.bottom
						width: control.width
						height: control.height*control.dimmer
						//anchors.margins: 10
						verticalAlignment: Image.AlignBottom
						
						
						//scale: 0.8
						//width: control.width*0.7
						//scale: 0.8
						//height: control.height*(control.dimmer)//*0.7
						fillMode: Image.PreserveAspectCrop
						//Behavior on scale { PropertyAnimation {properties: "scale"; easing.type: Easing.InQuad } }
						source: "LightBulb"
					}
				//}
				
			}
		}
	}
}