"""Creates a delauney triangulation and voronoi polygons from a set of x,y,z coordinates
The topology of the nodes and the triangles is calculated, inclusive the width of the connections

Usage:
Triangles=triangulator(x,y,z) # x,y,z are sequences of the coordinates
nodes=Triangles.Nodes() # returns the nodes of the triangulation
voronoipoly=nodes[0].voronoi() # returns a list of nodes setting up the polygon 
"""
# Imports
try:
    from delaunay.triangulate import Triangulation
except:
    raise ImportError("You need to have delaunay.triangulate installed from the scipy sandbox. A Windows binary can be found at the same source as this tool")
try:
    from numpy import *
except:
    raise ImportError("Please install NumPy first")
from math import *
from geometry import *

# Internal functions
def azimuth(p1,p2):
    """ Returns the azimuth of vector spanned by two points in degrees """ 
    azi=atan2(p2[0]-p1[0],p2[1]-p1[1])*180/pi
    if azi<0: return 360+azi
    else: return azi
def __distance(p1,p2):
    """ Calculates the distance between two point """
    return sqrt((p1[0]-p2[0])**2+(p1[1]-p2[1])**2)
class Edge:
    """ The edge of triangle
    Attributes:
        start  - The node where the edge starts
        end    - The node where the edge ends
        left   - The Triangle to the lefthand side of the edge (None for edge triangles)
        right  - The triangle to the righthand side of the edge
        length - The length of the edge"""
    def __init__(self,node1,node2,leftTriangle,rightTriangle):
        self.start=node1
        self.end=node2
        self.left=leftTriangle
        self.right=rightTriangle
        self.length=__distance(length.circumcenter, rightTriangle.circumcenter)
    def AsLine(self):
        l=Line(self.start.AsPoint(),self.end.AsPoint())

class NodeTriangleConnection:
    """ Defines the topological relation between a node and one of the triangles it belongs to, for internal use only """
    def __init__(self,node,triangle):
        # The node
        self.node=node
        # The traingle
        self.triangle=triangle
        # The circumcenter of the triangle
        self.triangleCenter=triangle.circumcenter
        # The azimuth of the line Node - circumcenter
        self.azimuth=azimuth(self.node, self.triangleCenter)
    def __cmp__(self,other):
        return cmp(self.azimuth,other.azimuth)

