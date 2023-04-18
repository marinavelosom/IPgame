#include "raylib.h"
#include "raymath.h"

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f
#define NUM_FRAMES  2

typedef struct Player {
    Vector2 position;
    float speed;
    bool canJump;
} Player;

typedef struct EnvItem {
    Rectangle rect;
    int blocking;
    Color color;
} EnvItem;

typedef struct Bat {
    Vector2 position;
} Bat;

typedef struct Mush {
    Vector2 position;
} Mush;
//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);
void UpdateCameraCenter(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
bool ButtonsMenu(Vector2 mousePoint, bool btnAction, Rectangle btnBounds, Sound fxButton);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    bool Start = false;
    bool Pause = false;
    char escolha;
    bool grounded = true;
    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    const int characterSpeed = 10;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");
    InitAudioDevice();
    
    Music music = LoadMusicStream("audio/music.wav"); 
    PlayMusicStream(music);
    
    const char *andando1 = "assets/Run 1.png";
	Texture2D playerTexture1Andando = LoadTexture(andando1);

    const char *parado1 = "assets/Idle 1.png";
	Texture2D playerTexture1Parado = LoadTexture(parado1);

    const char *andando2 = "assets/Run 2.png";
	Texture2D playerTexture2Andando = LoadTexture(andando2);

    const char *parado2 = "assets/Idle 12.png";
	Texture2D playerTexture2Parado = LoadTexture(parado2);

    const char *fall = "assets/Fall 1.png";
	Texture2D playerTexture1Fall = LoadTexture(fall);

    const char *fall2 = "assets/Fall 2.png";
	Texture2D playerTexture2Fall = LoadTexture(fall2);
    
    
    //MENU
    //button main menu -------------------------------------------------------------------------------------
    Sound fxControlButton = LoadSound("audio/buttonfx.wav");   // Load button sound
    Sound fxButton = LoadSound("audio/start2.wav");   
    Sound fxButtonBack = LoadSound("audio/click.mp3");   
    
    Texture2D button = LoadTexture("assets/buttonStartTest.png"); // Load start button texture
    Texture2D controlButton = LoadTexture("assets/buttonControlsTest.png"); // Load controls button texture
    
    bool btnAction = false;         
    bool btnActionControl = false;
    
    Vector2 mousePoint = { 0.0f, 0.0f };
    
    //-------------- Config. Button Control ------------------------
    Texture2D controls = LoadTexture("assets/controlMenu.png");
    Texture back = LoadTexture("assets/backButton.png");
    
    //-------------- Config. Frames ------------------------
    float frameHeight = (float)button.height/NUM_FRAMES;
    Rectangle sourceRec = { 0, 0, (float)button.width, frameHeight };
    Rectangle sourceRecBack = { 0, 0, (float)back.width, (float)back.height };
    
    //-------------- Config. Buttons Rec ------------------------
    Rectangle btnBounds = { screenWidth/2.0f - controlButton.width/2.0f, screenHeight/2.0f - controlButton.height/NUM_FRAMES/2.0f, (float)controlButton.width, frameHeight };
    Rectangle btnControlBounds = { screenWidth/2.0f - controlButton.width/2.0f, ( screenHeight/2.0f - controlButton.height/NUM_FRAMES/2.0f ) + 100, (float)controlButton.width, frameHeight };
    Rectangle btnBackBounds = { 330, 38, (float)back.width, frameHeight  };
    
    
    //-------------- Config. Title -----------------------------------------
    
    Texture2D title = LoadTexture("assets/title.png");
    
    //-------------- Config. background -----------------------------------------
    Texture2D sky = LoadTexture("assets/sky.png");
    Texture2D mountainBack = LoadTexture("assets/mountainsBack.png");
    Texture2D mountainFront = LoadTexture("assets/mountainsFront.png");
    Texture2D bushes = LoadTexture("assets/bushes.png");
    
    float scrollingSky = 0.0f;
    float scrollingBack = 0.0f;
    float scrollingFront = 0.0f;
    float scrollingBush = 0.0f;
    
    float scrollingSkyMenu = 0.0f;
    float scrollingBackMenu = 0.0f;
    float scrollingFrontMenu = 0.0f;
    float scrollingBushMenu = 0.0f;
    
    Player player = { 0 };
    player.position = (Vector2){ 400, 280 };
    player.speed = 0;
    player.canJump = false;
    
    //-------------- Config. Map ------------------------
    
    Color transparent = (Color){0, 0, 0, 0};
    
    EnvItem envItems[] = {
      //{{ X, Y, W, H }},
        {{ 0, 600, 300, 30 }, 1, transparent}, //plataform 1
        {{ 300, 490, 660, 140 }, 1, transparent }, //floor 1
        {{ 1380, 490, 580, 140 }, 1, transparent }, //floor 2
        {{ 2600, 490, 580, 140 }, 1, transparent }, //floor 3
        {{ 1050, 400, 100, 10 }, 1, transparent }, 
        {{ 1200, 300, 100, 10 }, 1, transparent },
        {{ 100, 90, 100, 10 }, 1, transparent },
        {{ 965, 600, 415, 28 }, 1, transparent }, //lake 1
        {{ 2050, 400, 165, 10 }, 1, transparent },
        {{ 2300, 400, 165, 10 }, 1, transparent },
        {{ 3300, 490, 580, 140 }, 1, transparent }, //floor 3
    };
    
    int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);
    
    //-------------- Textures map ------------------------
    const char *floor = "assets/floor1.png";
    Texture2D TextureFloor = LoadTexture(floor);
    
    const char *floor2 = "assets/floor2.png";
    Texture2D TextureFloor2 = LoadTexture(floor2);
    
    const char *floor3 = "assets/floor3.png";
    Texture2D TextureFloor3 = LoadTexture(floor3);
    
    const char *lake = "assets/lake1.png";
    Texture2D TextureLake = LoadTexture(lake);
    
    const char *plataform1 = "assets/plataformSky1.png";
    Texture2D TexturePlataform = LoadTexture(plataform1);
    
    const char *plataform2 = "assets/plataformSky2.png";
    Texture2D TexturePlataform2 = LoadTexture(plataform2);
    
    const char *plataformGround = "assets/plataformGround.png";
    Texture2D TexturePlataformG = LoadTexture(plataformGround);
    
    //-------------- Objects ------------------------
    const char *tree = "assets/tree2.png";
    Texture2D TextureTree = LoadTexture(tree);
    
    const char *plate = "assets/plate1.png";
    Texture2D TexturePlate = LoadTexture(plate);
    
    const char *rock = "assets/rock1.png";
    Texture2D TextureRock = LoadTexture(rock);
    
    //-------------- Camera ------------------------
    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    
    // ----------------Config dos frames andando -----------------------------------------

    unsigned numFramesAndando = 4; //quantidade de sprites na imagem
	int frameWidthAndando = playerTexture1Andando.width / numFramesAndando;
	Rectangle frameRecAndando = { 0.2f, 0.2f, (float)frameWidthAndando/2, (float)playerTexture1Andando.height };
    Vector2 characterPosition = {screenWidth / 1.5f, screenHeight / 1.5f};
    Vector2 characterVelocity = {0.0f,0.0f};
    
    unsigned numFramesAndando2 = 8; //quantidade de sprites na imagem
	int frameWidthAndando2 = playerTexture2Andando.width / numFramesAndando2;
	Rectangle frameRecAndando2 = { 0.2f, 0.2f, (float)frameWidthAndando2, (float)playerTexture2Andando.height };
    
    unsigned frameDelay = 6;
	unsigned frameDelayCounter = 0;
    unsigned frameIndex = 0;
    unsigned frameIndexF = 0;
    unsigned frameIndexF2 = 0;
    unsigned frameIndexBat = 0;
    unsigned frameIndexMush = 0;

    // -----------------------Config dos frames parado---------------------------------------

    unsigned numFramesParado = 8; //quantidade de sprites na imagem
	int frameWidthParado = playerTexture1Parado.width / numFramesParado;
	Rectangle frameRecParado = { 0.2f, 0.2f, (float)frameWidthParado, (float)playerTexture1Parado.height };

    unsigned numFramesParado2 = 10; //quantidade de sprites na imagem
	int frameWidthParado2 = playerTexture2Parado.width / numFramesParado2;
	Rectangle frameRecParado2 = { 0.2f, 0.2f, (float)frameWidthParado2, (float)playerTexture2Parado.height };

    
    //--------------------------- Config dos frames pulando-----------------------------------

    unsigned numFramesFall = 2; //quantidade de sprites na imagem
	int frameWidthFall = playerTexture1Fall.width / numFramesFall;
	Rectangle frameRecFall = { 0.2f, 0.2f, (float)frameWidthFall, (float)playerTexture1Fall.height };

    unsigned numFramesFall2 = 2; //quantidade de sprites na imagem
	int frameWidthFall2 = playerTexture2Fall.width / numFramesFall2;
	Rectangle frameRecFall2 = { 0.2f, 0.2f, (float)frameWidthFall2, (float)playerTexture2Fall.height };

     //--------------------------- Config camera--------------------------------------------------

    // Store pointers to the multiple update camera functions
    void (*cameraUpdaters[])(Camera2D*, Player*, EnvItem*, int, float, int, int) = {
        UpdateCameraCenter,
    };

    int cameraOption = 0;

    SetTargetFPS(60);
    
    //--------------------------------------------------------------------------------------
    int auxChoiceCharacter = 0, auxControl = 0, aux = 0;
    
    //===================== Confg. bat ======================================================
    
    const char *bat = "assets/batSprite.png";
    Texture2D TextureBat = LoadTexture(bat);
    
    unsigned numFramesBat = 4;
    int frameWidthBat = TextureBat.width / numFramesBat;
    Rectangle frameBat = { 0.2f, 0.2f, (float)frameWidthBat, (float)TextureBat.height };
    
    Bat bat1 = { 0 };
    bat1.position = (Vector2){ 1700, 380 };
    
    //=======================================================================================
    
    //===================== Confg. mushroom ======================================================
    
    const char *mush = "assets/mushroomIdle.png";
    Texture2D TextureMush = LoadTexture(mush);
    
    unsigned numFramesMush = 4;
    int frameWidthMush = TextureMush.width / numFramesMush;
    Rectangle frameMush = { 0.2f, 0.2f, (float)frameWidthMush, (float)TextureMush.height };
    
    Mush mush1 = { 0 };
    mush1.position = (Vector2){ 3400, 195 };
    
    //=======================================================================================
    
    // Main game loop
    while (!WindowShouldClose())
    {   
        // Update 
        UpdateMusicStream(music);
        
        //Start button ----------------------------------------------------------------------------------
        mousePoint = GetMousePosition();
        if (CheckCollisionPointRec(mousePoint, btnBounds)) {
            if ( IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && aux == 0 ) {
                PlaySound(fxButton);
                btnAction = true;
                aux++;
            }
        } 
        
        //Control button ----------------------------------------------------------------------------------
        if (CheckCollisionPointRec(mousePoint, btnControlBounds)) {
            if ( IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && auxControl % 2 == 0 ) {
                PlaySound(fxControlButton);
                btnActionControl = true;
                auxControl = auxControl + 2;
            }
        } 
        
        //Back button ----------------------------------------------------------------------------------
        if (CheckCollisionPointRec(mousePoint, btnBackBounds)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(fxButtonBack);
                btnActionControl = false;
            }
        } 
        
        //definitions for background menu ----------------------------------------------------------------------------------
        
        scrollingSkyMenu -= 0.1f;
        scrollingBackMenu -= 0.5f;
        scrollingFrontMenu -= 1.0f;
        scrollingBushMenu -= 1.5f;
        
        if(scrollingSkyMenu <= -sky.width * 2) scrollingSkyMenu = 0;
        if(scrollingBackMenu <= -mountainBack.width * 2) scrollingBackMenu = 0;
        if(scrollingFrontMenu <= -mountainFront.width * 2) scrollingFrontMenu = 0;
        if(scrollingBushMenu <= -bushes.width * 2) scrollingBushMenu = 0;
        
        if(scrollingSky <= -sky.width * 2) scrollingSky = 0;
        if(scrollingBack <= -mountainBack.width * 2) scrollingBack = 0;
        if(scrollingFront <= -mountainFront.width * 2) scrollingFront = 0;
        if(scrollingBush <= -bushes.width * 2) scrollingBush = 0;
        
        //-----------------------------zoom config------------------------------------------------
        
        float deltaTime = GetFrameTime();

        if(Start) UpdatePlayer(&player, envItems, envItemsLength, deltaTime);

        camera.zoom += ((float)GetMouseWheelMove()*0.05f);

        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.25f) camera.zoom = 0.25f;

        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            player.position = (Vector2){ 400, 280 };
        }

        //------atualizacao de frames do character----------------------------------------

        if(!Start) {//No Menu
            ++frameDelayCounter;
            if(frameDelayCounter > frameDelay) {
                frameDelayCounter = 0;
            
                ++frameIndex;
                frameIndex %= numFramesParado;
                frameRecParado.x = (float) frameWidthParado * frameIndex;
                
                ++frameIndex;
                frameIndex %= numFramesParado2;
                frameRecParado2.x = (float) frameWidthParado2 * frameIndex;
            }
        } else {//Dentro do jogo
            
            //Verificando se hitou o obstaculo
            float delta;
            int hitObstacle = 0;
            for (int i = 0; i < envItemsLength; i++){
                EnvItem *ei = envItems + i;
                Vector2 *p = &(player.position);
                if (ei->blocking && ei->rect.x <= p->x && ei->rect.x + ei->rect.width >= p->x && ei->rect.y >= p->y && ei->rect.y <= p->y + player.speed*delta) {
                    hitObstacle = 1;
                    player.speed = 0.0f;
                    p->y = ei->rect.y;
                }
            }
            if (!hitObstacle) {// Se não hitar o obstaculo
                player.position.y += player.speed*delta;
                player.speed += G*delta;
                player.canJump = false;
                grounded = false;

                ++frameDelayCounter;
                if(frameDelayCounter > frameDelay) {
                    frameDelayCounter = 0;

                    ++frameIndexF;
                    frameIndexF %= numFramesFall;
                    frameRecFall.x = (float) frameWidthFall * frameIndexF;

                    ++frameIndexF2;
                    frameIndexF2 %= numFramesFall2;
                    frameRecFall2.x = (float) frameWidthFall2 * frameIndexF2;
                    
                    ++frameIndexBat;
                    frameIndexBat %= numFramesBat;
                    frameBat.x = (float)frameWidthBat * frameIndexBat;
                    
                    ++frameIndexMush;
                    frameIndexMush %= numFramesMush;
                    frameMush.x = (float)frameWidthMush * frameIndexMush;
                    
                }
                
            } else { //Se hitar o obstaculo
                grounded = true;
                if (IsKeyDown(KEY_RIGHT)) {
                    
                    scrollingSky -= 0.1f;
                    scrollingBack -= 0.5f;
                    scrollingFront -= 1.0f;
                    scrollingBush -= 1.5f;
                    
                    characterVelocity.x = characterSpeed;
                    if(frameRecAndando.width < 0) {
                        frameRecAndando.width = -frameRecAndando.width;
                        frameRecParado.width = -frameRecParado.width;
                        frameRecFall.width = -frameRecFall.width;
                        frameRecAndando2.width = -frameRecAndando2.width;
                        frameRecParado2.width = -frameRecParado2.width;
                        frameRecFall2.width = -frameRecFall2.width;
                    }
                } else if (IsKeyDown(KEY_LEFT)) {
                    
                    scrollingSky += 0.1f;
                    scrollingBack += 0.5f;
                    scrollingFront+= 1.0f;
                    scrollingBush += 1.5f;
                    
                    characterVelocity.x = -characterSpeed;
                    if(frameRecAndando.width > 0) {
                        frameRecAndando.width = -frameRecAndando.width;
                        frameRecParado.width = -frameRecParado.width;
                        frameRecFall.width = -frameRecFall.width;
                        frameRecAndando2.width = -frameRecAndando2.width;
                        frameRecParado2.width = -frameRecParado2.width;
                        frameRecFall2.width = -frameRecFall2.width;
                    }
                } else {
                    characterVelocity.x = 0;
                }
                bool characterMoving = characterVelocity.x != 0.0f || characterVelocity.y != 0.0f;
                
                characterPosition = Vector2Add(characterPosition, characterVelocity);
                
                ++frameDelayCounter;
                if(frameDelayCounter > frameDelay) {
                    frameDelayCounter = 0;
                    
                    if(characterMoving) {
                        ++frameIndex;
                        frameIndex %= numFramesAndando;
                        frameRecAndando.x = (float) frameWidthAndando * frameIndex;
                        frameIndex %= numFramesAndando2;
                        frameRecAndando2.x = (float) frameWidthAndando2 * frameIndex;
                        
                        ++frameIndexBat;
                        frameIndexBat %= numFramesBat;
                        frameBat.x = (float)frameWidthBat * frameIndexBat;
                        
                        ++frameIndexMush;
                        frameIndexMush %= numFramesMush;
                        frameMush.x = (float)frameWidthMush * frameIndexMush;
                    } else {
                        ++frameIndex;
                        frameIndex %= numFramesParado;
                        frameRecParado.x = (float) frameWidthParado * frameIndex;

                        ++frameIndex;
                        frameIndex %= numFramesParado2;
                        frameRecParado2.x = (float) frameWidthParado2 * frameIndex;
                        
                        ++frameIndexBat;
                        frameIndexBat %= numFramesBat;
                        frameBat.x = (float)frameWidthBat * frameIndexBat;
                        
                        ++frameIndexMush;
                        frameIndexMush %= numFramesMush;
                        frameMush.x = (float)frameWidthMush * frameIndexMush;
                    }
                }
                
            }
        }
        
        // Call update camera function by its pointer
        cameraUpdaters[cameraOption](&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight);
        

        // Draw
        //----------------------------------------------------------------------------------
      
      BeginDrawing();
            //background animated for menu
            
            DrawTextureEx(sky, (Vector2){ scrollingSkyMenu, 0 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(sky, (Vector2){ sky.width*2 + scrollingSkyMenu, 0 }, 0.0f, 2.0f, WHITE);
            
            DrawTextureEx(mountainBack, (Vector2){ scrollingBackMenu, 10 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(mountainBack, (Vector2){ mountainBack.width*2 + scrollingBackMenu, 10 }, 0.0f, 2.0f, WHITE);
            
            DrawTextureEx(mountainFront, (Vector2){ scrollingFrontMenu, 130 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(mountainFront, (Vector2){ mountainFront.width*2 + scrollingFrontMenu, 130 }, 0.0f, 2.0f, WHITE);
            
            DrawTextureEx(bushes, (Vector2){ scrollingBushMenu, 130 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(bushes, (Vector2){ bushes.width*2 + scrollingBushMenu, 130 }, 0.0f, 2.0f, WHITE);
            
            if(!btnAction) {
                DrawTexture(title, 200, 20, WHITE);
                DrawTextureRec(button, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE);
                DrawTextureRec(controlButton, sourceRec, (Vector2){ btnControlBounds.x, btnControlBounds.y }, WHITE);
                
                if(btnActionControl){
                    DrawTexture(controls, 250, 150, WHITE); 
                    DrawTextureRec(back, sourceRecBack, (Vector2){ btnBackBounds.x, btnBackBounds.y }, WHITE);
                }
            } 
            if(btnAction && aux == 1) {
                DrawTexture(title, 200, 20, WHITE);
                DrawText("Press C for character 1 of V for character 2", 180, 200, 20, BLACK);
                
                Vector2 playerPos = { player.position.x - 180, player.position.y - 117};
                Vector2 playerPos2 = { player.position.x - 30, player.position.y - 97 };
                
                DrawTextureRec(playerTexture1Parado, frameRecParado, playerPos, WHITE);
                DrawTextureRec(playerTexture2Parado, frameRecParado2, playerPos2, WHITE);
                
                
                if(IsKeyPressed(KEY_C)) {
                    escolha = 'c';
                    Start = !Start;
                    auxChoiceCharacter++;
                } else if(IsKeyPressed(KEY_V)) {
                    escolha = 'v';
                    Start = !Start;
                    auxChoiceCharacter++;
                }
            }
            
            if(Start && auxChoiceCharacter == 1){
                // add to guarantee break with previous screen
                aux++;
                //ClearBackground(LIGHTGRAY);
                
                DrawTextureEx(sky, (Vector2){ scrollingSky, 0 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(sky, (Vector2){ sky.width*2 + scrollingSky, 0 }, 0.0f, 2.0f, WHITE);
                
                DrawTextureEx(mountainBack, (Vector2){ scrollingBack, 10 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(mountainBack, (Vector2){ mountainBack.width*2 + scrollingBack, 10 }, 0.0f, 2.0f, WHITE);
                
                DrawTextureEx(mountainFront, (Vector2){ scrollingFront, 130 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(mountainFront, (Vector2){ mountainFront.width*2 + scrollingFront, 130 }, 0.0f, 2.0f, WHITE);
                
                DrawTextureEx(bushes, (Vector2){ scrollingBush, 130 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(bushes, (Vector2){ bushes.width*2 + scrollingBush, 130 }, 0.0f, 2.0f, WHITE);
                
                BeginMode2D(camera);

                    for (int i = 0; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);
                    
                    //objects 
                    DrawTexture(TextureTree, 550, 330, WHITE);
                    DrawTexture(TexturePlate, 50, 550, WHITE);
                    DrawTexture(TextureRock, 750, 470, WHITE);
                    
                    //lake
                    DrawTexture(TextureLake, 965, 600, WHITE);
                    
                    //ground
                    DrawTexture(TexturePlataformG, 0, 600, WHITE);
                    DrawTexture(TextureFloor, 300, 490, WHITE);
                    DrawTexture(TextureFloor2, 1380, 490, WHITE);
                    DrawTexture(TextureFloor3, 2600, 490, WHITE);
                    DrawTexture(TextureFloor3, 3300, 490, WHITE);
                    
                    //plataforms
                    DrawTexture(TexturePlataform, 1050, 400, WHITE);
                    DrawTexture(TexturePlataform, 1200, 300, WHITE);
                    DrawTexture(TexturePlataform2, 2050, 400, WHITE);
                    DrawTexture(TexturePlataform2, 2300, 400, WHITE);
                    
                    //bat
                    Vector2 batPos = { bat1.position.x, bat1.position.y };
                    DrawTextureRec(TextureBat, frameBat, batPos, WHITE);
                    
                    //mushroom
                    Vector2 mushPos = { mush1.position.x, mush1.position.y };
                    DrawTextureRec(TextureMush, frameMush, mushPos, WHITE);
                    
                    Vector2 playerPos = { player.position.x - 20, player.position.y - 40 };

                    if(grounded) {//--------------Show frame if grounded--------------------------------------
                        if((IsKeyDown(KEY_LEFT) ^ IsKeyDown(KEY_RIGHT)) && escolha == 'c') {
                            DrawTextureRec(playerTexture1Andando, frameRecAndando, playerPos, WHITE);
                        } else if(((!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) || (IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_RIGHT))) && escolha == 'c') {
                            DrawTextureRec(playerTexture1Parado, frameRecParado, playerPos, WHITE);
                        } else if((IsKeyDown(KEY_LEFT) ^ IsKeyDown(KEY_RIGHT)) && escolha == 'v') {
                            DrawTextureRec(playerTexture2Andando, frameRecAndando2, playerPos, WHITE);
                        } else if(((!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) || (IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_RIGHT))) && escolha == 'v'){
                            DrawTextureRec(playerTexture2Parado, frameRecParado2, playerPos, WHITE);
                        }
                    }
                    
                    if(!grounded) {//--------------show frame if not grounded------------------------
                        if(escolha == 'c') {
                            DrawTextureRec(playerTexture1Fall, frameRecFall, playerPos, WHITE);
                        }
                        if(escolha == 'v') {
                            DrawTextureRec(playerTexture2Fall, frameRecFall2, playerPos, WHITE);
                        }
                    }


                EndMode2D();
                
                if(IsKeyPressed(KEY_Q)) {
                    Pause = !Pause;
                }
            }
            
            if(Pause){
                //DrawRectangle(0, 0, screenWidth, screenHeight, LIGHTGRAY);
                ClearBackground(LIGHTGRAY);
                DrawText("PAUSED", 550, 300, 40, RED);
                DrawText("PRESS Q FOR BACK TO GAME", 520, 350, 15, GRAY);
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(sky);  // Unload background texture
    UnloadTexture(mountainBack);   
    UnloadTexture(mountainFront);  
    UnloadTexture(bushes);  
    
    UnloadTexture(button);  // Unload button texture
    UnloadSound(fxButton);  // Unload sound

    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta)
{
    if (IsKeyDown(KEY_LEFT)) player->position.x -= PLAYER_HOR_SPD*delta;
    if (IsKeyDown(KEY_RIGHT)) player->position.x += PLAYER_HOR_SPD*delta;
    if (IsKeyDown(KEY_SPACE) && player->canJump)
    {
        player->speed = -PLAYER_JUMP_SPD;
        player->canJump = false;
    }

    int hitObstacle = 0;
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        Vector2 *p = &(player->position);
        if (ei->blocking &&
            ei->rect.x <= p->x &&
            ei->rect.x + ei->rect.width >= p->x &&
            ei->rect.y >= p->y &&
            ei->rect.y <= p->y + player->speed*delta)
        {
            hitObstacle = 1;
            player->speed = 0.0f;
            p->y = ei->rect.y;
        }
    }

    if (!hitObstacle)
    {
        player->position.y += player-> speed * delta;
        player->speed += G * delta;
        player->canJump = false;
    }
    else player->canJump = true;
}

void UpdateCameraCenter(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    camera->target = player->position;
}
