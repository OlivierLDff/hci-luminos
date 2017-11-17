import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

ApplicationWindow
{
	id: window
	width: 640
	height: 480
	visible: true

	Text
	{
		text:"hello world !"
		anchors.centerIn: parent
	}

	/*StackView
	{
		id: stackView
		anchors.fill: parent
		initialItem: ContactPage {}
	}*/
}
