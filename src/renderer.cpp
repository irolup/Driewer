#include "renderer.h"

void Renderer::setup()
{

  ofSetFrameRate(60);
  ofSetLogLevel(OF_LOG_VERBOSE);


  // paramètres
  rotation_speed = 0.3f;
  is_light_move_up = false;
  is_light_move_down = false;
  is_light_move_left = false;
  is_light_move_right = false;
  is_light_move_forward = false;
  is_light_move_backward = false;
  light_position = ofVec3f(0.0f, 0.0f, 0.0f);

  reset();
  init_camera();
  setup_camera();


  //setup material
  setupMaterial();

  setup_texture_arbitrary();

  //setup courbe de bezier et surface de bezier
  initializeBezierSpline();
  initializeBezierSurface();

  //shader managers
  setupLights();
  bPointLight.addListener(this, &Renderer::pointLightChanged);
  bSpotLight.addListener(this, &Renderer::spotLightChanged);
  bDirLight.addListener(this, &Renderer::dirLightChanged);
  bAmbientLight.addListener(this, &Renderer::ambientLightChanged);
  bUseTexture.addListener(this, &Renderer::textureToggled);
  
  shaderManager.load("shaders/Phong_Blinn_tex");
  
  colorTexture.load("textures_other/brick_diffuse.jpg");
  normalTexture.load("textures_other/brick_normal.jpg");
  metal_Texture.load("textures_other/brick_metal.jpg");
  roughness_Texture.load("textures_other/brick_roughness.jpg");
  occlusion_Texture.load("textures_other/brick_occlusion.jpg");
  colorTexture.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  normalTexture.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  metal_Texture.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  roughness_Texture.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  occlusion_Texture.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  
  shaderManager.useLight(&spotLight);
  shaderManager.useLight(&directionalLight);
  shaderManager.useLight(&pointLight);
  shaderManager.useLight(&ambientLight);
  shaderManager.useLight(&directionalLight2);
  shaderManager.setMaterial(&material);

  //Load sphere texture
  shaderManager.setTexture(&colorTexture);
  shaderManager.setNormalMapTexture(&normalTexture);
  shaderManager.setMetalTexture(&metal_Texture);
  shaderManager.setRoughnessTexture(&roughness_Texture);
  shaderManager.setOcculsionTexture(&occlusion_Texture);

  set_camera_active();

  setupPrimitives();
  initialize_cubemap();

  pbr_setup();

  //section tessellation
  setup_tessellation();

  ofLog() << "<setup>";
}

void Renderer::reset()
{
  cam_reset();
  pbr_reset();
  ofLog() << "<reset>";
}

void Renderer::update()
{
  // position au centre de la fenêtre d'affichage
  center_x = ofGetWidth() / 2.0f;
  center_y = ofGetHeight() / 2.0f;

  cam_update();
  light_update();
  pbr_update();
}

void Renderer::update_potato_pc()
{
  center_x = ofGetWidth() / 2.0f;
  center_y = ofGetHeight() / 2.0f;
  cam_update();
  light_update();
}

void Renderer::draw()
{
  //cam multiple
  camera->begin();

  ofEnableDepthTest();
  if(is_visible_frustum)
  {
    cam_draw_frustum();
  }
    
  ofDrawAxis(10);
    
  shaderManager.begin();
  drawScene();
  drawLights();
  shaderManager.end();
  draw_cube_map();
  drawScene_tess();
  camera->end();


  pbr_draw();

  
  ofDisableDepthTest();
  ofDrawBitmapString(shaderManager.shaderName(), 20, ofGetHeight()-30);

  draw_bezier_spline();
  draw_bezier_surface();

}

void Renderer::setup_potato_pc()
{
  ofSetFrameRate(30);
  shaderManager.reload();
  shaderManager.load("shaders/normLambert");
  //mettre le shader actif a celui le moins demandant
  //comme dans app quand on change shader mais mettre genre lambert
}

void Renderer::draw_potato_pc()
{
    //cam multiple
  camera->begin();

  ofEnableDepthTest();
  if(is_visible_frustum)
  {
    cam_draw_frustum();
  }
    
  ofDrawAxis(10);
    
  shaderManager.begin();
  drawScene();
  drawLights();
  shaderManager.end();
  draw_cube_map();
  //drawScene_tess();
  camera->end();


  //pbr_draw();

  
  ofDisableDepthTest();
  ofDrawBitmapString(shaderManager.shaderName(), 20, ofGetHeight()-30);

}





