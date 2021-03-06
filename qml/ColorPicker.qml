import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import LuminosModel 1.0
import QtGraphicalEffects 1.0

Item
{
	id: control
	property bool pressed: mouseArea.pressed
	//implicitWidth: colorPicker.width
	implicitHeight: colorPicker.height
	Image
	{
		id: colorPicker
		source: "ColorPickerRect"
		width: parent.width
		//sourceSize.width: parent.width
		//sourceSize.height: parent.height
		fillMode: Image.PreserveAspectFit
		MouseArea
		{
			id: mouseArea
			anchors.fill: colorPicker
			onPressed:
			{
				cursor.x = mouse.x - 4
				cursor.y = mouse.y - 4
				FixturesModel.SetColorFromPicker(cursor.x*360/colorPicker.width, cursor.y/colorPicker.height)	
			}
			onPositionChanged:
			{
				var clampX = (mouse.x > 0 && mouse.x < colorPicker.width) ?  mouse.x : (mouse.x > 0) ? colorPicker.width : 0;
				var clampY = (mouse.y > 0 && mouse.y < colorPicker.height) ? mouse.y : (mouse.y > 0) ? colorPicker.height : 0;
				cursor.x = clampX - 4;
				cursor.y = clampY - 4;
				FixturesModel.SetColorFromPicker(clampX*360/colorPicker.width, clampY/colorPicker.height)	
			}
			Image
			{
				id: cursor
				x: width/2
				y: height/2
				source: "colorPickerCursor"
			}
		}	
	}
	
	/*Rectangle
    {
        anchors.fill: parent
		id: rect
		//color: "red"
		/*gradient: Gradient
        {
            GradientStop { position: 0.0; color: Material.color(Material.Red) }
            GradientStop { position: 0.0625; color: Material.color(Material.DeepOrange) }
			GradientStop { position: 0.125; color: Material.color(Material.Orange) }
			GradientStop { position: 0.1875; color: Material.color(Material.Amber) }
			GradientStop { position: 0.25; color: Material.color(Material.Yellow) }
			GradientStop { position: 0.3125; color: Material.color(Material.Lime) }
			GradientStop { position: 0.375; color: Material.color(Material.LightGreen) }
			GradientStop { position: 0.4375; color: Material.color(Material.Green) }
			GradientStop { position: 0.5; color: Material.color(Material.Teal) }
			GradientStop { position: 0.5625; color: Material.color(Material.Cyan) }
			GradientStop { position: 0.625; color: Material.color(Material.LightBlue) }
			GradientStop { position: 0.6875; color: Material.color(Material.Blue) }
			GradientStop { position: 0.75; color: Material.color(Material.Indigo) }
			GradientStop { position: 0.8125; color: Material.color(Material.DeepPurple) }
			GradientStop { position: 0.875; color: Material.color(Material.Purple) }
			GradientStop { position: 0.9375; color: Material.color(Material.Pink) }
			GradientStop { position: 1.0; color: Material.color(Material.Red) }
        }*/
		/*gradient: Gradient
        {
            GradientStop { position: 0.0; color: Qt.rgba(1,0,0,1) }
            GradientStop { position: 0.16; color: Qt.rgba(1,1,0,1) }
			GradientStop { position: 0.33; color: Qt.rgba(0,1,0,1) }
			GradientStop { position: 0.5; color: Qt.rgba(0,1,1,1) }
			GradientStop { position: 0.66; color: Qt.rgba(0,0,1,1)  }
			GradientStop { position: 1.84; color: Qt.rgba(1,0,1,1)  }
			GradientStop { position: 1.0; color: Qt.rgba(1,0, 0,1)  }
        }*/

		
    //}*/
}