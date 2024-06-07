

#include "application.h"

void Application::setup()
{
  ofSetWindowTitle("Driewer");

  setupGui();
  renderer.setup();
  pbr_setup();
  //pc render settings
  potato_pc = false;
  if(potato_pc)
  {
    renderer.setup_potato_pc();
  }

  setup_menu_raytracer();
  setup_menu_graphics();
  setup_menu_scene_misc();
  setup_menu_camera();
  setup_menu_light();
}

void Application::update()
{
  time_current = ofGetElapsedTimef();
  time_elapsed = time_current - time_last;
  time_last = time_current;
  renderer.speed_translation = renderer.speed_delta * time_elapsed;

  if(!potato_pc)
  {
    update_key_camera();
    update_key_light();
    updateLights();
    updateMaterial();

    pbr_update();
    update_tessellation();

    renderer.update();
  } else {
    update_key_camera();
    update_key_light();
    updateLights();
    updateMaterial();
    renderer.update_potato_pc();
  }
  menu_raytracer->update();
}

void Application::draw()
{
  if (!potato_pc)
  {
    renderer.draw();
  } else {
    renderer.draw_potato_pc();
  }
  


  gui.draw();
  pbr_draw_gui();
}

void Application::reset()
{
  renderer.reset();
  pbr_reset();
}

void Application::keyPressed(int key)
{
  switch (key)
  {
    case 'w':
      is_key_press_w = true;
      break;
    case 's':
      is_key_press_s = true;
      break;
    case 'a':
      is_key_press_a = true;
      break;
    case 'd':
      is_key_press_d = true;
      break;
    case 'q':
      is_key_press_q = true;
      break;
    case 'e':
      is_key_press_e = true;
      break;
    case 'j':
      is_key_press_j = true;
      break;
    case 'u':
      is_key_press_u = true;
      break;
    case 'h':
      is_key_press_h = true;
      break;
    case 'k':
      is_key_press_k = true;
      break;
    case 'y':
      is_key_press_y = true;
      break;
    case 'i':
      is_key_press_i = true;
      break;
    case 'z':
      is_key_press_z = true;
      break;
    case 'x':
      is_key_press_x = true;
      break;
    //light control
    case OF_KEY_UP:
      is_key_press_up = true;
      break;
    case OF_KEY_DOWN:
      is_key_press_down = true;
      break;
    case OF_KEY_LEFT:
      is_key_press_left = true;
      break;
    case OF_KEY_RIGHT:
      is_key_press_right = true;
      break;
    case OF_KEY_PAGE_UP:
      is_key_press_page_up = true;
      break;
    case OF_KEY_PAGE_DOWN:
      is_key_press_page_down = true;
      break;
    default:
      ofLog() << "<keyPressed: " << key << ">";
      break;
  }
}

void Application::keyReleased(int key)
{
  switch (key)
  {
    case 'w':
        is_key_press_w = false;
        break;
    case 's':
        is_key_press_s = false;
        break;
    case 'a':
        is_key_press_a = false;
        break;
    case 'd':
        is_key_press_d = false;
        break;
    case 'q':
        is_key_press_q = false;
        break;
    case 'e':
        is_key_press_e = false;
        break;
    case 'j':
        is_key_press_j = false;
        break;
    case 'u':
        is_key_press_u = false;
        break;
    case 'h':
        is_key_press_h = false;
        break;
    case 'k':
        is_key_press_k = false;
        break;
    case 'y':
        is_key_press_y = false;
        break;
    case 'i':
        is_key_press_i = false;
        break;
    //light control
    case OF_KEY_UP:
      is_key_press_up = false;
      break;
    case OF_KEY_DOWN:
      is_key_press_down = false;
      break;
    case OF_KEY_LEFT:
      is_key_press_left = false;
      break;
    case OF_KEY_RIGHT:
      is_key_press_right = false;
      break;
    case OF_KEY_PAGE_UP:
      is_key_press_page_up = false;
      break;
    case OF_KEY_PAGE_DOWN:
      is_key_press_page_down = false;
      break;
    case '-':
      break;

    //changer de material key f5 f6 f7
    case OF_KEY_F5:
      renderer.shaderManager.setMaterial(&renderer.material);
      ofLog() << "<select material: default>";
      break;
    case OF_KEY_F6:
      renderer.shaderManager.setMaterial(&renderer.material_2);
      ofLog() << "<select material: material2>";
      break;
    case OF_KEY_F7:
      renderer.shaderManager.setMaterial(&renderer.material_3);
      ofLog() << "<select material: material3>";
      break;
    default:
      break;
  }
}

