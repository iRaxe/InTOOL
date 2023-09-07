#include "stdafx.h"
#include "Rect.h"

namespace UPasta {
	Rect Rect::Empty = Rect();

	Rect::Rect() {
		this->Position = Vector2(0, 0);
		this->Width = 0.0f;
		this->Height = 0.0f;
	}

	Rect::Rect(Vector2 position, float width, float height)
	{
		this->Position = position;
		this->Width = width;
		this->Height = height;
	}

	Rect::Rect(float x, float y, float width, float height) {
		this->Position = Vector2(x, y);
		this->Width = width;
		this->Height = height;
	}

	bool Rect::Contains(Vector2 point) {
		return point.x >= this->Position.x && point.x < this->Position.x + this->Width && point.y >= this->Position.y && point.y < this->Position.y + this->Height;
	}

	bool Rect::Contains(Rect rect) {
		return rect.Position.x >= this->Position.x && rect.Position.x + rect.Width <= this->Position.x + this->Width && rect.Position.y >= this->Position.y && rect.Position.y + rect.Height <= this->Position.y + this->Height;
	}

	bool Rect::Intersects(Rect rect, Rect* out) {
		auto newLeft = max(this->Position.x, rect.Position.x);
		auto newTop = max(this->Position.y, rect.Position.y);
		auto newRight = min(this->Position.x + this->Width, rect.Position.x + rect.Width);
		auto newBottom = min(this->Position.y + this->Height, rect.Position.y + rect.Height);

		if (newRight > newLeft && newBottom > newTop) {
			if (out) {
				*out = Rect(newLeft, newTop, newRight - newLeft, newBottom - newTop);
			}
			return true;
		}

		return false;
	}

	Rect Rect::Union(Rect rect) {
		auto left = min(this->Position.x, rect.Position.x);
		auto top = min(this->Position.y, rect.Position.y);
		auto right = max(this->Position.x + this->Width, rect.Position.x + rect.Width);
		auto bottom = max(this->Position.y + this->Height, rect.Position.y + rect.Height);
		return Rect(left, top, right - left, bottom - top);
	}

	bool Rect::operator==(Rect rect) {
		return this->Position.x == rect.Position.x && this->Position.y == rect.Position.y && this->Width == rect.Width && this->Height == rect.Height;
	}

	bool Rect::operator!=(Rect rect) {
		return this->Position.x != rect.Position.x || this->Position.y != rect.Position.y || this->Width != rect.Width || this->Height != rect.Height;
	}
}