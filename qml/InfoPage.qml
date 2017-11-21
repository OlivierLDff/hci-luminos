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

		WeatherWidget
		{
			Layout.alignment: Qt.AlignHCenter
		}
		
		RowLayout
		{
			id: row
			anchors.right: parent.right
			anchors.left: parent.left
			anchors.bottom: parent.bottom	

			Frame
			{
				id: group
				Layout.fillWidth: true	
				Layout.fillHeight: true	
				clip: true
				ButtonGroup 
				{
					id: radioButtonGroup
				}
				ListView 
				{
					ScrollBar.vertical: ScrollBar { }
					width: group.width; 
					height: group.height

					model: ListModel 
					{
						ListElement 
						{
							buttonFunction: "Weather"
							check: true
						}
						ListElement 
						{
							buttonFunction: "Eco"
							check: false
						}
						ListElement 
						{
							buttonFunction: "Full"
							check: false
						}				
					}
					delegate: RadioButton 
					{
						text: buttonFunction
						checked: check
						ButtonGroup.group: radioButtonGroup
						//							onPressed: 
						//		SensorModel.WeatherImage = "Cloudy";
					}
				}
			}	
			Frame
			{
				anchors.top: parent.top
				anchors.right: parent.right
				anchors.bottom: parent.bottom
				Slider 
				{
					height: parent.height
					implicitHeight: 40
					anchors.horizontalCenter: parent.horizontalCenter
					orientation: Qt.Vertical
					
					value: 1
					onValueChanged:
						console.log("value is " + value)
				}			
			}				
		}
	}	
}