void Application::mouseDragged(int x, int y, int button) {
    if (selectedControlPointIndex != -1) {
        renderer.bezierControlPoints[selectedControlPointIndex].set(x, y);
    }
}

//--------------------------------------------------------------
void Application::mousePressed(int x, int y, int button) {
    for (size_t i = 0; i < renderer.bezierControlPoints.size(); i++) {
        if (renderer.bezierControlPoints[i].distance(ofPoint(x, y)) < 10) {
            selectedControlPointIndex = i;
            break;
        }
    }
}


void Application::mouseReleased(int x, int y, int button) {
    selectedControlPointIndex = -1;
}

void Application::update_key_camera()
{
    renderer.is_camera_move_forward = is_key_press_w;
    renderer.is_camera_move_backward = is_key_press_s;

    renderer.is_camera_move_left = is_key_press_a;
    renderer.is_camera_move_right = is_key_press_d;

    renderer.is_camera_move_up = is_key_press_q;
    renderer.is_camera_move_down = is_key_press_e;

    renderer.is_camera_tilt_up = is_key_press_j;
    renderer.is_camera_tilt_down = is_key_press_u;

    renderer.is_camera_pan_left = is_key_press_h;
    renderer.is_camera_pan_right = is_key_press_k;

    renderer.is_camera_roll_left = is_key_press_y;
    renderer.is_camera_roll_right = is_key_press_i;
}

void Application::update_key_light()
{
    renderer.is_light_move_forward = is_key_press_up;
    renderer.is_light_move_backward = is_key_press_down;

    renderer.is_light_move_left = is_key_press_left;
    renderer.is_light_move_right = is_key_press_right;

    renderer.is_light_move_up = is_key_press_page_up;
    renderer.is_light_move_down = is_key_press_page_down;
}

void Application::updateLights(){

  renderer.spotLight.setPosition(renderer.light_position.x, renderer.light_position.y, renderer.light_position.z);
  renderer.spotLight.lookAt(ofVec3f(renderer.light_target.x, renderer.light_target.y, renderer.light_target.z));

  renderer.spotLight.setSpotlightCutOff(spotCutOff);
  renderer.spotLight.setSpotConcentration(spotConcentration);
  
}

void Application::updateMaterial(){
    
  renderer.material.setColors(diffuse, ambient, specular, emissive);
  renderer.material.setColors(diffuse, ambient, specular, emissive);
  renderer.material.setShininess(shininess);
    
}

void Application::setupGui()
{  
    
    
    gui.setup();

    settings_parameters.setName("General settings");
    settings_parameters.add(b_potato_pc.set("Specs switch", true));
    b_potato_pc.addListener(this, &Application::specs_changed);

    lightParameters.setName("Lights");
    lightParameters.add(renderer.bPointLight.set("PointLight",true));
    lightParameters.add(renderer.bDirLight.set("DirectionalLight", true));
    lightParameters.add(renderer.bSpotLight.set("SpotLight", true));
    lightParameters.add(renderer.bAmbientLight.set("AmbientLight", true));
    lightParameters.add(spotCutOff.set("Spot CuttOff", 4, 0, 90));
    lightParameters.add(spotConcentration.set("Spot Concentr.", 1, 0, 128));

    
    materialParameters.setName("Material");
    materialParameters.add(shininess.set("shinines", 100, 0, 128));
    
    materialParameters.add(diffuse.set("diffuse", ofFloatColor(0.2, 1.0),
                                       ofFloatColor(0.0), ofFloatColor(1.0)) );
    materialParameters.add(ambient.set("ambient", ofFloatColor(0.2, 1.0),
                                       ofFloatColor(0.0), ofFloatColor(1.0)) );
    materialParameters.add(specular.set("specular", ofFloatColor(0.2, 1.0),
                                        ofFloatColor(0.0), ofFloatColor(1.0)));
    materialParameters.add(emissive.set("emmisive", ofFloatColor(0.2, 1.0),
                                        ofFloatColor(0.0), ofFloatColor(1.0)));

    gui.add(settings_parameters);
    gui.add(lightParameters);
    gui.add(materialParameters);
    gui.add(renderer.bUseTexture.set("Use Texture",true));

    gui.add(tessLevelInner.set("INNER tess lev.", 1.0, 1.0, 5.0));
    gui.add(tessLevelOuter.set("OUTER tess lev, ", 1.0, 1.0, 5.0));

    //lof usetexture
    ofLog() << "<usetexture: " << renderer.bUseTexture << ">";
    
}

