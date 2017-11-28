import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQml.Models 2.1
import QtQuick.Controls.Material 2.1
import LuminosModel 1.0

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
		/*GroupBox
		{
			title: qsTr("Color Picker")
			Layout.fillWidth: true
			ColorPicker
			{
				anchors.fill: parent
			}
		}*/
		Label
		{
			id: textcolor
			text: "    Color Picker"
			Layout.fillWidth: true
			//Layout.fillHeight: true
			//anchors.top: parent.top
			//anchors.topMargin: 10
			//anchors.bottomMargin: 10
		}
		ColorPicker
		{
			id: colorPicker
			Layout.fillWidth: true
			//Layout.fillHeight: true
			//anchors.top: textcolor.bottom
			//anchors.topMargin: 10
			//anchors.bottomMargin: 10
			//Layout.fillHeight: true
			//anchors.fill: parent
			//anchors.bottom: parent.bottom
			//anchors.top: parent.top
		}
		GroupBox
		{
			title: qsTr("Color Palette")
			Layout.fillWidth: true
			Layout.fillHeight: true
			Flickable
			{
				id: flickable
				anchors.fill: parent
				clip: true

				/*anchors.top: colorPicker.bottom
				anchors.bottom: page.bottom 

				anchors.topMargin: 10
				anchors.bottomMargin: 10*/

				GridView
				{
					id: gridView
					//anchors.top: colorPicker.bottom
					//anchors.bottom: page.bottom 
					cellWidth: 46; cellHeight: 46
					//width: parent.width; height: parent.height
					anchors.fill: parent
					model: ListModel 
						{
							id: colorModel
							ListElement { color: Material.Red }
							ListElement { color: Material.DeepOrange }
							ListElement { color: Material.Orange }
							ListElement { color: Material.Amber }
							ListElement { color: Material.Yellow }
							ListElement { color: Material.Lime }
							ListElement { color: Material.LightGreen }
							ListElement { color: Material.Green }
							ListElement { color: Material.Teal }
							ListElement { color: Material.Cyan }
							ListElement { color: Material.LightBlue }
							ListElement { color: Material.Blue }
							ListElement { color: Material.Indigo }
							ListElement { color: Material.DeepPurple }
							ListElement { color: Material.Purple }
							ListElement { color: Material.Pink }
						
						}
						delegate: Rectangle 
						{
							id: icon
							width: 40; height: 40
							/*anchors 
							{
								horizontalCenter: gridView.horizontalCenter;
								verticalCenter: gridView.verticalCenter
							}*/
							color: Material.color(model.color)
							radius: 3
							MouseArea
							{
								anchors.fill: parent
								onClicked: FixturesModel.SetColor(Material.color(model.color))
							}
						}
				}
			}
		}
		
		

		/*ColumnLayout
		{
			Text
			{
				text: "Brightness : "
			}
			Slider 
			{
				id: brightness
				anchors.horizontalCenter: parent.horizontalCenter
				Layout.fillWidth: true
				value: 1
				onValueChanged: FixturesModel.SetBrighness(value)
			}
		}*/
	}	
}