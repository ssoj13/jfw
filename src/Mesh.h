#ifndef _Mesh
#define _Mesh
#include "j_framework.h"
/*
jmesh
	pt[]
	uv[]
	poly
	poly
	poly
	poly
		point
		point
		point
		point -> [ pts[index], [uvs[index], uvs[index]] ]

*/

// --==== jMesh ====--
class Mesh
{
	public:
	// Constructors
		Mesh() { clear(); }
		Mesh(const Mesh& b) { *this = b; }
		//TODO add both MDataHandle and MObject constructors - first for getting matrix, 2nd for normal mobjects
		Mesh(MDataHandle& b) { import_mesh(b); }
		Mesh(const std::vector<Pt>& pts, const std::vector<int>& counts, const std::vector<int>& connects) { clear(); import_mesh(pts, counts, connects); }
		Mesh(const std::vector<Pt>& pts, const std::vector<Uv>& uvs, const std::vector<Point>& points, const std::vector<Poly>& polys) { clear(); _pts = pts; _uvs = uvs; _polys = polys; }
		void clear() { _pts.clear(); _uvs.clear(); _polys.clear(); _matrix = glm::mat4(1); }

		// pts
		jint	numPts() const { return (jint)_pts.size(); }
		bool	ptid(const jint id) const { if(id>numPts()-1) return false; return true; }
		void	set(const Pt& v, jint id) { if(id>=numPts()) return; _pts[id] = v; }
		void	set(const jreal& xx, const jreal& yy, const jreal& zz, jint id) { if(id>=numPts()) return; _pts[id] = Pt(xx, yy, zz); }
		void	addPt(const Pt& pt) { _pts.push_back(pt); }
		void	addPt(const jreal& xx, const jreal& yy, const jreal& zz) { _pts.push_back(Pt(xx, yy, zz)); }
		void	addPts(const std::vector<Pt>& pts) { _pts.insert(_pts.end(), pts.begin(), pts.end()); }
		Pt&		getPt(jint i) { return _pts.at(i); }
		std::vector<Pt> getPts() const { return _pts; }

		// uvs
		jint	numUvs() const { return (jint)_uvs.size(); }
		//!
		//!VERY TIME EXPENSIVE
		jint	numUvChannels() const
		{
			jint r=0;
			for(auto &pl : _polys)
			{
				r = std::max(pl.numuvs(), r);
			}
			return r;
		}
		bool	uvid(const jint id) const { if(id>numUvs()-1) return false; return true; }
		void	set(const Uv& v, jint id) { if(id>=numUvs()) return; _uvs[id] = v; }
		void	set(const jreal& u, const jreal& v, jint id) { if(id>=numUvs()) return; _uvs[id] = Uv(u, v); }
		void	addUv(const Uv& uv) { _uvs.push_back(uv); }
		void	addUv(const jreal& u, const jreal& v) { _uvs.push_back(Uv(u, v)); }
		void	addUvs(const std::vector<Uv>& uvs) { _uvs.insert(_uvs.end(), uvs.begin(), uvs.end()); }
		Uv&		getUv(int i) { return _uvs.at(i); }
		std::vector<Uv> getUvs() const { return _uvs; }

		// normals
		jint	numNs() const { return (jint)_normals.size(); }
		bool	nid(const jint id) const { if(id>numNs()-1) return false; return true; }
		void	setN(const Pt& v, jint id) { if(id>=numNs()) return; _normals[id] = v; }
		void	setN(const jreal& xx, const jreal& yy, const jreal& zz, jint id) { if(id>=numNs()) return; _normals[id] = Pt(xx, yy, zz); }
		void	addN(const Pt& n) { _normals.push_back(n); }
		void	addN(const jreal& xx, const jreal& yy, const jreal& zz) { _normals.push_back(Pt(xx, yy, zz)); }
		void	addNs(const std::vector<Pt>& normals) { _normals.insert(_normals.end(), normals.begin(), normals.end()); }
		Pt&	getN(jint i) { return _normals.at(i); }
		std::vector<Pt> getNs() const { return _normals; }

