import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Pane
{
	implicitWidth: root.implicitWidth
	implicitHeight: root.implicitHeight
	ColumnLayout
	{
		id: root
		spacing: 2
		Image
		{
			id: weatherImage

			//image attribute
			sourceSize.width: 64
			sourceSize.height: 64
			fillMode: Image.PreserveAspectFit
			source: "Sun"

			//layout attribyte
			Layout.alignment: Qt.AlignCenter
		}
		Label
		{
			id: locationText
			text: "Budapest"
			font.bold : true
			font.capitalization: Font.AllUppercase
			horizontalAlignment : Text.AlignHCenter
		
			Layout.alignment: Qt.AlignCenter
		}
		Label
		{
			id: weatherText
			text: "Sunny - 19C"

			Layout.alignment: Qt.AlignCenter
		}
		Text
		{
			id: lumText
			text: "Luminosity - 90000 lux"

			Layout.alignment: Qt.AlignCenter
		}
	}
	
}