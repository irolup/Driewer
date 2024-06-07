#pragma once

#include "ofMain.h"
#include <vector>

class LightShaderManager {

public:
    
    void load(string shaderName);
    
    void begin();
    void end();
    void reload();

    void setMaterial(ofMaterial * p_mat){ mat = p_mat; }
    void setCamera(ofCamera * p_cam) { cam = p_cam; }
    void setTexture(ofImage * p_img) {tex = p_img; }
    void setNormalMapTexture(ofImage * p_img) {normTex = p_img; };
    void setMetalTexture(ofImage * p_img) {metal = p_img; };
    void setRoughnessTexture(ofImage * p_img) {roughness = p_img; };
    void setOcculsionTexture(ofImage * p_img) {occulsion = p_img; };
    
    void useLight(ofLight * p_light);
    void removeLight(ofLight * p_light);
    
    void toggleLight(ofLight * p_light, bool state);
    void toggleTexture(ofImage * p_img);
    
    string shaderName(){return _shaderName;}
    
    bool usingLight(ofLight* p_light);
    
    void setupLights();
    vector<string> generateLightPropsNames();
    void setLightPosition(size_t p_lightIndex, vector<unsigned char> & p_buffer, const GLint * p_offsets);
    void setLightColors(size_t p_lightIndex, vector<unsigned char> & p_buffer, const GLint * p_offsets);
    void setLightAttenuation(size_t p_lightIndex, vector<unsigned char> & p_buffer, const GLint * p_offsets);
    void setLightSpotProperties(size_t p_lightIndex, vector<unsigned char> & p_buffer, const GLint * p_offsets);
    void setupFixedPipelineLights();
    
    void setupMaterial();
    void setMaterialProperties(vector<unsigned char> & p_buffer, const GLint * p_offsets);
    
    ofShader* getShader() {return &shader; }


    //section for the materials
    ofColor material_color_ambient;
    ofColor material_color_diffuse;
    ofColor material_color_specular;
    glm::vec3 material_fresnel_ior;
    
    float material_metallic;
    float material_roughness;
    float material_occlusion;
    float material_brightness;

    //section for the light
    float light_intensity;
    ofColor light_color;
    glm::vec3 light_position;
    
    //section for the tone mapping
    float tone_mapping_exposure;
    float tone_mapping_gamma;
    bool tone_mapping_toggle;

    
protected:
    
    ofShader shader;
    
    vector<ofLight *> lights;
    
    ofMaterial defaultMat;
    ofCamera * cam;
    ofMaterial * mat;
    ofImage * tex;
    ofImage * normTex;
    ofImage * metal;
    ofImage * roughness;
    ofImage * occulsion;
    
    ofMatrix4x4 normalMatrix;
    
    string _shaderName;
    
    static const size_t MAX_LIGHTS = 8;
    static const size_t LIGHT_PROPS_NUMBER = 11; // Number of light properties in the shaders
    
};
