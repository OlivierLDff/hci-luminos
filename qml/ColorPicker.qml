import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Pane
{
	/*Image
	{
		id: colorpickerim

		//image attribute
		sourceSize.width: 128
		sourceSize.height: 128
		fillMode: Image.PreserveAspectFit
		source: "colorpicker"

		//layout attribyte
		Layout.alignment: Qt.AlignCenter
	}*/
	width: 256
    height: 256
	Rectangle
    {
        anchors.fill: parent
		color: "red"
		/*gradient: Gradient
        {
            GradientStop { position: 0.0; color: Qt.rgba(0, 0, 0, 0) }
            GradientStop { position: 1.0; color: Qt.rgba(255, 0, 0, 255) }
        }*/
    }
}