void Renderer::setupMaterial()
{
  //material 1
  mat_ambient = ofVec4f(0.1f, 0.1f, 0.1f, 1.0f); // dark gray
  mat_diffuse = ofVec4f(0.5f, 0.5f, 0.5f, 1.0f); // light gray
  mat_specular = ofVec4f(0.5f, 0.5f, 0.5f, 1.0f); // light gray
  mat_emissive = ofVec4f(0.0f, 0.0f, 0.0f, 1.0f); // black
  mat_shininess = 128.0f;

  //material 2
  mat_ambient_2 = ofVec4f(0.1f, 0.1f, 0.1f, 1.0f); // dark gray
  mat_diffuse_2 = ofVec4f(0.3f, 0.7f, 0.9f, 1.0f); // light blue
  mat_specular_2 = ofVec4f(0.8f, 0.2f, 0.1f, 1.0f); // reddish orange
  mat_emissive_2 = ofVec4f(0.0f, 0.0f, 0.0f, 1.0f); // black
  mat_shininess_2 = 64.0f;

  //material 3
  mat_ambient_3 = ofVec4f(0.5f, 0.2f, 0.3f, 1.0f); // dark pink
  mat_diffuse_3 = ofVec4f(0.8f, 0.8f, 0.1f, 1.0f); // yellow
  mat_specular_3 = ofVec4f(0.2f, 0.9f, 0.8f, 1.0f); // light greenish blue
  mat_emissive_3 = ofVec4f(0.0f, 0.0f, 0.0f, 1.0f); // black
  mat_shininess_3 = 32.0f;

  //material 4
  mat_ambient_4 = ofVec4f(0.1f, 0.1f, 0.1f, 1.0f); // dark gray
  mat_diffuse_4 = ofVec4f(0.5f, 0.5f, 0.5f, 1.0f); // light gray
  mat_specular_4 = ofVec4f(0.5f, 0.5f, 0.5f, 1.0f); // light gray
  mat_emissive_4 = ofVec4f(0.0f, 0.0f, 0.0f, 1.0f); // black
  mat_shininess_4 = 128.0f;

  material_1.setAmbientColor(ofFloatColor(mat_ambient.x, mat_ambient.y, mat_ambient.z, mat_ambient.w));
  material_1.setDiffuseColor(ofFloatColor(mat_diffuse.x, mat_diffuse.y, mat_diffuse.z, mat_diffuse.w));
  material_1.setSpecularColor(ofFloatColor(mat_specular.x, mat_specular.y, mat_specular.z, mat_specular.w));
  material_1.setEmissiveColor(ofFloatColor(mat_emissive.x, mat_emissive.y, mat_emissive.z, mat_emissive.w));
  material_1.setShininess(mat_shininess);

  material_2.setAmbientColor(ofFloatColor(mat_ambient_2.x, mat_ambient_2.y, mat_ambient_2.z, mat_ambient_2.w));
  material_2.setDiffuseColor(ofFloatColor(mat_diffuse_2.x, mat_diffuse_2.y, mat_diffuse_2.z, mat_diffuse_2.w));
  material_2.setSpecularColor(ofFloatColor(mat_specular_2.x, mat_specular_2.y, mat_specular_2.z, mat_specular_2.w));
  material_2.setEmissiveColor(ofFloatColor(mat_emissive_2.x, mat_emissive_2.y, mat_emissive_2.z, mat_emissive_2.w));
  material_2.setShininess(mat_shininess_2);


  material_3.setAmbientColor(ofFloatColor(mat_ambient_3.x, mat_ambient_3.y, mat_ambient_3.z, mat_ambient_3.w));
  material_3.setDiffuseColor(ofFloatColor(mat_diffuse_3.x, mat_diffuse_3.y, mat_diffuse_3.z, mat_diffuse_3.w));
  material_3.setSpecularColor(ofFloatColor(mat_specular_3.x, mat_specular_3.y, mat_specular_3.z, mat_specular_3.w));
  material_3.setEmissiveColor(ofFloatColor(mat_emissive_3.x, mat_emissive_3.y, mat_emissive_3.z, mat_emissive_3.w));
  material_3.setShininess(mat_shininess_3);

  material_4.setAmbientColor(ofFloatColor(mat_ambient_4.x, mat_ambient_4.y, mat_ambient_4.z, mat_ambient_4.w));
  material_4.setDiffuseColor(ofFloatColor(mat_diffuse_4.x, mat_diffuse_4.y, mat_diffuse_4.z, mat_diffuse_4.w));
  material_4.setSpecularColor(ofFloatColor(mat_specular_4.x, mat_specular_4.y, mat_specular_4.z, mat_specular_4.w));
  material_4.setEmissiveColor(ofFloatColor(mat_emissive_4.x, mat_emissive_4.y, mat_emissive_4.z, mat_emissive_4.w));
  material_4.setShininess(mat_shininess_4);

}

void Renderer::setupLights(){

  
  // Point lights emit light in all directions //
  pointLight.setPosition(20.0f, 20.0f, 14.0f);
  pointLight.setDiffuseColor(ofColor::white);
  pointLight.setSpecularColor( ofColor(255.f, 255.f, 0.f));
  pointLight.setPointLight();
  //spotlight 1
  //spotLight.setPosition(140.0f, 200.0f, -100.0f);
  light_position = ofVec3f(54.0f, 50.0f, 54.0f);
  spotLight.setPosition(light_position.x, light_position.y, light_position.z);
  //light_target = ofVec3f(light_position.x, -light_position.y, light_position.z);
  spotLight.lookAt(light_target);
  //mettre target vers -y et doit etre au dessus de 0,0,0
  spotLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
  spotLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
  spotLight.setSpotlightCutOff( 3 );
  spotLight.setSpotConcentration(1);
  spotLight.setSpotlight();
    
  // Directional Lights emit light based on their orientation, regardless of their position //
  directionalLight.setPosition(-25.0f, 30.0f, -25.0f);
  directionalLight.lookAt(ofVec3f(0,0,0));
  directionalLight.setDiffuseColor(ofColor(0.f, 0.f, 255.f));
  directionalLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
  // set the direction of the light
  // set it pointing from left to right -> //
  directionalLight.setOrientation( ofVec3f(0, 9, 0) );
  directionalLight.setDirectional();

  //lumiere ambiante
  ambientLight.setPosition(10.0f, 10.0f, 10.0f);
  ambientLight.setAmbientColor(ofColor(1.0f, 1.0f, 1.0f));

  //second directional light
  directionalLight2.setPosition(25.0f, 40.0f, -18.0f);
  directionalLight2.lookAt(ofVec3f(0.0f, 2.0f,18.0f));
  directionalLight2.setDiffuseColor(ofColor(0.f, 255.f, 0.f));
  directionalLight2.setSpecularColor(ofColor(255.f, 255.f, 255.f));
  directionalLight2.setDirectional();


  //Log for light position
  ofLog() << "Point Light Position: " << pointLight.getPosition();
  ofLog() << "Spot Light Position: " << spotLight.getPosition();
  ofLog() << "Directional Light Position: " << directionalLight.getPosition();

}

void Renderer::pointLightChanged(bool & bPointLight){
  shaderManager.toggleLight(&pointLight, bPointLight);
}
//--------------------------------------------------------------

void Renderer::spotLightChanged(bool & bSpotLight){
  shaderManager.toggleLight(&spotLight, bSpotLight);
}

