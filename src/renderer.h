#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "LightShaderManager.h"
#include "ofxGui.h"
#include "ofxAssimpMeshHelper.h"
#include "ofxRayTracer.h"

enum class Camera {front, back};

class Renderer
{
public:

    ofShader shader;
    LightShaderManager shaderManager;


    void setupLights();

    ofLight pointLight;
	  ofLight spotLight;
	  ofLight directionalLight;
    ofLight ambientLight;
    ofLight directionalLight2;
    ofParameter<bool> bPointLight,  
                      bSpotLight, 
                      bDirLight, 
                      bAmbientLight;
    ofParameter<bool> bUseTexture;
    //light ambiant slider
    void pointLightChanged(bool & bPointLight);
    void spotLightChanged(bool & bSpotLight);
    void dirLightChanged(bool & bDirLight);
    void ambientLightChanged(bool & bAmbientLight);
    void textureToggled(bool & bUseTexture);

    ofVec3f getPointLightPosition();
    ofVec3f getSpotLightPosition();
    ofVec3f getDirectionalLightPosition();

    // Courbe de bezier
    bool showBezierSpline;
    bool showBezierSurface;


    void initializeBezierSpline();
    std::vector<ofPoint> bezierControlPoints;

    // Surface parametrique
    void initializeBezierSurface();
    std::vector<std::vector<ofPoint>> bezierSurfaceControlPoints;

    void draw_bezier_spline();
    void draw_bezier_surface();

    //Materiau
    ofMaterial material;
    //material vecteur de float nax rgba quon nomme mmaterial mais cest vecteur de couleur
    //Texture image pour le plane et cube
    ofImage colorTexture;
    ofImage normalTexture;
    ofImage metal_Texture;
    ofImage roughness_Texture;
    ofImage occlusion_Texture;


    //material
    ofMaterial material_1;
    ofVec4f mat_ambient;         // material ambient color
    ofVec4f mat_diffuse;         // material diffuse color
    ofVec4f mat_specular;        // material specular
    ofVec4f mat_emissive;        // material emissive
    float mat_shininess;      // material shininess

    //material 2
    ofMaterial material_2;
    ofVec4f mat_ambient_2;         // material ambient color
    ofVec4f mat_diffuse_2;         // material diffuse color
    ofVec4f mat_specular_2;        // material specular
    ofVec4f mat_emissive_2;        // material emissive
    float mat_shininess_2;      // material shininess

    //material 3
    ofMaterial material_3;
    ofVec4f mat_ambient_3;         // material ambient color
    ofVec4f mat_diffuse_3;         // material diffuse color
    ofVec4f mat_specular_3;        // material specular
    ofVec4f mat_emissive_3;        // material emissive
    float mat_shininess_3;      // material shininess

    //material 4 fond -z de la scene
    ofMaterial material_4;
    ofVec4f mat_ambient_4;         // material ambient color
    ofVec4f mat_diffuse_4;         // material diffuse color
    ofVec4f mat_specular_4;        // material specular
    ofVec4f mat_emissive_4;        // material emissive
    float mat_shininess_4;      // material shininess

    void setupMaterial();
    vector<ofMaterial> materials;
    vector<of3dPrimitive> primitives;
    vector<ofLight*> lights;
    void stock_material_primitive();

    //Scene
    ofNode center_of_scence;
    //Primitive geometrique
    ofSpherePrimitive sphere;
    float radius;
    float radius_cylinder;
    float height_cylinder;
    ofBoxPrimitive box;
    ofPlanePrimitive plane;
    ofCylinderPrimitive cylinder;
    //box cubemap
    ofBoxPrimitive cube_map_tracer;

    void update_texture_default();
    void update_texture_pbr();
    
    float scale_plane;
    void drawScene();
    void drawLights();
    //vector of position for the priumitive
    ofVec3f position_sphere;
    ofVec3f position_box;
    ofVec3f position_plane;
    ofVec3f position_cylinder;
    ofVec3f position_cube_map_tracer;
    
    //Info pour mettre objet sur la scene
    ofVec3f center;
    float center_x;
    float center_y;
    float rotation_speed;
    float scale;
    float scale_cube_map_tracer;

