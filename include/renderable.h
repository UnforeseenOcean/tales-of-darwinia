#ifndef RENDERABLE_H_
#define RENDERABLE_H_

class IRenderable {
	public:
		bool hidden;
		virtual ~IRenderable {};
		virtual void draw(void) = 0;
};
#endif
