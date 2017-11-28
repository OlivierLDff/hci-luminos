import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

RowLayout
{
	ControlPage
	{
		id: control
		Layout.fillWidth: drawMap
		Layout.fillHeight: true
		Layout.minimumWidth: implicitWidth
		drawMap : Window.width <= implicitWidth + 200
	}
	MapPane
	{
		id: map
		visible: Window.width > control.implicitWidth + 200
		implicitHeight: 0
		Layout.fillWidth: true
		Layout.fillHeight: true
	}
}