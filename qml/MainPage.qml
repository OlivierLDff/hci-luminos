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
		//Layout.minimumWidth: 250
		//Layout.minimumHeight: 150	
	}
	MapPane
	{
		//Layout.fillWidth: true
		//Layout.fillHeight: true
		Layout.minimumWidth: 100
		//Layout.preferredWidth: 200
		Layout.preferredHeight: parent.height
	}
}