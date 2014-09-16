// All dimensions in mm
screenX = 70.85;
screenY = 55.04;
screenZ = 3.6;
frameThickness = 4;

sideX = 609.6;
sideY = 1219.2;
sideZ = 6.35;

screenColumns = 3;
screenRows = 8;

numberOfSides = 8;

module screen() {
	// Draws a black screen box
	color("Black") cube([screenX, screenY, screenZ]);
}

module screenFrame(pos) {
	// Draws a red frame around a yellow screen
	screen();
	translate([-frameThickness, -frameThickness, 0]) color("Red") cube([frameThickness, screenY + (2 * frameThickness), screenZ]);
	translate([screenX, -frameThickness, 0]) color("Red") cube([frameThickness, screenY + (2 * frameThickness), screenZ]);
	translate([0, -frameThickness, 0]) color("Red") cube([screenX, frameThickness, screenZ]);
	translate([0, screenY, 0]) color("Red") cube([screenX, frameThickness, screenZ]);
	translate([-frameThickness, -frameThickness, -screenZ]) color("Red") cube([screenX + (2 * frameThickness), screenY + (2 * frameThickness), screenZ]);
	
}

module side() {
	// Draws a Beigh temple side panel with screen rows and columns
	
	rotate([90, 0, 270]) {
	
		color("Beige") cube([sideX, sideY, sideZ]);
	
		for (y = [0 : screenRows - 1]) {
			for (x = [0 : screenColumns - 1]) {	
	
				// This algo needs work but it seems to work ok for 3 x 8
				translate([((sideX / screenColumns) * x) + (sideX / screenColumns) / screenColumns, 
				((sideY / screenRows) * y) + (sideY / screenRows) / 3, sideZ + 1]) screenFrame();
			}
		}
	}
}


/*
Trapezoid
	Create a Basic Trapezoid (Based on Isosceles_Triangle)

            d
          /----\
         /  |   \
     a  /   H    \ c
       /    |     \
 angle ------------ angle
            b

	b: Length of side b
	angle: Angle at points angleAB & angleBC
	H: The 2D height at which the triangle should be cut to create the trapezoid
	heights: If vector of size 3 (Standard for triangles) both cd & da will be the same height, if vector have 4 values [ab,bc,cd,da] than each point can have different heights.
*/
module Trapezoid(
			b, angle=60, H, height=1, heights=undef,
			center=undef, centerXYZ=[true,false,false])
{
	validAngle = (angle < 90);
	adX = H / tan(angle);

	// Calculate Heights at each point
	heightAB = ((heights==undef) ? height : heights[0])/2;
	heightBC = ((heights==undef) ? height : heights[1])/2;
	heightCD = ((heights==undef) ? height : heights[2])/2;
	heightDA = ((heights==undef) ? height : ((len(heights) > 3)?heights[3]:heights[2]))/2;

	// Centers
	centerX = (center || (center==undef && centerXYZ[0]))?0:b/2;
	centerY = (center || (center==undef && centerXYZ[1]))?0:H/2;
	centerZ = (center || (center==undef && centerXYZ[2]))?0:max(heightAB,heightBC,heightCD,heightDA);

	// Points
	y = H/2;
	bx = b/2;
	dx = (b-(adX*2))/2;

	pointAB1 = [centerX-bx, centerY-y, centerZ-heightAB];
	pointAB2 = [centerX-bx, centerY-y, centerZ+heightAB];
	pointBC1 = [centerX+bx, centerY-y, centerZ-heightBC];
	pointBC2 = [centerX+bx, centerY-y, centerZ+heightBC];
	pointCD1 = [centerX+dx, centerY+y, centerZ-heightCD];
	pointCD2 = [centerX+dx, centerY+y, centerZ+heightCD];
	pointDA1 = [centerX-dx, centerY+y, centerZ-heightDA];
	pointDA2 = [centerX-dx, centerY+y, centerZ+heightDA];

	validH = (adX < b/2);

	if (validAngle && validH)
	{
		polyhedron(
			points=[	pointAB1, pointBC1, pointCD1, pointDA1,
						pointAB2, pointBC2, pointCD2, pointDA2 ],
			triangles=[	
				[0, 1, 2],
				[0, 2, 3],
				[4, 6, 5],
				[4, 7, 6],
				[0, 4, 1],
				[1, 4, 5],
				[1, 5, 2],
				[2, 5, 6],
				[2, 6, 3],
				[3, 6, 7],
				[3, 7, 0],
				[0, 7, 4]	] );
	} else {
		if (!validAngle) echo("Trapezoid invalid, angle must be less than 90");
		else echo("Trapezoid invalid, H is larger than triangle");
	}
}

module leg() {
	
	rotate([0, 0, 270]) color("Beige") Trapezoid(b=sideX, angle=-86, H=sideY / 2, height=sideZ, centerXYZ=[false,false,true]);
}

module torso() {
	
	circumRadius = sideX / (2 * sin(180 / numberOfSides));
	//inRadius = sideX / (2 * tan(180 / numberOfSides));
	rotationAngle = 360 / numberOfSides;
	halfAngle = rotationAngle / 2;
	
	for (side = [0 : numberOfSides - 1]) {
			
		translate([-(cos((rotationAngle * side) + halfAngle) * circumRadius), sin((rotationAngle * side) + halfAngle) * circumRadius, 0]) 
			rotate([0, 0, -(rotationAngle * side)]) side();
	}
}

module legs() {
	
	circumRadius = sideX / (2 * sin(180 / numberOfSides));
	//inRadius = sideX / (2 * tan(180 / numberOfSides));
	rotationAngle = 360 / numberOfSides;
	halfAngle = rotationAngle / 2;
	
	for (side = [0 : numberOfSides - 1]) {
			
		translate([-(cos((rotationAngle * side) + halfAngle) * circumRadius), sin((rotationAngle * side) + halfAngle) * circumRadius, 0]) 
			rotate([0, 100, -(rotationAngle * side)]) leg();
	}
	
}

torso();
translate([0, 0, -5]) legs();


