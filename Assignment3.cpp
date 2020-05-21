// Assignment 3.cpp: A program using the TL - Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;
#include <complex>
using namespace std;
#include<sstream>
using namespace std;
#include <TL-Engine.h>// TL-Engine include file and namespace
using namespace tle;

// Constant floats to determine speed for the game //
float gameSpeed = 1.0f;						// Game speed of the hover car //
const float kSteerSpeed = 0.1f;						// Steer speed of the hover car //

													// EKeyCodes for camera //
const EKeyCode CameraUp = Key_Up;					// EKeyCode for the Key 'Up' to make the camera move up //
const EKeyCode CameraDown = Key_Down;				// EKeyCode for the Key 'Down' to make the camera move down //
const EKeyCode CameraLeft = Key_Left;				// EKeyCode for the Key 'Left' to make the camera move left //
const EKeyCode CameraRight = Key_Right;				// EKeyCode for the Key 'Right' to make the camera move right //

													// EKeyCodes for Movement //
const EKeyCode Forwards = Key_W;					// EKeyCode for the Key 'W' to make the hover car move forwards //
const EKeyCode Left = Key_A;						// EKeyCode for the Key 'A' to make the hover car move left //
const EKeyCode Backwards = Key_S;					// EKeyCode for the Key 'S' to make the hover car move backwards //
const EKeyCode Right = Key_D;						// EKeyCode for the Key 'D' to make the hover car move right //

													// Camera control constants //
const float kCameraMove = gameSpeed * 10.0f;		// Distance for the direction keys x and z axis //
const float kMouseWheelMove = gameSpeed * 10.0f;	// Distance for wheel movement z axis //
const float kMouseRotation = gameSpeed * 0.3f;		// Distance (in degrees) for rotation of the camera //
const float kCameraX = 0.0f;

// Score integers //							
int currentLap = 0.0f;									// The current lap increases by 1 when past over the last checkpoint //
string currentStage = "";							// the current stage which the hover car will enter //

													// Design Feature Integers //
const int kNumIsle = 50.0f;							// The number of isle models that have been used //
const int kNumWall = 93.0f;							// The number of wall models that have been used //
const int kNumCheckP = 3.0f;							// The number of checkpoint models that have been used //
const int kNumWaterT = 35.0f;							// The number of watertank models that have been used //
const int kNumWaterT2 = 11.0f;							// The number of watertank 2 models that have been used //
const int kNumTribune = 5.0f;							// The number of tribune models that have been used //
const int kNumInterstellar = 2.0f;						// The number of interstellar models that have been used //
const int kNumWalkway = 108.0f;						// The number of walkway models that have been used //

													// Radius for models //
const float carRadius = 2.0f;							// The radius of the car //
const float carRadius2 = 2.0f;							// The radius of the car //
const float checkpointRadius = 20.0f;					// The radius of the checkpoint //

														// Enums for game states //
enum GameState { stage1, stage2, stage3 };			// enum states for each stage within the game //
GameState gamestate = { stage1 };					// gamestate stage1 //

enum OtherGameStates { Playing, Finished, PreGame };	// enum states for each start, pre and end point of the game //
OtherGameStates gamestates = { PreGame };			// gamestate pregame //

enum CameraStates { FirstPerson, ThirdPerson };		// enum states for the camera, whether it is first or third person //
CameraStates camerastate = { ThirdPerson };			// gamestate thirdperson //


													// ---------- SphereToBox ---------- //
bool paused = false;

bool spheretobox(IModel* object, IModel* car, float width, float length, float radius, float carx, float carZ)		// SphereToBox collision Detection //
{
	bool collision = false;	// The collision is false //
	float minX = object->GetX() - width - radius;
	float maxX = object->GetX() + width + radius;
	float minZ = object->GetZ() - length - radius;
	float MaxZ = object->GetZ() + length + radius;

	if (carx > minX && carx < maxX && carZ > minZ && carZ < MaxZ)
	{
		collision = true;
		return collision;		// after all has been executed, return the collision and make it true //
	}
	return collision;
}

