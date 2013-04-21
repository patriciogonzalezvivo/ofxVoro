/*
 *  Boid.h
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

#ifndef BOID_H
#define BOID_H

#include "ofMain.h"

//Space variables. This need to be defined to contain the fish
typedef struct{
	int	xLeft;
	int	xRight;
	int	yTop;
	int	yBottom;
	int	zFront;
	int zBack;
} Space;

class Boid {
public:
	Boid();
	
	void	setNBoids(int * n){ nBoids = n;};
	void	setSpace(Space * s){ space = s;};
	
	void	flock(Boid * _b);
    
	void	wander();
	void	move();
	void	checkBounds();
	
	ofVec3f	steer(ofVec3f target, bool slowdown);
	ofVec3f	avoid(ofVec3f target, bool slowdown);
	
	ofVec3f	separate(Boid * b);
	ofVec3f	align(Boid* b);
	ofVec3f	cohesion(Boid * b);
	
	void	seek(ofVec3f target) { acc += steer(target,false); }
	void	arrive(ofVec3f target) { acc += steer(target,true); };
    
    void	update(Boid * _b);
	void	draw();
    
	bool	isOver(int x, int y);
	bool	isOver(ofVec2f _pos){isOver(_pos.x,_pos.y);};
    
    ofVec3f	loc, vel, acc;
	ofVec3f	ali, coh, sep;
	float	neighborhoodRadius, wanderTheta, sc, maxforce, maxspeed;
	bool	selected;
    
    ofColor color;
    
private:
    int*	nBoids;
    Space*	space;
};

#endif