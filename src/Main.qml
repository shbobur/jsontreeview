import QtQuick
import QtQuick.Controls

pragma ComponentBehavior: Bound

Window {
    id: root
    width: 680
    height: 480
    visible: true
    title: qsTr("JSON TreeView")
    color: "#f5f5f5"

    Rectangle {
        id: treeContainer
        width: Math.min(root.width * 0.3, 200)
        height: root.height

        ScrollView {
            id: scrollView
            anchors.fill: parent
            anchors.margins: 1
            
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.vertical.visible: true
            ScrollBar.vertical.interactive: true
            ScrollBar.vertical.width: 8
            
            ScrollBar.horizontal.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.visible: true
            ScrollBar.horizontal.interactive: true
            ScrollBar.horizontal.height: 8

            background: Rectangle {
                color: "white"
            }

            TreeView {
                id: tree
                width: scrollView.width
                model: jsonModel
                clip: true

                editTriggers: TreeView.DoubleClicked | TreeView.EditKeyPressed

                delegate: Rectangle {
                    id: delegate
                    anchors.leftMargin: 5
                    implicitWidth: parent.width
                    implicitHeight: 30
                    property bool selected: false
                    color: selected ? "#e0e0e0" : (contentArea.containsMouse ? "#20000000" : "transparent")
                    
                    readonly property real indentation: 20
                    readonly property real padding: 5

                    required property bool hasChildren
                    required property var model
                    required property bool editing
                    required property int depth
                    required property int row
                    required property int column

                    // Expand/collapse indicator
                    Label {
                        id: indicator
                        x: padding + (depth * indentation)
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 5
                        visible: hasChildren
                        text: tree.isExpanded(row) ? "▼" : "▶"
                        width: 20

                        TapHandler {
                            onSingleTapped: {
                                if (hasChildren) {
                                    tree.toggleExpanded(delegate.row)
                                }
                            }
                        }
                    }

                    Item {
                        id: contentArea
                        anchors {
                            left: indicator.right
                            right: parent.right
                            top: parent.top
                            bottom: parent.bottom
                            leftMargin: 5
                        }
                        
                        property bool containsMouse: false

                        Text {
                            id: displayText
                            anchors.fill: parent
                            text: model.display || ""
                            visible: !editing
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }

                        TextField {
                            id: editField
                            anchors.fill: parent
                            visible: editing && !hasChildren
                            text: model.value || ""
                            verticalAlignment: TextField.AlignVCenter
                            onAccepted: {
                                tree.model.setData(tree.index(delegate.row, delegate.column), text, Qt.EditRole)
                                editing = false
                            }
                            onEditingFinished: {
                                editing = false
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            enabled: !editing
                            hoverEnabled: true

                            onClicked: function (mouse) {
                                // Deselect all other items
                                let items = tree.contentItem.children
                                for (let i = 0; i < items.length; i++) {
                                    if (items[i].selected) {
                                        items[i].selected = false
                                    }
                                }
                                // Select this item
                                delegate.selected = true
                                
                                if (model.value === null) {
                                    valueDisplay.text = "null"
                                } else if (model.value === true || model.value === false) {
                                    valueDisplay.text = model.value.toString()
                                } else {
                                    valueDisplay.text = model.value || ""
                                }
                            }

                            onEntered: contentArea.containsMouse = true
                            onExited: contentArea.containsMouse = false

                            onDoubleClicked: function (mouse) {
                                if (!hasChildren) {
                                    editing = true
                                    editField.forceActiveFocus()
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: valueContainer
        anchors {
            top: parent.top
            right: parent.right
            left: treeContainer.right
            bottom: parent.bottom
            margins: 10
        }
        color: "white"
        radius: 4

        Row {
            anchors {
                top: parent.top
                right: parent.right
                left: parent.left
                margins: 10
            }
            spacing: 10

            Label {
                id: label
                text: "Clicked value: "
                anchors.verticalCenter: parent.verticalCenter
            }

            Rectangle {
                height: 30
                width: parent.width - label.width - parent.spacing
                color: "#f0f0f0"
                radius: 4

                Text {
                    id: valueDisplay
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
}