//--------------------------------------------------------------
void Renderer::dirLightChanged(bool & bDirLight){
  shaderManager.toggleLight(&directionalLight, bDirLight);
  shaderManager.toggleLight(&directionalLight2, bDirLight);
}

void Renderer::ambientLightChanged(bool & bAmbientLight){
  shaderManager.toggleLight(&ambientLight, bAmbientLight);
  ofLog() << "Ambient Light: " << bAmbientLight;
}

//--------------------------------------------------------------
void Renderer::textureToggled(bool &bUseTexture){
  //commence a true
  if (bUseTexture) {
    //shaderManager.setTexture(&colorTexture);
    shaderManager.toggleTexture(&colorTexture);
    ofLog() << "Texture: oui" << bUseTexture;
  } else {
    //shaderManager.setTexture(&normalTexture);
    shaderManager.toggleTexture(&colorTexture);
    ofLog() << "Texture: non" << bUseTexture;
  }
  
}

void Renderer::initializeBezierSpline() {
    showBezierSpline = false;
    bezierControlPoints.clear();

    // Obtenir les dimensions de la fenêtre
    float centerX = ofGetWidth() / 2.0f;
    float centerY = ofGetHeight() / 2.0f;

    // Définir les points de contrôle autour du centre de l'écran
    bezierControlPoints.push_back(ofPoint(centerX - 150, centerY - 100));
    bezierControlPoints.push_back(ofPoint(centerX - 120, centerY + 150));
    bezierControlPoints.push_back(ofPoint(centerX - 90, centerY - 150));
    bezierControlPoints.push_back(ofPoint(centerX - 60, centerY + 100));
    bezierControlPoints.push_back(ofPoint(centerX + 60, centerY - 100));
    bezierControlPoints.push_back(ofPoint(centerX + 90, centerY + 150));
    bezierControlPoints.push_back(ofPoint(centerX + 120, centerY - 150));
    bezierControlPoints.push_back(ofPoint(centerX + 150, centerY + 100));


    // Ajustez les valeurs numériques pour positionner la courbe comme vous le souhaitez
}


void Renderer::initializeBezierSurface() {
    showBezierSurface = false;
    bezierSurfaceControlPoints.clear();

    float centerX = ofGetWidth() / 2.0f;
    float centerY = ofGetHeight() / 2.0f;
    float maxOffset = 200;  // Ajustez cet offset pour modifier l'espacement entre les points
    float depth = 300;  // Profondeur de l'entonnoir

    int gridSize = 15;
    bezierSurfaceControlPoints = std::vector<std::vector<ofPoint>>(gridSize, std::vector<ofPoint>(gridSize, ofPoint()));
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            // Calculer la distance au centre de la grille
            float distanceToCenter = ofDist(i, j, gridSize / 2, gridSize / 2);
            // Utiliser la distance au centre pour ajuster la valeur z
            float zValue = -depth * exp(-distanceToCenter / 4.0);  // Exemple de fonction pour créer l'effet d'entonnoir

            // Positionner les points de manière à créer une grille centrée autour de centerX et centerY
            bezierSurfaceControlPoints[i][j] = ofPoint(centerX - maxOffset + j * maxOffset / (gridSize - 1),
                centerY - maxOffset + i * maxOffset / (gridSize - 1),
                zValue);
        }
    }
}

void Renderer::draw_bezier_spline()
{
//spline de Bézier
  if (showBezierSpline) {
      
      ofSetColor(ofColor::green);
      ofPolyline line;
      for (size_t i = 0; i < bezierControlPoints.size(); i++) {
          line.curveTo(bezierControlPoints[i]);
      }
      line.draw();

      for (size_t i = 1; i < bezierControlPoints.size() - 1; i++) {
          ofSetColor(ofColor::purple);
          ofDrawCircle(bezierControlPoints[i], 12);

      }
  }
}

void Renderer::draw_bezier_surface()
{
// surface de bezier
  if (showBezierSurface) {
      ofPoint surfaceCenter(0, 0, 0);
      int totalPoints = 0;
      for (const auto& row : bezierSurfaceControlPoints) {
          for (const auto& point : row) {
              surfaceCenter += point;
              totalPoints++;
          }
      }
      surfaceCenter /= totalPoints;

      float centerX = ofGetWidth() / 2.0f;
      float centerY = ofGetHeight() / 2.0f;
      ofPoint windowCenter(centerX, centerY, 0);
      ofPoint translation = windowCenter - surfaceCenter;

      ofPushMatrix();

      ofTranslate(translation.x, translation.y, translation.z);

      ofTranslate(surfaceCenter.x, surfaceCenter.y, surfaceCenter.z);
      ofRotateYDeg(ofGetElapsedTimef() * 20);
      ofTranslate(-surfaceCenter.x, -surfaceCenter.y, -surfaceCenter.z);

      for (int i = 0; i < bezierSurfaceControlPoints.size() - 1; i++) {
          for (int j = 0; j < bezierSurfaceControlPoints[i].size() - 1; j++) {

              ofSetColor(100 + bezierSurfaceControlPoints[i][j].z, 100, 255 - bezierSurfaceControlPoints[i][j].z);
              ofDrawLine(bezierSurfaceControlPoints[i][j], bezierSurfaceControlPoints[i][j + 1]);
              ofDrawLine(bezierSurfaceControlPoints[i][j], bezierSurfaceControlPoints[i + 1][j]);
          }
      }

      for (int i = 0; i < bezierSurfaceControlPoints.size() - 1; i++) {
          ofDrawLine(bezierSurfaceControlPoints[i][bezierSurfaceControlPoints.size() - 1], bezierSurfaceControlPoints[i + 1][bezierSurfaceControlPoints.size() - 1]);
      }

      for (int j = 0; j < bezierSurfaceControlPoints[0].size() - 1; j++) {
          ofDrawLine(bezierSurfaceControlPoints[bezierSurfaceControlPoints.size() - 1][j], bezierSurfaceControlPoints[bezierSurfaceControlPoints.size() - 1][j + 1]);
      }

      ofSetColor(ofColor::orange);
      for (const auto& row : bezierSurfaceControlPoints) {
          for (const auto& point : row) {
              ofDrawSphere(point, 2);
          }
      }

      ofPopMatrix();  // etat de la matrice
  }
}