		// polys
		jint	numPolys() const { return (jint)_polys.size(); }
		bool	polyid(const jint id) const { if(id>numPolys()-1) return false; return true; }
		void	set(const Poly& v, jint id) { if(id>=numPolys()) return; _polys[id] = v; }
		void	addPoly(const Poly& poly) { _polys.push_back(poly); }
		void	addPolys(const std::vector<Poly>& polys) { _polys.insert(_polys.end(), polys.begin(), polys.end()); }
		Poly&	getPoly(int i) { return _polys.at(i); }
		std::vector<Poly> getPolys() const { return _polys; }

		//!
		//! width/height/depth, posx/posy/posz
		//! replace with single poly and extrude later
		static Mesh cube(jreal sx, jreal sy, jreal sz, jreal ox=0, jreal oy=0, jreal oz=0)
		{
			Mesh r;
			jint cntr = 0;
			std::vector<Pt> pts;

			// pts
			Pt ofs(ox, oy, oz);
			jreal x = sx/2;
			jreal y = sy/2;
			jreal z = sz/2;
			pts.push_back(Pt(-x, -y, -z));
			pts.push_back(Pt(x, -y, -z));
			pts.push_back(Pt(-x, -y, z));
			pts.push_back(Pt(x, -y, z));
			pts.push_back(Pt(-x, y, -z));
			pts.push_back(Pt(x, y, -z));
			pts.push_back(Pt(-x, y, z));
			pts.push_back(Pt(x, y, z));
			// points
			for(auto i : pts) r.addPt(i+ofs);
			// polys
			//std::vector<jint> conn{0,2,3,1, 4,5,7,6, 0,1,5,4, 3,2,6,7, 0,4,6,2, 1,3,7,5};
			//std::reverse(conn.begin(), conn.end());
			std::vector<jint> conn{5,7,3,1, 2,6,4,0, 7,6,2,3, 4,5,1,0, 6,7,5,4, 1,3,2,0};
			jint nconn = (jint)conn.size();
			for(auto i = 0; i<nconn; i += 4)
			{
				Poly pl;
				pl += conn[i];
				pl += conn[i+1];
				pl += conn[i+2];
				pl += conn[i+3];
				r.addPoly(pl);
			}
			return r;
		}

		


		// IO
		/// Import
		///
		/// STL vectors -> jmesh
		void import_mesh(const std::vector<Pt>& pts, const std::vector<int>& counts, const std::vector<int>& connects)
		{
			clear();
			jint cntr = 0;

			addPts(pts);					// physical points
			cntr = 0;
			for(auto i : counts)
			{
				Poly pl;
				assert(cntr < connects.size());
				for(auto j = 0; j<i; j++) { pl += connects[cntr++]; }
				addPoly(pl);		// refs to points
			}
		}

		
		/// matrix from mesh data handle
		static void import_matrix(const MDataHandle &dhMesh, glm::mat4 &worldmat=glm::mat4(1), glm::mat4 &objmat=glm::mat4(1))
		{
			MMatrix m = dhMesh.geometryTransformMatrix();
			for(auto col=0; col<4; col++) worldmat[col] = glm::vec4(m[col][0],m[col][1],m[col][2],m[col][3]);
			m = m.inverse();
			for(auto col=0; col<4; col++) objmat[col] = glm::vec4(m[col][0],m[col][1],m[col][2],m[col][3]);
		}
		/// matrix from mobject
		static void import_matrix(const MObject &moMesh, glm::mat4 &worldmat=glm::mat4(1), glm::mat4 &objmat=glm::mat4(1))
		{
			MStatus status = MS::kSuccess;
			MDagPath moPath;
			MObject motemp = moMesh;
			MFnDagNode dn(motemp);
			status = dn.getPath(moPath);
			//status = MDagPath::getAPathTo(moMesh, moPath);
			MMatrix m = moPath.inclusiveMatrix();
			for(auto col=0; col<4; col++) objmat[col] = glm::vec4(m[col][0],m[col][1],m[col][2],m[col][3]);
			m = moPath.inclusiveMatrixInverse();
			for(auto col=0; col<4; col++) worldmat[col] = glm::vec4(m[col][0],m[col][1],m[col][2],m[col][3]);
		}
		static void print_matrix(const glm::mat4 &mat)
		{
			std::cout << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", " << mat[0][3] << std::endl;
			std::cout << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", " << mat[1][3] << std::endl;
			std::cout << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ", " << mat[2][3] << std::endl;
			std::cout << mat[3][0] << ", " << mat[3][1] << ", " << mat[3][2] << ", " << mat[3][3] << std::endl;
		}
		

