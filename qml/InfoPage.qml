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
				Flickable
				{
					anchors.fill: parent
					contentWidth: width; 
					contentHeight: group.height > (column.implicitHeight) ? height : (column.implicitHeight)
					ColumnLayout
					{
						
						anchors.fill: parent
						id:column
						RadioButton 
						{
							id : weatherButton
							text: "Weather"
							checked: true
							onClicked: 
							{
								console.log("mode : " + Mode.EConsumptionMode_Full)
								FixturesModel.ModelMode = Mode.EConsumptionMode_Weather
							}
							ButtonGroup.group: radioButtonGroup
						}
						RadioButton 
						{
							id : ecoButton
							text: "Eco"
							onClicked: FixturesModel.ModelMode = Mode.EConsumptionMode_Eco
							ButtonGroup.group: radioButtonGroup
						}
						RadioButton 
						{
							id : fullButton
							text: "Full"
							onClicked: FixturesModel.ModelMode = Mode.EConsumptionMode_Full
							ButtonGroup.group: radioButtonGroup
						}
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
					id:master
					height: parent.height
					implicitHeight: 40
					anchors.horizontalCenter: parent.horizontalCenter
					orientation: Qt.Vertical
					
					value: 0//FixturesModel.Master
					onValueChanged:
					{
						console.log("value is " + value)
						FixturesModel.Master = value
					}	
					Component.onCompleted:
					{
						//console.log("Nested Completed Running!" + FixturesModel.Master )
						master.value=FixturesModel.Master 
					}
				}			
			}				
		}
	}	
}