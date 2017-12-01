import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import LuminosModel 1.0

Page
{
	id: root
	header: ToolBar
	{
		Material.foreground: "white"
		RowLayout //Toolbar don't come with a layout, most frequent used layout is row layout
		{
			id: toolbar
			anchors.fill: parent

			Label 
			{
                id: titleLabel
                text: "Luminos App"
                font.pixelSize: 20
                elide: Label.ElideRight
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
				Layout.leftMargin: 20
				Layout.rightMargin: 20
            }

			ToolButton 
			{
                contentItem: Image 
				{
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "select"
                }
                onClicked: FixturesModel.SelectAll();
				
				ToolTip.visible: hovered | down
				ToolTip.delay: 500
				ToolTip.text: qsTr("Select all fixtures")
			}

			ToolButton 
			{
				visible: FixturesModel.SelectionSize > 0
                contentItem: Image 
				{			
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "clear"
                }
                onClicked: FixturesModel.ClearSelection();
				ToolTip.visible: hovered | down
				ToolTip.delay: 500
				ToolTip.text: qsTr("Clear the selection")
			}

            ToolButton 
			{
				anchors.right: parent.right
                contentItem: Image 
				{
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "menu"
                }
                onClicked: optionsMenu.open()

                Menu 
				{
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem 
					{
                        text: "Settings"
						onTriggered: root.StackView.view.push("Settings.qml", { })
                    }
                    MenuItem 
					{
                        text: "About"
                        onTriggered: aboutDialog.open()
                    }
					MenuItem 
					{
                        text: "Quit"
                        onTriggered: Qt.quit()
                    }
                }
            }
		}
	}
	Dialog {
        id: aboutDialog
        modal: true
        focus: true
        title: "About"
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: aboutColumn.height

        Column 
		{
            id: aboutColumn
            spacing: 20

            Label 
			{
                width: aboutDialog.availableWidth
                text: "Luminos App is the perfect lighting control system for small businesses and private houses."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }
	RowLayout
	{
		anchors.fill: parent
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
}
