/*
 *  boid.cpp
 *  boids
 *
 *  Created by Marco Gillies on 05/10/2010.
 *  Copyright 2010 Goldsmiths, University of London. All rights reserved.
 *
 */

#include "boid.h"
#include "ofMain.h"
#include "testApp.h"

Boid::Boid()
{
	separationWeight = 1.0f;
	cohesionWeight = 0.2f;
	alignmentWeight = 0.1f;
	
	separationThreshold = 15;
	neighbourhoodSize = 100;
	
	position = ofVec3f(ofRandom(0, 200), ofRandom(0, 200));
	velocity = ofVec3f(ofRandom(-2, 2), ofRandom(-2, 2));

}

Boid::Boid(ofVec3f &pos, ofVec3f &vel)
{

	separationWeight = 1.0f;
	cohesionWeight = 0.2f;
	alignmentWeight = 0.1f;
	
	separationThreshold = 15;
	neighbourhoodSize = 100;
	
	position = pos;
	velocity = vel;
}

Boid::~Boid()
{
	
}

float Boid::getSeparationWeight()
{
	return separationWeight;
}
float Boid::getCohesionWeight()
{
	return cohesionWeight;
}

float Boid::getAlignmentWeight()
{
	return alignmentWeight;
}


float Boid::getSeparationThreshold()
{
	return separationThreshold;
}

float Boid::getNeighbourhoodSize()
{
	return neighbourhoodSize;
}


void Boid::setSeparationWeight(float f)
{
	separationWeight = f;
}
void Boid::setCohesionWeight(float f)
{
	cohesionWeight = f;
}

void Boid::setAlignmentWeight(float f)
{
	alignmentWeight = f;
}


void Boid::setSeparationThreshold(float f)
{
	separationThreshold = f;
}

void Boid::setNeighbourhoodSize(float f)
{
	neighbourhoodSize = f;
}


ofVec3f Boid::getPosition()
{

	return position;
}

ofVec3f Boid::getVelocity()
{
	return velocity;
}

ofVec3f Boid::separation(std::vector<Boid *> &otherBoids)
{
	// finds the first collision and avoids that
	// should probably find the nearest one
	// can you figure out how to do that?
	for (int i = 0; i < otherBoids.size(); i++)
	{	
		if(position.distance(otherBoids[i]->getPosition()) < separationThreshold)
		{
			ofVec3f v = position -  otherBoids[i]->getPosition();
			v.normalize();
			return v;
		}
	}
}

ofVec3f Boid::cohesion(std::vector<Boid *> &otherBoids)
{
	ofVec3f average(0,0,0);
	int count = 0;
	for (int i = 0; i < otherBoids.size(); i++)
	{
		if (position.distance(otherBoids[i]->getPosition()) < neighbourhoodSize)
		{
			average += otherBoids[i]->getPosition();
			count += 1;
		}
	}
	average /= count;
	ofVec3f v =  average - position;
	v.normalize();
	return v;
}

ofVec3f Boid::alignment(std::vector<Boid *> &otherBoids)
{
	ofVec3f average(0,0,0);
	int count = 0;
	for (int i = 0; i < otherBoids.size(); i++)
	{
		if (position.distance(otherBoids[i]->getPosition()) < neighbourhoodSize)
		{
			average += otherBoids[i]->getVelocity();
			count += 1;
		}
	}
	average /= count;
	ofVec3f v =  average - velocity;
	v.normalize();
	return v;
}

void Boid::update(std::vector<Boid *> &otherBoids, ofVec3f &min, ofVec3f &max)
{
	velocity += separationWeight*separation(otherBoids);
	velocity += cohesionWeight*cohesion(otherBoids);
	velocity += alignmentWeight*alignment(otherBoids);
	velocity.limit(5);

	
	walls(min, max);
	position += velocity;

	
}

void Boid::walls(ofVec3f &min, ofVec3f &max)
{
	if (position.x < min.x){
		position.x = min.x;
		velocity.x *= -1;
	} else if (position.x > max.x){
		position.x = max.x;
		velocity.x *= -1;
	}
	
	if (position.y < min.y){
		position.y = min.y;
		velocity.y *= -1;
	} else if (position.y > max.y){
		position.y = max.y;
		velocity.y *= -1;
	}
	
	
}

void Boid::draw()
{
	rotate3++;
	rando1++;

	ofBoxPrimitive ico;
	ico.set(50);
	
	getPosition();
	
	ofSetColor(0, 0, 0, 20);

	ico.setPosition(position.x, position.y, 0);
	ico.rotate(rotate3 * 50, 1.0, 0.0, 0.0);
	ico.rotate(rotate3 * 50, 0.0, 1.0, 0.0);
	

	// get all the faces from the icoSphere, handy when you want to copy
	// individual vertices or tweak them a little ;)
	vector<ofMeshFace> triangles = ico.getMesh().getUniqueFaces();


	
	
	ico.drawWireframe();
	
	
	
}


void Boid2::draw() {

	
	rotate++;
	rando1++;
 
	ofConePrimitive cone;
	cone.set(15, ((position.x + position.y) / 8), 8, 8);
	getPosition();

	//ofNoFill();
	ofSetColor(0, 0, 255, 55);    // this is a shortcut for full blue color ;)
	
	cone.setPosition(position.x, position.y, 0);
	cone.rotate(rotate * 5, 1.0, 0.0, 0.5);
	cone.rotate(-rotate * 3, 0, 1.0, 0.0);
	//cone.rotate(-rotate * 5, 0, 0.0, 1.0);

	vector<ofMeshFace> triangles = cone.getMesh().getUniqueFaces();
	
	
	cone.drawWireframe();


	//cone.draw();
	//ofPushMatrix();
	//ofTranslate(position.x /4, position.y /4);
	//ofRotateXDeg(rotate /4); // THIS IS WHAT IS BROKEN!!!!
	//ofRotateYDeg(rotate /4); // THIS IS WHAT IS BROKEN!!!!
	//ofRotateZDeg(rotate /4); // THIS IS WHAT IS BROKEN!!!!
	//ofTranslate(-position.x / 4, -position.y /4);
	//ofPopMatrix();
	//ofDrawCone(position.x, position.y, 0.0, 20.0, 45.0);
}

