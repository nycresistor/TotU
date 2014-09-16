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
				((sideY / screenRows) * y) + (sideY / screenRows) / 3, sideZ]) screenFrame();
			}
		}
	}
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

torso();