class Node:
    """ A node of the network
    Usage Node(index,x,y,z,neighbors=[])
    Attributes:
        index - Internal number of the Node
        x     - X coordinate of the Node
        y     - Y coordinate of the Node
        z     - Height of the Node
        neighbor_ids - List of the id's of the connected nodes
        neighbors    - List of the instances of the connected nodes (Populated after calling sortneighbors)
        triangles    - Set of triangles this Node is part of (must be set externally, is done by triangulator.__init__
    list(Node) returns [x,y,z] -> Sequence interface
    """

    def AsPoint(self):
        return Point(self.x,self.y)
    def __init__(self,index,x,y,z,neighbors=[]):
        self.index=index
        self.x=x
        self.y=y
        self.z=z
        self.neighbor_ids=neighbors
        self.triangles=set()
        self.neighbors=None
    def __azimuth_compare(self,neighbor1,neighbor2):
        """ Compare function to sort the neighbors by azimuth, for internal use only """
        return cmp(azimuth(self, neighbor1),azimuth(self, neighbor2))
    def sortneighbors(self,nodes):
        """ Populates the list of neighbor nodes and sorts them azimuth, for the creation of voronoi polygons """
        self.neighbors=[nodes[i] for i in self.neighbor_ids]
        self.neighbors.sort(self.__azimuth_compare)
    def __connections(self):
        """ Returns a list of connections to triangles, for internal use only """
        connections=[NodeTriangleConnection(self,t) for t in self.triangles]
        connections.sort()
        return connections
    def neighbor_mean_height(self):
        if not self.neighbors:
            return None
        h_per_d=0   # height per inverse distance 
        id_sum=0 # sum of distances m
        for n in self.neighbors:
            d=self.distanceTo(n)
            if d:
                h=n.z
                id_sum+=1/d
                h_per_d+=h/d
        return h_per_d/id_sum
        
    def planar_displacement_error(self):
        """ Calculates the inverse distance weighted mean height (idwm) of all neighboring nodes and returns abs(idwm-z) """
        if not self.neighbors:
            return None
        h_per_id=0   # height per inverse distance 
        id_sum=0 # sum of distances m
        for n in self.neighbors:
            d=self.distanceTo(n)
            if d:
                h=n.z
                id_sum+=1/d
                h_per_id+=h/d
        return abs(h_per_id/id_sum-self.z)
    def voronoi(self,closed=False):
        """ Returns the voronoi polygon of this node, the polygon is given as a list of nodes, representing the vertices of the polygon
        If closed=True (not default) the last vertex equals the first vertex"""
        connections=self.__connections()
        points=[tuple(c.triangleCenter) for c in connections]
        if closed:
            points.append(tuple(connections[0].triangleCenter))        
        return array(points)
    def voronoiPolygon(self):
        connections=self.__connections()
        points=[Point(c.triangleCenter[0],c.triangleCenter[1]) for c in connections]
        return Polygon.FromPoints(points)
        
    def edges(self):
        """ Returns a list of edges, sharing this node 
        This method can be used after the list of neighbor nodes is created and sorted by sortneighbors()"""
        connections=self.connections()
        result=[]
        if self.neighbors==None: raise RuntimeError("Need to sort the neighbors first")
        for n in self.neighbors:
            edgeTris=[]
            for c in connections:
                if n in c.triangle:
                    edgeTris.append(c.triangle)
            if len(edgeTris)==2:
                result.append(Edge(self,n,edgeTris[0],edgeTris[1]))        
        return result
    def __getitem__(self,index):
        """ Returns x,y or z for 0,1,2 as index, useful for the creation of tuples, lists or arrays """
        if index==0 or index==-3:
            return self.x
        elif index==1 or index==-2:
            return self.y
        elif index==2 or index==-1:
            return self.z
        else:
            raise IndexError('Only indices 0..2 are supported')
    def distanceTo(self,node):
        """ Returns the euclidian distance to another node """
        return sqrt((self.x-node.x)**2+(self.y-node.y)**2)
    def __str__(self):
        return '(%g,%g,%g)[%i]' % (self.x,self.y,self.z,self.index)
class Neighbor:
    """ A Neighborhood relation between two triangles
    Attributes:
        owner - The triangle, whose neighbor is defined in this class
        target - The triangle, that is the neighbor
        width - the width of the shared line
    """
    def __init__(self,fromTriangle,toTriangle):
        self.owner=fromTriangle
        self.target=toTriangle
        sharedNodes=[]
        for n in self.owner.nodes:
            if n in self.target.nodes:
                sharedNodes.append(n)
        if len(sharedNodes)==2:
            self.width=sharedNodes[0].distanceTo(sharedNodes[1])
        elif len(sharedNodes)<2:
            raise RuntimeError("Creating neighbor: Less than 2 matching nodes found, Triangles are no neighbors")
        elif len(sharedNodes)>2:
            raise RuntimeError("Creating neighbor: More than 2 matching nodes found, Triangles are equal")
        
