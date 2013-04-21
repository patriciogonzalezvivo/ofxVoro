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
	space->xLeft = 0;
	space->xRight = ofGetWidth();
	space->yTop = 0;
	space->yBottom = ofGetHeight();
	space->zFront = 0;
	space->zBack = 1000;
	
    color.set(255,0,0);
	
    loc.set(500,500,500);
	vel.set(ofRandom(-0.1,0.1),
            ofRandom(0.1,-0.1),
            ofRandom(0.1,-0.1));
	acc.set(0,0,0);
    
    
	sc = ofRandom(1,3);
    maxspeed = ofRandom(0.25,0.75);
    neighborhoodRadius = 70*sc;
    maxforce = 0.1;
	
	selected	= false;
	
	wanderTheta = 0.1;
}
//-------------------------------------------------------------- Movement
void Boid::update(Boid * b) {
	// Evitar las paredes
	acc += 5 * avoid(ofVec3f(space->xLeft,loc.y,loc.z),true);
	acc += 5 * avoid(ofVec3f(space->xRight,loc.y,loc.z),true);
	acc += 5 * avoid(ofVec3f(loc.x,space->yTop,loc.z),true);
	acc += 5 * avoid(ofVec3f(loc.x,space->yBottom,loc.z),true);
	acc += 5 * avoid(ofVec3f(loc.x,loc.y,space->zFront),true);
	acc += 5 * avoid(ofVec3f(loc.x,loc.y,space->zBack),true);
	
    flock(b);

	move();
	checkBounds();
}

void Boid::flock(Boid * b) {
	sep = separate(b);
	ali = align(b);
	coh = cohesion(b);
	
	acc += ali * 1;
	acc += coh * 3;
	acc += sep * 6;	//1;
}
 

void Boid::move(){
	vel += acc;
	vel.limit(maxspeed);
	loc += vel;
    acc *= 0;
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
    circleloc += loc;				// Make it relative to boid's location
    
    ofVec3f circleOffSet = ofVec3f(wanderR*cos(wanderTheta),0,wanderR*sin(wanderTheta));
    ofVec3f target = circleloc + circleOffSet;
    acc += steer(target,false);		// Steer towards it
}

void Boid::checkBounds(){
    if (loc.x < space->xLeft) loc.x = space->xRight;
	if (loc.x > space->xRight) loc.x = space->xLeft;
	
    if (loc.y < space->yTop) loc.y = space->yBottom;
	if (loc.y > space->yBottom) loc.y = space->yTop;
	
	if (loc.z > space->zFront) space->zBack;
	if (loc.z < space->zBack) space->zFront;
}

ofVec3f Boid::steer(ofVec3f target, bool arrival) {
    ofVec3f steer; 
	if (!arrival){
		steer = target -loc;
		steer.limit(maxforce);
	} else {
		ofVec3f targetOffset = target - loc; 
		float distance = targetOffset.length();
		float rampedSpeed = maxspeed*(distance/100);
		float clippedSpeed = min(rampedSpeed, maxspeed);
		ofVec3f desiredVelocity = targetOffset * (clippedSpeed/distance);
		steer = desiredVelocity - vel;
	}
    return steer;
}

ofVec3f Boid::avoid(ofVec3f target, bool weight) {
    ofVec3f steer = loc - target; 
	float d = steer.length();
	
    if (weight)
		steer *= 1/(d*d);
	
	return steer;
}

ofVec3f Boid::separate(Boid * b) {
    ofVec3f posSum = ofVec3f(0,0,0);
	ofVec3f repulse;
	
    for (int i = 0 ; i < *nBoids ; i++) {
		Boid other = b[i];
		
		ofVec3f repulse = loc - other.loc;
		float d = repulse.length();
		
		if ((d > 0) && (d <= neighborhoodRadius)) {
			repulse.normalize();
			repulse /= d;
			posSum += repulse;
		}
    }
	return posSum;
}

ofVec3f Boid::align(Boid * b) {
    ofVec3f velSum = ofVec3f(0,0,0); 
    int count = 0;
    for (int i = 0 ; i < *nBoids; i++) {
		Boid other = b[i];
		
		ofVec3f diff = loc - other.loc;
		float d = diff.length();
		
		if ((d > 0) && (d <= neighborhoodRadius)) {
			velSum += other.vel;
			count++;
		}
    }
	
    if (count > 0) {
		velSum /= (float)count;
		velSum.limit(maxforce);
    }
	
	return velSum;
}

ofVec3f Boid::cohesion(Boid * b) {
	ofVec3f posSum = ofVec3f(0,0,0); 
	ofVec3f steer = ofVec3f(0,0,0);
    int count = 0;
	
    for (int i = 0 ; i < *nBoids ; i++) {
		Boid other = b[i];
		
		ofVec3f diff = loc - other.loc;
		float d = diff.length();
		
		if ((d > 0) && (d <= neighborhoodRadius)) {
			posSum += other.loc; // Add location
			count++;
		}
    }
    if (count > 0)
		posSum /= (float)count;
	steer = posSum - loc;
	steer.limit(0.04);
	
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
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    addFace(mesh, ofVec3f(3*sc,0,0), ofVec3f(-3*sc,2*sc,0), ofVec3f(-3*sc,-2*sc,0));
    addFace(mesh, ofVec3f(3*sc,0,0), ofVec3f(-3*sc,2*sc,0), ofVec3f(-3*sc,0,2*sc));
    addFace(mesh, ofVec3f(3*sc,0,0), ofVec3f(-3*sc,0,2*sc), ofVec3f(-3*sc,-2*sc,0));
    addFace(mesh, ofVec3f(-3*sc,0,2*sc), ofVec3f(-3*sc,2*sc,0), ofVec3f(-3*sc,-2*sc,0));
	ofPushMatrix();
	ofTranslate(loc.x, loc.y, loc.z);
	ofRotateY(ofRadToDeg(atan2(-vel.z,vel.x)));
	ofRotateZ(ofRadToDeg(asin(vel.y/vel.length())));
    
    mesh.drawFaces();
    
    ofPopMatrix();
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
	
	gluProject(loc.x, loc.y, loc.z,
			   mvmatrix, projmatrix, viewport,
			   &_x, &_y, &_z);
	
	_y = ofGetHeight() - _y;
	
	if ( ofDist(x,y,_x,_y) <= sc * ofMap(loc.z,0,-5000,9,1) ) selected = true;
	else selected = false;
	
	return selected; 
}