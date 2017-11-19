import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1

Pane
{
	id: control
	GroupBox
	{
		title: qsTr("Restaurant")
		anchors.fill: parent
		Image
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
		}
	}
}