class Triangle:
    """ A delauney triangle 
    Attributes:
        index - Index number of the Triangle
        nodes - List of the three nodes setting up the triangle
        neighbors - List of Neighbor instances
        circumcenter - circumcenter of the triangle, is set by triangulator.__init__ """
    def __init__(self,index,node1,node2,node3,circumcenter=None):
        self.index=index
        self.nodes=(node1,node2,node3)
        self.neighbors=[]
        self.circumcenter=circumcenter
    def AsRing(self):
        return Line(self.nodes[0],self.nodes[1],self.nodes[2],self.nodes[0])
    def AddNeighbor(self,triangle):
        """Adds a connected triangle"""
        try:
            n=Neighbor(self,triangle)
            self.neighbors.append(n)
            return n
        except:
            return None
    def area(self):
        """ Returns the area of the triangle """
        p,q,r=self.nodes
        ux = q.x-p.x
        uy = q.y-p.y
        vx = r.x-p.x
        vy = r.y-p.y
        return (ux*vy-uy*vx)/2.
    def centroid(self):
        """ Returns the centroid (weight center) of the triangle"""
        x=0
        y=0
        z=0
        for n in self.nodes:
            x+=n.x/3.
            y+=n.y/3.
            z+=n.z/3.
        return (x,y,z)
    def centroidPoint(self):
        """ Returns the centroid (weight center) of the triangle as point"""
        x=0
        y=0
        for n in self.nodes:
            x+=n.x/3.
            y+=n.y/3.
        return Point(x,y)
    def AsPolygon(self):
        return Polygon.FromPoints([n.AsPoint() for n in self.nodes])   
    def coordinates(self,closed=False):
        """ Returns the coordinates of the triangle as numpy.array, if closed=True the first vertex is copied as the last vertex """
        loc=[]
        for n in self.nodes:
            loc.append(tuple(n))
        if closed:
            loc.append(tuple(self.nodes[0]))
        return array(loc)
    def __str__(self):
        return '%i:[%i,%i,%i]' % (self.index,self.nodes[0].index,self.nodes[1].index,self.nodes[2].index)
    def __contains__(self,node):
        """ Returns true if node is one of the vertices """
        return node in self.nodes
class triangulator:
    """ Creates a trinagulation from a set of coordinates.
    The triangulation contains a fully connected list of nodes and a fully connected list of triangles
    locations : Sequences of 3-tuple of floats with coordinates values
    boundary : A geometry.Polygon representing the outer boundary   
"""
    def __init__(self,locations,boundary=None):
        print "Triangulate..."
        
        # Remove duplicate points, by creating a dictionary of locations 
        loc_dic={}
        for t in locations: loc_dic[tuple(t[:2])]=t
        loc=array(loc_dic.values())
        # Do triangulation
        self.triangulator=Triangulation(loc[:,0], loc[:,1])
        # Set used coordinates
        self.x=loc[:,0]
        self.y=loc[:,1]
        self.z=loc[:,2]
        
        # Get the connectivity of the nodes
        node_graph=self.triangulator.node_graph()
        self.nodes=[Node(i,self.x[i],self.y[i],self.z[i],node_graph[i]) for i in node_graph]
        
        # Create the triangles      
        self.triangles=[]
        bad_triangles=set()
        print "Create triangles..."
        for i,nn in enumerate(self.triangulator.triangle_nodes):
            t=Triangle(i,self.nodes[nn[0]],self.nodes[nn[1]],self.nodes[nn[2]],self.triangulator.circumcenters[i])      
            if (boundary!=None) and (not t.centroidPoint() in boundary):
                bad_triangles.add(t)                            
            self.triangles.append(t)
        print "Connect triangles..."
        for i,neighbors in enumerate(self.triangulator.triangle_neighbors):
            curTri=self.triangles[i]
            for n in neighbors:
                if n>=0 and not self.triangles[n] in bad_triangles:
                    curTri.AddNeighbor(self.triangles[n])
        for t in bad_triangles:
            self.triangles.remove(t)
        print "Connect nodes..."
        for i,triangle in enumerate(self.triangles):
            for n in triangle.nodes:
                n.triangles.add(triangle) 
        for n in self.nodes:
            n.sortneighbors(self.nodes)
    def __getitem__(self,index):
        return self.triangles[index]   
    def __len__(self):
        return len(self.triangles)
    def __iter__(self):
        return iter(self.triangles)             
                    
            
            
        
    