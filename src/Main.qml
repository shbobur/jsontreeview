import QtQuick
import QtQuick.Controls

pragma ComponentBehavior: Bound

Window {
    id: root
    width: 680
    height: 480
    visible: true
    title: qsTr("JSON TreeView")

    Flickable {
        id: treeWidget
        width: root.width * 0.4
        height: root.height
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

                onClicked: {
                    if (model.value === null) {
                        editableText.text = "null"
                    } else if (model.value === true || model.value === false) {
                        editableText.text = model.value.toString()
                    } else {
                        editableText.text = model.value || "<i>(expandable object)</i>"
                    }
                }
            }
        }
    }

    Row {
        anchors {
            top: parent.top
            right: parent.right
            left: treeWidget.right
            topMargin: 20
            leftMargin: 20
            rightMargin: 20
        }
        spacing: 10

        Label {
            id: label
            text: "Clicked value: "
            anchors.verticalCenter: parent.verticalCenter
        }

        Rectangle {
            height: editableText.contentHeight + 10
            width: parent.width - label.width - parent.spacing
            color: "#f0f0f0"  // Light gray
            radius: 4

            Text {
                id: editableText
                anchors {
                    fill: parent
                    margins: 5
                }
                wrapMode: Text.WordWrap
                font.pixelSize: 14
                text: "Select an item to view details"
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
