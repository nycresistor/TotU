// All dimensions in mm
screenX = 70.85; // 2 ft
screenY = 55.04; // 4 ft
screenZ = 3.6;
frameThickness = 4;

supportDiameter = 152.4; // 6in

sideX = 609.6;
sideY = 1219.2;
sideZ = 6.35;

cabinetSpacing = 50.8; // 2 in

screenColumns = 4;
screenRows = 8;

numberOfSides = 8;

legsHeight = sideY / 2;

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
	
	rotate([90, 0, 270]) {

		color("Beige") {
			cube([sideX, sideY / 2, sideZ]);
			translate([0, 0, -cabinetSpacing]) cube([sideX, sideY / 2, sideZ]);
			rotate([0, 90, 0]) translate([0, 0, 0]) cube([cabinetSpacing - sideZ, sideY / 2, sideZ]);
			rotate([0, 90, 0]) translate([0, 0, sideX - sideZ]) cube([cabinetSpacing - sideZ, sideY / 2, sideZ]);
		}
	}
}

module torso() {
	circumRadius = (sideX / (2 * sin(180 / numberOfSides))) * 2;
	//inRadius = sideX / (2 * tan(180 / numberOfSides));
	rotationAngle = 360 / numberOfSides;
	halfAngle = rotationAngle / 2;

	sphere(100);
	
	for (side = [0 : numberOfSides - 1]) {
			translate([-(cos((rotationAngle * side)) * circumRadius), sin((rotationAngle * side)) * circumRadius, 0]) rotate([0, 0, -(47 * side)]) side();
	}
}

module legs() {
	
	circumRadius = (sideX / (2 * sin(180 / numberOfSides))) * 2;
	//inRadius = sideX / (2 * tan(180 / numberOfSides));
	rotationAngle = 360 / numberOfSides;
	halfAngle = rotationAngle / 2;
	
	for (side = [0 : numberOfSides - 1]) {
			translate([-(cos((rotationAngle * side)) * circumRadius), sin((rotationAngle * side)) * circumRadius, 0]) rotate([0, 0, -(47 * side)]) leg();
	}
	
}

translate([-5000, -5000, -1]) color("DarkKhaki") cube([10000, 10000, 1]);
translate([0, 0, legsHeight + 5]) torso();
translate([0, 0, 0]) legs();



