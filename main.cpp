#include "raylib.h"
#include "raymath.h"

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f

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

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);
void UpdateCameraCenter(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraEvenOutOnLanding(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraPlayerBoundsPush(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
bool isTextureValid(const Texture2D *texture);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    bool Start = false;
    char escolha;
    bool grounded = true;
    //bool Pause = false;

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    const int characterSpeed = 10;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");

    const char *andando1 = "assets/character1andando.png";
	Texture2D playerTexture1Andando = LoadTexture(andando1);

    const char *parado1 = "assets/character1parado.png";
	Texture2D playerTexture1Parado = LoadTexture(parado1);

    const char *andando2 = "assets/character2andando.png";
	Texture2D playerTexture2Andando = LoadTexture(andando2);

    const char *parado2 = "assets/character2parado.png";
	Texture2D playerTexture2Parado = LoadTexture(parado2);

    const char *fall = "assets/Fall.png";
	Texture2D playerTexture1Fall = LoadTexture(fall);

    const char *fall2 = "assets/Fall2.png";
	Texture2D playerTexture2Fall = LoadTexture(fall2);
    

    if(!isTextureValid(&playerTexture1Andando) || !isTextureValid(&playerTexture1Parado) || !isTextureValid(&playerTexture2Andando) || !isTextureValid(&playerTexture2Parado)) {
		
		while (!WindowShouldClose()) {
			BeginDrawing();
				ClearBackground(RAYWHITE);
				DrawText(TextFormat("ERROR: Couldn't load %s.", andando1), 20, 0, 20, BLACK);
                DrawText(TextFormat("ERROR: Couldn't load %s.", parado1), 20, 50, 20, BLACK);
                DrawText(TextFormat("ERROR: Couldn't load %s.", andando2), 20, 100, 20, BLACK);
                DrawText(TextFormat("ERROR: Couldn't load %s.", parado2), 20, 150, 20, BLACK);
			EndDrawing();
		}
		return 10;
	}
    
    Image image = LoadImage("assets/sky.png");     // Loaded in CPU memory (RAM)
    Texture2D texture = LoadTextureFromImage(image);          // Image converted to texture, GPU memory (VRAM)
    UnloadImage(image);

    Player player = { 0 };
    player.position = (Vector2){ 400, 280 };
    player.speed = 0;
    player.canJump = false;
    EnvItem envItems[] = {
        {{ -100, 400, 1000, 200 }, 1, GREEN },
        {{ 300, 200, 400, 10 }, 1, GREEN },
        {{ 250, 300, 100, 10 }, 1, GREEN },
        {{ 650, 300, 100, 10 }, 1, GREEN }
    };

    int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);

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

    // -----------------------config dos frames parado---------------------------------------

    unsigned numFramesParado = 10; //quantidade de sprites na imagem
	int frameWidthParado = playerTexture1Parado.width / numFramesParado;
	Rectangle frameRecParado = { 0.2f, 0.2f, (float)frameWidthParado, (float)playerTexture1Parado.height };

    unsigned numFramesParado2 = 8; //quantidade de sprites na imagem
	int frameWidthParado2 = playerTexture2Parado.width / numFramesParado2;
	Rectangle frameRecParado2 = { 0.2f, 0.2f, (float)frameWidthParado2, (float)playerTexture2Parado.height };

    
    //---------------------------config dos frames pulando-----------------------------------

    unsigned numFramesFall = 2; //quantidade de sprites na imagem
	int frameWidthFall = playerTexture1Fall.width / numFramesFall;
	Rectangle frameRecFall = { 0.2f, 0.2f, (float)frameWidthFall, (float)playerTexture1Fall.height };

    unsigned numFramesFall2 = 2; //quantidade de sprites na imagem
	int frameWidthFall2 = playerTexture2Fall.width / numFramesFall2;
	Rectangle frameRecFall2 = { 0.2f, 0.2f, (float)frameWidthFall2, (float)playerTexture2Fall.height };


    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
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

        //------atualização de frames para o character----------------------------------------

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
                }



            } else { //Se hitar o obstaculo
                grounded = true;
                if (IsKeyDown(KEY_RIGHT)) {
                    
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
                    } else {
                        ++frameIndex;
                        frameIndex %= numFramesParado;
                        frameRecParado.x = (float) frameWidthParado * frameIndex;

                        ++frameIndex;
                        frameIndex %= numFramesParado2;
                        frameRecParado2.x = (float) frameWidthParado2 * frameIndex;
                    }
                }
            }
        }

        


        //cameraUpdaters[cameraOption](&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            if(!Start){
                DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);
                DrawText("NOME DO JOGO", 250, 50, 40, YELLOW);
                DrawText("Press C for character 1 of V for character 2", 180, 150, 20, BLACK);
                
                
                Vector2 playerPos = { player.position.x - 200, player.position.y - 100 };
                Vector2 playerPos2 = { player.position.x - (-35), player.position.y - 127 };
                
                DrawTextureRec(playerTexture1Parado, frameRecParado, playerPos, WHITE);
                DrawTextureRec(playerTexture2Parado, frameRecParado2, playerPos2, WHITE);
                
                if(IsKeyPressed(KEY_C)) {
                    escolha = 'c';
                    Start = !Start;
                } else if(IsKeyPressed(KEY_V)) {
                    escolha = 'v';
                    Start = !Start;
                }
            }
            
            

            if(Start) {
                ClearBackground(LIGHTGRAY);
            
                DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);

                BeginMode2D(camera);

                    for (int i = 0; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);
                
                    Vector2 playerPos = { player.position.x - 20, player.position.y - 40 };
                    double currentTime = 0;
                    if(grounded) {
                        currentTime = 0;
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
                    
                    if(!grounded) {
                        if(escolha == 'c') {
                            DrawTextureRec(playerTexture1Fall, frameRecFall, playerPos, WHITE);
                        }
                        if(escolha == 'v') {
                            DrawTextureRec(playerTexture2Fall, frameRecFall2, playerPos, WHITE);
                        }
                    }
                    

                EndMode2D();

                DrawText("Controls:", 20, 20, 10, BLACK);
                DrawText("- Right/Left to move", 40, 40, 10, DARKGRAY);
                DrawText("- Space to jump", 40, 60, 10, DARKGRAY);
                DrawText("- Mouse Wheel to Zoom in-out, R to reset zoom", 40, 80, 10, DARKGRAY);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


bool isTextureValid(const Texture2D *texture) {
	return texture->id > 0;
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
        player->position.y += player->speed*delta;
        player->speed += G*delta;
        player->canJump = false;
    }
    else player->canJump = true;
}