void Application::specs_changed(bool & b_potato_pc)
{
  if(!potato_pc)
  {
    renderer.shaderManager.reload();
    renderer.shaderManager.load("shaders/normLambert");
  } else {
    renderer.shaderManager.reload();
    renderer.shaderManager.load("shaders/Phong_Blinn_tex");
  }

  potato_pc = !potato_pc;
}

void Application::courbe_bezier_button_pressed() {
    renderer.showBezierSpline = !renderer.showBezierSpline;
    ofLog() << "<Courbe de bezier>";
}

void Application::surface_parametrique_button_pressed() {
    renderer.showBezierSurface = !renderer.showBezierSurface;
    ofLog() << "<Surface Parametrique>";
}

void Application::button_reset_pressed()
{
  reset();
}

void Application::pbr_setup()
{
  toggle_ui = true;


  reset();

  gui_pbr.setup("pbr material");

  // interface pour les couleurs du matériau
  group_material_color.setup("color");

  group_material_color.add(color_picker_ambient);
  group_material_color.add(color_picker_diffuse);
  group_material_color.add(color_picker_specular);

  gui_pbr.add(&group_material_color);

  // interface pour les facteurs numériques du matériau
  group_material_factor.setup("factor");

  group_material_factor.add(slider_metallic);
  group_material_factor.add(slider_roughness);
  group_material_factor.add(slider_occlusion);
  group_material_factor.add(slider_brightness);

  group_material_factor.add(slider_fresnel_ior);

  gui_pbr.add(&group_material_factor);

  // interface pour les paramètres de la lumière
  group_light.setup("light");

  group_light.add(color_picker_light_color);
  group_light.add(slider_light_intensity);
  group_light.add(toggle_light_motion);

  gui_pbr.add(&group_light);

  // interface pour les paramètres de mappage tonal
  group_tone_mapping.setup("tone mapping");

  group_tone_mapping.add(slider_exposure);
  group_tone_mapping.add(slider_gamma);
  group_tone_mapping.add(toggle_tone_mapping);

  gui_pbr.add(&group_tone_mapping);

  // interface pour les options de l'application
  toggle_ui.setName("hide ui");
  gui_pbr.add(toggle_ui);

  button_reset.setup("reset");
  button_reset.addListener(this, &Application::button_reset_pressed);
  gui_pbr.add(&button_reset);
  //move gui to top right
  gui_pbr.setPosition(ofGetWidth() - gui_pbr.getWidth(), 0);
}

void Application::pbr_reset()
{
  

  color_picker_ambient.set("ambient", renderer.material_color_ambient, ofColor(0, 0), ofColor(255, 255));
  color_picker_diffuse.set("diffuse", renderer.material_color_diffuse, ofColor(0, 0), ofColor(255, 255));
  color_picker_specular.set("specular", renderer.material_color_specular, ofColor(0, 0), ofColor(255, 255));

  slider_metallic.set("metallic", renderer.material_metallic, 0.0f, 1.0f);
  slider_roughness.set("roughness", renderer.material_roughness, 0.0f, 1.0f);
  slider_occlusion.set("occlusion", renderer.material_occlusion, 0.0f, 5.0f);
  slider_brightness.set("brightness", renderer.material_brightness, 0.0f, 5.0f);

  slider_fresnel_ior.set("fresnel ior", renderer.material_fresnel_ior, glm::vec3(0.0f), glm::vec3(1.0f));

  color_picker_light_color.set("color", renderer.light_color, ofColor(0, 0), ofColor(255, 255));
  slider_light_intensity.set("intensity", renderer.light_intensity, 0.0f, 10.0f);

  toggle_light_motion.set("motion", renderer.light_motion);

  slider_exposure.set("exposure", 1.0f, 0.0f, 5.0f);
  slider_gamma.set("gamma", 2.2f, 0.0f, 5.0f);

  if (renderer.tone_mapping_toggle)
    toggle_tone_mapping.set("aces filmic", true);
  else
    toggle_tone_mapping.set("reinhard", false);
}

