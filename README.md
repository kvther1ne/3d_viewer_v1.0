# 3d_viewer_v1.0

## Implementation of 3D Viewer v1.0.

In this project our team implemented a program to view 3D wireframe models (3D Viewer) in the C programming language. The models themselves can be loaded from .obj files and are viewable on the screen with the ability to rotate, scale and translate.

.Obj file is a geometry definition file format first developed by Wavefront Technologies. The file format is open and accepted by many 3D graphics application vendors.
The .obj file format is a simple data format that represents only three-dimensional geometry, specifically the position of each vertex, the UV of each texture coordinate vertex, vertex normals and the faces that define each polygon as a list of vertices and texture vertices. Obj coordinates have no units, but Obj files can contain scale information in a readable comment line.

Affine transformations section describes basic affine transformations (translation, rotation, scaling) in the plane, using two-dimensional objects (images) as an example. Similarly, affine transformations can also be used for three-dimensional space.
An affine transformation is a mapping in a plane or space onto itself in which parallel lines transforms into parallel lines, intersecting lines into intersecting lines, and skew lines into skew lines.
A transformation in a plane is called affine if it is mutually unambiguous and the image of any line is a line. A transformation (mapping) is called mutually unambiguous (bijective) if it converts different points to different ones, and each point is converted to a point.

The 3d Viewer v1.0 program implemented:
- The program built with Makefile which contains standard set of targets for GNU-programs: all, install, uninstall, clean, dvi, dist, tests, gcov
- Prepared full coverage of modules related to calculating expressions with unit-tests using the Check library
- GUI implementation, based on Qt
- The graphical user interface contains: A button to select the model file and a field to output its name; a visualisation area for the wireframe model; button/buttons and input fields for translating the model; button/buttons and input fields for rotating the model; button/buttons and input fields for scaling the model; information about the uploaded model - file name, number of vertices and edges.
- The program correctly processes and allows user to view models with details up to 100, 1000, 10,000, 100,000, 1,000,000  vertices without freezing (a freeze is an interface inactivity of more than 0.5 seconds)
- The program allows customizing the type of projection (parallel and central)
- The program allows setting up the type (solid, dashed), color and thickness of the edges, display method (none, circle, square), color and size of the vertices
- The program allows choosing the background color
- Settings can be saved between program restarts
- The program allows saving the captured (rendered) images as bmp and jpeg files
- The program allows recording small screencasts by a special button - the current custom affine transformation of the loaded object into gif-animation (640x480, 10fps, 5s)