void Renderer::stock_material_primitive()
{
  materials.push_back(material_1);
  materials.push_back(material_2);
  materials.push_back(material_3);
  materials.push_back(material_4);

  sphere.setParent(center_of_scence);
  box.setParent(center_of_scence);
  //plane.setParent(center_of_scence);
  cylinder.setParent(center_of_scence);
  cube_map_tracer.setParent(center_of_scence);

  primitives.push_back(sphere);
  primitives.push_back(box);
  //primitives.push_back(plane);
  primitives.push_back(cylinder);
  primitives.push_back(cube_map_tracer);

  
  lights.push_back(&pointLight);
  //lights.push_back(&spotLight);
  //lights.push_back(&directionalLight);
  //lights.push_back(&ambientLight);
  //lights.push_back(&directionalLight2);


  //log number in vector
  ofLog() << "Number of materials: " << materials.size();
  ofLog() << "Number of primitives: " << primitives.size();
  ofLog() << "Number of lights: " << lights.size();
  //center of scence position
  ofLog() << "Center of Scence: " << center_of_scence.getGlobalPosition();
  width = 320;
  height = 200;
  nIndirectRays = 1;
  ambientBias = 0.2f;
  samples = 1;
  rayTracer.setup(primitives, materials, lights, ambientBias);
  image = initImage(width, height);

  auto rect = ofRectangle(0, 0, width, height);
  //0 jsuqua 2048
  int n_rays = 2;
  rayTracer.traceImage(camera_raytracer, rect, image, runInParallel, n_rays);

  //export image
  image->save("raytraced_image.png");
  ofLog() << "Raytraced image saved";

}

void Renderer::update_texture_default()
{
  sphere.mapTexCoordsFromTexture(colorTexture.getTexture());
  box.mapTexCoordsFromTexture(colorTexture.getTexture());
  plane.mapTexCoordsFromTexture(colorTexture.getTexture());
  cube_map_tracer.mapTexCoordsFromTexture(colorTexture.getTexture());
}

void Renderer::update_texture_pbr()
{
  sphere.mapTexCoordsFromTexture(texture_arbitrary_diffuse.getTexture());
  box.mapTexCoordsFromTexture(texture_arbitrary_diffuse.getTexture());
  plane.mapTexCoordsFromTexture(texture_arbitrary_diffuse.getTexture());
  cube_map_tracer.mapTexCoordsFromTexture(texture_arbitrary_diffuse.getTexture());
}

void Renderer::setupPrimitives()
{
  //Center
  center_of_scence.move(0.0f, 0.0f, 0.0f);
  glm::vec3 position_center = center_of_scence.getGlobalPosition();
  center.set(position_center);

  position_sphere = ofVec3f{0.0f, 0.0f, -5.0f};
  radius = 1.0f;
  sphere.setPosition(position_sphere);
  sphere.setResolution(20);
  sphere.setRadius(radius);
  sphere.mapTexCoordsFromTexture(colorTexture.getTexture());
  //sphere.mapTexCoordsFromTexture(texture_arbitrary_diffuse.getTexture());
  sphere.setMode(OF_PRIMITIVE_TRIANGLES);


  position_box = ofVec3f{0.0f, 3.0f, -6.0f};
  box.setPosition(position_box);
  box.set(1.0f);
  box.mapTexCoordsFromTexture(colorTexture.getTexture());
  box.setMode(OF_PRIMITIVE_TRIANGLES);

  //Ajout dun plancher de type plane primitive 'a 0,0,0 de grandeur 1.0f
  //rotate de 90 sur x
  position_plane = ofVec3f{0.0f, -1.0f, 0.0f};
  scale_plane = 10.0f;
  plane.setPosition(position_plane);
  plane.set(scale_plane, scale_plane);
  plane.rotateDeg(90, 1, 0, 0);
  plane.mapTexCoordsFromTexture(colorTexture.getTexture());
  plane.setMode(OF_PRIMITIVE_TRIANGLES);


  position_cube_map_tracer = ofVec3f{0.0f, 0.0f, -30.0f};
  cube_map_tracer.setPosition(position_cube_map_tracer);
  scale_cube_map_tracer = 30.0f;
  cube_map_tracer.set(scale_cube_map_tracer);
  cube_map_tracer.mapTexCoordsFromTexture(colorTexture.getTexture());
  cube_map_tracer.setMode(OF_PRIMITIVE_TRIANGLES);

}

void Renderer::drawScene()
{
  scale = 1.0f;
  ofPushMatrix();
  ofScale(scale, scale, scale);
  sphere.draw();
  ofPopMatrix();

  //BOX-BIG
  ofPushMatrix();
  ofScale(scale, scale, scale);
  box.draw();
  ofPopMatrix();

  //PLANE
  ofPushMatrix();
  ofScale(scale_plane, scale_plane, scale_plane);
  plane.draw();
  ofPopMatrix();

  ofPushMatrix();
  cube_map_tracer.draw();
  ofPopMatrix();

}
//for debug
void Renderer::drawLights(){
  float scale = 0.08f;
  ofSetColor( pointLight.getDiffuseColor() );
  if(bPointLight) 
  {
    ofPushMatrix();
    ofScale(scale, scale, scale);
    pointLight.draw();
    ofPopMatrix();
  }
    
  ofSetColor(255, 255, 255);
  if(bDirLight)
  {
    ofPushMatrix();
    ofScale(scale, scale, scale);
    directionalLight.draw();
    directionalLight2.draw();
    ofPopMatrix();
  }
    
  ofSetColor( spotLight.getDiffuseColor() );
  if(bSpotLight)
  {
    ofPushMatrix();
    ofScale(scale, scale, scale);
    spotLight.draw();
    ofPopMatrix();
  }
  if (bAmbientLight)
  {
    ofPushMatrix();
    ofScale(scale, scale, scale);
    ambientLight.draw();
    ofPopMatrix();
  }
}


