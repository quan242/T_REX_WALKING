#include <iostream>
#include <fstream>
#include <SDL_image.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <vector>
using namespace std;

SDL_Window* gWindow;
SDL_Renderer* gRenderer;
SDL_Texture* gTexture;
TTF_Font *gFont = NULL;

SDL_Color WHITE_COLOR = {255, 255, 255};
SDL_Color RED_COLOR = {255, 0, 0};

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;
const int MAX_AIR_HEIGHT = 299;
const int MIN_AIR_HEIGHT = 399;
int score = 0;

//The music that will be played
Mix_Music *gMusic = NULL;
Mix_Music *gPlay = NULL;
//The sound effects that will be used

Mix_Chunk *gJump = NULL;
Mix_Chunk *gLose = NULL;
Mix_Chunk *gMouse_click = NULL;
class LTexture
{
    public:
        //Initializes variables
        LTexture();
        //Deallocates memory
        ~LTexture();
        //Loads image at specified path
        bool loadFromFile(string path);
        //Deallocates texture
        void free();

        void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
        //#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		bool loadFromRenderedText(string textureText, SDL_Color textColor );
		//#endif

        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        //Gets image dimensions
        int getWidth();
        int getHeight();
    private:
        //The actual hardware texture
        SDL_Texture* mTexture;
        //Image dimensions
        int mWidth;
        int mHeight;
};

LTexture gBackgroundTexture;
LTexture gScoreTexture;
LTexture gBackgroundPause;
LTexture gmenu;
LTexture high_score;
LTexture ginstruction;
LTexture play_again;
// hien thi diem luc choi game va sau khi thua
LTexture SCORE_IN_GAME;
SDL_Rect _SCORE = {0,0, 100, 100};

LTexture gSpriteSheetTexture[5];
SDL_Rect gSpriteClips[5][7];
SDL_Rect BLANK;


// diem trong high_score
vector <int> _high_score;
LTexture HIGH_SCORE_0;
LTexture HIGH_SCORE_1;
LTexture HIGH_SCORE_2;
LTexture HIGH_SCORE_3;
LTexture HIGH_SCORE_4;
LTexture HIGH_SCORE_5;
LTexture HIGH_SCORE_6;
LTexture HIGH_SCORE_7;
LTexture HIGH_SCORE_8;
LTexture HIGH_SCORE_9;

LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}
LTexture::~LTexture()
{
    //Deallocate
    free();
}
bool LTexture::loadFromFile(string path )
{
    //Get rid of preexisting texture
    free();
    SDL_Texture* newTexture = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}
