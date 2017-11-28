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
	ColumnLayout
	{
		anchors.fill: parent
		id: root
		Label
		{
			id: textcolor
			text: "    Color Picker"
			Layout.fillWidth: true
		}
		ColorPicker
		{
			id: colorPicker
			Layout.fillWidth: true
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

				GridView
				{
					id: gridView
					cellWidth: 46; cellHeight: 46
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
	}
}