void Renderer::init_camera()
{
  // paramètres
  camera_position = ofVec3f{0.0f, 60.0f, 0.0f};
  camera_target = ofVec3f(0.0f, 60.0f, 0.0f);

  camera_near = 1.0f;
  camera_far = 150.0f;

  float verticalFieldOfView = PI / 3.0f;
  const float side = -2.0f * tan(verticalFieldOfView / 2.0f);
  ofLog() << "side: " << side;


  camera_fov = 60.0f;
  camera_fov_delta = 16.0f;

  speed_delta = 250.0f;

  is_visible_grid = false;
  is_visible_camera = false;
  is_visible_frustum = false;

  is_camera_move_left = false;
  is_camera_move_right = false;
  is_camera_move_up = false;
  is_camera_move_down = false;
  is_camera_move_forward = false;
  is_camera_move_backward = false;

  is_camera_tilt_up = false;
  is_camera_tilt_down = false;
  is_camera_pan_left = false;
  is_camera_pan_right = false;
  is_camera_roll_left = false;
  is_camera_roll_right = false;

  is_camera_fov_narrow = false;
  is_camera_fov_wide = false;

  is_camera_perspective = true;
}

void Renderer::setup_camera()
{
  switch (camera_active)
  {
    case Camera::front:
      camera = &camera_front;
      camera_name = "avant";
      break;

    case Camera::back:
      camera = &camera_back;
      camera_name = "arrière";
      //mettre position de la camera back 'a 1 et direction vers 0,0,0
      //simule le pinholecam de laddons ratracer si on la bouge pas au debut
      camera_back.setPosition(0.0f, 0.0f, 1.0f);
      camera_back.lookAt(ofVec3f(0.0f, 0.0f, 0.0f));
      break;

    //ajouter cam player with locked mouse

    default:
      break;
  }

  camera_position = camera->getPosition();
  camera_orientation = camera->getOrientationQuat();

  // mode de projection de la caméra
  if (is_camera_perspective)
  {
    camera->disableOrtho();
    camera->setupPerspective(false, camera_fov, camera_near, camera_far, ofVec2f(0, 0));
    camera_projection = "perspective";
  }
  else
  {
    camera->enableOrtho();
    camera_projection = "orthogonale";
  }

  camera->setPosition(camera_position);
  camera->setOrientation(camera_orientation);

  ofLog() << "<setup camera: " << camera_name << ">";
}

void Renderer::cam_reset()
{
  // initialisation des variables
  offset_camera = -20.0f;

  // position initiale de chaque caméra
  camera_front.setPosition(0.0f, 3.0f, -offset_camera);
  camera_back.setPosition(0.0f, 3.0f, -offset_camera);

  // orientation de chaque caméra
  camera_front.lookAt(ofVec3f(0.0f, 3.0f, 0.0f));
  camera_back.lookAt(ofVec3f(0.0f, 3.0f, 0.0f));


  // caméra par défaut
  camera_active = Camera::front;

  ofLog() << "<cam reset>";
}

void Renderer::cam_update()
{
  time_current = ofGetElapsedTimef();
  time_elapsed = time_current - time_last;
  time_last = time_current;

  speed_translation = speed_delta * time_elapsed * 0.06f;
  speed_rotation = speed_translation;

  if (is_camera_move_left)
      camera->truck(-speed_translation);
  if (is_camera_move_right)
      camera->truck(speed_translation);

  if (is_camera_move_up)
      camera->boom(speed_translation);
  if (is_camera_move_down)
      camera->boom(-speed_translation);

  if (is_camera_move_forward)
      camera->dolly(-speed_translation);
  if (is_camera_move_backward)
      camera->dolly(speed_translation);

  if (is_camera_tilt_up)
      camera->tiltDeg(-speed_rotation);
  if (is_camera_tilt_down)
      camera->tiltDeg(speed_rotation);
  if (is_camera_pan_left)
      camera->panDeg(speed_rotation);
  if (is_camera_pan_right)
      camera->panDeg(-speed_rotation);

  if (is_camera_roll_left)
    camera->rollDeg(-speed_rotation);
  if (is_camera_roll_right)
    camera->rollDeg(speed_rotation);

  if (is_camera_perspective)
  {
    if (is_camera_fov_narrow)
    {
      camera_fov = std::max(camera_fov -= camera_fov_delta * time_elapsed, 0.0f);
      camera->setFov(camera_fov);
    }

    if (is_camera_fov_wide)
    {
      camera_fov = std::min(camera_fov += camera_fov_delta * time_elapsed, 180.0f);
      camera->setFov(camera_fov);
    }
  }
}

void Renderer::initialize_cubemap()
{
  std::string faces[6] = { "Cubemaps/posx.jpg", "Cubemaps/negx.jpg", "Cubemaps/posy.jpg", "Cubemaps/negy.jpg", "Cubemaps/posz.jpg", "Cubemaps/negz.jpg" };
  std::swap(faces[2], faces[3]);
  cube_half_size = -50.0f;
  cube_position = ofVec3f(0.0f, 0.0f, 0.0f);
  for (int i = 0; i < 6; i++)
  {
    cubemapFaces[i].load(faces[i]);
    ofLog() << "Cubemap loaded: " << faces[i];
  }
}