void Application::pbr_update()
{


  update_material_pbr();


}

void Application::pbr_draw_gui()
{
    if (toggle_ui)
    gui_pbr.draw();
}

void Application::update_material_pbr()
{
  renderer.material_color_ambient = color_picker_ambient;
  renderer.material_color_diffuse = color_picker_diffuse;
  renderer.material_color_specular = color_picker_specular;

  renderer.material_metallic = slider_metallic;
  renderer.material_roughness = slider_roughness;
  renderer.material_occlusion = slider_occlusion;
  renderer.material_brightness = slider_brightness;

  renderer.material_fresnel_ior = slider_fresnel_ior;

  renderer.light_color = color_picker_light_color;
  renderer.light_intensity = slider_light_intensity;
  renderer.light_motion = toggle_light_motion;

  renderer.tone_mapping_exposure = slider_exposure;
  renderer.tone_mapping_gamma = slider_gamma;
  renderer.tone_mapping_toggle = toggle_tone_mapping;

  if (renderer.tone_mapping_toggle)
    toggle_tone_mapping.set("aces filmic", true);
  else
    toggle_tone_mapping.set("reinhard", false);
}

void Application::update_tessellation()
{
  renderer.tessLevelInner = tessLevelInner;
  renderer.tessLevelOuter = tessLevelOuter;
}
map<int, imgDimension> Application::prepare_raytracer_resolution()
{
  map<int, imgDimension> avl;
  imgDimension small; small.width = 1; small.height = 1;
  imgDimension normal; normal.width = 120; normal.height = 75;
  imgDimension medium; medium.width = 320; medium.height = 200;
  imgDimension large; large.width = 640; large.height = 400;
  imgDimension big; big.width = 800; big.height = 600;
  imgDimension veryBig; veryBig.width = 1024; veryBig.height = 640;
  avl[0] = small;
  avl[1] = normal;
  avl[2] = medium;
  avl[3] = large;
  avl[4] = big;
  avl[5] = veryBig;
  return avl;
}

void Application::setup_menu_raytracer()
{
  vector<string> options = {"1x1", "120x75", "320x200", "640x400","800x600", "1280x768"};
  availableResolution = prepare_raytracer_resolution();

  menu_raytracer = new ofxDatGui(ofxDatGuiAnchor::BOTTOM_RIGHT);
  menu_raytracer->addHeader("Raytracer");
  ambientBias.set("Ambient bias", 0.2, 0.1, 0.6);
  samples.set("Samples per ray", 1, 8, 32);
  menu_raytracer->setAutoDraw(false);
  menu_raytracer->addTextInput("message", "Ray Casting");
  menu_raytracer->addDropdown("Resolution", options);
  menu_raytracer->addSlider("Indirect rays per pixel", 0, 2048);
  menu_raytracer->addSlider(ambientBias);
  menu_raytracer->addSlider(samples);
  runInParallel = false;
  menu_raytracer->addToggle("Run in parallel", runInParallel);
  menu_raytracer->addButton("start render");
  menu_raytracer->onDropdownEvent(this, &Application::onResolutionEvent);
  menu_raytracer->onButtonEvent(this, &Application::onRenderEvent);
  menu_raytracer->onToggleEvent(this, &Application::onToggleEvent);
  menu_raytracer->onSliderEvent(this, &Application::onSliderEvent);
  
}

void Application::onResolutionEvent(ofxDatGuiDropdownEvent e){
  raytracer_options.resolution = availableResolution[int(e.child)];
}

void Application::onRenderEvent(ofxDatGuiButtonEvent e){
    cout << e.target << endl;
    start_raytracer(raytracer_options);
}

void Application::onToggleEvent(ofxDatGuiToggleEvent e){
  runInParallel = e.checked;
};

void Application::onSliderEvent(ofxDatGuiSliderEvent e){
    if (e.target->is("Ambient bias")){
        raytracer_options.ambientBias = e.target->getValue();
    }

    if (e.target->is("Samples per ray")){
        raytracer_options.samples = e.target->getValue();
    }

    if (e.target->is("indirect rays per pixel")){
        raytracer_options.nIndirectRays = e.target->getValue();
    }

}

