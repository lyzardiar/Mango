#include "SceneView.h"
#include "imgui/imgui.h"
#include "renderer/TriangleRenderer.h"
#include "core/image/PngDecoder.h"
#include "renderer/FrameBuffer.h"


namespace RE {

	GLuint testTexture() {
		static Texture2D* tex = nullptr;
		if (tex == nullptr) {
			auto imgInfo = Png::Decode("images/1.png");

			tex = new Texture2D(imgInfo.width, imgInfo.height, imgInfo.data.getBytes(), imgInfo.data.getSize());
		}
		return tex->GetHandle();
	}

	void SceneView::OnGUI() {
		auto tex = testTexture();
		
		static FrameBuffer fbo;

		bool is_opened = true;
		if (ImGui::BeginDock("Scene", &is_opened, ImGuiWindowFlags_NoScrollWithMouse)) {
			bool open = true;
			//ImGui::ShowTestWindow(&open);
			auto size = ImGui::GetContentRegionAvail();

			fbo.Begin({ 0, 0, (int)size.x, (int)size.y });
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			fbo.End();

			ImGui::Image((GLuint*)fbo.GetTextureHandle(), size, ImVec2(0, 1), ImVec2(1, 0));

			//static TriangleRenderer* r = new TriangleRenderer();
			//r->draw();
		}
		ImGui::EndDock();
	}

}
