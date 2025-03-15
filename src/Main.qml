import QtQuick
import QtQuick.Controls

pragma ComponentBehavior: Bound

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("JSON TreeView")

    Flickable {
        anchors.fill: parent
        contentWidth: root.width * 0.4
        contentHeight: root.height

        TreeView {
            id: tree
            anchors.fill: parent
            model: jsonModel

            delegate: TreeViewDelegate {
                id: delegate
                text: model.display || ""
                implicitWidth: Math.max(200, 200 + leftPadding + rightPadding)

                highlighted: hovered || selected
            }
        }
    }
}
