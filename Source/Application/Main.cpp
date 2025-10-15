#include "../Engine/Renderer/Shader.h"
#include "../Engine/Renderer/Program.h"
int main(int argc, char* argv[]) {
	neu::file::SetCurrentDirectory("Assets");
	LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

	// initialize engine
	LOG_INFO("initialize engine...");
	neu::GetEngine().Initialize();

	// OPEN GL initialization
	std::vector<neu::vec3> points{

		{-0.5f, -0.5f, 0},
		{0.5f, -0.5f, 0},
		{0.5f, 0.5f, 0},
		{-0.5f, 0.5f, 0}
		


	};
	std::vector<neu::vec3> colors{
		//triangle
		{1,1,0},
		{0,1,1},
		{1,0,1},
		{1,1,1},

		
	};
	std::vector<neu::vec2> texcoord{ {0,0},
		{1,0},
		{1,1},
		{0,1} };

	struct Vertex {
		neu::vec3 points;
		neu::vec3 colors;
		neu::vec2 texcoord;
	};
	std::vector<Vertex> vertices{
		{{-0.5f, -0.5f, 0},
		{1,1,0},
		{0,0}},

		{{0.5f, -0.5f, 0},
		{0,1,1},
		{1,0},},

		{{0.5f, 0.5f, 0},
		{1,0,1},
		{1,1}},



	{ { -0.5f, 0.5f, 0 },
		{0,0,1},
		{0,1},}

	};
	std::vector<GLuint> indices{ 0,1,2, 2,3,0 };
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* vertices.size(), vertices.data(), GL_STATIC_DRAW);

	//index array
	GLuint ibo;
	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* indices.size(), indices.data(), GL_STATIC_DRAW);


	//vertex array
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, points));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, colors));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	/*
	GLuint vbo[3];
	glGenBuffers(3, vbo);

	//vertex buffer (position)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * points.size(), points.data(), GL_STATIC_DRAW);

	//vertex buffer (color)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);

	//vertex buffer (textCords)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec2) * texcoord.size(), texcoord.data(), GL_STATIC_DRAW);


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

	//texcoord
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	*/

	//vertex shader
	std::string vs_source;
	neu::file::ReadTextFile("shaders/basic.vert", vs_source);
	const char* vs_cstr = vs_source.c_str();

	/*GLuint vs;
	vs = glCreateShader(GL_VERTEX_SHADER);*/
	
	auto vs = neu::Resources().Get<neu::Shader>("shaders/basic.vert", GL_VERTEX_SHADER);
	auto fs = neu::Resources().Get<neu::Shader>("shaders/basic.frag", GL_FRAGMENT_SHADER);
	

	

	

	auto program = std::make_shared<neu::Program>();
	program->AttachShader(vs);
	program->AttachShader(fs);
	program->Link();
	program->Use();

	//texture
	neu::res_t<neu::Texture> texture = neu::Resources().Get < neu::Texture>("textures/beast.png");


	
	program->SetUniform("u_texture",0);

	
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
		program->SetUniform("u_time", neu::GetEngine().GetTime().GetTime());
		//program->SetUniform("loc_time", neu::GetEngine().GetTime().GetTime());

		
		
		// draw

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
		//moving triangle strip, look similar to the polygon star but it has more visible folds
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
		//glDrawArrays(GL_TRIANGLES, 0, (GLsizei)points.size()); //mouse triangle
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

		neu::GetEngine().GetRenderer().Present();
	}

	neu::GetEngine().Shutdown();

	return 0;
}
