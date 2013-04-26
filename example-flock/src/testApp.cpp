#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    light.setPosition(100,500, 100);
    cam.setAutoDistance(true);
    
    containerSize = 100;
    
    int nBoids = 50;
    for (int i = 0; i < nBoids; i++){
        Boid *newBoid = new Boid( );
        boids.push_back(newBoid);
    }
}

//--------------------------------------------------------------
void testApp::update(){

    bool press = ofGetKeyPressed();
    container con(-containerSize,containerSize,
                  -containerSize,containerSize,
                  -containerSize,containerSize,
                  1, 1, 1, press, press, press, 8);
    
    for(int i = 0; i < boids.size();i++){
        boids[i]->update( boids );
        if ( insideContainer( con, *boids[i])){
            addCellSeed( con, boids[i], i);
            boids[i]->color.set(50, 200,200);
        } else {
            boids[i]->color.set(255,0,0);
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

    for (int i = 0; i < boids.size(); i++){
		boids[i]->draw();
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