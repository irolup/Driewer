#include "ofxRTPinholeCamera.h"

//TODO, provide an option struct to the constructor

ofxRTPinholeCamera::ofxRTPinholeCamera(){
    //mettre z a -5 zoom sur le modele dans le rendu donc 5 plus loin et -5 plus proche
    centerOfProjection = glm::vec3(0.0f, 0.0f, 0.0f);
    z_near =  -1.0;
    verticalFieldOfView = PI/3; //donne 60
    //on tire rayon de 1 vers 0.0
    //side est la longueur de plan de coupe qui donne -1.15
    side = -2.0f * tan(verticalFieldOfView / 2.0f); //
}

void ofxRTPinholeCamera::draw(){
    ofDrawSphere(centerOfProjection.x, centerOfProjection.y, centerOfProjection.z, 10);
}

//The camera must convert a pixel position in the image to a point on the near clipping plane,
//and then to the ray that passes through that point from the center of projection.

// C++ A Camera Model [_rn_camera] from http://graphicscodex.com
/* x, y, width, and height in pixels; P in meters */
void ofxRTPinholeCamera::getPrimaryRay(double x, double y, int width, int height, glm::vec3& P, glm::vec3& w) const {
    // I invert the y-axis because we're moving from the 2D y=down to the 3D y=up coordinate system
    P = glm::vec3( z_near * (x / width - 0.5f) * side * width / height,
               z_near * -(y / height - 0.5f) * side,
               z_near);

    // The incoming direction is simply that from the origin to P
    glm::vec3 dir = P - centerOfProjection;
    w = glm::normalize(dir);
}

int ofxRTPinholeCamera::getSamplesPerPixel() const{
    return total_samples;
};

void ofxRTPinholeCamera::setSamples(int n_samples){
    total_samples = n_samples;
};

