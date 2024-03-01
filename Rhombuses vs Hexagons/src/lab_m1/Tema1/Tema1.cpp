#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <time.h>
#include <cstdlib>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // the player starts with 3 lives
    lives = 3;

    // the player starts with 5 stars
    currency = 5;

    // initial interval untill pink stars are rendered (8 - 15 seconds)
    std::srand(std::time(0));
    timeUntillNextStars = std::rand() % 8 + 8;

    timeSinceLastStars = 0;

    // prices of each type of rhombus
    rhombus_prices.push_back(1);
    rhombus_prices.push_back(2);
    rhombus_prices.push_back(2);
    rhombus_prices.push_back(3);

    // y coordinates of each line for hexagons
    yCoordLinesHexagon.push_back(0.3f);
    yCoordLinesHexagon.push_back(0.18f);
    yCoordLinesHexagon.push_back(0.06f);

    glm::vec3 red = glm::vec3(1, 0, 0);
    glm::vec3 green = glm::vec3(0, 1, 0);
    glm::vec3 orange = glm::vec3(1, 0.55f, 0);
    glm::vec3 blue = glm::vec3(0, 0, 1);
    glm::vec3 yellow = glm::vec3(1, 1, 0);
    glm::vec3 purple = glm::vec3(0.5f, 0, 0.5f);
    glm::vec3 silver = glm::vec3(0.75f, 0.75f, 0.75f);
    glm::vec3 turqoise = glm::vec3(0, 0.807f, 0.819f);
    glm::vec3 pink = glm::vec3(0.78f, 0.09f, 0.52f);

    // creating mesh for squares
    {
        // red square centered in origin
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(0.5, 0.5, 1), red),
                VertexFormat(glm::vec3(0.5, -0.5, 1), red),
                VertexFormat(glm::vec3(-0.5, -0.5, 1), red),
                VertexFormat(glm::vec3(-0.5, 0.5, 1), red),
            };

            vector<unsigned int> indices = 
            {
                0, 2, 1,
                0, 3, 2
            };

            meshes["identity_square_red"] = new Mesh("identity_square_red");
            meshes["identity_square_red"]->InitFromData(vertices, indices);
        }

        // green square centered in origin
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(0.5, 0.5, 1), green),
                VertexFormat(glm::vec3(0.5, -0.5, 1), green),
                VertexFormat(glm::vec3(-0.5, -0.5, 1), green),
                VertexFormat(glm::vec3(-0.5, 0.5, 1), green),
            };

            vector<unsigned int> indices = 
            {
                0, 2, 1,
                0, 3, 2
            };

            meshes["identity_square_green"] = new Mesh("identity_square_green");
            meshes["identity_square_green"]->InitFromData(vertices, indices);
        }
    }

    // creating mesh for rhombuses
    {
        // orange rhombus centered in origin
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(0, 0.4, 2), orange),
                VertexFormat(glm::vec3(0.25, 0, 2), orange),
                VertexFormat(glm::vec3(0, -0.4, 2), orange),
                VertexFormat(glm::vec3(-0.25, 0, 2), orange),
                VertexFormat(glm::vec3(0, -0.1, 2), orange),
                VertexFormat(glm::vec3(0.45, -0.1, 2), orange),
                VertexFormat(glm::vec3(0.45, 0.1, 2), orange),
                VertexFormat(glm::vec3(0, 0.1, 2), orange),
            };

            vector<unsigned int> indices = 
            {
                1, 0, 3,
                1, 3, 2,
                6, 7, 4,
                6, 4, 5
            };

            meshes["identity_rhombus_orange"] = new Mesh("identity_rhombus_orange");
            meshes["identity_rhombus_orange"]->InitFromData(vertices, indices);
        }

        // blue rhombus centered in origin
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(0, 0.4, 2), blue),
                VertexFormat(glm::vec3(0.25, 0, 2), blue),
                VertexFormat(glm::vec3(0, -0.4, 2), blue),
                VertexFormat(glm::vec3(-0.25, 0, 2), blue),
                VertexFormat(glm::vec3(0, -0.1, 2), blue),
                VertexFormat(glm::vec3(0.45, -0.1, 2), blue),
                VertexFormat(glm::vec3(0.45, 0.1, 2), blue),
                VertexFormat(glm::vec3(0, 0.1, 2), blue),
            };

            vector<unsigned int> indices = 
            {
                1, 0, 3,
                1, 3, 2,
                6, 7, 4,
                6, 4, 5
            };

            meshes["identity_rhombus_blue"] = new Mesh("identity_rhombus_blue");
            meshes["identity_rhombus_blue"]->InitFromData(vertices, indices);
        }

        // yellow rhombus centered in origin
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(0, 0.4, 2), yellow),
                VertexFormat(glm::vec3(0.25, 0, 2), yellow),
                VertexFormat(glm::vec3(0, -0.4, 2), yellow),
                VertexFormat(glm::vec3(-0.25, 0, 2), yellow),
                VertexFormat(glm::vec3(0, -0.1, 2), yellow),
                VertexFormat(glm::vec3(0.45, -0.1, 2), yellow),
                VertexFormat(glm::vec3(0.45, 0.1, 2), yellow),
                VertexFormat(glm::vec3(0, 0.1, 2), yellow),
            };

            vector<unsigned int> indices = 
            {
                1, 0, 3,
                1, 3, 2,
                6, 7, 4,
                6, 4, 5
            };

            meshes["identity_rhombus_yellow"] = new Mesh("identity_rhombus_yellow");
            meshes["identity_rhombus_yellow"]->InitFromData(vertices, indices);
        }

        // purple rhombus centered in origin
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(0, 0.4, 2), purple),
                VertexFormat(glm::vec3(0.25, 0, 2), purple),
                VertexFormat(glm::vec3(0, -0.4, 2), purple),
                VertexFormat(glm::vec3(-0.25, 0, 2), purple),
                VertexFormat(glm::vec3(0, -0.1, 2), purple),
                VertexFormat(glm::vec3(0.45, -0.1, 2), purple),
                VertexFormat(glm::vec3(0.45, 0.1, 2), purple),
                VertexFormat(glm::vec3(0, 0.1, 2), purple),
            };

            vector<unsigned int> indices = 
            {
                1, 0, 3,
                1, 3, 2,
                6, 7, 4,
                6, 4, 5
            };

            meshes["identity_rhombus_purple"] = new Mesh("identity_rhombus_purple");
            meshes["identity_rhombus_purple"]->InitFromData(vertices, indices);
        }
    }

    // creating mesh for stars
    {
        // pink star centered in origin
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(-0.587, 0.809, 3), pink),
                VertexFormat(glm::vec3(0.587, 0.809, 3), pink),
                VertexFormat(glm::vec3(0.951, -0.309, 3), pink),
                VertexFormat(glm::vec3(0, -1, 3), pink),
                VertexFormat(glm::vec3(-0.951, -0.309, 3), pink),
                VertexFormat(glm::vec3(0, 1.809, 3), pink),
                VertexFormat(glm::vec3(1.757, 0.809, 3), pink),
                VertexFormat(glm::vec3(1.073, -1.478, 3), pink),
                VertexFormat(glm::vec3(-1.073, -1.478, 3), pink),
                VertexFormat(glm::vec3(-1.757, 0.809, 3), pink)
            };

            vector<unsigned int> indices = 
            {
                1, 0, 4,
                1, 4, 3,
                1, 3, 2,

                5, 0, 1,
                0, 9, 4,
                4, 8, 3,
                2, 3, 7,
                6, 1, 2
            };

            meshes["identity_star_pink"] = new Mesh("identity_star_pink");
            meshes["identity_star_pink"]->InitFromData(vertices, indices);
        }

        // silver star centered in origin
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(-0.587, 0.809, 2), silver),
                VertexFormat(glm::vec3(0.587, 0.809, 2), silver),
                VertexFormat(glm::vec3(0.951, -0.309, 2), silver),
                VertexFormat(glm::vec3(0, -1, 2), silver),
                VertexFormat(glm::vec3(-0.951, -0.309, 2), silver),
                VertexFormat(glm::vec3(0, 1.809, 2), silver),
                VertexFormat(glm::vec3(1.757, 0.809, 2), silver),
                VertexFormat(glm::vec3(1.073, -1.478, 2), silver),
                VertexFormat(glm::vec3(-1.073, -1.478, 2), silver),
                VertexFormat(glm::vec3(-1.757, 0.809, 2), silver)
            };

            vector<unsigned int> indices = 
            {
                1, 0, 4,
                1, 4, 3,
                1, 3, 2,

                5, 0, 1,
                0, 9, 4,
                4, 8, 3,
                2, 3, 7,
                6, 1, 2
            };

            meshes["identity_star_silver"] = new Mesh("identity_star_silver");
            meshes["identity_star_silver"]->InitFromData(vertices, indices);
        }

        // orange star centered in origin
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(-0.587, 0.809, 2), orange),
                VertexFormat(glm::vec3(0.587, 0.809, 2), orange),
                VertexFormat(glm::vec3(0.951, -0.309, 2), orange),
                VertexFormat(glm::vec3(0, -1, 2), orange),
                VertexFormat(glm::vec3(-0.951, -0.309, 2), orange),
                VertexFormat(glm::vec3(0, 1.809, 2), orange),
                VertexFormat(glm::vec3(1.757, 0.809, 2), orange),
                VertexFormat(glm::vec3(1.073, -1.478, 2), orange),
                VertexFormat(glm::vec3(-1.073, -1.478, 2), orange),
                VertexFormat(glm::vec3(-1.757, 0.809, 2), orange)
            };

            vector<unsigned int> indices = 
            {
                1, 0, 4,
                1, 4, 3,
                1, 3, 2,

                5, 0, 1,
                0, 9, 4,
                4, 8, 3,
                2, 3, 7,
                6, 1, 2
            };

            meshes["identity_star_orange"] = new Mesh("identity_star_orange");
            meshes["identity_star_orange"]->InitFromData(vertices, indices);
        }

        // blue star centered in origin
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(-0.587, 0.809, 2), blue),
                VertexFormat(glm::vec3(0.587, 0.809, 2), blue),
                VertexFormat(glm::vec3(0.951, -0.309, 2), blue),
                VertexFormat(glm::vec3(0, -1, 2), blue),
                VertexFormat(glm::vec3(-0.951, -0.309, 2), blue),
                VertexFormat(glm::vec3(0, 1.809, 2), blue),
                VertexFormat(glm::vec3(1.757, 0.809, 2), blue),
                VertexFormat(glm::vec3(1.073, -1.478, 2), blue),
                VertexFormat(glm::vec3(-1.073, -1.478, 2), blue),
                VertexFormat(glm::vec3(-1.757, 0.809, 2), blue)
            };

            vector<unsigned int> indices = 
            {
                1, 0, 4,
                1, 4, 3,
                1, 3, 2,

                5, 0, 1,
                0, 9, 4,
                4, 8, 3,
                2, 3, 7,
                6, 1, 2
            };

            meshes["identity_star_blue"] = new Mesh("identity_star_blue");
            meshes["identity_star_blue"]->InitFromData(vertices, indices);
        }

        // yellow star centered in origin
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(-0.587, 0.809, 2), yellow),
                VertexFormat(glm::vec3(0.587, 0.809, 2), yellow),
                VertexFormat(glm::vec3(0.951, -0.309, 2), yellow),
                VertexFormat(glm::vec3(0, -1, 2), yellow),
                VertexFormat(glm::vec3(-0.951, -0.309, 2), yellow),
                VertexFormat(glm::vec3(0, 1.809, 2), yellow),
                VertexFormat(glm::vec3(1.757, 0.809, 2), yellow),
                VertexFormat(glm::vec3(1.073, -1.478, 2), yellow),
                VertexFormat(glm::vec3(-1.073, -1.478, 2), yellow),
                VertexFormat(glm::vec3(-1.757, 0.809, 2), yellow)
            };

            vector<unsigned int> indices = 
            {
                1, 0, 4,
                1, 4, 3,
                1, 3, 2,

                5, 0, 1,
                0, 9, 4,
                4, 8, 3,
                2, 3, 7,
                6, 1, 2
            };

            meshes["identity_star_yellow"] = new Mesh("identity_star_yellow");
            meshes["identity_star_yellow"]->InitFromData(vertices, indices);
        }

        // purple star centered in origin
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(-0.587, 0.809, 2), purple),
                VertexFormat(glm::vec3(0.587, 0.809, 2), purple),
                VertexFormat(glm::vec3(0.951, -0.309, 2), purple),
                VertexFormat(glm::vec3(0, -1, 2), purple),
                VertexFormat(glm::vec3(-0.951, -0.309, 2), purple),
                VertexFormat(glm::vec3(0, 1.809, 2), purple),
                VertexFormat(glm::vec3(1.757, 0.809, 2), purple),
                VertexFormat(glm::vec3(1.073, -1.478, 2), purple),
                VertexFormat(glm::vec3(-1.073, -1.478, 2), purple),
                VertexFormat(glm::vec3(-1.757, 0.809, 2), purple)
            };

            vector<unsigned int> indices = 
            {
                1, 0, 4,
                1, 4, 3,
                1, 3, 2,

                5, 0, 1,
                0, 9, 4,
                4, 8, 3,
                2, 3, 7,
                6, 1, 2
            };

            meshes["identity_star_purple"] = new Mesh("identity_star_purple");
            meshes["identity_star_purple"]->InitFromData(vertices, indices);
        }
    }

    // creating mesh for hexagons
    {

        // ration between inner hexagon and outer hexagon
        float inner_ratio = 0.7;

        // orange hexagon centered in origin
        {
            vector<VertexFormat> vertices
            {
                // outer hexagon (orange)
                VertexFormat(glm::vec3(-0.5, 0.866, 2), orange),
                VertexFormat(glm::vec3(0.5, 0.866, 2), orange),
                VertexFormat(glm::vec3(1, 0, 2), orange),
                VertexFormat(glm::vec3(0.5, -0.866, 2), orange),
                VertexFormat(glm::vec3(-0.5, -0.866, 2), orange),
                VertexFormat(glm::vec3(-1, 0, 2), orange),

                // inner hexagon
                VertexFormat(glm::vec3(-0.5 * inner_ratio, 0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3(0.5 * inner_ratio, 0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3((float)1 * inner_ratio, 0, 2.1), turqoise),
                VertexFormat(glm::vec3(0.5 * inner_ratio, -0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3(-0.5 * inner_ratio, -0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3((float)-1 * inner_ratio, 0, 2.1), turqoise),
            };

            vector<unsigned int> indices = 
            {
                // outer hexagon (orange)
                0, 5, 1,
                1, 5, 2,
                2, 5, 3,
                3, 5, 4,

                // inner hexagon
                6, 11, 7,
                7, 11, 8,
                8, 11, 9,
                9, 11, 10
            };

            meshes["identity_hexagon_orange"] = new Mesh("identity_hexagon_orange");
            meshes["identity_hexagon_orange"]->InitFromData(vertices, indices);
        }

        // bluehexagon centered in origin
        {
            vector<VertexFormat> vertices
            {
                // outer hexagon (blue)
                VertexFormat(glm::vec3(-0.5, 0.866, 2), blue),
                VertexFormat(glm::vec3(0.5, 0.866, 2), blue),
                VertexFormat(glm::vec3(1, 0, 2), blue),
                VertexFormat(glm::vec3(0.5, -0.866, 2), blue),
                VertexFormat(glm::vec3(-0.5, -0.866, 2), blue),
                VertexFormat(glm::vec3(-1, 0, 2), blue),

                // inner hexagon
                VertexFormat(glm::vec3(-0.5 * inner_ratio, 0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3(0.5 * inner_ratio, 0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3((float)1 * inner_ratio, 0, 2.1), turqoise),
                VertexFormat(glm::vec3(0.5 * inner_ratio, -0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3(-0.5 * inner_ratio, -0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3((float)-1 * inner_ratio, 0, 2.1), turqoise),
            };

            vector<unsigned int> indices = 
            {
                // outer hexagon (blue)
                0, 5, 1,
                1, 5, 2,
                2, 5, 3,
                3, 5, 4,

                // inner hexagon
                6, 11, 7,
                7, 11, 8,
                8, 11, 9,
                9, 11, 10
            };

            meshes["identity_hexagon_blue"] = new Mesh("identity_hexagon_blue");
            meshes["identity_hexagon_blue"]->InitFromData(vertices, indices);
        }

        // yellow hexagon centered in origin
        {
            vector<VertexFormat> vertices
            {
                // outer hexagon (yellow)
                VertexFormat(glm::vec3(-0.5, 0.866, 2), yellow),
                VertexFormat(glm::vec3(0.5, 0.866, 2), yellow),
                VertexFormat(glm::vec3(1, 0, 2), yellow),
                VertexFormat(glm::vec3(0.5, -0.866, 2), yellow),
                VertexFormat(glm::vec3(-0.5, -0.866, 2), yellow),
                VertexFormat(glm::vec3(-1, 0, 2), yellow),

                // inner hexagon
                VertexFormat(glm::vec3(-0.5 * inner_ratio, 0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3(0.5 * inner_ratio, 0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3((float)1 * inner_ratio, 0, 2.1), turqoise),
                VertexFormat(glm::vec3(0.5 * inner_ratio, -0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3(-0.5 * inner_ratio, -0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3((float)-1 * inner_ratio, 0, 2.1), turqoise),
            };

            vector<unsigned int> indices = 
            {
                // outer hexagon (yellow)
                0, 5, 1,
                1, 5, 2,
                2, 5, 3,
                3, 5, 4,

                // inner hexagon
                6, 11, 7,
                7, 11, 8,
                8, 11, 9,
                9, 11, 10
            };

            meshes["identity_hexagon_yellow"] = new Mesh("identity_hexagon_yellow");
            meshes["identity_hexagon_yellow"]->InitFromData(vertices, indices);
        }

        // purple hexagon centered in origin
        {
            vector<VertexFormat> vertices
            {
                // outer hexagon (purple)
                VertexFormat(glm::vec3(-0.5, 0.866, 2), purple),
                VertexFormat(glm::vec3(0.5, 0.866, 2), purple),
                VertexFormat(glm::vec3(1, 0, 2), purple),
                VertexFormat(glm::vec3(0.5, -0.866, 2), purple),
                VertexFormat(glm::vec3(-0.5, -0.866, 2), purple),
                VertexFormat(glm::vec3(-1, 0, 2), purple),

                // inner hexagon
                VertexFormat(glm::vec3(-0.5 * inner_ratio, 0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3(0.5 * inner_ratio, 0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3((float)1 * inner_ratio, 0, 2.1), turqoise),
                VertexFormat(glm::vec3(0.5 * inner_ratio, -0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3(-0.5 * inner_ratio, -0.866 * inner_ratio, 2.1), turqoise),
                VertexFormat(glm::vec3((float)-1 * inner_ratio, 0, 2.1), turqoise),
            };

            vector<unsigned int> indices = 
            {
                // outer hexagon (purple)
                0, 5, 1,
                1, 5, 2,
                2, 5, 3,
                3, 5, 4,

                // inner hexagon
                6, 11, 7,
                7, 11, 8,
                8, 11, 9,
                9, 11, 10
            };

            meshes["identity_hexagon_purple"] = new Mesh("identity_hexagon_purple");
            meshes["identity_hexagon_purple"]->InitFromData(vertices, indices);
        }
    }
    Mesh* background = object2D::CreateSquare("background", glm::vec3(0, 0, 0), resolution.x, glm::vec3(0.22f, 0.22f, 0.22f), true);
    AddMeshToList(background);

    Mesh* clearSquare = object2D::CreateSquare("clearSquare", glm::vec3(-0.5, -0.5, 1), 1, glm::vec3(0, 0, 0));
    AddMeshToList(clearSquare);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    int scale_factor = resolution.x;

    modelMatrix = glm::mat3(1);

    RenderMesh2D(meshes["background"], shaders["VertexColor"], modelMatrix);

    // render the red rectangle and the 9 green squares
    {
        // red rectangle J
        modelMatrix = getModelMatrix(0.03, 0.34, 0.025, 0.18, scale_factor);
        RenderMesh2D(meshes["identity_square_red"], shaders["VertexColor"], modelMatrix);

        // green square A
        modelMatrix = getModelMatrix(0.1, 0.1, 0.11, 0.3, scale_factor);
        RenderMesh2D(meshes["identity_square_green"], shaders["VertexColor"], modelMatrix);
        if (squares.size() < 9) {
            squares.push_back(*(new Square(0.11, 0.3, 0.1, false)));
        }

        // green square B
        modelMatrix = getModelMatrix(0.1, 0.1, 0.23, 0.3, scale_factor);
        RenderMesh2D(meshes["identity_square_green"], shaders["VertexColor"], modelMatrix);
        if (squares.size() < 9) {
            squares.push_back(*(new Square(0.23, 0.3, 0.1, false)));
        }

        // green square C
        modelMatrix = getModelMatrix(0.1, 0.1, 0.35, 0.3, scale_factor);
        RenderMesh2D(meshes["identity_square_green"], shaders["VertexColor"], modelMatrix);
        if (squares.size() < 9) {
            squares.push_back(*(new Square(0.35, 0.3, 0.1, false)));
        }

        // green square D
        modelMatrix = getModelMatrix(0.1, 0.1, 0.11, 0.18, scale_factor);
        RenderMesh2D(meshes["identity_square_green"], shaders["VertexColor"], modelMatrix);
        if (squares.size() < 9) {
            squares.push_back(*(new Square(0.11, 0.18, 0.1, false)));
        }

        // green square E
        modelMatrix = getModelMatrix(0.1, 0.1, 0.23, 0.18, scale_factor);
        RenderMesh2D(meshes["identity_square_green"], shaders["VertexColor"], modelMatrix);
        if (squares.size() < 9) {
            squares.push_back(*(new Square(0.23, 0.18, 0.1, false)));
        }

        // green square F
        modelMatrix = getModelMatrix(0.1, 0.1, 0.35, 0.18, scale_factor);
        RenderMesh2D(meshes["identity_square_green"], shaders["VertexColor"], modelMatrix);
        if (squares.size() < 9) {
            squares.push_back(*(new Square(0.35, 0.18, 0.1, false)));
        }

        // green square G
        modelMatrix = getModelMatrix(0.1, 0.1, 0.11, 0.06, scale_factor);
        RenderMesh2D(meshes["identity_square_green"], shaders["VertexColor"], modelMatrix);
        if (squares.size() < 9) {
            squares.push_back(*(new Square(0.11, 0.06, 0.1, false)));
        }

        // green square H
        modelMatrix = getModelMatrix(0.1, 0.1, 0.23, 0.06, scale_factor);
        RenderMesh2D(meshes["identity_square_green"], shaders["VertexColor"], modelMatrix);
        if (squares.size() < 9) {
            squares.push_back(*(new Square(0.23, 0.06, 0.1, false)));
        }

        // green square I
        modelMatrix = getModelMatrix(0.1, 0.1, 0.35, 0.06, scale_factor);
        RenderMesh2D(meshes["identity_square_green"], shaders["VertexColor"], modelMatrix);
        if (squares.size() < 9) {
            squares.push_back(*(new Square(0.35, 0.06, 0.1, false)));
        }
    }

    // render the 4 squares whith rhombuses inside
    {
        // square & orange rhombus
        modelMatrix = getModelMatrix(rhombus_square_scale, rhombus_square_scale, 0.08, 0.5, scale_factor);
        RenderMesh2D(meshes["clearSquare"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["identity_rhombus_orange"], shaders["VertexColor"], modelMatrix);
        if (rhombus_shop.size() < 4) {
            rhombus_shop.push_back(std::pair<float, float>(0.08, 0.5));
        }

        // square & blue rhombus
        modelMatrix = getModelMatrix(rhombus_square_scale, rhombus_square_scale, 0.24, 0.5, scale_factor);
        RenderMesh2D(meshes["clearSquare"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["identity_rhombus_blue"], shaders["VertexColor"], modelMatrix);
        if (rhombus_shop.size() < 4) {
            rhombus_shop.push_back(std::pair<float, float>(0.24, 0.5));
        }

        // square & yellow rhombus
        modelMatrix = getModelMatrix(rhombus_square_scale, rhombus_square_scale, 0.4, 0.5, scale_factor);
        RenderMesh2D(meshes["clearSquare"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["identity_rhombus_yellow"], shaders["VertexColor"], modelMatrix);
        if (rhombus_shop.size() < 4) {
            rhombus_shop.push_back(std::pair<float, float>(0.4, 0.5));
        }

        // square & purple rhombus
        modelMatrix = getModelMatrix(rhombus_square_scale, rhombus_square_scale, 0.56, 0.5, scale_factor);
        RenderMesh2D(meshes["clearSquare"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["identity_rhombus_purple"], shaders["VertexColor"], modelMatrix);
        if (rhombus_shop.size() < 4) {
            rhombus_shop.push_back(std::pair<float, float>(0.56, 0.5));
        }
    }

    // render the lives
    {
        for (int i = 0; i < lives; i++) {
            modelMatrix = getModelMatrix(0.06, 0.06, 0.68 + (float)i * 0.09, 0.5, scale_factor);
            RenderMesh2D(meshes["identity_square_red"], shaders["VertexColor"], modelMatrix);
        }
    }

    // render the cost of each rhombus
    {
        // one silver star for orange rhombus
        modelMatrix = getModelMatrix(0.008, 0.008, 0.045, 0.43, scale_factor);
        RenderMesh2D(meshes["identity_star_silver"], shaders["VertexColor"], modelMatrix);

        // two silver stars for blue rhombus
        modelMatrix = getModelMatrix(0.008, 0.008, 0.205, 0.43, scale_factor);
        RenderMesh2D(meshes["identity_star_silver"], shaders["VertexColor"], modelMatrix);

        modelMatrix = getModelMatrix(0.008, 0.008, 0.235, 0.43, scale_factor);
        RenderMesh2D(meshes["identity_star_silver"], shaders["VertexColor"], modelMatrix);

        // two silver stars for yellow rhombus
        modelMatrix = getModelMatrix(0.008, 0.008, 0.365, 0.43, scale_factor);
        RenderMesh2D(meshes["identity_star_silver"], shaders["VertexColor"], modelMatrix);

        modelMatrix = getModelMatrix(0.008, 0.008, 0.395, 0.43, scale_factor);
        RenderMesh2D(meshes["identity_star_silver"], shaders["VertexColor"], modelMatrix);

        // three silver stars for purple rhombus
        modelMatrix = getModelMatrix(0.008, 0.008, 0.525, 0.43, scale_factor);
        RenderMesh2D(meshes["identity_star_silver"], shaders["VertexColor"], modelMatrix);

        modelMatrix = getModelMatrix(0.008, 0.008, 0.555, 0.43, scale_factor);
        RenderMesh2D(meshes["identity_star_silver"], shaders["VertexColor"], modelMatrix);

        modelMatrix = getModelMatrix(0.008, 0.008, 0.585, 0.43, scale_factor);
        RenderMesh2D(meshes["identity_star_silver"], shaders["VertexColor"], modelMatrix);
    }

    // render the number of stars owned
    {
        for (int i = 0; i < currency; i++) {
            modelMatrix = getModelMatrix(0.008, 0.008, 0.67 + (float)i * 0.03, 0.45, scale_factor);
            RenderMesh2D(meshes["identity_star_silver"], shaders["VertexColor"], modelMatrix);
        }
    }

    // render the pink stars
    {
        timeSinceLastStars += deltaTimeSeconds;
        if ((int)timeSinceLastStars == timeUntillNextStars) {
            // 3 more stars need to be rendered

            for (int i = 0; i < 3; i ++) {
                pink_stars_pos.push_back(getRandomCoordinates());
            }

            timeSinceLastStars = 0;
            timeUntillNextStars = std::rand() % 8 + 8;
        }

        for (auto pair = pink_stars_pos.begin(); pair != pink_stars_pos.end(); pair++) {
            modelMatrix = getModelMatrix(pink_star_scale, pink_star_scale, pair->first, pair->second, scale_factor, resolution.y);
            RenderMesh2D(meshes["identity_star_pink"], shaders["VertexColor"], modelMatrix);
        }
    }

    // render the owned rhombuses
    {
        for (int pos = 0; pos < rhombuses.size(); pos++) {
            float translateX = rhombuses[pos].getCenterScaleX();
            float translateY = rhombuses[pos].getCenterScaleY();
            int color = rhombuses[pos].getColor();
            float rhombus_scale = rhombuses[pos].getSizeScale();
            
            modelMatrix = getModelMatrix(rhombus_scale, rhombus_scale, translateX, translateY, scale_factor);
                        
            if (color == 0) {
                RenderMesh2D(meshes["identity_rhombus_orange"], shaders["VertexColor"], modelMatrix);
            } else if (color == 1) {
                RenderMesh2D(meshes["identity_rhombus_blue"], shaders["VertexColor"], modelMatrix);
            } else if (color == 2) {
                RenderMesh2D(meshes["identity_rhombus_yellow"], shaders["VertexColor"], modelMatrix);
            } else if (color == 3) {
                RenderMesh2D(meshes["identity_rhombus_purple"], shaders["VertexColor"], modelMatrix);
            }

            if (rhombuses[pos].isFading()) {
                rhombuses[pos].setSizeScale(rhombus_scale - deltaTimeSeconds * 0.5);

                if (rhombuses[pos].getSizeScale() - 0.0f < 0.005) {
                    squares[rhombuses[pos].getSquarePos()].setOccupied(false);
                    rhombuses.erase(rhombuses.begin() + pos);
                }
            }
        }
    }

    // render the hexagons
    {
        timeSinceLastSpawnHexagon += deltaTimeSeconds;
        if ((int)timeSinceLastSpawnHexagon == timeBetweenSpawnsHexagon) {

            // another hexagon needs to be added
            timeSinceLastSpawnHexagon = 0;

            int line = Hexagon::getRandomLine();
            int color = Hexagon::getRandomColor();

            hexagons.push_back(*(new Hexagon(color, line, 1.0f)));
            timeBetweenSpawnsHexagon = Hexagon::getRandomTimeBetweenSpawns();
        }

        for (int pos = 0; pos < hexagons.size(); pos++) {
            float translateX = hexagons[pos].getCenterScaleX();
            float translateY = yCoordLinesHexagon[hexagons[pos].getLine()];
            int color = hexagons[pos].getColor();
            float sizeScale = hexagons[pos].getSizeScale();

            modelMatrix = getModelMatrix(sizeScale, sizeScale, translateX, translateY, scale_factor);

            if (color == 0) {
                RenderMesh2D(meshes["identity_hexagon_orange"], shaders["VertexColor"], modelMatrix);
            } else if (color == 1) {
                RenderMesh2D(meshes["identity_hexagon_blue"], shaders["VertexColor"], modelMatrix);
            } else if (color == 2) {
                RenderMesh2D(meshes["identity_hexagon_yellow"], shaders["VertexColor"], modelMatrix);
            } else if (color == 3) {
                RenderMesh2D(meshes["identity_hexagon_purple"], shaders["VertexColor"], modelMatrix);
            }

            // the hexagon is moving to the left
            hexagons[pos].setCenterScaleX(translateX - deltaTimeSeconds * 0.06);

            // the hexagon has reached the left end
            if (hexagons[pos].getCenterScaleX() - 0.0f < 0.03) {
                hexagons.erase(hexagons.begin() + pos);
                lives--;
            }

            // the game ends
            if (!lives) {
                exit(0);
            }

            // current hexagon is fading
            if (hexagons[pos].isFading()) {
                hexagons[pos].setSizeScale(sizeScale - deltaTimeSeconds * 0.2);

                if (hexagons[pos].getSizeScale() - 0.0f < 0.005) {
                    // the hexagon disappears
                    hexagons.erase(hexagons.begin() + pos);
                }
            }
        }
    }

    // render the attacking stars
    {
        for (int pos = 0; pos < stars.size(); pos++) {
            float translateX = stars[pos].getCenterScaleX();
            float translateY = stars[pos].getCenterScaleY();
            float angle = stars[pos].getAngle();
            float sizeScaleStar = stars[pos].getSizeScale();
            int color = stars[pos].getColor();
            modelMatrix = getModelMatrixAngular(sizeScaleStar, sizeScaleStar, translateX, translateY, scale_factor,
                                                1, angle);

            if (color == 0) {
                RenderMesh2D(meshes["identity_star_orange"], shaders["VertexColor"], modelMatrix);
            } else if (color == 1) {
                RenderMesh2D(meshes["identity_star_blue"], shaders["VertexColor"], modelMatrix);
            } else if (color == 2) {
                RenderMesh2D(meshes["identity_star_yellow"], shaders["VertexColor"], modelMatrix);
            } else if (color == 3) {
                RenderMesh2D(meshes["identity_star_purple"], shaders["VertexColor"], modelMatrix);
            }

            stars[pos].setCenterScaleX(translateX + deltaTimeSeconds * 0.09);
            stars[pos].setAngle(angle - glm::radians(200.0f) * deltaTimeSeconds);
        }
    }

    checkAttackingStarCollision(scale_factor);
    checkRhombusCollision(scale_factor);
    spawnAttackingStars(deltaTimeSeconds);
}

void Tema1::spawnAttackingStars(float deltaTimeSeconds) {
    for (int posR = 0; posR < rhombuses.size(); posR++) {
        int rhombusLine = rhombuses[posR].getSquarePos() / 3;
        for (int posH = 0; posH < hexagons.size(); posH++) {
            if (rhombusLine == hexagons[posH].getLine() &&
                rhombuses[posR].getColor() == hexagons[posH].getColor()) {

                // there is a hexagon of the same color as the rhombus
                rhombuses[posR].updateTimer(deltaTimeSeconds);
                
                if ((int)rhombuses[posR].getTimeSinceLastShot() == time_between_shots) {
                    // the current rhombus needs to shoot
                    stars.push_back(*(new Star(rhombuses[posR].getCenterScaleX() + 0.02, rhombuses[posR].getCenterScaleY(),
                                    rhombuses[posR].getColor())));

                    // restart the timer
                    rhombuses[posR].restartTimer();
                }
            }
        }
    }
}

void Tema1::checkAttackingStarCollision (int scale_factor) {
    for (int posS = 0; posS < stars.size(); posS++) {
        for (int posH = 0; posH < hexagons.size(); posH++) {
            float SX = stars[posS].getCenterScaleX() * scale_factor;
            float SY = stars[posS].getCenterScaleY() * scale_factor;
            float radiusS = stars[posS].getSizeScale() * scale_factor / 2;
            int colorS = stars[posS].getColor();

            float HX = hexagons[posH].getCenterScaleX() * scale_factor;
            float HY = yCoordLinesHexagon[hexagons[posH].getLine()] * scale_factor;
            float radiusH = scaleSizeHexagon * scale_factor / 2;
            int colorH = hexagons[posH].getColor();

            glm::vec3 centerS = {SX, SY, 0};
            glm::vec3 centerH = {HX, HY, 0};

            if (colorS == colorH && glm::distance(centerS, centerH) <= radiusS + radiusH) {
                // the star hit a hexagon of the same color

                // remove the star
                stars.erase(stars.begin() + posS);

                // decremente no of lives of hexagon
                hexagons[posH].updateLives();
                if (hexagons[posH].getLives() == 0) {
                    hexagons[posH].setFading(true);
                }
            }
        }
    }
}

void Tema1::checkRhombusCollision (int scale_factor) {
    // checking the collition between each non-fading rhombus and each hexagon
    for (int posR = 0; posR < rhombuses.size(); posR++) {
        for (int posH = 0; posH < hexagons.size() && !rhombuses[posR].isFading() && !rhombuses[posR].isTravelling(); posH++) {
            float RX = rhombuses[posR].getCenterScaleX() * scale_factor;
            float RY = rhombuses[posR].getCenterScaleY() * scale_factor;
            float radiusR = rhombuses[posR].getSizeScale() * scale_factor / 2;

            float HX = hexagons[posH].getCenterScaleX() * scale_factor;
            float HY = yCoordLinesHexagon[hexagons[posH].getLine()] * scale_factor;
            float radiusH = scaleSizeHexagon * scale_factor / 2;

            glm::vec3 centerR = {RX, RY, 0};
            glm::vec3 centerH = {HX, HY, 0};

            if (glm::distance(centerR, centerH) <= radiusR + radiusH) {
                // the rhombus must fade away
                rhombuses[posR].setFading(true);
            }
        }
    }
}

// computes the matrix specific for rotation, scaling and translation of a mesh
glm::mat3 Tema1::getModelMatrixAngular(float scaleX, float scaleY, float translateX, float translateY,
                                       float scale_factorX, float scale_factorY, float radians) {
    
    if (scale_factorY == 1) {
        scale_factorY = scale_factorX;
    }

    
    modelMatrix = glm::mat3(1);

    modelMatrix *= transform2D::Translate(translateX * scale_factorX, translateY * scale_factorY);
    modelMatrix *= transform2D::Scale(scaleX * scale_factorX, scaleY * scale_factorX);
    modelMatrix *= transform2D::Rotate(radians);

    return modelMatrix;
}

// computes the matrix specific for scaling and translation of a mesh
glm::mat3 Tema1::getModelMatrix(float scaleX, float scaleY, float translateX, float translateY,
                                float scale_factorX, float scale_factorY) {

    if (scale_factorY == 1) {
        scale_factorY = scale_factorX;
    }
    
    modelMatrix = glm::mat3(1);

    modelMatrix *= transform2D::Translate(translateX * scale_factorX, translateY * scale_factorY);
    modelMatrix *= transform2D::Scale(scaleX * scale_factorX, scaleY * scale_factorX);

    return modelMatrix;
}

// generates a pair of random coordinates between 0 and 1
std::pair<float, float> Tema1::getRandomCoordinates() {
    std::pair<float, float> coordinates;
    coordinates.first = (float)(std::rand() % 100) / 100.0f;
    coordinates.second = (float)(std::rand() % 100) / 100.0f;

    return coordinates;
}

bool Tema1::mouseInRectangle(float mouseX, float mouseY, std::pair<float, float> center, float length, float height) {
    return (mouseX >= center.first - length / 2 && mouseX <= center.first + length / 2 &&
            mouseY >= center.second - height / 2 && mouseY <= center.second + height / 2);
}

void Tema1::FrameEnd()
{
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    glm::ivec2 resolution = window->GetResolution();
    int scale_factor = resolution.x;

    mouseY = 720 - mouseY;

    mouseX *= (int)(resolution.x / 1280.0f);
    mouseY *= (int)(resolution.y / 720.0f);

    // there is a rhombus travelling
    if (rhombuses.size() && rhombuses[rhombuses.size() - 1].isTravelling()) {
        modelMatrix = getModelMatrix(rhombus_square_scale, rhombus_square_scale, (float)mouseX / scale_factor, (float)mouseY / scale_factor, scale_factor);
        
        rhombuses[rhombuses.size() - 1].setCenterScaleX((float)mouseX / scale_factor);
        rhombuses[rhombuses.size() - 1].setCenterScaleY((float)mouseY / scale_factor);
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    glm::ivec2 resolution = window->GetResolution();
    int scale_factor = resolution.x;


    mouseY = 720 - mouseY;

    mouseX *= (int)(resolution.x / 1280.0f);
    mouseY *= (int)(resolution.y / 720.0f);

    if (button == 1) {
        // check if the mouse is inside a pink star
        for (int pos = 0; pos < pink_stars_pos.size(); pos++) {
            std::pair<float, float> center;
            center.first = pink_stars_pos[pos].first * scale_factor;
            center.second = pink_stars_pos[pos].second * resolution.y;

            if (mouseInRectangle((float)mouseX, (float)mouseY, center, pink_star_scale * scale_factor * pink_star_sizeX, pink_star_scale * scale_factor * pink_star_sizeY)) {
                currency++;
                pink_stars_pos.erase(pink_stars_pos.begin() + pos);
            }
        }

        for (int pos = 0; pos < rhombus_shop.size(); pos++) {
            std::pair<float, float> center;
            center.first = rhombus_shop[pos].first * scale_factor;
            center.second = rhombus_shop[pos].second * scale_factor;

            if (mouseInRectangle((float)mouseX, (float)mouseY, center, rhombus_square_scale * scale_factor, rhombus_square_scale * scale_factor)) {
                // the player has enough resources to buy this type of rhombus
                if (currency >= rhombus_prices[pos]) {
                    rhombuses.push_back(*(new Rhombus(pos, 2, false, true, (float)mouseX / scale_factor,(float)mouseY / scale_factor )));
                    currency -= rhombus_prices[pos];
                }
            }
        }
    } else if (button == 2) {
        // right click on a placed rhombus => that rhombus needs to be removed
        for (int pos = 0; pos < squares.size(); pos++) {
            std::pair<float, float> center;
            center.first = squares[pos].getCenterScaleX() * scale_factor;
            center.second = squares[pos].getCenterScaleY() * scale_factor;
        
            if (mouseInRectangle((float)mouseX, (float)mouseY, center, squares[pos].getSizeScale() * scale_factor, squares[pos].getSizeScale() * scale_factor) &&
                squares[pos].isOccupied()) {
                
                // the rhombus placed on this square needs to be removed

                for (int i = 0; i < rhombuses.size(); i++) {
                    if (rhombuses[i].getSquarePos() == pos) {
                        // this is the rhombus that needs to be removed
                        rhombuses[i].setFading(true);
                    }
                }
            }
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    glm::ivec2 resolution = window->GetResolution();
    int scale_factor = resolution.x;

    mouseY = 720 - mouseY;

    mouseX *= (int)(resolution.x / 1280.0f);
    mouseY *= (int)(resolution.y / 720.0f);

    bool placed = false;

    if (button == 1 && rhombuses.size() && rhombuses[rhombuses.size() - 1].isTravelling()) {
        for (int pos = 0; pos < squares.size(); pos++) {
            float centerX = squares[pos].getCenterScaleX() * scale_factor;
            float centerY = squares[pos].getCenterScaleY() * scale_factor;
            float size = squares[pos].getSizeScale() * scale_factor;

            if (mouseInRectangle((float)mouseX, (float)mouseY, std::pair<float, float>(centerX, centerY), size, size) &&
                !squares[pos].isOccupied()) {
                // the rhombus can be placed in the current square
                placed  = true;

                // the current rhombus is no longer travelling
                rhombuses[rhombuses.size() - 1].setTravel(false);
                rhombuses[rhombuses.size() - 1].setSquarePos(pos);

                // the coordinates of the rhombus match the coordinates of the squared it is placed upon
                rhombuses[rhombuses.size() - 1].setCenterScaleX(squares[pos].getCenterScaleX());
                rhombuses[rhombuses.size() - 1].setCenterScaleY(squares[pos].getCenterScaleY());

                squares[pos].setOccupied(true);
            }
        }

        if (!placed) {
            int color = rhombuses[rhombuses.size() - 1].getColor();
            currency += rhombus_prices[color];
            rhombuses.pop_back();
        }
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