    //camm setup
    Camera camera_active;
    ofCamera camera_front;
    ofCamera camera_back;
    ofCamera* camera;
    //camera info
    ofQuaternion camera_orientation;
    ofVec3f camera_position;
    ofVec3f camera_target;
    string camera_name;
    string camera_projection;
    float camera_near;
    float camera_far;
    float camera_fov;
    float camera_fov_delta;
    float offset_camera;
    //camera control
    float speed_delta;
    float speed_translation;
    float speed_rotation;
    //time
    float time_current;
    float time_last;
    float time_elapsed;
    //camera move
    bool is_visible_frustum;
    bool is_camera_move_left;
    bool is_camera_move_right;
    bool is_camera_move_up;
    bool is_camera_move_down;
    bool is_camera_move_forward;
    bool is_camera_move_backward;
    bool is_camera_tilt_up;
    bool is_camera_tilt_down;
    bool is_camera_pan_left;
    bool is_camera_pan_right;
    bool is_camera_roll_left;
    bool is_camera_roll_right;
    bool is_camera_fov_narrow;
    bool is_camera_fov_wide;
    bool is_camera_perspective;
    
    //Light control
    bool is_light_move_up;
    bool is_light_move_down;
    bool is_light_move_left;
    bool is_light_move_right;
    bool is_light_move_forward;
    bool is_light_move_backward;
    
    ofVec3f light_position;
    ofVec3f light_target;

    //cubemap
    
    ofImage cubemapFaces[6];
    float cube_half_size;
    ofVec3f cube_position;
    void initialize_cubemap();
    void draw_cube_map();
    ofMesh faceMesh_cube_map;

    //Raytracer
    ofxRayTracer rayTracer;
    ofxRTPinholeCamera camera_raytracer;
    shared_ptr<ofImage> raytraced_image;
    shared_ptr<ofImage> init_raytraced_image(int width, int height);
    bool runInParallel = true;

    void light_update();
    void init_camera();
    void setup_camera();
    void cam_reset();
    void cam_update();
    void cam_draw_frustum();
    //fonc pour recuperer le pointeur vers la camera active
    void set_camera_active();

    //section pbr
    ofShader shader_pbr;
    ofColor material_color_ambient;
    ofColor material_color_diffuse;
    ofColor material_color_specular;
    float material_metallic;
    float material_roughness;
    float material_occlusion;
    float material_brightness;
    glm::vec3 material_fresnel_ior;
    ofImage texture_diffuse;
    ofImage texture_metallic;
    ofImage texture_roughness;
    ofImage texture_occlusion;
    ofImage texture_normal;
    ofLight light;
    ofColor light_color;
    float light_intensity;
    bool light_motion;
    float tone_mapping_exposure;
    float tone_mapping_gamma;
    bool tone_mapping_toggle;
    //shader update fonction
    ofSpherePrimitive sphere_pbr;
    ofVec3f position_sphere_pbr;
    //primtiives
    void setup_primitives();
    float scale_sphere;
    float initial_x;
    float initial_y;
    float initial_z; 
    float offset_z;

    //cylinder_Texture
    ofImage texture_cylinder_diffuse;
    ofImage texture_cylinder_metallic;
    ofImage texture_cylinder_roughness;
    ofImage texture_cylinder_occlusion;
    ofImage texture_cylinder_normal;

    //texture arbitrary
    ofImage texture_arbitrary_diffuse;
    ofImage texture_arbitrary_metallic;
    ofImage texture_arbitrary_roughness;
    ofImage texture_arbitrary_occlusion;
    ofImage texture_arbitrary_normal;
    void setup_texture_arbitrary();

    void pbr_setup();
    void pbr_reset();
    void pbr_update();
    void pbr_draw();
    void update_shader_pbr();
    void update_shader_pbr_second_texture();

    //section tesselation
    bool show_tess_sphere;
    bool show_tess_cube;
    ofShader shader_tessellation;
    ofVbo sphereVbo, boxVbo;
    float radius_tess;
    ofVec3f center_tess;
    ofVec3f position_sphere_tess;
    ofVec3f position_box_tess;
    float tessLevelInner, tessLevelOuter;
    void setup_tessellation();
    void setupVbos();
    void drawScene_tess();
    void draw_tess_sphere();
    void draw_tess_cube();

    void setup_potato_pc();
    void draw_potato_pc();
    
    //section for the materials
    ofColor material_color_ambient_protedted;

    void setup();
    void reset();
    void update();
    void update_potato_pc();
    void draw();
    
    void exit();
};