void Renderer::draw_cube_map()
{
  
  ofPushMatrix();
  float halfSize = cube_half_size;
  ofTranslate(cube_position);
  for (int i = 0; i < 6; i++) {
    cubemapFaces[i].getTexture().bind();

    ofMesh faceMesh;
    faceMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);

    switch (i) 
    {
      case 0: // Face +X
        faceMesh.addVertex(ofPoint(halfSize, -halfSize, halfSize));
        faceMesh.addVertex(ofPoint(halfSize, -halfSize, -halfSize));
        faceMesh.addVertex(ofPoint(halfSize, halfSize, -halfSize));
        faceMesh.addVertex(ofPoint(halfSize, halfSize, halfSize));
        break;
      case 1: // Face -X
        faceMesh.addVertex(ofPoint(-halfSize, -halfSize, -halfSize));
        faceMesh.addVertex(ofPoint(-halfSize, -halfSize, halfSize));
        faceMesh.addVertex(ofPoint(-halfSize, halfSize, halfSize));
        faceMesh.addVertex(ofPoint(-halfSize, halfSize, -halfSize));
        break;
      case 2: // Face +Y
        faceMesh.addVertex(ofPoint(-halfSize, halfSize, halfSize));
        faceMesh.addVertex(ofPoint(halfSize, halfSize, halfSize));
        faceMesh.addVertex(ofPoint(halfSize, halfSize, -halfSize));
        faceMesh.addVertex(ofPoint(-halfSize, halfSize, -halfSize));
        break;
      case 3: // Face -Y
        faceMesh.addVertex(ofPoint(-halfSize, -halfSize, -halfSize));
        faceMesh.addVertex(ofPoint(halfSize, -halfSize, -halfSize));
        faceMesh.addVertex(ofPoint(halfSize, -halfSize, halfSize));
        faceMesh.addVertex(ofPoint(-halfSize, -halfSize, halfSize));
        break;
      case 4: // Face +Z
        faceMesh.addVertex(ofPoint(-halfSize, -halfSize, halfSize));
        faceMesh.addVertex(ofPoint(halfSize, -halfSize, halfSize));
        faceMesh.addVertex(ofPoint(halfSize, halfSize, halfSize));
        faceMesh.addVertex(ofPoint(-halfSize, halfSize, halfSize));
        break;
      case 5: // Face -Z
        faceMesh.addVertex(ofPoint(halfSize, -halfSize, -halfSize));
        faceMesh.addVertex(ofPoint(-halfSize, -halfSize, -halfSize));
        faceMesh.addVertex(ofPoint(-halfSize, halfSize, -halfSize));
        faceMesh.addVertex(ofPoint(halfSize, halfSize, -halfSize));
        break;
    }
    // Ajout des coordonn�es de texture pour chaque sommet
    faceMesh.addTexCoord(cubemapFaces[i].getTexture().getCoordFromPercent(0, 0));
    faceMesh.addTexCoord(cubemapFaces[i].getTexture().getCoordFromPercent(1, 0));
    faceMesh.addTexCoord(cubemapFaces[i].getTexture().getCoordFromPercent(1, 1));
    faceMesh.addTexCoord(cubemapFaces[i].getTexture().getCoordFromPercent(0, 1));

    faceMesh.draw();

    cubemapFaces[i].getTexture().unbind();
  }

  ofPopMatrix();
}

shared_ptr<ofImage> Renderer::initImage(int _width, int _height)
{
  shared_ptr<ofImage> img = std::make_shared<ofImage>();
  img->allocate(_width, _height, OF_IMAGE_COLOR_ALPHA);
  return img;
}

void Renderer::light_update()
{
  if (is_light_move_up)
  {
    light_position.y += speed_translation;
    ofLog() << "Light Position: " << light_position;
  }
  if
   (is_light_move_down)
  {
    light_position.y -= speed_translation;
    ofLog() << "Light Position: " << light_position;
  }
  if (is_light_move_left)
  {
    light_position.x -= speed_translation;
    ofLog() << "Light Position: " << light_position;
  }
  if (is_light_move_right)
  {
    light_position.x += speed_translation;
    ofLog() << "Light Position: " << light_position;
  }
  if (is_light_move_forward)
  {
    light_position.z += speed_translation;
    ofLog() << "Light Position: " << light_position;
  }
  if (is_light_move_backward)
  {
    light_position.z -= speed_translation;
    ofLog() << "Light Position: " << light_position;
  }

  spotLight.setPosition(light_position);
  //ofLog() << "Spot Light Position: " << light_position;
  //ofLog() << "Spot Light Target: " << light_target;
  spotLight.lookAt(light_target);
}

void Renderer::cam_draw_frustum()
{
  if (camera_active != Camera::front)
    camera_front.drawFrustum();
  if (camera_active != Camera::back)
    camera_back.drawFrustum();
}

void Renderer::set_camera_active()
{
  switch (camera_active) {
  case Camera::front:
      shaderManager.setCamera(&camera_front);
      break;
  case Camera::back:
      shaderManager.setCamera(&camera_back);
      break;
  default:
      // Gérer le cas où la caméra active n'est pas définie correctement
      break;
  }
}

//Light getter
ofVec3f Renderer::getPointLightPosition(){
  return pointLight.getPosition();
}

ofVec3f Renderer::getSpotLightPosition(){
  return spotLight.getPosition();
}

ofVec3f Renderer::getDirectionalLightPosition(){
  return directionalLight.getPosition();
}


void Renderer::setup_primitives()
{
  scale_sphere = 5.0f;
  position_sphere_pbr = ofVec3f(-10.0f, 10.0f, 0.0f);
  sphere_pbr.setRadius(scale_sphere);
  sphere_pbr.setResolution(32);
  sphere_pbr.setPosition(position_sphere_pbr);

  position_cylinder = ofVec3f{4.0f, 0.0f, -5.0f};
  radius_cylinder = 1.0f;
  height_cylinder = 2.0f;
  cylinder.setPosition(position_cylinder);
  cylinder.set(radius_cylinder, height_cylinder);
  cylinder.setResolution(20, 20);

  //setup light
  light.setGlobalPosition(50, -175, 1);
}

