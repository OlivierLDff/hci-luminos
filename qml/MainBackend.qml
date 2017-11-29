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
	visible: true
	title:"Luminos Backend"
	minimumWidth: Math.max(toolbar.implicitWidth, root.implicitWidth)
	minimumHeight: toolbar.implicitHeight + root.implicitHeight//toolbar.implicitHeight + mainPage.minimumHeight

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
                text: window.title
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

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: "Backend of luminos app"
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }
	ColumnLayout
	{
		id: root
		anchors.fill: parent	
		anchors.margins : 20
		WeatherWidget
		{
			Layout.alignment: Qt.AlignHCenter
		}
		GridLayout
		{
			id: grid
			columns: window.width > 300 ? 3 : 2;

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
					source: "Sun"
				}
				onClicked: SensorModel.WeatherImage ="Sun"
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
					source: "Cloudy"
				}
				onClicked: SensorModel.WeatherImage = "Cloudy"
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
					source: "Rain"
				}
				onClicked: SensorModel.WeatherImage = "Rain"
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
					source: "Storm"
				}
				onClicked: SensorModel.WeatherImage = "Storm"
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
					source: "Night"				
				}
				onClicked: SensorModel.WeatherImage = "Night"
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
					source: "Snowing"		
				}	
				onClicked: SensorModel.WeatherImage = "Snowing"
			}
		}
		RowLayout
		{
			Layout.fillWidth: true	
			Dial
			{
				id: temperatureDial
				value: 20
				from: -50
				to: 50
				Layout.fillWidth: true	
				onValueChanged: SensorModel.Temperature = Math.floor(value)
			}
			Dial
			{
				id: luxDial
				value: 20000
				from: 0
				to: 20000
				Layout.fillWidth: true	
				onValueChanged: SensorModel.Lux = Math.floor(value)
				stepSize : 100
			}
		}
	}
}
