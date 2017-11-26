import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

Page 
{
    id: control
	implicitWidth: info.implicitWidth+ 20// Math.max(tabBar.implicitWidth + 20, swipeView.implicitWidth) // oliv: why +20 is needed?
	implicitHeight: tabBar.implicitHeight + info.implicitHeight

	property bool drawMap: false

	SwipeView 
	{
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
		clip:true 
		interactive: !color.pressed

		InfoPage
		{
			id: info
		}

		ColorPage
		{
			id: color
		}

		EffectPage
		{
			id: effect
		}
		function addPage(page) 
		{
            addItem(page)
            page.visible = true
        }

        function removePage(page)
		{
            for (var n = 0; n < count; n++) { if (page === itemAt(n)) { removeItem(n) } }
            page.visible = false
        }
	}

	onDrawMapChanged:
	{
		if(!mapPane.visible && drawMap)
			swipeView.addPage(mapPane);			
		else if(mapPane.visible && !drawMap)
			swipeView.removePage(mapPane);
	}

	MapPane
	{
		id : mapPane
		visible: false
	}

	footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
			text: qsTr("Info")
        }
        TabButton {
			text: qsTr("Color")
        }
        TabButton {
			text: qsTr("Effect")
        }
		TabButton {
			text: qsTr("Map")
			width: drawMap ? undefined : 0
			visible: drawMap
        }
		
    }
}
