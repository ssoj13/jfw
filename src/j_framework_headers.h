#ifndef _j_framework_headers
#define _j_framework_headers

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <float.h>
#include <iostream>
#include <math.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>

#include <time.h>
#include <vector>
#include <set>

#include <maya/M3dView.h>
#include <maya/MAnimControl.h>
#include <maya/MArgList.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MBoundingBox.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MDagModifier.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnCamera.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnLattice.h>
#include <maya/MFnLatticeData.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnNurbsCurveData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnNurbsSurfaceData.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MFnTransform.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MItCurveCV.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshEdge.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItSurfaceCV.h>
#include <maya/MMatrix.h>
#include <maya/MMeshIntersector.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MProgressWindow.h>
#include <maya/MPxCommand.h>
#include <maya/MPxGeometryFilter.h>
#include <maya/MPxNode.h>
#include <maya/MRampAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MSelectionList.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MVector.h>

// Glm
#define GLM_FORCE_RADIANS 
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> // lookAt, glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/norm.hpp>		// glm::length2(vec3)

// OpenGL lib
#include <GL/gl.h>
// Noise lib
#include <simplexnoise.h>
//#include <fastnoise.h>

// https://stackoverflow.com/questions/5004858/why-is-stdmin-failing-when-windows-h-is-included
#define NOMINMAX
#include <windows.h>

#endif
