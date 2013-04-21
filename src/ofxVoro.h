//
//  ofxVoro.h
//
//  Created by Patricio Gonzalez Vivo on 4/17/13.
//
//

#pragma once

#include "ofMain.h"
#include "voro++.hh"

//  cell (voronoicell) wrappers
//
float           getCellRadius(voro::voronoicell &_c);
ofPoint         getCellCentroid(voro::voronoicell &_c, ofPoint _pos = ofPoint(0,0,0));
vector<ofPoint> getCellVerteces(voro::voronoicell &_c, ofPoint _pos = ofPoint(0,0,0));
ofMesh          getCellMesh(voro::voronoicell &_c, ofPoint _pos = ofPoint(0,0,0));

//  container wrappers
//
vector<ofMesh>  getCellsFromContainer(voro::container &_con);