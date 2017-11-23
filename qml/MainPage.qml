import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

RowLayout
{
	ControlPage
	{
		id: control
		Layout.fillWidth: true
		Layout.fillHeight: true
		Layout.minimumWidth: implicitWidth
	}
	MapPane
	{
		id: map
		visible: Window.width > 500
		implicitHeight: 0 //todo: remove this and find a better way
		//implicitWidth: Window.width ? 0 : undefined
		Layout.fillWidth: true
		Layout.fillHeight: true
	}
}