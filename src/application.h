#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "renderer.h"
#include "ofxDatGui.h"
#include "LightShaderManager.h"

struct imgDimension{
  int width;
  int height;
};

struct menu_raytracer_options{
  imgDimension resolution;
  int nIndirectRays;
  float ambientBias;
  int samples;
};

class Application : public ofBaseApp
{
public:

  Renderer renderer;
  LightShaderManager light_shader_manager;

  //Gui section
  ofxPanel gui;

  //Pc render setting
  ofParameterGroup settings_parameters;
  ofParameter<bool> b_potato_pc;
  bool potato_pc;
  void specs_changed(bool & b_potato_pc);
  
  
  //parametres
  ofParameterGroup lightParameters;
  
  ofParameter<int> spotCutOff; //0-90 degrees
  ofParameter<int> spotConcentration;// 0 - 128




  
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
  void pbr_draw_gui();
  void update_material_pbr();

  //section tesselation
  ofParameter<int> tessLevelInner, tessLevelOuter;
  void update_tessellation();

  //Section menus with Datgui
  //Gui tracer
  ofxDatGui* menu_raytracer;
  void onResolutionEvent(ofxDatGuiDropdownEvent e);
  void onRenderEvent(ofxDatGuiButtonEvent e);
  void onSliderEvent(ofxDatGuiSliderEvent e);
  void onToggleEvent(ofxDatGuiToggleEvent e);
  map<int, imgDimension> availableResolution;
  map<int, imgDimension> prepare_raytracer_resolution();
  ofParameter<float> ambientBias = 0.2;
  ofParameter<int> samples = 8;
  bool runInParallel;
  menu_raytracer_options raytracer_options;
  void setup_menu_raytracer();
  void start_raytracer(menu_raytracer_options opt);

  //Gui pour choix shaders
  ofxDatGui* menu_graphics;
  void setup_menu_graphics();
  vector<string> available_shader;
  ofxDatGuiDropdown* shader_dropdown;
  void onShaderEvent(ofxDatGuiDropdownEvent e);

  //Gui pour choix affichage dans la scene
  ofxDatGui* menu_scene_misc;
  void setup_menu_scene_misc();
  ofxDatGuiFolder* misc_folder;
  ofxDatGuiButton* show_bezier_curve_button;
  ofxDatGuiButton* show_parametric_surface_button;
  void courbe_bezier_button_pressed_datgui(ofxDatGuiButtonEvent e);
  void surface_parametrique_button_pressed_datgui(ofxDatGuiButtonEvent e);
  void courbe_bezier_button_pressed();
  void surface_parametrique_button_pressed();
  int selectedControlPointIndex = -1;
  
  //Gui pour parametre de camera
  ofxDatGui* menu_camera;
  void setup_menu_camera();
  //dropdown pour choix de la camera
  ofxDatGuiDropdown* camera_choices_dropdown;
  void on_camera_choices_dropdown(ofxDatGuiDropdownEvent e);
  //render distance
  ofxDatGuiSlider* camera_render_far_slider;
  void on_camera_render_far_slider(ofxDatGuiSliderEvent e);
  //camera fov slider (default 60)
  ofxDatGuiSlider* camera_fov_slider;
  void on_camera_fov_slider(ofxDatGuiSliderEvent e);
  //toggle pour activer ou desactiver le frustum
  ofxDatGuiToggle* camera_frustum_toggle;
  void on_camera_frustum_toggle(ofxDatGuiToggleEvent e);
  //toggle pour activer ou desactiver la perspective
  ofxDatGuiToggle* camera_perspective_toggle;
  void on_camera_perspective_toggle(ofxDatGuiToggleEvent e);
  //slider pour vitesse camera qui est speed_delta
  ofxDatGuiSlider* camera_speed_slider;
  void on_camera_speed_slider(ofxDatGuiSliderEvent e);

  //Gui pour parametre de lumiere/shader
  ofxDatGui* menu_light;
  void setup_menu_light();
  //color picker pour ambiant color
  ofxDatGuiColorPicker* light_ambient_color_picker;
  void on_light_ambient_color_picker(ofxDatGuiColorPickerEvent e);
  //color picker pour diffuse color
  ofxDatGuiColorPicker* light_diffuse_color_picker;
  void on_light_diffuse_color_picker(ofxDatGuiColorPickerEvent e);

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
