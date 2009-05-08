""" The geometry module provides classes to deal with geometric primiteves, like point, line, ring, polyline, polygon """
import math
class Point:
    """ Represents a point"""
    def __init__(self,x=0.0,y=0.0):
        self.x=x
        self.y=y
    def Center(self):
        return self
    def distance(self,point):
        return (self-point).norm()
    def __add__(self,left):
        if hasattr(left, 'x') and hasattr(left, 'y'):
            return Point(self.x+left.x,self.y+left.y)
        else:
            return Point(self.x+left,self.y+left)
    def __sub__(self,left):
        if hasattr(left, 'x') and hasattr(left, 'y'):
            return Point(self.x-left.x,self.y-left.y)
        else:
            return Point(self.x-left,self.y-left)
    def __mul__(self,left):
        if hasattr(left, 'x') and hasattr(left, 'y'):
            return Point(self.x*left.x,self.y*left.y)
        else:
            return Point(self.x*left,self.y*left)
    def __iadd__(self,left):
        if hasattr(left, 'x') and hasattr(left, 'y'):
            self.x+=left.x
            self.y+=left.y
        else:
            self.x+=left
            self.y+=right
        return self
    def __isub__(self,left):
        if hasattr(left, 'x') and hasattr(left, 'y'):
            self.x-=left.x
            self.y-=left.y
        else:
            self.x-=left
            self.y-=right
        return self
    def __imul__(self,left):
        if hasattr(left, 'x') and hasattr(left, 'y'):
            self.x*=left.x
            self.y*=left.y
        else:
            self.x*=left
            self.y*=right
        return self
    def __eq__(self,left):
        if hasattr(left, 'x') and hasattr(left, 'y'):
            return self.x==left.x and self.y==left.y
        else:
            return False
    def __str__(self):
        return "POINT(%f %f)" % (self.x,self.y)
    def norm(self):
        return math.sqrt(self.x**2+self.y**2)
    def __getitem__(self,index):
        if index<0: index=2-index
        if index==0 :
            return self.x
        elif index==1 :
            return self.y
        else:
            raise IndexError("For points only indices 0 and 1 are valid")
    def __setitem__(self,index,value):
        if index<0: index=2-index
        if index==0 :
            self.x=value
        elif index==1 :
            self.y=value
        else:
            raise IndexError("For points only indices 0 and 1 are valid")
    def __len__(self):
        return 2
class PointZM(Point):
    def __init__(self,x=0.0,y=0.0,z=0.0,m=0.0):
        self.x=x
        self.y=y
        self.z=z 
        self.m=m 
    def __getitem__(self,index):
        if index<0: index=3-index
        if index==0 :
            return self.x
        elif index==1 :
            return self.y
        elif index==2 :
            return self.z
        elif index==3 :
            return self.m
        else:
            raise IndexError("For points with z data only indices 0 to 2 are valid")
    def __setitem__(self,index,value):
        if index<0: index=2-index
        if index==0 :
            self.x=value
        elif index==1 :
            self.y=value
        elif index==2 :
            self.z=value
        elif index==3 :
            self.m=value
        else:
            raise IndexError("For points only indices 0 and 1 are valid")
    def __len__(self):
        return 4

def interpolate_between(points,distance):
    res_points=[]
    for i,p in enumerate(points[:-1]):
        start=p
        end=points[i+1]
        dist=(end-start).norm()
        steps=int(dist/distance)
        for j in range(1,steps):
            new_p=Point()
            for dim in range(len(new_p)):
                new_p[dim]=start[dim]+(end[dim]-start[dim])*float(j)/steps
            res_points.append(new_p)
    return res_points
class MultiPoint:
    """ Represents a cloud of points"""
    @classmethod
    def FromPoints(cls,pointsequence):
        if not pointsequence: return None
        ret=cls(BoundingBox.FromPoints(pointsequence))
        for p in pointsequence:
            l.AddPoint(p)
        ret.parts.append(l)
        return ret

    def __init__(self,bbox):
        self.bbox=bbox
        self.points=[]
    def __getitem__(self,index):
        return self.points[index]
    def __setitem__(self,index,point):
        self.points[index]=point
    def __iter__(self):
        return iter(self.points)
    def AddPoint(self,point):
        self.points.append(point)
    def Center(self):
        x=0
        y=0
        s=len(self.points)
        for p in self.points:
            x+=p.x/float(s)
            y+=p.y/float(s)
        return Point(x,y)
    def __contains___(self,point):
        return point in self.points
class BoundingBox:
    @classmethod
    def FromPoints(cls,pointsequence):
        if not pointsequence: return None
        xmin,ymin,xmax,ymax=pointsequence[0].x,pointsequence[0].y,pointsequence[0].x,pointsequence[0].y
        for p in pointsequence:
            xmin=min(p.x,xmin)
            xmax=max(p.x,xmax)
            ymin=min(p.y,ymin)
            ymax=max(p.y,ymax)
        return BoundingBox((xmin,ymin,xmax,ymax))

    def __init__(self,coords):
        self.x1,self.y1=(min(coords[0],coords[2]),min(coords[1],coords[3]))
        self.x2,self.y2=(max(coords[0],coords[2]),max(coords[1],coords[3]))
    def tuple(self):
        return (self.x1,self.y1,self.x2,self.y2)
    def __getitem__(self,index):
        return self.tuple()[index]                
    def Area(self):
        return (self.x2-self.x1)*(self.y2-self.y1)
    def __contains__(self,point):
        return point.x>=self.x1 and point.x<self.x2 and point.y>=self.y1 and point.y<self.y2                   