		// TODO: technically, if we import mesh in worldSpace, matrices should be (1). Fix later.
		/// DataHandle -> jmesh
		MStatus import_mesh(const MDataHandle &dhMesh)
		{
			//if(moMesh.apiType() == MFn::kMeshData)
			MStatus status = MS::kSuccess;
			clear();
			MObject moMesh = dhMesh.asMeshTransformed();
			if(moMesh.isNull()) return MS::kFailure;
			import_meshMO(moMesh);
			import_matrix(dhMesh, _worldMatrix, _matrix);
			return status;
		}
		/// MObject -> jmesh
		MStatus import_mesh(const MObject &moMesh)
		{
			//if(moMesh.apiType() == MFn::kMesh)
			MStatus status = MS::kSuccess;
			clear();
			import_meshMO(moMesh);
			import_matrix(moMesh, _worldMatrix, _matrix);
			for(auto &pt:_pts) pt*=_worldMatrix;
			return status;
		}

		/// Backend function
		MStatus import_meshMO(const MObject &moMesh)
		{
			MStatus status = MS::kSuccess;
			clear();

			int				nVerts;
			int				nPolys;
			MPointArray		polyVertices;
			MFloatArray		polyU;
			MFloatArray		polyV;
			MString			uvSet;
			MVectorArray	polyNormals;
			bool doUVs = false;
			int uvIdx;

			// attach fn
			MFnMesh fnPoly(moMesh, &status);
			errstatus(status);
			nVerts = fnPoly.numVertices(&status);
			nPolys = fnPoly.numPolygons(&status);
			// get points
			status = fnPoly.getPoints(polyVertices, MSpace::kObject);
			errstatus(status);
			for(auto i = 0u; i<polyVertices.length(); i++) addPt(polyVertices[i].x, polyVertices[i].y, polyVertices[i].z);

			// get uv points
			if(fnPoly.numUVSets())
			{
				status = fnPoly.getCurrentUVSetName(uvSet);
				errstatus(status);
				if(uvSet.length())
				{
					status = fnPoly.getUVs(polyU, polyV, &uvSet);
					errstatus(status);
					for(auto i = 0u; i<polyU.length(); i++) addUv(polyU[i], polyV[i]);
					doUVs = true;
				}
			}

			MItMeshPolygon pit(moMesh, &status);
			for(pit.reset(); !pit.isDone(); pit.next())
			{
				Poly pl;
				// count
				int vCntr = pit.polygonVertexCount();
				// building jpoly
				MIntArray connects;
				status = pit.getVertices(connects);
				errstatus(status);
				for(auto i = 0u; i<connects.length(); i++)
				{
					Point pnt(connects[i]);
					if(doUVs && pit.hasUVs()) { pit.getUVIndex(i, uvIdx); pnt.addUv(uvIdx); }
					pl.addPoint(pnt);
				}
				// TODO: fix normals here
				//MFloatVectorArray normals;
				//fnPoly.getNormals(normals, MSpace::kWorld);

				//getting per-vertex normals
				/*
				MVectorArray vtmp;
				status = pit.getNormals(vtmp, MSpace::kWorld);
				if(status.error()) jutil::mtrace("ERROR getting normals for given face");
				for(int i = 0; i<vtmp.length(); i++) { polyNormals.append(vtmp[i]); }
				/**/
				addPoly(pl);
			}
			return status;
		}





