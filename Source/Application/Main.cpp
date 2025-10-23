#include "../Engine/Renderer/Shader.h"
#include "../Engine/Renderer/Program.h"
int main(int argc, char* argv[]) {
	neu::file::SetCurrentDirectory("Assets");
	LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

	// initialize engine
	LOG_INFO("initialize engine...");
	neu::GetEngine().Initialize();

	

	
	auto model3d = std::make_shared<neu::Model>();
	model3d->Load("models/sphere.obj");

	
	
	
	//vertex shader
	std::string vs_source;
	neu::file::ReadTextFile("shaders/basic.vert", vs_source);
	const char* vs_cstr = vs_source.c_str();

	/*GLuint vs;
	vs = glCreateShader(GL_VERTEX_SHADER);*/
	
	auto vs = neu::Resources().Get<neu::Shader>("shaders/basic_lit.vert", GL_VERTEX_SHADER);
	auto fs = neu::Resources().Get<neu::Shader>("shaders/basic_lit.frag", GL_FRAGMENT_SHADER);
	

	

	

	auto program = std::make_shared<neu::Program>();
	program->AttachShader(vs);
	program->AttachShader(fs);
	program->Link();
	program->Use();

	//texture
	neu::res_t<neu::Texture> texture = neu::Resources().Get < neu::Texture>("textures/beast.png");

	float rotation = 0;

	
	program->SetUniform("u_texture",0);

	//light
	program->SetUniform("u_ambient_light",glm::vec3(0.2f));
	
	neu::Transform light{ {2,4,3}  };
	
	neu::Transform transform{ {0,0,0}  };
	neu::Transform camera{ {0,0,5} ,{0,0,-1},{1,1,1} };

	SDL_Event e;
	bool quit = false;
	
	//projection matrix
	float aspect = neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
	program->SetUniform("u_projection", projection);
	// MAIN LOOP
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				quit = true;
			}
		}

		// update
		neu::GetEngine().Update();
		float dt= neu::GetEngine().GetTime().GetTime() * 90;
		if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

		glm::mat4 model = glm::mat4(1.0f);//identity matrix

		
		
		transform.rotation.y += 90*dt;
		program->SetUniform("u_model", transform.GetMatrix());

		program->SetUniform("u_time", neu::GetEngine().GetTime().GetTime());
		//program->SetUniform("loc_time", neu::GetEngine().GetTime().GetTime());
		
		
		

		float speed = .0000050f;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x -= speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.z -= speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.z += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_Q)) camera.position.y -= speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_E)) camera.position.y += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_UP)) camera.rotation.y += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_DOWN)) camera.rotation.y -= speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_LEFT)) camera.rotation.x -= speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_RIGHT)) camera.rotation.x += speed * dt;
		//view matrix
		glm::mat4 view = glm::lookAt(camera.position, camera.position +camera.rotation, glm::vec3{0,1,0});
		program->SetUniform("u_view", view);
		
		program->SetUniform("u_light.color",glm::vec3(0,0,4));
		light.position.y = neu::math::sin(neu::GetEngine().GetTime().GetTime() * 3)*2;
		program->SetUniform("u_light.position",glm::vec3(view*glm::vec4(light.position,1)));

		
		// draw

		neu::GetEngine().GetRenderer().Clear();
		
		model3d->Draw(GL_TRIANGLES);

		
		
		

		neu::GetEngine().GetRenderer().Present();
	}

	neu::GetEngine().Shutdown();

	return 0;
}
