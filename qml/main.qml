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
	title:"Luminos App"
	minimumWidth: cl.implicitWidth
	minimumHeight: cl.implicitHeight
	ColumnLayout
	{
		id: cl
		anchors.centerIn: parent
		spacing: 10
		Text
		{
			text:"Luminos App!"
			anchors.horizontalCenter: parent
		}
		Image
		{
			sourceSize.width: 64
       sourceSize.height: 64
			anchors.horizontalCenter: parent
			fillMode: Image.PreserveAspectFit
			source: "AppIcon"
			smooth : true
		}
	}



	/*StackView
	{
		id: stackView
		anchors.fill: parent
		initialItem: ContactPage {}
	}*/
}
