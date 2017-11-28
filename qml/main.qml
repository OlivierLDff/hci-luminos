import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import LuminosModel 1.0

ApplicationWindow
{
	id: window
	width: 640
	height: 480
	//visible: true
	title:"Luminos App"
	minimumWidth: toolbar.implicitWidth//Math.max(toolbar.implicitWidth, mainPage.implicitWidth)
	minimumHeight: toolbar.implicitHeight + mainPage.implicitHeight//toolbar.implicitHeight + mainPage.minimumHeight

	Material.primary : Material.color(Material.Red)
	Material.accent : Material.color(Material.Orange)

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
                text: "LuminosApp"
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
				Layout.leftMargin: 20
				Layout.rightMargin: 20
            }

			ToolButton 
			{
				//anchors.right: parent.right
                contentItem: Image 
				{
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "select"
                }
                onClicked: FixturesModel.SelectAll();
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
                        onTriggered: settingsDialog.open()
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

	StackView 
	{
        id: stackView
        anchors.fill: parent

        initialItem: MainPage
		{
			id: mainPage
			//anchors.fill: parent
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

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: "The Qt Quick Controls 2 module delivers the next generation user interface controls based on Qt Quick."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "In comparison to the desktop-oriented Qt Quick Controls 1, Qt Quick Controls 2 "
                    + "are an order of magnitude simpler, lighter and faster, and are primarily targeted "
                    + "towards embedded and mobile platforms."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }

	Component.onCompleted: visible = true
}