class Line:
    """ A simple line, represents a part of a polyline or a ring of a polygon """
    def __init__(self,*params):
        self.points=[]
        for p in params:
            if type(p)==Point:
                self.points.append(p)
    def AddPoint(self,point):
        """ Adds a point to the line """
        self.points.append(point)
    def AddXY(self,x,y):
        """ Adds a point with the coordinates x,y to the line """
        try:
            lx=len(x)
            ly=len(y)
            if lx!=ly: raise TypeError()
            for i in range(lx):
                self.points.append(Point(x[i],y[i]))
        except:
            self.points.append(Point(x,y))
            return
    def __getitem__(self,index):
        return self.points[index]
    def __setitem__(self,index,point):
        self.points[index]=point
    def __iter__(self):
        return iter(self.points)
    def asList(self):
        return [(p.x,p.y) for p in self.points]
    def Length(self):
        """ Length of the line """
        length=0.0
        for i,p in enumerate(self.points[:-1]):
            length+=p.distance(self.points[i+1])
        return length 
    def IsRing(self):
        """ Returns true if the line is a ring (= first point equals last point) """
        return self.points[0]==self.points[-1]
    def Center(self):
        """ Returns the centroid of the Ring if IsRing()=True, else returns the point at the midpoint of the line always on the line) """
        # If line is a ring, find centroid of the ring
        if self.IsRing():
            A=self.__signedarea()
            p_res=Point()
            for i,p in enumerate(self.points[:-1]):
                p1=self.points[i+1]
                p_res.x+=(p.x+p1.x)*(p.x*p1.y-p1.x*p.y)/(6*A)
                p_res.y+=(p.y+p1.y)*(p.x*p1.y-p1.x*p.y)/(6*A)
            return p_res
        else: # Find position at the half length on the line
            lmax=self.Length()/2.
            l=0
            i=0
            lastl=0
            while l<lmax:
                lastl=l
                l+=self.points[i].distance(self.points[i+1])
            return self.points[i+1]*(lmax-lastl)+self.points[i]*(l-lmax)
    def __signedarea(self):
        area=0.0
        for i,p in enumerate(self.points[:-1]):
           p1=self.points[i+1]
           area+=p.x*p1.y-p1.x*p.y
        return area*0.5
    def Area(self):
        return abs(self.__signedarea())
    def __contains__(self,p):
        if not self.IsRing(): return false
        c=False
        for j in range(len(self.points)-1):
            i=j+1
            pi=self.points[i]
            pj=self.points[j]
            if ( ( ((pi.y<=p.y) and (p.y<pj.y)) or ((pj.y<=p.y) and (p.y<pi.y)) ) and (p.x<(pj.x-pi.x) * (p.y-pi.y) / (pj.y-pi.y) + pi.x) ) :
                c=not c
        return c
        
class PolyLine:
    """ A shape file line feature, may consist of serveral disconnected lines """
    @classmethod
    def FromPoints(cls,pointsequence):
        if not pointsequence: return None
        ret=cls(BoundingBox.FromPoints(pointsequence))
        l=Line()
        for p in pointsequence:
            l.AddPoint(p)
        ret.parts.append(l)
        return ret
    
    @classmethod
    def FromArray(cls,a):
        points=[Point(t[0],t[1]) for t in a]
        return Polygon.FromPoints(points)
    def __init__(self,bbox):
        self.parts=[]
        self.bbox=bbox
    def Length(self):
        length=0.0
        for p in self.parts:
            length+=p.Length()
        return length
    def Center(self):
        c=Point()
        for p in self.parts:
            c+=p.Center()*1./len(self.parts)
        return c
class Polygon:
    """ A polygon, may consist of serveral disconnetected or containing (not intersecting!) parts. 

    A clean polygon is always ordered clockwise for outer rings and counterclockwise for inner (empty) regions.
    """
    @classmethod
    def FromPoints(cls,pointsequence):
        if not pointsequence: return None
        ret=cls(BoundingBox.FromPoints(pointsequence))
        l=Line()
        for p in pointsequence:
            l.AddPoint(p)
        if not l.IsRing():
            l.AddPoint(pointsequence[0])
        ret.parts.append(l)
        return ret
    def __init__(self,bbox):
        self.parts=[]
        self.bbox=bbox
    def Center(self):
        """ Returns the centroid of the polygon (area weighted mean of the centroid of the parts"""
        c=Point()
        a=self.Area()
        for p in self.parts:
            c+=p.Center()*p.Area()/a
        return c
    def Area(self):
        a=0.0
        for p in self.parts:
            a+=p.Area()
        return a
    def __contains__(self,point):
        if self.bbox and not point in self.bbox:
            return False   
        res=False
        for ring in self.parts:
            if point in ring:
                res=not res
        return res
