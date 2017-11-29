import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import LuminosModel 1.0

Pane
{
	//id: control
	GroupBox
	{
		title: qsTr("Restaurant")
		anchors.fill: parent
		Flickable
		{
			id: flick
			anchors.fill: parent
			contentWidth: 730
			contentHeight: 730
			clip: true
			ScrollIndicator.vertical: ScrollIndicator { }
			ScrollIndicator.horizontal: ScrollIndicator { }
			PinchArea 
			{
				anchors.fill:parent
				pinch.target : restaurantMap

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
					var newW = initialWidth * pinch.scale
					var newH = initialHeight * pinch.scale
					var s = Math.min(flick.width, flick.height)
					if(newW > 2000) newW = 2000
					else if(newW<s) newW = s
					if(newH > 2000) newH = 2000
					else if(newH<s) newH = s
					flick.resizeContent(newW, newH, pinch.center)
				}

				onPinchFinished: 
				{
					// Move its content within bounds.
					flick.returnToBounds()
				}

				Image 
				{
					id: restaurantMap
					//anchors.fill: parent
					width: flick.contentWidth
					height: flick.contentHeight
					//Behavior on scale { PropertyAnimation {properties: "scale"; easing.type: Easing.InQuad } }
					source: "RestaurantMap"
				}

				MouseArea 
				{
					anchors.fill: parent
					width: Math.max(flick.contentWidth, flick.width)
					height: Math.max(flick.contentHeight, flick.height)
					//onClicked:// console.log(flick.contentX)
					onWheel: 
					{
						var newW = flick.contentWidth + wheel.angleDelta.y/2
						var newH = flick.contentHeight + wheel.angleDelta.y/2
						var s = Math.min(flick.width, flick.height)
						if(newW > 730) newW = 730
						else if(newW<s) newW = s
						if(newH > 730) newH = 730
						else if(newH<s) newH = s
						flick.resizeContent(newW, newH, Qt.point(wheel.x, wheel.y))
						flick.returnToBounds()
						console.log("contentWidth : " + flick.width)

					}
					onDoubleClicked:
					{
						console.log("double click : " + flick.contentWidth + " " + flick.width)
						var s = Math.min(flick.width, flick.height)
						if(flick.contentWidth === flick.width || flick.contentHeight === flick.height)
							flick.resizeContent(730, 730, Qt.point(mouse.x, mouse.y))
						else
							flick.resizeContent(s,s, Qt.point(mouse.x, mouse.y))						
						flick.returnToBounds()
					}
				}

				Repeater 
				{
					model: FixturesModel
					FixtureButton
					{
						dim : model.dimmer/255
						scale : flick.contentWidth/730
						selected: model.selected

						color: model.color 
						colorBack1: model.colorBack1
						colorBack2: model.colorBack2

						x: flick.contentWidth*model.x - width/2
						y: flick.contentHeight*model.y - height/2
						onClicked: FixturesModel.SelectOrDeselectFixture(model.index)//console.log(model.index)
					}
				}
				/*Component.onCompleted:
				{
					var s = Math.min(flick.width, flick.height)
					flick.resizeContent(s, s, Qt.point(0, 0))
					flick.returnToBounds()
				}*/
			}	
		}
	}
}