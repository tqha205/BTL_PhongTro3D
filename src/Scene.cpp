#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>

void Scene::Initialize() {
    Objects.clear();
    for(int i=0; i<4; i++) recessedLights[i] = nullptr;

    // Palette
    glm::vec3 colWall(0.95f, 0.95f, 0.95f);      
    glm::vec3 colFloor(0.6f, 0.4f, 0.3f);     
    glm::vec3 colWood(0.7f, 0.45f, 0.3f); 
    glm::vec3 colDarkWood(0.3f, 0.15f, 0.1f);
    glm::vec3 colWhite(0.95f, 0.95f, 0.95f);
    glm::vec3 colBlack(0.15f, 0.15f, 0.15f);
    glm::vec3 colBlue(0.2f, 0.4f, 0.5f);
    glm::vec3 colGreen(0.3f, 0.6f, 0.3f);
    glm::vec3 colOrange(0.8f, 0.4f, 0.3f);
    glm::vec3 colYellow(1.0f, 0.9f, 0.5f);
    glm::vec3 colGrey(0.6f, 0.6f, 0.6f);
    glm::vec3 colGrass(0.25f, 0.45f, 0.25f);
    glm::vec3 colBase(0.2f, 0.2f, 0.22f);

    // 0. Environment
    // Grass
    AddBox(glm::vec3(0, -0.2f, 0), glm::vec3(60.0f, 0.1f, 60.0f), colGrass, 0.1f, 0.9f);
    // Dark grey base plate
    AddBox(glm::vec3(0, -0.05f, 0), glm::vec3(22.0f, 0.1f, 26.0f), colBase, 0.1f, 0.9f);

    // 1. Structure (Sàn, Tường, Trần)
    // Sàn nhà (Floor)
    AddBox(glm::vec3(0, 0.1f, -0.5f), glm::vec3(16.0f, 0.2f, 21.0f), colFloor, 0.1f, 0.7f); 
    // Tường sau (Back Wall with Window)
    // Bottom part (y=0 to 5.0) -> height 5.0, center y=2.5
    AddBox(glm::vec3(0, 2.5f, -10.5f), glm::vec3(16.0f, 5.0f, 0.2f), colWall, 0.0f, 1.0f);
    // Top part (y=8.0 to 9.0) -> height 1.0, center y=8.5
    AddBox(glm::vec3(0, 8.5f, -10.5f), glm::vec3(16.0f, 1.0f, 0.2f), colWall, 0.0f, 1.0f);
    // Left part (x=-8.0 to x=2.5) -> width 10.5, center x=-2.75, height 3.0 (y=6.5)
    AddBox(glm::vec3(-2.75f, 6.5f, -10.5f), glm::vec3(10.5f, 3.0f, 0.2f), colWall, 0.0f, 1.0f);
    // Right part (x=6.5 to x=8.0) -> width 1.5, center x=7.25, height 3.0 (y=6.5)
    AddBox(glm::vec3(7.25f, 6.5f, -10.5f), glm::vec3(1.5f, 3.0f, 0.2f), colWall, 0.0f, 1.0f);
    // Tường trái (Left Wall)
    AddBox(glm::vec3(-8.1f, 4.5f, -0.5f), glm::vec3(0.2f, 9.0f, 20.0f), colWall, 0.0f, 1.0f);  
    // Tường phải (Right Wall)
    AddBox(glm::vec3(8.1f, 4.5f, -0.5f), glm::vec3(0.2f, 9.0f, 20.0f), colWall, 0.0f, 1.0f);   
    // Tường trước (Front Wall with Door Gap)
    // Left part of front wall (width 6.5, center -4.75)
    AddBox(glm::vec3(-4.75f, 4.5f, 9.5f), glm::vec3(6.5f, 9.0f, 0.2f), colWall, 0.0f, 1.0f);
    // Right part of front wall (width 6.5, center 4.75)
    AddBox(glm::vec3(4.75f, 4.5f, 9.5f), glm::vec3(6.5f, 9.0f, 0.2f), colWall, 0.0f, 1.0f);
    // Top part of front wall (header)
    AddBox(glm::vec3(0, 8.0f, 9.5f), glm::vec3(3.0f, 2.0f, 0.2f), colWall, 0.0f, 1.0f);

    // Cửa chính (Interactive Door)
    // Positioned at the gap: x=0, z=9.5. Size: 3.0 wide, 7.0 high.
    // We'll place the pivot at the edge (x=-1.5).
    AddBox(glm::vec3(0, 3.5f, 9.5f), glm::vec3(3.0f, 7.0f, 0.1f), colDarkWood, 0.1f, 0.8f);
    mainDoor = Objects.back();

    // Tay nắm cửa chính (Main Door Handle)
    AddBox(glm::vec3(1.2f, 3.5f, 9.6f), glm::vec3(0.04f, 0.6f, 0.15f), colBlack, 0.8f, 0.2f); // Front handle
    mainDoorHandle = Objects.back();

    // Trần (Ceiling)
    AddBox(glm::vec3(0, 9.1f, -0.5f), glm::vec3(16.4f, 0.2f, 20.0f), colWall, 0.0f, 1.0f); 

    // Gác xép (Loft floor)
    float loftZ = -6.2f;
    float loftDepth = 8.4f;
    float loftY = 4.5f;
    AddBox(glm::vec3(0, loftY, loftZ), glm::vec3(16.0f, 0.2f, loftDepth), colFloor, 0.1f, 0.7f); 

    // --- LOFT FURNITURE ---
    // 1. Wardrobe (Tủ quần áo) - 3 doors
    glm::vec3 wdPos(-6.25f, loftY + 2.7f, -9.5f);
    AddBox(wdPos, glm::vec3(3.0f, 5.4f, 1.8f), colDarkWood, 0.1f, 0.8f);
    // Wardrobe doors background
    AddBox(wdPos + glm::vec3(0, 0, 0.92f), glm::vec3(2.8f, 5.2f, 0.05f), colDarkWood, 0.1f, 0.9f);
    // Gaps between 3 doors
    AddBox(wdPos + glm::vec3(-0.47f, 0, 0.94f), glm::vec3(0.03f, 5.2f, 0.05f), colBlack, 0.5f, 0.2f); 
    AddBox(wdPos + glm::vec3(0.47f, 0, 0.94f), glm::vec3(0.03f, 5.2f, 0.05f), colBlack, 0.5f, 0.2f);
    // Door handles
    AddBox(wdPos + glm::vec3(-0.55f, -0.2f, 0.96f), glm::vec3(0.04f, 0.8f, 0.04f), colGrey, 0.8f, 0.2f);
    AddBox(wdPos + glm::vec3(0.35f, -0.2f, 0.96f), glm::vec3(0.04f, 0.8f, 0.04f), colGrey, 0.8f, 0.2f);
    AddBox(wdPos + glm::vec3(0.55f, -0.2f, 0.96f), glm::vec3(0.04f, 0.8f, 0.04f), colGrey, 0.8f, 0.2f);

    // 2. Bed (Redesigned completely)
    glm::vec3 bedBasePos(-1.0f, loftY + 0.25f, -8.0f);
    // Bed frame
    AddBox(bedBasePos, glm::vec3(4.6f, 0.3f, 4.2f), colWood, 0.1f, 0.6f);
    // Headboard
    AddBox(bedBasePos + glm::vec3(0, 0.7f, -2.0f), glm::vec3(4.6f, 1.5f, 0.2f), colWood, 0.1f, 0.8f);
    // Mattress
    glm::vec3 mattressPos = bedBasePos + glm::vec3(0, 0.3f, 0);
    AddBox(mattressPos, glm::vec3(4.2f, 0.4f, 3.8f), colWhite, 0.0f, 1.0f);
    // Blanket (Wrapped over the foot of the bed)
    AddBox(mattressPos + glm::vec3(0, 0.22f, 0.9f), glm::vec3(4.3f, 0.08f, 2.1f), colGrey, 0.0f, 0.9f); // Top part of blanket
    AddBox(mattressPos + glm::vec3(0, 0.0f, 1.9f), glm::vec3(4.3f, 0.45f, 0.1f), colGrey, 0.0f, 0.9f); // Drape over foot
    // Pillows
    AddBox(mattressPos + glm::vec3(-1.0f, 0.25f, -1.3f), glm::vec3(1.4f, 0.15f, 0.9f), colWhite, 0.0f, 0.9f);
    AddBox(mattressPos + glm::vec3(1.0f, 0.25f, -1.3f), glm::vec3(1.4f, 0.15f, 0.9f), colWhite, 0.0f, 0.9f);

    // Nightstands
    glm::vec3 nsLeft = glm::vec3(bedBasePos.x - 2.8f, loftY + 0.35f, bedBasePos.z - 1.5f);
    glm::vec3 nsRight = glm::vec3(bedBasePos.x + 2.8f, loftY + 0.35f, bedBasePos.z - 1.5f);
    AddBox(nsLeft, glm::vec3(1.0f, 0.5f, 1.0f), colWood, 0.1f, 0.8f);
    AddBox(nsRight, glm::vec3(1.0f, 0.5f, 1.0f), colWood, 0.1f, 0.8f);
    
    // Left Nightstand: Detailed Lamp
    AddBox(nsLeft + glm::vec3(0, 0.26f, 0), glm::vec3(0.3f, 0.02f, 0.3f), colBlack, 0.5f, 0.5f); // Lamp base
    AddBox(nsLeft + glm::vec3(0, 0.4f, 0), glm::vec3(0.05f, 0.3f, 0.05f), colBlack, 0.5f, 0.5f);  // Lamp stem
    AddBox(nsLeft + glm::vec3(0, 0.6f, 0), glm::vec3(0.4f, 0.2f, 0.4f), colYellow, 0.0f, 1.0f);   // Lamp shade
    
    // Right Nightstand: Flower Vase
    AddBox(nsRight + glm::vec3(0, 0.4f, 0), glm::vec3(0.2f, 0.3f, 0.2f), colWhite, 0.1f, 0.1f);   // Ceramic Vase
    AddBox(nsRight + glm::vec3(0, 0.65f, 0), glm::vec3(0.05f, 0.2f, 0.05f), glm::vec3(0.2f, 0.8f, 0.2f), 0.0f, 0.8f); // Stem
    AddBox(nsRight + glm::vec3(0, 0.8f, 0), glm::vec3(0.35f, 0.3f, 0.35f), glm::vec3(0.2f, 0.6f, 0.2f), 0.0f, 0.9f);  // Leaves
    AddBox(nsRight + glm::vec3(0, 0.95f, 0), glm::vec3(0.15f, 0.15f, 0.15f), colOrange, 0.0f, 0.8f); // Flower

    // 3. Desk Area - Taller
    glm::vec3 deskPos(4.5f, loftY + 1.0f, -8.0f);
    AddBox(deskPos, glm::vec3(3.5f, 0.1f, 1.8f), colWood, 0.1f, 0.7f); // Desktop
    AddBox(deskPos + glm::vec3(-1.6f, -0.45f, -0.7f), glm::vec3(0.1f, 0.9f, 0.1f), colBlack, 0.0f, 0.8f);
    AddBox(deskPos + glm::vec3(1.6f, -0.45f, -0.7f), glm::vec3(0.1f, 0.9f, 0.1f), colBlack, 0.0f, 0.8f);
    AddBox(deskPos + glm::vec3(-1.6f, -0.45f, 0.7f), glm::vec3(0.1f, 0.9f, 0.1f), colBlack, 0.0f, 0.8f);
    AddBox(deskPos + glm::vec3(1.6f, -0.45f, 0.7f), glm::vec3(0.1f, 0.9f, 0.1f), colBlack, 0.0f, 0.8f);
    
    // Monitor
    AddBox(deskPos + glm::vec3(0, 0.1f, -0.5f), glm::vec3(0.6f, 0.2f, 0.4f), colBlack, 0.1f, 0.5f); // Base
    AddBox(deskPos + glm::vec3(0, 0.8f, -0.6f), glm::vec3(1.8f, 1.2f, 0.05f), colBlack, 0.0f, 0.2f); // Screen
    // Keyboard & Mouse
    AddBox(deskPos + glm::vec3(0, 0.06f, 0.2f), glm::vec3(1.2f, 0.02f, 0.4f), colGrey, 0.0f, 0.5f); // Keyboard
    AddBox(deskPos + glm::vec3(0.9f, 0.06f, 0.2f), glm::vec3(0.15f, 0.03f, 0.2f), colBlack, 0.1f, 0.5f); // Mouse
    // Books
    AddBox(deskPos + glm::vec3(-1.2f, 0.15f, 0.3f), glm::vec3(0.4f, 0.1f, 0.6f), colBlue, 0.0f, 0.8f);
    AddBox(deskPos + glm::vec3(-1.2f, 0.25f, 0.3f), glm::vec3(0.4f, 0.1f, 0.6f), colOrange, 0.0f, 0.8f);

    // Ergonomic Chair - Taller
    glm::vec3 chairPos = deskPos + glm::vec3(0, -0.4f, 1.3f);
    AddBox(chairPos, glm::vec3(0.9f, 0.12f, 0.9f), colBlack, 0.1f, 0.5f); // Seat
    AddBox(chairPos + glm::vec3(0, 0.6f, 0.45f), glm::vec3(0.8f, 1.3f, 0.15f), colBlack, 0.1f, 0.5f); // Ergonomic Back
    AddBox(chairPos + glm::vec3(0, 1.3f, 0.45f), glm::vec3(0.5f, 0.3f, 0.1f), colBlack, 0.1f, 0.5f); // Headrest
    AddBox(chairPos + glm::vec3(0.45f, 0.4f, 0), glm::vec3(0.1f, 0.7f, 0.6f), colBlack, 0.1f, 0.5f); // Armrest L
    AddBox(chairPos + glm::vec3(-0.45f, 0.4f, 0), glm::vec3(0.1f, 0.7f, 0.6f), colBlack, 0.1f, 0.5f); // Armrest R
    // Base and 5-star legs with wheels
    AddBox(chairPos + glm::vec3(0, -0.2f, 0), glm::vec3(0.08f, 0.4f, 0.08f), colBlack, 0.5f, 0.2f); // Central pillar
    for(int i = 0; i < 5; i++) {
        float angle = i * (360.0f / 5.0f);
        float rad = glm::radians(angle);
        float lx = cos(rad);
        float lz = sin(rad);
        
        // Leg extending outward
        glm::vec3 legPos = chairPos + glm::vec3(lx * 0.175f, -0.4f, lz * 0.175f);
        AddBox(legPos, glm::vec3(0.35f, 0.04f, 0.04f), colBlack, 0.5f, 0.2f, glm::vec3(0, -angle, 0));
        
        // Wheel at the end of the leg
        glm::vec3 wheelPos = chairPos + glm::vec3(lx * 0.35f, -0.45f, lz * 0.35f);
        AddBox(wheelPos, glm::vec3(0.06f, 0.06f, 0.06f), colBlack, 0.1f, 0.8f, glm::vec3(0, -angle, 0));
    }

    // Window Glass
    AddBox(glm::vec3(4.5f, 6.5f, -10.45f), glm::vec3(4.0f, 3.0f, 0.05f), glm::vec3(0.3f, 0.6f, 0.9f), 0.9f, 0.05f);
    // Frames
    AddBox(glm::vec3(2.5f, 6.5f, -10.4f), glm::vec3(0.15f, 3.0f, 0.2f), colBlack, 0.1f, 0.8f);
    AddBox(glm::vec3(6.5f, 6.5f, -10.4f), glm::vec3(0.15f, 3.0f, 0.2f), colBlack, 0.1f, 0.8f);
    AddBox(glm::vec3(4.5f, 5.0f, -10.4f), glm::vec3(4.0f, 0.15f, 0.2f), colBlack, 0.1f, 0.8f);
    AddBox(glm::vec3(4.5f, 8.0f, -10.4f), glm::vec3(4.0f, 0.15f, 0.2f), colBlack, 0.1f, 0.8f);
    // Crossbars
    AddBox(glm::vec3(4.5f, 6.5f, -10.4f), glm::vec3(4.0f, 0.1f, 0.2f), colBlack, 0.1f, 0.8f);
    AddBox(glm::vec3(3.5f, 6.5f, -10.4f), glm::vec3(0.1f, 3.0f, 0.2f), colBlack, 0.1f, 0.8f);
    AddBox(glm::vec3(5.5f, 6.5f, -10.4f), glm::vec3(0.1f, 3.0f, 0.2f), colBlack, 0.1f, 0.8f);

    // Loft Railing
    float railZ = -2.1f;
    AddBox(glm::vec3(-1.0f, loftY + 0.9f, railZ), glm::vec3(14.0f, 0.05f, 0.05f), colGrey, 0.8f, 0.2f);
    AddBox(glm::vec3(-1.0f, loftY + 0.5f, railZ), glm::vec3(14.0f, 0.05f, 0.05f), colGrey, 0.8f, 0.2f);
    AddBox(glm::vec3(-1.0f, loftY + 0.1f, railZ), glm::vec3(14.0f, 0.05f, 0.05f), colGrey, 0.8f, 0.2f);
    for (float rx = -7.8f; rx <= 5.8f; rx += 1.5f) {
        AddBox(glm::vec3(rx, loftY + 0.5f, railZ), glm::vec3(0.05f, 0.9f, 0.05f), colGrey, 0.8f, 0.2f);
    }

    // --- STAIRS ---
    float startZ = 4.0f;
    float endZ = railZ;
    int numSteps = 16;
    float stepHeight = (loftY) / numSteps;
    float stepDepth = (startZ - endZ) / numSteps;
    float stairX = 6.9f;

    for (int i = 0; i < numSteps; i++) {
        glm::vec3 stepPos(stairX, 0.1f + i * stepHeight + stepHeight/2, startZ - i * stepDepth - stepDepth/2);
        AddBox(stepPos, glm::vec3(2.2f, 0.08f, stepDepth * 0.9f), colWood, 0.1f, 0.7f);
    }
    
    // Continuous Slanted Elements for Stairs
    float dY = loftY; 
    float dZ = startZ - endZ; 
    float stairLength = sqrt(dY * dY + dZ * dZ) + 0.2f; // Slight extension for overlaps
    float stairAngle = glm::degrees(atan2(dY, dZ)); // Positive rotation makes +Z go down
    
    glm::vec3 stairCenter(stairX, 0.1f + dY / 2.0f, startZ - dZ / 2.0f);
    
    // Left Stringer (Outside)
    AddBox(stairCenter + glm::vec3(-1.1f, 0, 0), glm::vec3(0.12f, 0.3f, stairLength), colBlack, 0.5f, 0.2f, glm::vec3(stairAngle, 0, 0));
    // Right Stringer (Against Wall)
    AddBox(stairCenter + glm::vec3(1.1f, 0, 0), glm::vec3(0.12f, 0.3f, stairLength), colBlack, 0.5f, 0.2f, glm::vec3(stairAngle, 0, 0));
    
    // Simple Wooden Handrail (Outside/Left)
    AddBox(stairCenter + glm::vec3(-1.1f, 0.9f, 0), glm::vec3(0.08f, 0.06f, stairLength), colWood, 0.1f, 0.7f, glm::vec3(stairAngle, 0, 0));

    // Vertical metal posts connecting stringer to handrail
    for (int k = 0; k <= 5; k++) {
        float t = k / 5.0f;
        float pZ = startZ - t * dZ;
        float pY = 0.1f + t * dY + 0.45f; // Center of the post
        AddBox(glm::vec3(stairX - 1.1f, pY, pZ), glm::vec3(0.04f, 0.9f, 0.04f), colBlack, 0.5f, 0.2f);
    }

    // --- GROUND FLOOR ---
    // 1. Kitchen
    glm::vec3 kitPos(-2.0f, 1.2f, -9.5f);
    AddBox(kitPos, glm::vec3(7.0f, 2.4f, 1.8f), colWhite, 0.0f, 0.9f); // Lower cabinet base
    
    // Lower cabinet doors and handles
    for(int i=-3; i<=3; i++) {
        AddBox(kitPos + glm::vec3(i * 0.95f, 0, 0.91f), glm::vec3(0.9f, 2.2f, 0.02f), colWhite, 0.1f, 0.8f);
        AddBox(kitPos + glm::vec3(i * 0.95f, 0.8f, 0.93f), glm::vec3(0.3f, 0.05f, 0.05f), colGrey, 0.8f, 0.2f);
    }

    AddBox(kitPos + glm::vec3(0, 1.25f, 0), glm::vec3(7.2f, 0.1f, 1.9f), colBlack, 0.1f, 0.3f); // Countertop

    // Sink & Faucet
    glm::vec3 sinkPos = kitPos + glm::vec3(1.5f, 1.25f, 0.3f);
    AddBox(sinkPos + glm::vec3(0, 0.02f, 0), glm::vec3(1.2f, 0.12f, 0.8f), colGrey, 0.9f, 0.1f); // Sink lip
    AddBox(sinkPos + glm::vec3(0, 0.03f, 0), glm::vec3(1.1f, 0.12f, 0.7f), colBlack, 0.5f, 0.8f); // Sink hole
    AddBox(sinkPos + glm::vec3(0, 0.25f, -0.3f), glm::vec3(0.06f, 0.4f, 0.06f), colGrey, 0.9f, 0.1f); // Faucet base
    AddBox(sinkPos + glm::vec3(0, 0.45f, -0.15f), glm::vec3(0.06f, 0.06f, 0.3f), colGrey, 0.9f, 0.1f); // Faucet spout

    // Upper cabinet (Tủ bát)
    glm::vec3 upperPos = kitPos + glm::vec3(0, 2.6f, -0.4f);
    AddBox(upperPos, glm::vec3(7.0f, 1.0f, 1.0f), colWhite, 0.0f, 0.9f); // Main body
    AddBox(upperPos + glm::vec3(0, -0.55f, 0), glm::vec3(7.0f, 0.05f, 0.8f), colWhite, 0.0f, 1.0f); // Under-cabinet light

    // Dish rack (Giá để bát) in the center
    AddBox(upperPos + glm::vec3(0, 0, 0.45f), glm::vec3(2.0f, 0.8f, 0.1f), colBlack, 0.1f, 0.9f); // Dark interior
    for(int i=-3; i<=3; i++) {
        // Vertical plates
        AddBox(upperPos + glm::vec3(i * 0.25f, -0.1f, 0.3f), glm::vec3(0.05f, 0.5f, 0.5f), colWhite, 0.1f, 0.2f);
    }
    // Glass doors for the rest of the upper cabinet
    AddBox(upperPos + glm::vec3(-2.25f, 0, 0.51f), glm::vec3(2.4f, 0.95f, 0.05f), glm::vec3(0.2f, 0.3f, 0.4f), 0.5f, 0.1f);
    AddBox(upperPos + glm::vec3(2.25f, 0, 0.51f), glm::vec3(2.4f, 0.95f, 0.05f), glm::vec3(0.2f, 0.3f, 0.4f), 0.5f, 0.1f);

    // Realistic Fridge
    glm::vec3 fridgePos(2.5f, 2.2f, -9.0f);
    AddBox(fridgePos, glm::vec3(1.5f, 4.1f, 1.5f), colGrey, 0.8f, 0.2f); // Main Body
    AddBox(fridgePos + glm::vec3(0, -0.6f, 0.77f), glm::vec3(1.4f, 2.7f, 0.06f), glm::vec3(0.85f), 0.9f, 0.1f); // Fridge Door
    AddBox(fridgePos + glm::vec3(0, 1.3f, 0.77f), glm::vec3(1.4f, 1.0f, 0.06f), glm::vec3(0.85f), 0.9f, 0.1f); // Freezer Door
    AddBox(fridgePos + glm::vec3(0, 0.75f, 0.76f), glm::vec3(1.4f, 0.04f, 0.08f), colBlack, 0.5f, 0.5f); // Gap
    // Handles
    AddBox(fridgePos + glm::vec3(-0.5f, 0.3f, 0.85f), glm::vec3(0.05f, 0.8f, 0.1f), colBlack, 0.5f, 0.2f);
    AddBox(fridgePos + glm::vec3(-0.5f, 1.3f, 0.85f), glm::vec3(0.05f, 0.6f, 0.1f), colBlack, 0.5f, 0.2f);

    // --- BATHROOM ---
    // Bathroom Walls (under the loft, between fridge and stairs)
    glm::vec3 bathCol(0.9f, 0.9f, 0.9f); // Slightly darker white for bathroom walls
    AddBox(glm::vec3(3.4f, 2.25f, -8.2f), glm::vec3(0.1f, 4.5f, 4.4f), bathCol, 0.0f, 0.9f); // Left wall
    AddBox(glm::vec3(5.7f, 2.25f, -8.2f), glm::vec3(0.1f, 4.5f, 4.4f), bathCol, 0.0f, 0.9f); // Right wall
    AddBox(glm::vec3(5.2f, 2.25f, -6.0f), glm::vec3(1.0f, 4.5f, 0.1f), bathCol, 0.0f, 0.9f); // Front wall (right side)
    AddBox(glm::vec3(4.55f, 3.75f, -6.0f), glm::vec3(2.2f, 1.5f, 0.1f), bathCol, 0.0f, 0.9f); // Front wall (top, leaving doorway)

    // Bathroom Door
    AddBox(glm::vec3(4.05f, 1.5f, -6.0f), glm::vec3(1.3f, 3.0f, 0.1f), colWood, 0.1f, 0.7f);
    bathroomDoor = Objects.back();
    AddBox(glm::vec3(4.5f, 1.5f, -5.92f), glm::vec3(0.05f, 0.3f, 0.05f), colBlack, 0.5f, 0.2f); // Handle
    bathroomDoorHandle = Objects.back();

    // Toilet
    AddBox(glm::vec3(4.5f, 0.7f, -10.1f), glm::vec3(0.6f, 0.6f, 0.3f), colWhite, 0.1f, 0.3f); // Tank
    AddBox(glm::vec3(4.5f, 0.35f, -9.6f), glm::vec3(0.5f, 0.5f, 0.7f), colWhite, 0.1f, 0.3f); // Bowl
    AddBox(glm::vec3(4.5f, 0.6f, -9.6f), glm::vec3(0.5f, 0.05f, 0.7f), colWhite, 0.1f, 0.2f); // Lid

    // Sink and Mirror
    AddBox(glm::vec3(3.6f, 1.0f, -7.0f), glm::vec3(0.4f, 0.1f, 1.0f), colWhite, 0.1f, 0.2f); // Sink counter
    AddBox(glm::vec3(3.6f, 1.1f, -7.0f), glm::vec3(0.3f, 0.1f, 0.6f), colWhite, 0.1f, 0.2f); // Sink basin
    AddBox(glm::vec3(3.6f, 1.25f, -7.0f), glm::vec3(0.1f, 0.2f, 0.05f), colBlack, 0.5f, 0.2f); // Faucet
    AddBox(glm::vec3(3.46f, 1.8f, -7.0f), glm::vec3(0.02f, 1.0f, 0.8f), glm::vec3(0.8f, 0.9f, 1.0f), 1.0f, 0.0f); // Mirror

    // Shower Area
    AddBox(glm::vec3(5.0f, 1.5f, -8.5f), glm::vec3(1.4f, 3.0f, 0.05f), glm::vec3(0.3f, 0.6f, 0.8f), 0.9f, 0.1f); // Glass partition
    AddBox(glm::vec3(5.6f, 2.6f, -9.8f), glm::vec3(0.2f, 0.05f, 0.4f), colBlack, 0.5f, 0.2f); // Shower head pipe
    AddBox(glm::vec3(5.5f, 2.5f, -9.6f), glm::vec3(0.2f, 0.1f, 0.2f), colBlack, 0.5f, 0.2f); // Shower head

    // 2. Dining Area - Taller
    glm::vec3 dinPos(-2.0f, 1.8f, -5.0f);
    AddBox(dinPos, glm::vec3(3.0f, 0.1f, 2.0f), colWood, 0.1f, 0.6f);
    AddBox(dinPos + glm::vec3(-1.3f, -0.8f, -0.8f), glm::vec3(0.1f, 1.6f, 0.1f), colBlack, 0.0f, 0.8f);
    AddBox(dinPos + glm::vec3(1.3f, -0.8f, -0.8f), glm::vec3(0.1f, 1.6f, 0.1f), colBlack, 0.0f, 0.8f);
    AddBox(dinPos + glm::vec3(-1.3f, -0.8f, 0.8f), glm::vec3(0.1f, 1.6f, 0.1f), colBlack, 0.0f, 0.8f);
    AddBox(dinPos + glm::vec3(1.3f, -0.8f, 0.8f), glm::vec3(0.1f, 1.6f, 0.1f), colBlack, 0.0f, 0.8f);
    
    glm::vec3 c1 = dinPos + glm::vec3(-2.0f, -0.4f, 0);
    AddBox(c1, glm::vec3(0.8f, 0.1f, 0.8f), colWood, 0.1f, 0.8f); // Seat
    AddBox(c1 + glm::vec3(-0.35f, 0.5f, 0), glm::vec3(0.1f, 1.0f, 0.8f), colWood, 0.1f, 0.8f); // Back
    for(int i=-1; i<=1; i+=2) for(int j=-1; j<=1; j+=2)
        AddBox(c1 + glm::vec3(i*0.35f, -0.6f, j*0.35f), glm::vec3(0.1f, 1.2f, 0.1f), colBlack, 0.0f, 0.9f); // Legs

    glm::vec3 c2 = dinPos + glm::vec3(2.0f, -0.4f, 0);
    AddBox(c2, glm::vec3(0.8f, 0.1f, 0.8f), colWood, 0.1f, 0.8f); // Seat
    AddBox(c2 + glm::vec3(0.35f, 0.5f, 0), glm::vec3(0.1f, 1.0f, 0.8f), colWood, 0.1f, 0.8f); // Back
    for(int i=-1; i<=1; i+=2) for(int j=-1; j<=1; j+=2)
        AddBox(c2 + glm::vec3(i*0.35f, -0.6f, j*0.35f), glm::vec3(0.1f, 1.2f, 0.1f), colBlack, 0.0f, 0.9f); // Legs

    glm::vec3 c3 = dinPos + glm::vec3(0, -0.4f, 1.5f);
    AddBox(c3, glm::vec3(0.8f, 0.1f, 0.8f), colWood, 0.1f, 0.8f); // Seat
    AddBox(c3 + glm::vec3(0, 0.5f, 0.35f), glm::vec3(0.8f, 1.0f, 0.1f), colWood, 0.1f, 0.8f); // Back
    for(int i=-1; i<=1; i+=2) for(int j=-1; j<=1; j+=2)
        AddBox(c3 + glm::vec3(i*0.35f, -0.6f, j*0.35f), glm::vec3(0.1f, 1.2f, 0.1f), colBlack, 0.0f, 0.9f); // Legs

    // 3. Living Room Area - Taller
    glm::vec3 tvStandPos(-7.2f, 0.9f, 2.0f);
    AddBox(tvStandPos, glm::vec3(1.5f, 1.5f, 4.0f), colWood, 0.1f, 0.8f);
    AddBox(glm::vec3(-7.9f, 3.5f, 2.0f), glm::vec3(0.1f, 2.0f, 3.5f), colBlack, 0.1f, 0.2f);

    glm::vec3 rugPos(-3.5f, 0.2f, 2.0f);
    AddBox(rugPos, glm::vec3(4.5f, 0.05f, 5.0f), colWhite, 0.0f, 1.0f);
    // Coffee Table with legs - Taller
    AddBox(rugPos + glm::vec3(0, 0.6f, 0), glm::vec3(1.5f, 0.1f, 2.0f), colDarkWood, 0.1f, 0.6f);
    float tlX[2] = {-0.6f, 0.6f}, tlZ[2] = {-0.8f, 0.8f};
    for(int i=0; i<2; i++) for(int j=0; j<2; j++)
        AddBox(rugPos + glm::vec3(tlX[i], 0.3f, tlZ[j]), glm::vec3(0.1f, 0.6f, 0.1f), colDarkWood, 0.1f, 0.6f);

    // Couch with legs - Taller
    glm::vec3 couchPos(-1.0f, 1.0f, 2.0f);
    AddBox(couchPos, glm::vec3(1.5f, 0.4f, 4.5f), colBlue, 0.0f, 0.9f); // Seat
    AddBox(couchPos + glm::vec3(0.6f, 0.9f, 0), glm::vec3(0.4f, 1.5f, 4.5f), colBlue, 0.0f, 0.9f); // Back
    AddBox(couchPos + glm::vec3(0, 0.6f, 2.0f), glm::vec3(1.5f, 0.8f, 0.4f), colBlue, 0.0f, 0.9f); // Arm L
    AddBox(couchPos + glm::vec3(0, 0.6f, -2.0f), glm::vec3(1.5f, 0.8f, 0.4f), colBlue, 0.0f, 0.9f); // Arm R
    // 4 Straight black legs for sofa (like dining chairs)
    float slX[2] = {-0.6f, 0.6f}, slZ[2] = {-2.0f, 2.0f};
    for(int i=0; i<2; i++) {
        for(int j=0; j<2; j++) {
            // Straight black leg, y offset -0.5, height 0.6
            AddBox(couchPos + glm::vec3(slX[i], -0.5f, slZ[j]), glm::vec3(0.1f, 0.6f, 0.1f), colBlack, 0.0f, 0.9f);
        }
    }

    // 8. Đèn
    glm::vec3 lPos[] = { glm::vec3(-4.0f, 8.9f, -5.0f), glm::vec3(4.0f, 8.9f, -5.0f), glm::vec3(-4.0f, 8.9f, 3.0f), glm::vec3(4.0f, 8.9f, 3.0f) };
    for (int i = 0; i < 4; i++) {
        AddBox(lPos[i], glm::vec3(1.0f, 0.05f, 1.0f), colWhite, 0.5f, 0.1f);
        recessedLights[i] = Objects.back();
    }
}

