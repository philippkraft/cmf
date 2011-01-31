import sys, math, random, types, time, thread
from shapely import geometry as geom
from PyQt4 import QtGui, QtCore
from geos_shapereader import shapefile

"""
The QtGui module extends QtCore with GUI functionality.

- QGraphicsView:

    The QGraphicsView class provides a widget for displaying the contents of a 
    QGraphicsScene.

- QGraphicsScene:
    The QGraphicsScene class provides a surface for managing a large number of 
    2D graphical items. The class serves as a container for QGraphicsItems. It 
    is used together with QGraphicsView for visualizing graphical items, such as
    lines, rectangles, text, or even custom items, on a 2D surface. 
    
- QGraphicsItem:
    The QGraphicsItem class is the base class for all graphical items in a 
    QGraphicsScene. Qt provides a set of standard graphics items for the most 
    common shapes. 
    
    These are:
        -QGraphicsEllipseItem provides an ellipse item
        -QGraphicsLineItem provides a line item
        -QGraphicsPathItem provides an arbitrary path item
        -QGraphicsPixmapItem provides a pixmap item
        -QGraphicsPolygonItem provides a polygon item
        -QGraphicsRectItem provides a rectangular item
        -QGraphicsSimpleTextItem provides a simple text label item
        -QGraphicsTextItem provides an advanced text browser item

- QMatrix:
    The QMatrix class specifies 2D transformations of a coordinate system.
    A matrix specifies how to translate, scale, shear or rotate the coordinate 
    system, and is typically used when rendering graphics.
"""
 
