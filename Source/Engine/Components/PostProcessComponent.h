#pragma once

namespace neu {
	class PostProcessComponent : public Component {
	public:
		enum class Parameters : uint32_t {
			None = 0,
			GrayScale = (1 << 0),
			ColorTint= (1 << 1),
			ScanLine= (1 << 2),
			Grain= (1 << 3),
			Invert= (1 << 4),
			EdgeDetection= (1 << 5),
		};

	public:
		CLASS_PROTOTYPE(PostProcessComponent)

		void Update(float dt) override;
		void Apply(Program& program);

		void Read(const serial_data_t& value);
		void UpdateGui() override;

	public:
		Parameters parameters = Parameters::None;
		glm::vec3 colorTint{ 0,0,1 };
		float blend{ 1.0f };
		glm::mat3 kernel = { -20, -1, 0,
							-20,  63, -1,
							-20, -1, 0 };
	};
}