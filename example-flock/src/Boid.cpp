/*
 *  Boid.cpp
 *  boid
 *
 *  Copyright 2010 Patricio Gonzalez Vivo http://www.patriciogonzalezvivo.com
 *	All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/

#include "Boid.h"

Boid::Boid() {
	//Set the variable in case the flock don´t set them
	space = new Space();
	space->xLeft = -ofGetScreenWidth()*0.5;
	space->xRight = ofGetScreenWidth()*0.5;
	space->yTop = -ofGetScreenHeight()*0.5;
	space->yBottom = ofGetScreenHeight()*0.5;
	space->zFront = -500;
	space->zBack = 500;
	
    color.set(255,0,0);
    
    set(ofRandom(space->xLeft, space->xRight),
        ofRandom(space->yTop, space->yBottom),
        ofRandom(space->zFront,space->zBack));
    
	vel.set(ofRandom(-0.1,0.1),
            ofRandom(-0.1,0.1),
            ofRandom(-0.1,0.1));
	acc.set(0,0,0);
    
    
	sc = ofRandom(1,3);
    maxspeed = ofRandom(0.25,0.75);
    neighborhoodRadius = 70*sc;
    maxforce = 0.1;
	
	selected	= false;
	wanderTheta = 0.1;
}

Boid::Boid(Space *_s) {
	//Set the variable in case the flock don´t set them
	space = _s;
    set(ofRandom(space->xLeft, space->xRight),
        ofRandom(space->yTop, space->yBottom),
        ofRandom(space->zFront,space->zBack));

    color.set(255,0,0);
    
	vel.set(ofRandom(-0.1,0.1),
            ofRandom(-0.1,0.1),
            ofRandom(-0.1,0.1));
    
	acc.set(0,0,0);
    
	sc = ofRandom(1,3);
    maxspeed = ofRandom(0.25,0.75);
    neighborhoodRadius = 70*sc;
    maxforce = 0.1;
	
	selected	= false;
	wanderTheta = 0.1;
}

//-------------------------------------------------------------- Movement
void Boid::update(vector<Boid*> &_b) {	
    //  SEPARATION
    //
    ofVec3f sep; 
    
    //  ALIGNMENT
    //
    ofVec3f ali;
    int aliCount = 0;
    
    //  COHESION
    //
	ofVec3f coh;
    ofVec3f posSum;
    int cohCount = 0;
    
    for (int i = 0 ; i < _b.size() ; i++) {
        
		ofVec3f diff = *this - *_b[i];
        float d = diff.length();
        
		if ( (d > 0) && (d <= neighborhoodRadius)) {
            
            //  add separation force
            //
            ofVec3f repulse = diff;
			repulse.normalize();
			repulse /= d;
			sep += repulse;
            
            //  add align force
            //
            ali += _b[i]->vel;
			aliCount++;
            
            //  add cohesion force
            //
            posSum += *_b[i]; // Add location
			cohCount++;
		}
    
    }
    
    //  Average of Align
    //
    if (aliCount > 0) {
		ali /= (float)aliCount;
		ali.limit(maxforce);
    }
    
    //  Average of cohesion
    //
    if (cohCount > 0)
		posSum /= (float)cohCount;
	coh = posSum - *this;
	coh.limit(0.04);
	
    
    //  Apply forces into acc
    //
	acc += ali * 3;
	acc += coh * 3;
	acc += sep * 3;//6;	//1;

    //  Do the physics (acc -> vel -> pos)
    //
	vel += acc;
	vel.limit(maxspeed);
	*this += vel;
    acc *= 0;
    
	checkBounds();
}

void Boid::wander(){
	float wanderR = 16.0f;         // Radius for our "wander circle"
    float wanderD = 60.0f;         // Distance for our "wander circle"
    float change = 0.25f;
    wanderTheta += ofRandom(-change,change);     // Randomly change wander theta
	
    // Now we have to calculate the new location to steer towards on the wander circle
    ofVec3f circleloc = vel;			// Start with velocity
    circleloc.normalize();          // Normalize to get heading
    circleloc *= wanderD;			// Multiply by distance
    circleloc += *this;				// Make it relative to boid's location
    
    ofVec3f circleOffSet = ofVec3f(wanderR*cos(wanderTheta),0,wanderR*sin(wanderTheta));
    ofVec3f target = circleloc + circleOffSet;
    acc += steer(target,false);		// Steer towards it
}

void Boid::checkBounds(){
    if ( x < space->xLeft) x = space->xRight;
	if ( x > space->xRight) x = space->xLeft;
	
    if ( y < space->yTop) y = space->yBottom;
	if ( y > space->yBottom) y = space->yTop;
	
	if ( z > space->zFront) space->zBack;
	if ( z < space->zBack) space->zFront;
}

ofVec3f Boid::steer(ofVec3f target, bool arrival) {
    ofVec3f steer; 
	if (!arrival){
		steer = target - *this;
		steer.limit(maxforce);
	} else {
		ofVec3f targetOffset = target - *this;
		float distance = targetOffset.length();
		float rampedSpeed = maxspeed*(distance/100);
		float clippedSpeed = min(rampedSpeed, maxspeed);
		ofVec3f desiredVelocity = targetOffset * (clippedSpeed/distance);
		steer = desiredVelocity - vel;
	}
    return steer;
}

ofVec3f Boid::avoid(ofVec3f target, bool weight) {
    ofVec3f steer = *this - target;
	float d = steer.length();
	
    if (weight)
		steer *= 1/(d*d);
	
	return steer;
}

//------------------------------------------------------ Render

void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	ofVec3f normal = ((b - a).cross(c - a)).normalize();
	mesh.addNormal(normal);
	mesh.addVertex(a);
	mesh.addNormal(normal);
	mesh.addVertex(b);
	mesh.addNormal(normal);
	mesh.addVertex(c);
}

void Boid::draw() {
    
    ofPushStyle();
    
    ofSetColor(color);
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    addFace(mesh, ofVec3f(3*sc,0,0), ofVec3f(-3*sc,2*sc,0), ofVec3f(-3*sc,-2*sc,0));
    addFace(mesh, ofVec3f(3*sc,0,0), ofVec3f(-3*sc,2*sc,0), ofVec3f(-3*sc,0,2*sc));
    addFace(mesh, ofVec3f(3*sc,0,0), ofVec3f(-3*sc,0,2*sc), ofVec3f(-3*sc,-2*sc,0));
    addFace(mesh, ofVec3f(-3*sc,0,2*sc), ofVec3f(-3*sc,2*sc,0), ofVec3f(-3*sc,-2*sc,0));
	ofPushMatrix();
	ofTranslate(*this);
	ofRotateY(ofRadToDeg(atan2(-vel.z,vel.x)));
	ofRotateZ(ofRadToDeg(asin(vel.y/vel.length())));
    
    mesh.drawFaces();
    
    ofPopMatrix();
    ofPopStyle();
}

//-------------------------------------------------------- Asking
bool Boid::isOver(int x, int y){
	GLdouble _x = 0;
	GLdouble _y = 0;
	GLdouble _z = 0;
	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
	
	gluProject(x, y, z,
			   mvmatrix, projmatrix, viewport,
			   &_x, &_y, &_z);
	
	_y = ofGetHeight() - _y;
	
	if ( ofDist(x,y,_x,_y) <= sc * ofMap(z,0,-5000,9,1) ) selected = true;
	else selected = false;
	
	return selected; 
}