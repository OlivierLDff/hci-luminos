import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

RowLayout
{
	ControlPage
	{
		Layout.fillWidth: true
		Layout.fillHeight: true
		Layout.minimumWidth: 250
	}
	MapPane
	{
		Layout.fillWidth: true
		Layout.fillHeight: true
		Layout.minimumWidth: 100
		Layout.preferredWidth: parent.width*2/3
		Layout.preferredHeight: parent.height
	}
}