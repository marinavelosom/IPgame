#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

//#include "include/updatePlayer.h"
#include "include/structPlayer.h"
#include "include/structEnvItem.h"
#include "include/structBat.h"
#include "include/structMashroom.h"

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f
#define NUM_FRAMES  2
#define PLAYER_WIDTH 30
#define PLAYER_HEIGHT 60
#define BAT_HOR_SPD 10.0f

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);
void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
bool ButtonsMenu(Vector2 mousePoint, bool btnAction, Rectangle btnBounds, Sound fxButton);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    bool Start = false;
    bool Pause = false;
    bool GameOver = false;
    char escolha;
    bool grounded = true;
    bool IsAtk = false;
    bool leftAtk = false;
    bool credits = false;
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    const int characterSpeed = 10;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");
    InitAudioDevice();
    
    Music music = LoadMusicStream("audio/music.wav"); 
    PlayMusicStream(music);
    
    Sound atkSound = LoadSound("audio/attack.wav");
    Sound fallSound = LoadSound("audio/FallScream.mp3");
    Sound collsionSound = LoadSound("audio/collision.mp3");
    Sound gameOverSound = LoadSound("audio/GameOver.wav");
    Sound lakeFallSound = LoadSound("audio/lakesplash.mp3");
    Sound soundCredits = LoadSound("audio/credits.mp3"); 
    
    const char *andando1 = "assets/Run 1.png";
	Texture2D playerTexture1Andando = LoadTexture(andando1);

    const char *parado1 = "assets/Idle 1.png";
	Texture2D playerTexture1Parado = LoadTexture(parado1);

    const char *andando2 = "assets/Run 2.png";
	Texture2D playerTexture2Andando = LoadTexture(andando2);

    const char *parado2 = "assets/Idle 2.png";
	Texture2D playerTexture2Parado = LoadTexture(parado2);

    const char *fall = "assets/Fall 1.png";
	Texture2D playerTexture1Fall = LoadTexture(fall);

    const char *fall2 = "assets/Fall 2.png";
	Texture2D playerTexture2Fall = LoadTexture(fall2);
    
    const char *attack1 = "assets/Attack1.png";
	Texture2D playerTexture1Attack = LoadTexture(attack1);

    const char *attack2 = "assets/Attack2.png";
	Texture2D playerTexture2Attack = LoadTexture(attack2);

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
    Rectangle btnBackBounds = { 330, 388, (float)back.width, frameHeight  };
    
    
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
    //----------------------personagem------------------------
    
    Player player = { 0 };
    player.position = (Vector2){ 400, 280 };
    player.speed = 0;
    player.canJump = false;
    
    //-------------- Config. Map ------------------------
    
    Color transparent = (Color){0, 0, 0, 0};
    
    EnvItem envItems[] = {
      //{{ X, Y, W, H }},
        {{ 0, 350, 5, 300 }, 1, transparent}, //90 graus
        {{ 960, 490, 5, 150 }, 1, transparent}, //90 graus lake 1
        {{ 1380, 500, 30, 150 }, 1, transparent}, //90 graus lake 2
        {{ 0, 490, 960, 140 }, 1, transparent }, //floor 1
        {{ 1380, 490, 580, 140 }, 1, transparent }, //floor 2
        {{ 2600, 490, 580, 140 }, 1, transparent }, //floor 3
        {{ 1050, 400, 100, 10 }, 1, transparent }, 
        {{ 1200, 300, 100, 10 }, 1, transparent },
        {{ 100, 90, 100, 10 }, 1, transparent },
        {{ 965, 640, 415, 5 }, 1, transparent }, //lake 1
        {{ 2050, 400, 165, 10 }, 1, transparent },
        {{ 2300, 400, 165, 10 }, 1, transparent },
        {{ 3300, 490, 580, 140 }, 1, transparent }, //floor 4
        {{ 3980, 490, 580, 140 }, 1, transparent }, //floor 5
    };

    int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);
    
    //-------------- Textures map ------------------------
    const char *floor = "assets/floor1Test.png";
    Texture2D TextureFloor = LoadTexture(floor);
    
    const char *floor2 = "assets/floor2.png";
    Texture2D TextureFloor2 = LoadTexture(floor2);
    
    const char *floor3 = "assets/floor3.png";
    Texture2D TextureFloor3 = LoadTexture(floor3);
    
    const char *floor4 = "assets/floor4.png";
    Texture2D TextureFloor4 = LoadTexture(floor4);
    
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
    
    const char *tree1 = "assets/tree3.png";
    Texture2D TextureTree1 = LoadTexture(tree1);
    
    const char *tree2 = "assets/tree4.png";
    Texture2D TextureTree2 = LoadTexture(tree2);
    
    const char *plate = "assets/plate1.png";
    Texture2D TexturePlate = LoadTexture(plate);
    
    const char *rock = "assets/rock1.png";
    Texture2D TextureRock = LoadTexture(rock);
    // ------------ Life ------------------------
    const char *heart = "assets/heart.png";
    Texture2D TexturePlayerheart = LoadTexture(heart);
    
    //-------------- Camera ------------------------
    
    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    //-------------- Sound effects ------------------------
    Sound soundFall = LoadSound("audio/fall.mp3"); 
    
    // ----------------Config dos frames andando -----------------------------------------

    unsigned numFramesAndando = 4; //quantidade de sprites na imagem
	int frameWidthAndando = playerTexture1Andando.width / numFramesAndando;
	Rectangle frameRecAndando = { 0.2f, 0.2f, (float)frameWidthAndando/2, (float)playerTexture1Andando.height };
    Vector2 characterPosition = {screenWidth / 1.5f, screenHeight / 1.5f};
    Vector2 characterVelocity = {0.0f,0.0f};
    
    unsigned numFramesAndando2 = 8; //quantidade de sprites na imagem
	int frameWidthAndando2 = playerTexture2Andando.width / numFramesAndando2;
	Rectangle frameRecAndando2 = { 0.2f, 0.2f, (float)frameWidthAndando2, (float)playerTexture2Andando.height };
    
    unsigned frameDelay = 7;
	unsigned frameDelayCounter = 0;
    unsigned frameDelayCounter2 = 0;
    unsigned frameIndex = 0;
    unsigned frameIndexF = 0;
    unsigned frameIndexF2 = 0;
    unsigned frameIndexBat = 0;
    unsigned frameIndexBat2 = 0;
    unsigned frameIndexMush = 0;
    unsigned frameIndexAtk1 = 0;
    unsigned frameIndexAtk2 = 0;
    unsigned frameIndexPortal = 0;
    unsigned frameIndexBoss = 0;
    
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
    
    //------------------------config dos frames ataque------------------------------------------

    	unsigned numFramesAttack2 = 6; //quantidade de sprites na imagem
	int frameWidthAttack2 = playerTexture2Attack.width / numFramesAttack2;
	Rectangle frameRecAttack2 = { 0.2f, 0.2f, (float)frameWidthAttack2, (float)playerTexture2Attack.height };

    	unsigned numFramesAttack1 = 5; //quantidade de sprites na imagem
	int frameWidthAttack1 = playerTexture1Attack.width / numFramesAttack1;
	Rectangle frameRecAttack1 = { 0.2f, 0.2f, (float)frameWidthAttack1, (float)playerTexture1Attack.height };

     //--------------------------- Config camera--------------------------------------------------

    // Store pointers to the multiple update camera functions
    void (*cameraUpdaters[])(Camera2D*, Player*, EnvItem*, int, float, int, int) = {
        UpdateCameraCenterInsideMap,
    };

    int cameraOption = 0;
    int cameraUpdatersLength = sizeof(cameraUpdaters)/sizeof(cameraUpdaters[0]);

    SetTargetFPS(60);
    
    int auxChoiceCharacter = 0, auxControl = 0, aux = 0;
    
    //===================== Confg. bat ======================================================
    
    const char *bat = "assets/batSprite.png";
    Texture2D TextureBat = LoadTexture(bat);
    
    unsigned numFramesBat = 4;
    int frameWidthBat = TextureBat.width / numFramesBat;
    Rectangle frameBat = { 0.2f, 0.2f, (float)frameWidthBat, (float)TextureBat.height };
    Rectangle frameBat2 = { 0.2f, 0.2f, (float)frameWidthBat, (float)TextureBat.height };


    Bat bat1 = { 0 };
    Bat bat2 = { 0 };
    bat1.position = (Vector2){ 1700, 400 };
    bat2.position = (Vector2){ 2900, 400 };
    
    //===================== Confg. mushroom ======================================================
    
    const char *mush = "assets/mushroomRun.png";
    Texture2D TextureMush = LoadTexture(mush);
    
    unsigned numFramesMush = 8;
    int frameWidthMush = TextureMush.width / numFramesMush;
    Rectangle frameMush = { 0.2f, 0.2f, (float)frameWidthMush, (float)TextureMush.height };
    
    frameMush.width = -frameMush.width; 

    Mush mush1 = { 0 };
    mush1.position = (Vector2){ 3400, 230 };
    
    //===================== Confg. portal ======================================================
    
    const char *portal = "assets/PurplePortal.png";
    Texture2D TexturePortal = LoadTexture(portal);
    
    unsigned numFramesPortal = 8;
    int frameWidthPortal = TexturePortal.width / numFramesPortal;
    Rectangle framePortal = { 0.2f, 0.2f, (float)frameWidthPortal, (float)TexturePortal.height };
    
    Mush portal1 = { 0 };
    portal1.position = (Vector2){ 4465, 350 };
    
    //===================== Confg. BOSS ======================================================
    
    const char *boss = "assets/NightBorneRun.png";
    Texture2D TextureBoss = LoadTexture(boss);
    
    unsigned numFramesBoss = 6;
    int frameWidthBoss = TextureBoss.width / numFramesBoss;
    Rectangle frameBoss = { 0.2f, 0.2f, (float)frameWidthBoss, (float)TextureBoss.height };
    
    frameBoss.width = -frameBoss.width; 

    Mush boss1 = { 0 };
    boss1.position = (Vector2){ 4100, 320 };

    //===================== Confg. arrow ======================================================
    // const char *arrow = "assets/arrow.png";
    // Texture TextureArrow = LoadTexture(arrow);

    // unsigned numFramesArrow = ;
    // ------------------------------
    int life = 0;
    int hitBat1 = 0, hitBat2 = 0, hitMush = 0, hitBoss = 0;
    bool BhitMush = false;
    bool BhitBoss = false;
    int cont1Dist = 0,  cont2Dist = 0, cont3Dist = 0, cont4Dist = 0;
    int  arrowAtk = 0, contArrow = 0;
    // Main game loop ======================================================================================= 
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
        
        bool collisionWallLeft = false; 
        bool collisionWallRight = false; 
        bool invertPosition = false;

        float arrowPositionX = player.position.x + 50;
        float arrowPositionY = player.position.y - 70;

        if(Start) {
            //------------------------------------config hitWall--------------------------------------------------------
            Rectangle rec1 = { 0, 350, 5, 900 }; // wall begin
            Rectangle rec2 = { 970, 490, 5, 150 }; //wall 1 lake
            Rectangle rec3 = { 1380, 500, 30, 150 };//wall 2 lake
            Rectangle rec4 = {4560, 350, 5, 900};
            Rectangle playerRect = { player.position.x - 10, player.position.y - 60, 40, 60 }; // player
            Rectangle monsterRect = { bat1.position.x + 15, bat1.position.y + 15, 40, 30  }; // Bat 1
            Rectangle monster1Rect = { bat2.position.x + 15, bat2.position.y + 15, 40, 30  }; // Bat 2
            Rectangle monster2Rect = { mush1.position.x + 170, mush1.position.y + 170, 100, 150  }; // Mushroom
            Rectangle monster3Rect = { boss1.position.x + 60, boss1.position.y + 30, 150, 150  }; // BOSS
            
            UpdatePlayer(&player, envItems, envItemsLength, deltaTime);

            // ================= Bat 2 walk =================================================
            
            if(cont1Dist < 200) {
                bat1.position.x -= 1.0f;
                cont1Dist++;
                if(cont1Dist == 200) {
                    cont1Dist = cont1Dist * 2;
                    frameBat.width = -frameBat.width;
                }
            }
            
            if(cont1Dist > 200) {
                bat1.position.x += 1.0f;
                cont1Dist--;
                if(cont1Dist == 200) {
                    cont1Dist = 0;
                    frameBat.width = -frameBat.width;
                }
            }

            // ================= Bat 2 walk =================================================
            
            if(cont2Dist < 200) {
                bat2.position.x -= 1.0f;
                cont2Dist++;
                if(cont2Dist == 200) {
                    cont2Dist = cont2Dist * 2;
                    frameBat2.width = -frameBat2.width;
                }
            }
            
            if(cont2Dist > 200) {
                bat2.position.x += 1.0f;
                cont2Dist--;
                if(cont2Dist == 200) {
                    cont2Dist = 0;
                    frameBat2.width = -frameBat2.width;
                }
            }
            
            // ================= Mushroom Walk =================================================  

            if(cont3Dist < 200) {
                mush1.position.x -= 1.0f;
                cont3Dist++;
                if(cont3Dist == 200) {
                    cont3Dist = cont3Dist * 2;
                    TextureMush.width = -TextureMush.width;
                }
            }
            
            if(cont3Dist > 200) {
                mush1.position.x += 1.0f;
                cont3Dist--;
                if(cont3Dist == 200) {
                    cont3Dist = 0;
                    TextureMush.width = -TextureMush.width;
                }
            }        

            // ================================ BOSS Walk ========================================


            if(cont4Dist < 200) {
                boss1.position.x -= 1.0f;
                cont4Dist++;
                if(cont4Dist == 200) {
                    cont4Dist = cont4Dist * 2;
                    TextureBoss.width = -TextureBoss.width;
                }
            }
            
            if(cont4Dist > 200) {
                boss1.position.x += 1.0f;
                cont4Dist--;
                if(cont4Dist == 200) {
                    cont4Dist = 0;
                    TextureBoss.width = -TextureBoss.width;
                }

            } 

            // ================= Attack ====================================================          
            
            if(IsKeyPressed(KEY_K)) {
                PlaySound(atkSound);
                IsAtk = true;
            } 

            if(IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_SPACE)) IsAtk = false;
            if(IsKeyDown(KEY_A)) leftAtk = true;
            if(IsKeyDown(KEY_D)) leftAtk = false;
            
            if(IsAtk == true && !leftAtk) { //Right
                Rectangle recAttack = { player.position.x + 50, player.position.y - 70, 20, 30};

                if (CheckCollisionRecs(recAttack, monsterRect)) bat1.position.y = 4000;
                if (CheckCollisionRecs(recAttack, monster1Rect)) bat2.position.y = 4000;
                
                if(CheckCollisionRecs(recAttack, monster2Rect)) {
                    hitMush++;
                }
                if (hitMush > 50) mush1.position.y = 4000;

                if(CheckCollisionRecs(recAttack, monster3Rect)) {
                    hitBoss++;
                } 
                if (hitBoss > 100) boss1.position.y = 4000;

            } else if(IsAtk == true && leftAtk) {
                Rectangle recAttack = { player.position.x - 60, player.position.y - 70, 20, 30};

                if (CheckCollisionRecs(recAttack, monsterRect)) bat1.position.y = 4000;
                if (CheckCollisionRecs(recAttack, monster1Rect)) bat2.position.y = 4000;
                
                if(CheckCollisionRecs(recAttack, monster2Rect)) {
                    hitMush++;
                }
                if (hitMush > 50) mush1.position.y = 4000;

                if(CheckCollisionRecs(recAttack, monster3Rect)) {
                    hitBoss++;
                } 
                if (hitBoss > 100) boss1.position.y = 4000;
            }
            
            // ================= Arrow attack ============================================= (PENDENTE!!!!!!!!!!!!!!!!!!!!!!!!!)
            
            if(IsAtk == true && escolha == 'v') {
                arrowAtk++;
            }

            if(arrowAtk != 0){
                Rectangle recArrow = { arrowPositionX, arrowPositionY, 20, 30};
                arrowPositionX += 1.0f;
                contArrow++;
                if(contArrow == 120) {
                    contArrow = 0;
                    arrowAtk = 0;
                } 
            }

            // ================= Collision monsters =============================================
            
            if (CheckCollisionRecs(playerRect, monsterRect) && !IsKeyDown(KEY_K) 
                || CheckCollisionRecs(playerRect, monster1Rect)
                || CheckCollisionRecs(playerRect, monster2Rect)
                || CheckCollisionRecs(playerRect, monster3Rect)
            ) {// Left collision wall begin and monsters collison
                PlaySound(collsionSound);
                life++;
                camera.zoom = 1.0f;
                player.position = (Vector2){ 140, 480 };
            }

            // ================= Collisions Wall and Lake =============================================

            if (CheckCollisionRecs(playerRect, rec1)) {// Left collision wall begin
                collisionWallLeft = true;
                if (player.position.x + playerRect.width > rec1.x && player.position.x < rec1.x) {
                    player.position.x = rec1.x - rec1.width;// Right collision wall begin
                    collisionWallRight = true;
                } else if (player.position.x < rec1.x + rec1.width && player.position.x + playerRect.width > rec1.x + rec1.width) {
                    player.position.x = rec1.x + rec1.width;
                    collisionWallRight = true;
                }
            }

            if (CheckCollisionRecs(playerRect, rec2)) {// Left collision wall 1 lake
                collisionWallLeft = true;

                if (player.position.x + playerRect.width > rec2.x && player.position.x < rec2.x) {
                    player.position.x = rec2.x - rec2.width;// Right collision wall 1 lake
                    collisionWallRight = true;
                } else if (player.position.x < rec2.x + rec2.width && player.position.x + playerRect.width > rec2.x + rec2.width) {
                    player.position.x = rec2.x + rec2.width;
                    collisionWallRight = true;
                }
            }

            if (CheckCollisionRecs(playerRect, rec3)) {// Left collision wall 2 lake
                collisionWallLeft = true;
                if (player.position.x + playerRect.width > rec3.x && player.position.x < rec3.x) {
                    player.position.x = rec3.x - rec3.width;// Right collision wall 2 leke
                    collisionWallRight = true;
                } else if (player.position.x < rec3.x + rec3.width && player.position.x + playerRect.width > rec3.x + rec3.width) {
                    player.position.x = rec3.x + rec3.width;
                    collisionWallRight = true;
                }
            }

            if (CheckCollisionRecs(playerRect, rec4)) {// Left collision wall begin
                collisionWallLeft = true;
                if (player.position.x + playerRect.width > rec4.x && player.position.x < rec4.x) {
                    player.position.x = rec4.x - rec4.width;// Right collision wall begin
                    collisionWallRight = true;
                } else if (player.position.x < rec4.x + rec4.width && player.position.x + playerRect.width > rec4.x + rec4.width) {
                    player.position.x = rec4.x + rec4.width;
                    collisionWallRight = true;
                }
            }
            
            // ================= Return ===================================================

            if (IsKeyPressed(KEY_R))
            {
                camera.zoom = 1.0f;
                player.position = (Vector2){ 140, 480 };
            }

            // ================= Camera =====================================================
            // Call update camera function by its pointer
            
            cameraUpdaters[cameraOption](&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight);
            
            // ================= Credits config. =============================================
            
            if(player.position.x > 4500) {
                StopMusicStream(music);
                PlaySound(soundCredits);
                credits = true; 
            }

            // ================= Fall config. =================================================
            
            if(player.position.x >= 965 && player.position.x <= 1380 && player.position.y > 635) {
                PlaySound(lakeFallSound);
                camera.zoom = 1.0f;
                player.position = (Vector2){ 140, 480 };
                life++;
            }

            if(player.position.y > 750) {
                PlaySound(fallSound);
            }
            
            if(player.position.y > 1050) {
                camera.zoom = 1.0f;
                player.position = (Vector2){ 140, 480 };
                life++;
            }
            
            // ================= Game Over config. =================================================
            
            if(life == 3) {
                StopMusicStream(music);
                StopSound(lakeFallSound);
                PlaySound(gameOverSound);
                life++;
            }
        }
        
        // ================= Menu player position =================================================
        
        if(!Start) {
            if (IsKeyPressed(KEY_C) || IsKeyPressed(KEY_V)) {
                player.position = (Vector2){ 140, 480 };
            }
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

            //-----------------atualizacao dos frames do atak independe se esta grounded--------------------
            
            if(IsAtk == true) {
                ++frameDelayCounter;
                if(frameDelayCounter > frameDelay) {
                    frameDelayCounter = 0;
                    
                    ++frameIndexAtk1;
                    frameIndexAtk1 %= numFramesAttack1;
                    frameRecAttack1.x = (float) frameWidthAttack1 * frameIndexAtk1;

                    ++frameIndexAtk2;
                    frameIndexAtk2 %= numFramesAttack2;
                    frameRecAttack2.x = (float) frameWidthAttack2 * frameIndexAtk2;

                    ++frameIndexBat;
                    frameIndexBat %= numFramesBat;
                    frameBat.x = (float)frameWidthBat * frameIndexBat;

                    ++frameIndexBat2;
                    frameIndexBat2 %= numFramesBat;
                    frameBat2.x = (float)frameWidthBat * frameIndexBat2;
                    
                    ++frameIndexMush;
                    frameIndexMush %= numFramesMush;
                    frameMush.x = (float)frameWidthMush * frameIndexMush;
                    
                    ++frameIndexPortal;
                    frameIndexPortal %= numFramesPortal;
                    framePortal.x = (float)frameWidthPortal * frameIndexPortal;

                    ++frameIndexBoss;
                    frameIndexBoss %= numFramesBoss;
                    frameBoss.x = (float)frameWidthBoss * frameIndexBoss;
                }
            }
            
            //------------------------------------------------------------------------------------------------
            
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

                    ++frameIndexBat2;
                    frameIndexBat2 %= numFramesBat;
                    frameBat2.x = (float)frameWidthBat * frameIndexBat2;
                    
                    ++frameIndexMush;
                    frameIndexMush %= numFramesMush;
                    frameMush.x = (float)frameWidthMush * frameIndexMush;
                    
                    ++frameIndexPortal;
                    frameIndexPortal %= numFramesPortal;
                    framePortal.x = (float)frameWidthPortal * frameIndexPortal;

                    ++frameIndexBoss;
                    frameIndexBoss %= numFramesBoss;
                    frameBoss.x = (float)frameWidthBoss * frameIndexBoss;
                }
                
            } else { //Se hitar o obstaculo
                grounded = true;
                if (IsKeyDown(KEY_D)) {
                    
                    if(collisionWallRight == true){
                        scrollingSky -= 0.0f;
                        scrollingBack -= 0.0f;
                        scrollingFront -= 0.0f;
                        scrollingBush -= 0.0f;
                    } else {
                        scrollingSky -= 0.1f;
                        scrollingBack -= 0.5f;
                        scrollingFront -= 1.0f;
                        scrollingBush -= 1.5f;
                    }
                    
                    characterVelocity.x = characterSpeed;
                    if(frameRecAndando.width < 0) {
                        frameRecAndando.width = -frameRecAndando.width;
                        frameRecParado.width = -frameRecParado.width;
                        frameRecFall.width = -frameRecFall.width;
                        frameRecAndando2.width = -frameRecAndando2.width;
                        frameRecParado2.width = -frameRecParado2.width;
                        frameRecFall2.width = -frameRecFall2.width;
                        frameRecAttack1.width = -frameRecAttack1.width;
                        frameRecAttack2.width = -frameRecAttack2.width;
                    }
                } else if (IsKeyDown(KEY_A)) {
                    
                    if(collisionWallLeft == true){
                        scrollingSky += 0.0f;
                        scrollingBack += 0.0f;
                        scrollingFront += 0.0f;
                        scrollingBush += 0.0f;
                    } else {
                        scrollingSky += 0.1f;
                        scrollingBack += 0.5f;
                        scrollingFront+= 1.0f;
                        scrollingBush += 1.5f;
                    }
                    
                    characterVelocity.x = -characterSpeed;
                    if(frameRecAndando.width > 0) {
                        frameRecAndando.width = -frameRecAndando.width;
                        frameRecParado.width = -frameRecParado.width;
                        frameRecFall.width = -frameRecFall.width;
                        frameRecAndando2.width = -frameRecAndando2.width;
                        frameRecParado2.width = -frameRecParado2.width;
                        frameRecFall2.width = -frameRecFall2.width;
                        frameRecAttack1.width = -frameRecAttack1.width;
                        frameRecAttack2.width = -frameRecAttack2.width;
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
                        
                        ++frameIndexBat2;
                        frameIndexBat2 %= numFramesBat;
                        frameBat2.x = (float)frameWidthBat * frameIndexBat2;
                    
                        ++frameIndexMush;
                        frameIndexMush %= numFramesMush;
                        frameMush.x = (float)frameWidthMush * frameIndexMush;
                        
                        ++frameIndexPortal;
                        frameIndexPortal %= numFramesPortal;
                        framePortal.x = (float)frameWidthPortal * frameIndexPortal;
                        
                        ++frameIndexBoss;
                        frameIndexBoss %= numFramesBoss;
                        frameBoss.x = (float)frameWidthBoss * frameIndexBoss;
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
                        
                        ++frameIndexBat2;
                        frameIndexBat2 %= numFramesBat;
                        frameBat2.x = (float)frameWidthBat * frameIndexBat2;

                        ++frameIndexMush;
                        frameIndexMush %= numFramesMush;
                        frameMush.x = (float)frameWidthMush * frameIndexMush;
                        
                        ++frameIndexPortal;
                        frameIndexPortal %= numFramesPortal;
                        framePortal.x = (float)frameWidthPortal * frameIndexPortal;

                        ++frameIndexBoss;
                        frameIndexBoss %= numFramesBoss;
                        frameBoss.x = (float)frameWidthBoss * frameIndexBoss;
                    }
                }
            }
        }
        

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
                
                Vector2 playerPos = { player.position.x - 200, player.position.y - 125 };
                Vector2 playerPos2 = { player.position.x , player.position.y - 108 };
                
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
                    
                    // Objects 
                    DrawTexture(TextureTree, 550, 330, WHITE);
                    DrawTexture(TextureTree1, 1550, 350, WHITE);
                    DrawTexture(TextureTree2, 2610, 350, WHITE);
                    DrawTexture(TextureTree2, 3300, 350, WHITE);
                    
                    DrawTexture(TexturePlate, 50, 550, WHITE);
                    DrawTexture(TextureRock, 750, 470, WHITE);
                    
                    //Plataforms
                    DrawTexture(TexturePlataform, 1050, 400, WHITE);
                    DrawTexture(TexturePlataform, 1200, 300, WHITE);
                    DrawTexture(TexturePlataform2, 2050, 400, WHITE);
                    DrawTexture(TexturePlataform2, 2300, 400, WHITE);
                    
                    // Bat=============================================================
                    Vector2 batPos = { bat1.position.x, bat1.position.y };
                    DrawTextureRec(TextureBat, frameBat, batPos, WHITE);
                    
                    // Bat2 =============================================================
                    Vector2 bat2Pos = { bat2.position.x, bat2.position.y };
                    DrawTextureRec(TextureBat, frameBat2, bat2Pos, WHITE);

                    // Mushroom =============================================================
                    Vector2 mushPos = { mush1.position.x, mush1.position.y };
                    DrawTextureRec(TextureMush, frameMush, mushPos, WHITE);
                    
                    // BOSS =============================================================
                    Rectangle monster3Rect = { boss1.position.x + 60, boss1.position.y + 30, 150, 150  };
                    Vector2 bossPos = { boss1.position.x, boss1.position.y };
                    DrawTextureRec(TextureBoss, frameBoss, bossPos, WHITE);
                    DrawRectangleRec(monster3Rect, BLACK);
                    
                    // Portal =============================================================
                    Vector2 portalPos = { portal1.position.x, portal1.position.y };
                    DrawTextureRec(TexturePortal, framePortal, portalPos, WHITE);
                    
                    Vector2 playerPos1 = { player.position.x - 100, player.position.y - 138 };
                    Vector2 playerPos2 = { player.position.x - 85, player.position.y - 115 };

                    if(grounded) {//--------------Show frame if grounded--------------------------------------
                    
                        // Rec Arrow Attack Draw =============================================================

                        // if(arrowAtk != 0) {
                        //     Rectangle recArrow = { arrowPositionX, arrowPositionY, 20, 30};
                        //     DrawRectangleRec(recArrow, BLACK);
                        // } 

                        // Rec Attack Draw ===================================================================
                        // if(IsAtk == true && escolha == 'c' && !leftAtk) {
                        //     Rectangle recAttack = { player.position.x + 50, player.position.y - 70, 20, 30};
                        //     DrawRectangleRec(recAttack, RED);  
                        // }

                        // if(IsAtk == true && escolha == 'c'&& leftAtk) {
                        //     Rectangle recAttack = { player.position.x - 60, player.position.y - 70, 20, 30};
                        //     DrawRectangleRec(recAttack, BLUE);  
                        // }

                        if(IsAtk == true && escolha == 'c') {
                            DrawTextureRec(playerTexture1Attack, frameRecAttack1, playerPos1, WHITE);

                        } else if(IsAtk == true && escolha == 'v') {
                            DrawTextureRec(playerTexture2Attack, frameRecAttack2, playerPos2, WHITE);
                        } else {
                            if((IsKeyDown(KEY_A) ^ IsKeyDown(KEY_D)) && escolha == 'c') {
                                DrawTextureRec(playerTexture1Andando, frameRecAndando, playerPos1, WHITE);
                            } else if(((!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) || (IsKeyDown(KEY_A) && IsKeyDown(KEY_D))) && escolha == 'c') {
                                DrawTextureRec(playerTexture1Parado, frameRecParado, playerPos1, WHITE);
                            } else if((IsKeyDown(KEY_A) ^ IsKeyDown(KEY_D)) && escolha == 'v') {
                                DrawTextureRec(playerTexture2Andando, frameRecAndando2, playerPos2, WHITE);
                            } else if(((!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) || (IsKeyDown(KEY_A) && IsKeyDown(KEY_D))) && escolha == 'v'){
                                DrawTextureRec(playerTexture2Parado, frameRecParado2, playerPos2, WHITE);
                            }
                        }
                    }
                    
                    if(!grounded) {//--------------show frame if not grounded------------------------
                        if(escolha == 'c') {
                            DrawTextureRec(playerTexture1Fall, frameRecFall, playerPos1, WHITE);
                        }
                        if(escolha == 'v') {
                            DrawTextureRec(playerTexture2Fall, frameRecFall2, playerPos1, WHITE);
                        }
                    }

                    // Draw Lake
                    DrawTexture(TextureLake, 965, 600, WHITE);
                    
                    // Draw Floor
                    DrawTexture(TextureFloor, 0, 490, WHITE);
                    DrawTexture(TextureFloor2, 1380, 490, WHITE);
                    DrawTexture(TextureFloor3, 2600, 490, WHITE);
                    DrawTexture(TextureFloor3, 3300, 490, WHITE);
                    DrawTexture(TextureFloor4, 3980, 490, WHITE);
    
                EndMode2D();
                
                if(life == 0){
                    DrawTexture(TexturePlayerheart, 20, 20, WHITE);
                    DrawTexture(TexturePlayerheart, 60, 20, WHITE);
                    DrawTexture(TexturePlayerheart, 100, 20, WHITE);
                }else if (life == 1){
                    DrawTexture(TexturePlayerheart, 20, 20, WHITE);
                    DrawTexture(TexturePlayerheart, 60, 20, WHITE);
                    DrawTexture(TexturePlayerheart, 100, 20, transparent);
                } else if(life == 2) {
                    DrawTexture(TexturePlayerheart, 20, 20, WHITE);
                    DrawTexture(TexturePlayerheart, 60, 20, transparent);
                    DrawTexture(TexturePlayerheart, 100, 20, transparent);
                } else {
                    ClearBackground(BLACK);
                    DrawText("GAME OVER", 300, 100, 40, YELLOW);
                    DrawText("[ ESC for exit ]", 600, 420, 15, GRAY);
                }
            }

            if(credits == true){
                ClearBackground(BLACK);
                
                DrawText("CREDITS", 300, 100, 40, YELLOW);
                DrawText("GABRIEL SOARES", 280, 200, 25, WHITE);
                DrawText("MARINA MELO", 300, 230, 25, WHITE);
                DrawText("PEDRO PEREIRA", 290, 260, 25, WHITE);
                DrawText("JULIO CÉSAR", 305, 290, 25, WHITE);
                DrawText("[ ESC for exit ]", 600, 420, 15, GRAY);
                
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
//================================================================================================================================================================================
//estou tentando alocar essa função no arquivo em include -> mas ta tendo erro

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta) {
    if (IsKeyDown(KEY_A)) player->position.x -= PLAYER_HOR_SPD*delta;
    if (IsKeyDown(KEY_D)) player->position.x += PLAYER_HOR_SPD*delta;
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

void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    camera->target = player->position;
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        minX = fminf(ei->rect.x, minX);
        maxX = fmaxf(ei->rect.x + ei->rect.width, maxX);
        minY = fminf(ei->rect.y, minY);
        maxY = fmaxf(ei->rect.y + ei->rect.height, maxY);
    }

    Vector2 max = GetWorldToScreen2D((Vector2){ maxX, maxY }, *camera);
    Vector2 min = GetWorldToScreen2D((Vector2){ minX, minY }, *camera);

    if (max.x < width) camera->offset.x = width - (max.x - width/2);
    if (max.y < height) camera->offset.y = height - (max.y - height/2);
    if (min.x > 0) camera->offset.x = width/2 - min.x;
    if (min.y > 0) camera->offset.y = height/2 - min.y;
    camera->offset.y -= (-22);

}