class MainFrame(QtGui.QMainWindow):
    """
    The GUI for virtual farm.
    
    Description
    ===========
      The main frame acts as the GUI for virtual farm. It is capable in visualising
      the attributes of the underlying shape file in connection with the model farm 
      operating this patch.
      Therefore the shape object representing the outline of a patch is connected 
      with the model patch over a common identifier. 
    """

    max_Number_of_Child_Items = 0
    
    def __init__(self, features, Model_Run = None):
        """
        Constructor of model_visualisation.GUI_Virtual_Farm.MainFrame
        
        @param features: A list containing object  representation the graphical of a shape object which shall be merged to the model object.
        @type features: List containing shapely objects
        
        @param Model_Run: The model_run contains all the modeled objects and the start function for the model.
        @type Model_Run: model_run.Virtual_Farm_Model_Run.Model_Run
        
        @param Attribute_Name_Field_ID: Common identifier between the modeled object and the shape object.
        @type Attribute_Name_Field_ID: -
        
        @param max_Number_of_Child_Items: The depth of child attributes to be displayed
        @type max_Number_of_Child_Items: Integer
        
        @return: MainFrame
        @rtype: Instance of a model_visualisation.GUI_Virtual_Farm.MainFrame
        """
        
        self.Model_Run = Model_Run
        self.my_model_run = None
        
        self.Dict_of_Colors = {}#holds the QBrush for each unique attribute plotted in current_Map
        
        QtGui.QMainWindow.__init__(self)
 
        self.setWindowTitle("A mapping utility Kraft & Windhorst") # title
        self.resize(1024, 768) # size
        self.setMinimumSize(800, 600) # minimum size
        self.move(0, 0) # position window frame at top left
        
        # Set the central widget for the main window
        central_widget = QtGui.QWidget(self)
        
        self.current_Map = MyMap(features = features)
        
        self.current_Map.scale(0.2, 0.2)
        
        
        layout = QtGui.QVBoxLayout() #vertical box layout
        #layout = QtGui.QHBoxLayout() #horizontal box layout
        
        #GUI controls
    
        #Zoom
        """zoom_slider"""
        self.zoom_slider = QtGui.QSlider()
        self.zoom_slider.setMinimum(0)
        self.zoom_slider.setMaximum(500)
        self.zoom_slider.setValue(250)
        self.zoom_slider.setTickInterval(10)
        self.zoom_slider.setTickPosition(QtGui.QSlider.TicksAbove);
        self.zoom_slider.setOrientation(QtCore.Qt.Horizontal)
        self.connect(self.zoom_slider, QtCore.SIGNAL('valueChanged(int)'), self.set_matrix)
        """zoom_in_button"""
        self.zoom_in_button = QtGui.QPushButton("&Zoom in")
        self.connect(self.zoom_in_button, QtCore.SIGNAL('clicked()'), self.zoom_in)
        """zoom_out_button"""
        self.zoom_out_button = QtGui.QPushButton("&Zoom out")
        self.connect(self.zoom_out_button, QtCore.SIGNAL('clicked()'), self.zoom_out)
        """Layout_zoom"""
        Layout_zoom = QtGui.QHBoxLayout()
        for w in [self.zoom_out_button,self.zoom_slider,self.zoom_in_button]:
            Layout_zoom.addWidget(w)
            Layout_zoom.setAlignment(w, QtCore.Qt.AlignVCenter)
        #Item_Tree
        self.Item_Tree = QtGui.QTreeWidget()
        self.Item_Tree.setMinimumHeight(200)
        #self.Item_Tree.setSelectionMode(QtGui.QAbstractItemView.SingleSelection)
        self.connect(self.Item_Tree,QtCore.SIGNAL("currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)"), self.plot_selected_attribute)
       
        
        #populateTree if selectionChanged
        self.connect(self.current_Map.scene, QtCore.SIGNAL('selectionChanged()'), self.populateTree)
        #Layout_controls
        Layout_Controls = QtGui.QVBoxLayout()
        
        Layout_Controls.addLayout(Layout_zoom)
        Layout_Controls.setAlignment(Layout_zoom, QtCore.Qt.AlignVCenter)
        
        layout.addLayout(Layout_Controls)
        layout.addWidget(self.current_Map)
        layout.addWidget(self.Item_Tree)
        central_widget.setLayout(layout)

        self.setCentralWidget(central_widget)
        
        #Menu
        self.create_menu()
        
    def on_about(self):
        msg = """ MyMap:
        
          is a Basic visualization of shapely.geometries using 
          PyQt4.QtGui.QGraphicsView element
        """
        QtGui.QMessageBox.about(self, "About MyMap", msg.strip())
        
    def save_plot(self):
        file_choices = "PNG (*.png)|*.png"
        
        path = unicode(QtGui.QFileDialog.getSaveFileName(self, 
                        'Save file', '', 
                        file_choices))
        if path:
            
            #qPixmap = QtGui.QPixmap()
            #qPainter = QtGui.QPainter(qPixmap)
            image = QtGui.QImage(2400, 2400, QtGui.QImage.Format_ARGB32_Premultiplied)
            qPainter = QtGui.QPainter(image)
           
            qPainter.setRenderHint(QtGui.QPainter.Antialiasing)

            self.current_Map.render(qPainter) 
            qPainter.end()    
            
            #qPixmap.save(path)
            image.save(path + ".png", "PNG");
            
    def run_Model(self):
        
        if self.my_model_run == None:
            self.my_model_run = thread.start_new(self.Model_Run.start_Model, (), {})
            
        self.Model_Run.pause_Model = False
       
    def pause_Model(self):
        self.Model_Run.pause_Model = True
        
    def create_menu(self):        
        self.file_menu = self.menuBar().addMenu("&File")
        
        load_file_action = self.create_action("&Save plot",
            shortcut="Ctrl+S", slot=self.save_plot, 
            tip="Save the plot")
        quit_action = self.create_action("&Quit", slot=self.close, 
            shortcut="Ctrl+Q", tip="Close the application")
        
        self.add_actions(self.file_menu, 
            (load_file_action, None, quit_action))
        
        self.help_menu = self.menuBar().addMenu("&Help")
        about_action = self.create_action("&About", 
            shortcut='F1', slot=self.on_about, 
            tip='About the demo')
        
        self.add_actions(self.help_menu, (about_action,))
        
        #custom for model
        self.model_menu = self.menuBar().addMenu("&Model")
        run_model_action = self.create_action("&run", 
            shortcut='F2', slot=self.run_Model, 
            tip='Start the model run')
        pause_model_action = self.create_action("&pause", 
            shortcut='F3', slot=self.pause_Model, 
            tip='Pause the model run')
        
        self.add_actions(self.model_menu, 
            (run_model_action, None, pause_model_action))
        
    def add_actions(self, target, actions):
        for action in actions:
            if action is None:
                target.addSeparator()
            else:
                target.addAction(action)

    def create_action(  self, text, slot=None, shortcut=None, 
                        icon=None, tip=None, checkable=False, 
                        signal="triggered()"):
        
        action = QtGui.QAction(text, self)
        if icon is not None:
            action.setIcon(QtGui.QIcon(":/%s.png" % icon))
        if shortcut is not None:
            action.setShortcut(shortcut)
        if tip is not None:
            action.setToolTip(tip)
            action.setStatusTip(tip)
        if slot is not None:
            self.connect(action, QtCore.SIGNAL(signal), slot)
        if checkable:
            action.setCheckable(True)
        return action
        
    def set_matrix(self):         
        scale = math.pow(2,(self.zoom_slider.value() - 250) / 50)
        qm = QtGui.QMatrix() 
        qm.scale(scale,scale)
        #qm.rotate(self.rotate_slider.value())
        self.current_Map.setMatrix(qm)

    def zoom_in(self):
        self.zoom_slider.setValue(self.zoom_slider.value()+50)
       
    def zoom_out(self):
        self.zoom_slider.setValue(self.zoom_slider.value()-50)
    
    def rotate_clockwise(self):
        self.rotate_slider.setValue(self.rotate_slider.value()+10)
          
    def rotate_counterclockwise(self):
        self.rotate_slider.setValue(self.rotate_slider.value()-10)
    
    def clickedTree(self, clickedItem=None):
        print clickedItem
    
    def populateTree(self):
        """Add all attributes and child attributes of an object as (name, value) 
        pairs sorted by name to the QTreeWidget.
        """
        
        def get_attributes(object):
            return [(key,getattr(object,key)) for key in dir(object) \
                          if not (hasattr(getattr(object,key),'__call__') \
                                  or key.startswith('_')) \
                         ]
        def add_property(parent,key,value,recursion_depth=0):
            new_item = QtGui.QTreeWidgetItem(parent,[str(key),str(value)])
            if recursion_depth < self.max_Number_of_Child_Items:
                attr = get_attributes(value)
                for key,value in attr:
                    add_property(new_item, key, value, recursion_depth+1)
                
            
        
        
        selectedItem = self.selectedItem()
        
        self.Item_Tree.clear()
        
        self.Item_Tree.setColumnCount(2)
        self.Item_Tree.setHeaderLabels(["Name", "Value"])
        self.Item_Tree.setItemsExpandable(True)

        if selectedItem != None:
            for key,value in get_attributes(selectedItem.attributes):
                add_property(self.Item_Tree,key,value)
            
        self.Item_Tree.resizeColumnToContents(0)
        self.Item_Tree.resizeColumnToContents(1)
    
      
    def get_Child_Items(self, Parent, List_of_Attributes, Reference = [] ,recursion_depth = 1):
        """Adds all child Items as QTreeWidgetItem attributes to the given Parent.
        A Reference to the position of each attribute in tree structure is added 
        to the QTreeWidgetItem as List [prarent, child, grandchild,...]
        """
        for i_attribute in List_of_Attributes:
            my_Reference = list(Reference)
            my_Reference.append(str(i_attribute[0]))
            List_of_Child_Attributes = self.getattributes(object = i_attribute[1])
            if len(List_of_Child_Attributes)>0:
                if recursion_depth < self.__class__.max_Number_of_Child_Items:
                    parentItem = QtGui.QTreeWidgetItem(Parent, [str(i_attribute[0]),str(i_attribute[1])])
                    parentItem.Reference = my_Reference
                    childItem = self.get_Child_Items(Parent = parentItem, List_of_Attributes = List_of_Child_Attributes, Reference = parentItem.Reference, recursion_depth = recursion_depth + 1)
                else:
                    childItem = QtGui.QTreeWidgetItem(Parent, [str(i_attribute[0]),str(i_attribute[1])])
                    childItem.Reference = my_Reference
            else:  
                childItem = QtGui.QTreeWidgetItem(Parent, [str(i_attribute[0]),str(i_attribute[1])])
                childItem.Reference = my_Reference
                
            childItem.feature = str(i_attribute[0])

        return childItem
                    
    def plot_selected_attribute(self, item, previous):
        if not item:
            return        

        for i_Item in self.current_Map.scene.items():
            current_attribute_refernce = i_Item.feature
            for i_attribute in item.Reference:
                current_attribute_refernce = getattr(current_attribute_refernce,i_attribute)
            
            if isinstance(current_attribute_refernce,list):
                current_attribute_refernce = current_attribute_refernce[0]
            
            my_brush = self.Dict_of_Colors.get(current_attribute_refernce)
                
            if my_brush is None:
                my_brush = QtGui.QBrush(QtGui.QColor(random.random()*255, random.random()*255, random.random()*255))
                self.Dict_of_Colors[current_attribute_refernce] = my_brush
            i_Item.setBrush(my_brush)
        
    def selectedItem(self):
        items = self.current_Map.scene.selectedItems()
        if len(items) == 1:
            return items[0]
        return None

