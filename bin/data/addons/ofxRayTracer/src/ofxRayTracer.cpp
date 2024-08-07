#include "ofxRayTracer.h"
ofxRayTracer::ofxRayTracer(){};

void ofxRayTracer::setup(const vector<of3dPrimitive>& _primitives, const vector<ofMaterial>& _materials, const vector<ofLight*>& _lights, const float& _ambientTermBias){
    primitives = _primitives;
    materials = _materials;
    lights = _lights;
    ambientTerm = _ambientTermBias;
};

// TODO, Textures
// https://bheisler.github.io/post/writing-raytracer-in-rust-part-3/

// C++ Ray Casting implementation following http://graphicscodex.com
void ofxRayTracer::traceImage(const ofxRTPinholeCamera& camera, ofRectangle& rectangle, shared_ptr<ofImage>& image, bool& parallel, const int& n_rays) {
    const int width = int(rectangle.getWidth());
    const int height = int(rectangle.getHeight());
    auto startAtTime = ofGetElapsedTimeMillis();
    ofPixels renderPixels;
    renderPixels.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
    int tot_samples = camera.getSamplesPerPixel();

    if (!parallel) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                glm::vec4 averageColor;
                for (int sample = 0; sample < tot_samples; sample++) {
                	//P est la pos du lancement de rayon
                	//glm::vec3 P = glm::vec3(0.0f, 0.5f, -3.0f);
                    glm::vec3 P;
                    glm::vec3 w;

                    // Generate random offset within the pixel
                    double xx = x + random() / (double)RAND_MAX;
                    double yy = y + random() / (double)RAND_MAX;

                    // Get primary ray towards the center of the scene that is p and w w is normalize direction
                    camera.getPrimaryRay(xx + 0.5f, yy + 0.5f, width, height, P, w);

                    // Calculate the color of the pixel intersected by the ray
                    auto color = L_i(ofxRTRay(P, w), n_rays);
                    averageColor.x += color.r;
                    averageColor.y += color.g;
                    averageColor.z += color.b;
                    averageColor.w += color.a;
                }

                auto finalCol = averageColor * (1 / (double)tot_samples);
                ofColor finalColor = ofColor(finalCol.r, finalCol.g, finalCol.b, finalCol.a);
                renderPixels.setColor(x, y, finalColor);
            }
        }
    } else {
    }       

    image->setFromPixels(renderPixels);
    displayTime(ofGetElapsedTimeMillis() - startAtTime);
}

// This method return the incoming light for X.
ofColor ofxRayTracer::L_i(const ofxRTRay& ray, const int n_rays) const{
    // for all the triangles in a mesh
    // Find the first intersection (and the closest!) with the scene
    const shared_ptr<ofxRTSurfel>& surfelY = findFirstIntersectionWithThePrimitives(ray);
    if (surfelY) {
        // Debugging implementation that computes white if there is any surface on this ray and black when there is not.
        // return ofColor(255,255,255);
        return L_0(surfelY, -ray.direction, n_rays);
    } else {
        return ofColor(0,0,0);
    }
}

// This method find the first intersection between a ray and a collection of meshes.
// If an intersection is founded, it returns a surfel, otherwise null.
shared_ptr<ofxRTSurfel> ofxRayTracer::findFirstIntersectionWithThePrimitives(const ofxRTRay& ray) const{
    // at the beginning, no intersection is found and the distance to the closest surface
    // is set to an high value;
    bool found = false;
    int meshesIndex = 0;
    float distanceToTheClosestSurface = std::numeric_limits<float>::max();
    glm::vec3 faceNormal;
    glm::vec3 position;
    glm::vec3 rayDirection;
    ofFloatColor color;
    // then we iterate through all the triangles in all the meshes, searching
    // for the closest intersection
    for (const of3dPrimitive& primitive : this->primitives) {
        for (const ofMeshFace& face : primitive.getMesh().getUniqueFaces()) {
            float distance;
            glm::vec2 baricenter;
            bool intersection = glm::intersectRayTriangle(
                                    glm::vec3(ray.origin), 
                                    glm::vec3(ray.direction),
                                    glm::vec3(primitive.getGlobalTransformMatrix() * glm::vec4(face.getVertex(0), 1.f)),
                                    glm::vec3(primitive.getGlobalTransformMatrix() * glm::vec4(face.getVertex(1), 1.f)),
                                    glm::vec3(primitive.getGlobalTransformMatrix() * glm::vec4(face.getVertex(2), 1.f)),
                                    baricenter,
                                    distance);
            // when an intersection is found, it updates the distanceToTheClosestSurface value
            // this value is used to order the new intersections, if a new intersection with a smaller baricenter.z
            // value is found, this one will become the new intersection
            if (intersection) {
                if (distance < distanceToTheClosestSurface) {
                    found = true;
                    color = getColor(face, meshesIndex);
                    distanceToTheClosestSurface = distance;
                    faceNormal = face.getFaceNormal();
                    glm::vec3 baryPosition(baricenter.x, baricenter.y, distance);
                    position = getPointOnTriangle(ray, baryPosition);
                    rayDirection = ray.direction;
                }
            }
        }
        meshesIndex++;
    }

    if (found) {
        return shared_ptr<ofxRTSurfel>(new ofxRTSurfel(faceNormal, rayDirection, position, color));
    } else {
        return nullptr;
    }
};

