import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

RowLayout
{
	//implicitHeight: control.implicitHeight 
	//implicitWidth: control.implicitWidth //not working, why???? kurwa fuck
	ControlPage
	{
		id: control
		Layout.fillWidth: true
		Layout.fillHeight: true
		Layout.minimumWidth: implicitWidth
		drawMap : Window.width <= 500
	}
	MapPane
	{
		id: map
		visible: Window.width > 500
		implicitHeight: 0
		Layout.fillWidth: true
		Layout.fillHeight: true
	}
}