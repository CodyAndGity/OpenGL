#include "../Engine/Renderer/Shader.h"
#include "../Engine/Renderer/Program.h"
#include <imgui.h>
int main(int argc, char* argv[]) {
	neu::file::SetCurrentDirectory("Assets");
	LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

	// initialize engine
	LOG_INFO("initialize engine...");
	neu::GetEngine().Initialize();

	

	
	auto model3d = std::make_shared<neu::Model>();
	model3d->Load("models/spot.obj");

	
	
	
	//vertex shader
	std::string vs_source;
	neu::file::ReadTextFile("shaders/basic.vert", vs_source);
	const char* vs_cstr = vs_source.c_str();

	/*GLuint vs;
	vs = glCreateShader(GL_VERTEX_SHADER);*/
	
	
	
	auto program = neu::Resources().Get<neu::Program>("shaders/basic_lit.prog");
	
	program->Use();

	//texture
	neu::res_t<neu::Texture> texture = neu::Resources().Get < neu::Texture>("textures/cow.jpg");

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
		float dt= neu::GetEngine().GetTime().GetDeltaTime() ;
		if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

		glm::mat4 model = glm::mat4(1.0f);//identity matrix

		
		
		transform.rotation.y += 9*dt;
		program->SetUniform("u_model", transform.GetMatrix());

		program->SetUniform("u_time", neu::GetEngine().GetTime().GetTime());
		//program->SetUniform("loc_time", neu::GetEngine().GetTime().GetTime());
		
		
		

		float speed = 5.0f;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x -= speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.z -= speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.z += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_Q)) camera.position.y -= speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_E)) camera.position.y += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_UP)) camera.rotation.x += speed * 10 * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_DOWN)) camera.rotation.x -= speed*10 * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_LEFT)) camera.rotation.y += speed * 10 * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_RIGHT)) camera.rotation.y -= speed * 10 * dt;
		//view matrix
		glm::vec3 forward = glm::normalize(glm::vec3(camera.GetMatrix() * glm::vec4{ 0, 0, -1, 0 }));
		glm::mat4 view = glm::lookAt(camera.position, camera.position + forward, glm::vec3{0,1,0});
		program->SetUniform("u_view", view);
		
		program->SetUniform("u_light.color",glm::vec3(0,0,4));
		light.position.y = neu::math::sin(neu::GetEngine().GetTime().GetTime() * 3)*2;
		program->SetUniform("u_light.position",glm::vec3(view*glm::vec4(light.position,1)));

		
		// draw

		neu::GetEngine().GetRenderer().Clear();
		// start new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		// set ImGui
		ImGui::Begin("Editor");
		ImGui::Text("Hello World");
		ImGui::Text("Press 'Esc' to quit.");
		ImGui::End();

		model3d->Draw(GL_TRIANGLES);

		// draw ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		
		

		neu::GetEngine().GetRenderer().Present();
	}

	neu::GetEngine().Shutdown();

	return 0;
}