void Scene::AddBox(glm::vec3 pos, glm::vec3 sc, glm::vec3 alb, float met, float rough, glm::vec3 rot, GLuint texID) {
    Object3D* obj = new Object3D(pos, sc, rot, alb, met, rough, texID);
    Objects.push_back(obj);
}
void Scene::AddCylinder(glm::vec3 pos, float radius, float height, glm::vec3 alb, float met, float rough, glm::vec3 rot) {}
void Scene::AddSphere(glm::vec3 pos, float radius, glm::vec3 alb, float met, float rough) {}
void Scene::Update(float deltaTime) {
    if (isDoorOpen && mainDoorAngle < 90.0f) {
        mainDoorAngle += 200.0f * deltaTime;
        if (mainDoorAngle > 90.0f) mainDoorAngle = 90.0f;
    } else if (!isDoorOpen && mainDoorAngle > 0.0f) {
        mainDoorAngle -= 200.0f * deltaTime;
        if (mainDoorAngle < 0.0f) mainDoorAngle = 0.0f;
    }
    
    // Update doorAngle for bathroom door (we can just share the angle value but flip direction if needed)
    doorAngle = mainDoorAngle;

    if (mainDoor) {
        // Pivot at x=-1.5 relative to door center.
        // Rotation around Y.
        mainDoor->rotation.y = mainDoorAngle;
        mainDoor->position.x = -1.5f + cos(glm::radians(mainDoorAngle)) * 1.5f;
        mainDoor->position.z = 9.5f - sin(glm::radians(mainDoorAngle)) * 1.5f;

        if (mainDoorHandle) {
            mainDoorHandle->rotation.y = mainDoorAngle;
            // Handle is at x=1.2 (relative to room). Distance from pivot (-1.5) is 2.7.
            mainDoorHandle->position.x = -1.5f + cos(glm::radians(mainDoorAngle)) * 2.7f;
            // Add a slight offset in Z so it protrudes through the door
            mainDoorHandle->position.z = 9.5f - sin(glm::radians(mainDoorAngle)) * 2.7f + 0.0f;
        }
    }
    
    if (bathroomDoor) {
        // Pivot for bathroom door is at x=3.4 (left side of door).
        // Center is at x=4.05. Distance = 0.65.
        // Rotation around Y. Let's make it open inwards to the bathroom (positive angle).
        bathroomDoor->rotation.y = doorAngle;
        bathroomDoor->position.x = 3.4f + cos(glm::radians(doorAngle)) * 0.65f;
        bathroomDoor->position.z = -6.0f - sin(glm::radians(doorAngle)) * 0.65f;
        
        if (bathroomDoorHandle) {
            bathroomDoorHandle->rotation.y = doorAngle;
            // Handle is at x=4.5. Distance from pivot = 1.1.
            bathroomDoorHandle->position.x = 3.4f + cos(glm::radians(doorAngle)) * 1.1f;
            bathroomDoorHandle->position.z = -6.0f - sin(glm::radians(doorAngle)) * 1.1f + 0.08f; 
        }
    }
}
void Scene::ToggleDoor() {
    isDoorOpen = !isDoorOpen;
}

void Scene::DrawAll(Shader& shader) {
    for (auto obj : Objects) {
        shader.setMat4("model", obj->GetModelMatrix());
        shader.setVec3("material.albedo", obj->material.albedo);
        shader.setFloat("material.metallic", obj->material.metallic);
        shader.setFloat("material.roughness", obj->material.roughness);
        shader.setVec3("emissive", (obj->material.albedo == glm::vec3(1.0f) && isLightOn) ? glm::vec3(15.0f) : glm::vec3(0.0f));
        obj->Draw();
    }
}

void Scene::ToggleLights() {
    isLightOn = !isLightOn;
    for (int i = 0; i < 4; i++) {
        if (recessedLights[i]) {
            recessedLights[i]->material.albedo = isLightOn ? glm::vec3(1.0f) : glm::vec3(0.2f);
        }
    }
}
