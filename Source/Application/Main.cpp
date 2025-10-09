int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // OPEN GL initialization
    std::vector<neu::vec3> points{
        //mouse triangle
        {-0.25f, -0.25f, 0},
        {0.25f, -0.25f, 0},
        {0, 0.25f, 0},
         
        //corner rectangle
  //      { -0.5f, 0.5f, 0 },
  //      {0.5f, 0.5f, 0},
  //      {0.5f, -0.5f, 0},
		//{-0.5f, -0.5f, 0},

  //      //polygon
  //      {0,1,0},
  //      {0,0,0},
  //      {-0.75f,0.25f,0},
  //      {0,0,0},
  //      {-0.5f,-1,0},
  //      {0,0,0},
  //      {0.5f,-1,0},
  //      {0,0,0},
  //      {0.75f,0.25f,0},
  //      {0.25f,0,0},

  //      //triangle strip
  //      {-0.5f,0.5f,0},
  //      {0,0.5f,0},
  //      {0.5f,0.5f,0},
  //      {0.5f,0,0},
  //      {0.5f,-0.5f,0},
  //      {0,-0.5f,0},
  //      {-0.5f,-0.5f,0},


    };
    std::vector<neu::vec3> colors{
        //triangle
        {1,1,0},
        {0,1,1},
        {1,0,1},

        //rectangle
        //{0,0,1},
        //{0,0,1},
        //{0,0,1},
        //{0,0,1},

        ////polygon
        //{1,0,0},
        //{0,0,0},
        //{1,0,0},
        //{0,0,0},
        //{1,0,0},
        //{0,0,0},
        //{1,0,0},
        //{0,0,0},
        //{1,0,0},
        //{0,0,0},

        ////triangle strip
        //{0,0,1},
        //{0,1,1},
        //{1,1,1},
        //{1,1,0},
        //{1,0,0},
        //{0,1,0},
        //{1,0,1},
        
    };

    GLuint vbo[2];
    glGenBuffers(2, vbo);

    //vertex buffer (position)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
    
    //vertex buffer (color)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);
    

    //vertex array
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //vertex attributes
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //color
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //vertex shader
    std::string vs_source;
	neu::file::ReadTextFile("shaders/basic.vert", vs_source);
    const char* vs_cstr = vs_source.c_str();

    GLuint vs;
    vs = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vs, 1, &vs_cstr, NULL);
    glCompileShader(vs);

    //Check for compile errors
    int success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(vs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

    //fragment shader
    std::string fs_source;
	neu::file::ReadTextFile("shaders/basic.frag", fs_source);
    const char* fs_cstr = fs_source.c_str();

    GLuint fs;
	fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_cstr, NULL);
    glCompileShader(fs);

    //Check for compile errors
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(fs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }


    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    //check for linking errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetProgramInfoLog(program, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Program link failed: {}", infoLog);
    }

    glUseProgram(program);

    //uniform
    GLint uniform= glGetUniformLocation(program, "u_time");
    ASSERT(uniform != -1);

    // loc_time
    int loc_time = glGetUniformLocation(program, "u_time");
    ASSERT_MSG(loc_time != -1, "Could not find uniform u_time.");

    /*
    std::vector<neu::vec3> velocities;
	//random velocities for last 7 points
    for(int i=0;i<7;i++)
		velocities.push_back({ neu::random::getReal(-0.00025f,0.00025f), neu::random::getReal(-0.00025f,0.00025f),0 }
    );
    */
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

        glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());
        glUniform1f(loc_time, neu::GetEngine().GetTime().GetTime());

        // draw
        neu::vec3 color{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();

        //old code before shaders
        /*
        glLoadIdentity();
        
        //mouse triangle, it looks like a isosceles triangle
        glPushMatrix();
        
        //follow mouse code
        neu::InputSystem input = neu::GetEngine().GetInput();
        neu::vec2 mouse= input.GetMousePosition();
        float width =neu::GetEngine().GetRenderer().GetWidth()*0.5;
        float height =neu::GetEngine().GetRenderer().GetHeight()*0.5;
        glTranslatef(( -width +mouse.x) / width, (height - mouse.y) / height, 0);

        //rotation based on time but faster
        int rotation = 300*neu::GetEngine().GetTime().GetTime();

        glRotatef(rotation, 0, 0, 1);
        
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 3; i++) {
            
			//Changing colors based on rotation and index, modulus is to keep in range
            //Specifically rotation / index (for difference between points)
            //(result) mod 100 then divided to get back to 0-1 range while keeping the color change smooth
			//modulus values are different for each color to keep them from being the same
			//Division values are slightly lower to let colors go slightly above 1.0f for a bit
            glColor3f(colors[i].r + (rotation / ((i+1)*11) % 100) / 95.0f, colors[i].g+(rotation / ((i + 1) * 12) % 110) / 110.0f, colors[i].b+ (rotation / ((i + 1) * 10) % 100) / 90.0f);
            glVertex3f(points[i].x,points[i].y,points[i].z);

        }
        glEnd();
        glPopMatrix();

        //corner rectangle, its a square in it's coordinates, 
        // but looks like a rectangle due to the aspect ratio
        glPushMatrix();
		//move to corner based on time
        if ((int)roundf(neu::GetEngine().GetTime().GetTime())%4 == 0) {
            glTranslatef(0.5f,0.5f,0);

        }else if ((int)roundf(neu::GetEngine().GetTime().GetTime())%4 == 1) {
            glTranslatef(0.5f,-0.5f,0);

        }else if ((int)roundf(neu::GetEngine().GetTime().GetTime())%4 == 2) {
            glTranslatef(-0.5f,-0.5f,0);

        }else if ((int)roundf(neu::GetEngine().GetTime().GetTime())%4 == 3) {
            glTranslatef(-0.5f,0.5f,0);

        }
        
        //Scale based on sine of time
		glScalef(sinf(neu::GetEngine().GetTime().GetTime()), sinf(neu::GetEngine().GetTime().GetTime()), sinf(neu::GetEngine().GetTime().GetTime()));
        glBegin(GL_QUADS);
        for (int i = 3; i < 7; i++) {
            //Changing colors based on time and index, modulus is to keep in range
            //Specifically Time * speed(for faster change) * index (for difference between points)
            //(result) mod 10 then divided to get back to 0-1 range while keeping the color change smooth
            glColor3f(((int)(colors[i].r + (neu::GetEngine().GetTime().GetTime()) * 4*(i)) % 10) / 10.0f,
                ((int)(colors[i].g + (neu::GetEngine().GetTime().GetTime()) * 2*(i/2)) % 10) / 10.0f,
                ((int)(colors[i].b + (neu::GetEngine().GetTime().GetTime()) * 3*(i/3)) % 10) / 10.0f);
			//moving vertices based on sine of time and index
            glVertex3f(points[i].x+ sinf(neu::GetEngine().GetTime().GetTime()*4)/(i),
            points[i].y+ sinf(neu::GetEngine().GetTime().GetTime()*5)/(2*i),
                points[i].z);

        }
		glEnd();
        glPopMatrix();
        */
		//moving triangle strip, look similar to the polygon sta but it has more visible folds
        /* 
        * The reason the indexes are 17-24 is because I wanted this to 
        * layer/show above the star in the center, but I made the star earlier 
        * which gave the star lower indexes
        */
        /*
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 17; i < 24; i++) {
			//update position based on velocity, and kept points in bounds via reversing velocity
			//Thank you github copilot for helping with this bit of code
            for (int j = 0; j < 3; j++) {
                points[i].x += velocities[i - 17].x;
                points[i].y += velocities[i - 17].y;
                if (points[i].x > 1 || points[i].x < -1) velocities[i - 17].x = -velocities[i - 17].x;
                if (points[i].y > 1 || points[i].y < -1) velocities[i - 17].y = -velocities[i - 17].y;
            }
            //End of code helped
            //Color based on position and base color
            glColor3f(colors[i].r+ points[i].x, colors[i].g + points[i].y, colors[i].b + (points[i].x/2 + points[i].y/2));
            glVertex3f(points[i].x, points[i].y, points[i].z);

        }
        glEnd();

		//center star/polygon, simplistic polygon but looks like a star due to the shape
        glBegin(GL_POLYGON);
        for (int i = 7; i < 17; i++) {
            //Changing colors based on time, index and base color, modulus is to keep in range
            //Specifically Time * speed(for faster change) * index (for difference between points)
			//(result) mod 100 then divided to get back to 0-1 range while keeping the color change smooth
            //modulus values are different for each color to keep them from being the same
            glColor3f(((int)(colors[i].r+ (neu::GetEngine().GetTime().GetTime())*20)%100)/100.0f,
                ((int)(colors[i].g +(neu::GetEngine().GetTime().GetTime()) * 30) % 100)/100.0f,
                ((int)(colors[i].b + (neu::GetEngine().GetTime().GetTime()) * 40) % 100)/100.0f);
            glVertex3f(points[i].x, points[i].y, points[i].z);

        }
        glEnd();
        */
       
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)points.size()); //mouse triangle


        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
