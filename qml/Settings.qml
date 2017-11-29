import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import LuminosModel 1.0

Page
{
	id: root
	//anchors.fill : parent
	header: ToolBar
	{
		Material.foreground: "white"
		RowLayout //Toolbar don't come with a layout, most frequent used layout is row layout
		{
			id: toolbar
			anchors.fill: parent

			ToolButton 
			{
				text: qsTr("Back")
				anchors.left: toolbar.left
				anchors.leftMargin: 10
				anchors.verticalCenter: toolbar.verticalCenter
                contentItem: Image 
				{
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "back"
                }
                onClicked: root.StackView.view.pop()
			}
			Label 
			{
                id: titleLabel
                text: "Settings"
                font.pixelSize: 20
                elide: Label.ElideRight
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
				Layout.leftMargin: 20
				Layout.rightMargin: 20
            }
		}
	}
	Pane
	{
		anchors.fill: parent
	
		ColumnLayout 
		{
			anchors.right: parent.right
			anchors.left: parent.left
			SwitchDelegate 
			{
				Layout.fillWidth: true
				text: "Artnet Output"
				checked: true
				onCheckedChanged: LuminosModel.ArtnetOutput = checked
				ToolTip.visible: hovered | down
				ToolTip.delay: 500
				ToolTip.text: qsTr("Switch Artnet output")
			}
			RowLayout
			{
				spacing : 10
				Layout.fillWidth: true
				Label
				{
					text: "Artnet Network Adapter"
				}
				ComboBox 
				{
					Layout.fillWidth: true
					model: ListModel 
					{
						id: model
						ListElement { text: "Banana" }
						ListElement { text: "Apple" }
						ListElement { text: "Coconut" }
					}
					onAccepted:
					{
						if (find(editText) === -1)
							model.append({text: editText})
					}
					ToolTip.visible: hovered | down
					ToolTip.delay: 500
					ToolTip.text: qsTr("Network adapter for Artnet")
				}		
			}	
		}
	}
}