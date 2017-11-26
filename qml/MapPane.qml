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
		ListView 
		{
			//width: 200; height: 250
			anchors.fill: parent
			//model: FixturesModel
			model: FixturesModel
			delegate: Text 
			{ 
				text: ("fixture dimmer : " + dimmer )
			}
		}
	}
}