		/// STL Export
		///
		/// jmesh -> STL vectors
		void export_mesh(std::vector<Pt>& pts, std::vector<int>& counts, std::vector<int>& connects)
		{
			pts.clear();
			counts.clear();
			connects.clear();
			if(!numPts()) return;

			pts = getPts();
			// iterating faces
			jint cntr = 0;
			for(auto &face : _polys)
			{
				counts.push_back(face.numpts());
				for(auto facePoint : face) connects.push_back(facePoint.get());
			}
		}
		///
		/// jmesh -> STL vectors of UVs
		void export_uvs(std::vector<Uv>& uvs, std::vector<int>& counts, std::vector<int>& connects, jint chan=0)
		{
			uvs.clear();
			counts.clear();
			connects.clear();
			if(!numUvs()) return;

			uvs = getUvs();
			// iterating faces
			jint cntr = 0;
			for(auto &face : _polys)
			{
				counts.push_back(face.numpts());
				for(auto &facePoint : face)
				{
					jint uv = facePoint.getUv();
					if(uv == -1) continue;
					connects.push_back(uv);
				}
			}
		}
		///
		/// jmesh -> STL vector of normals
		void export_normals(std::vector<Pt>& normals)
		{
			normals.clear();
			if(!numNs()) return;
			normals = getNs();
		}

		///
		/// jmesh -> Maya arrays of pts
		void export_mesh(jint& nVerts, jint& nPolys, MPointArray& mpts, MIntArray& mcounts, MIntArray& mconnects)
		{
			mpts.clear();
			mcounts.clear();
			mconnects.clear();

			for(auto &p : _pts) { mpts.append(MPoint(p.x(), p.y(), p.z())); }
			nVerts = mpts.length();
			for(auto &f : _polys)
			{
				mcounts.append(f.numpts());
				for(auto fpt : f) mconnects.append(fpt.get());
			}
			nPolys = mcounts.length();
		}
		///
		/// jmesh -> Maya arrays of UVs
		void export_uvs(MFloatArray& u, MFloatArray& v, MIntArray& counts, MIntArray& connects, jint chan=0)
		{
			u.clear();
			v.clear();
			counts.clear();
			connects.clear();
			if(!numUvs() || chan > numUvChannels()) return;

			std::vector<Uv> uvs = getUvs();
			for(auto &t : uvs) { u.append(t[0]); v.append(t[1]); }

			// iterating faces
			jint cntr = 0;
			for(auto &f : _polys)
			{
				counts.append(f.numpts());
				for(auto &fpt : f)
				{
					jint uv = fpt.getUv(chan);
					if(uv == -1) continue;
					connects.append(uv);
				}
			}
		}
		///
		/// jmesh -> Maya arrays of normals
		void export_normals(MVectorArray& normals)
		{
			normals.clear();
			if(!numNs()) return;

			jint cntr = 0;
			for(auto &n : _normals) { normals.append(MVector(n[0], n[1], n[2])); }
		}


		/// Maya export
		///
		/// jmesh -> Maya MObject
		MStatus export_mesh(MObject &moNewMesh)
		{
			MStatus status = MS::kSuccess;
			jint nVerts, nPolys;
			MPointArray pts;
			MIntArray counts, connects;
			export_mesh(nVerts, nPolys, pts, counts, connects);

			// create new mesh
			MFnMeshData mdMesh;
			moNewMesh = mdMesh.create(&status);
			errstatus(status);
			MFnMesh fnMesh;
			MObject moMesh = fnMesh.create(nVerts, nPolys, pts, counts, connects, moNewMesh, &status);
			errstatus(status);

			// get uv data if there's any
			if(!numUvChannels()) return status;
			MFloatArray u, v;
			export_uvs(u, v, counts, connects);
			// apply uv data to the mesh
			MString uvSet("set1");
			status = fnMesh.getCurrentUVSetName(uvSet);
			errstatus(status);
			status = fnMesh.setUVs(u, v, &uvSet);
			errstatus(status);
			status = fnMesh.assignUVs(counts, connects, &uvSet);
			errstatus(status);
			/*
			if(useNormals)
			{
			// recreating faceList from polyConnects
			MIntArray faceList;
			for(unsigned int i=0; i<mMesh.polyCounts.length(); i++)
			{
			for(int j=0; j<mMesh.polyCounts[i]; j++) faceList.append(i);
			}

			status = fnMesh.setFaceVertexNormals(mMesh.polyNormals, faceList, mMesh.polyConnects, MSpace::kWorld);
			if(MS::kSuccess != status)
			{
			msg("j_import()->assembleMesh(): error assigning normals, continuing");
			status = MS::kSuccess;
			}
			}
			*/
			return status;
		}

