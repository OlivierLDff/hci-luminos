import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Pane
{
	id: page
	property bool pressed: colorPicker.pressed
	//implicitWidth: root.implicitWidth
	//implicitHeight: root.implicitHeight
	ColumnLayout
	{
		anchors.fill: parent	
		id: root
		ColorPicker
		{
			id: colorPicker
			Layout.fillWidth: true
			Layout.fillHeight: true
			//anchors.fill: parent
		}
	}	
}