void UpdateCameraCenter(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    camera->target = player->position;
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
}

void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    static float minSpeed = 30;
    static float minEffectLength = 10;
    static float fractionSpeed = 0.8f;

    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    Vector2 diff = Vector2Subtract(player->position, camera->target);
    float length = Vector2Length(diff);

    if (length > minEffectLength)
    {
        float speed = fmaxf(fractionSpeed*length, minSpeed);
        camera->target = Vector2Add(camera->target, Vector2Scale(diff, speed*delta/length));
    }
}

void UpdateCameraEvenOutOnLanding(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    static float evenOutSpeed = 700;
    static int eveningOut = false;
    static float evenOutTarget;

    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    camera->target.x = player->position.x;

    if (eveningOut)
    {
        if (evenOutTarget > camera->target.y)
        {
            camera->target.y += evenOutSpeed*delta;

            if (camera->target.y > evenOutTarget)
            {
                camera->target.y = evenOutTarget;
                eveningOut = 0;
            }
        }
        else
        {
            camera->target.y -= evenOutSpeed*delta;

            if (camera->target.y < evenOutTarget)
            {
                camera->target.y = evenOutTarget;
                eveningOut = 0;
            }
        }
    }
    else
    {
        if (player->canJump && (player->speed == 0) && (player->position.y != camera->target.y))
        {
            eveningOut = 1;
            evenOutTarget = player->position.y;
        }
    }
}

void UpdateCameraPlayerBoundsPush(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    static Vector2 bbox = { 0.2f, 0.2f };

    Vector2 bboxWorldMin = GetScreenToWorld2D((Vector2){ (1 - bbox.x)*0.5f*width, (1 - bbox.y)*0.5f*height }, *camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D((Vector2){ (1 + bbox.x)*0.5f*width, (1 + bbox.y)*0.5f*height }, *camera);
    camera->offset = (Vector2){ (1 - bbox.x)*0.5f * width, (1 - bbox.y)*0.5f*height };

    if (player->position.x < bboxWorldMin.x) camera->target.x = player->position.x;
    if (player->position.y < bboxWorldMin.y) camera->target.y = player->position.y;
    if (player->position.x > bboxWorldMax.x) camera->target.x = bboxWorldMin.x + (player->position.x - bboxWorldMax.x);
    if (player->position.y > bboxWorldMax.y) camera->target.y = bboxWorldMin.y + (player->position.y - bboxWorldMax.y);
}
