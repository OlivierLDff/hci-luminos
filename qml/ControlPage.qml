import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Page 
{
    id: page

	SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Repeater {
            model: 3

            Pane {
                width: swipeView.width
                height: swipeView.height

                Column {
                    spacing: 40
                    width: parent.width

                    Label {
                        width: parent.width
                        wrapMode: Label.Wrap
                        horizontalAlignment: Qt.AlignHCenter
                        text: "TabBar is a bar with icons or text which allows the user"
                              + "to switch between different subtasks, views, or modes."
                    }

                    Image {
                        source: "../images/arrows.png"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
	}

	footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: "Main"
        }
        TabButton {
            text: "Color"
        }
        TabButton {
            text: "Effect"
        }
    }
}