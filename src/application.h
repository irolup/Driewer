#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "renderer.h"


class Application : public ofBaseApp
{
public:

  Renderer renderer;

  //Gui section
  ofxPanel gui;


  
//parametres
  ofParameterGroup lightParameters;
  
  ofParameter<int> spotCutOff; //0-90 degrees
  ofParameter<int> spotConcentration;// 0 - 128



  ofxButton courbe_bezier_button;
  ofxButton surface_parametrique_button;
  void courbe_bezier_button_pressed();
  void surface_parametrique_button_pressed();
  int selectedControlPointIndex = -1;
  
  ofParameterGroup materialParameters;
  vector<float> matetial_vector;
  ofParameter<ofFloatColor> diffuse;
  ofParameter<ofFloatColor> ambient;
  ofParameter<ofFloatColor> specular;
  ofParameter<ofFloatColor> emissive;
  ofParameter<float> shininess;//0-128
  
  //key
  bool is_key_press_w;
  bool is_key_press_s;
  bool is_key_press_a;
  bool is_key_press_d;
  bool is_key_press_q;
  bool is_key_press_e;
  bool is_key_press_j;
  bool is_key_press_u;
  bool is_key_press_h;
  bool is_key_press_k;
  bool is_key_press_y;
  bool is_key_press_i;
  bool is_key_press_z;
  bool is_key_press_x;
  bool is_key_press_m;
  bool is_key_press_n;

  //Key for controlling light position
  bool is_key_press_up;
  bool is_key_press_down;
  bool is_key_press_left;
  bool is_key_press_right;
  bool is_key_press_page_up;
  bool is_key_press_page_down;

  //section pbr
  //pbr section
  ofxPanel gui_pbr;
  ofxGuiGroup group_material_color;
  ofxGuiGroup group_material_factor;
  ofxGuiGroup group_light;
  ofxGuiGroup group_tone_mapping;
  ofParameter<ofColor> color_picker_ambient;
  ofParameter<ofColor> color_picker_diffuse;
  ofParameter<ofColor> color_picker_specular;
  ofParameter<float> slider_metallic;
  ofParameter<float> slider_roughness;
  ofParameter<float> slider_occlusion;
  ofParameter<float> slider_brightness;
  ofParameter<glm::vec3> slider_fresnel_ior;
  ofParameter<ofColor> color_picker_light_color;
  ofParameter<float> slider_light_intensity;
  ofParameter<float> slider_exposure;
  ofParameter<float> slider_gamma;
  ofParameter<bool> toggle_tone_mapping;
  ofParameter<bool> toggle_ui;
  ofParameter<bool> toggle_light_motion;
  ofxButton button_reset;
  void button_reset_pressed();
  float time_current;
  float time_last;
  float time_elapsed;

  //section pbr function
  void pbr_setup();
  void pbr_reset();
  void pbr_update();
  void pbr_draw();
  void update_material_pbr();

  //section tesselation
  ofParameter<int> tessLevelInner, tessLevelOuter;
  void update_tessellation();

  void setup();
  void update();
  void draw();
  void reset();

  void keyPressed(int key);
  void keyReleased(int key);

  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);

  void update_key_camera();
  void update_key_light();
  void updateLights();
  void updateMaterial();
  void setupGui();

  void exit();
};