bool LTexture::loadFromRenderedText(string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}

	//Return success
	return mTexture != NULL;
}
void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}
void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(gRenderer, mTexture, clip,&renderQuad, angle, center, flip);
}
int LTexture::getWidth()
{
    return mWidth;
}
int LTexture::getHeight()
{
    return mHeight;
}
struct nhan_vat// bao gom nguoi choi va chuong ngai vat
{
    int index;
    int x;
    int y;
    int speed_x;
    int speed_y;
    int frame;
    SDL_Rect current_clip;
    int status;// chi so danh rieng cho player
    nhan_vat(int _i = 0,int _x = SCREEN_WIDTH, int _y = 470, int sp_x = 20, int sp_y = 0, int _f = 0, SDL_Rect tmp = BLANK, int st = 0)
    {
        index = _i;
        x = _x;
        y = _y;
        speed_x = sp_x;
        speed_y = sp_y;
        frame = _f;
        current_clip = tmp;
        status = st;
    }
    void animation(int n)
    {
        if(status == 0)
        {
            if(frame >= n-1)
            frame = 0;
            current_clip = gSpriteClips[index][frame];
            gSpriteSheetTexture[index].render(x, y, &current_clip);
            SDL_RenderPresent(gRenderer);
            frame++;
        }
        else// danh rieng cho player
        {
            frame = 6;
            current_clip = gSpriteClips[index][frame];
            gSpriteSheetTexture[index].render(x, y, &current_clip);
            SDL_RenderPresent(gRenderer);
        }
    }
    void auto_move(int &score)// ham danh rieng cho chuong ngai vat
    {
        animation(7);
       if(x <= -100)
        x = SCREEN_WIDTH;
        x -= speed_x;
        speed_x += score/500;
    }
};
bool init()
{
    bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialisze! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if(!( IMG_Init( imgFlags ) & imgFlags ))
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
                if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}

            }
        }
	}
	return success;
}
bool loadMedia()
{
    bool success = true;
    //Load sprite sheet texture
    gFont = TTF_OpenFont( "times.ttf", 40);
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
    if(!gSpriteSheetTexture[0].loadFromFile("dino.png"))
    {
         printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        for(int i = 0; i < 5; i++)
        {
            gSpriteClips[0][i].x = i*100;
            gSpriteClips[0][i].y = 0;
            gSpriteClips[0][i].w = 80;
            gSpriteClips[0][i].h = 100;
        }
        gSpriteClips[0][ 6 ].x = 500;
        gSpriteClips[0][ 6 ].y =   0;
        gSpriteClips[0][ 6 ].w = 115;
        gSpriteClips[0][ 6 ].h = 70;

        //Khung hinh khoi tao cua tat ca cac struct nhan vat
        BLANK.x = 620;
        BLANK.y = 0;
        BLANK.h = 100;
        BLANK.w = 100;

    }
    if( !gSpriteSheetTexture[1].loadFromFile( "bat.png" ) )
    {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        gSpriteClips[1][ 0 ].x = 200;
        gSpriteClips[1][ 0 ].y =   0;
        gSpriteClips[1][ 0 ].w = 100;
        gSpriteClips[1][ 0 ].h = 100;

        gSpriteClips[1][ 1 ].x = 100;
        gSpriteClips[1][ 1 ].y =   0;
        gSpriteClips[1][ 1 ].w = 100;
        gSpriteClips[1][ 1 ].h = 100;

        gSpriteClips[1][ 2 ].x = 0;
        gSpriteClips[1][ 2 ].y = 0;
        gSpriteClips[1][ 2 ].w = 100;
        gSpriteClips[1][ 2 ].h = 100;

        gSpriteClips[1][ 3 ].x = 200;
        gSpriteClips[1][ 3 ].y = 100;
        gSpriteClips[1][ 3 ].w = 100;
        gSpriteClips[1][ 3 ].h = 100;

        gSpriteClips[1][ 4 ].x = 100;
        gSpriteClips[1][ 4 ].y = 100;
        gSpriteClips[1][ 4 ].w = 100;
        gSpriteClips[1][ 4 ].h = 100;

        gSpriteClips[1][ 5 ].x = 0;
        gSpriteClips[1][ 5 ].y = 100;
        gSpriteClips[1][ 5 ].w = 100;
        gSpriteClips[1][ 5 ].h = 100;

        gSpriteClips[1][ 6 ].x = 200;
        gSpriteClips[1][ 6 ].y = 0;
        gSpriteClips[1][ 6 ].w = 100;
        gSpriteClips[1][ 6 ].h = 100;
    }
    if( !gSpriteSheetTexture[2].loadFromFile( "cartus.png" ) )
    {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        for(int i = 0; i < 7; i++)
        {
            gSpriteClips[2][i].x = 0;
            gSpriteClips[2][ i ].y = 0;
            gSpriteClips[2][ i ].w = 85;
            gSpriteClips[2][ i ].h = 100;
        }
    }

    if( !gSpriteSheetTexture[3].loadFromFile( "angry_cartus.png" ) )
    {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        for(int i = 0; i < 7; i++)
        {
            gSpriteClips[3][ i ].x = 0;
            gSpriteClips[3][ i ].y = 0;
            gSpriteClips[3][ i ].w = 90;
            gSpriteClips[3][ i ].h = 130;
        }
    }
    if( !gSpriteSheetTexture[4].loadFromFile( "some_cartus.png" ) )
    {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        for(int i = 0; i < 7; i++)
        {
            gSpriteClips[4][i].x = 0;
            gSpriteClips[4][i].y = 0;
            gSpriteClips[4][i].w = 145;
            gSpriteClips[4][i].h = 90;
        }
    }
    if( !gBackgroundTexture.loadFromFile( "night.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }
    if( !gmenu.loadFromFile( "menu.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }
    if( !ginstruction.loadFromFile( "mouse_instruction.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }
    if( !play_again.loadFromFile( "play_again.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }
    if( !high_score.loadFromFile( "high.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }
    if(!gBackgroundPause.loadFromFile("background_pause.jpg"))
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }
     //Load music
    gPlay = Mix_LoadMUS( "sound_bkgr_audio.wav" );
	if( gPlay == NULL )
	{
		printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gMusic = Mix_LoadMUS( "sound_menu_audio.wav" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	//Load sound effects
	gJump = Mix_LoadWAV( "sound_jump_sound.wav" );
	if( gJump == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gLose = Mix_LoadWAV( "sound_lose_sound.wav" );
	if( gLose == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gMouse_click = Mix_LoadWAV( "sound_mouse_click.wav" );
	if( gMouse_click == NULL )
	{
		printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
    return success;
}
void close()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    for(int i = 0; i < 5; i++)
    {
        gSpriteSheetTexture[i].free();
    }
    gBackgroundTexture.free();
    gBackgroundPause.free();
    gmenu.free();
    ginstruction.free();
    play_again.free();
    high_score.free();
    SCORE_IN_GAME.free();
    //Free the sound effects
	Mix_FreeChunk( gJump );
	Mix_FreeChunk( gLose );
	Mix_FreeMusic( gPlay );
	Mix_FreeChunk( gMouse_click );
	gJump = NULL;
	gLose = NULL;
	gPlay = NULL;
	gMouse_click = NULL;
	//Free the music
	Mix_FreeMusic( gMusic );
	gMusic = NULL;
    IMG_Quit();
    SDL_Quit();
}
void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true)
    {
        if ( SDL_WaitEvent(&e) != 0)
        return;
        SDL_Delay(0);
    }
}
void load_high_score(string path)
{
    ifstream file(path);
    while(!file.eof())
    {
        int x;
        file>>x;
        _high_score.push_back(x);
    }
}
void update_high_score(string path)
{
    int i = 9;
    while(score > _high_score[i] && i >= 0)
    {
        i--;
    }
    if(i < 9)
    {
        _high_score.insert(_high_score.begin() +i + 1, score);
        _high_score.erase(_high_score.begin() + 9);
    }

}
string trans(int k)
{
    string s = "";
    if(k == 0) s = s + "0";
    else
    {
        while(k > 0)
        {
            int r = k%10;
            s = (char)(r+48) + s;
            k = k/10;
        }
    }
    while(s.length() < 4) s = " " + s;
    return s;
}
bool check_collision(nhan_vat &a, nhan_vat &b)
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    //Calculate the sides of rect A
    leftA = a.x + 10;
    rightA = a.x + a.current_clip.w - 10;
    topA = a.y;
    bottomA = a.y + a.current_clip.h;
    //Calculate the sides of rect B
    leftB = b.x + 10;
    rightB = b.x + b.current_clip.w -10;
    topB = b.y + 10;
    bottomB = b.y + b.current_clip.h;
    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }
    if( topA >= bottomB )
    {
        return false;
    }
    if( rightA <= leftB )
    {
        return false;
    }
    if( leftA >= rightB )
    {
        return false;
    }
      Mix_PlayChannel( -1, gLose, 0 );
    //If none of the sides from A are outside B
    return true;
}
void rng(nhan_vat &k,nhan_vat enemy[])
{
    int random = rand()%4;
    if(k.x <= -100)
    {
        if(random == 0)
        {
            k.y = MIN_AIR_HEIGHT;
        }
        k.x = SCREEN_WIDTH + rand()%401 + 300;
    }
    if(k.x >= SCREEN_WIDTH)
    {
        k = enemy[random];
        if(random == 0)
        {
            k.y = rand()%(MIN_AIR_HEIGHT - MAX_AIR_HEIGHT) + 1 + MAX_AIR_HEIGHT;
        }
    }
    k.auto_move(score);
}

bool mouse_play( int x,int y )
{
    if( x >560 && x<680 && y>300 && y<350 )
    {
        return true;
    }
    return false;
}
bool mouse_exit(int x,int y)
{
    if( x > 420 && x<550 && y > 450 && y<500 )
    {
        return true;
    }
    return false;
}
bool mouse_instruction(int x,int y)
{
    if( x >685 && x<810 && y>455 && y<505 )
    {
        return true;
    }
    return false;
}
bool mouse_back(int x,int y)
{
    if( x >0 && x<145 && y>0 && y<70 )
    {
        return true;
    }
    return false;
}
bool mouse_pause(int x,int y)
{
    if( x>935 && x < 965 && y>10 && y<55 )
    {
        return true;
    }
    return false;
}
bool mouse_again_exit(int x,int y)
{
    if( x>675 && x < 915 && y> 455 && y< 560 )
    {
        return true;
    }
    return false;
}
bool mouse_again(int x,int y)
{
    if( x>355 && x < 570 && y>450 && y<560 )
    {
        return true;
    }
    return false;
}
bool mouse_high(int x,int y)
{
    if( x > 560 && x < 680 && y > 385 && y < 435 )
    {
        return true;
    }
    return false;
}
void render_menu( bool &quit, bool &is_instruction,
                  bool &is_menu,bool &is_play,
                  bool &high_sc,SDL_Event &e, int &mou_x,int &mou_y  )
{
    Mix_PlayMusic(gMusic, -1);
    while( quit==false && ( is_instruction == true || is_menu== true || is_play == true || high_sc == true )  )
    {
        if( is_menu== true )
        {
            gmenu.render(0,0,NULL);
            SDL_RenderPresent(gRenderer);
        }
        if( is_instruction==true)
        {
            ginstruction.render(0,0,NULL);
            SDL_RenderPresent(gRenderer);
        }
        if( high_sc == true )
        {
            high_score.render(0,0,NULL);
            HIGH_SCORE_0.loadFromRenderedText(trans(_high_score[0]), RED_COLOR);
            HIGH_SCORE_0.render(855, 170,NULL);
            HIGH_SCORE_1.loadFromRenderedText(trans(_high_score[1]), RED_COLOR);
            HIGH_SCORE_1.render(855, 205,NULL);
            HIGH_SCORE_2.loadFromRenderedText(trans(_high_score[2]), RED_COLOR);
            HIGH_SCORE_2.render(855, 240, NULL);
            HIGH_SCORE_3.loadFromRenderedText(trans(_high_score[3]), RED_COLOR);
            HIGH_SCORE_3.render(855, 270,NULL);
            HIGH_SCORE_4.loadFromRenderedText(trans(_high_score[4]), RED_COLOR);
            HIGH_SCORE_4.render(855, 305,NULL);
            HIGH_SCORE_5.loadFromRenderedText(trans(_high_score[5]), RED_COLOR);
            HIGH_SCORE_5.render(855, 340,NULL);
            HIGH_SCORE_6.loadFromRenderedText(trans(_high_score[6]), RED_COLOR);
            HIGH_SCORE_6.render(855, 370,NULL);
            HIGH_SCORE_7.loadFromRenderedText(trans(_high_score[7]), RED_COLOR);
            HIGH_SCORE_7.render(855, 405,NULL);
            HIGH_SCORE_8.loadFromRenderedText(trans(_high_score[8]), RED_COLOR);
            HIGH_SCORE_8.render(855, 444, NULL);
            HIGH_SCORE_9.loadFromRenderedText(trans(_high_score[9]), RED_COLOR);
            HIGH_SCORE_9.render(855, 473,NULL);
            SDL_RenderPresent(gRenderer);
            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                 SDL_GetMouseState( &mou_x,&mou_y );
                 if(e.button.button == SDL_BUTTON_LEFT)
                 {
                     cout<<mou_x<<" "<<mou_y<<endl;
                 }
            }
        }
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type ==SDL_QUIT   )
            {
                quit=true;
            }
            if( e.type == SDL_MOUSEBUTTONDOWN  )
            {
                SDL_GetMouseState( &mou_x,&mou_y );
                if(e.button.button == SDL_BUTTON_LEFT && mouse_play(mou_x,mou_y)==true)
                {
                    Mix_PlayChannel(-1, gMouse_click, 0);
                    is_play=true;
                    quit = true;
                    is_menu = false;
                    is_instruction = false;
                    high_sc = false;
                }
                if( e.button.button == SDL_BUTTON_LEFT && mouse_instruction(mou_x,mou_y)==true )
                {
                    Mix_PlayChannel(-1, gMouse_click, 0);
                    is_instruction = true;
                    is_menu = false;
                    is_play = false;
                    high_sc = false;
                }
                if( e.button.button == SDL_BUTTON_LEFT && mouse_exit(mou_x,mou_y)==true )
                {
                    Mix_PlayChannel(-1, gMouse_click, 0);
                    quit=true;
                    is_play = false;
                }
                if( e.button.button == SDL_BUTTON_LEFT && mouse_back(mou_x,mou_y) ==true && is_instruction==true )
                {
                    Mix_PlayChannel(-1, gMouse_click, 0);
                    is_instruction=false;
                    is_menu= true;
                    is_play=false;
                    high_sc = false;
                }
                if( e.button.button == SDL_BUTTON_LEFT && mouse_high(mou_x,mou_y) ==true  )
                {
                    Mix_PlayChannel(-1, gMouse_click, 0);
                    is_instruction=false;
                    is_menu= false;
                    is_play=false;
                    high_sc = true;
                }
                if( e.button.button == SDL_BUTTON_LEFT && mouse_back(mou_x,mou_y) ==true && high_sc==true )
                {
                    Mix_PlayChannel(-1, gMouse_click, 0);
                    is_instruction=false;
                    is_menu= true;
                    is_play=false;
                    high_sc = false;
                }
            }
        }
    }
    Mix_PauseMusic();
}
void play_game(bool &quit,bool &is_play,SDL_Event &e,
                int &mou_x,int &mou_y ,bool pause, nhan_vat &player,
                nhan_vat &k,nhan_vat enemy[],
                bool &is_menu,bool &is_instruction,bool &high_sc)
{
    player.x = 200;
    player.y = 470;
    score = 0;
    Mix_PlayMusic(gPlay, -1);
    while(quit == false )
    {
        int h=0,t=0;
        while( is_play==true  )
        {
            while( SDL_PollEvent( &e ) != 0 )
            {
            //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                    //is_play = false;
                }
                if( e.type == SDL_MOUSEBUTTONDOWN  )
                {
                    SDL_GetMouseState( &mou_x,&mou_y );
                    if(e.button.button == SDL_BUTTON_LEFT && mouse_pause(mou_x,mou_y)==true)
                    {
                        if( pause == true )
                        {
                            pause = false;
                        }
                        else
                        {
                            pause = true;
                        }
                    }
                }
                if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
                {
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_SPACE:
                        {
                            if( pause == true )
                            {
                                pause = false;
                            }
                            else
                            {
                                pause = true;
                            }
                            break;
                        }
                        default:
                        break;
                    }
                }
                if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
                {
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_UP:
                        {
                            Mix_PlayChannel(-1, gJump, 0);
                            if(player.status == 1)
                            {
                                player.y -= 30;
                            }
                            player.status = 0;
                            is_play = !check_collision(player, k);
                            player.speed_y = 49;
                            while(player.speed_y > 0 && is_play == true)
                            {
                                is_play = !check_collision(player, k);
                                if( pause == false )
                                {
                                    Mix_ResumeMusic();
                                    player.y -= player.speed_y;
                                    player.speed_y -= 7;
                                    gBackgroundTexture.render(0,0,NULL);
                                    score++;
                                    gScoreTexture.loadFromRenderedText(trans(score), WHITE_COLOR);
                                    gScoreTexture.render(1100, 10);
                                    rng(k,enemy);
                                    player.animation(6);
                                    SDL_Delay(50);
                                    // cout<<player.x<<" "<<player.y<<endl;
                                }
                                else if( pause == true)
                                {
                                    Mix_PauseMusic();
                                    gBackgroundPause.render(0,0,NULL);
                                     gScoreTexture.loadFromRenderedText(trans(score), WHITE_COLOR);
                                    gScoreTexture.render(1100, 10);
                                    gSpriteSheetTexture[player.index].render(player.x, player.y, &player.current_clip);
                                    gSpriteSheetTexture[k.index].render(k.x, k.y, &k.current_clip);
                                    SDL_RenderPresent(gRenderer);
                                }
                            }
                            while(player.y < 470 && is_play == true)
                            {
                                is_play = !check_collision(player, k);
                                if( pause == false )
                                {
                                    Mix_ResumeMusic();
                                    player.speed_y += 7;
                                    player.y += player.speed_y;
                                    gBackgroundTexture.render(0,0,NULL);
                                    score++;
                                    gScoreTexture.loadFromRenderedText(trans(score), WHITE_COLOR);
                                    gScoreTexture.render(1100, 10);
                                    rng(k,enemy);
                                    player.animation(6);
                                    SDL_Delay(50);
                                    // cout<<player.x<<" "<<player.y<<endl;
                                }
                                else if( pause == true )
                                {
                                    Mix_PauseMusic();
                                    gBackgroundPause.render(0,0,NULL);
                                    gScoreTexture.loadFromRenderedText(trans(score), WHITE_COLOR);
                                    gScoreTexture.render(1100, 10);
                                    gSpriteSheetTexture[player.index].render(player.x, player.y, &player.current_clip);
                                    gSpriteSheetTexture[k.index].render(k.x, k.y, &k.current_clip);
                                    SDL_RenderPresent(gRenderer);
                                }
                            }
                            break;
                        }
                        case SDLK_DOWN:
                        {
                            if(is_play == true && pause == false)
                            {
                                player.status = 1;
                                player.y += 30;
                                is_play = !check_collision(player, k);
                            }
                            break;
                        }
                    }
                }
                if(e.type == SDL_KEYUP && e.key.repeat == 0)
                {
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_DOWN:
                        {
                            if(is_play == true && pause == false)
                            {
                                player.status = 0;
                                player.y -= 30;
                                is_play = !check_collision(player, k);
                            }
                            break;
                        }
                        default:
                        break;
                    }
                }
            }
            if(is_play == false) break;
            if( pause == false)
            {
                Mix_ResumeMusic();
                //cout<<player.x<<" "<<player.y<<endl;
                gBackgroundTexture.render(0,0,NULL);
                score++;
                gScoreTexture.loadFromRenderedText(trans(score), WHITE_COLOR);
                gScoreTexture.render(1100, 10);
                rng(k,enemy);
                player.animation(6);
                SDL_Delay(50);
            }
            else if( pause== true )
            {
                Mix_PauseMusic();
                gBackgroundPause.render(0,0,NULL);
                gScoreTexture.loadFromRenderedText(trans(score), WHITE_COLOR);
                gScoreTexture.render(1100, 10);
                gSpriteSheetTexture[player.index].render(player.x, player.y, &player.current_clip);
                gSpriteSheetTexture[k.index].render(k.x, k.y, &k.current_clip);
                SDL_RenderPresent(gRenderer);

            }
            is_play = !check_collision(player, k);
        // conndition game over
            /*if( ( ( k.x >= player.x && k.x <( player.x+100 ) ) ||( k.x <=player.x && ( k.x+100 ) > player.x ) ) && ( player.y <= k.y && player.y+100 >= k.y ) )
            {
                is_play=false;
            }*/
        }
        Mix_PauseMusic();
        update_high_score("high_score.txt");
        SDL_Delay(1000);
        while(is_play==false && quit==false )
        {
            play_again.render(0,0,NULL);
            gScoreTexture.loadFromRenderedText(trans(score), RED_COLOR);
            gScoreTexture.render(795, 340);
            SDL_RenderPresent(gRenderer);
            while( SDL_PollEvent( &e ) != 0 )
            {

                if(e.type == SDL_MOUSEBUTTONDOWN)
                {
                    SDL_GetMouseState( &mou_x,&mou_y );
                    if(e.button.button == SDL_BUTTON_LEFT && mouse_again_exit(mou_x,mou_y)==true)
                    {
                        Mix_PlayChannel(-1, gMouse_click, 0);
                        quit = true;
                        Mix_PauseMusic();
                    }
                    if(e.button.button == SDL_BUTTON_LEFT && mouse_again(mou_x,mou_y)==true)
                    {
                        Mix_PlayChannel(-1, gMouse_click, 0);
                        is_play = true;
                        quit = false;
                        // set up lai vi tri cho cnv va diem so
                        k.x=1200;
                        player.x = 200;
                        player.y = 470;
                        score = 0;
                    }
                }
            }
        }
    }
     Mix_PauseMusic();
}
int main(int argc, char* argv[])
{
    score = 0;
    srand(time(NULL));
    init();
    loadMedia();
    load_high_score("high_score.txt");
    SDL_Event e;
    nhan_vat player(0, 200, 470,20, 20,0);
    nhan_vat no_2(1, SCREEN_WIDTH, (SCREEN_HEIGHT-100)/2 + 50, 20,0, 0);
    nhan_vat enemy[4];
    enemy[0] = no_2;
    for(int i = 1; i < 4; i++)
    {
        enemy[i].index = i+1;
    }
    enemy[1].y = 570 - 100;
    enemy[2].y = 570 - 130;
    enemy[3].y = 570 - 90;
    nhan_vat k;
    bool quit = false;
    bool is_play = false;
    bool is_instruction = false;
    bool is_menu=true;
    int mou_x,mou_y;
    bool pause= false;
    bool high_sc=false;
    while( true )
    {
        render_menu( quit,is_instruction,is_menu,is_play,high_sc, e,mou_x,mou_y );
        if( is_play == false && quit == true )
        {
            break;
        }
        Mix_PauseMusic();
        if( is_play == true && quit == true )
        {
            quit = false;
            play_game(quit,is_play,e,mou_x,mou_y,pause,player,k,enemy,is_menu,is_instruction,high_sc);
            Mix_PauseMusic();
            is_instruction = false;
            is_menu = true;
            is_play= false;
            high_sc = false;
            quit = false;
            k.x = 1200;
            player.x = 470;
            player.y = 200;
            score = 0;
        }
    }
    waitUntilKeyPressed();
    close();
    ofstream file("high_score.txt");
    for(int i = 0; i < 10; i++)
    {
        file<<_high_score[i]<<endl;
    }
    file.close();
    return 0;
}
