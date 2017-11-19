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

		WeatherWidget
		{
			Layout.alignment: Qt.AlignHCenter
		}

		RowLayout
		{
			id: row
			Layout.alignment: Qt.AlignHCenter
			Layout.fillWidth: true	
			Frame
			{
				id: group
				Layout.fillWidth: true	
				Column 
				{
					anchors.horizontalCenter: parent.horizontalCenter
					RadioButton 
					{
						text: "Weather"
						checked: true
					}
					RadioButton 
					{
						text: "Eco"						
					}
					RadioButton 
					{
						text: "Full"
					}
				}
			}	
			Frame
			{
				Layout.maximumHeight: group.height
				//Layout.fillWidth: true
				Slider 
				{		
				//anchors.fill: parent
					height: parent.height
					orientation: Qt.Vertical
					value: 1
					anchors.horizontalCenter: parent.horizontalCenter
					onValueChanged:
						console.log("value is " + value)
				}			
			}				
		}
	}	
}