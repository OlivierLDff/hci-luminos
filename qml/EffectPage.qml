import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Pane
{
	id:pane
	ColumnLayout
	{
		id:root
		anchors.fill: parent	
		Button
		{
		Layout.alignment: Qt.AlignHCenter
			text:"Off"
			Layout.fillWidth: true	
		}
		GridLayout
		{
			id: grid
			columns: 2

			Layout.alignment: Qt.AlignHCenter
			Layout.fillWidth: true	
			Layout.fillHeight: true	
			
			Button
			{
				Layout.fillWidth: true	
				Layout.fillHeight: true	
				Image
				{
					sourceSize.width: Math.min(parent.height*0.7, parent.width*0.7)
					sourceSize.height: Math.min(parent.height*0.7, parent.width*0.7)
					fillMode: Image.PreserveAspectFit
					anchors.centerIn: parent
					source: "Wave"
				}
			}
			Button
			{
				Layout.fillWidth: true	
				Layout.fillHeight: true	
				Image
				{
					sourceSize.width: Math.min(parent.height*0.7, parent.width*0.7)
					sourceSize.height: Math.min(parent.height*0.7, parent.width*0.7)
					fillMode: Image.PreserveAspectFit
					anchors.centerIn: parent
					source: "Level"
				}
			}
			Button
			{
				Layout.fillWidth: true	
				Layout.fillHeight: true	
				Image
				{
					sourceSize.width: Math.min(parent.height*0.7, parent.width*0.7)
					sourceSize.height: Math.min(parent.height*0.7, parent.width*0.7)
					fillMode: Image.PreserveAspectFit
					anchors.centerIn: parent
					source: "Shuffle"
				}
			}
			Button
			{
				Layout.fillWidth: true	
				Layout.fillHeight: true	
				Image
				{
					sourceSize.width: Math.min(parent.height*0.7, parent.width*0.7)
					sourceSize.height: Math.min(parent.height*0.7, parent.width*0.7)
					fillMode: Image.PreserveAspectFit
					anchors.centerIn: parent
					source: "Rainbow"
				}
			}
		}
	}	
}