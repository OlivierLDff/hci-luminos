import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import LuminosModel 1.0

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
			onClicked: FixturesModel.SetFx(FxType.None)
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
				onClicked: FixturesModel.SetFx(FxType.Lin)
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
				onClicked: FixturesModel.SetFx(FxType.Sin)
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
				onClicked: FixturesModel.SetFx(FxType.Random)
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
				onClicked: FixturesModel.SetFx(FxType.Rainbow)
			}
		}
	}	
}