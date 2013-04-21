#include "testApp.h"

#include "voro++.hh"
using namespace voro;

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    light.setPosition(100,500, 100);
    cam.setAutoDistance(true);
    
    size = 400;
    nBoids = 10;
    
    space.xLeft = -size;
    space.xRight = size;
    space.yTop = -size;
    space.yBottom = size;
    space.zFront = -size;
    space.zBack = size;
    
    boids = new (nothrow) Boid[nBoids];
    
    for (int i = 0; i < nBoids; i++){
        boids[i] = Boid();
        boids[i].setNBoids(&nBoids);
        boids[i].setSpace(&space);
        boids[i].loc.set(ofRandom(space.xLeft, space.xRight),
                         ofRandom(space.yTop, space.yBottom),
                         ofRandom(space.zFront,space.zBack));
    }
    
    size = 100;
    
}

//--------------------------------------------------------------
void testApp::update(){
    for (int i = 0; i < nBoids; i++)
		boids[i].update(boids);
    
    container con(-size,size,
                  -size,size,
                  -size,size,
                  6,6,6,
                  false,false,false,
                  8);
    
    for(int i = 0; i < nBoids;i++){
        con.put(i,  boids[i].loc.x,
                    boids[i].loc.y,
                    boids[i].loc.z);
    }
    
    cellMeshes.clear();
    cellMeshes = getCellsFromContainer(con);
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    ofPushMatrix();
    cam.begin();
    light.enable();
    ofEnableLighting();
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_SMOOTH);
    
    ofSetColor(255,0,0);
    for (int i = 0; i < nBoids; i++)
		boids[i].draw();
    
    
    for(int i = 0; i < cellMeshes.size(); i++){
        ofSetColor(0,200,200, 30);
        cellMeshes[i].drawFaces();
        
        ofPushStyle();
        
        ofSetLineWidth(3);
        ofSetColor(0,255,255);
        cellMeshes[i].drawWireframe();
        ofPopStyle();
    }
    
    
//    glDisable(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);
    ofDisableLighting();
    light.disable();
    cam.end();
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}