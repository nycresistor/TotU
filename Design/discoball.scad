screenX = 78.2;
screenY = 55.75;
screenZ = 3.6;

ballD = (screenX + 10) * 12 / 3.141592653;

module screen() {
	cube([screenX, screenY, screenZ + 2], center=true);
}

union() {
	#sphere(d=ballD, center=true);
	for (z = [0:11]) {
		rotate([90,0,30*z]) translate([0,0,ballD/2]) screen();
	}

	rotate([0,0,15])
	for (z = [0:9]) {
		rotate([69,0,36*z]) translate([0,0,ballD/2]) screen();
	}
/*
	for (z = [0:7]) {
		rotate([55,0,30*z]) translate([0,0,ballD/2]) screen();
	}

	for (z = [0:5]) {
		rotate([38,0,40*z]) translate([0,0,ballD/2]) screen();
	}

	for (z = [0:3]) {
		rotate([22,0,72*z]) translate([0,0,ballD/2]) screen();
	}
*/
}