void Renderer::pbr_setup()
{
  ofDisableArbTex();

  // charger, compiler et linker les sources des shaders
  shader_pbr.load(
    "shader/pbr_norm_330_vs.glsl",
    "shader/pbr_norm_330_fs.glsl");

  // charger les textures du matériau
  texture_diffuse.load("textures_pbr/metal_diffuse_1.jpg");
  texture_metallic.load("textures_pbr/metal_metal_1.jpg");
  texture_roughness.load("textures_pbr/metal_roughness_1.jpg");
  texture_occlusion.load("textures_pbr/metal_occlusion_1.jpg");
  texture_normal.load("textures_pbr/metal_normal_1.jpg");

  texture_cylinder_diffuse.load("textures_pbr/dry_riverbed_rock_diffuse_1k.jpg");
  texture_cylinder_metallic.load("textures_pbr/dry_riverbed_rock_metallic_1k.jpg");
  texture_cylinder_roughness.load("textures_pbr/dry_riverbed_rock_roughness_1k.jpg");
  texture_cylinder_occlusion.load("textures_pbr/dry_riverbed_rock_occlusion_1k.jpg");
  texture_cylinder_normal.load("textures_pbr/dry_riverbed_rock_nor_gl_1k.jpg");


  // paramètres des textures du matériau
  texture_diffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_metallic.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_roughness.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_occlusion.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

  texture_cylinder_diffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_cylinder_metallic.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_cylinder_roughness.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_cylinder_occlusion.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_cylinder_normal.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

  ofEnableArbTex();

  setup_primitives();
}

void Renderer::pbr_reset()
{
  initial_x = 0.0f;
  initial_z = -1.0f;
  scale_sphere = 30.0f;//radius

  // positions initiales des maillages géométriques
  position_sphere_pbr.set(0.0f, 0.0f, 0.0f);

  // paramètres du matériau
  material_color_ambient = ofColor(63, 63, 63);
  material_color_diffuse = ofColor(255, 255, 255);
  material_color_specular = ofColor(255, 255, 255);

  material_metallic = 0.5f;
  material_roughness = 0.5f;
  material_occlusion = 1.0f;
  material_brightness = 1.0f;

  material_fresnel_ior = glm::vec3(0.04f, 0.04f, 0.04f);

  // paramètres de la lumière
  light_color = ofColor(255, 255, 255);
  light_intensity = 1.0f;
  light_motion = false;

  // paramètres de mappage tonal
  tone_mapping_exposure = 1.0f;
  tone_mapping_toggle = true;
  tone_mapping_gamma = 2.2f;

  // position initiale de la caméra
  offset_z = initial_z;

  ofLog() << "<reset>";
}

void Renderer::pbr_update()
{
  if (light_motion)
  {
    // transformer la lumière
    light.setGlobalPosition(
      ofMap(ofGetMouseX() / (float) ofGetWidth(), 0.0f, 1.0f, -center_x, center_x),
      ofMap(ofGetMouseY() / (float) ofGetHeight(), 0.0f, 1.0f, -center_y, center_y),
      -offset_z * 1.0f);
  }
  // passer les attributs uniformes au shader de sommets
  //update_shader_pbr();
}

void Renderer::pbr_draw()
{
  // activer l'occlusion en profondeur
  //ofEnableDepthTest();
  camera->begin();
  // activer l'éclairage dynamique
  ofEnableLighting();
  // activer la lumière dynamique
  light.enable();
  // activer le shader
  update_shader_pbr();
  shader_pbr.begin();
  sphere_pbr.draw();
  shader_pbr.end();

  update_shader_pbr_second_texture();
  shader_pbr.begin();
  cylinder.draw();
  shader_pbr.end();
  // désactiver la lumière
  light.disable();
  // désactiver l'éclairage dynamique
  ofDisableLighting();
  // désactiver l'occlusion en profondeur
  //ofDisableDepthTest();
  camera->end();
}

void Renderer::update_shader_pbr()
{
  shader_pbr.begin();

  shader_pbr.setUniform3f("material_color_ambient", material_color_ambient.r/255.0f, material_color_ambient.g/255.0f, material_color_ambient.b/255.0f);
  shader_pbr.setUniform3f("material_color_diffuse", material_color_diffuse.r/255.0f, material_color_diffuse.g/255.0f, material_color_diffuse.b/255.0f);
  shader_pbr.setUniform3f("material_color_specular", material_color_specular.r/255.0f, material_color_specular.g/255.0f, material_color_specular.b/255.0f);

  shader_pbr.setUniform1f("material_brightness", material_brightness);
  shader_pbr.setUniform1f("material_metallic", material_metallic);
  shader_pbr.setUniform1f("material_roughness", material_roughness);
  shader_pbr.setUniform1f("material_occlusion", material_occlusion);

  shader_pbr.setUniform3f("material_fresnel_ior", material_fresnel_ior);

  shader_pbr.setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
  shader_pbr.setUniformTexture("texture_metallic", texture_metallic.getTexture(), 2);
  shader_pbr.setUniformTexture("texture_roughness", texture_roughness.getTexture(), 3);
  shader_pbr.setUniformTexture("texture_occlusion", texture_occlusion.getTexture(), 4);
  shader_pbr.setUniformTexture("texture_normal", texture_normal.getTexture(), 5);

  shader_pbr.setUniform1f("light_intensity", light_intensity);
  shader_pbr.setUniform3f("light_color", light_color.r/255.0f, light_color.g/255.0f, light_color.b/255.0f);
  shader_pbr.setUniform3f("light_position", light.getGlobalPosition());

  shader_pbr.setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
  shader_pbr.setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
  shader_pbr.setUniform1i("tone_mapping_toggle", tone_mapping_toggle);

  shader_pbr.end();
}

