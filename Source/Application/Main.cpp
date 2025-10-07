int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene
    std::vector<neu::vec3> points{
        {-0.25f, -0.25f, 0},
        {0.25f, -0.25f, 0},
        {0, 0.25f, 0},
         
        { -0.5f, 0.5f, 0 },
        {0.5f, 0.5f, 0},
        {0.5f, -0.5f, 0},
		{-0.5f, -0.5f, 0},

        {0,1,0},
        {0,0,0},
        {-0.75f,0.25f,0},
        {0,0,0},
        {-0.5f,-1,0},
        {0,0,0},
        {0.5f,-1,0},
        {0,0,0},
        {0.75f,0.25f,0},
        {0.25f,0,0},

        {-0.5f,0.5f,0},
        {0,0.5f,0},
        {0.5f,0.5f,0},
        {0.5f,0,0},
        {0.5f,-0.5f,0},
        {0,-0.5f,0},
        {-0.5f,-0.5f,0},


    };
    std::vector<neu::vec3> colors{
        {0,1,0},
        {0,1,0},
        {0,1,0},

        {0,0,1},
        {0,0,1},
        {0,0,1},
        {0,0,1},

        {1,0,0},
        {0,0,0},
        {1,0,0},
        {0,0,0},
        {1,0,0},
        {0,0,0},
        {1,0,0},
        {0,0,0},
        {1,0,0},
        {0,0,0},

        {0,0,1},
        {0,1,1},
        {1,1,1},
        {1,1,0},
        {1,0,0},
        {0,1,0},
        {1,0,1},
        
        

    };

    std::vector<neu::vec3> velocities;
    for(int i=0;i<7;i++)
		velocities.push_back({ neu::random::getReal(-0.00025f,0.00025f), neu::random::getReal(-0.00025f,0.00025f),0 }
    );

    SDL_Event e;
    bool quit = false;

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        // draw
        neu::vec3 color{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();
        glLoadIdentity();
        
        //mouse triangle
        glPushMatrix();
    neu::InputSystem input = neu::GetEngine().GetInput();
        neu::vec2 mouse= input.GetMousePosition();
        //std::cout << mouse << std::endl;
        glTranslatef(( - 640  +mouse.x) / 640, (512 - mouse.y) / 512, 0);
        int rotation = 300*neu::GetEngine().GetTime().GetTime();

        glRotatef(rotation, 0, 0, 1);
        
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 3; i++) {
            

            glColor3f(colors[i].r + (rotation / ((i+1)*11) % 100) / 100.0f, colors[i].g+(rotation / ((i + 1) * 12) % 110) / 110.0f, colors[i].b+ (rotation / ((i + 1) * 10) % 100) / 90.0f);
            glVertex3f(points[i].x,points[i].y,points[i].z);

        }
        glEnd();
        glPopMatrix();

        //corner rectangle
        glPushMatrix();
        if ((int)roundf(neu::GetEngine().GetTime().GetTime())%4 == 0) {
            glTranslatef(0.5f,0.5f,0);

        }else if ((int)roundf(neu::GetEngine().GetTime().GetTime())%4 == 1) {
            glTranslatef(0.5f,-0.5f,0);

        }else if ((int)roundf(neu::GetEngine().GetTime().GetTime())%4 == 2) {
            glTranslatef(-0.5f,-0.5f,0);

        }else if ((int)roundf(neu::GetEngine().GetTime().GetTime())%4 == 3) {
            glTranslatef(-0.5f,0.5f,0);

        }
        

		glScalef(sinf(neu::GetEngine().GetTime().GetTime()), sinf(neu::GetEngine().GetTime().GetTime()), sinf(neu::GetEngine().GetTime().GetTime()));
        glBegin(GL_QUADS);
        for (int i = 3; i < 7; i++) {
            glColor3f(((int)(colors[i].r + (neu::GetEngine().GetTime().GetTime()) * 4*(i)) % 10) / 10.0f,
                ((int)(colors[i].g + (neu::GetEngine().GetTime().GetTime()) * 2*(i/2)) % 10) / 10.0f,
                ((int)(colors[i].b + (neu::GetEngine().GetTime().GetTime()) * 3*(i/3)) % 10) / 10.0f);
            glVertex3f(points[i].x+ sinf(neu::GetEngine().GetTime().GetTime()*4)/(i),
            points[i].y+ sinf(neu::GetEngine().GetTime().GetTime()*5)/(2*i),
                points[i].z);

        }
		glEnd();
        glPopMatrix();

		//moving strip
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 17; i < 24; i++) {
            for (int j = 0; j < 3; j++) {
                points[i].x += velocities[i - 17].x;
                points[i].y += velocities[i - 17].y;
                if (points[i].x > 1 || points[i].x < -1) velocities[i - 17].x = -velocities[i - 17].x;
                if (points[i].y > 1 || points[i].y < -1) velocities[i - 17].y = -velocities[i - 17].y;
            }
            glColor3f(colors[i].r+ points[i].x, colors[i].g + points[i].y, colors[i].b + (points[i].x/2 + points[i].y/2));
            glVertex3f(points[i].x, points[i].y, points[i].z);

        }


        glEnd();

        //center star
        glBegin(GL_POLYGON);
        for (int i = 7; i < 17; i++) {
            glColor3f(((int)(colors[i].r+ (neu::GetEngine().GetTime().GetTime())*20)%100)/100.0f,
                ((int)(colors[i].g +(neu::GetEngine().GetTime().GetTime()) * 30) % 100)/100.0f,
                ((int)(colors[i].b + (neu::GetEngine().GetTime().GetTime()) * 40) % 100)/100.0f);
            glVertex3f(points[i].x, points[i].y, points[i].z);

        }
        glEnd();

       

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