class Feature(QtGui.QGraphicsPathItem):
    """
    my_QGraphicsPolygonItem
    """
    
    """Constructor"""
    def __init__(self, shape, attributes, parent = None, scene = None):
        """
        shape - A shapely geometry (until now only polygons)
        attributes - An object representing the attributes of the shape
        parent As QGraphicsItem
        scene As QGraphicsScene 
        """
        def xy2QPolygon(xyz):
            poly = QtGui.QPolygonF()
            for x,y,z in xyz:
                poly.append(QtCore.QPointF(x,y))
            return poly    
        def shape2QPainterPath(shape):
            path = QtGui.QPainterPath()
            if isinstance(shape, geom.MultiPolygon):
                for poly in shape.geoms:
                    path.addPath(shape2QPainterPath(poly))
            elif isinstance(shape, geom.Polygon):
                path.addPolygon(xy2QPolygon(shape.exterior.coords))
                if shape.interiors:
                    innerpath = QtGui.QPainterPath()
                    for inner in shape.interiors:
                        innerpath.addPolygon(xy2QPolygon(inner.coords))
                    path.substracted(inner)
            else:
                raise NotImplementedError("until now only Polygons are supported")
            return path
        path = shape2QPainterPath(shape)
        self.geometry = shape   
        QtGui.QGraphicsPathItem.__init__(self, path, parent, scene)
        
        self.attributes = attributes #holds all informations from the shape file

        self.setFlag(QtGui.QGraphicsItem.ItemIsSelectable)
        self.setBrush(QtGui.QColor("blue")) 


