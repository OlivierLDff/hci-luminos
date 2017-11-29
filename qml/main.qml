import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import LuminosModel 1.0

ApplicationWindow
{
	id: window
	width: 640
	height: 480
	title:"Luminos App"
	minimumWidth: mainPage.implicitWidth
	minimumHeight: mainPage.implicitHeight

	Material.primary : Material.color(Material.Red)
	Material.accent : Material.color(Material.Orange)

	StackView 
	{
        id: stackView
        anchors.fill: parent
        initialItem: MainPage
		{
			id: mainPage
		}
    }

	Component.onCompleted: visible = true
}