void Application::start_raytracer(menu_raytracer_options opt)
{
  renderer.camera_raytracer.setSamples(opt.samples);
  renderer.stock_material_primitive();

  renderer.rayTracer.setup(renderer.primitives, renderer.materials, renderer.lights, opt.ambientBias);
  renderer.raytraced_image = renderer.init_raytraced_image(opt.resolution.width, opt.resolution.height);
  auto image_delimiter = ofRectangle(0, 0, opt.resolution.width, opt.resolution.height);
  int n_ray = ceil(opt.nIndirectRays);

  renderer.rayTracer.traceImage(renderer.camera_raytracer, image_delimiter,renderer.raytraced_image, runInParallel, n_ray);
  renderer.raytraced_image->save("raytraced_image.png");
}

void Application::setup_menu_graphics()
{
  available_shader = {"Blinn Phong", "Phong", "Lambert", 
                      "Gouraud", "Cel", "goosh", "flat", "pbr"};
  menu_graphics = new ofxDatGui(ofxDatGuiAnchor::BOTTOM_LEFT);
  menu_graphics->addHeader("Graphics");

  shader_dropdown = menu_graphics->addDropdown("Shader", available_shader);
  shader_dropdown->onDropdownEvent(this, &Application::onShaderEvent);
}

void Application::onShaderEvent(ofxDatGuiDropdownEvent e)
{
  switch (e.child)
  {
  case 0:
    renderer.update_texture_default();
    renderer.shaderManager.reload();
    renderer.shaderManager.load("shaders/Phong_Blinn_tex");
    //log pour dire quel shader
    ofLog() << "<select shader: Blinn Phong>";
    break;
  case 1:
    renderer.shaderManager.reload();
    renderer.shaderManager.load("shaders/Phong_Phong_tex");
    //log
    ofLog() << "<select shader: Phong>";
    break;
  case 2:
    renderer.shaderManager.reload();
    renderer.shaderManager.load("shaders/normLambert");
    ofLog() << "<select shader: Lambert >";
    break;
  case 3:
    renderer.shaderManager.reload();
    renderer.shaderManager.load("shaders/normMapGouraud");
    ofLog() << "<select shader: Gouraud>";
    break;
  case 4:
    renderer.shaderManager.reload();
    renderer.shaderManager.load("shaders/normCel");
    ofLog() << "<select shader: Cel>";
    break;
  case 5:
    renderer.shaderManager.reload();
    renderer.shaderManager.load("shaders/goosh");
    break;
  case 6:
    renderer.shaderManager.reload();
    renderer.shaderManager.load("shaders/flat");
    break;
  case 7:
    renderer.update_texture_pbr();
    renderer.shaderManager.load("shaders/pbr");
    break;
  default:
    break;
  }
}

void Application::setup_menu_scene_misc()
{
  menu_scene_misc = new ofxDatGui(ofxDatGuiAnchor::BOTTOM_LEFT);
  menu_scene_misc->addHeader("Misc");
  misc_folder = menu_scene_misc->addFolder("Misc");

  show_bezier_curve_button = misc_folder->addButton("Show Bezier Curve");
  show_bezier_curve_button->onButtonEvent(this, &Application::courbe_bezier_button_pressed_datgui);
  show_parametric_surface_button = misc_folder->addButton("Show Parametric Surface");
  show_parametric_surface_button->onButtonEvent(this, &Application::surface_parametrique_button_pressed_datgui);
}

void Application::courbe_bezier_button_pressed_datgui(ofxDatGuiButtonEvent e)
{
  renderer.showBezierSpline = !renderer.showBezierSpline;
  ofLog() << "<Courbe de bezier>";
}

void Application::surface_parametrique_button_pressed_datgui(ofxDatGuiButtonEvent e)
{
  renderer.showBezierSurface = !renderer.showBezierSurface;
  ofLog() << "<Surface Parametrique>";
}