		//sort
		//unique
		//extract
		//extractRange

		/// Mesh/Poly copy ops
		///
		/// A -> B, vector<jint>
		static void m_copy(Mesh& a, const std::vector<jint>& plist, Mesh& b)
		{
			b.clear();
			jint sz = a.numPolys();
			std::vector<jint> cacheP, cacheU;
			for(auto f : plist) { if(f <= sz) p_copy(a, f, b, cacheP, cacheU); }
		}
		///
		/// A -> B, jrange
		static void m_copy(Mesh& a, const Range& rng, Mesh& b)
		{
			m_copy(a, rng.asInt(), b);
		}
		///
		/// A -> B, s-e
		static void m_copy(Mesh& a, jint s, jint e, Mesh& b)
		{
			b.clear();
			// sanity checks
			if(s > e) std::swap(s, e);
			if(s < 0) s = 0;
			if(e > a.numPolys()) e = a.numPolys();
			// copy poly by poly
			Range rng(s, e);
			std::vector<jint> cacheP, cacheU;
			for(auto &f : rng) p_copy(a, f, b, cacheP, cacheU);
		}
		///
		/// *this -> B, s-e
		void m_copy(jint s, jint e, Mesh& b) { m_copy(*this, s, e, b); }

		// mesh funcs
		void m_sort_rnd(const Pt&)
		{}

		void m_sort_dist(const Pt&)
		{}

		void m_sort_area(const Pt&)
		{}

		std::vector<Mesh> m_separate()
		{
			std::vector<Mesh> r;
			return r;
		}

		void m_translate	(const Pt& ofs) { for(auto &p : _pts) p += ofs; }
		void m_scale		(const jreal& scl) { for(auto &p : _pts) p *= scl; }
		void m_rotate		(const jreal&, const jreal&, const jreal&) {}
		void m_transform	(const Mat4&) {}

		void m_recenter		(void)
		{
			Pt centroid = m_centroid();
			m_translate(centroid*-1.0f);
		}
		void m_rescale		(void)
		{
			Pt bbmin, bbmax;
			m_bbox(bbmin, bbmax);
			jreal scl = std::max(std::max(bbmax.x()-bbmin.x(), bbmax.y()-bbmin.y()),	bbmax.z()-bbmin.z());
			m_scale(1.0f/scl);
		}

		// minPt, maxPt
		void m_bbox(Pt &mn, Pt &mx) const
		{
			jreal nx, ny, nz, xx, xy, xz, f;
			nx = ny = nz = FLT_MAX;
			xx = xy = xz = FLT_MIN;
			for(auto &p : _pts)
			{
				f = p.x(); nx = std::min(f, nx); xx = std::max(f, xx);
				f = p.y(); ny = std::min(f, ny); xy = std::max(f, xy);
				f = p.z(); nz = std::min(f, nz); xz = std::max(f, xz);
			}
			mn = Pt(nx, ny, nz);
			mx = Pt(xx, xy, xz);
		}

		Pt m_centroid() const
		{
			Pt r;
			jint cntr = 0;
			for(auto &p : _pts) { r += p;  cntr++; }
			r /= cntr;
			return r;
		}


		// poly funcs
		// copies a poly from one mesh to another using std::vector<jint> = -1.
		// meshFrom, polyIndex, meshOut, cachePts, cacheUvs, makeUniqueFlag
		static void p_copy(Mesh& a, jint f, Mesh& b, std::vector<jint>& cacheP, std::vector<jint>& cacheU, jint makeUnique=0)
		{
			if(!cacheP.size()) { auto n = a.numPts(); cacheP.resize(n); cacheP.assign(n, -1); }
			if(!cacheU.size()) { auto n = a.numUvs(); cacheU.resize(n); cacheU.assign(n, -1); }

			Poly jPoly, face = a.getPoly(f);
			// loop poly -> points
			for(auto &point : face)
			{
				auto ptId = point.get();	// jpt ref
				if(cacheP[ptId] == -1) { cacheP[ptId] = b.numPts(); b.addPt(a.getPt(ptId)); }		// check if point is in the cache/new point
				Point jPointNew(cacheP[ptId]);
				// loop uv refs
				for(auto &uvNum : point)
				{
					if(cacheU[uvNum] == -1) { cacheU[uvNum] = b.numUvs(); b.addUv(a.getUv(uvNum)); }	// check if uv is in the cache/new uv
					jPointNew.addUv(cacheU[uvNum]);
				}
				jPoly.addPoint(jPointNew);		// add new point to the new poly
			}
			b.addPoly(jPoly);		// add new poly to new mesh
		}

