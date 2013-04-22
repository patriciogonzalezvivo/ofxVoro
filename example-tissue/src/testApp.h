#pragma once

#include "ofMain.h"

#include "ofxVoro.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofEasyCam   cam;
    ofLight     light;
    
    vector<ofMesh>  cellMeshes;
    vector<ofPoint> cellNucles;
    vector<float>   cellRadius;
    
    int con_nBlocks;    // Set up the number of blocks that the container is divided into
    int con_init_mem;   // ??
    bool con_periodic;
    
    int size;
};
