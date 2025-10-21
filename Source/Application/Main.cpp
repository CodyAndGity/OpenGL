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
	
	auto vs = neu::Resources().Get<neu::Shader>("shaders/basic.vert", GL_VERTEX_SHADER);
	auto fs = neu::Resources().Get<neu::Shader>("shaders/basic.frag", GL_FRAGMENT_SHADER);
	

	

	

	auto program = std::make_shared<neu::Program>();
	program->AttachShader(vs);
	program->AttachShader(fs);
	program->Link();
	program->Use();

	//texture
	neu::res_t<neu::Texture> texture = neu::Resources().Get < neu::Texture>("textures/beast.png");

	float rotation = 0;

	
	program->SetUniform("u_texture",0);
	glm::vec3 eye{ 0,0,3 };
	
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

		if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

		glm::mat4 model = glm::mat4(1.0f);//identity matrix

		rotation = neu::GetEngine().GetTime().GetTime()*90;
		//model matrix
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.4f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		program->SetUniform("u_model", model);

		program->SetUniform("u_time", neu::GetEngine().GetTime().GetTime());
		//program->SetUniform("loc_time", neu::GetEngine().GetTime().GetTime());
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) {
			eye.x +=  0.001f;
		}
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) {
			eye.x -=  0.001f;
		}
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) {
			eye.z -=  0.001f;
		}
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) {
			eye.z +=  0.001f;
		}

		
		//view matrix
		glm::mat4 view = glm::lookAt(eye, eye + glm::vec3{ 0,0,-1 }, glm::vec3{ 0,1,0 });
		program->SetUniform("u_view", view);
		
		
		
		// draw

		neu::GetEngine().GetRenderer().Clear();
		
		model3d->Draw(GL_TRIANGLES);

		
		
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

		neu::GetEngine().GetRenderer().Present();
	}

	neu::GetEngine().Shutdown();

	return 0;
}