// It computes the light leaving Y, which is the same as
// the light entering X when the medium is non-absorptive
ofColor ofxRayTracer::L_0(const shared_ptr<ofxRTSurfel>& surfelY, const glm::vec3 wo, const int n_rays) const{
    // as emitted Radiance is 0, for now, I will just caclulate the direct scattered radiance
    //return surfelX->emittedRadiance(wo) + L_scatteredDirect(surfelX, wo);
    return L_scatteredDirect(surfelY, wo, n_rays);
}

/*
 From the chapter "Direct Illumination":
 Note that actual parameter surfelY is now called surfelX in method L_scatteredDirect.
 I changed the frame of reference by advancing one step closer to the light along a
 transport path. As in all equations so far, X is the point at which radiance is being
 scattered and Y is the next node closer to the light on the light transport path.
*/
ofFloatColor ofxRayTracer::L_scatteredDirect(const shared_ptr<ofxRTSurfel>& surfelX,const glm::vec3 wo, const int n_rays) const{
    glm::vec3 Light = surfelX->emittedRadiance(wo);
    for (size_t i = 0; i < lights.size(); i++) {
        glm::vec3 lightPos = lights[i]->getGlobalPosition();
        glm::vec3 offset = lightPos - surfelX->getPosition();
        const float distanceToLight = glm::length(offset);
        glm::vec3 wi = glm::normalize(offset);
        glm::vec3 color = surfelX->getColor();
        glm::vec3 vecAmbientLight = getAmbientLight(color);

        if (visible(surfelX->getPosition(), wi, distanceToLight)) {
            // light power is not implemented in ofLight,
            // I use a getDiffuseColor().getBrightness() for this
            // TODO. put lightPower in a constant or in the config.
            float lightPower = lights[i]->getDiffuseColor().getBrightness() * 30;
            float biradiance = lightPower / (4 * PI * sqrt(distanceToLight));

            //implement indirect rays only if the number of indirect rays is bigger than zero
            if (n_rays > 0) {
                for(int i = 0; i < n_rays; i++){
                    glm::vec3 rdir = getRandomDir();
                    float acos = glm::dot(wi, rdir);
                    if(acos > 0){
                        float dProd = abs(glm::dot(wi, surfelX->getGeometricNormal()));
                        //glm::vec3 refractedDirection = surfelX->refractedDirection(rdir);
                        glm::vec3 finiteScatteringDensity = surfelX->finiteScatteringDensity(rdir, wo);
                        //glm::vec3 finiteScatteringDensity = surfelX->finiteScatteringDensity(refractedDirection, wo);
                        Light +=
                        //vecAmbientLight + this is not needed when calculating
                        // indirect lights because they are already adding enough light
                        biradiance/(n_rays/2) * // comment out this when debugging
                        finiteScatteringDensity *
                        glm::vec3( dProd ) * color;
                    }
                }
            } else {
                float dProd = abs(glm::dot(wi, surfelX->getGeometricNormal()));
                glm::vec3 refractedDirection = surfelX->refractedDirection(wi);
                glm::vec3 finiteScatteringDensity = surfelX->finiteScatteringDensity(wi, wo);
                //glm::vec3 finiteScatteringDensity = surfelX->finiteScatteringDensity(refractedDirection, wo);
                Light +=
                vecAmbientLight +
                biradiance * // comment out this when debugging
                finiteScatteringDensity *
                glm::vec3( dProd ) * color;
            }
        } else {
            Light += vecAmbientLight ;
            //Light = shadowRay(surfelX,wo,n_rays);
            //shadow
            // http://graphicscodex.com/projects/rays/index.html cerca "For a shadow ray"
        }
    }
    return ofFloatColor(Light.x, Light.y, Light.z);
};