void Renderer::update_shader_pbr_second_texture()
{
  shader_pbr.begin();

  shader_pbr.setUniform3f("material_color_ambient", material_color_ambient.r/255.0f, material_color_ambient.g/255.0f, material_color_ambient.b/255.0f);
  shader_pbr.setUniform3f("material_color_diffuse", material_color_diffuse.r/255.0f, material_color_diffuse.g/255.0f, material_color_diffuse.b/255.0f);
  shader_pbr.setUniform3f("material_color_specular", material_color_specular.r/255.0f, material_color_specular.g/255.0f, material_color_specular.b/255.0f);

  shader_pbr.setUniform1f("material_brightness", material_brightness);
  shader_pbr.setUniform1f("material_metallic", material_metallic);
  shader_pbr.setUniform1f("material_roughness", material_roughness);
  shader_pbr.setUniform1f("material_occlusion", material_occlusion);

  shader_pbr.setUniform3f("material_fresnel_ior", material_fresnel_ior);

  shader_pbr.setUniformTexture("texture_diffuse", texture_cylinder_diffuse.getTexture(), 1);
  shader_pbr.setUniformTexture("texture_metallic", texture_cylinder_metallic.getTexture(), 2);
  shader_pbr.setUniformTexture("texture_roughness", texture_cylinder_roughness.getTexture(), 3);
  shader_pbr.setUniformTexture("texture_occlusion", texture_cylinder_occlusion.getTexture(), 4);
  shader_pbr.setUniformTexture("texture_normal", texture_cylinder_normal.getTexture(), 5);

  shader_pbr.setUniform1f("light_intensity", light_intensity);
  shader_pbr.setUniform3f("light_color", light_color.r/255.0f, light_color.g/255.0f, light_color.b/255.0f);
  shader_pbr.setUniform3f("light_position", light.getGlobalPosition());
  //1 lum
  shader_pbr.setUniform1i("lightsNumber", 1);

  shader_pbr.setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
  shader_pbr.setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
  shader_pbr.setUniform1i("tone_mapping_toggle", tone_mapping_toggle);

  shader_pbr.end();
}

void Renderer::setup_tessellation()
{
  // chargement du shader de tessellation
  shader_tessellation.setGeometryInputType(GL_TRIANGLES);
  shader_tessellation.setGeometryOutputType(GL_TRIANGLE_STRIP);
  shader_tessellation.setGeometryOutputCount(4);

  shader_tessellation.setupShaderFromFile(GL_VERTEX_SHADER, "tes_shader/tessQuads.vert");
  shader_tessellation.setupShaderFromFile(GL_FRAGMENT_SHADER, "tes_shader/tessQuads.frag");
  shader_tessellation.setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, "tes_shader/tessQuads.geom");
  shader_tessellation.setupShaderFromFile(GL_TESS_CONTROL_SHADER, "tes_shader/tessQuads.cont");
  shader_tessellation.setupShaderFromFile(GL_TESS_EVALUATION_SHADER, "tes_shader/tessQuads.eval");
  shader_tessellation.linkProgram();
  glPatchParameteri(GL_PATCH_VERTICES, 4);

  //verifier si le shader est bien chargé
  if (!shader_tessellation.isLoaded())
  {
      ofLogError() << "Failed to load shader";
  }
  setupVbos();
}

void Renderer::setupVbos()
{
  ofIcoSpherePrimitive sphere;
  ofBoxPrimitive box;
  //mettre le mesh a fill
  sphere.setMode(OF_PRIMITIVE_TRIANGLES);
  radius_tess = 2.0f;
  position_sphere_tess = ofVec3f{-20.0f, 0.0f, -5.0f};
  sphere.setRadius(radius_tess);
  sphere.setResolution(1);
  ofVboMesh sphereMesh = sphere.getMesh();
  sphereVbo.setMesh(sphereMesh, GL_DYNAMIC_DRAW);
  
  position_box_tess = ofVec3f{-20.0f, 0.0f, 15.0f};
  box.set(10);
  box.setResolution(1);
  ofVboMesh boxMesh = box.getMesh();
  boxVbo = boxMesh.getVbo();
  //log vbo sans erreur
  ofLog() << "VBOs created without errors";
    
}

void Renderer::drawScene_tess()
{
  shader_tessellation.begin();
  shader_tessellation.setUniform1f("u_tessLevelInner", tessLevelInner);
  shader_tessellation.setUniform1f("u_tessLevelOuter", tessLevelOuter);
  //SPHERE
  ofPushMatrix();
  ofTranslate(position_sphere_tess);
  ofRotate(ofGetElapsedTimef() * .2 * RAD_TO_DEG, 0, 1, 0);
  sphereVbo.drawElements(GL_PATCHES, sphereVbo.getNumIndices());
  ofPopMatrix();
  //BOX
  ofPushMatrix();
  ofTranslate(position_box_tess);
  ofRotate(ofGetElapsedTimef() * .2 * RAD_TO_DEG, 0, 1, 0);
  boxVbo.drawElements(GL_PATCHES,  boxVbo.getNumIndices());
  ofPopMatrix();
  shader_tessellation.end();
}

void Renderer::setup_texture_arbitrary()
{
  ofDisableArbTex();
  texture_arbitrary_diffuse.load("textures_other/brick_diffuse.jpg");
  texture_arbitrary_metallic.load("textures_other/brick_metal.jpg");
  texture_arbitrary_roughness.load("textures_other/brick_roughness.jpg");
  texture_arbitrary_occlusion.load("textures_other/brick_occlusion.jpg");
  texture_arbitrary_normal.load("textures_other/brick_normal.jpg");

  texture_arbitrary_diffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_arbitrary_metallic.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_arbitrary_roughness.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_arbitrary_occlusion.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_arbitrary_normal.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  ofEnableArbTex();
}

void Renderer::exit()
{
  //empty vector
  materials.clear();
  primitives.clear();

  bPointLight.removeListener(this, &Renderer::pointLightChanged);
  bSpotLight.removeListener(this, &Renderer::spotLightChanged);
  bDirLight.removeListener(this, &Renderer::dirLightChanged);
  bAmbientLight.removeListener(this, &Renderer::ambientLightChanged);
  bUseTexture.removeListener(this, &Renderer::textureToggled);
  //Delete sphere
  ofLog() << "<exit>";
}