import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Page 
{
    id: page
	implicitWidth: info.implicitWidth// Math.max(tabBar.implicitWidth + 20, swipeView.implicitWidth) // oliv: why +20 is needed?
	implicitHeight: tabBar.implicitHeight + info.implicitHeight
	SwipeView 
	{
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

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
	}

	footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: "Info"
        }
        TabButton {
            text: "Color"
        }
        TabButton {
            text: "Effect"
        }
    }
}