class MyMap(QtGui.QGraphicsView):
    """
    MyMap
    
    Description:
    ============
        MyMap is a QGraphicsView which converts the given shapes (shapely.geometry)
        to QGraphicsItem and adds them to self.scene (QGraphicsScene) for displaying.
        
        
        QGraphicsView:
        --------------
            - The QGraphicsView class provides a widget for displaying the contents of a 
              QGraphicsScene.

        QGraphicsScene:
        --------------
            - The QGraphicsScene class provides a surface for managing a large number of 
              2D graphical items. The class serves as a container for QGraphicsItems. It 
              is used together with QGraphicsView for visualizing graphical items, such as
              lines, rectangles, text, or even custom items, on a 2D surface. 
            
        QGraphicsItem:
        --------------
            - The QGraphicsItem class is the base class for all graphical items in a 
              QGraphicsScene. Qt provides a set of standard graphics items for the most 
              common shapes. 
            
              These are:
                - QGraphicsEllipseItem provides an ellipse item
                - QGraphicsLineItem provides a line item
                - QGraphicsPathItem provides an arbitrary path item
                - QGraphicsPixmapItem provides a pixmap item
                - QGraphicsPolygonItem provides a polygon item
                - QGraphicsRectItem provides a rectangular item
                - QGraphicsSimpleTextItem provides a simple text label item
                - QGraphicsTextItem provides an advanced text browser item
    
    Parameters:
        - features As List containing shapely.geometry
        
    Attributes:
        - features As List containing shapely.geometry
          shapes As List containing QGraphicsItem
    """
    
    """Constructor"""
    def __init__(self, features = None):
        
        QtGui.QGraphicsView.__init__(self)
       
        self.scene = QtGui.QGraphicsScene(self)
        #self.setBackgroundBrush(QtGui.QColor(117, 125, 132)) #sets the Background color
        self.shapes=[Feature(feature.shape,feature) for feature in features]
        for shape in self.shapes:
            self.scene.addItem(shape)      
        self.setScene(self.scene)
        self.matrix()
              


def main(features, Model_Run, Attribute_Name_Field_ID):  
    app = QtGui.QApplication(sys.argv)
 
    
    frame = MainFrame(features =features)
    frame.show()
 
    exit_code = app.exec_()
    sys.exit(exit_code)

if __name__ == "__main__":
    """main() function
    Within this function, we instantiate a QApplication object, app. 
    QApplication is responsible for various application-wide resources, 
    such as the default font and cursor, and for running an event loop. 
    Hence, there is always one QApplication object in every GUI application 
    using Qt.
    We construct a new MainFrame widget on the heap and invoke its show() 
    function to display it. 
    However, the widget will not be shown until the application's event loop is 
    started. We start the event loop by calling the application's exec() 
    function; the result returned by this function is used as the return value 
    from the main() function.
    """
    app = QtGui.QApplication(sys.argv)
 
    frame = MainFrame(features = [f for f in shapefile("cells.shp") if f.Id>0])
    frame.show()
 
    exit_code = app.exec_()
    sys.exit(exit_code)