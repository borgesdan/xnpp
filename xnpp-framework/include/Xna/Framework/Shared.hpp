#ifndef XNA_FRAMEWORK_SHARED_HPP
#define XNA_FRAMEWORK_SHARED_HPP

namespace Xna {
	//Specifies different tangent types to be calculated for CurveKey points in a Curve. 
	enum class CurveTangent
	{
		//A Flat tangent always has a value equal to zero
		Flat,
		//A Linear tangent at a CurveKey is equal to the difference between its Value and the Value of the preceding or succeeding CurveKey.
		Linear,
		//A Smooth tangent smooths the inflection between a TangentIn and TangentOut by taking into account the values of both neighbors of the CurveKey.
		Smooth,
	};

	//Defines how the value of a Curve will be determined for positions before the first point on the Curve or after the last point on the Curve. 
	enum class CurveLoopType {
		//The Curve will evaluate to its first key for positions before the first point in the Curve and to the last key for positions after the last point.
		Constant,
		//Positions specified past the ends of the curve will wrap around to the opposite side of the Curve.
		Cycle,
		//Positions specified past the ends of the curve will wrap around to the opposite side of the Curve. The value will be offset by the difference between the values of the first and last CurveKey multiplied by the number of times the position wraps around. If the position is before the first point in the Curve, the difference will be subtracted from its value; otherwise, the difference will be added. 
		CycleOffset,
		//Positions specified past the ends of the Curve act as an offset from the same side of the Curve toward the opposite side.
		Oscillate,
		//Linear interpolation will be performed to determine the value.
		Linear,
	};

	//Defines the continuity of CurveKeys on a Curve. 
	enum class CurveContinuity {
		//Interpolation can be used between this CurveKey and the next.
		Smooth,
		//Interpolation cannot be used between this CurveKey and the next. Specifying a position between the two points returns this point.
		Step,
	};

	//Indicates the extent to which bounding volumes intersect or contain one another. 
	enum class ContainmentType {
		//Indicates there is no overlap between the bounding volumes.
		Disjoint,
		//Indicates that one bounding volume completely contains the other.
		Contains,
		//Indicates that the bounding volumes partially overlap.
		Intersects,
	};

	//Defines the display orientation.
    enum class DisplayOrientation
    {
		//The default orientation.
        Default = 0,
		//The display is rotated counterclockwise into a landscape orientation. Width is greater than height.
        LandscapeLeft = 1,
		//The display is rotated clockwise into a landscape orientation. Width is greater than height.
        LandscapeRight = 2,
		//The display is rotated as portrait, where height is greater than width.
        Portrait = 4,
    };

	//Describes the intersection between a plane and a bounding volume.
	enum class PlaneIntersectionType {
		//There is no intersection, and the bounding volume is in the positive half-space of the Plane.
		Front,
		//There is no intersection, and the bounding volume is in the negative half - space of the Plane.
		Back,
		//The Plane is intersected.
		Intersecting,
	};
}

#endif