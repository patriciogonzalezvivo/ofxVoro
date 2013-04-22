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
    
    int size = 400;
    nBoids = 10;
    
    space.xLeft = -size;
    space.xRight = size;
    space.yTop = -size;
    space.yBottom = size;
    space.zFront = -size;
    space.zBack = size;
    
    int voroSize = 100;
    voroSpace.xLeft = -voroSize;
    voroSpace.xRight = voroSize;
    voroSpace.yTop = -voroSize;
    voroSpace.yBottom = voroSize;
    voroSpace.zFront = -voroSize;
    voroSpace.zBack = voroSize;
    
    boids = new (nothrow) Boid[nBoids];
    
    for (int i = 0; i < nBoids; i++){
        boids[i] = Boid();
        boids[i].setNBoids(&nBoids);
        boids[i].setSpace(&space);
        boids[i].loc.set(ofRandom(space.xLeft, space.xRight),
                         ofRandom(space.yTop, space.yBottom),
                         ofRandom(space.zFront,space.zBack));
    }
    
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    for (int i = 0; i < nBoids; i++)
		boids[i].update(boids);
    
    bool press = ofGetKeyPressed();
    
    container con(voroSpace.xLeft,voroSpace.xRight,
                  voroSpace.yTop,voroSpace.yBottom,
                  voroSpace.zFront,voroSpace.zBack,
                  1,1,1,
                  press,press,press,
                  8);
    
    for(int i = 0; i < nBoids;i++){
        
        if ( voroSpace.inside(boids[i].loc)){
            con.put(i,  boids[i].loc.x,
                        boids[i].loc.y,
                        boids[i].loc.z);
        }
    }
    
    cellMeshes.clear();
    cellMeshes = getCellsFromContainer(con);
    
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
    
    ofNoFill();
    ofSetColor(150,255,255);
    ofBox(200);
    ofFill();

    for (int i = 0; i < nBoids; i++){
        if ( voroSpace.inside(boids[i].loc))
            ofSetColor(50, 200,200);
        else
            ofSetColor(255,0,0);
            
		boids[i].draw();
    }
    
    
    for(int i = 0; i < cellMeshes.size(); i++){
        ofSetColor(0,200,200, 30);
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
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'f')
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