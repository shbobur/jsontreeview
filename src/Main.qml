import QtQuick
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("JSON TreeView")

    Flickable {
        anchors.fill: parent
        

        TreeView {
            id: tree
            anchors.fill: parent

            model: treeModel

            delegate: TreeViewDelegate {
                id: delegate
                text: model.display || ""
                implicitWidth: Math.max(100, 100 + leftPadding + rightPadding)
            }
        }
    }

}