		jreal p_area(jint f)
		{
			/*
			area = 0;         // Accumulates area in the loop
			j = numPoints-1;  // The last vertex is the 'previous' one to the first
			for(i = 0; i<numPoints; i++) {
			area = area +  (X[j]+X[i]) * (Y[j]-Y[i]);
			j = i;  //j is previous vertex to i
			}
			/**/
			jreal r = 0;
			jint i, j, npts;
			npts = numPts();
			j = npts-1;

			for(i = 0; i<npts; i++)
			{
				Pt pCur = getPt(i);
				Pt pPrev = getPt(j);
				r += (pPrev.x()+pCur.x()) * (pPrev.y()-pCur.y());
				j = i;
			}
			return r;
		}


		Pt p_centroid(jint id)
		{
			Pt r;
			Poly pl = getPoly(id);
			for(auto &p : pl) r += getPt(p.get());
			r /= pl.numpts();
			return r;
		}

		// TODO make a test
		Pt p_normal(jint id)
		{
			Pt v1, v2;
			Poly pl = getPoly(id);
			if(pl.numpts() < 3) return v1;
			v1 = getPt(pl[1].get()) - getPt(pl[0].get());
			v2 = getPt(pl[2].get()) - getPt(pl[1].get());
			v1.cross(v2);
			v1.normalize();
			return v1;
		}

		void p_triangulate(jint)
		{}

		static Mesh circle(jreal radii=1.0, jint sides=4, jint createPoly=0)
		{
			Mesh r;
			if(!sides) return r;
			jint ptStart=0, ptEnd=0, ptCur=0;
			jreal astep = 2*M_PI/(jreal)sides;
			jreal halfastep = astep/2.0;
			Pt pt;
			Poly poly;

			for(auto angle=0.0f; angle<2*M_PI; angle+=astep)
			{
				jreal x = std::sin(angle-halfastep)*radii;
				jreal y = std::cos(angle-halfastep)*radii;
				jreal z = 0.0;
				poly.addPoint(r.numPts());
				r.addPt(Pt(x,y,z));
			}
			if(createPoly) r.addPoly(poly);
			return r;
		}

		// creates translate/rotate matrix from point and normal
		static glm::mat4 makeMatrix(const Pt& pta, const Pt& ptb, const Pt& vup=Pt(0.0f, 1.0f, 0.0f))
		{
			jreal veyelen;
			Pt veye, vup2, vside, vnoise, vfinal;
			glm::mat4 mm(1);
			// preflight
			veye = ptb-pta;
			veyelen = veye.length();
			if(veyelen == 0.0f) return mm;	// if distance between A..B is 0, NaNs will pop, so return identity matrix
			veye /= veyelen;
			vup2 = vup;
			vup2.normalize();
			vside = Pt::cross(vup2, veye);
			vside.normalize();
			vup2 = Pt::cross(veye, vside);
			vup2.normalize();
			// create edge matrix
			mm[0] = glm::vec4(vside.x(), vside.y(), vside.z(), 0.0f);
			mm[1] = glm::vec4(vup2.x(), vup2.y(), vup2.z(), 0.0f);
			mm[2] = glm::vec4(veye.x(), veye.y(), veye.z(), 0.0f);
			mm[3] = glm::vec4(pta.x(), pta.y(), pta.z(), 1.0f);
			return mm;
		}


