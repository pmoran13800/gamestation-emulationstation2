#pragma once

#include "GuiComponent.h"
#include "components/NinePatchComponent.h"
#include "components/ComponentGrid.h"
#include "resources/Font.h"

class TextComponent;
class ButtonComponent;
class ScrollableContainer;

class GuiMsgBoxScroll : public GuiComponent
{
public:
	GuiMsgBoxScroll(Window* window, const std::string& title, const std::string& text,
		const std::string& name1 = "OK", const std::function<void()>& func1 = nullptr,
		const std::string& name2 = "", const std::function<void()>& func2 = nullptr, 
		const std::string& name3 = "", const std::function<void()>& func3 = nullptr,
					Alignment align = ALIGN_CENTER,
					float size = 0);

	bool input(InputConfig* config, Input input) override;
	void onSizeChanged() override;
	std::vector<HelpPrompt> getHelpPrompts() override;

private:
	void deleteMeAndCall(const std::function<void()>& func);

	NinePatchComponent mBackground;
	ComponentGrid mGrid;

	std::shared_ptr<TextComponent> mTitle;
	std::shared_ptr<TextComponent> mMsg;
	std::vector< std::shared_ptr<ButtonComponent> > mButtons;
	std::shared_ptr<ComponentGrid> mButtonGrid;
	std::function<void()> mAcceleratorFunc;
	std::shared_ptr<ScrollableContainer> mMsgContainer;
};