import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Pane
{
	Image
	{
		id: colorpickerim

		//image attribute
		sourceSize.width: 128
		sourceSize.height: 128
		fillMode: Image.PreserveAspectFit
		source: "colorpicker"

		//layout attribyte
		Layout.alignment: Qt.AlignCenter
	}
}