		// loft + noise made this way for wireframe/wires
		static void loft(Mesh &rMesh, const Mesh &profile, const Pt &pta, const Pt &ptb, const jreal divs=1.0f, const bool closed=true, const Pt &vup=Pt(0.0f, 1.0f, 0.0f),
						 const jreal nscale=0.0f, const jreal nfreq=0.2f, const jreal noffset=0.0f, const jreal nseed=0.0f)
		{
			//vars
			jint pOffset, pOffsetOld, sections;
			jreal veyelen, step, divs2, march=0.0f, x, y, z;
			glm::mat4 mm;
			Mesh profileMesh, sectionMesh, oldSectionMesh;
			Pt veye, vup2, vside, vnoise, vfinal;

			//TODO: rewrite: generate line at 0 and then mult by matrix using makeMatrix()
			// preflight
			veye = ptb-pta;
			veyelen = veye.length();
			if(veyelen == 0.0f) return;	// if distance between A..B is 0, NaNs will pop
			veye /= veyelen;
			vside = Pt::cross(vup, veye);
			vup2 = Pt::cross(veye, vside);
			// create edge matrix
			mm[0] = glm::vec4(vside.x(), vside.y(), vside.z(), 0.0f);
			mm[1] = glm::vec4(vup2.x(), vup2.y(), vup2.z(), 0.0f);
			mm[2] = glm::vec4(veye.x(), veye.y(), veye.z(), 0.0f);
			mm[3] = glm::vec4(pta.x(), pta.y(), pta.z(), 1.0f);
			// transform source profile to starting point
			profileMesh = profile*mm;
			//for(Pt &ptt : profileMesh._pts) { if(isnan(ptt.x())) __debugbreak(); }

			// divs sets number of _subdivisions_
			// if divs < 0, then it's divs per unit
			// number of pointSections = start + _subdivisions_ + end
			// number of poly section = pointSection - 1;
			divs2 = divs;
			if (divs2<0.0f) { divs2 = std::abs(divs2); divs2 = std::ceil(veyelen*divs2); }
			sections = divs2 + 2;
			step = veyelen/(sections-1u);

			// ---------== sections LOOP
			for(jint i=0; i<sections; i++)
			{
				jreal inorm = march/veyelen;	// normalized i
				jreal isin = std::sin(inorm*M_PI);	// normalized sin(i)
				// remember points offset in mesh
				pOffset = rMesh.numPts();
				// final vector to add to start point A
				vfinal = veye*march;
				// process noise
				if(nscale > 0.0f)
				{
					// every point has fixed start and just marched part getting scaled by freq
					x = noffset + (march*nfreq);
					y = nseed*10.0f;
					z = 0.0f;
					// float scaled_octave_noise_3d(const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y, const float z);
					vnoise = vup2 * (scaled_octave_noise_3d(3.0f, 0.25f, 1.0f, -1.0, 1.0, x,y,z) * nscale * isin);
					vfinal += vnoise;
				}
				// transform original profile
				sectionMesh = profileMesh + vfinal;
				// skip adding polys on zeroth step
				if(!march) goto CONT;
				// add polys
				jint npts = sectionMesh.numPts();
				for(auto j=0; j<npts; j++)
				{
					Poly pl;
					jint pta0 = pOffset+j;
					jint pta1 = pta0+1;
					jint ptb0 = pOffsetOld+j;
					jint ptb1 = ptb0+1;
					// loop to start at the end
					if(j==npts-1)
					{
						if(!closed) continue;
						pta1 = pOffset;
						ptb1 = pOffsetOld;
					}
					pl.addPoint(pta0);
					pl.addPoint(pta1);
					pl.addPoint(ptb1);
					pl.addPoint(ptb0);
					sectionMesh.addPoly(pl);
				}

				CONT:
				rMesh.merge(sectionMesh);
				oldSectionMesh = sectionMesh;
				pOffsetOld = pOffset;
				march += step;
			}
			// LOOP end ==-------------
		}




		// Operators
		void append(const Mesh& b)
		{
			auto ptOfs = numPts();
			auto uvOfs = numUvs();
			auto polyOfs = numPolys();

			addPts(b._pts);
			addUvs(b._uvs);
			addPolys(b._polys);

			auto sz = numPolys();
			for(auto i = polyOfs; i<sz; i++) getPoly(i).offsetPoints(ptOfs, uvOfs);
		}