// ---------- SphereToSphere ---------- //
bool spheretosphere(IModel* object, IModel* car, float carRadius, float carRadius2)			// SpherToSphere collision Dectection //
{
	bool collision = false;	// The collision is false //
	float VectorX, VectorY, VectorZ;
	float distanceX, distanceY, distanceZ;

	VectorX = car->GetX() - object->GetX();
	VectorY = car->GetY() - object->GetY();
	VectorZ = car->GetZ() - object->GetZ();

	distanceX = VectorX * VectorX; //distanceX squared
	distanceY = VectorY * VectorY; //distanceY squared
	distanceZ = VectorZ * VectorZ; //distanceZ squared

	float hoverCarDistance = sqrt(distanceX + distanceY + distanceZ);	// current distance of hover car from water tanks //

	if (hoverCarDistance < (carRadius + carRadius2))
	{
		collision = true;
		return collision;		// after all has been executed, return the collision and make it true //
	}
	return collision;
}


void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("Media");

	/**** Set up your scene here ****/

	IMesh* checkpointMesh;
	IMesh* skyboxMesh;
	IMesh* isleMesh;
	IMesh* wallMesh;
	IMesh* groundMesh;
	IMesh* watertankMesh;						// All the Mesh's used within the game //
	IMesh* watertank2Mesh;
	IMesh* tribuneMesh;
	IMesh* interstellarMesh;
	IMesh* walkwayMesh;
	IMesh* hoverCarMesh;


	IModel* checkpoint[kNumCheckP];
	IModel* skybox;
	IModel* isle[kNumIsle];
	IModel* wall[kNumWall];
	IModel* ground;
	IModel* watertank[kNumWaterT];				// All the Model's used within the game //
	IModel* watertank2[kNumWaterT2];
	IModel* tribune[kNumTribune];
	IModel* interstellar[kNumInterstellar];
	IModel* walkway[kNumWalkway];
	IModel* hoverCar;


	// ---------- Models ---------- //

	groundMesh = myEngine->LoadMesh("ground.x");
	ground = groundMesh->CreateModel(0.0f, 0.0f, 0.0f);

	skyboxMesh = myEngine->LoadMesh("Skybox 07.x");
	skybox = skyboxMesh->CreateModel(0.0f, -960.0f, 0.0f);

	hoverCarMesh = myEngine->LoadMesh("race2.x");
	hoverCar = hoverCarMesh->CreateModel(0.0f, 0.0f, 0.0f);

	// Variables throughout the game //
	float hoverCarSpeed = 0.0f;				// Current driving speed of the hoverCar //
	int currentHoverCar = 0;				// the current hover cars position //
	float SpeedStep = 30.0f;				// Speed step of the hover car //
	float countdown = 4.0f;					// the countdown for the start of the race //
	float go = 2.0f;						// the text will say go and stay for 2 seconds //
	float racetime = 0.0f;

	// ---------- Loading the text and deciding the text size ---------- //

	IFont* myFont;

	myFont = myEngine->LoadFont("Calibri", 72);	// loading the calibri font with the size being 72 //


												// ---------- Checkpoint Placement ---------- //

	checkpointMesh = myEngine->LoadMesh("Checkpoint.x");
	checkpoint[kNumCheckP];

	float xCheckPos[kNumCheckP] = { 0.0f, 186.0f, 129.0f };
	float yCheckPos[kNumCheckP] = { 0.0f };
	float zCheckPos[kNumCheckP] = { 0.0f, 110.0f, -405.0f };

	for (int i = 0; i < kNumCheckP; i++)
	{
		checkpoint[i] = checkpointMesh->CreateModel(xCheckPos[i], yCheckPos[i], zCheckPos[i]);	// Loading the checkpoints into the game stating their starting positions //
	}

	checkpoint[0]->Scale(2.0f);
	checkpoint[1]->Scale(2.0f);
	checkpoint[2]->Scale(3.0f);
	checkpoint[2]->RotateY(90.0f);

	

	// ---------- Isle Placement ---------- //

	isleMesh = myEngine->LoadMesh("IsleStraight.x");
	isle[kNumIsle];

	float xislePos[kNumIsle] = { -10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f, 247.0f, 276.0f, 243.0f, 281.0f, 239.0f, 285.0f, 235.0f, 289.0f, 231.0f, 293.0f, 227.0f, 297.0f, 223.0f, 301.0f, 219.0f, 305.0f, 219.0f, 305.0f, 223.0f, 301.0f, 227.0f, 297.0f, 231.0f, 293.0f, 235.0f, 289.0f, 239.0f, 285.0f, 243.0f, 281.0f, 247.0f, 277.0f, 198.0f, 198.0f, 198.0f, 173.5f, 173.5f, 173.5f };
	float yislePos[kNumIsle] = { 0.0f };
	float zislePos[kNumIsle] = { 0.0f, 0.0f, 8.0f, 8.0f, -8.0f, -8.0f, 146.0f, 146.0f, 151.0f, 151.0f, 156.0f, 156.0f, -65.0f, -65.0f, -70.0f, -70.0f, -75.0f, -75.0f, -80.0f, -80.0f, -85.0f, -85.0f, -90.0f, -90.0f, -95.0f, -95.0f, -100.0f, -100.0f, -140.0f, -140.0f, -145.0f, -145.0f, -150.0f, -150.0f, -155.0f, -155.0f, -160.0f, -160.0f, -165.0f, -165.0f, -170.0f, -170.0f, -175.0f, -175.0f, 110.0f, 102.0f, 118.0f, 110.0f, 102.0f, 118.0f };

	for (int i = 0; i < kNumIsle; i++)
	{
		isle[i] = isleMesh->CreateModel(xislePos[i], yislePos[i], zislePos[i]);	// Loading the islands into the game stating their starting positions //
	}

	// ---------- Wall Placement ---------- //

	wallMesh = myEngine->LoadMesh("Wall.x");
	wall[kNumWall];

	float xWallPos[kNumWall] = { -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , 
								-10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f , -10.0f, -10.0f, -10.0f,  10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 
								10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f , 10.0f, 10.0f };
	float yWallPos[kNumWall] = { 0.0f , 0.0f };
	float zWallPos[kNumWall] = { 134.0f ,126.0f ,118.0f ,110.0f, 102.0f ,94.0f, 86.0f ,78.0f ,70.0f ,62.0f ,54.0f, 46.0f ,38.0f, 30.0f ,22.0f, 14.0f ,6.0f ,-2.0f, -10.0f, -18.0f, -26.0f,-34.0f,-42.0f, -50.0f, -58.0f, -66.0f, -74.0f, -82.0f, -90.0f, -98.0f, -106.0f, -114.0f, -122.0f, -130.0f, -138.0f, -146.0f, -154.0f, -162.0f, -170.0f, -178.0f, -186.0f, -194.0f, -202.0f, -210.0f, -218.0f, -226.0f, -234.0f,  134.0f ,126.0f ,118.0f ,110.0f, 102.0f ,94.0f, 86.0f ,78.0f ,70.0f ,62.0f ,54.0f, 46.0f ,38.0f, 30.0f ,22.0f, 14.0f ,6.0f ,-2.0f, -10.0f, -18.0f, -26.0f,-34.0f,-42.0f, -50.0f, -58.0f, -66.0f, -74.0f, -82.0f, -90.0f, -98.0f, -106.0f, -114.0f, -122.0f, -130.0f, -138.0f, -146.0f, -154.0f, -162.0f, -170.0f, -178.0f, -186.0f, -194.0f, -202.0f, -210.0f, -218.0f, -226.0f };

	for (int i = 0; i < kNumWall; i++)
	{
		wall[i] = wallMesh->CreateModel(xWallPos[i], yWallPos[i], zWallPos[i]);	// Loading the walls into the game stating their starting positions //
	}

	// ---------- Water Tank Placement ---------- //

	watertankMesh = myEngine->LoadMesh("TankSmall1.x");
	watertank[kNumWaterT];

	float xWaterPos[kNumWaterT] = { -10.0f, 10.0f, -10.0f , 219.0f, 305.0f, 248.0f, 277.0f, 168.0f, 197.0f, 88.0f, 57.0f, 10.0f, -10.0f, -5.0f, 5.0f, 15.0f, 25.0f, 45.0f, 65.0f, 85.0f, 105.0f, 125.0f, 145.0f, 165.0f, 175.0f, 185.0f, 195.0f, 65.0f, 85.0f, 105.0f, 125.0f, 30.0f, 40.0f, 150.0f, 140.0f };
	float yWaterPos[kNumWaterT] = { 0.0f };
	float zWaterPos[kNumWaterT] = { 140.0f , 140.0f, 162.0f, -120.0f, -120.0f, -228.0f, -242.0f, -348.0f, -402.0f, -348.0f, -402.0f, -228.0f, -242.0f , 180.0f, 200.0f, 220.0f, 240.0f, 260.0f, 260.0f, 260.0f, 260.0f, 260.0f, 260.0f, 240.0f, 220.0f, 200.0f, 180.0f, 220.0f, 220.0f, 220.0f, 220.0f, 178.0f, 198.0f, 178.0f, 198.0f };

	for (int i = 0; i < kNumWaterT; i++)
	{
		watertank[i] = watertankMesh->CreateModel(xWaterPos[i], yWaterPos[i], zWaterPos[i]);	// Loading the water tanks into the game stating their starting positions //
	}

	watertank[0]->RotateY(180.0f);
	watertank[2]->RotateY(180.0f);
	watertank[5]->RotateY(180.0f);
	watertank[6]->RotateY(180.0f);
	watertank[7]->RotateY(180.0f);

	// ---------- Water Tank 2 Placement ---------- //

	watertank2Mesh = myEngine->LoadMesh("TankSmall2.x");
	watertank2[kNumWaterT2];

	float xWater2Pos[kNumWaterT2] = { 219.0f, 305.0f, 219.0f, 305.0f, 208.0f, 237.0f, 128.0f, 130.0f, 48.0f, 17.0f, 265.0f };
	float yWater2Pos[kNumWaterT2] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -4.0f };
	float zWater2Pos[kNumWaterT2] = { -110.0f, -110.0f, -130.0f, -130.0f, -288.0f, -322.0f, -368.0f, -442.0f, -288.0f, -322.0f, -118.0f };

	for (int i = 0; i < kNumWaterT2; i++)
	{
		watertank2[i] = watertank2Mesh->CreateModel(xWater2Pos[i], yWater2Pos[i], zWater2Pos[i]);	// Loading the water tanks 2 into the game stating their starting positions //
	}


	watertank2[1]->RotateY(180.0f);
	watertank2[3]->RotateY(180.0f);
	watertank2[4]->RotateY(180.0f);
	watertank2[10]->RotateY(90.0f);
	watertank2[10]->RotateZ(25.0f);
	watertank2[10]->Scale(2.0f);

	
	// ---------- Intersteller Placement ---------- //

	interstellarMesh = myEngine->LoadMesh("Interstellar.x");
	interstellar[kNumInterstellar];

	float xInterstellarPos[kNumInterstellar] = { 60.0f, 126.0f };
	float yInterstellarPos[kNumInterstellar] = { 20.0f, 20.0f };
	float zInterstellarPos[kNumInterstellar] = { 110.0f, 110.0f };

	for (int i = 0; i < kNumInterstellar; i++)
	{
		interstellar[i] = interstellarMesh->CreateModel(xInterstellarPos[i], yInterstellarPos[i], zInterstellarPos[i]);	// Loading the interstellars into the game stating their starting positions //
	}

	interstellar[0]->Scale(2.0f);
	interstellar[1]->Scale(2.0f);



	// ---------- Camera ---------- //

	ICamera* myCamera = myEngine->CreateCamera(kManual);
	myCamera->SetPosition(0.0f, 13.0f, -40.0f);						// setting up the position of the camera //					
	myCamera->RotateX(20.0f);										// Rotating the camera on the x axis by 20 //
	myCamera->AttachToParent(hoverCar);								// attaching the camera to the car //

	bool mouseCaptureActive = true;									// state of mouse capture
	myEngine->StartMouseCapture();

	bool chaseCamActive = false;

	int mouseMoveX = 0; // Mouse movement on x axis //
	int mouseMoveY = 0; // Mouse movement on y axis //

	//TIMER CODE --------------------------------------------------------------//

	float timeElapsed = 0.0f;					//total time elapsed
	float collisionTimer = 0.0f;				//timer for bouncing back
	bool isBouncingBack = false;
	float collisionTimeout = 0.25f;				//time to bounce back
	float deltaTime = 0.0f;						//the time difference between frames


	//END: TIMER CODE ---------------------------------------------------------//

	// The main game loop, repeat until engine is stopped //
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		float frametime = myEngine->Timer();	// Frametime for all the mph, timer etc. to increase/decrease //

		//TIMER!!!! ------------------------------------------------//

		deltaTime = myEngine->Timer() * 10000;	//frame time
		if (!paused) 
		{
			timeElapsed += deltaTime;				//overall time
		}
		if (gamestates == Playing)
		{
			myFont->Draw(to_string(timeElapsed), 10, 200, 0xFFFFFFFF);
		}
		//if we are bouncing back from a collision
		if (isBouncingBack)
		{
			//calculate time elapsed since collison timer last reset
			float bounceTimeElapsed = timeElapsed - collisionTimer;

			myFont->Draw(to_string(bounceTimeElapsed), 10, 300, 0xFFFFFFFF);

			if (bounceTimeElapsed > collisionTimeout)
			{
				isBouncingBack = false;			//unset flag
				collisionTimer = timeElapsed;	//reset collision timer
			}
		}

		//END TIMER-------------------------------------------//

		if (gamestates == PreGame)				// Game state pregame //
		{
			stringstream countText;
			countText << (int)countdown;									// Making the text countdown in the middle of the screen preparing the racer to start in blue //
			myFont->Draw(countText.str(), 650, 50, kBlue, kCentre, kTop);	// Setting up the size, colour, positioning of the text //
			countText.str("");

			countdown -= 1 * frametime;
			
			if (countdown <= 1.0f)				// The amount of laps within the game //
			{
				timeElapsed = 0.0f;
				gamestates = Playing;			// Game state playing
			}

		}
		

		if (gamestates == Finished)				// Game State finished //
		{
			stringstream timeText;
			timeText << "Total Racetime: " << racetime;					// Making the text say "Total Ractime:" in the centre of the screen when the race is over in cyan //
			myFont->Draw(timeText.str(), 650, 50, kCyan, kCentre, kTop);	// Setting up the size, colour, positioning of the text //
			timeText.str("");
			currentStage = "   Race Finished!!!";		// Text appears as soon as hover car finishes the race //
		}


		if (paused == true)
		{
			stringstream pausedText;
			pausedText << "Game Paused!";									// Making the text say "Game Paused!" in the centre of the screen when the game is paused in blue //
			myFont->Draw(pausedText.str(), 650, 50, kBlue, kCentre, kTop);	// Setting up the size, colour, positioning of the text //
			pausedText.str("");
		}

		// ---------- Camera control ---------- //

		if (paused == false)
		{

			switch (camerastate)				// Switch state declaring the camera state //
			{
			case ThirdPerson:					// the camera state is in third person //
			{
				myCamera->AttachToParent(hoverCar);											// Attatching the camera to the hover car //
				myCamera->SetLocalPosition(0.0f, 13.0f, -40.0f);							// Position of camera in the third person view //
				if (myEngine->KeyHeld(CameraUp)) myCamera->MoveLocalY(kCameraMove);			// Ability to move the camera up when pressing the up key //
				if (myEngine->KeyHeld(CameraDown)) myCamera->MoveLocalY(-kCameraMove);		// Ability to move the camera down when pressing the down key //
				if (myEngine->KeyHeld(CameraRight)) myCamera->MoveLocalX(kCameraMove);		// Ability to move the camera right when pressing the right key //
				if (myEngine->KeyHeld(CameraLeft)) myCamera->MoveLocalX(-kCameraMove);		// Ability to move the camera left when pressing the left key //

				if (myEngine->KeyHit(Key_2))
				{
					camerastate = FirstPerson;		// When hitting the 2 key, the camera goes to first person view //
				}
				break;
			}

			case FirstPerson:					// the camera state is in first person //
			{
				myCamera->AttachToParent(hoverCar);		// Attaching the camera to the hover car //
				myCamera->SetLocalX(kCameraX);			// Setting the Local X position of the camera //
				myCamera->SetLocalY(6.0f);				// Setting the Local Y position of the camera //
				myCamera->SetLocalZ(0.0f);				// Setting the Local Z position of the camera //
				if (myEngine->KeyHit(Key_1))
				{
					camerastate = ThirdPerson;		//	When hitting the 1 key, the camera will go back to the third person view //
				}
				break;
			}

			}


			// ---------- Collision Detection - SphereToBox ---------- //

			float carXPosition = hoverCar->GetX();
			float carZposition = hoverCar->GetZ();

			for (int i = 0; i < kNumIsle; i++)
			{
				if (spheretobox(isle[i], hoverCar, 2, 4, carRadius, carXPosition, carZposition))	// Setting up the radius for the car colliding with the isles //
				{
					if (!isBouncingBack)
					{
						isBouncingBack = true;		// Making sure that collision detection works when going backwards //
						collisionTimer = timeElapsed;
					}
				}
			}

			for (int i = 0; i < kNumWall; i++)
			{

				if (spheretobox(wall[i], hoverCar, 2, 4, carRadius, carXPosition, carZposition))		// Setting up the radius for the car colliding with the wall //
				{
					if (hoverCarSpeed > 0.0f)
					{
						hoverCar->MoveLocalZ(-5.0f);		// Making sure that collision detection works when going backwards //
					}
					else
					{
						hoverCar->MoveLocalZ(5.0f);
					}
					if (!isBouncingBack)
					{
						isBouncingBack = true;		// Making sure that collision detection works when going backwards //
						collisionTimer = timeElapsed;
					}
				}
			}

			// ---------- Collision Detection - SphereToSphere ---------- //

			for (int i = 0; i < kNumWaterT; i++)
			{
				if (spheretosphere(watertank[i], hoverCar, carRadius, carRadius2))	// Setting up the radius for the car colliding with the watertank //
				{
					if (!isBouncingBack)
					{
						isBouncingBack = true;		// Making sure that collision detection works when going backwards //
						collisionTimer = timeElapsed;
					}
				}
			}

			for (int i = 0; i < kNumWaterT2; i++)
			{
				if (spheretosphere(watertank2[i], hoverCar, carRadius, carRadius2))	// Setting up the radius for the car colliding with the watertank 2 //
				{
					if (!isBouncingBack)
					{
						isBouncingBack = true;		// Making sure that collision detection works when going backwards //
						collisionTimer = timeElapsed;
					}
				}
			}

			//if bouncing back - move hover car accordingly
			if (isBouncingBack)
			{
				if (hoverCarSpeed > 0.0f)
				{
					hoverCar->MoveLocalZ(-5.0f * deltaTime);		// Making sure that collision detection works when going backwards //
					hoverCarSpeed -= deltaTime * 500.0f;
				}
				else
				{
					hoverCar->MoveLocalZ(5.0f * deltaTime);
					if (hoverCarSpeed < -0.1f)
					{
						hoverCarSpeed += deltaTime * 500.0f;
					}

				}
			}


			// ---------- Car Movement ---------- //

			if (gamestates == Playing)				// Game state playing //
			{
				go -= 1 * frametime;
				if (go > 0.0f)
				{
					stringstream GoText;
					GoText << "GO!!!";											// Making the text say "GO!!!" in the centre of the screen for 2 seconds in blue //
					myFont->Draw(GoText.str(), 650, 50, kGreen, kCentre, kTop);	// Setting up the size, colour, positioning of the text //
					GoText.str("");
				}

				stringstream speedText;
				speedText << "" << (int)hoverCarSpeed << "MPH";						// Making the text say "MPH" in the top right corner in yellow //
				myFont->Draw(speedText.str(), 1150, 20, kYellow, kCentre, kTop);	// Setting up the size, colour, positioning of the text //
				speedText.str("");

				racetime += 1 * frametime;

				//if not bouncing back - move stuff
				if (!isBouncingBack)
				{
					hoverCar->MoveLocalZ(hoverCarSpeed*frametime);

					if (myEngine->KeyHeld(Key_W))
					{
						if (hoverCarSpeed < 150.0f)						// The speed limit the car can go in mph
						{
							(hoverCarSpeed += SpeedStep*frametime);
						}

					}

					if (!isBouncingBack)
					{
						if (!myEngine->KeyHeld(Key_W) && !myEngine->KeyHeld(Key_S))
						{
							if (hoverCarSpeed > 0.0f)
							{
								(hoverCarSpeed -= SpeedStep*frametime);		// multiplying the speedstep by frametime to determine the speed of the car //
							}
							else
							{
								(hoverCarSpeed += SpeedStep*frametime);
							}

						}
					}

					if (!isBouncingBack)
					{
						if (myEngine->KeyHeld(Key_S))
						{
							if (hoverCarSpeed > -50.0f)						// The reverse limit the car can go in mph //
							{
								(hoverCarSpeed -= SpeedStep*frametime);
							}
							else
							{
								(hoverCarSpeed += SpeedStep*frametime);
							}
						}
					}
					
					if (myEngine->KeyHeld(Key_A))
					{
						hoverCar->RotateLocalY(-kSteerSpeed);			// The steer speed when the user holds A //
					}

					if (myEngine->KeyHeld(Key_D))						// The steer speed when the uder holds D
					{
						hoverCar->RotateLocalY(kSteerSpeed);
					}
				}
			}



			// ---------- Game States ---------- //

			switch (gamestate)
			{

			case stage1:
			{

				bool checkpointReached = spheretosphere(checkpoint[0], hoverCar, carRadius, checkpointRadius);		// a bool stating that when a checkpoint is reached, the sphere to sphere collisiond decttion will be activated on the hover car radius //
				if (checkpointReached)
				{
					if (currentLap < 3.0f)		// stating how many laps there is //
					{
						currentLap++;
					}
					else
					{
						gamestates = Finished;	// game state finishes //
					}

					currentStage = "Checkpoint 1 Reached";		// When reaching stage 2, this message appears, this happens when you go through checkpoint 1 //
					gamestate = stage2;
				}

				break;
			}

			case stage2:
			{
				bool checkpointReached = spheretosphere(checkpoint[1], hoverCar, carRadius, checkpointRadius);		// a bool stating that when a checkpoint is reached, the sphere to sphere collisiond decttion will be activated on the hover car radius //
				if (checkpointReached)
				{
					currentStage = "Checkpoint 2 Reached";		// When reaching stage 3, this message appears, this happens when you go through checkpoint 2 //
					gamestate = stage3;
				}

				break;

			}

			case stage3:
			{
				bool checkpointReached = spheretosphere(checkpoint[2], hoverCar, carRadius, checkpointRadius);		// a bool stating that when a checkpoint is reached, the sphere to sphere collisiond decttion will be activated on the hover car radius //
				if (checkpointReached)
				{
					currentStage = "Checkpoint 3 Reached";		// When reaching stage 3, this message appears, this happens when you go through checkpoint 2 //
					gamestate = stage1;
				}

				break;

			}

			}

			// ---------- Scores ---------- //

			stringstream lapText;
			lapText << "Lap:" << currentLap;							// Making the text say "Lap" in the top left corner in red //
			myFont->Draw(lapText.str(), 20, 20, kRed, kLeft, kTop);		// Setting up the size, colour, positioning of the text //
			lapText.str("");

			stringstream stageText;
			stageText << "Stage:" << currentStage;							// Making the text say "Stage:" in the bottom middle part of the screen in blue //
			myFont->Draw(stageText.str(), 300, 700, kBlue, kLeft, kBottom);		// Setting up the size, colour, positioning of the text //
			stageText.str("");

		}

		// ---------- Pausing the game ---------- //

		if (myEngine->KeyHit(Key_P))
		{
			paused = !paused;	// When hitting the pause key, the game will pause //
		}

		// ---------- Stopping the game ---------- //

		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();	// When hitting the escape key, the game will stop //
		}

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}