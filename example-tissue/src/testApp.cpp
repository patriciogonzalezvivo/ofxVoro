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
    
    size = 100;
    int nCells = 50;
    
    
    for(int i = 0; i < nCells;i++){
        ofPoint newCell = ofPoint(ofRandom(-size,size),
                                  ofRandom(-size,size),
                                  ofRandom(-5,5));
        
        cellNucles.push_back(newCell);
    }

    con_nBlocks = 10;
    con_periodic = false;
    con_init_mem = 8;
}

//--------------------------------------------------------------
void testApp::update(){

    cellMeshes.clear();
    
    container con(-size,size,
                  -size,size,
                  -10,10,
                  con_nBlocks,con_nBlocks,1,
                  con_periodic,con_periodic,con_periodic,
                  con_init_mem);
    
    for(int i = 0; i < cellNucles.size(); i++){
        con.put(i, cellNucles[i].x, cellNucles[i].y, cellNucles[i].z);
    }
    
    cellMeshes = getCellsFromContainer(con);
    cellRadius = getCellsRadius(con);
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofPushMatrix();
    cam.begin();
    light.enable();
    ofEnableLighting();
    glEnable(GL_DEPTH_TEST);
   
    for (int i = 0; i < cellNucles.size(); i++){
        ofSetColor(255,0,0);
		ofSphere(cellNucles[i], cellRadius[i]*0.2);
    }
    
    
    for(int i = 0; i < cellMeshes.size(); i++){
        ofSetColor(0,200,200,30);
        cellMeshes[i].drawFaces();
        
        ofPushStyle();
        
        ofSetLineWidth(3);
        ofSetColor(0,255,255);
        cellMeshes[i].drawWireframe();
        ofPopStyle();
    }
    
    glDisable(GL_DEPTH_TEST);
    ofDisableLighting();
    light.disable();
    cam.end();
    ofPopMatrix();
    
    ofSetColor(255);
    ofDrawBitmapString("con_nBlocks: " + ofToString(con_nBlocks), 15,15 );
    ofDrawBitmapString("con_init_mem: " + ofToString(con_init_mem), 15,30 );
    ofDrawBitmapString("con_periodic: " + ofToString(con_periodic), 15,45 );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if ( key == '-' ){
        con_nBlocks--;
    } else if ( key == '+'){
        con_nBlocks++;
    } else if ( key == '['){
        con_init_mem--;
    } else if ( key == ']'){
        con_init_mem++;
    } else if ( key == ' '){
        con_periodic = !con_periodic;
    } else if ( key == 'f')
        ofToggleFullscreen();

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