bool ofxRayTracer::visible(const glm::vec3& P, const glm::vec3& direction, const float& distance) const{
    const ofxRTRay ray = ofxRTRay(P + direction * CONST_EPSILON, direction);

    float dist = distance - CONST_EPSILON;

    for (const of3dPrimitive& primitive : this->primitives) {
        for (const ofMeshFace& face : primitive.getMesh().getUniqueFaces()) {
            glm::vec2 baricenter;
            // Test each potential shadow caster to see if it lies between P and the light
            bool intersection = glm::intersectRayTriangle(
                                                          glm::vec3(ray.origin), 
                                                          glm::vec3(ray.direction),
                                                          glm::vec3(primitive.getGlobalTransformMatrix() * glm::vec4(face.getVertex(0), 1.f)),
                                                          glm::vec3(primitive.getGlobalTransformMatrix() * glm::vec4(face.getVertex(1), 1.f)),
                                                          glm::vec3(primitive.getGlobalTransformMatrix() * glm::vec4(face.getVertex(2), 1.f)),
                                                          baricenter,
                                                          dist);
            if (intersection) {
                if (baricenter.y < dist) {
                    // Ah! This triangle is closer than the light.
                    // Shadow!
                    return false;
                }
            }
        }
    }
    return true;
}

/*
 This method takes as argument a ray and the baricentric coordinates and returns
 the exact point on the triangle where the intersection happened.
 The variable values that are stored in the baryPosition vector need to be
 explained, because there is no documentation in the glm website for the
 glm::intersectRayTriangle method, but just in this github issue 
 https://github.com/g-truc/glm/issues/6

 the baryPosition output uses barycentric coordinates for the x and y components.
 The z component is the scalar factor for ray.

 That is,
 1.0 - baryPosition.x - baryPosition.y = actual z barycentric coordinate

 if you compute the point inside the triangle that corresponds to those barycentric coordinates, you'll find the exact same answer as if you did:
 origin + direction * baryPosition.z
*/

glm::vec3 ofxRayTracer::getPointOnTriangle(const ofxRTRay& _ray, const glm::vec3& _baryPosition) const {
    return _ray.origin + (_ray.direction * _baryPosition.z);
};

ofFloatColor ofxRayTracer::getColor(const ofMeshFace &face, int indexMeshes) const {
  
    
    ofFloatColor final_color_material;
    
    if (materials.size() >= static_cast<std::vector<ofMaterial>::size_type>(indexMeshes+1)){
    
    	final_color_material = materials.at(indexMeshes).getDiffuseColor();
    	//ajouter
    	//ofFloatColor est un vec4, se base sur normapPhong.frag de shaderlibrairy
    	//vec4 mat_ambient;         // material ambient color
	//vec4 mat_diffuse;         // material diffuse color
	//vec4 mat_specular;        // material specular
	//vec4 mat_emissive;        // material emissive
	//float mat_shininess;      // material shininess 
    	return final_color_material;
    } else if (face.hasColors()){
    	return face.getColor(1);
    } else {
    return ofFloatColor(1.0f,1.0f,1.0f);
    }
    
}

// TODO, move these method in an utility class
void ofxRayTracer::displayTime(uint64_t ellapsed) const {
    auto str = "ofxRayTracer: render time: " + ofToString(ellapsed/1000.0) + " seconds";
    cout << str << endl;
}

// http://corysimon.github.io/articles/uniformdistn-on-sphere/
glm::vec3 ofxRayTracer::getRandomDir() const{
    float x = ofRandom(-1.0f, 1.0f);
    float y = ofRandom(-1.0f, 1.0f);
    float z = ofRandom(-1.0f, 1.0f);
    return glm::normalize(glm::vec3(x, y, z));
}

glm::vec3 ofxRayTracer::getAmbientLight(const glm::vec3& color) const{
    return color * ambientTerm;
};


