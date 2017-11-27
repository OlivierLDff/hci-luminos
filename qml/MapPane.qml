import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
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
			Item
			{
				id: control
				property real xPos: 0.3
				property real yPos: 0.3
				property real dimmer : slid.value

				property bool selected: false;
				property bool pressed: false;

				state: selected ? "SELECTED" : "RELEASED"

				//color: "#80FF0051"
				//border.color : "#FFFF0051"

				scale : flick.contentWidth/730

				width: 64
				height: 64

				x: flick.contentWidth*xPos - width/2
				y: flick.contentHeight*yPos - height/2

				Rectangle
				{
					id: foreground
					radius: width/2
					anchors.centerIn: parent
					color: "#40FF0051"
					border.color : "#FFFF0051"
				}
				Rectangle
				{
					id: background
					anchors.centerIn: parent
					color: "transparent"
					border.color: "#80FF0051"
					border.width: 10
					radius: width/2
					//opacity: control.selected ? 1 : 0 
				}

				MouseArea 
				{
					id: mouseArea
					anchors.fill: parent
					onClicked:
					{
						control.selected = !control.selected;
						console.log(control.selected)
					}
					onPressed: control.pressed = true
					onReleased: control.pressed = false
				}

				/*Rectangle
				{
					radius: control.radius
					color: "#80FF0051"
					anchors.fill: control
					border.width : 2
					border.color : "#FFFF0051"
				}*/

				Rectangle
				{
					anchors.fill: parent
					anchors.margins: 10
					color: "transparent"
					
					Image 
					{
						id: lightbulbback
						anchors.fill: parent
						opacity : 0.6
						source: "LightBulb"
					}
					Image 
					{
						id: lightbulb
						clip: true

						anchors.bottom: parent.bottom
						anchors.right: parent.right
						anchors.left: parent.left
						verticalAlignment: Image.AlignBottom

						height: parent.height*control.dimmer

						fillMode: Image.PreserveAspectCrop
						source: "LightBulb"
					}		
					DropShadow {
        anchors.fill: lightbulbback
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: lightbulbback
    }
				}

				states: 
				[
					State
					{
						name: "SELECTED"
						PropertyChanges { target: foreground.border; width: 32}
						PropertyChanges { target: foreground; width: 50}
						PropertyChanges { target: foreground; height: 50}
						PropertyChanges { target: background; width: 64}
						PropertyChanges { target: background; height: 64}
					},
					State 
					{
						name: "RELEASED"
						PropertyChanges { target: foreground.border; width: 3}
						PropertyChanges { target: foreground; width: 64}
						PropertyChanges { target: foreground; height: 64}
						PropertyChanges { target: background; width: 0}
						PropertyChanges { target: background; height: 0}
					}
				]

				transitions: 
				[
					Transition
					{
						from: "SELECTED"
						to: "RELEASED"
						NumberAnimation { properties: "width"; target: foreground.border; duration: 250; easing.type: Easing.OutQuad}

						NumberAnimation { properties: "width"; target: foreground; duration: 250; easing.type: Easing.OutQuad}
						NumberAnimation { properties: "height"; target: foreground; duration: 250; easing.type: Easing.OutQuad}

						NumberAnimation { properties: "width"; target: background; duration: 250; easing.type: Easing.OutQuad}
						NumberAnimation { properties: "height"; target: background; duration: 250; easing.type: Easing.OutQuad}
					},
					Transition 
					{
						from: "RELEASED"
						to: "SELECTED"
						NumberAnimation { properties: "width"; target: foreground.border; duration: 250; easing.type: Easing.OutQuad}

						NumberAnimation { properties: "width"; target: foreground; duration: 250; easing.type: Easing.OutQuad}
						NumberAnimation { properties: "height"; target: foreground; duration: 250; easing.type: Easing.OutQuad}

						NumberAnimation { properties: "width"; target: background; duration: 250; easing.type: Easing.OutQuad}
						NumberAnimation { properties: "height"; target: background; duration: 250; easing.type: Easing.OutQuad}
					}
				]
			}			
		}
	}
}