//setup menu camera
void Application::setup_menu_camera()
{
  menu_camera = new ofxDatGui(ofxDatGuiAnchor::BOTTOM_RIGHT);
  menu_camera->addHeader("Camera");
  
  camera_choices_dropdown = menu_camera->addDropdown("Camera", {"Front", "Back"});
  camera_choices_dropdown->onDropdownEvent(this, &Application::on_camera_choices_dropdown);
  ofxDatGuiLog::quiet();
  camera_render_far_slider = menu_camera->addSlider("Render distance", 2, 500);
  camera_render_far_slider->setValue(renderer.camera_far);
  camera_render_far_slider->onSliderEvent(this, &Application::on_camera_render_far_slider);

  //poroduit un warning [WARNING] :: Event Handler Not Set mais fonctionne donc faire
  camera_fov_slider = menu_camera->addSlider("Field of view", 30, 120);
  camera_fov_slider->setValue(renderer.camera_fov);
  camera_fov_slider->onSliderEvent(this, &Application::on_camera_fov_slider);


  //section pour afficher frustum aussi [WARNING] :: Event Handler Not Set mais fonctionne
  camera_frustum_toggle = menu_camera->addToggle("Show frustum", false);
  camera_frustum_toggle->onToggleEvent(this, &Application::on_camera_frustum_toggle);

  //section pour activer ou desactiver la perspective
  camera_perspective_toggle = menu_camera->addToggle("Perspective", true);
  camera_perspective_toggle->onToggleEvent(this, &Application::on_camera_perspective_toggle);

  //section speed qui est default a 250
  camera_speed_slider = menu_camera->addSlider("Camera speed", 0, 500);
  camera_speed_slider->setValue(renderer.speed_delta);
  camera_speed_slider->onSliderEvent(this, &Application::on_camera_speed_slider);

}

void Application::on_camera_choices_dropdown(ofxDatGuiDropdownEvent e)
{
  switch (e.child)
  {
  case 0:
    renderer.camera_active = Camera::front;
    break;
  case 1:
    renderer.camera_active = Camera::back;
    break;
  default:
    break;
  }
  renderer.setup_camera();
}

void Application::on_camera_render_far_slider(ofxDatGuiSliderEvent e)
{
  renderer.camera_far = e.value;
  renderer.setup_camera();
}

void Application::on_camera_fov_slider(ofxDatGuiSliderEvent e)
{
  renderer.camera_fov = e.value;
  renderer.setup_camera();
}

void Application::on_camera_frustum_toggle(ofxDatGuiToggleEvent e)
{
  renderer.is_visible_frustum = e.checked;
}

void Application::on_camera_perspective_toggle(ofxDatGuiToggleEvent e)
{
  renderer.is_camera_perspective = e.checked;
  renderer.setup_camera();
}

void Application::on_camera_speed_slider(ofxDatGuiSliderEvent e)
{
  renderer.speed_delta = e.value;

}

void Application::setup_menu_light()
{
  menu_light = new ofxDatGui(ofxDatGuiAnchor::BOTTOM_RIGHT);
  menu_light->addHeader("Shader control");
  //ajouter le color picker pour la couleur ambiante pour la variable ambient
  light_ambient_color_picker = menu_light->addColorPicker("Ambient color", ofColor(255, 255, 255));
  light_ambient_color_picker->onColorPickerEvent(this, &Application::on_light_ambient_color_picker);
  //ajouter le color picker pour la couleur diffuse
  light_diffuse_color_picker = menu_light->addColorPicker("Diffuse color", ofColor(255, 255, 255));
  light_diffuse_color_picker->onColorPickerEvent(this, &Application::on_light_diffuse_color_picker);
}

void Application::on_light_ambient_color_picker(ofxDatGuiColorPickerEvent e)
{
  //renderer.light_color = e.color;
  ambient = e.color;
  //log pour la couleur dans console
  ofLog() << "<ambient color: " << light_shader_manager.material_color_ambient << ">";
}

void Application::on_light_diffuse_color_picker(ofxDatGuiColorPickerEvent e)
{
  //renderer.light_color = e.color;
  light_shader_manager.material_color_diffuse = e.color;
  //log pour la couleur dans console
  ofLog() << "<diffuse color: " << light_shader_manager.material_color_diffuse << ">";
}

void Application::exit()
{
  b_potato_pc.removeListener(this, &Application::specs_changed);
  button_reset.removeListener(this, &Application::button_reset_pressed);
  ofLog() << "<app::exit>";
}
