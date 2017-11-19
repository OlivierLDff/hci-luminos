import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Page 
{
    id: page

	SwipeView 
	{
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

		InfoPage
		{
			//width: swipeView.width
            //height: swipeView.height
		}

		ColorPage
		{
			//width: swipeView.width
            //height: swipeView.height
		}

		EffectPage
		{
			//width: swipeView.width
			//height: swipeView.height
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