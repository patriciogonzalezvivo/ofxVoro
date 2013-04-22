//
//  ofxVoro.cpp
//  example
//
//  Created by Patricio Gonzalez Vivo on 4/21/13.
//
//

#include "ofxVoro.h"

float getCellRadius(voro::voronoicell &_c){
    return pow(3* _c.volume()/4*PI,1.0/3.0);
};

ofPoint getCellCentroid(voro::voronoicell &_c, ofPoint _pos ){
    double x,y,z;
    _c.centroid(x,y,z);
    return ofPoint(x,y,z)*0.5 + _pos;
};

vector<ofPoint> getCellVerteces(voro::voronoicell &_c, ofPoint _pos ){
    double *ptsp=_c.pts;
    vector<ofPoint> vertices;
    for(int i = 0; i < _c.p; i++, ptsp+=3){
        ofPoint newPoint;
        newPoint.x = _pos.x + ptsp[0]*0.5;
        newPoint.y = _pos.y + ptsp[1]*0.5;
        newPoint.z = _pos.z + ptsp[2]*0.5;
        vertices.push_back(newPoint);
    }
    return vertices;
};

ofMesh getCellMesh(voro::voronoicell &_c, ofPoint _pos ){
    if( _c.p ) {
        
        ofPoint centroid = getCellCentroid(_c,_pos);
        
        int i,j,k,l,m,n;
        
        //  Vertex
        //
        double *ptsp=_c.pts;
        vector<ofPoint> vertices;
        vector<ofPoint> normals;
        for(i = 0; i < _c.p; i++, ptsp+=3){
            ofPoint newPoint;
            newPoint.x = _pos.x + ptsp[0]*0.5;
            newPoint.y = _pos.y + ptsp[1]*0.5;
            newPoint.z = _pos.z + ptsp[2]*0.5;
            vertices.push_back(newPoint);
            
            ofPoint newNormal;
            newNormal =  newPoint - centroid ;
            newNormal = newNormal.normalize();
            normals.push_back(newNormal);
        }
        
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLES );
        mesh.addVertices(vertices);
        mesh.addNormals(normals);
        
        //  Index
        //
        for(i = 1; i < _c.p; i++){
            for(j = 0; j < _c.nu[i]; j++) {
                
                k = _c.ed[i][j];
                
                if( k >= 0 ) {
                    _c.ed[i][j]=-1-k;
                    l = _c.cycle_up( _c.ed[i][ _c.nu[i]+j], k);
                    m = _c.ed[k][l];
                    _c.ed[k][l]=-1-m;
                    
                    while(m!=i) {
                        n = _c.cycle_up( _c.ed[k][ _c.nu[k]+l],m);
                        mesh.addTriangle(i, k, m);
                        
                        k=m;
                        l=n;
                        m=_c.ed[k][l];
                        _c.ed[k][l]=-1-m;
                    }
                }
            }
        }
        
        return mesh;
    }
    
    return ofMesh();
};

vector<ofMesh>  getCellsFromContainer(voro::container &_con){
    
    vector<ofMesh> cells;
    
    voro::c_loop_all vl( _con );
    int i = 0;
	if( vl.start() ){
        
        do {
            voro::voronoicell c;
            if( !_con.compute_cell(c, vl) ) {
                return cells;
            } else {
                double *pp = _con.p[vl.ijk] + _con.ps * vl.q;
                ofMesh cellMesh = getCellMesh(c, ofPoint(pp[0],pp[1],pp[2]));
                cells.push_back( cellMesh );
                i++;
            }
            
        } while( vl.inc() );
    }
    
    return cells;
}

vector<float> getCellsRadius(voro::container &_con){
    vector<float> radius;
    
    voro::c_loop_all vl( _con );
    int i = 0;
	if( vl.start() ){
        
        do {
            voro::voronoicell c;
            if( !_con.compute_cell(c, vl) ) {
                return radius;
            } else {
                double *pp = _con.p[vl.ijk] + _con.ps * vl.q;
                float rad = getCellRadius(c);
                radius.push_back( rad );
                i++;
            }
            
        } while( vl.inc() );
    }
    
    return radius;
}
