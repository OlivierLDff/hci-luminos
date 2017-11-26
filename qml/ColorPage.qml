import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Pane
{
	id: page
	property bool pressed: colorPicker.pressed
	//implicitWidth: root.implicitWidth
	//implicitHeight: root.implicitHeight
	ColumnLayout
	{
		anchors.fill: parent	
		id: root
		Text
		{
			text: "Color : "
			Layout.fillWidth: true
		}
		ColorPicker
		{
			id: colorPicker
			Layout.fillWidth: true
			//Layout.fillHeight: true
			//anchors.fill: parent
		}
		ColumnLayout
		{
			Text
			{
				text: "Brightness : "
			}
			Slider 
			{
				id: brightness
				anchors.horizontalCenter: parent.horizontalCenter
				Layout.fillWidth: true
				value: 1
				onValueChanged: FixturesModel.SetBrighness(value)
			}
		}
	}	
}