		// same as append, just without indices offsetting - to not screw up precalculated ones
		void merge(const Mesh& b)
		{
			auto ptOfs = numPts();
			auto uvOfs = numUvs();
			auto polyOfs = numPolys();

			addPts(b._pts);
			addUvs(b._uvs);
			addPolys(b._polys);
		}

		glm::mat4 matrix(void) { return _matrix; }
		glm::mat4 worldMatrix(void) { return _worldMatrix; }

		// --== Operators: Mesh with Mesh
		Mesh&		operator=(const Mesh &b)
		{
			if(this == &b) return(*this);
			clear();
			_pts = b._pts;
			_polys = b._polys;
			_uvs = b._uvs;
			_normals = b._normals;
			_matrix = b._matrix;
			_worldMatrix = b._worldMatrix;
			return(*this);
		}

		bool		operator==(const Mesh &b) const { if(_pts == b._pts && _uvs == b._uvs &&  _polys == b._polys) return true; return false; }
		bool		operator!=(const Mesh &b) const { if(_pts != b._pts || _uvs != b._uvs ||  _polys != b._polys) return true; return false; }
		Mesh&		operator+=(const Mesh &b) { this->append(b); return *this; }
		Mesh		operator+(const Mesh& b) { Mesh r(*this); r+=b; return(r); }
		Poly		operator[](jint i) const { return _polys[i]; }
		Poly&		operator[](jint i) { return _polys.at(i); }

		// --== Operators: Mesh with Pt
		Mesh		operator+(const Pt& b) { Mesh r(*this); for(auto &p:r._pts) p += b; return r; }
		Mesh&		operator+=(const Pt &b) { *this = *this+b; return *this; }
		Mesh		operator-(const Pt& b) { Mesh r(*this); for(auto &p:r._pts) p -= b; return r; }
		Mesh&		operator-=(const Pt &b) { *this = *this-b; return *this; }
		// --== Operators: Mesh with glm::mat3/4
		Mesh operator* (const glm::mat3 &m) const { Mesh r(*this); for(auto &p:r._pts) p *= m; return r; }
		Mesh& operator*= (const glm::mat3 &m) { *this = *this * m; return *this;}
		Mesh operator* (const glm::mat4 &m) const { Mesh r(*this); for(auto &p:r._pts) p *= m; return r; }
		Mesh& operator*= (const glm::mat4 &m) { *this = *this * m; return *this; }
		// --== Operators: Mesh with glm::vec3/4
		Mesh		operator+(const glm::vec3 &b) { Mesh r(*this); for(auto &p:r._pts) p += b; return r; }
		Mesh&		operator+=(const glm::vec3 &b) { *this = *this+b; return *this; }



		friend std::ostream& operator<<(std::ostream& ostr, const Mesh& b)
		{
			OSTR_SET
			ostr << "Mesh<" << b._pts.size() << " pts, " << b._uvs.size() << " uvs, " << b._polys.size() << " polys>" << endl;
			//for(auto e : b._polys) { cout << e; }
			return ostr;
		}

		// TODO add objects support
		// TODO backtracks - for efficient copying and findNearby
		// TODO make an "extrude polygon" function
		//jint loft(const std::vector<int>&, const std::vector<int>&);
		//jmesh facet();

		std::vector<Poly>::iterator begin() { return _polys.begin(); }
		std::vector<Poly>::iterator end() { return _polys.end(); }
		std::vector<Poly>::const_iterator begin() const { return _polys.begin(); }
		std::vector<Poly>::const_iterator end() const { return _polys.end(); }

		glm::mat4 _matrix;			// world -> obj matrix
		glm::mat4 _worldMatrix;		// obj->world matrix
	private:
		std::vector<Pt>	_pts;		// actual point data
		std::vector<Uv>	_uvs;		// and uvs
		std::vector<Pt>	_normals;	// normals in jpt containers
		std::vector<Poly> _polys;	// refs to _points
		// todo add counts-based checksum for mesh
		// and per-object here
};

#endif
