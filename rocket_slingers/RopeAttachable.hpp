#ifndef ROPEATTACHABLE_HPP
#define ROPEATTACHABLE_HPP

#include <Box2D/Box2D.h>

class RopeAttachable {
public:
	bool useRopeAttachmentAdjustmentPoint = false;
	b2Vec2 ropeAttachmentAdjustmentPoint;
};

#endif
