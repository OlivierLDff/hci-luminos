import QtQuick 2.7
import QtQuick.Controls 2.1
import LuminosModel 1.0
import QtGraphicalEffects 1.0

Item
{
	id: control	
	property real dim : 1

	property bool selected: false;
	property bool pressed: false;

	property int red: 255
	property int green: 255
	property int blue: 255

	state: selected ? "SELECTED" : "RELEASED"	

	width: 64
	height: 64

	signal clicked()

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
			//control.selected = !control.selected;
			control.clicked()
			//console.log(control.selected)
		}
		onPressed: control.pressed = true
		onReleased: control.pressed = false
	}

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

			height: parent.height*control.dim

			fillMode: Image.PreserveAspectCrop
			source: "LightBulb"
		}
		/*DropShadow 
		{
			opacity: !selected ? 1 : 0
			anchors.fill: lightbulb
			horizontalOffset: 3
			verticalOffset: 3
			radius: 8.0
			samples: 17
			color: "#80000000"
			source: lightbulb
		}*/
	}

	states:
	[
		State
		{
			name: "SELECTED"
			PropertyChanges { target: foreground.border; width: 32}
			PropertyChanges { target: foreground; width: 55}
			PropertyChanges { target: foreground; height: 55}
			PropertyChanges { target: background; width: 74}
			PropertyChanges { target: background; height: 74}
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
