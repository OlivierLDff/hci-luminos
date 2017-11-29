import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import LuminosModel 1.0

ColumnLayout
{
	MapPane
	{
		Layout.fillHeight : true
		Layout.fillWidth : true
	}
	Label
	{
		text: "Master"
		Layout.leftMargin: 10
	}
	Slider
	{
		Layout.fillWidth: true
		Layout.leftMargin: 10
		Layout.rightMargin: 10
		Layout.alignment: Qt.AlignHCenter

		value: 1

		ToolTip.visible: hovered
		ToolTip.delay: 1000
		ToolTip.text: qsTr("Change the dimmer of selection")

		onValueChanged:
		{
			console.log("value is " + value)
			FixturesModel.Master = value
		}	
	}
}