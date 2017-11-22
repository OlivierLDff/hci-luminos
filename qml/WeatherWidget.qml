import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import LuminosModel 1.0

Pane
{
	//implicitWidth: root.implicitWidth
	implicitHeight: root.implicitHeight + 10
	RowLayout
	{
		id: root
		//layout attribyte
		spacing : 10
		Layout.alignment: Qt.AlignCenter
		ColumnLayout
		{		
			spacing: 2
			Image
			{
				id: weatherImage

				Layout.minimumWidth: 32
				Layout.minimumHeight: 32
				Layout.preferredWidth: 64
				Layout.preferredHeight: 64
				fillMode: Image.PreserveAspectFit
				source: SensorModel.WeatherImage
			}
			Label
			{
				id: locationText
				text: qsTr("Budapest")
				font.bold : true
				font.capitalization: Font.AllUppercase
				horizontalAlignment : Text.AlignHCenter

				Layout.alignment: Qt.AlignCenter
			}
		}
		ColumnLayout
		{
			Label
			{
				id: weatherText
				text: qsTr("Sunny : " + SensorModel.Temperature + "C")

				//Layout.alignment: Qt.AlignCenter
			}
			Text
			{
				id: lumText
				text: qsTr("Luminosity : " + SensorModel.Lux + " lux")

				//Layout.alignment: Qt.AlignCenter
			}
		}
	}	
}