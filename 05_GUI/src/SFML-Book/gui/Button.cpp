#include <SFML-Book/gui/Button.hpp>

namespace book
{
    namespace gui
    {
        Button::FuncType Button::defaultFunc = [](const sf::Event&,Button&)->void{};

        Button::Button(Widget* parent) : Widget(parent), on_click(defaultFunc)
        {
        }

        Button::~Button()
        {
        }

        bool Button::processEvent(const sf::Event& event,const sf::Vector2f& parent_pos)
        {
            bool res = false;
            if(event.type == sf::Event::MouseButtonReleased)
            {
                const sf::Vector2f pos = _position + parent_pos;
                const sf::Vector2f size = getSize();
                sf::FloatRect rect;

                rect.left = pos.x;
                rect.top = pos.y;
                rect.width = size.x;
                rect.height = size.y;

                if(rect.contains(event.mouseButton.x,event.mouseButton.y))
                {
                    on_click(event,*this);
                    res = true;
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                const sf::Vector2f pos = _position + parent_pos;
                const sf::Vector2f size = getSize();
                sf::FloatRect rect;

                rect.left = pos.x;
                rect.top = pos.y;
                rect.width = size.x;
                rect.height = size.y;

                int old_status = _status;
                _status = 0;

                const sf::Vector2f mouse_pos(event.mouseMove.x,event.mouseMove.y);
                if(rect.contains(mouse_pos))
                {
                    _status|=Status::Hover;
                }

                if(old_status != _status)
                    repaint();

            }
            return res;
        }
    }
}