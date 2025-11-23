#include "../Engine/Renderer/Shader.h"
#include "../Engine/Renderer/Program.h"

int main(int argc, char* argv[]) {
	float offsets[9] = { 1,2,3,4,5,6,7,8,9 };
	float kernel[9] = { 1,2,3,4,5,6,7,8,9 };
	glm::mat3 kernelm = { 1,2,3,4,5,6,7,8,9};
	glm::vec4 sum = glm::vec4(0.0);
	glm::vec4 sumM = glm::vec4(0.0);

	int offset = 0;
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			sum +=offsets[offset] * kernel[offset];
			std::cout << "kernel[offset]: " << kernel[offset] << std::endl;

			sumM +=offsets[offset] * kernelm[row][column];
			std::cout << "kernelm[row][column]: " << kernelm[row][column] << std::endl;
			offset++;
		}
	}
	std::cout << "kernel[offset]: " << sum.x<<", "<< sum.y<<" "<< sum.z << std::endl;
	std::cout << "kernelM[row][c]: " << sumM.x<<", "<< sumM.y<<" "<< sumM.z << std::endl;

	


	neu::file::SetCurrentDirectory("Assets");
	LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

	// initialize engine
	LOG_INFO("initialize engine...");
	neu::GetEngine().Initialize();
	SDL_Event e;
	bool quit = false;
	
	//create render textures
	auto renderTexture = std::make_shared<neu::RenderTexture>();
	renderTexture->Create(512, 512);
	neu::Resources().AddResource("renderTexture", renderTexture);


	renderTexture = std::make_shared<neu::RenderTexture>();
	renderTexture->Create(1024, 1024);
	neu::Resources().AddResource("postProcessTexture", renderTexture);

	//Init scene
	auto scene = std::make_unique<neu::Scene>();
	scene->Load("scenes/scene01.json");
	scene->Start();

	auto editor = std::make_unique<neu::Editor>();	
	
	// MAIN LOOP
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				quit = true;
			}
			ImGui_ImplSDL3_ProcessEvent(&e);
		}

		// update
		neu::GetEngine().Update();
		float dt= neu::GetEngine().GetTime().GetDeltaTime() ;
		if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;
		
		scene->Update(dt);
		
		editor->Begin();
		editor->UpdateGui(*scene);
		
		// draw

		scene->Draw(neu::GetEngine().GetRenderer());
		// draw ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		
		neu::GetEngine().GetRenderer().Present();
	}
	neu::GetEngine().Shutdown();
	return 0;
}
