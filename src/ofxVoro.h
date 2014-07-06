//
//  ofxVoro.h
//  Funtions for wrapping Voro++ library ( http://math.lbl.gov/voro++/ )
//
//  Created by Patricio Gonzalez Vivo on 4/17/13.
//
//

#pragma once

#include "ofMain.h"

#include "voro++.hh"

//  cell (voronoicell) wrappers
//
float           getCellRadius(voro::voronoicell &_con);
ofPoint         getCellCentroid(voro::voronoicell &_con, ofPoint _pos = ofPoint(0,0,0));
vector<ofPoint> getCellVerteces(voro::voronoicell &_con, ofPoint _pos = ofPoint(0,0,0));
ofVboMesh          getCellMesh(voro::voronoicell &_con, ofPoint _pos = ofPoint(0,0,0));
void            getCellMesh(voro::voronoicell &_c, ofPoint _pos, ofVboMesh& mesh );
void            getCellMesh(voro::voronoicell &_c, ofVboMesh& mesh );

//  container wrappers
//
void            addCellSeed(voro::container &_con, ofPoint &_pnt, int _i, bool _checkInside = false);
void            addCellSeed(voro::container &_con, ofPoint *_pnt, int _i, bool _checkInside = false);
void            addCellsSeeds(voro::container &_con, vector<ofPoint> &_pnts, bool _checkInside = false);
void            addCellsSeeds(voro::container &_con, vector<ofPoint*> &_pnts, bool _checkInside = false);
void            addCellsSeeds(voro::container &_con, ofPoint *_pnts, int _nSize, bool _checkInside = false);

vector<float>   getCellsRadius(voro::container &_con);
vector<ofVboMesh>  getCellsFromContainer(voro::container &_con, float _wallsThikness);
void            getCellsFromContainer(voro::container &_con, float _wallsThikness, vector<ofVboMesh>& cells);
void            getCellsFromContainer(voro::container &_con, vector<ofVboMesh>& cells);
void            getCellsFromContainer(voro::container &_con, vector<ofVboMesh>& cells, float _maxRadius);
vector<ofPoint> getCellsPositions(voro::container &_con);
vector<ofPoint> getCellsCentroids(voro::container &_con);
vector< ofPolyline > getCellsPolylines(voro::container &_con);
vector< vector<ofPoint> > getCellsVertices(voro::container &_con);


bool            insideContainer(voro::container &_con, ofPoint _pnt);