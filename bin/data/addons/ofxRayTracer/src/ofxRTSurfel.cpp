#include "ofxRTSurfel.h"

ofxRTSurfel::ofxRTSurfel(const glm::vec3& _faceNormal, const glm::vec3& _rayDirection, const glm::vec3 _position, const ofFloatColor _color){
    geometricNormal = _faceNormal;
    shadingNormal = _faceNormal; // TODO, this should be calculated, for example from a bump map
    position = _position;

    if(glm::dot(_faceNormal, _rayDirection) >= 0){
        backface = true;
    }
    color = glm::vec3(_color.r,_color.g,_color.b);
}

glm::vec3 ofxRTSurfel::emittedRadiance(glm::vec3 wo) const {

    return glm::vec3(0.0f,0.0f,0.0f); //material is not emissive
};

glm::vec3 ofxRTSurfel::finiteScatteringDensity(const glm::vec3& w_i, const glm::vec3& w_o) const {
    //easy for now. TODO, implement different reflections
    bool reflective_on = false; // R�flexion activ�e pour l'exemple, vous pouvez ajuster cette condition selon vos besoins
    bool refractive_on = false; // R�fraction activ�e pour l'exemple, vous pouvez ajuster cette condition selon vos besoins

    if (reflective_on && refractive_on) {
        // Si le mat�riau est � la fois r�fl�chissant et r�fractif, combinez les deux effets
        return 0.5f * (reflectiveDensity(w_i, w_o) + refractiveDensity(w_i, w_o));
    } else if (reflective_on) {
        // Si le mat�riau est uniquement r�fl�chissant, calculez la densit� de r�flexion
        return reflectiveDensity(w_i, w_o);
    } else if (refractive_on) {
        // Si le mat�riau est uniquement r�fractif, calculez la densit� de r�fraction
        return refractiveDensity(w_i, w_o);
    } else {

        glm::vec3 reflectionDirection = glm::reflect(-w_i, getGeometricNormal());
        if (glm::dot(reflectionDirection, w_o) > 0) {
              return lambertianReflectivity();
        } else {

              return glm::vec3(0.5f,0.5f,0.5f);
          	}
   	}
}

glm::vec3 ofxRTSurfel::getColor() const {
    return color;
}


glm::vec3 ofxRTSurfel::lambertianReflectivity() const {
	return glm::vec3(1.0f,1.0f,1.0f);
    //return color;
}

glm::vec3 ofxRTSurfel::refractedDirection(const glm::vec3& w_i) const {
    float n = 1.5f; // Indice de r�fraction du mat�riau
    glm::vec3 normal = getGeometricNormal(); // Normal � la surface
    float cosThetaI = glm::dot(w_i, normal);
    float sinThetaI = glm::sqrt(1 - cosThetaI * cosThetaI);
    float sinThetaT = sinThetaI / n; // Loi de Snell
    if (sinThetaT > 1) return glm::vec3(0.0f); // R�flexion totale interne
    float cosThetaT = glm::sqrt(1 - sinThetaT * sinThetaT);
    glm::vec3 w_t = -n * w_i + (n * cosThetaI - cosThetaT) * normal; // Vecteur de r�fraction
    return w_t; // Le mat�riau est r�fractif
}

glm::vec3 ofxRTSurfel::reflectiveDensity(const glm::vec3& w_i, const glm::vec3& w_o) const {

    float kr = 0.1; // Coefficient de r�flexion sp�culaire
    glm::vec3 reflectionDirection = glm::reflect(-w_i, getGeometricNormal());
    float cosineTerm = glm::max(glm::dot(reflectionDirection, w_o), 0.0f);
    return kr * color * cosineTerm;
}

glm::vec3 ofxRTSurfel::refractiveDensity(const glm::vec3& w_i, const glm::vec3& w_o) const {
    // R�fraction bas�e sur la loi de Snell-Descartes et la formule de Fresnel
    // Vous devrez ajuster les valeurs des coefficients selon vos besoins
    float kt = 0.8; // Coefficient de transmission
    float n1 = 1.0; // Indice de r�fraction du milieu incident (air)
    float n2 = 1.5; // Indice de r�fraction du milieu de sortie (par exemple, verre)
    glm::vec3 normal = getGeometricNormal();
    float cosThetaI = glm::dot(-w_i, normal);
    float eta = n1 / n2;
    float cosThetaT = 1.0 - eta * eta * (1.0 - cosThetaI * cosThetaI);
    if (cosThetaT > 0.0) {
        glm::vec3 transmissionDirection = eta * w_i + (eta * cosThetaI - sqrt(cosThetaT)) * normal;
        float cosineTerm = glm::max(glm::dot(transmissionDirection, w_o), 0.0f);
        return kt * color * cosineTerm;
    } else {
        // R�flexion totale interne, retourne une densit� de r�flexion totale
        return reflectiveDensity(w_i, w_o);
    }
}

