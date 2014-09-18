// All dimensions in mm
screenX = 70.85;			// Screen X dimension (horizontal)
screenY = 55.04; 		// Screen Y dimension
screenZ = 3.6;			// Screen thickness
frameThickness = 4;		// Thickness of the 3D printed frame

sideX = 609.6;			// X dimesion of the cabinet side (2 ft)
sideY = 1219.2;			// Y dimension of the cabinet side (4 ft)
sideZ = 6.35;			// Thickness of the cabinet side

cabinetSpacing = 50.8; 	// Internal space inside the cabinet, for hardware etc. (2 in)

screenColumns = 4;		// Number of columns of LCDs
screenRows = 8;			// Number of rows of LCDs

numberOfSides = 8;		// Number of cabinets

legsHeight = sideY * .75;		// Height of the legs that support the cabinets

radius = 3048; 			// Radius of the cabinets from the center (10 ft)

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
	// Draws a Beige temple side panel with screen rows and columns
	
	rotate([90, 0, 270]) {
	
		color("Beige") {
			cube([sideX, sideY, sideZ]);
			translate([0, 0, -cabinetSpacing]) cube([sideX, sideY, sideZ]);
			rotate([0, 90, 0]) translate([0, 0, 0]) cube([cabinetSpacing - sideZ, sideY, sideZ]);
			rotate([0, 90, 0]) translate([0, 0, sideX - sideZ]) cube([cabinetSpacing - sideZ, sideY, sideZ]);
			rotate([90, 90, 0]) translate([0, sideZ, -sideY]) cube([cabinetSpacing - sideZ , sideX - (2 * sideZ), sideZ]);
		}

		for (y = [0 : screenRows - 1]) {
			for (x = [0 : screenColumns - 1]) {	
	
				// This algo needs work but it seems to work ok for 3 x 8
				translate([((sideX / screenColumns) * x) + (sideX / screenColumns) / screenColumns, 
				((sideY / screenRows) * y) + (sideY / screenRows) / 3, sideZ + 5]) screenFrame();
				
				translate([((sideX / screenColumns) * x) + (sideX / screenColumns) / screenColumns, 
				((sideY / screenRows) * y) + (sideY / screenRows) / 1.5, -cabinetSpacing - 5]) rotate([180, 0, 0]) screenFrame();
			}
		}
	}
}

module leg() {
	// Draws each leg

	rotate([90, 0, 270]) {

		color("Beige") {
			cube([sideX, legsHeight, sideZ]);
			translate([0, 0, -cabinetSpacing]) cube([sideX, legsHeight, sideZ]);
			rotate([0, 90, 0]) translate([0, 0, 0]) cube([cabinetSpacing - sideZ, legsHeight, sideZ]);
			rotate([0, 90, 0]) translate([0, 0, sideX - sideZ]) cube([cabinetSpacing - sideZ, legsHeight, sideZ]);
		}
	}
}

module legs() {
	// Draws the Legs that support the LCD cabinets
	
	rotationAngle = 360 / numberOfSides;
	halfAngle = rotationAngle / 2;
	
	for (side = [0 : numberOfSides - 1]) {
			translate([-(cos((rotationAngle * side)) * radius), sin((rotationAngle * side)) * radius, 0]) rotate([0, 0, -(47 * side)]) leg();
	}
	
}

module torso() {
	// Draws the cabinets that hold the screens

	rotationAngle = 360 / numberOfSides;
	halfAngle = rotationAngle / 2;
	
	for (side = [0 : numberOfSides - 1]) {
			translate([-(cos((rotationAngle * side)) * radius), sin((rotationAngle * side)) * radius, 0]) rotate([0, 0, -(47 * side)]) side();
	}
}

module person() {
	// Draws a 6 ft person for scale

	translate([-300, -100, 0]) {	
		cube([610, 200, 1830]);
		translate([300, 100, 1800]) sphere(200);
	}
}


// Main

// Draw the Playa
translate([-5000, -5000, -1]) color("DarkKhaki") cube([10000, 10000, 1]);

// Draw the LCD cabinets
translate([0, 0, legsHeight + 5]) torso();

// Draw the legs
translate([0, 0, 0]) legs();

// Person to scale
person();

