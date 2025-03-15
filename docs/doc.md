# JSON TreeView Implementation Documentation

## Overview
This documentation covers the implementation of a tree view model in C++ with QML integration, specifically designed for handling hierarchical data structures.

## C++ Components

### TreeModel (treemodel.h)
The `TreeModel` class is a custom implementation that inherits from `QAbstractItemModel`, providing a tree-structured data model that can be used with QML views.

#### Key Features
- Single column tree structure
- Parent-child relationship management
- QML integration through Q_INVOKABLE methods

#### Important Methods
- `addTopLevelItem(TreeItem* child)`: Adds items at the root level
- `addItem(TreeItem* parent, TreeItem* child)`: Adds child items to a parent node
- `removeItem(TreeItem* item)`: Removes an item from the tree
- `depth(const QModelIndex& index)`: Returns the depth of a node in the tree
- `clear()`: Removes all items from the tree

#### Model Implementation
The model implements the required virtual methods from QAbstractItemModel:
- `rowCount()`: Returns the number of rows under a given parent
- `columnCount()`: Returns the number of columns for a given parent
- `index()`: Creates a model index for the specified row, column, and parent
- `parent()`: Returns the parent of a given index
- `data()`: Returns the data stored under the given role for the item
- `setData()`: Sets the role data for the item at index

### TreeItem
The TreeItem class represents individual nodes in the tree structure, managing:
- Parent-child relationships
- Item data storage
- Tree manipulation operations

### JsonModel
The JsonModel class is a custom implementation that inherits from `TreeModel`, providing a tree-structured data model specifically designed for JSON data manipulation.

#### Additional Roles
- `ValueRole`: Represents the JSON value of the item
- `KeyRole`: Represents the JSON key/property name

#### Key Methods
- `loadJson(const QString &path)`: Loads and parses JSON data from a file
- `role(const QString &roleName)`: Maps role names to role enums for QML usage
- `setData(const QModelIndex& index, const QVariant& value, int role)`: Updates JSON values, supporting all JSON types:
  - Numbers (integer and floating-point)
  - Strings
  - Booleans
  - Arrays
  - Objects
  - null values

#### Features
- Full JSON data structure support
- Dynamic loading from JSON files
- Two-way data binding with QML
- Editable values through model interface

## QML Usage

### Integrating with QML Views
The model can be used with QML TreeView. The main application provides a rich TreeView implementation with editing capabilities:

```qml
TreeView {
    model: jsonModel
    delegate: TreeViewDelegate {
        text: model.display // Uses the DisplayRole from the C++ model
    }
}
```

### TreeView Implementation Details
The main application implements a full-featured TreeView with a sophisticated delegate structure:

#### Delegate Structure
```qml
delegate: Rectangle {
    // Base container with selection and hover states
    property bool selected: false
    color: selected ? "#e0e0e0" : (contentArea.containsMouse ? "#20000000" : "transparent")
    
    // Important delegate properties. They are set by the TreeView.
    required property bool hasChildren
    required property bool editing
    required property int depth
    required property var model

    // Components

    Label {
        // Expand/Collapse indicator
    }
    
    Text {
        // Display text
    }

    TextField {
        // Edit text
    }
}
```

#### Expand/Collapse Indicator
- Uses a `Label` component that's only visible when `hasChildren` is true
- Toggles between "▼" (expanded) and "▶" (collapsed) symbols
- Position is calculated based on depth for proper indentation:
```qml
Label {
    x: padding + (depth * indentation)
    visible: hasChildren
    text: tree.isExpanded(row) ? "▼" : "▶"
}
```

#### Content Display
The delegate switches between two display modes:
1. Read Mode (`Text` component):
```qml
Text {
    visible: !editing // 
    text: model.display || ""
    ...
}
```

2. Edit Mode (`TextField` component):
```qml
TextField {
    visible: editing && !hasChildren // Visible only when editing
    text: model.value || "" // We edit (JSON) Value, not "Display" text
    ...
}
```

#### Interaction Handling
- MouseArea manages selection and editing:
```qml
MouseArea {
    hoverEnabled: true
    onDoubleClicked: {
        if (!hasChildren) {
            editing = true
            editField.forceActiveFocus()
        }
    }
}
```

#### Value Display Handling
- Special handling for different JSON types in the display component:
```qml
if (model.value === null) { // null handling
    valueDisplay.text = "null"
} else if (model.value === true || model.value === false) { // boolean handling
    valueDisplay.text = model.value.toString()
} else { // string handling
    valueDisplay.text = model.value || ""
}
```

## Notes
- The model is designed for single-column tree structures
- Custom roles can be added by extending the `roleNames()` method
- The implementation is thread-safe for read operations but should be modified from the main thread