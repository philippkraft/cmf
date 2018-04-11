from math import sqrt


class Quadtree:
    """
    A simple quad tree to check if the boundaries of geometries overlap or not
    """
    def iterbounds(self, bounds):
        """
        Iterates through all positions in the boundaries
        :param bounds: The boundary rectangle. A 4 item sequence (xmin, ymin, xmax, ymax)
        :return: (i,j) position tuple
        """
        imin = int(bounds[0] / self.dx)
        jmin = int(bounds[1] / self.dy)
        imax = int(bounds[2] / self.dx)
        jmax = int(bounds[3] / self.dy)
        for i in range(imin, imax + 1):
            for j in range(jmin, jmax + 1):
                yield i, j

    def append(self, obj, bounds):
        """
        Appends an object to the qtree using the given bounds.
        :param obj: An object to append to the tree, must be hashable
        :param bounds: The boundary rectangle of the object. A 4 item sequence (xmin, ymin, xmax, ymax)
        """
        for i, j in self.iterbounds(bounds):
            # Get or create the storage at (i, j)
            stor = self.areas.setdefault((i, j), set())
            stor.add(obj)

    def extend(self, objects_with_boundaries):
        """
        Extends the Quadtree with objects
        :param objects_with_boundaries: an iterable of (object, bounds) tuples
        :return:
        """
        for obj, bounds in objects_with_boundaries:
            self.append(obj, bounds)

    def __call__(self, bounds):
        """
        Get all objects in the given boundaries
        :param bounds: The boundary rectangle of the tree. A 4 item sequence (xmin, ymin, xmax, ymax)
        :return:
        """
        global_objs = set()
        for i, j in self.iterbounds(bounds):
            # get object set at (i,j) may be empty
            local_objs = self.areas.get((i, j), set())
            global_objs.update(local_objs)
        return global_objs

    def __init__(self, area, divisions=50):
        """

        :param area: The total area of the study area in m2
        :param divisions: The number of divisions of the area, default is 20
        """
        scale = sqrt(area)
        self.dx = scale / divisions
        self.dy = scale / divisions
        self.areas = {}
