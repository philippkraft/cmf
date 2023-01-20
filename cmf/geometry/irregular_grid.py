import numpy as np
import scipy.spatial as scsp
from shapely.geometry import Polygon

from .. import project as Project
from . import create_cell, mesh_project

def delaunay_cells(project: Project, x, y, z):
    """
    Creates a mesh using delaunay triangles between the given coordinates as cells.
    cell.x, cell.y and cell.z represent the centroid of the cell. This results in loss of information.
    In most cases one would use the voronoi_cells function

    x, y and z must have the same length

    :param project:
    :param x: sequence of x coordinates
    :param y: sequence of y coordinates
    :param z: sequence of heights
    :return: list of cells
    """
    points = np.array([x, y]).T
    tri = scsp.Delaunay(points)

    def get_edge_length(tri: scsp.Delaunay, tri1: int, tri2: int):
        try:
            n1, n2 = set(tri.simplices[tri1]) & set(tri.simplices[tri2])
        except ValueError:
            raise ValueError(
                f'Triangle {tri1}:{tri.simplices[tri1]} and {tri2}:{tri.simplices[tri2]} do not share an edge')
        c1, c2 = tri.points[[n1, n2]]
        return np.sqrt(((c1 - c2) ** 2).sum())

    def get_polygons(tri: scsp.Delaunay):

        return [
            Polygon(tri.points[s])
            for s in tri.simplices
        ]

    def get_heights(tri: scsp.Delaunay, z):
        return z[tri.simplices].mean(1)

    polys = get_polygons(tri)
    z_cell = get_heights(tri, z)

    cells = [
        create_cell(project, polygon, height)
        for polygon, height in zip(polys, z_cell)
    ]

    for i, c in enumerate(cells):
        for n in tri.neighbors[i]:
            if n >= 0:
                l = get_edge_length(tri, i, n)
                c.topology.AddNeighbor(cells[n], l)

    return cells


# %%
def __voronoi_finite_polygons_2d(vor, radius=None):
    """
    Reconstruct infinite voronoi regions in a 2D diagram to finite
    regions.

    Parameters
    ----------
    vor : Voronoi
        Input diagram
    radius : float, optional
        Distance to 'points at infinity'.

    Returns
    -------
    regions : list of tuples
        Indices of vertices in each revised Voronoi regions.
    vertices : list of tuples
        Coordinates for revised Voronoi vertices. Same as coordinates
        of input vertices, with 'points at infinity' appended to the
        end.

    """

    if vor.points.shape[1] != 2:
        raise ValueError("Requires 2D input")

    new_regions = []
    new_vertices = vor.vertices.tolist()

    center = vor.points.mean(axis=0)
    if radius is None:
        radius = vor.points.ptp(axis=0).max()

    # Construct a map containing all ridges for a given point
    all_ridges = {}
    for (p1, p2), (v1, v2) in zip(vor.ridge_points, vor.ridge_vertices):
        all_ridges.setdefault(p1, []).append((p2, v1, v2))
        all_ridges.setdefault(p2, []).append((p1, v1, v2))

    # Reconstruct infinite regions
    for p1, region in enumerate(vor.point_region):
        vertices = vor.regions[region]

        if all(v >= 0 for v in vertices):
            # finite region
            new_regions.append(vertices)
            continue
        elif p1 not in all_ridges:
            continue
        # reconstruct a non-finite region
        ridges = all_ridges[p1]
        new_region = [v for v in vertices if v >= 0]

        for p2, v1, v2 in ridges:
            if v2 < 0:
                v1, v2 = v2, v1
            if v1 >= 0:
                # finite ridge: already in the region
                continue

            # Compute the missing endpoint of an infinite ridge

            t = vor.points[p2] - vor.points[p1]  # tangent
            t /= np.linalg.norm(t)
            n = np.array([-t[1], t[0]])  # normal

            midpoint = vor.points[[p1, p2]].mean(axis=0)
            direction = np.sign(np.dot(midpoint - center, n)) * n
            far_point = vor.vertices[v2] + direction * radius

            new_region.append(len(new_vertices))
            new_vertices.append(far_point.tolist())

        # sort region counterclockwise
        vs = np.asarray([new_vertices[v] for v in new_region])
        c = vs.mean(axis=0)
        angles = np.arctan2(vs[:, 1] - c[1], vs[:, 0] - c[0])
        new_region = np.array(new_region)[np.argsort(angles)]

        # finish
        new_regions.append(new_region.tolist())

    return new_regions, np.asarray(new_vertices)

def voronoi_polygons(x, y, buffer=0.0, mask=None):
    """
    Creates closed voronoi polygons for x and y coordinates. With voronoi_cells, cmf cells are easily created
    from x, y and z coordinates. Use this function directly only, if you need to process the voronoi polygons further,
    eg. with unions of landuse or soil maps.

    If this throws an unclear error, make sure you do not have any duplicate coordinates

    :param x: a sequence of x values
    :param y: a sequence of y values
    :param buffer: a float indicating the buffer around the hull. Expressed as a fraction of the sqrt of the hull's area
    :param mask: a polygon to use as a mask. If given, the convex hull is ignored
    :return: A list of polygons
    """
    points = np.array([x, y]).T
    vor = scsp.Voronoi(points)
    if not mask:
        hull = Polygon(points[scsp.ConvexHull(points).vertices])
        mask = hull.buffer(buffer * hull.area ** 0.5)
    regions, vertices = __voronoi_finite_polygons_2d(vor)
    return [Polygon(vertices[r]).intersection(mask) for r in regions]


def voronoi_cells(project: Project, x, y, z, buffer=0.0, mask=None):
    """
    Creates for each (x, y, z) coordinate a voronoi (nearest neighbor) cell.
    The cells are cut of at a buffer around the convex hull of the point or using a mask.

    For each coordinate a cell is created.

    If this throws an unclear error, make sure you do not have any duplicate coordinates

    :param project: The cmf project
    :param x: a sequence of x values
    :param y: a sequence of x values
    :param z: a sequence of x values
    :param buffer: a float indicating the buffer around the hull. Expressed as a fraction of the sqrt of the hull's area
    :param mask: a polygon to use as a mask. If given, the convex hull is ignored
    :return: list of cells
    """
    def make_cell(project, x, y, z, shape, Id=None, with_surfacewater=False):
        cell = project.NewCell(x, y, z, shape.area, with_surfacewater=with_surfacewater)
        cell.geometry = shape
        if Id is not None:
            cell.Id = Id
        return cell

    vpolys = voronoi_polygons(x, y, z, buffer, mask)
    cells = [
        make_cell(project, x[i], y[i], z[i], vpolys[i], Id=i, with_surfacewater=True)
        for i in range(len(x))
    ]

    mesh_project(project)

    return cells

